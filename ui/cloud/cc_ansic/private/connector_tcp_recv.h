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

STATIC connector_buffer_t * tcp_new_receive_packet(connector_data_t * const connector_ptr)
{
    connector_buffer_t * buffer_ptr;
    connector_buffer_t * packet = NULL;

    /* return an available packet for receiving data */
    buffer_ptr = connector_ptr->edp_data.receive_packet.free_packet_buffer;
    if (buffer_ptr != NULL)
    {
        packet = buffer_ptr;
        connector_ptr->edp_data.receive_packet.free_packet_buffer = buffer_ptr->next;
    }


    return packet;
}

STATIC void tcp_release_receive_packet(connector_data_t * const connector_ptr, connector_buffer_t const * const packet)
{
    ASSERT(packet != NULL);

    /* release a packet that is from tcp_new_receive_packet() */
    if (packet != NULL)
    {
        connector_buffer_t * const buffer_ptr = (connector_buffer_t *)packet;

        buffer_ptr->next = connector_ptr->edp_data.receive_packet.free_packet_buffer;
        connector_ptr->edp_data.receive_packet.free_packet_buffer = buffer_ptr;
    }
    return;
}


STATIC connector_callback_status_t tcp_receive_buffer(connector_data_t * const connector_ptr, uint8_t  * const buffer, size_t * const length)
{
    connector_callback_status_t status;
    connector_request_id_t request_id;

    /* Call callback to receive data from Device Cloud */
    request_id.network_request = connector_request_id_network_receive;

    {
        connector_network_receive_t read_data;

        read_data.handle = connector_ptr->edp_data.network_handle;
        read_data.buffer = buffer;
        read_data.bytes_available = *length;
        read_data.bytes_used = 0;


        status = connector_callback(connector_ptr->callback, connector_class_id_network_tcp, request_id, &read_data, connector_ptr->context);
        ASSERT(status != connector_callback_unrecognized);
        switch (status)
        {
        case connector_callback_unrecognized:
            status = connector_callback_abort;
            /* no break */
        case connector_callback_abort:
            connector_debug_line("tcp_receive_buffer: callback returns abort");
           goto done;
        case connector_callback_busy:
            *length = 0;
            break;
        case connector_callback_continue:
            *length = read_data.bytes_used;
            break;
        case connector_callback_error:
            edp_set_close_status(connector_ptr, connector_close_status_device_error);
            goto done;
        }

        if (read_data.bytes_used > 0 || connector_ptr->edp_data.keepalive.last_tx_received_time == 0)
        {
            /* Retain the "last (tx keepalive) message send" time. */
            if (get_system_time(connector_ptr, &connector_ptr->edp_data.keepalive.last_tx_received_time) != connector_working)
            {
                status = connector_callback_abort;
            }
            else
            {
                if (connector_ptr->edp_data.keepalive.miss_tx_count > 0)
                {
                    if (notify_status(connector_ptr->callback, connector_tcp_keepalive_restored, connector_ptr->context) != connector_working)
                        status = connector_callback_abort;
                    connector_ptr->edp_data.keepalive.miss_tx_count = 0;
                }
            }
            goto done;
        }
    }


    /* check Tx keepalive timing */
    if (GET_TX_KEEPALIVE_INTERVAL(connector_ptr) > 0)
    {
        unsigned long const tx_keepalive_interval = GET_TX_KEEPALIVE_INTERVAL(connector_ptr);

        unsigned long const wait_count = connector_ptr->edp_data.keepalive.miss_tx_count + UINT32_C(1);
        unsigned long const max_timeout = (tx_keepalive_interval * wait_count);

        if (!is_valid_timing_limit(connector_ptr, connector_ptr->edp_data.keepalive.last_tx_received_time, max_timeout))
        {
            /* notify callback we have missing a tx keep alive */
            if (notify_status(connector_ptr->callback, connector_tcp_keepalive_missed, connector_ptr->context) != connector_working)
            {
                status = connector_callback_abort;
                goto done;
            }
            connector_ptr->edp_data.keepalive.miss_tx_count++;
            if (connector_ptr->edp_data.keepalive.miss_tx_count == GET_WAIT_COUNT(connector_ptr))
            {
                /* consider a lost connection */
                if (notify_error_status(connector_ptr->callback, connector_class_id_network_tcp, request_id, connector_keepalive_error, connector_ptr->context) != connector_working)
                {
                    status = connector_callback_abort;
                    goto done;
                }

                connector_debug_line("connector_receive: keepalive fails");
                edp_set_close_status(connector_ptr, connector_close_status_no_keepalive);
                status = connector_callback_error;
            }
        }

    }


done:
    connector_ptr->edp_data.receive_packet.timeout = MAX_RECEIVE_TIMEOUT_IN_SECONDS;
    return status;
}


