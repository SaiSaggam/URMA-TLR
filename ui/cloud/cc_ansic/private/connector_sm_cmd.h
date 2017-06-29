/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
STATIC connector_status_t sm_copy_user_request(connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;
    connector_bool_t response_needed;

    ASSERT_GOTO(sm_ptr->pending.data != NULL, error);
    session->bytes_processed = 0;

    switch (sm_ptr->pending.request)
    {
        case connector_initiate_ping_request:
        {
            connector_sm_send_ping_request_t const * const request = sm_ptr->pending.data;

            session->user.context = request->user_context;
            session->user.header = NULL;
            session->command = connector_sm_cmd_ping;
            response_needed = request->response_required;
            session->sm_state = connector_sm_state_prepare_segment;
            session->timeout_in_seconds = request->timeout_in_seconds;
            break;
        }
#if (defined CONNECTOR_DATA_SERVICE)
        case connector_initiate_send_data:
        {
            connector_request_data_service_send_t const * const request = sm_ptr->pending.data;

            session->user.context = request->user_context;
            session->user.header = request->path;
            session->command = request->path != NULL ? connector_sm_cmd_data : connector_sm_cmd_no_path_data;
            response_needed = request->response_required;
            session->sm_state = connector_sm_state_get_total_length;
            #if (defined CONNECTOR_DATA_POINTS)
            if (request->path != NULL)
            {
                if (strncmp(request->path, internal_dp4d_path, internal_dp4d_path_strlen) == 0)
                {
                    char * const modifiable_path = (char *)request->path; /* Discarding "const" qualifier */

                    memcpy(modifiable_path, dp4d_path_prefix, dp4d_path_prefix_strlen);

                    SmSetDatapoint(session->flags);
                }
            }
            #endif
            session->timeout_in_seconds = request->timeout_in_seconds;
            break;
        }
#endif
        default:
            ASSERT_GOTO(connector_false, error);
            break;
    }

    if (response_needed) SmSetResponseNeeded(session->flags);

    result = connector_working;

error:
    return result;
}

STATIC void sm_verify_result(connector_sm_data_t * const sm_ptr, connector_status_t * const result)
{
    switch (*result)
    {
        case connector_pending:
        case connector_working:
        case connector_idle:
            goto done;

        case connector_invalid_data_size:
            connector_debug_line("WARNING: received a 'connector_invalid_data_size'");
            break;
        case connector_abort:
        case connector_invalid_response:
            sm_ptr->close.status = connector_close_status_abort;
            break;

        default:
            sm_ptr->close.status = connector_close_status_device_error;
            break;
    }

    *result = connector_working;
    switch(sm_ptr->transport.state)
    {
        case connector_transport_idle:
        case connector_transport_close:
        case connector_transport_terminate:
            break;

        default:
            sm_ptr->transport.state = connector_transport_close;
            break;
    }

done:
    return;
}

#if (defined CONNECTOR_COMPRESSION) || (defined CONNECTOR_SM_MULTIPART)
STATIC size_t sm_get_max_payload_bytes(connector_sm_data_t * const sm_ptr)
{
    size_t const sm_header_size = 5;

    /* This is used for Rx path... */
    size_t const max_payload_bytes = sm_ptr->transport.sm_mtu_rx - sm_header_size;

    return max_payload_bytes;
}
#endif

STATIC connector_status_t sm_allocate_user_buffer(connector_data_t * const connector_ptr, sm_data_block_t * const dblock)
{
    void * ptr = NULL;
    connector_status_t result = connector_working;

    if (dblock->bytes > 0)
    {
        ASSERT(dblock->data == NULL);
        result = malloc_data_buffer(connector_ptr, dblock->bytes, named_buffer_id(sm_data_block), &ptr);
    }

    dblock->data = ptr;

    return result;
}

STATIC connector_status_t sm_map_callback_status_to_connector_status(connector_callback_status_t const callback_status)
{
    connector_status_t result;

    switch (callback_status)
    {
        case connector_callback_continue:
            result = connector_working;
            break;

        case connector_callback_busy:
            result = connector_pending;
            break;

        case connector_callback_error:
            result = connector_device_error;
            break;

        default:
            result = connector_abort;
            break;
    }

    return result;
}

