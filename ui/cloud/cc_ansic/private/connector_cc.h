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

#define FAC_CC_DISCONNECT           0x00
#define FAC_CC_REDIRECT_TO_SDA      0x03
#define FAC_CC_REDIRECT_REPORT      0x04
#define FAC_CC_CONNECTION_REPORT    0x05
#define FAC_CC_REBOOT               0x06

#define CC_REDIRECT_URL_COUNT    2

typedef enum {
    cc_not_redirect,
    cc_redirect_success,
    cc_redirect_error
} cc_redirect_status_t;

typedef enum {
    ethernet_type = 1,
    ppp_over_modem_type
} cc_connection_type_t;

enum {
    cc_state_redirect_report,
    cc_state_connect_report,
    cc_state_close,
    cc_state_connect

};


typedef struct {
    char    origin_url[CLOUD_URL_LENGTH];
    size_t  origin_url_length;

    uint8_t * redirect;

    int     state;
    size_t redirect_count;
    uint8_t report_code;
} connector_cc_data_t;

STATIC void cc_init(connector_cc_data_t * const cc_ptr)
{
    cc_ptr->state = cc_state_redirect_report;
    cc_ptr->report_code = cc_not_redirect;
    cc_ptr->origin_url_length = 0;

}

STATIC connector_status_t edp_send_redirect_report(connector_data_t * const connector_ptr, connector_cc_data_t * const cc_ptr)
{

enum cc_redirect_report {
    field_define(redirect_report, opcode, uint8_t),
    field_define(redirect_report, code, uint8_t),
    field_define(redirect_report, message_length, uint8_t),
    /* TODO: Need to fix this enum if message_length > 0. For now, cloud always send message length =  0 */
    field_define(redirect_report, url_length, uint16_t),
    record_end(redirect_report)
};

    size_t const report_message_length = 0;
    size_t const redirect_report_header_size = record_bytes(redirect_report);

    connector_status_t result;
    uint8_t * edp_header;
    uint8_t * redirect_report;
    size_t url_length;
    size_t redirect_length;

    ASSERT(report_message_length == 0);

    connector_debug_line("Connection Control: send redirect_report");

    /* build and send redirect report
     *  ----------------------------------------------------
     * |   0    |    1   |    2    |   3...  | x1-x2  | ... |
     *  ----------------------------------------------------
     * | opcode | report | message | report  |  url   | url |
     * |        |  code  | length  | message | length |     |
     *  ----------------------------------------------------
     */

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_CC_NUM, &redirect_report, &redirect_length);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    message_store_u8(redirect_report, opcode, FAC_CC_REDIRECT_REPORT);
    message_store_u8(redirect_report, code, cc_ptr->report_code);
    {
        uint8_t const length8 = (uint8_t) report_message_length;

        ASSERT(report_message_length <= UCHAR_MAX);
        message_store_u8(redirect_report, message_length, length8);
    }

    url_length = cc_ptr->origin_url_length;

    if (url_length > 0)
    {
        char const prefix_url[] = URL_PREFIX;
        size_t const prefix_len = sizeof prefix_url -1;
        uint8_t * const redirect_report_url = redirect_report + redirect_report_header_size;

        ASSERT(redirect_length > (redirect_report_header_size + prefix_len + cc_ptr->origin_url_length));

        memcpy(redirect_report_url, prefix_url, prefix_len);
        memcpy((redirect_report_url + prefix_len), cc_ptr->origin_url, cc_ptr->origin_url_length);

        url_length += prefix_len;

    }
    {
        uint16_t const length16 = (uint16_t) url_length;

        ASSERT(url_length <= UINT16_MAX);
        message_store_be16(redirect_report, url_length, length16);
    }

    result = tcp_initiate_send_facility_packet(connector_ptr, edp_header,
                                           redirect_report_header_size + url_length,
                                           E_MSG_FAC_CC_NUM, tcp_release_packet_buffer, NULL);
done:
    return result;
}

