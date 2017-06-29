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

STATIC connector_status_t connect_to_cloud(connector_data_t * const connector_ptr, char const * cloud_url)
{
    connector_status_t result = connector_working;

    connector_callback_status_t status;
    connector_network_open_t open_data;

    connector_request_id_t request_id;

    open_data.device_cloud.url = cloud_url;
    open_data.handle = connector_ptr->edp_data.network_handle;

    request_id.network_request = connector_request_id_network_open;
    status = connector_callback(connector_ptr->callback, connector_class_id_network_tcp, request_id, &open_data, connector_ptr->context);
    ASSERT(status != connector_callback_unrecognized);
    switch (status)
    {
    case connector_callback_continue:
        connector_ptr->edp_data.network_handle = open_data.handle;
        break;
    case  connector_callback_abort:
        result = connector_abort;
        break;

    case connector_callback_unrecognized:
        result = connector_unavailable;
        break;

    case connector_callback_error:
        connector_ptr->edp_data.network_handle = CONNECTOR_NETWORK_HANDLE_NOT_INITIALIZED;
        result = connector_open_error;
        break;
    case connector_callback_busy:
        connector_ptr->edp_data.network_handle = open_data.handle; /* Keep user handle */
        result = connector_pending;
        break;
    }

    return result;
}

STATIC connector_status_t send_version(connector_data_t * connector_ptr, uint16_t const type, uint32_t const version)
{
enum edp_version {
    field_define(edp_version, version, uint32_t),
    record_end(edp_version)
};
    size_t const version_message_size = record_bytes(edp_version);

    connector_status_t result = connector_pending;
    uint8_t * edp_version;
    uint8_t * packet;

    /*
     *  version packet format:
     *  -------------------------
     * | 0 - 1 | 2 - 3 |  4 - 7  |
     *  -------------------------
     * |  EDP header   | version |
     *  -------------------------
    */

    packet = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &edp_version, NULL);
    if (packet != NULL)
    {
        message_store_be32(edp_version, version, version);

        result = tcp_initiate_send_packet(connector_ptr, packet, version_message_size,
                                    type,
                                    tcp_release_packet_buffer,
                                    NULL);
    }

    return result;
}

STATIC connector_status_t receive_edp_version(connector_data_t * const connector_ptr)
{
#define CLOUD_OVERLOAD_RESPONSE    0x02

    connector_status_t result;
    uint8_t * ptr;
    connector_buffer_t * packet_buffer;

    result = tcp_receive_packet(connector_ptr, &packet_buffer);

    if (result == connector_working && packet_buffer != NULL)
    {
        uint16_t type;
        uint8_t  response_code;
        uint8_t * const packet = packet_buffer->buffer;

        connector_debug_line("Receive Mt version");
        /*
         * MT version response packet format:
         *  ---------------
         * |0 - 1 |  2 - 3 |
         *  ---------------
         * | Type | length |
         *  ---------------
         *
         */

        ptr = GET_PACKET_DATA_POINTER(packet, PACKET_EDP_HEADER_SIZE);
        {
            uint8_t * edp_header = packet;
            type = message_load_be16(edp_header, type);
        }
        response_code = *ptr;

        tcp_release_receive_packet(connector_ptr, packet_buffer);
        if (type != E_MSG_MT2_TYPE_VERSION_OK)
        {
            /*
             * The received message is not acceptable.
             */
            switch (type)
            {
                /* Expected MTv2 message types... */
                case E_MSG_MT2_TYPE_LEGACY_EDP_VER_RESP:
                    if (response_code == CLOUD_OVERLOAD_RESPONSE) {
                        connector_debug_line("receive_edp_version: MTv2 legacy Cloud responded with overload msg");
                    }
                    else {
                        connector_debug_line("receive_edp_version: MTv2 legacy bad version");
                    }
                    break;
                case E_MSG_MT2_TYPE_VERSION_BAD:
                    connector_debug_line("receive_edp_version: bad version");
                    break;
                case E_MSG_MT2_TYPE_CLOUD_OVERLOAD:
                    connector_debug_line("receive_edp_version: Device Cloud responded with overload msg");
                    break;
                /* Unexpected/unknown MTv2 message types... */
                default:
                    connector_debug_line("receive_edp_version: unknown MTv2 message");
            }
            /* mt version error. let's notify user.
             *
             * ignore error status callback return value since Device Cloud
             * will close the connection.
             */
            result = connector_abort;
        }
    }
    else if (result == connector_idle) result = connector_pending;

    return result;
}