#if (defined CONNECTOR_DATA_SERVICE)
STATIC connector_callback_status_t sm_inform_data_complete(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_request_id_t request_id;
    connector_callback_status_t callback_status;
    connector_data_service_status_t status_info;

    status_info.transport = session->transport;
    status_info.user_context = session->user.context;
    status_info.session_error = connector_session_error_none;
    switch (session->error)
    {
        case connector_sm_error_cancel:
            status_info.status = connector_data_service_status_cancel;
            break;

        case connector_sm_error_timeout:
            status_info.status = connector_data_service_status_timeout;
            break;

        case connector_sm_error_complete:
            status_info.status = connector_data_service_status_complete;
            break;

        case connector_sm_error_no_resource:
            status_info.session_error = connector_session_error_memory;
            /* no break */
        default:
            status_info.status = connector_data_service_status_session_error;
            break;
    }

    #if (defined CONNECTOR_DATA_POINTS)
    if (SmIsDatapoint(session->flags))
    {
        callback_status = dp_handle_callback(connector_ptr, connector_request_id_data_service_send_status, &status_info);
    }
    else
    #endif
    {
        request_id.data_service_request = SmIsClientOwned(session->flags) ? connector_request_id_data_service_send_status : connector_request_id_data_service_receive_status;
        callback_status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, &status_info, connector_ptr->context);
    }

    return callback_status;
}
#endif

#if (defined CONNECTOR_SM_CLI)
STATIC connector_callback_status_t sm_inform_cli_complete(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_request_id_t request_id;
    connector_callback_status_t callback_status;
    connector_sm_cli_status_t cb_data;

    cb_data.transport = session->transport;
    cb_data.user_context = session->user.context;
    switch (session->error)
    {
        case connector_sm_error_complete:
            cb_data.status = connector_sm_cli_status_success;
            break;
        case connector_sm_error_cancel:
            cb_data.status = connector_sm_cli_status_cancel;
            break;
        case connector_sm_error_timeout:
        case connector_sm_error_no_resource:
        case connector_sm_error_none:
        case connector_sm_error_in_request:
        case connector_sm_error_unavailable:
        case connector_sm_error_unknown:
            cb_data.status = connector_sm_cli_status_error;
            break;
    }

    request_id.sm_request = connector_request_id_sm_cli_status;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cb_data, connector_ptr->context);
    if (callback_status == connector_callback_unrecognized)
        callback_status = connector_callback_error;

    return callback_status;
}
#endif

STATIC connector_callback_status_t sm_inform_ping_complete(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_callback_status_t callback_status = connector_callback_continue;

    if (SmIsClientOwned(session->flags))
    {
        connector_request_id_t request_id;
        connector_sm_ping_response_t cb_data;

        cb_data.transport = session->transport;
        cb_data.user_context = session->user.context;
        switch (session->error)
        {
            case connector_sm_error_none:
                cb_data.status = connector_sm_ping_status_success;
                break;

            case connector_sm_error_cancel:
                cb_data.status = connector_sm_ping_status_cancel;
                break;

            case connector_sm_error_timeout:
                cb_data.status = connector_sm_ping_status_timeout;
                break;

            case connector_sm_error_complete:
                cb_data.status = connector_sm_ping_status_complete;
                break;

            default:
                cb_data.status = connector_sm_ping_status_error;
                break;
        }

        request_id.sm_request = connector_request_id_sm_ping_response;
        callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cb_data, connector_ptr->context);
        if (callback_status == connector_callback_unrecognized)
            callback_status = connector_callback_continue;
    }

    return callback_status;
}

STATIC connector_status_t sm_inform_session_complete(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t result;
    connector_callback_status_t callback_status = connector_callback_continue;

    if (session->sm_state == connector_sm_state_complete) goto done;

    switch (session->command)
    {
        case connector_sm_cmd_data:
        case connector_sm_cmd_no_path_data:
#if (defined CONNECTOR_DATA_SERVICE)
            callback_status = sm_inform_data_complete(connector_ptr, session);
#endif
            break;

        #if (defined CONNECTOR_SM_CLI)
        case connector_sm_cmd_cli:
            callback_status = sm_inform_cli_complete(connector_ptr, session);
            break;
        #endif

        case connector_sm_cmd_ping:
            callback_status = sm_inform_ping_complete(connector_ptr, session);
            break;

        default:
            connector_debug_line("sm_inform_session_complete: cancelling the session cmd [%d]", session->command);
            callback_status = connector_callback_continue;
            break;
    }

done:
    result = sm_map_callback_status_to_connector_status(callback_status);

    if (session->in.data != NULL)
    {
        if (free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), session->in.data) != connector_working)
            result = connector_abort;

        session->in.bytes = 0;
        session->in.data = NULL;
    }

    return result;
}

