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
#if (defined CONNECTOR_TRANSPORT_SMS)
STATIC connector_status_t sm_decode_segment(connector_data_t * const connector_ptr, connector_sm_packet_t * const recv_ptr)
{
    size_t const data_size = 1 + ((recv_ptr->total_bytes - recv_ptr->processed_bytes) * 4)/5;
    void * data_ptr = NULL;
    connector_status_t result = malloc_data_buffer(connector_ptr, data_size, named_buffer_id(sm_data_block), &data_ptr);

    if (result == connector_working)
    {
        recv_ptr->total_bytes = sm_decode85(data_ptr, data_size, recv_ptr->data + recv_ptr->processed_bytes, recv_ptr->total_bytes - recv_ptr->processed_bytes);
        memcpy(recv_ptr->data, data_ptr, recv_ptr->total_bytes);
        result = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), data_ptr);
    }

    return result;
}

STATIC connector_status_t sm_verify_sms_preamble(connector_sm_data_t * const sm_ptr)
{
    connector_status_t result = connector_working;

    if (sm_ptr->transport.id_length > 0)
    {
        uint8_t * const data_ptr = sm_ptr->network.recv_packet.data;
        char const prefix = '(';
        char const suffix[] = "):";
        size_t const suffix_bytes = sizeof suffix - 1;
        size_t const prefix_bytes = sizeof prefix;
        size_t const suffix_position = prefix_bytes + sm_ptr->transport.id_length;
        connector_bool_t const valid_prefix = connector_bool(*data_ptr == prefix);
        connector_bool_t const valid_shared_key = connector_bool(memcmp(data_ptr + prefix_bytes, sm_ptr->transport.id, sm_ptr->transport.id_length) == 0);
        connector_bool_t const valid_suffix = connector_bool(memcmp(data_ptr + suffix_position, suffix, suffix_bytes) == 0);

        if (valid_prefix && valid_shared_key && valid_suffix)
            sm_ptr->network.recv_packet.processed_bytes = suffix_position + suffix_bytes;
        else
        {
            connector_debug_line("sm_verify_sms_preamble: valid_prefix=%d, valid_shared_key=%d, valid_suffix=%d", valid_prefix, valid_shared_key, valid_suffix);
            connector_debug_line("%s",(char *)data_ptr);

            result = connector_invalid_response;
        }
    }

    return result;
}
#endif

#if (defined CONNECTOR_TRANSPORT_UDP)
STATIC connector_status_t sm_verify_udp_header(connector_sm_data_t * const sm_ptr)
{
    connector_status_t result = connector_invalid_response;
    connector_sm_packet_t * const recv_ptr = &sm_ptr->network.recv_packet;
    uint8_t * data_ptr = recv_ptr->data;
    uint8_t const version_and_type = *data_ptr++;
    uint8_t const version = version_and_type >> 4;
    connector_sm_id_type_t const type = (connector_sm_id_type_t)(version_and_type & 0x0F);

    if (version != SM_UDP_VERSION)
    {
        connector_debug_line("sm_verify_udp_header: invalid SM UDP version [%d]", version);
        result = connector_abort;
        goto error;
    }

    if (type != sm_ptr->transport.id_type)
        goto done;

    if (memcmp(data_ptr, sm_ptr->transport.id, sm_ptr->transport.id_length))
        goto done; /* not for us */

    recv_ptr->processed_bytes = sm_ptr->transport.id_length + 1;
    result = connector_working;

done:
error:
    return result;
}
#endif

#if (defined CONNECTOR_SM_MULTIPART)
STATIC connector_status_t sm_multipart_allocate(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t status = connector_working;
    size_t const max_payload_bytes = sm_get_max_payload_bytes(sm_ptr);
    size_t const max_session_bytes = (((max_payload_bytes * session->segments.count) + 3)/4) * 4; /* make sure it is word aligned */
    size_t const size_array_bytes = sizeof(*session->segments.size_array) * session->segments.count;

    ASSERT_GOTO(session->in.data == NULL, error);
    session->in.bytes = max_session_bytes + size_array_bytes;
    status = sm_allocate_user_buffer(connector_ptr, &session->in);
    ASSERT_GOTO(status == connector_working, error);
    session->segments.size_array = (void *)(session->in.data + max_session_bytes); /* alignment issue is taken care where max_session_bytes is defined */
    memset(session->segments.size_array, 0, size_array_bytes);

error:
    return status;
}
#endif

