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
STATIC connector_status_t sm_get_user_data_length(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result;
    connector_callback_status_t status = connector_callback_continue;

#if !(defined CONNECTOR_DATA_SERVICE) && !(defined CONNECTOR_SM_CLI)
    (void)(connector_ptr); /* Unused argument */
#endif

    switch (session->command)
    {
        case connector_sm_cmd_data:
        case connector_sm_cmd_no_path_data:
        {
#if (defined CONNECTOR_DATA_SERVICE)
            connector_data_service_length_t cb_data;

            cb_data.transport = session->transport;
            cb_data.user_context = session->user.context;
            cb_data.total_bytes = 0;

            #if (defined CONNECTOR_DATA_POINTS)
            if (SmIsDatapoint(session->flags))
            {
                status = dp_handle_callback(connector_ptr, connector_request_id_data_service_send_length, &cb_data);
            }
            else
            #endif
            {
                connector_request_id_t request_id;

                request_id.data_service_request = SmIsClientOwned(session->flags) ? connector_request_id_data_service_send_length : connector_request_id_data_service_receive_reply_length;
                status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, &cb_data, connector_ptr->context);
                if (status == connector_callback_unrecognized)
                    status = connector_callback_continue;
            }

            session->in.bytes = cb_data.total_bytes;
#endif
            break;
        }

        #if (defined CONNECTOR_SM_CLI)
        case connector_sm_cmd_cli:
        {
            connector_sm_cli_response_length_t cb_data;
            connector_request_id_t request_id;

            cb_data.transport = session->transport;
            cb_data.user_context = session->user.context;
            cb_data.total_bytes = 0;

            request_id.sm_request = connector_request_id_sm_cli_response_length;
            status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cb_data, connector_ptr->context);
            if (status == connector_callback_unrecognized)
                status = connector_callback_continue;
            session->in.bytes = cb_data.total_bytes;
            break;
        }
        #endif

        default:
            result = connector_abort;
            ASSERT_GOTO(connector_false, error);
            break;
    }

    result = sm_map_callback_status_to_connector_status(status);
    if (status == connector_callback_continue)
    {
        ASSERT_GOTO(session->in.bytes < (sm_ptr->transport.sm_mtu_tx * UCHAR_MAX), error);
        session->sm_state = connector_sm_state_prepare_payload;
    }

error:
    return result;
}

#if (defined CONNECTOR_DATA_SERVICE)
STATIC connector_status_t sm_get_more_request_data(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;
    uint8_t * dptr = session->in.data;
    connector_data_service_send_data_t cb_data;

    ASSERT_GOTO(session->in.bytes > session->bytes_processed, error);

    cb_data.transport = session->transport;
    cb_data.user_context = session->user.context;
    cb_data.buffer = &dptr[session->bytes_processed];
    cb_data.bytes_available = session->in.bytes - session->bytes_processed;
    cb_data.bytes_used = 0;
    cb_data.more_data = connector_false;

    {
        connector_callback_status_t status;

#if (defined CONNECTOR_DATA_POINTS)
        if (SmIsDatapoint(session->flags))
        {
            status = dp_handle_callback(connector_ptr, connector_request_id_data_service_send_data, &cb_data);
        }
        else
#endif
        {
            connector_request_id_t request_id;

            request_id.data_service_request = connector_request_id_data_service_send_data;
            status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, &cb_data, connector_ptr->context);
        }

        result = sm_map_callback_status_to_connector_status(status);
    }

    session->user.context = cb_data.user_context;
    if (result == connector_working)
    {
        session->bytes_processed += cb_data.bytes_used;
        ASSERT(session->bytes_processed <= session->in.bytes);
        if (!cb_data.more_data)
            sm_set_payload_complete(session);
    }

error:
    return result;
}

STATIC connector_status_t sm_get_more_response_data(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;
    uint8_t * dptr = session->in.data;
    connector_data_service_receive_reply_data_t cb_data;

    ASSERT_GOTO(session->in.bytes > session->bytes_processed, error);

    cb_data.transport = session->transport;
    cb_data.user_context = session->user.context;
    cb_data.buffer = &dptr[session->bytes_processed];
    cb_data.bytes_available = session->in.bytes - session->bytes_processed;
    cb_data.bytes_used = 0;
    cb_data.more_data = connector_false;

    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.data_service_request = connector_request_id_data_service_receive_reply_data;
        status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, &cb_data, connector_ptr->context);
        result = sm_map_callback_status_to_connector_status(status);
    }

    session->user.context = cb_data.user_context;
    if (result == connector_working)
    {
        session->bytes_processed += cb_data.bytes_used;
        ASSERT(session->bytes_processed <= session->in.bytes);
        if (!cb_data.more_data)
        {
            sm_set_payload_complete(session);
        }
        else
        {
            ASSERT(cb_data.bytes_used < cb_data.bytes_available);
        }
    }