STATIC size_t build_keepalive_param(uint8_t * const edp_header, uint16_t const type, uint16_t const value)
{
enum edp_keepalive{
    field_define(edp_keepalive, interval, uint16_t),
    record_end(edp_keepalive)
};

    size_t const keepalive_size = record_bytes(edp_keepalive);
    uint8_t * edp_keepalive;

    edp_keepalive = GET_PACKET_DATA_POINTER(edp_header, PACKET_EDP_HEADER_SIZE);
    message_store_be16(edp_header, type, type);
    {
        uint16_t const keepalive_size16 = (uint16_t) keepalive_size;

        ASSERT(keepalive_size <= UINT16_MAX);
        message_store_be16(edp_header, length, keepalive_size16);
    }
    message_store_be16(edp_keepalive, interval, value);


    return (PACKET_EDP_HEADER_SIZE + keepalive_size); /* return count of bytes added to buffer */
}

STATIC connector_status_t send_keepalive(connector_data_t * const connector_ptr)
{
    connector_status_t result;

    uint8_t * ptr;
    uint8_t * packet;
    uint8_t * start_ptr;
    size_t i;

    struct {
        uint16_t type;
        uint16_t value;
    } keepalive_parameters[3];

#if !(defined CONNECTOR_TX_KEEPALIVE_IN_SECONDS)
    result = get_config_keepalive(connector_ptr, connector_request_id_config_tx_keepalive);
    COND_ELSE_GOTO(result == connector_working, done);
#endif

#if !(defined CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
    result = get_config_keepalive(connector_ptr, connector_request_id_config_rx_keepalive);
    COND_ELSE_GOTO(result == connector_working, done);
#endif

#if !(defined CONNECTOR_WAIT_COUNT)
    result = get_config_wait_count(connector_ptr);
    COND_ELSE_GOTO(result == connector_working, done);
#endif

    keepalive_parameters[0].type = E_MSG_MT2_TYPE_KA_RX_INTERVAL;
    keepalive_parameters[0].value = GET_RX_KEEPALIVE_INTERVAL(connector_ptr);

    keepalive_parameters[1].type = E_MSG_MT2_TYPE_KA_TX_INTERVAL;
    keepalive_parameters[1].value = GET_TX_KEEPALIVE_INTERVAL(connector_ptr);

    keepalive_parameters[2].type = E_MSG_MT2_TYPE_KA_WAIT;
    keepalive_parameters[2].value = GET_WAIT_COUNT(connector_ptr);

    connector_debug_line("Send keepalive params");
    connector_debug_line("Rx keepalive parameter = %d", keepalive_parameters[0].value);
    connector_debug_line("Tx keepalive parameter = %d", keepalive_parameters[1].value);
    connector_debug_line("Wait Count parameter = %d", keepalive_parameters[2].value);

    packet = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &ptr, NULL);
    if (packet == NULL)
    {
        result = connector_pending;
        goto  done;
    }
    else
    {
        result = connector_working;
    }

    ptr = (uint8_t *)packet;
    start_ptr = ptr;

    for (i=0; i < ARRAY_SIZE(keepalive_parameters); i++)
    {
        size_t const len = build_keepalive_param(ptr, keepalive_parameters[i].type, keepalive_parameters[i].value);
        ptr += len;
    }
    /* Setting the total_length will enable tcp_send_packet_process.
     * Clear length to 0 for actual length that has been sent.
     */
    {
        size_t const total_packet_length = (size_t)(ptr - start_ptr);
        ASSERT(ptr > start_ptr);
        connector_ptr->edp_data.send_packet.total_length = total_packet_length;
        connector_ptr->edp_data.send_packet.bytes_sent = 0;
        connector_ptr->edp_data.send_packet.ptr = packet;
        connector_ptr->edp_data.send_packet.complete_cb = tcp_release_packet_buffer;
    }

done:
    return result;
}