STATIC connector_status_t sm_more_data_callback(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr)
{
    connector_status_t result;
    connector_request_id_t request_id;
    connector_callback_status_t callback_status;
    connector_sm_more_data_t cb_data;

    cb_data.transport = sm_ptr->network.transport;
    request_id.sm_request = connector_request_id_sm_more_data;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_short_message, request_id, &cb_data, connector_ptr->context);
    result = sm_map_callback_status_to_connector_status(callback_status);

    return result;
}

typedef struct
{
    uint32_t request_id;
    size_t bytes;

    connector_bool_t isRequest;
    connector_bool_t isResponseNeeded;
    connector_bool_t isPackCmd;
    connector_bool_t isMultipart;
    connector_bool_t isCompressed;
    connector_bool_t isError;

    connector_sm_cmd_t command;

    enum
    {
        sm_single_segment,
        sm_multipart_first_segment,
        sm_multipart_subsequent_segment
    } type;

    struct
    {
        uint8_t count;
        uint8_t number;
    } segment;

    uint16_t crc16;
    uint8_t cmd_status;
} sm_header_t;

STATIC connector_status_t sm_process_header(connector_sm_packet_t * const recv_ptr, sm_header_t * const header)
{
    connector_status_t result = connector_invalid_payload_packet;
    uint8_t * data_ptr = &recv_ptr->data[recv_ptr->processed_bytes];
    uint8_t * const segment = data_ptr;

    header->segment.count = 1;
    header->segment.number = 0;
    header->bytes = 0;
    header->command = connector_sm_cmd_opaque_response;
    header->isError = connector_false;
    header->isCompressed = connector_false;

    {
        uint8_t const info_field = message_load_u8(segment, info);
        uint8_t const request_id_high_bits_mask = 0x03;

        header->request_id = (info_field & request_id_high_bits_mask) << 8;
        header->request_id |= message_load_u8(segment, request);

        header->isMultipart = SmIsMultiPart(info_field);
        header->isRequest = SmIsRequest(info_field);
        header->isResponseNeeded = SmIsResponseNeeded(info_field);
    }

    if (header->isMultipart)
    {
        #if (defined CONNECTOR_SM_MULTIPART)
        {
            uint8_t * const segment0 = data_ptr;

            header->segment.number = message_load_u8(segment0, segment);
            if (header->segment.number > 0)
            {
                uint8_t * const segmentn = data_ptr;

                header->cmd_status = 0;
                header->type = sm_multipart_subsequent_segment;

                if (header->isPackCmd)
                {
                    header->bytes = record_end(segmentn) - sizeof header->crc16;
                }
                else
                {
                    header->crc16 = message_load_be16(segmentn, crc);
                    message_store_be16(segmentn, crc, 0);
                    header->bytes = record_end(segmentn);
                }
            }
            else
            {
                header->type = sm_multipart_first_segment;
                header->segment.count = message_load_u8(segment0, count);
                header->cmd_status = message_load_u8(segment0, cmd_status);

                if (header->isPackCmd)
                {
                    header->bytes = record_end(segment0) - sizeof header->crc16;
                }
                else
                {
                    header->crc16 = message_load_be16(segment0, crc);
                    message_store_be16(segment0, crc, 0);
                    header->bytes = record_end(segment0);
                }
            }
        }
        #else
        {
            connector_debug_line("Received multipart packet, but multipart is disabled");
            connector_debug_line("Review CONNECTOR_SM_MULTIPART and CONNECTOR_SM_MAX_RX_SEGMENTS defines");
            goto error;
        }
        #endif
    }
    else
    {
        header->cmd_status = message_load_u8(segment, cmd_status);
        if (header->isPackCmd)
        {
            header->bytes = record_end(segment) - sizeof header->crc16;
        }
        else
        {
            header->crc16 = message_load_be16(segment, crc);
            message_store_be16(segment, crc, 0);
            header->bytes = record_end(segment);
        }
    }

    header->isCompressed = SmIsCompressed(header->cmd_status);
    #if !(defined CONNECTOR_COMPRESSION)
    if (header->isCompressed)
    {
        connector_debug_line("sm_process_header: Received compressed packet, but compression is disabled");
        goto error;
    }
    #endif

    if (header->isRequest)
    {
        header->command = (connector_sm_cmd_t)(header->cmd_status & 0x7F);
        if (header->isPackCmd && (header->command == connector_sm_cmd_pack))
        {
            connector_debug_line("sm_process_header: Pack command inside a pack command is not allowed");
            goto error;
        }
        header->isError = connector_false;
    }
    else
    {
        header->isError = SmIsError(header->cmd_status);
        header->command = connector_sm_cmd_opaque_response;
    }

    if (!header->isPackCmd)
    {
        size_t const sm_bytes = recv_ptr->total_bytes - recv_ptr->processed_bytes;
        uint16_t calculated_crc = 0;

        calculated_crc = sm_calculate_crc16(calculated_crc, data_ptr, sm_bytes);
        if(calculated_crc != header->crc16)
        {
            connector_debug_line("sm_process_header: crc error");
            goto error;
        }

        if (header->isRequest)
            header->isPackCmd = connector_bool(header->command == connector_sm_cmd_pack);
    }

    recv_ptr->processed_bytes += header->bytes;
    result = connector_working;

error:
    return result;
}

