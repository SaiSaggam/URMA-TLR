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

STATIC connector_status_t notify_error_status(connector_callback_t const callback, connector_class_id_t const class_number, connector_request_id_t const request_number, connector_status_t const status, void * const context)
{
    connector_status_t result = connector_working;

#if (defined CONNECTOR_DEBUG)
    connector_config_error_status_t err_status;
    connector_request_id_t request_id;

    request_id.config_request = connector_request_id_config_error_status;
    err_status.class_id = class_number;
    err_status.request_id = request_number;
    err_status.status = status;

    {
        connector_callback_status_t const callback_status = connector_callback(callback, connector_class_id_config, request_id, &err_status, context);
        switch (callback_status)
        {
            case connector_callback_continue:
                break;
            default:
                result = connector_abort;
                break;
        }
    }

#else
    UNUSED_PARAMETER(callback);
    UNUSED_PARAMETER(class_number);
    UNUSED_PARAMETER(request_number);
    UNUSED_PARAMETER(status);
    UNUSED_PARAMETER(context);
#endif
    return result;
}

STATIC connector_status_t get_config_device_id(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_callback_status_t callback_status;
    connector_request_id_t request_id;
    connector_config_pointer_data_t device_id;

    device_id.bytes_required = DEVICE_ID_LENGTH;

    request_id.config_request = connector_request_id_config_device_id;

    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &device_id, connector_ptr->context);
    switch (callback_status)
    {
    case connector_callback_continue:
        if (device_id.bytes_required != DEVICE_ID_LENGTH)
            result = connector_invalid_data_size;
        /* coverity[uninit_use] */
        else if (device_id.data == NULL)
            result = connector_invalid_data;
        else
            memcpy(connector_ptr->device_id, device_id.data, DEVICE_ID_LENGTH);
        break;
    case connector_callback_busy:
        result = connector_pending;
        break;
    case connector_callback_unrecognized:
    case connector_callback_abort:
    case connector_callback_error:
        result = connector_abort;
        goto done;
    }

    if (result != connector_working)
    {
        notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context);
        result = connector_abort;
    }

done:
    return result;
}

#if !(defined CONNECTOR_CLOUD_URL)
STATIC connector_status_t get_config_device_cloud_url(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_callback_status_t callback_status;
    connector_config_pointer_string_t   cloud_url;
    connector_request_id_t request_id;

    request_id.config_request = connector_request_id_config_device_cloud_url;


    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &cloud_url, connector_ptr->context);
    switch (callback_status)
    {
    case connector_callback_continue:
        /* coverity[uninit_use] */
        if ((cloud_url.length == 0) || (cloud_url.length > CLOUD_URL_LENGTH-1))
        {
            result =  connector_invalid_data_size;
        }
        /* coverity[uninit_use] */
        else if (cloud_url.string == NULL)
        {
            result = connector_invalid_data;
        }
        else
        {
            connector_ptr->device_cloud_url_length = cloud_url.length;
            connector_ptr->device_cloud_url = cloud_url.string;
        }
        break;

    case connector_callback_busy:
    case connector_callback_unrecognized:
    case connector_callback_abort:
    case connector_callback_error:
        result = connector_abort;
        goto done;
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

#if (defined CONNECTOR_TRANSPORT_SMS)
#if !(defined CONNECTOR_CLOUD_PHONE)
STATIC connector_status_t get_config_device_cloud_phone(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_callback_status_t callback_status;
    connector_config_pointer_string_t   cloud_phone;
    connector_request_id_t request_id;

    request_id.config_request = connector_request_id_config_get_device_cloud_phone;

    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &cloud_phone, connector_ptr->context);
    switch (callback_status)
    {
    case connector_callback_continue:
        /* coverity[uninit_use] */
        if (cloud_phone.length == 0)
        {
            result =  connector_invalid_data_size;
        }
        /* coverity[uninit_use] */
        else if (cloud_phone.string == NULL)
        {
            result = connector_invalid_data;
        }
        else
        {
            connector_ptr->device_cloud_phone_length = cloud_phone.length;
            connector_ptr->device_cloud_phone = cloud_phone.string;
        }
        break;

    case connector_callback_busy:
    case connector_callback_unrecognized:
    case connector_callback_abort:
    case connector_callback_error:
        result = connector_abort;
        goto done;
    }

    if (result != connector_working)
    {
        notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context);
        result = connector_abort;
    }

done:
    return result;
}

/* This function will only be called under SMS transport when a 'provisioning' message is received */
STATIC connector_status_t set_config_device_cloud_phone(connector_data_t * const connector_ptr, char * phone_number)
{
    connector_status_t result = connector_working;
    connector_callback_status_t callback_status;
    connector_config_pointer_string_t cloud_phone;
    connector_request_id_t request_id;

    request_id.config_request = connector_request_id_config_set_device_cloud_phone;

    cloud_phone.string = phone_number;
    cloud_phone.length = strlen(phone_number);

    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &cloud_phone, connector_ptr->context);
    switch (callback_status)
    {
    case connector_callback_continue:
        /* User may have changed the pointer during reconfiguration... So reload it again */
        get_config_device_cloud_phone(connector_ptr);
        break;

    case connector_callback_busy:
    case connector_callback_unrecognized:
    case connector_callback_abort:
    case connector_callback_error:
        result = connector_abort;
        goto error;
    }