STATIC connector_status_t sm_switch_path(connector_data_t * const connector_ptr, connector_sm_session_t * const session, connector_sm_state_t const next_state)
{
    connector_status_t result = connector_working;

    if (session->in.data != NULL)
    {
        result = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), session->in.data);
        if (result != connector_working) goto error;

        session->in.bytes = 0;
        session->in.data = NULL;
    }

    if (SmIsResponseNeeded(session->flags))
    {
        session->sm_state = next_state;
        SmClearCompressed(session->flags);
        SmClearMultiPart(session->flags);
        SmSetResponse(session->flags);
        session->segments.processed = 0;
        switch(session->command)
        {
            case connector_sm_cmd_ping:
            case connector_sm_cmd_connect:
            case connector_sm_cmd_reboot:
            case connector_sm_cmd_pack:
            case connector_sm_cmd_pad:
            case connector_sm_cmd_config:
            case connector_sm_cmd_opaque_response:
                break;
            case connector_sm_cmd_data:
            case connector_sm_cmd_no_path_data:
            case connector_sm_cmd_cli:
                SmClearResponseNeeded(session->flags); /* After the response is sent, inform the user that the session is completed */
                break;
        }
    }
    else
    {
        if (!SmIsError(session->flags)) /* If it is an error, it has already been called by sm_handle_error() */
        {
            connector_bool_t inform_user = connector_bool(SmIsClientOwned(session->flags));

            if (!inform_user)
            {
                switch(session->command)
                {
                    case connector_sm_cmd_ping:
                    case connector_sm_cmd_connect:
                    case connector_sm_cmd_reboot:
                    case connector_sm_cmd_pack:
                    case connector_sm_cmd_pad:
                    case connector_sm_cmd_config:
                    case connector_sm_cmd_opaque_response:
                        break;
                    case connector_sm_cmd_data:
                    case connector_sm_cmd_no_path_data:
                    case connector_sm_cmd_cli:
                        inform_user = connector_true;
                        break;
                }
            }

            if (inform_user)
            {
                session->error = connector_sm_error_complete;
                result = sm_inform_session_complete(connector_ptr, session);
                if (result != connector_working)
                {
                    goto error;
                }
            }
        }

        session->sm_state = connector_sm_state_complete;
    }

error:
    return result;
}

STATIC void sm_set_payload_process(connector_sm_session_t * const session)
{
    size_t const zlib_header_bytes = 2;

    session->in.bytes = session->bytes_processed;
    session->sm_state = connector_sm_state_prepare_segment;
    session->bytes_processed = SmIsCompressed(session->flags) ? zlib_header_bytes : 0;
}

STATIC void sm_set_payload_complete(connector_sm_session_t * const session)
{
    ASSERT(session->bytes_processed <= session->in.bytes);

    #if (defined CONNECTOR_COMPRESSION)
    session->sm_state = connector_sm_state_compress;
    #else
    sm_set_payload_process(session);
    #endif
}

STATIC void sm_set_header_complete(connector_sm_session_t * const session)
{
    if (session->bytes_processed < session->in.bytes)
        session->sm_state = connector_sm_state_more_data;
    else
        sm_set_payload_complete(session);
}

STATIC connector_status_t sm_prepare_data_request(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;
    size_t const path_length_field_bytes = 1;
    char const * path = NULL;
    size_t path_len = 0;

    if (session->command == connector_sm_cmd_data)
    {
        size_t const five_bits_max_len = (1 << 5);

        path = session->user.header;
        ASSERT_GOTO(path != NULL, error);
        path_len = strlen(path);
        ASSERT_GOTO(path_len < five_bits_max_len, error);
        session->in.bytes += (path_length_field_bytes + path_len);
    }

    session->bytes_processed = 0;
    result = sm_allocate_user_buffer(connector_ptr, &session->in);
    if (result != connector_working)
    {
        session->error = connector_sm_error_no_resource;
        goto error;
    }

    if (path != NULL)
    {
        uint8_t * header = session->in.data;

        *header++ = path_len;
        memcpy(header, path, path_len);
        session->bytes_processed = path_length_field_bytes + path_len;
    }

    sm_set_header_complete(session);

error:
    return result;
}