STATIC connector_callback_status_t tcp_receive_data_status(connector_data_t * const connector_ptr)
{
    connector_callback_status_t status = connector_callback_continue;

    /* send data if we have more data to send */
    if (connector_ptr->edp_data.receive_packet.bytes_received < connector_ptr->edp_data.receive_packet.total_length)
    {
        uint8_t * const buf = connector_ptr->edp_data.receive_packet.ptr + connector_ptr->edp_data.receive_packet.bytes_received;
        size_t length = connector_ptr->edp_data.receive_packet.total_length - connector_ptr->edp_data.receive_packet.bytes_received;
        status = tcp_receive_buffer(connector_ptr, buf, &length);

        if (status == connector_callback_continue)
        {
            connector_ptr->edp_data.receive_packet.bytes_received += length;
        }
        else if (status != connector_callback_busy)
        {
            goto done;
        }
    }

    if (connector_ptr->edp_data.receive_packet.bytes_received < connector_ptr->edp_data.receive_packet.total_length)
    {
        /* still more data */
        status = connector_callback_busy;
    }
done:
    return status;
}


STATIC connector_status_t tcp_receive_packet(connector_data_t * const connector_ptr, connector_buffer_t ** packet)
{
    enum {
        receive_packet_init,
        receive_packet_type,
        receive_packet_length,
        receive_packet_data,
        receive_packet_complete
    };

    connector_status_t result = connector_idle;

    *packet = NULL;

    ASSERT_GOTO(edp_get_edp_state(connector_ptr) != edp_communication_connect_to_cloud && edp_get_edp_state(connector_ptr) != edp_configuration_init, done);
    /*
     * Read the MT message type.
     *
     * For MT version 2, there are numerous message types. Some of these
     * messages require special handling, in that they may be legacy EDP
     * version response message varieties. These messages are sent by
     * device cloud that does not support MTv2. Since the client doesn't support
     * both MTv1 and MTv2 concurrently, an MTv2 client must terminate its
     * MT connection if it finds the Device Cloud to be incompatible insofar as
     * the MT version is concerned.
     *
     * We only accept messages of the expected types from the Device Cloud. Any
     * message other than an expected type is handled as an error, and an
     * error is returned to the caller. This must be done since any unknown
     * message type cannot be correctly parsed for length and discarded from
     * the input stream.
     */

    /* we have to read 3 times to get a complete packet.
     * 1. read message type
     * 2. read message length
     * 3. read actual message data
     *
     * So we use index == receive_packet_init to initialize the packet before starting reading.
     * When index == receive_packet_type, set to receive message type. After message type is received,
     * we must check valid message type.
     *
     * When index == receive_packet_length, set to receive message length.
     * When index == receive_packet_data, set to receive message data.
     * When index == receive_packet_complete, message data is completely received and
     * reset index = receive_packet_init and exit.
     *
     */
    while (connector_ptr->edp_data.receive_packet.index <= receive_packet_complete)
    {
        connector_callback_status_t status = connector_callback_continue;

        if (connector_ptr->edp_data.receive_packet.index != receive_packet_init)
        {   /* continue for any pending receive */
            status = tcp_receive_data_status(connector_ptr);

            switch (status)
            {
                case connector_callback_continue:
                    break;
                case connector_callback_busy:
                    result = (connector_ptr->edp_data.receive_packet.index <= receive_packet_length) ? connector_idle : connector_pending;
                    goto done;
                case connector_callback_error:
                    result = connector_unavailable;
                    goto done;
                case connector_callback_abort:
                    edp_set_close_status(connector_ptr, connector_close_status_abort);
                    result = connector_abort;
                    goto done;
                default:
                    ASSERT(connector_false);
            }
        }
        result = connector_pending;

        switch (connector_ptr->edp_data.receive_packet.index)
        {
        case receive_packet_init:
            /* initialize and setup packet for receive */
            connector_ptr->edp_data.receive_packet.packet_type = 0;
            connector_ptr->edp_data.receive_packet.packet_length = 0;
            connector_ptr->edp_data.receive_packet.bytes_received = 0;
            connector_ptr->edp_data.receive_packet.total_length = 0;
            connector_ptr->edp_data.receive_packet.index = 0;

            if (edp_get_active_state(connector_ptr) == connector_transport_open)
            {
                /* We are still in edp connection process.
                 * So we need to setup the packet for any message
                 * during edp connection process since no facility is
                 * running.
                 * Otherwise, we setup the packet in receive_packet_data index
                 * to receive actual data. Otherwise, we don't have actual
                 * data received but we need to be able to receive keepalive.
                 */
                connector_ptr->edp_data.receive_packet.data_packet = tcp_new_receive_packet(connector_ptr);
                if (connector_ptr->edp_data.receive_packet.data_packet == NULL)
                {
                    result = connector_idle;
                    goto done;
                }
            }

            connector_ptr->edp_data.receive_packet.index++;
            break;

        case receive_packet_type:
            /* set to read the message type */
            connector_ptr->edp_data.receive_packet.ptr = (uint8_t *)&connector_ptr->edp_data.receive_packet.packet_type;
            connector_ptr->edp_data.receive_packet.bytes_received = 0;
            connector_ptr->edp_data.receive_packet.total_length = sizeof connector_ptr->edp_data.receive_packet.packet_type;
            connector_ptr->edp_data.receive_packet.index++;
            break;

        case receive_packet_length:
        {
            /* Got message type let's get to message length.
             * So make sure we support the message type.
             * Then, set to read message length.
             */
            uint16_t type_val;

            type_val = FROM_BE16(connector_ptr->edp_data.receive_packet.packet_type);
            connector_ptr->edp_data.receive_packet.packet_type = type_val;

            switch (type_val)
            {
                /* Expected MTv2 message types... */
                case E_MSG_MT2_TYPE_VERSION_OK:
                    break;
                case E_MSG_MT2_TYPE_KA_KEEPALIVE:
                    break;
                case E_MSG_MT2_TYPE_PAYLOAD:
                    break;
                case E_MSG_MT2_TYPE_LEGACY_EDP_VER_RESP:
                case E_MSG_MT2_TYPE_VERSION_BAD:
                case E_MSG_MT2_TYPE_CLOUD_OVERLOAD:
                    connector_debug_line("tcp_receive_packet: unsupported or unexpected error type 0x%x", (unsigned) type_val);
                    edp_set_close_status(connector_ptr, connector_close_status_abort);
                    result = connector_abort;
                    goto done;
                /* Unexpected/unknown MTv2 message types... */
                case E_MSG_MT2_TYPE_VERSION:
                case E_MSG_MT2_TYPE_KA_RX_INTERVAL:
                case E_MSG_MT2_TYPE_KA_TX_INTERVAL:
                case E_MSG_MT2_TYPE_KA_WAIT:
                default:
                    /* Just tell caller we have unexpected packet message */
                    connector_debug_line("tcp_receive_packet: unsupported or unexpected error type 0x%x", (unsigned) type_val);
                    break;
             }

            /* set up to read message length */
            connector_ptr->edp_data.receive_packet.ptr = (uint8_t *)&connector_ptr->edp_data.receive_packet.packet_length;
            connector_ptr->edp_data.receive_packet.bytes_received = 0;
            connector_ptr->edp_data.receive_packet.total_length = sizeof connector_ptr->edp_data.receive_packet.packet_length;
            connector_ptr->edp_data.receive_packet.index++;
            break;
        }
        case receive_packet_data:
        {
            /* got packet length so set to read message data */
            uint16_t packet_length = FROM_BE16(connector_ptr->edp_data.receive_packet.packet_length);

            if (connector_ptr->edp_data.receive_packet.packet_type != E_MSG_MT2_TYPE_PAYLOAD)
            {
                /*
                 * For all but payload messages, the length field value should be
                 * zero, as there is no data accompanying the message. The MT
                 * messages to which this applies here are:
                 *    E_MSG_MT2_TYPE_VERSION_OK
                 *    E_MSG_MT2_TYPE_VERSION_BAD
                 *    E_MSG_MT2_TYPE_CLOUD_OVERLOAD
                 *    E_MSG_MT2_TYPE_KA_KEEPALIVE
                 */
                if (packet_length != 0)
                {
                    connector_debug_line("connector_get_receive_packet: Invalid payload");
                }
            }


            if (packet_length == 0)
            {
                /* set to complete data since no data to be read. */
                connector_ptr->edp_data.receive_packet.index = receive_packet_complete;
                connector_ptr->edp_data.receive_packet.packet_length = packet_length;
                connector_ptr->edp_data.receive_packet.total_length = packet_length;
            }
            else
            {
                /*
                 * Read the actual message data bytes into the packet buffer.
                 */
                ASSERT(packet_length <= (sizeof connector_ptr->edp_data.receive_packet.packet_buffer.buffer - PACKET_EDP_HEADER_SIZE));

                if (edp_get_active_state(connector_ptr) != connector_transport_open)
                {
                    /* We already setup data_packet in
                     * receive_packet_init index. So we setup data_packet when we
                     * are already established the EDP.
                     */
                    connector_ptr->edp_data.receive_packet.data_packet = tcp_new_receive_packet(connector_ptr);
                    if (connector_ptr->edp_data.receive_packet.data_packet == NULL)
                    {
                        goto done;
                    }
                }

                connector_ptr->edp_data.receive_packet.packet_length = packet_length;
                connector_ptr->edp_data.receive_packet.total_length = packet_length;

                connector_ptr->edp_data.receive_packet.ptr = GET_PACKET_DATA_POINTER(connector_ptr->edp_data.receive_packet.data_packet->buffer, PACKET_EDP_HEADER_SIZE);
                connector_ptr->edp_data.receive_packet.bytes_received = 0;
                connector_ptr->edp_data.receive_packet.index++;

            }
            break;
        }
        case receive_packet_complete:

            if (connector_ptr->edp_data.receive_packet.data_packet != NULL &&
                connector_ptr->edp_data.receive_packet.packet_type != E_MSG_MT2_TYPE_KA_KEEPALIVE)
            {
                uint8_t * edp_header = connector_ptr->edp_data.receive_packet.data_packet->buffer;
                /* got message data. Let's set edp header */
                message_store_be16(edp_header, type, connector_ptr->edp_data.receive_packet.packet_type);
                message_store_be16(edp_header, length, connector_ptr->edp_data.receive_packet.packet_length);
                *packet = connector_ptr->edp_data.receive_packet.data_packet;
                result = connector_working;
            }

            connector_ptr->edp_data.receive_packet.index = receive_packet_init;
            goto done;
        } /* switch */
    }

done:
    return result;
}