STATIC connector_status_t sm_update_session(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr,
                                            sm_header_t * const header, size_t const payload_bytes)
{
    connector_status_t result;
    connector_sm_packet_t * const recv_ptr = &sm_ptr->network.recv_packet;
    connector_bool_t const client_originated = connector_bool(!header->isRequest);
    connector_sm_session_t * session = get_sm_session(sm_ptr, header->request_id, client_originated);

    if (session == NULL)
    {
        session = sm_create_session(connector_ptr, sm_ptr, connector_false);
        if (session == NULL)
        {
            result = connector_pending;
            goto error;
        }

        session->request_id = header->request_id;
        session->command = client_originated ? connector_sm_cmd_opaque_response : header->command;
    }

    if (header->segment.number == 0)
    {
        if (header->isCompressed) SmSetCompressed(session->flags);

        if (header->isRequest)
        {
            if (header->isResponseNeeded) SmSetResponseNeeded(session->flags);
            /* If the first segment of a multipart SMS that arrived was not segment0, previously stored session->command information is wrong as
             * for multipart messages, type is only included in the 0th segment. Update it here */
            session->command = header->command;
        }
        else if (header->isError)
        {
            uint16_t const error_value = LoadBE16(&recv_ptr->data[recv_ptr->processed_bytes]);

            switch (error_value)
            {
                case connector_sm_error_in_request:
                    session->error = connector_sm_error_in_request;
                    break;

                case connector_sm_error_unavailable:
                    session->error = connector_sm_error_unavailable;
                    break;

                default:
                    session->error = connector_sm_error_unknown;
                    break;
            }

            SmSetError(session->flags);
            recv_ptr->processed_bytes += sizeof error_value;
        }

        session->segments.count = header->segment.count;
    }

    #if (defined CONNECTOR_SM_MULTIPART)
    if (header->isMultipart)
    {
        SmSetMultiPart(session->flags);
        if (session->in.data == NULL)
        {
            if (header->segment.number > 0)
                session->segments.count = sm_ptr->session.max_segments;

            result = sm_multipart_allocate(connector_ptr, sm_ptr, session);
            ASSERT_GOTO(result == connector_working, error);
            ASSERT_GOTO(session->in.data != NULL, error);
        }

        if (session->segments.size_array[header->segment.number] == 0)
        {
            size_t const max_payload_bytes = sm_get_max_payload_bytes(sm_ptr);
            uint8_t * copy_to = session->in.data + (header->segment.number * max_payload_bytes);

            session->segments.size_array[header->segment.number] = payload_bytes;
            memcpy(copy_to, &recv_ptr->data[recv_ptr->processed_bytes], payload_bytes);
            session->segments.processed++;
        }
        else
        {
            connector_debug_line("sm_update_session: duplicate segment %d, in id %d", header->segment.number, session->request_id);
        }
    }
    else
    #endif
    {
        session->in.bytes = payload_bytes;
        if (payload_bytes > 0)
        {
            result = sm_allocate_user_buffer(connector_ptr, &session->in);
            ASSERT_GOTO(result == connector_working, error);
            memcpy(session->in.data, &recv_ptr->data[recv_ptr->processed_bytes], payload_bytes);
        }
        else
            session->in.data = NULL;
        session->segments.processed++;
    }

    if (session->segments.processed >= session->segments.count)
    {
        session->bytes_processed = 0;
        session->segments.processed = 0;
        session->sm_state = connector_sm_state_process_payload;

        #if (defined CONNECTOR_COMPRESSION)
        if (SmIsCompressed(session->flags))
        {
            session->compress.out.data = NULL;
            session->sm_state = connector_sm_state_decompress;
        }
        #endif
    }

    recv_ptr->processed_bytes += payload_bytes;
    result = connector_working;

error:
    return result;
}