STATIC connector_status_t sm_prepare_data_response(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t const result = sm_allocate_user_buffer(connector_ptr, &session->in);

    if (result == connector_working)
    {
        session->bytes_processed = 0;
        sm_set_header_complete(session);
    }
    else
        session->error = connector_sm_error_no_resource;

    return result;
}

#if (defined CONNECTOR_SM_CLI)
STATIC connector_status_t sm_process_cli_request(connector_data_t * const connector_ptr, connector_sm_session_t * const session, void * const payload, size_t const bytes)
{
    connector_status_t result;
    char * const cli_command = payload;
    size_t cli_bytes;
    connector_sm_data_t * sm_ptr = NULL;

    if (SmIsLastData(session->flags))
    {
        cli_bytes = strlen(cli_command) + sizeof "";
    }
    else
    {
        cli_bytes = bytes;
    }

    switch (session->transport)
    {
        #if (defined CONNECTOR_TRANSPORT_UDP)
        case connector_transport_udp:
            sm_ptr = &connector_ptr->sm_udp;
            break;
        #endif

        #if (defined CONNECTOR_TRANSPORT_SMS)
        case connector_transport_sms:
            sm_ptr = &connector_ptr->sm_sms;
            break;
        #endif

        default:
            ASSERT(connector_false);
            break;
    }

    if (bytes > cli_bytes)
    {
        size_t const max_response_packets = LoadBE16(cli_command + cli_bytes);
        size_t max_response_bytes = 0;

        size_t const max_payload = sm_ptr->transport.sm_mtu_tx - record_end(segment);

        if (max_response_packets == 0)
            max_response_bytes = 0;
        else if (max_response_packets == 1)
            max_response_bytes = max_payload;
        else
        {
            size_t const segment0_overhead_bytes = record_end(segment0) - record_end(segmentn);
            max_response_bytes = max_response_packets * max_payload - segment0_overhead_bytes;
        }

        session->user.header = (void *)max_response_bytes;
    }
    else
    {
        session->user.header = (void *)SIZE_MAX;
    }

    {
        connector_request_id_t request_id;
        connector_sm_cli_request_t cli_request;
        connector_callback_status_t callback_status;

        cli_request.transport = session->transport;
        cli_request.user_context = session->user.context;
        cli_request.bytes_used = cli_bytes;
        cli_request.buffer = cli_request.bytes_used != 0 ? cli_command : NULL;
        cli_request.response_required = SmIsResponseNeeded(session->flags);
        cli_request.more_data = SmIsNotLastData(session->flags);

        request_id.sm_request = connector_request_id_sm_cli_request;
        callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cli_request, connector_ptr->context);
        if (callback_status == connector_callback_unrecognized)
            callback_status = connector_callback_error;
        result = sm_map_callback_status_to_connector_status(callback_status);
        if (callback_status == connector_callback_continue)
            session->user.context = cli_request.user_context;
    }

    return result;
}

STATIC connector_status_t sm_prepare_cli_response(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t result;
    connector_sm_cli_response_t cli_response;
    size_t const allowed_bytes = (size_t)session->user.header;

    if (session->in.data == NULL)
    {
        if (session->in.bytes > allowed_bytes)
            session->in.bytes = allowed_bytes;

        session->bytes_processed = 0;
        result = sm_allocate_user_buffer(connector_ptr, &session->in);
        if (result != connector_working)
            goto error;
    }

    cli_response.transport = session->transport;
    cli_response.user_context = session->user.context;
    ASSERT(session->in.bytes >= session->bytes_processed);
    cli_response.bytes_available = (session->in.bytes - session->bytes_processed);
    cli_response.buffer = ((char *)session->in.data) + session->bytes_processed;
    cli_response.bytes_used = 0;
    cli_response.more_data = connector_false;

    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.sm_request = connector_request_id_sm_cli_response;
        status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cli_response, connector_ptr->context);
        if (status == connector_callback_unrecognized)
            status = connector_callback_error;
        ASSERT(cli_response.bytes_available >= cli_response.bytes_used);
        result = sm_map_callback_status_to_connector_status(status);
    }

    switch (result)
    {
        case connector_working:
            session->bytes_processed += cli_response.bytes_used;
            if ((!cli_response.more_data) || (session->bytes_processed >= session->in.bytes))
                sm_set_payload_complete(session);
            break;

        case connector_device_error:
            session->error = connector_sm_error_cancel;
            result = connector_working;
            break;

        default:
            break;
    }