error:
    return result;
}
#endif

#if (defined CONNECTOR_COMPRESSION)
STATIC connector_status_t sm_compress_data(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t status;
    size_t const excluded_header_adler32_footer_bytes = 6;

    session->compress.out.data = NULL;
    session->compress.out.bytes = session->bytes_processed + excluded_header_adler32_footer_bytes;
    status = sm_allocate_user_buffer(connector_ptr, &session->compress.out);
    ASSERT_GOTO(status == connector_working, error);

    {
        z_streamp const zlib_ptr = &session->compress.zlib;
        int zret;

        memset(zlib_ptr, 0, sizeof *zlib_ptr);
        zret = deflateInit(zlib_ptr, Z_DEFAULT_COMPRESSION);
        ASSERT_GOTO(zret == Z_OK, error);

        zlib_ptr->next_in = session->in.data;
        zlib_ptr->avail_in = session->bytes_processed;
        zlib_ptr->next_out = session->compress.out.data;
        zlib_ptr->avail_out = session->compress.out.bytes;
        zret = deflate(zlib_ptr, Z_FINISH);
        switch(zret)
        {
            case Z_STREAM_END:
            {
                size_t const compressed_bytes = session->compress.out.bytes - (zlib_ptr->avail_out + excluded_header_adler32_footer_bytes);

                if (compressed_bytes < session->bytes_processed)
                {
                    uint8_t * data_ptr = session->compress.out.data;

                    SmSetCompressed(session->flags);
                    status = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), session->in.data);
                    if (status != connector_working) goto error;
                    session->in.data = data_ptr;
                    session->bytes_processed = compressed_bytes;
                    sm_set_payload_process(session);
                    break;
                }
            }
            /* no break */

            case Z_OK:
                status = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), session->compress.out.data);
                if (status != connector_working) goto error;
                sm_set_payload_process(session);
                break;

            default:
                status = connector_abort;
                ASSERT_GOTO(connector_false, error);
                break;
        }

        zret = deflateEnd(zlib_ptr);
        ASSERT_GOTO(zret == Z_OK, error);
    }

error:
   return status;
}
#endif

STATIC connector_status_t sm_prepare_segment(connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;
    size_t const max_payload = sm_ptr->transport.sm_mtu_tx - record_end(segment);

    session->segments.processed = 0;
    session->segments.size_array = NULL;
    if (session->in.bytes <= max_payload)
        session->segments.count = 1;
    #if (defined CONNECTOR_SM_MULTIPART)
    else
    {
        size_t const segment0_overhead_bytes = record_end(segment0) - record_end(segmentn);
        size_t const segment_count = (session->in.bytes + ((max_payload + segment0_overhead_bytes) - 1))/max_payload;

        ASSERT_GOTO(segment_count < 256, error);
        session->segments.count = segment_count;
        SmSetMultiPart(session->flags);
    }
    #else
    else
    {
        connector_debug_line("sm_prepare_segment: Multipart is disabled. Please define CONNECTOR_SM_MULTIPART in connector_config.h.");
        ASSERT(connector_false);
        result = connector_invalid_data_size;
        goto error;
    }
    #endif

    session->sm_state = connector_sm_state_send_data;
    result = connector_working;

error:
    return result;
}

STATIC connector_status_t sm_send_segment(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr)
{
    connector_status_t result = connector_no_resource;
    connector_sm_packet_t * const send_packet = &sm_ptr->network.send_packet;
    connector_callback_status_t status;
    connector_network_send_t send_data;
    connector_request_id_t request_id;

    send_data.buffer = &send_packet->data[send_packet->processed_bytes];
    send_data.bytes_available = send_packet->total_bytes - send_packet->processed_bytes;
    send_data.handle = sm_ptr->network.handle;
    send_data.bytes_used = 0;

    request_id.network_request = connector_request_id_network_send;
    status = connector_callback(connector_ptr->callback, sm_ptr->network.class_id, request_id, &send_data, connector_ptr->context);
    ASSERT_GOTO(status != connector_callback_unrecognized, error);
    result = sm_map_callback_status_to_connector_status(status);
    if (status != connector_callback_continue) goto error;

    send_packet->processed_bytes += send_data.bytes_used;
    if (send_packet->processed_bytes >= send_packet->total_bytes)
    {
        connector_sm_session_t * const session = send_packet->pending_session;

        ASSERT_GOTO(session != NULL, error);
        session->segments.processed++;
        if (session->segments.count == session->segments.processed)
        {
            if (session->in.bytes != 0)
            {
                connector_debug_line("ERROR: sm_send_segment: All segments processed but still remaining bytes");
            }
            result = sm_switch_path(connector_ptr, session, SmIsResponse(session->flags) ? connector_sm_state_complete : connector_sm_state_receive_data);
            if (result != connector_working) goto error;
        }

        send_packet->total_bytes = 0;
        send_packet->processed_bytes = 0;
        send_packet->pending_session = NULL;
    }

error:
    return result;
}