STATIC connector_status_t layer_facility_process(connector_data_t * const connector_ptr);

STATIC connector_status_t edp_tcp_receive_process(connector_data_t * connector_ptr)
{
enum {
    facility_receive_message,
    facility_process_message
};
    connector_status_t result;

    connector_buffer_t * packet_buffer = NULL;
    connector_bool_t done_packet = connector_true;
    connector_facility_t * fac_ptr;


    /* Facility layer is the layer that the Connector has fully established
     * communication with Device Cloud. It keeps waiting messages from Device Cloud
     * and passes it to the appropriate facility:
     * 1. waits message from Device Cloud
     * 2. parses message and passes it to the facility
     * 3. invokes facility to process message.
     */
    result = tcp_receive_packet(connector_ptr, &packet_buffer);

    if (result == connector_working)
    {
        uint8_t * packet ;
        uint8_t * edp_header;
        uint8_t * edp_protocol;

        ASSERT(packet_buffer != NULL);

        packet = packet_buffer->buffer;
        edp_header = packet;
        edp_protocol = packet + PACKET_EDP_HEADER_SIZE;

        /*
         * received packet format:
         *  ----------------------------------------------------------
         * |  0 - 1  |  2 - 3 |   4    |     5     |  6 - 7   |  8... |
         *  ----------------------------------------------------------
         * | Payload | length | coding | discovery | facility | Data  |
         * |   Type  |        | scheme |  payload  |          |       |
         * -----------------------------------------------------------
         * |    EDP Header    |           EDP Protocol                |
         * -----------------------------------------------------------
        */

        if (message_load_be16(edp_header, type) == E_MSG_MT2_TYPE_PAYLOAD)
        {
            uint16_t total_length = message_load_be16(edp_header, length);
            uint8_t const sec_code = message_load_u8(edp_protocol, sec_coding);
            uint8_t const payload = message_load_u8(edp_protocol, payload);

            /* currently we don't support any other security protocol */
            if (sec_code != SECURITY_PROTO_NONE)
            {
                /* This occurs when a bad EDP password has been used to authenticate */
                return connector_open_error;
            }
            ASSERT_GOTO(payload == DISC_OP_PAYLOAD, error);
            ASSERT_GOTO(total_length > PACKET_EDP_PROTOCOL_SIZE, error);

            connector_debug_line("edp_tcp_receive_process: receive data facility = 0x%04x", message_load_be16(edp_protocol, facility));
            /* adjust the length for facility process */
            {
                uint16_t const length = (uint16_t)(total_length - PACKET_EDP_PROTOCOL_SIZE);
                message_store_be16(edp_header, length, length);
            }
        }

        if (message_load_be16(edp_header, type) == E_MSG_MT2_TYPE_PAYLOAD)
        {
            uint16_t facility;
            uint8_t * edp_protocol = packet + PACKET_EDP_HEADER_SIZE;
            /* search facility
             *
             * Make sure the facility is not processing previous packet.
             */
            facility = message_load_be16(edp_protocol, facility);
            for (fac_ptr = connector_ptr->edp_data.facilities.list; fac_ptr != NULL; fac_ptr = fac_ptr->next)
            {
                if (fac_ptr->facility_num  == facility)
                {
                    if (fac_ptr->packet_buffer == NULL)
                    {
                        fac_ptr->packet_buffer = packet_buffer;
                        connector_ptr->edp_data.facilities.current = fac_ptr;
                        done_packet = connector_false;
                    }
                    else
                    {
                        /* we only have one receive buffer.
                         * Since a facility holds on the receive buffer,
                         * receive should not able to get a free receive buffer
                         * to receive any data
                         */
                        ASSERT(connector_false);
                    }
                    break;
                }
            }
        }
    }
    else if (result != connector_idle && result != connector_pending)
    {
        goto done;
    }

error:
    /* if we are done with packet, release it for another
     * receive_packet .
     */
    if (done_packet && packet_buffer != NULL)
    {
        tcp_release_receive_packet(connector_ptr, packet_buffer);
    }

    /* Let's process facility */
    result = layer_facility_process(connector_ptr);

done:
    if (result != connector_idle && result != connector_pending && result != connector_working && result != connector_active)
    {
        edp_set_active_state(connector_ptr, connector_transport_close);
        result = connector_working;
        goto done;
    }
    return result;
}