STATIC connector_status_t receive_protocol_version(connector_data_t * const connector_ptr)
{
    enum {
        initialization_version_response_acceptable,
        initialization_version_response_unacceptable,
        initialization_version_response_unavailable
    };

    connector_status_t result;
    connector_buffer_t * packet_buffer;

    result = tcp_receive_packet(connector_ptr, &packet_buffer);

    if (result == connector_working)
    {
        uint8_t * edp_header;

        ASSERT(packet_buffer != NULL);

        edp_header = packet_buffer->buffer;

        connector_debug_line("Receive protocol version");
        /*
         *  version response packet format:
         *  ---------------------------------------
         * |    0 - 1     |  2 - 3 |     4         |
         *  ---------------------------------------
         * | Payload Type | length | response code |
         *  ----------------------------------------
        */
        /*
         * Empty data packet
         */
        if (message_load_be16(edp_header, length) > 0)
        {
            uint8_t * const response_code = GET_PACKET_DATA_POINTER(edp_header, PACKET_EDP_HEADER_SIZE);
            /* Parse the version response (0 = version response ok).
             * If the protocol version number was not acceptable to Device Cloud,
             * tell the application.
             */
            if (*response_code != initialization_version_response_acceptable)
            {
                edp_set_close_status(connector_ptr, connector_close_status_abort);
                connector_debug_line("receive_protocol_version:version is not accepted by Device Cloud");
                edp_set_active_state(connector_ptr, connector_transport_close);
            }
        }

        tcp_release_receive_packet(connector_ptr, packet_buffer);
    }
    else if (result == connector_idle) result = connector_pending;

    return result;
}

STATIC connector_status_t send_identity_verification(connector_data_t * const connector_ptr)
{
    /* The security operations other than encryption... */
    #define SECURITY_OPER_IDENT_FORM     UINT32_C(0x80)

    /* Identity verification form codes... */
    #define SECURITY_IDENT_FORM_SIMPLE   UINT32_C(0x00) /* simple verification */
    #define SECURITY_IDENT_FORM_PASSWORD UINT32_C(0x02) /* password verification */

    enum edp_security {
        field_define(edp_security, opcode, uint8_t),
        field_define(edp_security, identity, uint8_t),
        record_end(edp_security)
    };

    connector_status_t result;
    size_t const edp_security_size = record_bytes(edp_security);
    uint8_t * edp_security;
    uint8_t * edp_header;
    uint8_t identity = SECURITY_IDENT_FORM_SIMPLE;

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &edp_security, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }


    connector_debug_line("Send identity verification");

    /*
     * packet format:
     *  -----------------------------------------------------
     * | 0 - 1 | 2 - 3 |     4       |     5                 |
     *  -----------------------------------------------------
     * |   EDP header  | Form opcode | identity verification |
     *  -----------------------------------------------------
    */
    message_store_u8(edp_security, opcode, SECURITY_OPER_IDENT_FORM);

    {

#if (defined CONNECTOR_IDENTITY_VERIFICATION)
        connector_identity_verification_t const identity_verification = CONNECTOR_IDENTITY_VERIFICATION;
#else
        connector_identity_verification_t const identity_verification = connector_ptr->edp_data.config.identity_verification;
#endif

        switch (identity_verification)
        {
        case connector_identity_verification_simple:
            identity = SECURITY_IDENT_FORM_SIMPLE;
            break;
        case connector_identity_verification_password:
            identity = SECURITY_IDENT_FORM_PASSWORD;
            break;
        }
    }

    message_store_u8(edp_security, identity, identity);

    result = tcp_initiate_send_packet(connector_ptr, edp_header, edp_security_size,
                                E_MSG_MT2_TYPE_PAYLOAD,
                                tcp_release_packet_buffer,
                                NULL);

done:
    return result;
}

STATIC connector_status_t send_provisioning(connector_data_t * const connector_ptr)
{
    #define SECURITY_OPER_PROVISION_ID      0x89
    #define PROVISION_ID_LENGTH             4

    enum edp_device_id {
        field_define(edp_provision_id, opcode, uint8_t),
        field_define(edp_provision_id, provision_id, uint32_t),
        record_end(edp_provision_id)
    };

    connector_status_t result;
    uint8_t * edp_header;
    uint8_t * edp_provision_id;
    size_t const provision_id_message_size = record_bytes(edp_provision_id);

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &edp_provision_id, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    /*
     * packet format:
     *  -------------------------------------------------------
     * | 0 - 1 | 2 - 3 |        4                  |   5 - 8   |
     *  -------------------------------------------------------
     * |   EDP header  | Provision ID opcode | Provision ID ID |
     *  -------------------------------------------------------
    */
    message_store_u8(edp_provision_id, opcode, SECURITY_OPER_PROVISION_ID);

#if !(defined CONNECTOR_VENDOR_ID)
    ASSERT(connector_ptr->edp_data.config.vendor_id != 0);
    message_store_be32(edp_provision_id, provision_id, connector_ptr->edp_data.config.vendor_id);
#else
    message_store_be32(edp_provision_id, provision_id, CONNECTOR_VENDOR_ID);
#endif

    result = tcp_initiate_send_packet(connector_ptr, edp_header, provision_id_message_size,
                                E_MSG_MT2_TYPE_PAYLOAD, tcp_release_packet_buffer,
                                NULL);
    done:
    return result;
}