#if (defined CONNECTOR_TRANSPORT_SMS)
STATIC connector_status_t sm_encode_segment(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    size_t const data_size = 1+(sm_ptr->network.send_packet.total_bytes * 5)/4;
    void * data_ptr = NULL;
    connector_status_t result = malloc_data_buffer(connector_ptr, data_size, named_buffer_id(sm_data_block), &data_ptr);

    if (result == connector_working)
    {
        connector_sm_packet_t * const send_ptr = &sm_ptr->network.send_packet;

        send_ptr->total_bytes = sm_encode85(data_ptr, data_size, send_ptr->data, send_ptr->total_bytes);
        memcpy(send_ptr->data, data_ptr, send_ptr->total_bytes);
        result = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), data_ptr);
        if (result != connector_working) goto error;
        session->sm_state = connector_sm_state_send_data;
    }

error:
    return result;
}
#endif

STATIC connector_status_t sm_send_data(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_working;
    connector_sm_packet_t * const send_ptr = &sm_ptr->network.send_packet;
    uint8_t * data_ptr = send_ptr->data;
    uint8_t * sm_header;

    if (send_ptr->total_bytes > 0)
    {
        goto send;
    }

    switch (sm_ptr->network.transport)
    {
        #if (defined CONNECTOR_TRANSPORT_UDP)
        case connector_transport_udp:
        {
            uint8_t const sm_udp_version_num = SM_UDP_VERSION << 4;
            uint8_t const version_byte = sm_udp_version_num | sm_ptr->transport.id_type;

            *data_ptr++ = version_byte;
            ASSERT(connector_ptr->connector_got_device_id);
            memcpy(data_ptr, sm_ptr->transport.id, sm_ptr->transport.id_length);
            data_ptr += sm_ptr->transport.id_length;
            break;
        }
        #endif

        #if (defined CONNECTOR_TRANSPORT_SMS)
        case connector_transport_sms:
        {
            /* service ID available? */
            if (sm_ptr->transport.id_length > 0)
            {
                ASSERT(sm_ptr->transport.id != NULL);
                memcpy(data_ptr, sm_ptr->transport.id, sm_ptr->transport.id_length);
                data_ptr += sm_ptr->transport.id_length;
                *data_ptr++ = ' ';
            }

            break;
        }
        #endif

        default:
            ASSERT(connector_false);
            break;
    }

    sm_header = data_ptr;

    {
        uint8_t const sm_version_num = 0x01 << 5;
        uint8_t const request_id_hi = (session->request_id & SM_REQUEST_ID_MASK) >> 8;
        uint8_t const request_id_low = session->request_id & 0xFF;
        uint8_t info_field = sm_version_num | request_id_hi;

        if (SmIsResponse(session->flags))
            SmSetResponse(info_field);
        else if (SmIsResponseNeeded(session->flags))
            SmSetResponseNeeded(info_field);

        if (session->segments.processed == 0)
        {
            uint8_t cmd_field = SmIsRequest(session->flags) ? session->command : 0;

            if (SmIsError(session->flags))
                SmSetError(cmd_field);
            if (SmIsCompressed(session->flags))
                SmSetCompressed(cmd_field);

            #if (defined CONNECTOR_SM_MULTIPART)
            if (SmIsMultiPart(session->flags))
            {
                uint8_t * const segment0 = sm_header;

                SmSetMultiPart(info_field);
                message_store_u8(segment0, info, info_field);
                message_store_u8(segment0, request, request_id_low);
                message_store_u8(segment0, segment, session->segments.processed);
                message_store_u8(segment0, count, session->segments.count);
                message_store_u8(segment0, cmd_status, cmd_field);
                message_store_be16(segment0, crc, 0);
                data_ptr += record_end(segment0);
            }
            else
            #endif
            {
                uint8_t * const segment = sm_header;

                ASSERT_GOTO(SmIsNotMultiPart(session->flags), done);
                message_store_u8(segment, info, info_field);
                message_store_u8(segment, request, request_id_low);
                message_store_u8(segment, cmd_status, cmd_field);
                message_store_be16(segment, crc, 0);
                data_ptr += record_end(segment);
            }
        }
        else
        {
            uint8_t * const segmentn = sm_header;

            ASSERT_GOTO(SmIsMultiPart(session->flags), done);
            SmSetMultiPart(info_field);
            message_store_u8(segmentn, info, info_field);
            message_store_u8(segmentn, request, request_id_low);
            message_store_u8(segmentn, segment, session->segments.processed);
            message_store_be16(segmentn, crc, 0);
            data_ptr += record_end(segmentn);
        }
    }

    {
        size_t const filled_bytes = data_ptr - send_ptr->data;
        size_t const bytes_available = sm_ptr->transport.sm_mtu_tx - (data_ptr - sm_header);
        size_t payload_bytes;

        if (SmIsError(session->flags))
        {
            uint16_t const error_code = (uint16_t)session->error;
            char * const error_text = (session->error == connector_sm_error_in_request) ? "Request error" : "Unexpected request";
            size_t const error_text_length = strlen(error_text) + 1;
            size_t const error_code_length = sizeof error_code;

            StoreBE16(data_ptr, error_code);
            memcpy(data_ptr + error_code_length, error_text, error_text_length);
            payload_bytes = error_code_length + error_text_length;
        }
        else
        {
            payload_bytes = (session->in.bytes < bytes_available) ? session->in.bytes : bytes_available;

            if (payload_bytes > 0)
            {
                memcpy(data_ptr, &session->in.data[session->bytes_processed], payload_bytes);
                session->bytes_processed += payload_bytes;
                session->in.bytes -= payload_bytes;
            }
        }

        send_ptr->total_bytes = filled_bytes + payload_bytes;

        {
            uint8_t * const crc_field = data_ptr - 2;
            size_t const header_bytes = data_ptr - sm_header;
            size_t const crc_bytes = header_bytes + payload_bytes;
            uint16_t crc_value = 0;

            crc_value = sm_calculate_crc16(crc_value, sm_header, crc_bytes);
            StoreBE16(crc_field, crc_value);
        }
    }

    send_ptr->pending_session = session;
    #if (defined CONNECTOR_TRANSPORT_SMS)
    if (SmIsEncoded(session->flags))
    {
        session->sm_state = connector_sm_state_encoding;
        /* Increase pointer to skip preamble encoding */
        if (sm_ptr->transport.id_length)
        {
            send_ptr->data += (sm_ptr->transport.id_length + SMS_SERVICEID_WRAPPER_TX_SIZE);
            send_ptr->total_bytes -= (sm_ptr->transport.id_length + SMS_SERVICEID_WRAPPER_TX_SIZE);
        }

        result = sm_encode_segment(connector_ptr, sm_ptr, session);

        /* Restore pointer if it has preamble */
        if (sm_ptr->transport.id_length)
        {
            send_ptr->data -= (sm_ptr->transport.id_length + SMS_SERVICEID_WRAPPER_TX_SIZE);
            send_ptr->total_bytes += (sm_ptr->transport.id_length + SMS_SERVICEID_WRAPPER_TX_SIZE);
        }
    }
    #endif
send:
    if (result == connector_working)
    {
        result = sm_send_segment(connector_ptr, sm_ptr);
    }

done:
    return result;
}

