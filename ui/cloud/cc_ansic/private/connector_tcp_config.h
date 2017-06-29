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


#if !(defined CONNECTOR_VENDOR_ID)
STATIC connector_status_t get_config_vendor_id(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_config_vendor_id_t config_vendor;

    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.config_request = connector_request_id_config_vendor_id;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_vendor, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
            {
                uint32_t const max_vendor_id = CONNECTOR_MAX_VENDOR_ID_NUMBER;
                /* coverity[uninit_use] */
                if ((config_vendor.id == 0) || (config_vendor.id >= max_vendor_id))
                {
                    notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, connector_invalid_data_range, connector_ptr->context);
                    result = connector_abort;
                }
                else
                    connector_ptr->edp_data.config.vendor_id = config_vendor.id;

            }
            break;

        case connector_callback_busy:
        case connector_callback_abort:
        case connector_callback_error:
        case connector_callback_unrecognized:
            result =  connector_abort;
            break;
        }
    }

    return result;
}
#endif

#if !(defined CONNECTOR_DEVICE_TYPE)
STATIC connector_status_t get_config_device_type(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_config_pointer_string_t device_type;
    connector_request_id_t request_id;

    {
        connector_callback_status_t status;

        request_id.config_request = connector_request_id_config_device_type;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &device_type, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
            /* coverity[uninit_use] */
            if (device_type.string == NULL)
                result = connector_invalid_data;
            /* coverity[uninit_use] */
            else if ((device_type.length == 0) || (device_type.length > DEVICE_TYPE_LENGTH))
                result = connector_invalid_data_size;
            else
            {
                connector_ptr->edp_data.config.device_type_length = device_type.length;
                connector_ptr->edp_data.config.device_type = device_type.string;
            }
            break;

        case connector_callback_busy:
        case connector_callback_abort:
        case connector_callback_error:
        case connector_callback_unrecognized:
            result =  connector_abort;
            goto done;
        }
    }

    if (result != connector_working)
    {
        notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context);
        result = connector_abort;
    }

done:
    return result;
}
#endif

#if !(defined CONNECTOR_RX_KEEPALIVE_IN_SECONDS) || !(defined CONNECTOR_TX_KEEPALIVE_IN_SECONDS)
STATIC connector_status_t get_config_keepalive(connector_data_t * const connector_ptr, connector_request_id_config_t const config_request)
{
    connector_status_t result = connector_working;
    connector_config_keepalive_t keepalive;

    ASSERT((config_request == connector_request_id_config_rx_keepalive) || (config_request == connector_request_id_config_tx_keepalive));
    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.config_request = config_request;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &keepalive, connector_ptr->context);

        switch (status)
        {
            case connector_callback_continue:
            {

#if !defined(CONNECTOR_TX_KEEPALIVE_IN_SECONDS) && !defined(CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
                connector_bool_t const is_tx = connector_bool(config_request == connector_request_id_config_tx_keepalive);
                uint16_t const min_interval = is_tx ? MIN_TX_KEEPALIVE_INTERVAL_IN_SECONDS : MIN_RX_KEEPALIVE_INTERVAL_IN_SECONDS;
                uint16_t const max_interval = is_tx ? MAX_TX_KEEPALIVE_INTERVAL_IN_SECONDS : MAX_RX_KEEPALIVE_INTERVAL_IN_SECONDS;

#elif !(defined CONNECTOR_TX_KEEPALIVE_IN_SECONDS)
                uint16_t const min_interval = MIN_TX_KEEPALIVE_INTERVAL_IN_SECONDS;
                uint16_t const max_interval = MAX_TX_KEEPALIVE_INTERVAL_IN_SECONDS;

#elif !(defined CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
                uint16_t const min_interval = MIN_RX_KEEPALIVE_INTERVAL_IN_SECONDS;
                uint16_t const max_interval = MAX_RX_KEEPALIVE_INTERVAL_IN_SECONDS;
#endif
                /* coverity[uninit_use] */
                if ((keepalive.interval_in_seconds < min_interval) || (keepalive.interval_in_seconds > max_interval))
                {
                    notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, connector_invalid_data_range, connector_ptr->context);
                    result = connector_abort;
                    goto done;
                }

                {
#if !defined(CONNECTOR_TX_KEEPALIVE_IN_SECONDS) && !defined(CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
                    uint16_t * const keepalive_store_at = is_tx ? &connector_ptr->edp_data.config.tx_keepalive_interval : &connector_ptr->edp_data.config.rx_keepalive_interval;
#elif !defined(CONNECTOR_TX_KEEPALIVE_IN_SECONDS)
                    uint16_t * const keepalive_store_at = &connector_ptr->edp_data.config.tx_keepalive_interval;
#elif !defined(CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
                    uint16_t * const keepalive_store_at = &connector_ptr->edp_data.config.rx_keepalive_interval;
#endif
                    *keepalive_store_at = keepalive.interval_in_seconds;
                }
                break;
            }
            case connector_callback_busy:
            case connector_callback_abort:
            case connector_callback_error:
            case connector_callback_unrecognized:
                result =  connector_abort;
                goto done;
        }
    }

