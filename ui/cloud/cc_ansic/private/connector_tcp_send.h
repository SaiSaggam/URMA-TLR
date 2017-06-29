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

/* The data security coding schemes (a.k.a. encryption types)... */
#define SECURITY_PROTO_NONE          0x00 /* no encryption, no authentication */


/* Discovery layer opcodes */
#define DISC_OP_PAYLOAD       0
#define DISC_OP_DEVICETYPE    4
#define DISC_OP_INITCOMPLETE  5
#define DISC_OP_VENDOR_ID     6

#define tcp_is_send_active(connector_ptr)   connector_bool(connector_ptr->edp_data.send_packet.total_length > 0)

STATIC connector_status_t tcp_initiate_send_packet(connector_data_t * const connector_ptr, uint8_t * const edp_header,
                                                    size_t const length, uint16_t const type,
                                                    send_complete_cb_t send_complete_cb, void * const user_data)
{
    connector_status_t status = connector_working;

    /* Setup data to be sent. tcp_send_packet_process() will actually
     * send out the data.
     */

    ASSERT_GOTO(edp_header != NULL, done);
    ASSERT_GOTO(length <= UINT16_MAX, done);

    if (connector_ptr->edp_data.send_packet.total_length > 0)
    {
        /* connector_debug_line("tcp_initiate_send_packet: unable to trigger another send since previous data is still pending"); */
        status = connector_pending;
        goto done;
    }

    /*
     * MTv2 (and later)...
     * Packet format for MT version:
     *    ------------------------
     *   | 0 - 1 | 2 - 3  | 4 ... |
     *    ------------------------
     *   |  type | length |  data |
     *    ------------------------
     *   |   EDP Header   |
     *    ----------------
     *
     *
    */

    /* total bytes to be sent to Device Cloud (packet data length + the edp header length) */
    connector_ptr->edp_data.send_packet.total_length = length + PACKET_EDP_HEADER_SIZE;
    connector_ptr->edp_data.send_packet.ptr = edp_header;

    message_store_be16(edp_header, type, type);

    {
        uint16_t const length16 = (uint16_t) length;

        ASSERT(length <= UINT16_MAX);
        message_store_be16(edp_header, length, length16);
    }

    /* clear the actual number of bytes to be sent */
    connector_ptr->edp_data.send_packet.bytes_sent = 0;
    connector_ptr->edp_data.send_packet.complete_cb = send_complete_cb;
    connector_ptr->edp_data.send_packet.user_data = user_data;
done:
    return status;
}

STATIC connector_status_t tcp_initiate_send_facility_packet(connector_data_t * const connector_ptr, uint8_t * const edp_header,
                                                             size_t const length, uint16_t const facility,
                                                             send_complete_cb_t send_complete_cb, void * const user_data)
{
    uint8_t * const edp_protocol = edp_header + PACKET_EDP_HEADER_SIZE;


    /* this function is to set up a facility packet to be sent.
     * Setup security coding, discovery payload & facility.
     *
     * facility packet:
     *    -------------------------------------------------------------------------------------
     *   | 0 - 1 | 2 - 3  |         4            |        5          |  6 - 7   |     8...     |
     *    -------------------------------------------------------------------------------------
     *   |  type | length | data security coding | discovery payload | facility | facility data|
     *    -------------------------------------------------------------------------------------
     *   |   EDP Header   |                   EDP  Protocol                     |
     *    ----------------------------------------------------------------------
     */
    message_store_u8(edp_protocol, sec_coding, SECURITY_PROTO_NONE);
    message_store_u8(edp_protocol, payload, DISC_OP_PAYLOAD);
    message_store_be16(edp_protocol, facility, facility);


    return tcp_initiate_send_packet(connector_ptr, edp_header,
                                (length + PACKET_EDP_PROTOCOL_SIZE),
                                E_MSG_MT2_TYPE_PAYLOAD,
                                send_complete_cb,
                                user_data);
}