STATIC connector_status_t send_device_id(connector_data_t * const connector_ptr)
{
    #define SECURITY_OPER_DEVICE_ID      0x81

    enum edp_device_id {
        field_define(edp_device_id, opcode, uint8_t),
        field_define_array(edp_device_id, id, DEVICE_ID_LENGTH),
        record_end(edp_device_id)
    };

    connector_status_t result;
    uint8_t * edp_header;
    uint8_t * edp_device_id;
    size_t const device_id_message_size = record_bytes(edp_device_id);

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &edp_device_id, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    /*
     * packet format:
     *  ----------------------------------------------
     * | 0 - 1 | 2 - 3 |        4         |  5 - 20   |
     *  ----------------------------------------------
     * |   EDP header  | Device ID opcode | Device ID |
     *  ----------------------------------------------
    */
    message_store_u8(edp_device_id, opcode, SECURITY_OPER_DEVICE_ID);

    ASSERT(connector_ptr->device_id != NULL);
    memcpy((edp_device_id + field_named_data(edp_device_id, id, offset)), connector_ptr->device_id, DEVICE_ID_LENGTH);

    connector_debug_print_buffer("Sending Device ID", connector_ptr->device_id, DEVICE_ID_LENGTH);

    result = tcp_initiate_send_packet(connector_ptr, edp_header, device_id_message_size,
                                E_MSG_MT2_TYPE_PAYLOAD, tcp_release_packet_buffer,
                                NULL);
done:
    return result;
}

STATIC connector_status_t receive_device_id(connector_data_t * const connector_ptr)
{
    connector_status_t result;
    connector_buffer_t * packet_buffer;

    result = tcp_receive_packet(connector_ptr, &packet_buffer);

    if (result == connector_working)
    {
        uint8_t * edp_header;

        ASSERT(packet_buffer != NULL);

        edp_header = packet_buffer->buffer;

        /*
         * packet format:
         *  ----------------------------------------------
         * | 0 - 1 | 2 - 3 |        4         |  5 - 20   |
         *  ----------------------------------------------
         * |   EDP header  | Device ID opcode | Device ID |
         *  ----------------------------------------------
        */
        {
            uint8_t * const opcode = GET_PACKET_DATA_POINTER(edp_header, PACKET_EDP_HEADER_SIZE);

            if (*opcode != SECURITY_OPER_DEVICE_ID)
            {
                /* Device ID error. Notify user. */
                result = connector_abort;
            }
            else
            {
                uint8_t * const device_id = opcode + sizeof(*opcode);
                connector_request_id_t request_id;
                connector_callback_status_t status;
                connector_config_pointer_data_t device_id_data;

                /* Update internal device_id, needed for future connections */
                memcpy(connector_ptr->device_id, device_id, DEVICE_ID_LENGTH);
                /* Call user function to save the provisioned Device ID */
                device_id_data.bytes_required = DEVICE_ID_LENGTH;
                device_id_data.data = device_id;

                connector_debug_print_buffer("Received Device ID", device_id, DEVICE_ID_LENGTH);

                request_id.config_request = connector_request_id_config_set_device_id;
                status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &device_id_data, connector_ptr->context);
                switch (status)
                {
                case connector_callback_continue:
                    break;
                case connector_callback_abort:
                    result = connector_abort;
                    break;
                case connector_callback_unrecognized:
                case connector_callback_error:
                    result = connector_device_error;
                    break;
                case connector_callback_busy:
                    result = connector_pending;
                    break;
                }
            }
        }

        tcp_release_receive_packet(connector_ptr, packet_buffer);
    }
    else if (result == connector_idle) result = connector_pending;

    return result;
}

