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

typedef enum
{
    data_service_opcode_put_request,
    data_service_opcode_put_response,
    data_service_opcode_device_request,
    data_service_opcode_device_response
} data_service_opcode_t;

typedef struct
{
    void * callback_context;
    connector_request_data_service_send_t const * header;
    connector_request_id_data_service_t request_type;
} data_service_context_t;

STATIC void set_data_service_error(msg_service_request_t * const service_request, connector_session_error_t const error_code)
{
    service_request->error_value = error_code;
    service_request->service_type = msg_service_type_error;
}


STATIC connector_status_t call_ds_receive_callback(connector_data_t * const connector_ptr,
                                                   data_service_context_t * const data_service,
                                                   void * const data)
{
    connector_status_t result = connector_working;
    connector_request_id_t request_id;

    request_id.data_service_request = data_service->request_type;
    switch (request_id.data_service_request)
    {
        case connector_request_id_data_service_receive_reply_length:
            request_id.data_service_request = connector_request_id_data_service_receive_reply_data;
            /* fall thru for error (not-handle) response data */
        case connector_request_id_data_service_receive_target:
        case connector_request_id_data_service_receive_data:
        case connector_request_id_data_service_receive_status:
        case connector_request_id_data_service_receive_reply_data:
        {
            connector_callback_status_t const status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request_id, data, connector_ptr->context);
            switch (status)
            {
                case connector_callback_continue:
                    break;
                case connector_callback_error:
                    data_service->request_type = connector_request_id_data_service_receive_reply_length;
                    break;
                case connector_callback_busy:
                    result = connector_pending;
                    break;
                default:
                    result = connector_abort;
                    break;

            }
            break;
        }

        default:
            ASSERT(connector_false);
            break;
    }

    return result;
}

STATIC connector_status_t process_ds_receive_target(connector_data_t * const connector_ptr,
                                                    data_service_context_t * const data_service,
                                                    uint8_t const * const data,
                                                    size_t * const data_length)
{
    /* 1st message so let's parse message-start packet:
     *
     * Data Service Device request format:
     *  -------------------------------------------------------------------------------------------------------
     * |   0    |   1    |  2+N   |    +1     |     +1      |    +1       |    +M       |  ...       | +P      |
     *  -------------------------------------------------------------------------------------------------------
     * | Opcode | Target | Target | Parameter | Parameter 1 | Parameter 1 | Parameter 1 | Additional | Payload |
     * |        | length | string |   count   |     ID      | data length |    data     | parameters |         |
     *  -------------------------------------------------------------------------------------------------------
     *
     */
    enum {
        field_define(ds_device_request, opcode, uint8_t),
        field_define(ds_device_request, target_length, uint8_t),
        record_end(ds_device_request_header)
    };

    enum {
        field_define(ds_device_request, parameter_count, uint8_t)
    };

    enum {
        field_define(ds_device_request, parameter_id, uint8_t),
        field_define(ds_device_request, parameter_length, uint8_t),
        record_end(ds_device_request_parameter)
    };

    connector_status_t result = connector_working;
    uint8_t const * ds_device_request = data;

    char * target_string = NULL;
    uint8_t const target_length =  message_load_u8(ds_device_request, target_length);

    size_t const min_data_length = (size_t)(target_length +
                      record_bytes(ds_device_request_header) +
                      field_named_data(ds_device_request, parameter_count, size));

    ASSERT_GOTO((message_load_u8(ds_device_request, opcode) == data_service_opcode_device_request), done);
    ASSERT_GOTO(*data_length >= min_data_length, done);

    ds_device_request += record_bytes(ds_device_request_header);

    target_string = (char *)ds_device_request;
    ds_device_request += target_length;

    {
        /* TODO: Parse and process each parameter in the future.
         *      Ignore all parameters now.
         */

        uint8_t const parameter_count = message_load_u8(ds_device_request, parameter_count);
        uint8_t i;

        ds_device_request += field_named_data(ds_device_request, parameter_count, size);

        for (i=0; i < parameter_count; i++)
        {
             unsigned int const parameter_length = message_load_u8(ds_device_request, parameter_length);
             size_t const min_parameter_length = min_data_length + record_bytes(ds_device_request_parameter) + parameter_length;
             ASSERT_GOTO(*data_length >= min_parameter_length, done);

             ds_device_request += record_bytes(ds_device_request_parameter); /* skip id and length */
             ds_device_request += parameter_length;

        }
    }

    *data_length = (ds_device_request - data);

    /* Add NUL to the target string. Must NULL-terminate it after parsing all parameters.
     * The NUL char is on parameter_count field in the request.
     */
    target_string[target_length] = '\0';

    switch (data_service->request_type)
    {
        case connector_request_id_data_service_receive_target:
        {

            connector_data_service_receive_target_t device_request;

            device_request.transport = connector_transport_tcp;
            device_request.user_context = data_service->callback_context;
            device_request.target = target_string;
            device_request.response_required = connector_true;

            result = call_ds_receive_callback(connector_ptr, data_service, &device_request);
            data_service->callback_context = device_request.user_context;
            break;
        }
        default:
            /* just skip the header and return */
            break;
    }

done:
    return result;
}