STATIC connector_callback_status_t tcp_send_buffer(connector_data_t * const connector_ptr, uint8_t * const buffer, size_t * const length)
{
    connector_callback_status_t status;
    connector_network_send_t send_data;
    connector_request_id_t request_id;

    send_data.buffer = buffer;
    send_data.bytes_available = *length;
    send_data.bytes_used = 0;
    send_data.handle = connector_ptr->edp_data.network_handle;

    request_id.network_request = connector_request_id_network_send;
    status = connector_callback(connector_ptr->callback, connector_class_id_network_tcp, request_id, &send_data, connector_ptr->context);
    ASSERT(status != connector_callback_unrecognized);
    switch (status)
    {
    case connector_callback_continue:
        *length = send_data.bytes_used;
        if (*length > 0)
        {
            /* Retain the "last (RX) message send" time. */
            if (get_system_time(connector_ptr, &connector_ptr->edp_data.keepalive.last_rx_sent_time) != connector_working)
            {
                status = connector_callback_abort;
            }
        }
        break;
    case connector_callback_busy:
        *length = 0;
        break;
    case connector_callback_unrecognized:
        status = connector_callback_abort;
        /* no break */
    case connector_callback_abort:
        break;
    case connector_callback_error:
        edp_set_close_status(connector_ptr, connector_close_status_device_error);
        break;
    }

    return status;
}

STATIC connector_status_t tcp_release_packet_buffer(connector_data_t * const connector_ptr, uint8_t const * const packet, connector_status_t const status, void * const user_data)
{
    /* this is called when the Connector is done sending or after tcp_get_packet_buffer()
     * is called to release connector_ptr->edp_data.send_packet.packet_buffer.buffer.
     *
     */
    UNUSED_PARAMETER(status);
    UNUSED_PARAMETER(packet);
    UNUSED_PARAMETER(user_data);

    ASSERT(connector_ptr->edp_data.send_packet.packet_buffer.buffer == packet);

    connector_ptr->edp_data.send_packet.packet_buffer.in_use = connector_false;

    return connector_working;
}
#if (defined CONNECTOR_DEBUG)
static unsigned int debug_count = 0;
#endif

STATIC uint8_t * tcp_get_packet_buffer(connector_data_t * const connector_ptr, uint16_t const facility, uint8_t ** data_ptr, size_t * data_length)
{
#define MIN_EDP_MESSAGE_SIZE    (PACKET_EDP_HEADER_SIZE +CLOUD_URL_LENGTH)
    uint8_t * packet = NULL;
    uint8_t * ptr = NULL;
    size_t length = 0;

    /* Return a pointer to caller to setup data to be sent to Device Cloud.
     * Must call tcp_release_packet_buffer() to release the buffer (pass
     * tcp_release_packet_buffer as complete_callback to tcp_initiate_send_packet()
     * or tcp_initiate_send_facility_packet().
     */


     /* make sure no send is pending */
    if ((connector_ptr->edp_data.send_packet.total_length == 0) &&
        (!connector_ptr->edp_data.send_packet.packet_buffer.in_use))
    {
        connector_ptr->edp_data.send_packet.packet_buffer.in_use = connector_true;

        packet = connector_ptr->edp_data.send_packet.packet_buffer.buffer;

        /* set ptr to the data portion */
        ptr = GET_PACKET_DATA_POINTER(packet, PACKET_EDP_HEADER_SIZE);

        if (facility != E_MSG_MT2_MSG_NUM)
        {
            /* set ptr to the data portion of facility packet */
            ptr += PACKET_EDP_PROTOCOL_SIZE;
        }

        {
            size_t const max_packet_size = sizeof connector_ptr->edp_data.send_packet.packet_buffer.buffer;
            size_t const header_size = (size_t)(ptr - packet);

            ASSERT(max_packet_size >= MIN_EDP_MESSAGE_SIZE);
            ASSERT(ptr > packet);
            length = max_packet_size - header_size;
        }
#if (defined CONNECTOR_DEBUG)
        debug_count = 0;
#endif

    }
#if (defined CONNECTOR_DEBUG)
    else
    {
        if (debug_count == 0)
           connector_debug_line("tcp_get packet buffer: send pending");

        debug_count++;
    }
#endif
    if (data_ptr != NULL)
    {
        *data_ptr = ptr;
    }

    if (data_length != NULL)
    {
        *data_length = length;
    }

    return packet;
}