error:
    return result;
}
#endif

#if (defined CONNECTOR_TRANSPORT_SMS)
STATIC connector_status_t sm_process_config_request(connector_data_t * const connector_ptr, connector_sm_session_t * const session, void * const payload, size_t const bytes)
{
    connector_status_t result = connector_success;
    connector_sm_receive_config_request_t config_request;

    ASSERT(SmIsLastData(session->flags));
    config_request.phone_number = payload;

    {
        size_t const phone_bytes = strlen(config_request.phone_number) + 1;
        config_request.service_id = (phone_bytes < bytes) ? config_request.phone_number + phone_bytes : NULL;
    }

    /* Callback sms transport close/open so new phone makes effect */
    {
        connector_sm_data_t * const sm_ptr = &connector_ptr->sm_sms;    /* Assume it's SMS transport */
        ASSERT(sm_ptr->network.class_id == connector_class_id_network_sms);

        if (sm_ptr->network.handle != CONNECTOR_NETWORK_HANDLE_NOT_INITIALIZED && SmIsSmsConfigNotInit(session->flags))
        {
            connector_callback_status_t callback_status;
            connector_request_id_t request_id;
            connector_network_close_t close_data;

            /* Close */
            close_data.handle = sm_ptr->network.handle;
            close_data.status = connector_close_status_device_stopped;

            request_id.network_request = connector_request_id_network_close;
            callback_status = connector_callback(connector_ptr->callback, sm_ptr->network.class_id, request_id, &close_data, connector_ptr->context);
            ASSERT(callback_status != connector_callback_unrecognized);
            switch (callback_status)
            {
                case connector_callback_busy:
                    result = connector_pending;
                    sm_ptr->transport.state = connector_transport_receive; /* Keep on receive state to complete reconfiguration operation */
                    break;

                case connector_callback_continue:
                {
                    unsigned long int uptime;

                    SmSetSmsConfigInit(session->flags);
                    sm_ptr->network.handle = CONNECTOR_NETWORK_HANDLE_NOT_INITIALIZED;

                    if (get_system_time(connector_ptr, &uptime) == connector_working)
                    {
                        sm_ptr->transport.connect_at = uptime;
                        sm_ptr->transport.state = connector_transport_wait_for_reconnect;
                        result = connector_pending;
                    }
                    else
                    {
                        result = connector_abort;
                    }
                    break;
                }

                default:
                    sm_ptr->close.status = connector_close_status_abort;
                    break;
            }
            goto done;
        }
    }

    /* Callback to config.c so user can save the new phone to persistent storage */
#if !(defined CONNECTOR_CLOUD_PHONE)
    result = set_config_device_cloud_phone(connector_ptr, config_request.phone_number);
    if (result != connector_working)
    {
        goto done;
    }
#endif

    /* Callback to user */
    {
        connector_request_id_t request_id;
        connector_callback_status_t callback_status;
        config_request.transport = session->transport;
        config_request.response_required = SmIsResponseNeeded(session->flags);

        request_id.sm_request = connector_request_id_sm_config_request;
        callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &config_request, connector_ptr->context);
        result = sm_map_callback_status_to_connector_status(callback_status);
    }
done:
    return result;
}
#endif

#if (defined CONNECTOR_DATA_SERVICE)
STATIC connector_status_t sm_pass_target_info(connector_data_t * const connector_ptr, connector_sm_session_t * const session, uint8_t * const target_ptr, size_t target_bytes)
{
    #define SM_TARGET_MAX_LENGTH    32
    connector_status_t result;
    connector_callback_status_t callback_status;
    connector_request_id_t request_id;
    connector_data_service_receive_target_t cb_data;
    char target_name[SM_TARGET_MAX_LENGTH];

    cb_data.transport = session->transport;
    cb_data.user_context = session->user.context;
    if (target_bytes > 0)
        memcpy(target_name, target_ptr, target_bytes);
    target_name[target_bytes] = '\0';
    cb_data.target = target_name;
    cb_data.response_required = SmIsResponseNeeded(session->flags);

    request_id.data_service_request = connector_request_id_data_service_receive_target;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, &cb_data, connector_ptr->context);
    if (callback_status == connector_callback_unrecognized)
        callback_status = connector_callback_error;
    result = sm_map_callback_status_to_connector_status(callback_status);
    session->user.context = cb_data.user_context;

    return result;
}