STATIC connector_status_t send_cloud_url(connector_data_t * const connector_ptr)
{
    #define SECURITY_OPER_URL            UINT32_C(0x86)

    /*
     * packet format:
     *  ------------------------------------------------
     * | 0 - 1 | 2 - 3 |    4       |   5 - 6    | 7... |
     *  ------------------------------------------------
     * |   EDP Header  | URL opcode | URL length | URL  |
     *  ------------------------------------------------
    */
    enum edp_cloud_url {
        field_define(edp_cloud_url, opcode, uint8_t),
        field_define(edp_cloud_url, url_length, uint16_t),
        record_end(edp_cloud_url)
    };
    connector_status_t result;
    uint8_t * edp_header;

    char * cloud_url = connector_ptr->edp_data.config.cloud_url;
    uint8_t * edp_cloud_url;
    uint8_t * start_ptr;

    static char const url_prefix[] = URL_PREFIX;
    static size_t const prefix_length = sizeof url_prefix -1;

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &start_ptr, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    edp_cloud_url = start_ptr;

    connector_debug_line("Send Device Cloud url = %.*s", (int)connector_ptr->edp_data.config.cloud_url_length, connector_ptr->edp_data.config.cloud_url);

    message_store_u8(edp_cloud_url, opcode, SECURITY_OPER_URL);

    {
        size_t const len = connector_ptr->edp_data.config.cloud_url_length + prefix_length;
        uint16_t const len16 = (uint16_t) len;

        ASSERT(len <= UINT16_MAX);
        message_store_be16(edp_cloud_url, url_length, len16);
    }

    edp_cloud_url += record_bytes(edp_cloud_url);
    memcpy(edp_cloud_url, url_prefix, prefix_length);
    edp_cloud_url += prefix_length;

    memcpy(edp_cloud_url, cloud_url, connector_ptr->edp_data.config.cloud_url_length);
    edp_cloud_url += connector_ptr->edp_data.config.cloud_url_length;

    {
        size_t const length = (size_t)(edp_cloud_url-start_ptr);
        ASSERT(edp_cloud_url > start_ptr);

        result = tcp_initiate_send_packet(connector_ptr, edp_header, length,
                                E_MSG_MT2_TYPE_PAYLOAD,
                                tcp_release_packet_buffer,
                                NULL);
    }
done:
    return result;
}

STATIC connector_status_t send_password(connector_data_t * const connector_ptr)
{
    #define SECURITY_OPER_PASSWORD            0x88

    /*
     * packet format:
     *  -----------------------------------------------------
     * | 0 - 1 | 2 - 3 |    4            |   5    | 7...     |
     *  -----------------------------------------------------
     * |   EDP Header  | password opcode | length | password |
     *  -----------------------------------------------------
    */
    enum edp_password {
        field_define(edp_password, opcode, uint8_t),
        field_define(edp_password, password_length, uint16_t),
        record_end(edp_password)
    };
    connector_status_t result;
    uint8_t * edp_header;

    uint8_t * edp_password;
    uint8_t * start_ptr;

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &start_ptr, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    edp_password = start_ptr;

    message_store_u8(edp_password, opcode, SECURITY_OPER_PASSWORD);

    {
        uint16_t const length16 = (uint16_t) connector_ptr->edp_data.config.password_length;

        ASSERT(connector_ptr->edp_data.config.password_length <= UINT16_MAX);
        message_store_be16(edp_password, password_length, length16);
    }
    edp_password += record_bytes(edp_password);

    memcpy(edp_password, connector_ptr->edp_data.config.password, connector_ptr->edp_data.config.password_length);
    edp_password += connector_ptr->edp_data.config.password_length;

    {
        size_t const length = (size_t)(edp_password-start_ptr);

        ASSERT(edp_password > start_ptr);
        result = tcp_initiate_send_packet(connector_ptr, edp_header, length,
                                E_MSG_MT2_TYPE_PAYLOAD,
                                tcp_release_packet_buffer,
                                NULL);
    }
done:
    return result;
}