STATIC connector_status_t tcp_rx_keepalive_process(connector_data_t * const connector_ptr)
{
   connector_status_t status = connector_idle;

    if (edp_get_active_state(connector_ptr) == connector_transport_open)
    {
        goto done;
    }

    /* Sends rx keepalive if keepalive timing is expired.
     *
     * last_rx_keepalive_time is last time we sent Rx keepalive.
     */
    if (is_valid_timing_limit(connector_ptr, connector_ptr->edp_data.keepalive.last_rx_sent_time, GET_RX_KEEPALIVE_INTERVAL(connector_ptr)))
    {
        /* not expired yet. no need to send rx keepalive */
        goto done;
    }

    connector_debug_line("tcp_rx_keepalive_process: time to send Rx keepalive");

    status = tcp_initiate_send_packet(connector_ptr, connector_ptr->edp_data.keepalive.send_rx_packet, 0, E_MSG_MT2_TYPE_KA_KEEPALIVE, NULL, NULL);

done:
    return status;
}

STATIC connector_status_t tcp_send_complete_callback(connector_data_t * const connector_ptr, connector_status_t status)
{
    connector_status_t result = connector_working;
    send_complete_cb_t callback = connector_ptr->edp_data.send_packet.complete_cb;

    if (callback != NULL)
    {
        connector_ptr->edp_data.send_packet.complete_cb = NULL;
        result = callback(connector_ptr, connector_ptr->edp_data.send_packet.ptr, status, connector_ptr->edp_data.send_packet.user_data);
        ASSERT(result != connector_pending);
    }

    return result;
}

STATIC connector_status_t edp_tcp_send_process(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_idle;

    /* if nothing needs to be sent, check whether we need to send rx keepalive */
    if (connector_ptr->edp_data.send_packet.total_length == 0)
    {

        result = tcp_rx_keepalive_process(connector_ptr);
    }

    if (connector_ptr->edp_data.send_packet.total_length > 0)
    {
        /* We have something to be sent */

        uint8_t * const buf = connector_ptr->edp_data.send_packet.ptr + connector_ptr->edp_data.send_packet.bytes_sent;
        size_t length = connector_ptr->edp_data.send_packet.total_length;

        connector_callback_status_t const status = tcp_send_buffer(connector_ptr, buf, &length);

        switch (status)
        {
            case connector_callback_continue:
                connector_ptr->edp_data.send_packet.total_length -= length;
                connector_ptr->edp_data.send_packet.bytes_sent += length;

                if (connector_ptr->edp_data.send_packet.total_length == 0)
                {   /* sent completed so let's call the complete callback */
                    result = tcp_send_complete_callback(connector_ptr, connector_success);
                }
                else if (connector_ptr->edp_data.send_packet.total_length > 0)
                {
                    result = connector_pending;
                }
                break;

            case connector_callback_busy:
                result = connector_pending;
                goto done;

            case connector_callback_error:
                edp_set_close_status(connector_ptr, connector_close_status_device_error);
                edp_set_active_state(connector_ptr, connector_transport_close);
                break;
            case connector_callback_abort:
                edp_set_close_status(connector_ptr, connector_close_status_abort);
                edp_set_active_state(connector_ptr, connector_transport_close);
                break;
            default:
                ASSERT(connector_false);
                break;
        }
    }

done:
    return result;

}