STATIC connector_status_t sm_process_packet(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr)
{
    sm_header_t sm_header;
    connector_status_t result;
    connector_sm_packet_t * const recv_ptr = &sm_ptr->network.recv_packet;
    size_t sm_bytes = recv_ptr->total_bytes - recv_ptr->processed_bytes;

    sm_header.isPackCmd = connector_false;
    sm_header.crc16 = 0;
    do
    {
        result = sm_process_header(recv_ptr, &sm_header);
        if (result != connector_working) goto error;

        if ((sm_header.segment.count > sm_ptr->session.max_segments) || (sm_header.segment.number >= sm_ptr->session.max_segments))
        {
            connector_debug_line("sm_process_packet: Exceeded maximum segments [%" PRIsize "/%" PRIsize "]", sm_header.segment.count, sm_ptr->session.max_segments);
            connector_debug_line("Review CONNECTOR_SM_MULTIPART and CONNECTOR_SM_MAX_RX_SEGMENTS defines");
            goto error;
        }

        if (sm_header.command == connector_sm_cmd_pack)
        {
            enum sm_pack_t
            {
                field_define(pack_header, flag, uint8_t),
                field_define(pack_header, length, uint16_t),
                record_end(pack_header)
            };
            uint8_t * const pack_header = &recv_ptr->data[recv_ptr->processed_bytes];
            uint8_t const flag = message_load_u8(pack_header, flag);

            #define SM_MESSAGE_PENDING 0x01
            if ((flag & SM_MESSAGE_PENDING) == SM_MESSAGE_PENDING)
            {
                result = sm_more_data_callback(connector_ptr, sm_ptr);
                if (result != connector_working) goto error;
            }
            #undef SM_MESSAGE_PENDING

            sm_bytes = message_load_be16(pack_header, length);
            recv_ptr->processed_bytes += record_end(pack_header);
            continue;
        }

        {
            size_t const payload_bytes = sm_bytes - sm_header.bytes;

            ASSERT(sm_bytes >= sm_header.bytes);
            result = sm_update_session(connector_ptr, sm_ptr, &sm_header, payload_bytes);
            if (result != connector_working) goto error;
        }

        if (!sm_header.isPackCmd) break;

        {
            size_t const sm_header_size = 5;
            size_t const remaining_bytes = recv_ptr->total_bytes - recv_ptr->processed_bytes;

            if (remaining_bytes < sm_header_size) break;
        }

        sm_bytes = LoadBE16(&recv_ptr->data[recv_ptr->processed_bytes]);
        recv_ptr->processed_bytes += sizeof(uint16_t);

    } while (recv_ptr->processed_bytes < recv_ptr->total_bytes);

    result = connector_working;

error:
    if (result == connector_invalid_payload_packet)  /* unreliable method, so silently ignore the packet */
        result = connector_working;

    recv_ptr->total_bytes = 0;
    recv_ptr->processed_bytes = 0;

    return result;
}