STATIC connector_status_t send_vendor_id(connector_data_t * const connector_ptr)
{
    /*
     * packet format:
     *  ----------------------------------------------------------------
     * | 0 - 1 | 2 - 3 |       4         |       5          |  6 - 9    |
     *  ----------------------------------------------------------------
     * |   EDP Header  | security coding | vendor ID opcode | vendor ID |
     *  ----------------------------------------------------------------
    */
    enum edp_vendor_msg {
        field_define(edp_vendor_msg, security_coding, uint8_t),
        field_define(edp_vendor_msg, opcode, uint8_t),
        field_define(edp_vendor_msg, vendor_id, uint32_t),
        record_end(edp_vendor_msg)
    };

    connector_status_t result;
    uint8_t * edp_header = NULL;

    size_t const discovery_vendor_header_size = record_bytes(edp_vendor_msg);
    uint8_t * edp_vendor_msg;

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &edp_vendor_msg, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    message_store_u8(edp_vendor_msg, security_coding, SECURITY_PROTO_NONE);
    message_store_u8(edp_vendor_msg, opcode, DISC_OP_VENDOR_ID);
#if !(defined CONNECTOR_VENDOR_ID)
    message_store_be32(edp_vendor_msg, vendor_id, connector_ptr->edp_data.config.vendor_id);
    connector_debug_line("Send vendor id = 0x%08X", connector_ptr->edp_data.config.vendor_id);
#else
    message_store_be32(edp_vendor_msg, vendor_id, CONNECTOR_VENDOR_ID);
    connector_debug_line("Send vendor id = 0x%08X", CONNECTOR_VENDOR_ID);
#endif

    result = tcp_initiate_send_packet(connector_ptr, edp_header,
                                discovery_vendor_header_size,
                                E_MSG_MT2_TYPE_PAYLOAD,
                                tcp_release_packet_buffer,
                                NULL);
done:
    return result;
}

STATIC connector_status_t send_device_type(connector_data_t * const connector_ptr)
{
    /*
     * packet format:
     *  --------------------------------------------------------------------
     * | 0 - 1 | 2 - 3 |    4     |     5       |    6 - 7    |    8...     |
     *  --------------------------------------------------------------------
     * |   EDP Header  | security | device type | Device type | Device type |
     * |               |  coding  |   opcode    |   length    |             |
     *  --------------------------------------------------------------------
    */
    enum edp_device_type_msg {
        field_define(edp_device_type, security_coding, uint8_t),
        field_define(edp_device_type, opcode, uint8_t),
        field_define(edp_device_type, length, uint16_t),
        record_end(edp_device_type)
    };
    size_t const device_type_header_size = record_bytes(edp_device_type);

    connector_status_t result;
    uint8_t * edp_header = NULL;
    uint8_t * edp_device_type;

#if (defined CONNECTOR_DEVICE_TYPE)
    static const char connector_device_type[] = CONNECTOR_DEVICE_TYPE;
    size_t device_type_length  = sizeof connector_device_type-1;
#else
    char const * const connector_device_type = connector_ptr->edp_data.config.device_type;
    size_t const device_type_length = connector_ptr->edp_data.config.device_type_length;
#endif

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &edp_device_type, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    message_store_u8(edp_device_type, security_coding, SECURITY_PROTO_NONE);
    message_store_u8(edp_device_type, opcode, DISC_OP_DEVICETYPE);

    {
        uint16_t const length16 = (uint16_t) device_type_length;

        ASSERT(device_type_length <= UINT8_MAX); /* Despite the size is a 16-bit integer, the maximum allowed is 255 */
        message_store_be16(edp_device_type, length, length16);
    }

    edp_device_type += device_type_header_size;
    memcpy(edp_device_type, connector_device_type, device_type_length);

    connector_debug_line("Send device type = %.*s", (int)device_type_length, connector_device_type);

    result = tcp_initiate_send_packet(connector_ptr, edp_header,
                                (device_type_header_size + device_type_length),
                                E_MSG_MT2_TYPE_PAYLOAD,
                                tcp_release_packet_buffer,
                                NULL);
done:
    return result;
}

STATIC connector_status_t send_complete(connector_data_t * const connector_ptr)
{
    /*
     * packet format:
     *  -----------------------------------------
     * | 0 - 1 | 2 - 3 |   4    |      5         |
     *  -----------------------------------------
     * |   EDP Header  | coding | initialization |
     * |               | scheme |   done opcode  |
     *  -----------------------------------------
    */
    enum edp_discovery_complete {
        field_define(edp_discovery_complete, security_coding, uint8_t),
        field_define(edp_discovery_complete, opcode, uint8_t),
        record_end(edp_discovery_complete)
    };
    size_t const discovery_complete_message_size = record_bytes(edp_discovery_complete);
    uint8_t * edp_discovery_complete;
    connector_status_t result;
    uint8_t * edp_header = NULL;

    edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_MT2_MSG_NUM, &edp_discovery_complete, NULL);
    if (edp_header == NULL)
    {
        result = connector_pending;
        goto done;
    }

    connector_debug_line("Send complete");
    message_store_u8(edp_discovery_complete, security_coding, SECURITY_PROTO_NONE);
    message_store_u8(edp_discovery_complete, opcode, DISC_OP_INITCOMPLETE);

    result = tcp_initiate_send_packet(connector_ptr, edp_header,
                                discovery_complete_message_size,
                                E_MSG_MT2_TYPE_PAYLOAD,
                                tcp_release_packet_buffer,
                                NULL);