STATIC void build_ip_addr(connector_data_t * const connector_ptr, uint8_t * ipv6_addr)
{
    connector_debug_print_buffer("Send device IP address", connector_ptr->edp_data.config.ip_addr, (int) connector_ptr->edp_data.config.ip_addr_length);

    if (connector_ptr->edp_data.config.ip_addr_length == CC_IPV6_ADDRESS_LENGTH)
    {
        /* Just copy IPv6 address */
        memcpy(ipv6_addr, connector_ptr->edp_data.config.ip_addr, CC_IPV6_ADDRESS_LENGTH);
    }
    else
    {
        /* good ipv4 addr. Map ipv4 to ipv6 address:
         * 10 all-zeros octets, 2 all-ones octets, and
         * then the ipv4 addr
         */

        static unsigned char const ipv6_padding_for_ipv4[] = {0,0,0,0,0,0,0,0,0,0,0xff,0xff};
        size_t const padding_length = sizeof ipv6_padding_for_ipv4/sizeof ipv6_padding_for_ipv4[0];

        ASSERT(padding_length == (CC_IPV6_ADDRESS_LENGTH - CC_IPV4_ADDRESS_LENGTH));

        memcpy(ipv6_addr, ipv6_padding_for_ipv4, padding_length);
        ipv6_addr += padding_length;
        memcpy(ipv6_addr, connector_ptr->edp_data.config.ip_addr, CC_IPV4_ADDRESS_LENGTH);
    }

    return;

}

STATIC connector_status_t send_connection_report(connector_data_t * const connector_ptr, connector_cc_data_t * const cc_ptr)
{
#define FAC_CC_CLIENTTYPE_REBOOTABLE_DEVICE 2

enum cc_connection_report {
    field_define(connection_report, opcode, uint8_t),
    field_define(connection_report, client_type, uint8_t),
    field_define(connection_report, ip1, uint32_t),
    field_define(connection_report, ip2, uint32_t),
    field_define(connection_report, ip3, uint32_t),
    field_define(connection_report, ip4, uint32_t),
    field_define(connection_report, connection_type, uint8_t),
    record_end(connection_report)
};

enum cc_connection_info {
    field_define(connection_info, link_speed, uint32_t),
    record_end(connection_info)
};
    connector_status_t result;
    uint8_t * edp_header;
    uint8_t * connection_report;
    size_t avail_length;
    size_t report_length;

    UNUSED_PARAMETER(cc_ptr);

    connector_debug_line("Connection Control: send connection report");

    /* Build Connection report
     *  -------------------------------------------------------
     * |   0    |    1   | 2  - 17 |     18      |  19 ...     |
     *  -------------------------------------------------------
     * | opcode | client |   IP    | connection  | connection  |
     * |        |  type  | address |    type     | information |
     *  -------------------------------------------------------
     *
     * 1. call callback to get ip address & build ip_address
     * 2. call callback to get & build connection type
     * 3. if connection type is LAN, call callback to get and build mac address for connection information
     * 4. if connection type is WAN, call callback to get and build link speed for connection information
     * 5. if connection type is WAN, call callback to get and build phone number for connection information
     */
    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_CC_NUM, &connection_report, &avail_length);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    {
        result = get_config_ip_addr(connector_ptr);

        if (result != connector_working)
        {
            goto done;
        }

        message_store_u8(connection_report, opcode, FAC_CC_CONNECTION_REPORT);
        message_store_u8(connection_report, client_type, FAC_CC_CLIENTTYPE_REBOOTABLE_DEVICE);
        report_length = field_named_data(connection_report, opcode, size)+field_named_data(connection_report, client_type, size);

        build_ip_addr(connector_ptr, (connection_report+report_length));
        report_length += CC_IPV6_ADDRESS_LENGTH;

    }

    {
        cc_connection_type_t const connection_type =  (connector_ptr->connection_type == connector_connection_type_lan) ? ethernet_type : ppp_over_modem_type;

        message_store_u8(connection_report, connection_type, connection_type);

        report_length += field_named_data(connection_report, connection_type, size);

        switch (connector_ptr->connection_type)
        {
        case connector_connection_type_lan:
            result = get_config_mac_addr(connector_ptr);
            if (result != connector_working)
            {
                goto done;

            }

            {
                uint8_t * report_ptr = connection_report + report_length;

                ASSERT(connector_ptr->mac_addr != NULL);
                /* build MAC address for LAN connection type */
                memcpy(report_ptr, connector_ptr->mac_addr, MAC_ADDR_LENGTH);
                report_length += MAC_ADDR_LENGTH;

                connector_debug_print_buffer("Sending MAC address", connector_ptr->mac_addr, MAC_ADDR_LENGTH);

                break;
            }
        case connector_connection_type_wan:
            {
                /* build Link speed for WAN connection type */
                uint8_t * connection_info = connection_report + record_bytes(connection_report);


#if (defined CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND)
                message_store_be32(connection_info, link_speed, CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND);
                connector_debug_line("send_connection_report: link_speed = %d", CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND);

#else
                message_store_be32(connection_info, link_speed, connector_ptr->link_speed);
                connector_debug_line("send_connection_report: link_speed = %d", connector_ptr->link_speed);
#endif
                report_length += field_named_data(connection_info, link_speed, size);

                {
                    /* build phone number for WAN connection type */

#if (defined CONNECTOR_WAN_PHONE_NUMBER_DIALED)
                    char const phone[] = CONNECTOR_WAN_PHONE_NUMBER_DIALED;
                    size_t const length = sizeof phone -1;
#else
                    size_t length = connector_ptr->phone_dialed_length;
                    char const * const phone = connector_ptr->phone_dialed;
#endif
                    connector_debug_line("send_connection_report: phone number = %.*s", (int)length, phone);
                    memcpy(connection_report+report_length, phone, length);
                    report_length += length;
                    *(connection_report + report_length) = 0x0;
                    report_length++;
                }
            }
        }
    }

    ASSERT(avail_length > report_length);
    result = tcp_initiate_send_facility_packet(connector_ptr, edp_header, report_length, E_MSG_FAC_CC_NUM, tcp_release_packet_buffer, NULL);