STATIC connector_status_t sm_process_data_request(connector_data_t * const connector_ptr, connector_sm_session_t * const session, void * const payload, size_t const bytes)
{
    uint8_t * data_ptr = payload;
    connector_status_t status = connector_working;

    if (session->error == connector_sm_error_complete)
        goto done;

    if (session->bytes_processed == 0)
    {
        size_t target_length =  0;

        if (session->command == connector_sm_cmd_data)
            target_length = 0x1F & *data_ptr;

        status = sm_pass_target_info(connector_ptr, session, data_ptr + 1, target_length);
        if (status != connector_working)
            goto error;

        /* Increase target_length:
         * It's required for connector_sm_cmd_data to point correctly to actual data,
         * and will be used for connector_sm_cmd_no_path_data to signal that sm_pass_target_info
         * has already been called */
        target_length++;

        /* Set pre-processed bytes for segment 0 */
        session->bytes_processed = target_length;

        /* Return pending so bytes_processed and session->segments.processed are not incremented */
        status = connector_pending;
        SmSetTargetInPayload(session->flags);
        goto done;
    }

    {
        connector_callback_status_t callback_status;
        connector_request_id_t request_id;
        connector_data_service_receive_data_t cb_data;
        size_t bytes_pre_processed =  0;

        /* Compute pre-processed bytes for segment 0 */
        if (SmIsTargetInPayload(session->flags) && (session->command == connector_sm_cmd_data))
            bytes_pre_processed = session->bytes_processed;

        data_ptr += bytes_pre_processed;

        cb_data.transport = session->transport;
        cb_data.user_context = session->user.context;
        cb_data.buffer = data_ptr;
        cb_data.bytes_used = bytes - bytes_pre_processed;
        cb_data.more_data = SmIsNotLastData(session->flags);
        request_id.data_service_request = connector_request_id_data_service_receive_data;
        callback_status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, &cb_data, connector_ptr->context);
        if (callback_status == connector_callback_unrecognized)
            callback_status = connector_callback_error;
        if (callback_status != connector_callback_busy)
            SmClearTargetInPayload(session->flags);
        status = sm_map_callback_status_to_connector_status(callback_status);
        session->user.context = cb_data.user_context;
    }

error:
    switch (status)
    {
        case connector_device_error:
            session->error = connector_sm_error_complete;
            session->bytes_processed = bytes;
            status = connector_working;
            break;

        case connector_working:
            /* Restore pre-processed bytes for segment 0 */
            if (session->segments.processed == 0)
                session->bytes_processed = 0;
            break;

        default:
            break;
    }

done:
    return status;
}

STATIC connector_status_t sm_process_data_response(connector_data_t * const connector_ptr, connector_sm_session_t * const session, void * const payload, size_t const bytes)
{
    connector_status_t status;
    connector_callback_status_t callback_status;
    connector_data_service_send_response_t cb_data;
    char * const text = payload;

    cb_data.transport = session->transport;
    cb_data.user_context = session->user.context;
    if (SmIsError(session->flags))
    {
        switch (session->error)
        {
            case connector_sm_error_in_request:
                cb_data.response = connector_data_service_send_response_bad_request;
                break;

            case connector_sm_error_unavailable:
                cb_data.response = connector_data_service_send_response_unavailable;
                break;

            default:
                cb_data.response = connector_data_service_send_response_cloud_error;
                break;
        }
    }
    else
        cb_data.response = connector_data_service_send_response_success;

    cb_data.hint = (bytes > 0) ? text : NULL;

    #if (defined CONNECTOR_DATA_POINTS)
    if (SmIsDatapoint(session->flags))
    {
        callback_status = dp_handle_callback(connector_ptr, connector_request_id_data_service_send_response, &cb_data);
    }
    else
    #endif
    {
        connector_request_id_t request_id;

        request_id.data_service_request = connector_request_id_data_service_send_response;
        callback_status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, &cb_data, connector_ptr->context);
    }

    status = sm_map_callback_status_to_connector_status(callback_status);
    if (SmIsError(session->flags) && (status != connector_pending))
    {
        session->error = connector_sm_error_none;
        SmClearError(session->flags);
    }

    return status;
}
#endif