STATIC connector_status_t sm_receive_data(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr)
{
    connector_status_t result = connector_pending;
    connector_sm_packet_t * const recv_ptr = &sm_ptr->network.recv_packet;
    connector_callback_status_t status;
    connector_request_id_t request_id;
    connector_network_receive_t read_data;

    if (recv_ptr->total_bytes > 0) goto done;

    read_data.handle = sm_ptr->network.handle;
    read_data.buffer = recv_ptr->data;
    read_data.bytes_available = sm_ptr->transport.mtu;
    read_data.bytes_used = 0;

    request_id.network_request = connector_request_id_network_receive;
    status = connector_callback(connector_ptr->callback, sm_ptr->network.class_id, request_id, &read_data, connector_ptr->context);
    ASSERT(status != connector_callback_unrecognized);
    switch (status)
    {
        case connector_callback_busy:
            result = connector_idle;
            goto done;

        case connector_callback_continue:
            recv_ptr->total_bytes = read_data.bytes_used;
            recv_ptr->processed_bytes = 0;

            switch (sm_ptr->network.transport)
            {
                #if (defined CONNECTOR_TRANSPORT_SMS)
                case connector_transport_sms:
                    result = sm_verify_sms_preamble(sm_ptr);
                    switch(result)
                    {
                        case connector_working:
                            break;
                        case connector_invalid_response:
                            /* not Device Cloud packet? Ignore the packet */
                            recv_ptr->total_bytes = 0;
                            recv_ptr->processed_bytes = 0;
                            result = connector_working;
                            goto done;
                        default:
                            goto done;
                    }

                    result = sm_decode_segment(connector_ptr, recv_ptr);

                    /* Remove sms preamble */
                    sm_ptr->network.recv_packet.processed_bytes = 0;

                    break;
                #endif

                #if (defined CONNECTOR_TRANSPORT_UDP)
                case connector_transport_udp:
                    result = sm_verify_udp_header(sm_ptr);
                    break;
                #endif

                default:
                    ASSERT_GOTO(connector_false, done);
                    break;
            }

            if(result != connector_working) goto done; /* not Device Cloud packet? */
                result = sm_process_packet(connector_ptr, sm_ptr);
            break;

        case connector_callback_abort:
            result = connector_abort;
            break;

        default:
            connector_debug_line("sm_receive_data: callback returned error [%d]", status);
            result = connector_device_error;
            break;
    }

    sm_verify_result(sm_ptr, &result);

done:
    return result;
}

#if (defined CONNECTOR_COMPRESSION)
STATIC connector_status_t sm_decompress_data(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t status;
    size_t const max_payload_bytes = sm_get_max_payload_bytes(sm_ptr);
    uint8_t zlib_header[] = {0x58, 0xC3};
    z_streamp const zlib_ptr = &session->compress.zlib;
    int zret;

    if (session->compress.out.data == NULL)
    {
        session->compress.out.bytes = max_payload_bytes;
        status = sm_allocate_user_buffer(connector_ptr, &session->compress.out);
        ASSERT_GOTO(status == connector_working, done);

        memset(zlib_ptr, 0, sizeof *zlib_ptr);
        zret = inflateInit(zlib_ptr);
        ASSERT_GOTO(zret == Z_OK, error);
        zlib_ptr->next_out = session->compress.out.data;
        zlib_ptr->avail_out = session->compress.out.bytes;
        zlib_ptr->next_in = zlib_header;
        zlib_ptr->avail_in = sizeof zlib_header;
    }

    while (zlib_ptr->avail_out > 0)
    {
        if (zlib_ptr->avail_in == 0)
        {
            if (session->segments.processed == session->segments.count)
            {
                SmSetLastData(session->flags);
                break;
            }
            else
            {
                size_t const data_index = session->segments.processed * max_payload_bytes;

                zlib_ptr->next_in = &session->in.data[data_index];
                zlib_ptr->avail_in = SmIsMultiPart(session->flags) ? session->segments.size_array[session->segments.processed] : session->in.bytes;
                session->segments.processed++;
            }
        }

        zret = inflate(zlib_ptr, Z_NO_FLUSH);
        switch(zret)
        {
            case Z_STREAM_END:
            case Z_BUF_ERROR:
            case Z_OK:
                break;

            default:
                status = connector_abort;
                connector_debug_line("ZLIB Return value [%d]", zret);
                ASSERT_GOTO(connector_false, error);
                break;
        }
    }

    {
        size_t const payload_bytes = session->compress.out.bytes - zlib_ptr->avail_out;

        status = sm_pass_user_data(connector_ptr, session, session->compress.out.data, payload_bytes);
        switch (status)
        {
            case connector_pending:
                goto done;

            case connector_working:
                if (SmIsNotLastData(session->flags))
                {
                    zlib_ptr->next_out = session->compress.out.data;
                    zlib_ptr->avail_out = session->compress.out.bytes;
                    goto done;
                }
                break;

            default:
                break;
        }
    }

error:
    zret = inflateEnd(zlib_ptr);
    if (zret != Z_OK)
    {
        status = connector_abort;
        goto done;
    }
    ASSERT_GOTO(zret == Z_OK, done);

    if (status != connector_abort)
    {
        status = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), session->compress.out.data);
        session->compress.out.data = NULL;
    }