done:
    return result;
}


static connector_status_t  process_redirect(connector_data_t * const connector_ptr, connector_cc_data_t * const cc_ptr, uint8_t const * const packet)
{
enum cc_redirect {
    field_define(redirect, opcode, uint8_t),
    field_define(redirect, count, uint8_t),
    record_end(redirect)
};


    connector_status_t result = connector_working;
    uint8_t * redirect;

    connector_debug_line("process_redirect:  redirect to new destination");
    /* Redirect to new destination:
     * The connector will close connection and connect to new destination. If connect
     * to new destination fails, this function will returns SUCCESS to try
     * connecting to the original destination.
     */

    /*
     * parse new destinations
     *  --------------------------------------------------------------------
     * |   0    |     1     |    2 - 3    |  4 ... |              |         |
     *  --------------------------------------------------------------------
     * | opcode | URL count | URL 1 Length|  URL 1 | URL 2 Length |  URL 2  |
     *  --------------------------------------------------------------------
     *
    */
    redirect = GET_PACKET_DATA_POINTER(packet, PACKET_EDP_FACILITY_SIZE);
    cc_ptr->redirect_count = message_load_u8(redirect, count);

    if (cc_ptr->redirect_count == 0)
    {   /* nothing to redirect */
        connector_debug_line("cc_process_redirect: redirect with no url specified");
        result = connector_idle;
        goto done;
    }

    if (cc_ptr->redirect_count > CC_REDIRECT_URL_COUNT)
    {
        cc_ptr->redirect_count = CC_REDIRECT_URL_COUNT;
    }


    /* let's start parsing url length and url string */
    redirect += record_bytes(redirect);
    cc_ptr->redirect = redirect;

    /* save original Device Cloud url that we connected before */
    memcpy(cc_ptr->origin_url, connector_ptr->edp_data.config.cloud_url, connector_ptr->edp_data.config.cloud_url_length);
    cc_ptr->origin_url_length = connector_ptr->edp_data.config.cloud_url_length;

    /* Close the connection before parsing new destination url */
    edp_set_active_state(connector_ptr, connector_transport_redirect);
    cc_ptr->state = cc_state_close;

done:
    return result;
}