STATIC connector_status_t sm_process_send_path(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;

    ASSERT_GOTO(session != NULL, error);
    switch (session->sm_state)
    {
        case connector_sm_state_get_total_length:
            result = sm_get_user_data_length(connector_ptr, sm_ptr, session);
            break;

        case connector_sm_state_prepare_payload:
            result = sm_prepare_payload(connector_ptr, session);
            break;

        case connector_sm_state_more_data:
#if (defined CONNECTOR_DATA_SERVICE)
            result = SmIsClientOwned(session->flags) ? sm_get_more_request_data(connector_ptr, session) : sm_get_more_response_data(connector_ptr, session);
#endif
            break;

        #if (defined CONNECTOR_COMPRESSION)
        case connector_sm_state_compress:
            result = sm_compress_data(connector_ptr, session);
            break;
        #endif

        case connector_sm_state_prepare_segment:
            result = sm_prepare_segment(sm_ptr, session);
            break;

        #if (defined CONNECTOR_TRANSPORT_SMS)
        case connector_sm_state_encoding:
            result = sm_encode_segment(connector_ptr, sm_ptr, session);
            if (result == connector_working)
            {
                result = sm_send_segment(connector_ptr, sm_ptr);
            }
            break;
        #endif

        case connector_sm_state_send_data:
            result = sm_send_data(connector_ptr, sm_ptr, session);
            break;

        default:
            ASSERT(connector_false);
            break;
    }

    sm_verify_result(sm_ptr, &result);

error:
    return result;
}