done:
    return status;
}
#endif

STATIC connector_status_t sm_handle_error(connector_data_t * const connector_ptr, connector_sm_session_t * const session)
{
    connector_status_t result;
    connector_sm_state_t next_state = connector_sm_state_complete;

    if (SmIsCloudOwned(session->flags) && SmIsRequest(session->flags))
    {
        next_state = connector_sm_state_send_data;
    }

    SmSetError(session->flags);
    if (session->user.context != NULL) /* let the user know */
    {
        result = sm_inform_session_complete(connector_ptr, session);
        if (result != connector_working)
        {
            goto error;
        }
    }

    result = sm_switch_path(connector_ptr, session, next_state);

error:
    return result;
}

STATIC connector_status_t sm_handle_complete(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_working;

    if (session->in.data != NULL)
    {
        result = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), session->in.data);
        if (result != connector_working) goto error;
    }

    if (SmIsReboot(session->flags))
        result = sm_process_reboot(connector_ptr);

    switch(result)
    {
        case connector_abort:
            /* Keep connector_abort as the result */
            sm_delete_session(connector_ptr, sm_ptr, session);
            break;
        case connector_pending:
            break;
        default:
            result = sm_delete_session(connector_ptr, sm_ptr, session);
    }

error:
    return result;
}

STATIC connector_status_t sm_process_recv_path(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_abort;

    ASSERT_GOTO(session != NULL, error);
    switch (session->sm_state)
    {
        case connector_sm_state_receive_data:
            if (session->timeout_in_seconds != SM_WAIT_FOREVER)
            {
                unsigned long current_time = 0;

                result = get_system_time(connector_ptr, &current_time);
                ASSERT_GOTO(result == connector_working, error);
                if (current_time > (session->start_time + session->timeout_in_seconds))
                {
                    session->sm_state = connector_sm_state_error;
                    session->error = connector_sm_error_timeout;
                    connector_debug_line("Sm session [%u] timeout... start time:%u, current time:%u", session->request_id, session->start_time, current_time);
                }
            }

            result = connector_idle; /* still receiving data, handled in sm_receive_data() */
            break;

        #if (defined CONNECTOR_COMPRESSION)
        case connector_sm_state_decompress:
            result = sm_decompress_data(connector_ptr, sm_ptr, session);
            break;
        #endif

        case connector_sm_state_process_payload:
            result = sm_process_payload(connector_ptr, sm_ptr, session);
            break;

        case connector_sm_state_complete:
            result = sm_handle_complete(connector_ptr, sm_ptr, session);
            break;

        case connector_sm_state_error:
            result = sm_handle_error(connector_ptr, session);
            break;

        default:
            ASSERT(connector_false);
            break;
    }

    sm_verify_result(sm_ptr, &result);

error:
    return result;
}