STATIC connector_status_t cc_process(connector_data_t * const connector_ptr, void * const facility_data,
                                          uint8_t * const packet, unsigned int * const receive_timeout)
{
    connector_status_t result = connector_idle;

    UNUSED_PARAMETER(receive_timeout);

    /* process incoming message from Device Cloud for Connection Control facility */
    if (packet != NULL)
    {
        uint8_t opcode;
        uint8_t * ptr;
        connector_cc_data_t * cc_ptr = facility_data;

        /* get the DATA portion of the packet */
        ptr = GET_PACKET_DATA_POINTER(packet, PACKET_EDP_FACILITY_SIZE);
        opcode = *ptr;

        switch (opcode)
        {

        case FAC_CC_DISCONNECT:
        {

            edp_set_close_status(connector_ptr, connector_close_status_cloud_disconnected);
            edp_set_active_state(connector_ptr, connector_transport_close);
            result = connector_working;
            break;
        }
        case FAC_CC_REDIRECT_TO_SDA:
            result = process_redirect(connector_ptr, cc_ptr, packet);
            break;
        case FAC_CC_REBOOT:
            result = connector_reboot(connector_ptr);
            break;
        default:
            connector_debug_line("connector_cc_process: unsupported opcode %u", opcode);
            break;
        }
    }

    return result;
}
STATIC connector_status_t cc_discovery(connector_data_t * const connector_ptr, void * const facility_data,
                                            uint8_t * const packet, unsigned int * receive_timeout)
{
    connector_status_t result = connector_idle;
    connector_cc_data_t * cc_ptr = facility_data;

    UNUSED_PARAMETER(receive_timeout);
    UNUSED_PARAMETER(packet);
    /* Connection control facility needs to send redirect and
     * connection reports on discovery layer.
     */
    if (cc_ptr->state == cc_state_redirect_report)
    {
        result = edp_send_redirect_report(connector_ptr, cc_ptr);
        if (result == connector_working)
        {
            result = connector_pending; /* still not complete cc_discovery */
            cc_ptr->report_code = cc_not_redirect;
            cc_ptr->origin_url_length = 0;
            cc_ptr->state = cc_state_connect_report;
        }
    }
    else if (cc_ptr->state == cc_state_connect_report)
    {
        result = send_connection_report(connector_ptr, cc_ptr);
    }

    return result;
}

STATIC connector_status_t connector_facility_cc_cleanup(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_cc_data_t * const cc_ptr = get_facility_data(connector_ptr, E_MSG_FAC_CC_NUM);
    if (cc_ptr != NULL && cc_ptr->state != cc_state_close)
    {
        cc_ptr->state = cc_state_redirect_report;
        cc_ptr->report_code = cc_not_redirect;
        cc_ptr->origin_url_length = 0;
        cc_ptr->redirect_count = 0;
    }

    return result;
}

STATIC connector_status_t connector_facility_cc_delete(connector_data_t * const connector_ptr)
{
    return del_facility_data(connector_ptr, E_MSG_FAC_CC_NUM);
}

STATIC connector_status_t connector_facility_cc_init(connector_data_t * const connector_ptr, unsigned int const facility_index)
{
    connector_status_t result = connector_working;
    connector_cc_data_t * cc_ptr;

    /* Add Connection control facility
     *
     * Make sure connection control is not already created. If Connection
     * control facility is already created, we probably reconnect to Device Cloud
     * so just need to reset to initial state.
     *
     */
    cc_ptr = get_facility_data(connector_ptr, E_MSG_FAC_CC_NUM);
    if (cc_ptr == NULL)
    {
        void * ptr;

        result = add_facility_data(connector_ptr, facility_index, E_MSG_FAC_CC_NUM, &ptr, sizeof *cc_ptr);

        if (result != connector_working || ptr == NULL)
        {
            goto done;
        }
        cc_ptr = ptr;

        cc_init(cc_ptr);
    }

done:
    return result;
}