STATIC connector_status_t process_ds_receive_data(connector_data_t * const connector_ptr,
                                                  data_service_context_t * const data_service,
                                                  uint8_t const * const data,
                                                  size_t const data_length,
                                                  unsigned int const flags)
{
    connector_status_t result;

    connector_data_service_receive_data_t device_request;
    device_request.transport = connector_transport_tcp;
    device_request.user_context = data_service->callback_context;
    device_request.bytes_used = data_length;
    device_request.buffer = device_request.bytes_used != 0 ? data : NULL;
    device_request.more_data = MsgIsLastData(flags) ? connector_false : connector_true;

    result = call_ds_receive_callback(connector_ptr, data_service, &device_request);
    data_service->callback_context = device_request.user_context;

    return result;
}

STATIC connector_status_t process_data_service_device_request(connector_data_t * const connector_ptr,
                                                 msg_service_request_t * const service_request)
{
    connector_status_t result = connector_working;
    msg_session_t * const session = service_request->session;
    data_service_context_t * data_service = session->service_context;
    msg_service_data_t * const service_data = service_request->have_data;

    uint8_t const * ds_device_request = service_data->data_ptr;
    size_t ds_device_request_length = service_data->length_in_bytes;

    connector_bool_t const isFirstRequest = connector_bool(MsgIsStart(service_data->flags));

    if (isFirstRequest)
    {
        if (data_service == NULL)
        {
            /* 1st time here so let's allocate service context memory for device request service */
            void * ptr;

            result = malloc_data_buffer(connector_ptr, sizeof *data_service, named_buffer_id(msg_service), &ptr);
            if (result != connector_working)
            {
                goto done;
            }

            data_service = ptr;
            session->service_context = data_service;
            data_service->callback_context = NULL;
            data_service->request_type = connector_request_id_data_service_receive_target;
        }
    }

    switch (data_service->request_type)
    {
        case connector_request_id_data_service_receive_target:
            ds_device_request_length = service_data->length_in_bytes;

            result = process_ds_receive_target(connector_ptr, data_service, ds_device_request, &ds_device_request_length);

            switch (result)
            {
            case connector_working:
                if (data_service->request_type == connector_request_id_data_service_receive_target)
                    data_service->request_type = connector_request_id_data_service_receive_data;
                /* make it return busy so it comes here to skip parsing the header for the 1st data block */
                result = connector_pending;
                break;
            default:
                break;
            }
            break;

        case connector_request_id_data_service_receive_data:
            if (isFirstRequest)
            {
                /* skip the header; just get the point to data  */
                result = process_ds_receive_target(connector_ptr, data_service, ds_device_request, &ds_device_request_length);
                if (result != connector_working) goto done;
                ds_device_request += ds_device_request_length;
            }
            {
                size_t const data_length = (size_t)(ds_device_request - (uint8_t *)service_data->data_ptr);
                ds_device_request_length = service_data->length_in_bytes - data_length;

                result = process_ds_receive_data(connector_ptr, data_service, ds_device_request, ds_device_request_length, service_data->flags);
                switch (result)
                {
                    case connector_working:
                        /* data_service->request_type may have been changed inside process_ds_receive_data() */
                        switch (data_service->request_type)
                        {
                            case connector_request_id_data_service_receive_reply_length:
                                break;

                            default:
                                if (MsgIsLastData(service_data->flags))
                                    data_service->request_type = connector_request_id_data_service_receive_reply_data;
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
        case connector_request_id_data_service_receive_reply_length:
            /* We set this when callback returns error.
             * We need to respond with an error after receiving all request data.
             */
            break;

        default:
            ASSERT(connector_false);
            goto done;
    }

done:
    return result;
}

STATIC connector_status_t process_data_service_device_response(connector_data_t * const connector_ptr,
                                                               msg_service_request_t * const service_request)
{
    /* Data Service Device response format:
     *  ---------------------------------
     * |   0    |   1    |     2...      |
     *  ---------------------------------
     * | Opcode | status | Response Data |
     *  ---------------------------------
     */
    enum {
        field_define(ds_device_response, opcode, uint8_t),
        field_define(ds_device_response, status, uint8_t),
        record_end(ds_device_response_header)
    };

    connector_status_t result = connector_working;
    msg_service_data_t * const service_data = service_request->need_data;
    msg_session_t * const session = service_request->session;
    data_service_context_t * const data_service = session->service_context;
    connector_bool_t const isFirstResponse = connector_bool(MsgIsStart(service_data->flags));

    /* save some room for response header on 1st response data */
    size_t const header_length = isFirstResponse == connector_true ? record_bytes(ds_device_response_header) : 0;
    uint8_t * const data_ptr = service_data->data_ptr;


    connector_data_service_receive_reply_data_t device_request;

    device_request.transport = connector_transport_tcp;
    device_request.user_context = data_service->callback_context;
    device_request.buffer = data_ptr + header_length;
    device_request.bytes_available = service_data->length_in_bytes - header_length;
    device_request.bytes_used = 0;
    device_request.more_data = connector_false;

    {
        connector_request_id_data_service_t const request_type = data_service->request_type;

        switch (request_type)
        {
            case connector_request_id_data_service_receive_reply_length:
            /* We got here because callback returns error for request data.
             * Just get the replay data.
             */
             /* fall thru */
            case connector_request_id_data_service_receive_reply_data:
            {

                result = call_ds_receive_callback(connector_ptr, data_service, &device_request);
                data_service->callback_context = device_request.user_context;
                if (request_type != data_service->request_type &&
                    data_service->request_type == connector_request_id_data_service_receive_reply_length)
                {
                    /* callback returns error on reply data
                     * We need to cancel the message in messaging layer.
                     */
                    device_request.bytes_used = 0;
                    device_request.more_data = connector_false;
                    set_data_service_error(service_request, connector_session_error_cancel);
                }
                break;
            }
            default:
                /* should be here */
                ASSERT(connector_false);
                goto done;
        }
    }
    if (isFirstResponse)
    {

        enum {
            connector_data_service_device_success,
            connector_data_service_device_not_handled
        };

        /* Add header for 1st response message */
        uint8_t * const ds_device_response = service_data->data_ptr;
        uint8_t const target_status = (data_service->request_type == connector_request_id_data_service_receive_reply_length) ?
                                connector_data_service_device_not_handled: connector_data_service_device_success;

        message_store_u8(ds_device_response, opcode, data_service_opcode_device_response);
        message_store_u8(ds_device_response, status, target_status);
    }

    if (!device_request.more_data)
    {
        MsgSetLastData(service_data->flags);
    }
    service_data->length_in_bytes = device_request.bytes_used + header_length;

done:
    return result;
}


STATIC connector_status_t process_data_service_device_error(connector_data_t * const connector_ptr,
                                                       msg_service_request_t * const service_request)
{
    connector_status_t result;
    msg_session_t * const session = service_request->session;
    data_service_context_t * const data_service = session->service_context;
    connector_data_service_status_t device_request;


    device_request.transport = connector_transport_tcp;
    device_request.user_context = data_service->callback_context;
    data_service->request_type = connector_request_id_data_service_receive_status;

    switch (service_request->error_value)
    {
        case connector_session_error_none:
            device_request.status = connector_data_service_status_complete;
            break;

        case connector_session_error_cancel:
            device_request.status = connector_data_service_status_cancel;
            break;

        case connector_session_error_timeout:
            device_request.status = connector_data_service_status_timeout;
            break;

        default:
            device_request.status = connector_data_service_status_session_error;
            device_request.session_error = service_request->error_value;
            break;
    }

    result = call_ds_receive_callback(connector_ptr, data_service, &device_request);

    data_service->callback_context = device_request.user_context;

    return result;
}

STATIC connector_status_t data_service_device_request_callback(connector_data_t * const connector_ptr, msg_service_request_t * const service_request)
{
    connector_status_t status = connector_working;

    switch (service_request->service_type)
    {
    case msg_service_type_need_data:
        status = process_data_service_device_response(connector_ptr, service_request);
        break;

    case msg_service_type_have_data:
        status = process_data_service_device_request(connector_ptr, service_request);
        break;

    case msg_service_type_error:
    {
        msg_session_t * const session = service_request->session;
        session->error = service_request->error_value;
        status = process_data_service_device_error(connector_ptr, service_request);
        break;
    }

    case msg_service_type_free:
        {
            msg_session_t const * const session = service_request->session;
            data_service_context_t const * const data_service = session->service_context;
            connector_request_id_data_service_t const previous_request = data_service->request_type;

            if (previous_request != connector_request_id_data_service_receive_status)
            {
                /* Always call the user to inform that session is done */
                status = process_data_service_device_error(connector_ptr, service_request);
                if (status != connector_working)
                    break;
            }
            status = free_data_buffer(connector_ptr, named_buffer_id(msg_service), session->service_context);
            break;
        }

    default:
        ASSERT(connector_false);
        break;
    }

    return status;
}

STATIC size_t fill_put_request_header(connector_request_data_service_send_t const * const request, uint8_t * const data)
{
    uint8_t * ptr = data;

    *ptr++ = data_service_opcode_put_request;

    /* fill path */
    if (request->path != NULL)
    {
        uint8_t const bytes = (uint8_t) strlen(request->path);

        ASSERT(strlen(request->path) <= UCHAR_MAX);
        *ptr++ = bytes;
        memcpy(ptr, request->path, bytes);
        ptr += bytes;
    }

    /* fill parameters */
    {
        connector_bool_t const have_type = connector_bool(request->content_type != NULL);
        uint8_t const parameter_requested = 1;
        uint8_t params = have_type ? 1 : 0;

        enum
        {
            parameter_id_content_type,
            parameter_id_archive,
            parameter_id_append,
            parameter_id_transient
        };

        if (request->option != connector_data_service_send_option_overwrite) params++;

        *ptr++ = params;

        if (have_type)
        {
            uint8_t const bytes = (uint8_t) strlen(request->content_type);

            ASSERT(strlen(request->content_type) <= UCHAR_MAX);
            *ptr++ = parameter_id_content_type;
            *ptr++ = bytes;
            memcpy(ptr, request->content_type, bytes);
            ptr += bytes;
        }

        switch(request->option)
        {
            case connector_data_service_send_option_archive:
                *ptr++ = parameter_id_archive;
                *ptr++ = parameter_requested;
                break;

            case connector_data_service_send_option_append:
                *ptr++ = parameter_id_append;
                *ptr++ = parameter_requested;
                break;

            case connector_data_service_send_option_transient:
                *ptr++ = parameter_id_transient;
                *ptr++ = parameter_requested;
                break;

            default:
                break;
        }
    }

    return (size_t)(ptr - data);
}

STATIC connector_status_t call_put_request_user(connector_data_t * const connector_ptr, msg_service_request_t * const service_request, connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_status_t status = connector_working;
    connector_send_data_initiator_t const request_initiator = service_request->send_data_initiator;
    connector_callback_status_t callback_status = connector_callback_continue;

    switch (request_initiator)
    {
#if (defined CONNECTOR_DATA_POINTS)
        case connector_send_data_initiator_data_point:
        {
            callback_status = dp_handle_callback(connector_ptr, request_id, cb_data);
            break;
        }
#endif
        case connector_send_data_initiator_user:
        {
            connector_request_id_t request;

            request.data_service_request = request_id;
            callback_status = connector_callback(connector_ptr->callback, connector_class_id_data_service, request, cb_data, connector_ptr->context);
            break;
        }
        case connector_send_data_initiator_unknown:
            ASSERT_GOTO(request_initiator != connector_send_data_initiator_unknown, done);
    }

    switch (callback_status)
    {
        case connector_callback_continue:
            status = connector_working;
            break;

        case connector_callback_error:
            set_data_service_error(service_request, connector_session_error_cancel);
            status = connector_working;
            break;

        case connector_callback_busy:
            status = connector_pending;
            break;

        default:
            status = connector_abort;
            break;
    }
done:
    return status;
}

STATIC connector_status_t process_send_request(connector_data_t * const connector_ptr, msg_service_request_t * const service_request, data_service_context_t * const ds_ptr)
{
    connector_status_t status = connector_working;
    msg_service_data_t * const service_data = service_request->need_data;
    connector_data_service_send_data_t user_data;

    user_data.transport = connector_transport_tcp;
    user_data.user_context = ds_ptr->callback_context;
    user_data.bytes_used = 0;
    user_data.more_data = connector_false;

    if (MsgIsStart(service_data->flags))
    {
        uint8_t * dptr = service_data->data_ptr;
        size_t const bytes = fill_put_request_header(ds_ptr->header, dptr);

        if (bytes >= service_data->length_in_bytes)
        {
            connector_debug_line("process_send_request: required bytes [%" PRIsize "] is more than available [%" PRIsize "]", bytes, service_data->length_in_bytes);
            goto error;
        }

        user_data.buffer = dptr + bytes;
        user_data.bytes_available = service_data->length_in_bytes - bytes;
        service_data->length_in_bytes = bytes;
    }
    else
    {
        user_data.buffer = service_data->data_ptr;
        user_data.bytes_available = service_data->length_in_bytes;
        service_data->length_in_bytes = 0;
    }

    status = call_put_request_user(connector_ptr, service_request, connector_request_id_data_service_send_data, &user_data);

    if (status == connector_working)
    {
        service_data->flags = 0;
        service_data->length_in_bytes += user_data.bytes_used;
        if (user_data.more_data == connector_false)
            MsgSetLastData(service_data->flags);
    }
    goto done;

error:
    set_data_service_error(service_request, connector_session_error_format);

done:
    return status;
}

STATIC connector_status_t process_send_response(connector_data_t * const connector_ptr, msg_service_request_t * const service_request, data_service_context_t * const ds_ptr)
{
    connector_status_t status = connector_working;
    connector_data_service_send_response_t user_data;

    /* Data Service put response format:
     *  ---------------------------------
     * |   0    |   1    |     2...      |
     *  ---------------------------------
     * | Opcode | status | Response Data |
     *  ---------------------------------
     */
    enum
    {
        field_define(put_response, opcode, uint8_t),
        field_define(put_response, status, uint8_t),
        record_end(put_response)
    };

    enum
    {
        ds_data_success,
        ds_data_bad_request,
        ds_data_service_unavailable,
        ds_data_cloud_error
    };

    msg_service_data_t * const service_data = service_request->have_data;
    uint8_t * const put_response = service_data->data_ptr;
    uint8_t const opcode = message_load_u8(put_response, opcode);
    uint8_t const result = message_load_u8(put_response, status);

    ASSERT_GOTO(MsgIsStart(service_data->flags), error);
    ASSERT_GOTO(opcode == data_service_opcode_put_response, error);

    user_data.transport = connector_transport_tcp;
    user_data.user_context = ds_ptr->callback_context;
    if (service_data->length_in_bytes > record_end(put_response))
    {
        int const max_hint_length = (MSG_MAX_RECV_PACKET_SIZE - PACKET_EDP_HEADER_SIZE - record_end(start_packet) - record_end(put_response));
        uint8_t * const hint_start = put_response + record_end(put_response);
        uint8_t * const hint_end = put_response + service_data->length_in_bytes;
        size_t hint_length = (hint_end - hint_start) < max_hint_length ? hint_end - hint_start : max_hint_length;
        char * const hint = (char *)hint_start;

        /* Add a null-terminator only if necessary */
        if (hint[hint_length - 1] != '\0')
            hint[hint_length] = '\0';
        user_data.hint = hint;
    }
    else
    {
        user_data.hint = NULL;
    }

    switch (result)
    {
    case ds_data_success:
        user_data.response = connector_data_service_send_response_success;
        break;

    case ds_data_bad_request:
        user_data.response = connector_data_service_send_response_bad_request;
        break;

    case ds_data_service_unavailable:
        user_data.response = connector_data_service_send_response_unavailable;
        break;

    case ds_data_cloud_error:
        user_data.response = connector_data_service_send_response_cloud_error;
        break;

    default:
        ASSERT(connector_false);
        break;
    }

    status = call_put_request_user(connector_ptr, service_request, connector_request_id_data_service_send_response, &user_data);
    goto done;

error:
    set_data_service_error(service_request, connector_session_error_format);

done:
    return status;
}

STATIC connector_status_t process_send_error(connector_data_t * const connector_ptr, msg_service_request_t * const service_request, void * const cb_context)
{
    connector_status_t status;
    connector_data_service_status_t user_data;
    msg_session_t * const session = service_request->session;
    connector_bool_t const session_created = connector_bool(session != NULL);

    user_data.transport = connector_transport_tcp;
    user_data.user_context = cb_context;
    user_data.session_error = connector_session_error_none;

    switch (service_request->error_value)
    {
        case connector_session_error_none:
            user_data.status = connector_data_service_status_complete;
            break;

        case connector_session_error_cancel:
            user_data.status = connector_data_service_status_cancel;
            break;

        case connector_session_error_timeout:
            user_data.status = connector_data_service_status_timeout;
            break;

        default:
            user_data.status = connector_data_service_status_session_error;
            user_data.session_error = service_request->error_value;
            break;
    }

    if (!session_created)
    {
        status = call_put_request_user(connector_ptr, service_request, connector_request_id_data_service_send_status, &user_data);
    }
    else
    {
        data_service_context_t * const data_service = session->service_context;

        data_service->request_type = connector_request_id_data_service_send_status;
        status = call_put_request_user(connector_ptr, service_request, data_service->request_type, &user_data);
    }

    return status;
}

STATIC connector_status_t data_service_put_request_callback(connector_data_t * const connector_ptr, msg_service_request_t * const service_request)
{
    connector_status_t status;
    msg_session_t * const session = service_request->session;
    data_service_context_t * const ds_ptr = session->service_context;

    switch (service_request->service_type)
    {
        case msg_service_type_need_data:
            status = process_send_request(connector_ptr, service_request, ds_ptr);
            break;

        case msg_service_type_have_data:
            status = process_send_response(connector_ptr, service_request, ds_ptr);
            break;

        case msg_service_type_error:
        {
            msg_session_t * const session = service_request->session;
            session->error = service_request->error_value;
            status = process_send_error(connector_ptr, service_request, ds_ptr->callback_context);
            break;
        }

        case msg_service_type_free:
            {
                msg_session_t const * const session = service_request->session;
                data_service_context_t const * const data_service = session->service_context;
                connector_request_id_data_service_t const previous_request = data_service->request_type;

                if (previous_request != connector_request_id_data_service_send_status)
                {
                    /* Always call the user to inform that session is done */
                    status = process_send_error(connector_ptr, service_request, ds_ptr->callback_context);
                    if (status != connector_working)
                        break;
                }
                if (ds_ptr != NULL)
                    status = free_data_buffer(connector_ptr, named_buffer_id(put_request), ds_ptr);
                else
                    status = connector_working;
                break;
            }
        default:
            status = connector_idle;
            ASSERT(connector_false);
            break;
    }

    return status;
}

STATIC connector_status_t data_service_put_request_init(connector_data_t * const connector_ptr, msg_service_request_t * const service_request)
{
    connector_status_t status = connector_invalid_data;
    connector_session_error_t result = service_request->error_value;
    msg_session_t * const session = service_request->session;
    connector_request_data_service_send_t * send_ptr = (void *)service_request->have_data;
    data_service_context_t * ds_ptr = NULL;
    void * ptr = NULL;

    ASSERT_GOTO(send_ptr != NULL, done);

#if !(defined CONNECTOR_DATA_POINTS)
    service_request->send_data_initiator = connector_send_data_initiator_user;
#else
    if (strncmp(send_ptr->path, internal_dp4d_path, internal_dp4d_path_strlen) == 0)
    {
        char * const modifiable_path = (char *)send_ptr->path; /* Discarding "const" qualifier */

        memcpy(modifiable_path, dp4d_path_prefix, dp4d_path_prefix_strlen);

        service_request->send_data_initiator = connector_send_data_initiator_data_point;
    }
    else
    {
        service_request->send_data_initiator = connector_send_data_initiator_user;
    }
#endif

    status = connector_working;
    if ((result != connector_session_error_none) || (session == NULL)) goto error;

    status = malloc_data_buffer(connector_ptr, sizeof *ds_ptr, named_buffer_id(put_request), &ptr);
    ASSERT_GOTO(status == connector_working, error);
    ASSERT_GOTO(ptr != NULL, error);

    ds_ptr = ptr;

    ds_ptr->header = send_ptr;
    ds_ptr->callback_context = send_ptr->user_context;
    ds_ptr->request_type = connector_request_id_data_service_send_data;
    session->service_context = ds_ptr;

    goto done;

error:
    set_data_service_error(service_request, result);
    process_send_error(connector_ptr, service_request, send_ptr->user_context);

done:
    return status;
}

STATIC connector_status_t data_service_callback(connector_data_t * const connector_ptr, msg_service_request_t * const service_request)
{
    connector_status_t status = connector_idle;
    msg_session_t * session;
    data_service_context_t * ds_ptr;

    ASSERT_GOTO(connector_ptr != NULL, done);
    ASSERT_GOTO(service_request != NULL, done);

    session = service_request->session;

    if (service_request->service_type == msg_service_type_pending_request || session == NULL)
    {
        status = data_service_put_request_init(connector_ptr, service_request);
        goto done;
    }

    ds_ptr = session->service_context;

    if (ds_ptr == NULL)
    {
        status = data_service_device_request_callback(connector_ptr, service_request);
        goto done;
    }

    switch (ds_ptr->request_type)
    {
        case connector_request_id_data_service_send_length:
        case connector_request_id_data_service_send_data:
        case connector_request_id_data_service_send_status:
        case connector_request_id_data_service_send_response:
            status = data_service_put_request_callback(connector_ptr, service_request);
            break;

        case connector_request_id_data_service_receive_target:
        case connector_request_id_data_service_receive_data:
        case connector_request_id_data_service_receive_status:
        case connector_request_id_data_service_receive_reply_length:
        case connector_request_id_data_service_receive_reply_data:
            status = data_service_device_request_callback(connector_ptr, service_request);
            break;

        default:
            ASSERT(connector_false);
            break;
    }

done:
    return status;
}

STATIC connector_status_t connector_facility_data_service_cleanup(connector_data_t * const connector_ptr)
{
    return msg_cleanup_all_sessions(connector_ptr,  msg_service_id_data);
}

STATIC connector_status_t connector_facility_data_service_delete(connector_data_t * const data_ptr)
{
    return msg_delete_facility(data_ptr, msg_service_id_data);
}

STATIC connector_status_t connector_facility_data_service_init(connector_data_t * const data_ptr, unsigned int const facility_index)
{
    return msg_init_facility(data_ptr, facility_index, msg_service_id_data, data_service_callback);
}

STATIC connector_status_t data_service_initiate(connector_data_t * const connector_ptr,  void const * request)
{
    connector_status_t status = connector_invalid_data;
    msg_request_initiator_t initiator = MSG_REQUEST_USER;

    ASSERT_GOTO(request != NULL, error);
#if (defined CONNECTOR_DATA_POINTS)
    {
        connector_request_data_service_send_t const * const data_service_send = request;
        char const * const path = data_service_send->path;
        #if (defined CONNECTOR_DATA_POINTS)
        connector_bool_t const request_is_data_point = connector_bool(strncmp(path, internal_dp4d_path, internal_dp4d_path_strlen) == 0);
        #else
        connector_bool_t const request_is_data_point = connector_false;
        #endif

        if (request_is_data_point)
        {
            initiator = MSG_REQUEST_INTERNAL;
        }
    }
#endif

    status = msg_initiate_request(connector_ptr, request, initiator) == connector_true ? connector_success : connector_service_busy;

error:
    return status;
}