error:
    if (result != connector_working)
    {
        notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context);
        result = connector_abort;
    }

    return result;
}
#endif

#if !(defined CONNECTOR_CLOUD_SERVICE_ID)
STATIC connector_status_t get_config_device_cloud_service_id(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_callback_status_t callback_status;
    connector_config_pointer_string_t   cloud_service_id;
    connector_request_id_t request_id;

    request_id.config_request = connector_request_id_config_device_cloud_service_id;

    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &cloud_service_id, connector_ptr->context);
    switch (callback_status)
    {
    case connector_callback_continue:
        /* coverity[uninit_use] */
        if (cloud_service_id.string == NULL)
        {
            result = connector_invalid_data;
        }
        else
        {
            connector_ptr->device_cloud_service_id_length = cloud_service_id.length;
            connector_ptr->device_cloud_service_id = cloud_service_id.string;
        }
        break;

    case connector_callback_busy:
    case connector_callback_unrecognized:
    case connector_callback_abort:
    case connector_callback_error:
        result = connector_abort;
        goto done;
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
#endif

STATIC connector_status_t get_config_connection_type(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;

#if (defined CONNECTOR_CONNECTION_TYPE)
    ASSERT((CONNECTOR_CONNECTION_TYPE == connector_connection_type_lan) || (CONNECTOR_CONNECTION_TYPE == connector_connection_type_wan));
    connector_ptr->connection_type = CONNECTOR_CONNECTION_TYPE;
#else
    connector_config_connection_type_t  config_connection;
    connector_callback_status_t callback_status;
    connector_request_id_t request_id;

    request_id.config_request = connector_request_id_config_connection_type;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_connection, connector_ptr->context);

    switch (callback_status)
    {
    case connector_callback_continue:

        /* coverity[uninit_use] */
        switch (config_connection.type)
        {
        case connector_connection_type_lan:
        case connector_connection_type_wan:
            connector_ptr->connection_type = config_connection.type;
            break;

        default:
            notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, connector_invalid_data_range, connector_ptr->context);
            result = connector_abort;
            break;
        }
        break;

    case connector_callback_busy:
        result = connector_pending;
        break;

    case connector_callback_unrecognized:
    case connector_callback_abort:
    case connector_callback_error:
        result = connector_abort;
        break;
    }
#endif

    return result;
}

STATIC connector_status_t get_config_mac_addr(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;

    if (connector_ptr->mac_addr == NULL)
    {
        connector_callback_status_t callback_status;
        connector_request_id_t request_id;
        connector_config_pointer_data_t mac_addr;

        mac_addr.bytes_required = MAC_ADDR_LENGTH;

        request_id.config_request = connector_request_id_config_mac_addr;

        callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &mac_addr, connector_ptr->context);
        switch (callback_status)
        {
        case connector_callback_continue:
            if (mac_addr.bytes_required != MAC_ADDR_LENGTH)
                result = connector_invalid_data_size;
            /* coverity[uninit_use] */
            else if (mac_addr.data == NULL)
                result = connector_invalid_data;
            else
                connector_ptr->mac_addr = mac_addr.data;
            break;

        case connector_callback_busy:
        case connector_callback_unrecognized:
        case connector_callback_abort:
        case connector_callback_error:
            result = connector_abort;
            goto done;
        }

        if (result != connector_working)
        {
            notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context);
            result = connector_abort;
        }
    }

done:
    return result;
}

#if !(defined CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND)
STATIC connector_status_t get_config_link_speed(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    connector_callback_status_t callback_status;
    connector_request_id_t request_id;
    connector_config_link_speed_t config_link;

    request_id.config_request = connector_request_id_config_link_speed;

    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_link, connector_ptr->context);
    switch (callback_status)
    {
    case connector_callback_continue:
        /* coverity[uninit_use] */
        connector_ptr->link_speed = config_link.speed;
        break;
    case connector_callback_busy:
    case connector_callback_unrecognized:
    case connector_callback_abort:
    case connector_callback_error:
        result = connector_abort;
        break;
    }
    return result;
}
#endif

#if !(defined CONNECTOR_WAN_PHONE_NUMBER_DIALED)
STATIC connector_status_t get_config_phone_number(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    if (connector_ptr->phone_dialed == NULL)
    {
        connector_callback_status_t callback_status;
        connector_request_id_t request_id;
        connector_config_pointer_string_t phone_number;

        request_id.config_request = connector_request_id_config_phone_number;

        callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &phone_number, connector_ptr->context);
        switch (callback_status)
        {
        case connector_callback_continue:
            /* coverity[uninit_use] */
            if (phone_number.string == NULL)
            {
                result = connector_invalid_data;
            }
            /* coverity[uninit_use] */
            else if (phone_number.length == 0)
            {
                result = connector_invalid_data_size;
            }
            else
            {
                connector_ptr->phone_dialed_length = phone_number.length;
                connector_ptr->phone_dialed = phone_number.string;
            }
            break;
        case connector_callback_busy:
        case connector_callback_unrecognized:
        case connector_callback_abort:
        case connector_callback_error:
            result = connector_abort;
            goto done;
        }

        if (result != connector_working)
        {
            notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context);
            result = connector_abort;
        }
    }

done:
    return result;
}
#endif