done:
    return result;
}

STATIC connector_status_t layer_discovery_facility(connector_data_t * const connector_ptr);
STATIC connector_status_t connector_edp_init(connector_data_t * const connector_ptr);

STATIC connector_status_t edp_tcp_open_process(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_idle;

    switch (edp_get_edp_state(connector_ptr))
    {
    case edp_communication_connect_to_cloud:
        edp_get_device_cloud(connector_ptr);
        result = connect_to_cloud(connector_ptr, connector_ptr->edp_data.config.cloud_url);
        switch(result)
        {
        case connector_working:
            edp_set_edp_state(connector_ptr, edp_configuration_init);
            connector_ptr->edp_data.send_packet.packet_buffer.in_use = connector_false;
            connector_ptr->edp_data.receive_packet.packet_buffer.in_use = connector_false;
            connector_ptr->edp_data.receive_packet.packet_buffer.next = NULL;
            connector_ptr->edp_data.receive_packet.free_packet_buffer = &connector_ptr->edp_data.receive_packet.packet_buffer;
            break;
        case connector_unavailable:
            edp_set_active_state(connector_ptr, connector_transport_idle);
            connector_ptr->edp_data.stop.auto_connect = connector_false;
            break;
        case connector_open_error:
        {
            connector_ptr->edp_data.connect_at = 0;
            edp_set_active_state(connector_ptr, connector_transport_wait_for_reconnect);
#if (defined CONNECTOR_NETWORK_TCP_START)
            if (CONNECTOR_NETWORK_TCP_START == connector_connect_manual)
#else
            if (connector_ptr->edp_data.connect_type == connector_connect_manual)
#endif
            {
                /* Application must call initiate_action to Start Cloud Connector */
                edp_set_active_state(connector_ptr, connector_transport_idle);
            }
            break;
        }
        case connector_success:
        case connector_init_error:
        case connector_invalid_data_size:
        case connector_invalid_data_range:
        case connector_invalid_data:
        case connector_keepalive_error:
        case connector_bad_version:
        case connector_device_terminated:
        case connector_service_busy:
        case connector_invalid_response:
        case connector_no_resource:
        case connector_idle:
        case connector_pending:
        case connector_active:
        case connector_abort:
        case connector_device_error:
        case connector_exceed_timeout:
        case connector_invalid_payload_packet:
            break;
        }
        break;
    case edp_configuration_init:
        result = connector_edp_init(connector_ptr);
        switch(result)
        {
        case connector_working:
            edp_set_edp_state(connector_ptr, edp_communication_send_version);
            break;
        default:
            break;
        }
        break;
    case edp_communication_send_version:
    case edp_communication_send_keepalive:
    case edp_initialization_send_protocol_version:
    case edp_security_send_identity_verification:
    case edp_security_send_device_id:
    case edp_security_send_device_cloud_url:
    case edp_security_send_password:
    case edp_discovery_send_vendor_id:
    case edp_discovery_send_device_type:
    case edp_discovery_facility:
    case edp_discovery_send_complete:
    case edp_state_send_in_progress:
    case edp_connected:
    {
        switch (edp_get_edp_state(connector_ptr))
        {
        case edp_communication_send_version:
            connector_debug_line("Send MT Version");
            result = send_version(connector_ptr, E_MSG_MT2_TYPE_VERSION, EDP_MT_VERSION);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_communication_receive_version_response);
            }
            break;

        case edp_communication_send_keepalive:
            result = send_keepalive(connector_ptr);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_initialization_send_protocol_version);
            }
            break;

        case edp_initialization_send_protocol_version:
        {
            #define EDP_PROTOCOL_VERSION    0x120

            connector_debug_line("Send protocol version");
            result = send_version(connector_ptr, E_MSG_MT2_TYPE_PAYLOAD, EDP_PROTOCOL_VERSION);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_initialization_receive_protocol_version);
            }
            break;
        }
        case edp_security_send_identity_verification:
            result = send_identity_verification(connector_ptr);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_security_send_device_id);
            }
            break;
        case edp_security_send_device_id:
            if (connector_ptr->connector_got_device_id)
            {
                result = send_device_id(connector_ptr);
                if (result == connector_working)
                {
                    edp_set_next_edp_state(connector_ptr, edp_security_send_device_cloud_url);
                }
            }
            else
            {
                result = send_provisioning(connector_ptr);
                if (result == connector_working)
                {
                    edp_set_next_edp_state(connector_ptr, edp_security_receive_device_id);
                }
            }
            break;
        case edp_security_send_device_cloud_url:
            result = send_cloud_url(connector_ptr);
            if (result == connector_working)
            {
#if (defined CONNECTOR_IDENTITY_VERIFICATION)
                edp_set_next_edp_state(connector_ptr, (CONNECTOR_IDENTITY_VERIFICATION == connector_identity_verification_password) ? edp_security_send_password : edp_discovery_send_vendor_id);
#else
                edp_set_next_edp_state(connector_ptr, (connector_ptr->edp_data.config.identity_verification == connector_identity_verification_password) ? edp_security_send_password : edp_discovery_send_vendor_id);
#endif
            }
            break;

        case edp_security_send_password:
            result = send_password(connector_ptr);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_discovery_send_vendor_id);
            }
            break;

        case edp_discovery_send_vendor_id:
            result = send_vendor_id(connector_ptr);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_discovery_send_device_type);
            }
            break;

        case edp_discovery_send_device_type:
            result = send_device_type(connector_ptr);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_discovery_facility);
            }
            break;
        case edp_discovery_facility:
            result = layer_discovery_facility(connector_ptr);

            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_discovery_send_complete);
            }
            break;

        case edp_discovery_send_complete:
            result = send_complete(connector_ptr);
            if (result == connector_working)
            {
                edp_set_next_edp_state(connector_ptr, edp_connected);
            }
            break;
        case edp_connected:
        {
            edp_set_edp_state(connector_ptr, edp_facility_process);
            edp_set_active_state(connector_ptr, connector_transport_receive);

            result = notify_status(connector_ptr->callback, connector_tcp_communication_started, connector_ptr->context);
            if (result != connector_working)
            {
                result = connector_abort;
            }
            goto done;
        }
        case edp_state_send_in_progress:
            break;
        default:
            break;
        }

        if (result == connector_working || result == connector_idle || result == connector_pending)
        {
            result = edp_tcp_send_process(connector_ptr);

            switch (result)
            {
                case connector_idle:
                    /* This means that no data is pending to be sent. */
                    ASSERT(result != connector_idle);
                    result = connector_abort;
                    goto done;
                case connector_working:
                    edp_set_edp_state(connector_ptr, edp_get_next_edp_state(connector_ptr));
                    break;
                case connector_pending:
                    edp_set_edp_state(connector_ptr, edp_state_send_in_progress);
                    break;
                default:
                    break;
            }
        }

        goto done;
    }
    case edp_communication_receive_version_response:
        result = receive_edp_version(connector_ptr);
        if (result == connector_working)
        {
            edp_set_edp_state(connector_ptr, edp_communication_send_keepalive);
        }

        break;

    case edp_initialization_receive_protocol_version:
        result = receive_protocol_version(connector_ptr);
        if (result == connector_working)
        {
            edp_set_edp_state(connector_ptr, edp_security_send_identity_verification);
        }
        break;
    case edp_security_receive_device_id:
        result = receive_device_id(connector_ptr);
        if (result == connector_working)
        {
            connector_ptr->connector_got_device_id = connector_true;
            edp_set_edp_state(connector_ptr, edp_security_send_device_cloud_url);
        }
        break;
    case edp_facility_process:
        /* Should not be here since active state should not be open state. */
        ASSERT(connector_false);
        break;
    }

done:
    if (result != connector_idle && result != connector_pending && result != connector_working)
    {
        if (edp_get_edp_state(connector_ptr) != edp_communication_connect_to_cloud && edp_get_edp_state(connector_ptr) != edp_configuration_init)
        {
           /* set the close state and make it goes to close connection state */
            edp_set_close_status(connector_ptr, connector_close_status_device_error);
            edp_set_active_state(connector_ptr, connector_transport_close);
            result = connector_working;
        }
    }
    return result;
}