done:
    return result;
}
#endif

#if !(defined CONNECTOR_WAIT_COUNT)
STATIC connector_status_t get_config_wait_count(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_config_wait_count_t wait_count;

    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.config_request = connector_request_id_config_wait_count;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &wait_count, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
            /* coverity[uninit_use] */
            if ((wait_count.count < WAIT_COUNT_MIN) ||
                (wait_count.count > WAIT_COUNT_MAX))
            {
                notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, connector_invalid_data_range, connector_ptr->context);
                result = connector_abort;
            }
            else
            {
                connector_ptr->edp_data.config.wait_count = wait_count.count;
            }
            break;

        case connector_callback_busy:
        case connector_callback_abort:
        case connector_callback_error:
        case connector_callback_unrecognized:
            result = connector_abort;
            break;
        }
    }
    return result;
}
#endif


STATIC connector_status_t get_config_ip_addr(connector_data_t * const connector_ptr)
{
#define CC_IPV6_ADDRESS_LENGTH 16
#define CC_IPV4_ADDRESS_LENGTH 4

    connector_status_t result = connector_working;
    connector_config_ip_address_t config_ip;
    connector_request_id_t request_id;

    {
        connector_callback_status_t status;

        request_id.config_request = connector_request_id_config_ip_addr;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_ip, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
        {
            #define CC_ZERO_IP_ADDR         0x00000000
            #define CC_BROADCAST_IP_ADDR    0xFFFFFFFF

            /* coverity[uninit_use] */
            if (config_ip.address == NULL)
            {
                result = connector_invalid_data;
                goto error;
            }
            /* coverity[uninit_use] */
            if ((config_ip.ip_address_type != connector_ip_address_ipv6) && (config_ip.ip_address_type != connector_ip_address_ipv4))
            {
                result =  connector_invalid_data_size;
                goto error;
            }

            if (config_ip.ip_address_type == connector_ip_address_ipv4)
            {
                static uint32_t const zero_ip_addr = CC_ZERO_IP_ADDR;
                static uint32_t const broadcast_ip_addr = CC_BROADCAST_IP_ADDR;

                uint32_t const ip =  LoadBE32(config_ip.address);

                if ((ip == LoadBE32(&zero_ip_addr)) || (ip == LoadBE32(&broadcast_ip_addr)))
                {
                    result = connector_invalid_data_range;
                    goto error;
                }
            }

            connector_ptr->edp_data.config.ip_addr_length = (config_ip.ip_address_type == connector_ip_address_ipv4) ? CC_IPV4_ADDRESS_LENGTH : CC_IPV6_ADDRESS_LENGTH;
            connector_ptr->edp_data.config.ip_addr = config_ip.address;
            break;
        }

        case connector_callback_busy:
        case connector_callback_abort:
        case connector_callback_error:
        case connector_callback_unrecognized:
            result =  connector_abort;
            goto done;
        }
    }


error:
    if (result != connector_working)
    {
        notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context);
        result = connector_abort;
    }

done:
    return result;
}

#if !(defined CONNECTOR_IDENTITY_VERIFICATION)
STATIC connector_status_t get_config_identity_verification(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_config_identity_verification_t config_identity;


    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.config_request = connector_request_id_config_identity_verification;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_identity, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
            /* coverity[uninit_use] */
            switch (config_identity.type)
            {
            case connector_identity_verification_simple:
            case connector_identity_verification_password:
                connector_ptr->edp_data.config.identity_verification = config_identity.type;
                break;
            default:
                notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, connector_invalid_data_range, connector_ptr->context);
                result = connector_abort;
                break;
            }
            break;

        case connector_callback_busy:
        case connector_callback_abort:
        case connector_callback_error:
            result = connector_abort;
            break;

        case connector_callback_unrecognized:
            connector_ptr->edp_data.config.identity_verification = connector_identity_verification_simple;
            break;
        }
    }
    return result;
}
#endif

STATIC connector_status_t get_config_password(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_config_pointer_string_t config_password;


    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.config_request = connector_request_id_config_password;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_password, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
            /* coverity[uninit_use] */
            if (config_password.string == NULL)
            {
                notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, connector_invalid_data, connector_ptr->context);
                result = connector_abort;
            }
            else
            {
                /* coverity[uninit_use] */
                connector_ptr->edp_data.config.password_length = config_password.length;
                connector_ptr->edp_data.config.password = config_password.string;

            }
            break;

        case connector_callback_busy:
        case connector_callback_abort:
        case connector_callback_error:
        case connector_callback_unrecognized:
            result = connector_abort;
            break;
        }
    }

    return result;
}