STATIC connector_status_t sm_process_reboot(connector_data_t * const connector_ptr)
{
    connector_status_t result;
    connector_request_id_t request_id;
    connector_callback_status_t callback_status;

    request_id.os_request = connector_request_id_os_reboot;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_operating_system, request_id, NULL, connector_ptr->context);
    ASSERT(callback_status != connector_callback_unrecognized);

    result = sm_map_callback_status_to_connector_status(callback_status);

    return result;
}

STATIC connector_status_t sm_process_ping_response(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t status;
    connector_callback_status_t const callback_status = sm_inform_ping_complete(connector_ptr, session);

    status = sm_map_callback_status_to_connector_status(callback_status);
    if (SmIsError(session->flags) && (status != connector_pending))
    {
        session->error = connector_sm_error_none;
        SmClearError(session->flags);
    }
    return status;
}

STATIC connector_status_t sm_process_ping_request(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t status;
    connector_request_id_t request_id;
    connector_sm_receive_ping_request_t cb_data;
    connector_callback_status_t callback_status;

    cb_data.transport = session->transport;
    cb_data.response_required = SmIsResponseNeeded(session->flags);

    request_id.sm_request = connector_request_id_sm_ping_request;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cb_data, connector_ptr->context);
    if (callback_status == connector_callback_unrecognized)
        callback_status = connector_callback_continue;
    status = sm_map_callback_status_to_connector_status(callback_status);

    return status;
}

STATIC connector_status_t sm_process_opaque_response(connector_data_t * const connector_ptr, connector_sm_session_t * const session, void * payload, size_t const bytes)
{
    connector_status_t status;
    connector_request_id_t request_id;
    connector_sm_opaque_response_t cb_data;
    connector_callback_status_t callback_status;

    cb_data.transport = session->transport;
    cb_data.id = session->request_id;
    cb_data.data = payload;
    cb_data.bytes_used = bytes;
    cb_data.error = SmIsError(session->flags);

    request_id.sm_request = connector_request_id_sm_opaque_response;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cb_data, connector_ptr->context);
    status = sm_map_callback_status_to_connector_status(callback_status);

    return status;
}

STATIC connector_status_t sm_prepare_payload(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;
    connector_status_t (* prepare_fn) (connector_data_t * const connector_ptr, connector_sm_session_t * const session) = NULL;

    ASSERT_GOTO(session != NULL, error);
    switch (session->command)
    {
        case connector_sm_cmd_data:
        case connector_sm_cmd_no_path_data:
            prepare_fn = SmIsClientOwned(session->flags) ? sm_prepare_data_request : sm_prepare_data_response;
            break;

        #if (defined CONNECTOR_SM_CLI)
        case connector_sm_cmd_cli:
            ASSERT_GOTO(SmIsCloudOwned(session->flags), error);
            prepare_fn = sm_prepare_cli_response;
            break;
        #endif

        default:
            result = connector_unavailable;
            goto unexpected;
    }

    result = prepare_fn(connector_ptr, session);

unexpected:
    if ((result == connector_working) && (session->error != connector_sm_error_none) && (session->error != connector_sm_error_complete))
    {
        session->sm_state = connector_sm_state_error;
        SmSetError(session->flags);
    }

error:
    return result;
}