STATIC connector_status_t edp_redirect_process(connector_data_t * const connector_ptr)
{

    connector_status_t result = connector_working;


    connector_cc_data_t * const cc_ptr = get_facility_data(connector_ptr, E_MSG_FAC_CC_NUM);
    ASSERT(cc_ptr != NULL);

    switch (cc_ptr->state)
    {
    case cc_state_close:

        /* set the reason for closing */
        edp_set_close_status(connector_ptr, connector_close_status_cloud_redirected);

        result = tcp_close_cloud(connector_ptr);
        if (result == connector_working)
        {
            if (edp_get_active_state(connector_ptr) == connector_transport_idle)
            {
                /* continue to redirect state */
                edp_set_active_state(connector_ptr, connector_transport_redirect);
                cc_ptr->state = cc_state_connect;
                goto done;
            }
        }

        if (result != connector_pending && result != connector_abort && result != connector_device_terminated)
        {
            /* reset initial value */
            cc_init(cc_ptr);
        }
        break;

    case cc_state_connect:

        /* let parse url length and url string */
        if (cc_ptr->redirect_count > 0)
        {
            enum cc_redirect_url {
                field_define(redirect, url_length, uint16_t),
                record_end(redirect_url_length)
            };

            uint8_t * redirect = cc_ptr->redirect;
            char const * cloud_url;
            uint16_t cloud_url_length;

            uint16_t const url_length = message_load_be16(redirect, url_length);
            redirect += record_bytes(redirect_url_length);

            {
                size_t const prefix_len = sizeof URL_PREFIX -1;

                cloud_url = (char const *)redirect;
                cloud_url_length = url_length;

                if (memcmp(cloud_url, URL_PREFIX, prefix_len) == 0)
                {
                    cloud_url += prefix_len;
                    cloud_url_length -= (uint16_t) prefix_len;
                }

                ASSERT(cloud_url_length < sizeof connector_ptr->edp_data.config.cloud_url);
                ASSERT(cloud_url_length != 0);

                memcpy(connector_ptr->edp_data.config.cloud_url, cloud_url, cloud_url_length);
                connector_ptr->edp_data.config.cloud_url_length = cloud_url_length;
                connector_ptr->edp_data.config.cloud_url[cloud_url_length] = 0x0;

                result = connect_to_cloud(connector_ptr, connector_ptr->edp_data.config.cloud_url);
            }

            if (result == connector_working)
            {
                cc_ptr->report_code = cc_redirect_success;
                cc_ptr->state = cc_state_redirect_report;
                edp_set_active_state(connector_ptr, connector_transport_open);
                edp_set_edp_state(connector_ptr, edp_communication_send_version);

                connector_ptr->edp_data.send_packet.packet_buffer.in_use = connector_false;
                connector_ptr->edp_data.receive_packet.packet_buffer.in_use = connector_false;
                connector_ptr->edp_data.receive_packet.packet_buffer.next = NULL;
                connector_ptr->edp_data.receive_packet.free_packet_buffer = &connector_ptr->edp_data.receive_packet.packet_buffer;
            }
            else if (result != connector_pending && result != connector_abort)
            {
                /* connect to next redirect url */
                result = connector_pending;
                cc_ptr->redirect = redirect;
                cc_ptr->redirect += url_length;
                cc_ptr->redirect_count--;
            }
        }
        else
        {
            cc_ptr->report_code = cc_redirect_error;
            cc_ptr->state = cc_state_redirect_report;
            edp_set_active_state(connector_ptr, connector_transport_open);
            /* restore the original url */
            edp_get_device_cloud(connector_ptr);
        }
        break;
    }

done:
    return result;
}