STATIC connector_status_t sm_pass_user_data(connector_data_t * const connector_ptr, connector_sm_session_t * const session, uint8_t * payload, size_t const bytes)
{
    connector_status_t result = connector_abort;
    connector_sm_state_t next_state = connector_sm_state_send_data;

    switch (session->command)
    {
        case connector_sm_cmd_data:
        case connector_sm_cmd_no_path_data:
            if (SmIsCloudOwned(session->flags))
            {
#if (defined CONNECTOR_DATA_SERVICE)
                result = sm_process_data_request(connector_ptr, session, payload, bytes);
#endif
                next_state = connector_sm_state_get_total_length;
            }
            else
            {
#if (defined CONNECTOR_DATA_SERVICE)
                result = sm_process_data_response(connector_ptr, session, payload, bytes);
#endif
                next_state = connector_sm_state_complete;
            }
            break;

        #if (defined CONNECTOR_SM_CLI)
        case connector_sm_cmd_cli:
            result = sm_process_cli_request(connector_ptr, session, payload, bytes);
            next_state = connector_sm_state_get_total_length;
            break;
        #endif

        #if (defined CONNECTOR_TRANSPORT_SMS)
        case connector_sm_cmd_config:
            if (SmIsCloudOwned(session->flags))
            {
                result = sm_process_config_request(connector_ptr, session, payload, bytes);
            }
            break;
        #endif

        case connector_sm_cmd_connect:
            #if (defined CONNECTOR_TRANSPORT_TCP)
            if (edp_get_active_state(connector_ptr) == connector_transport_idle)
            {
                connector_request_id_t request_id;
                connector_callback_status_t callback_status;

                connector_sm_request_connect_t request_connect;
                request_connect.transport = session->transport;
                request_connect.allow = connector_true;

                request_id.sm_request = connector_request_id_sm_request_connect;
                callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &request_connect, connector_ptr->context);
                ASSERT(callback_status == connector_callback_continue);
                if (callback_status != connector_callback_continue)
                {
                    connector_debug_line("WARNING: connector_request_id_sm_request_connect returned invalid value, starting TCP");
                }

                if (request_connect.allow)
                {
                    edp_set_active_state(connector_ptr, connector_transport_open);
                }
            }
            result = connector_working;
            #else
            connector_debug_line("WARNING: received a 'request connect' but TCP transport is not available");
            result = connector_device_error;
            #endif
            break;

        case connector_sm_cmd_ping:
            if (SmIsCloudOwned(session->flags))
            {
                result = sm_process_ping_request(connector_ptr, session);
            }
            else
            {
                result = sm_process_ping_response(connector_ptr, session);
                next_state = connector_sm_state_complete;
            }
            break;

        case connector_sm_cmd_reboot:
            SmSetReboot(session->flags);
            result = connector_working;
            break;

        case connector_sm_cmd_opaque_response:
            result = sm_process_opaque_response(connector_ptr, session, payload, bytes);
            break;

        default:
            result = connector_unavailable;
            break;
    }

    switch (result)
    {
        case connector_working:
            session->bytes_processed += bytes;
            switch (session->error)
            {
                case connector_sm_error_none:
                case connector_sm_error_complete:
                    if (SmIsLastData(session->flags))
                        sm_switch_path(connector_ptr, session, next_state);
                    SmClearSmsConfigInit(session->flags);
                    break;

                default:
                    session->sm_state = connector_sm_state_error;
                    SmSetError(session->flags);
                    break;
            }
            break;

        case connector_pending:
            break;

        case connector_device_error:
            session->error = connector_sm_error_cancel;
            session->sm_state = connector_sm_state_error;
            SmSetError(session->flags);
            result = connector_working;
            break;

        default:
            session->sm_state = connector_sm_state_error;
            SmSetError(session->flags);
            break;
    }

    return result;
}

STATIC connector_status_t sm_process_payload(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;
    uint8_t * data_ptr;
    size_t bytes;

#if (defined CONNECTOR_SM_MULTIPART)
    if (SmIsMultiPart(session->flags))
    {
        size_t const max_payload_bytes = sm_get_max_payload_bytes(sm_ptr);
        size_t const data_index = session->segments.processed * max_payload_bytes;

        ASSERT_GOTO(session->in.data != NULL, error);
        ASSERT_GOTO(session->segments.processed < session->segments.count, error);
        if (session->segments.processed == (session->segments.count - 1))
            SmSetLastData(session->flags);

        data_ptr = &session->in.data[data_index];
        bytes = session->segments.size_array[session->segments.processed];
    }
    else
#endif
    {
        UNUSED_PARAMETER(sm_ptr);
        ASSERT_GOTO(SmIsNotMultiPart(session->flags), error);
        SmSetLastData(session->flags);
        data_ptr = session->in.data;
        bytes = session->in.bytes;
    }

    result = sm_pass_user_data(connector_ptr, session, data_ptr, bytes);
    if ((result == connector_working) && (SmIsNotLastData(session->flags)))
        session->segments.processed++;

error:
    return result;
}
