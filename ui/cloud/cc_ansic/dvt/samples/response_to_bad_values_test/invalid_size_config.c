/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ctype.h>
#include "errno.h"

#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "application.h"

/* Cloud Connector Configuration routines */

#define MAX_INTERFACES      128
#define DEVICE_ID_LENGTH    16
#define VENDOR_ID_LENGTH    4
#define MAC_ADDR_LENGTH     6


/*
 * Routine to get the IP address, you will need to modify this routine for your
 * platform.
 */

connector_callback_status_t app_get_invalid_size_ip_address(connector_config_ip_address_t * const config_ip)
{
    static uint8_t invalid_size_ip_address;

    memset(&config_ip->ip_address_type, 0xFF, sizeof config_ip->ip_address_type);
    config_ip->address = (uint8_t *)&invalid_size_ip_address;

    return connector_callback_continue;
}

/* MAC address used in this sample */
static connector_callback_status_t app_get_invalid_size_mac_addr(connector_config_pointer_data_t * const config_mac)
{

    config_mac->data = NULL;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_device_id(connector_config_pointer_data_t * const config_device_id)
{

    config_device_id->data   = NULL;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_vendor_id(connector_config_vendor_id_t * const config_vendor)
{
    /* no invalid size for vendor id */
    config_vendor->id = 0x0;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_device_type(connector_config_pointer_string_t * const config_device_type)
{
    static char invalid_size_device_type;

    config_device_type->string = &invalid_size_device_type;
    config_device_type->length = 0;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_size_cloud_url(connector_config_pointer_string_t * const config_url)
{
    static char  invalid_size_cloud_url;

    /* Return pointer to device type. */
    config_url->string = &invalid_size_cloud_url;
    config_url->length = 0;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_connection_type(connector_config_connection_type_t * const config_connection)
{

    /* No invalid_size for connection type so return good connection type */
    config_connection->type = device_connection_type;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_link_speed(connector_config_link_speed_t * const config_link)
{
    /* no invalid size */
    config_link->speed = 0;
    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_phone_number(connector_config_pointer_string_t * const config_phone_number)
{
    static char invalid_size_phone_number;

    config_phone_number->string = &invalid_size_phone_number;
    config_phone_number->length = 0;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_tx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    /* no invalid size */
    config_keepalive->interval_in_seconds = 0;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_rx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    /* no invalid size */
    config_keepalive->interval_in_seconds = 0;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_size_wait_count(connector_config_wait_count_t * const config_wait)
{
    /* no invalid size */
    config_wait->count = 0;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_firmware_support(connector_config_supported_t * const config_status)
{
    /* not invalid size so return valid response */
    config_status->supported = connector_true;
    return connector_callback_continue;

}

static connector_callback_status_t app_get_invalid_size_data_service_support(connector_config_supported_t * const config_status)
{
    /* not invalid size so return valid response */
    config_status->supported = connector_true;
    return connector_callback_continue;
}

static connector_callback_status_t  app_get_invalid_size_file_system_support(connector_config_supported_t * const config_status)
{
    /* not invalid size so return valid response */
    config_status->supported = connector_true;
    return connector_callback_continue;
}

static connector_callback_status_t  app_get_invalid_size_remote_configuration_support(connector_config_supported_t * const config_status)
{
    /* not invalid size so return valid response */
    config_status->supported = connector_true;
    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_max_message_transactions(connector_config_max_transaction_t * const config_max_transaction)
{
    /* not invalid size so return valid response */
    config_max_transaction->count = 0;
    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_size_device_id_method(connector_config_device_id_method_t * const config_device)
{

    /* not invalid size so return valid response */
    config_device->method = device_id_method;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_imei_number(connector_config_pointer_data_t * const config_imei)
{

    config_imei->data = NULL;


    return connector_callback_continue;
}

static connector_callback_status_t app_start_network_tcp(connector_config_connect_type_t * const config_connect)
{
    /* not invalid size so return valid response */
    config_connect->type = connector_connect_auto;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_wan_type(connector_config_wan_type_t * const config_wan)
{

    /* not invalid size so return valid response */
    config_wan->type = connector_wan_type_imei;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_size_esn(connector_config_pointer_data_t * const config_esn)
{

    config_esn->data = NULL;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_meid(connector_config_pointer_data_t * const config_meid)
{

    config_meid->data = NULL;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_size_identity_verification(connector_config_identity_verification_t * const config_identity)
{

    /* not invalid size so return valid response */
    config_identity->type = connector_identity_verification_simple;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_size_password(connector_config_pointer_string_t * const config_password)
{
    static char const connector_password[] = "";

    /* Return pointer to password. */
    config_password->string = (char *)connector_password;
    config_password->length = 0;

    return connector_callback_continue;
}


/* End of Cloud Connector configuration routines */


/*
 * Configuration callback routine.
 */
extern char const * app_config_class_to_string(connector_request_id_config_t const value);

connector_callback_status_t app_invalid_size_config_handler(connector_request_id_config_t const request_id,
                                              void * const data)
{
    connector_callback_status_t status = connector_callback_unrecognized;

    APP_DEBUG("app_invalid_size_config_handler: %s\n", app_config_class_to_string(request_id));

    switch (request_id)
    {
    case connector_request_id_config_device_id:
        APP_DEBUG("app_config_handler:connector_config_device_id\n");
        status = app_get_invalid_size_device_id(data);
        break;

    case connector_request_id_config_mac_addr:
        APP_DEBUG("app_config_handler:connector_config_mac_addr\n");
        status = app_get_invalid_size_mac_addr(data);
        break;

    case connector_request_id_config_vendor_id:
        APP_DEBUG("app_config_handler:connector_config_vendor_id\n");
        status = app_get_invalid_size_vendor_id(data);
        break;

    case connector_request_id_config_device_type:
        APP_DEBUG("app_config_handler:connector_config_vendor_id\n");
        status = app_get_invalid_size_device_type(data);
        break;

    case connector_request_id_config_device_cloud_url:
        APP_DEBUG("app_config_handler:connector_config_cloud_url\n");
        status = app_get_invalid_size_cloud_url(data);
        break;

    case connector_request_id_config_connection_type:
        APP_DEBUG("app_config_handler:connector_config_connection_type\n");
        status = app_get_invalid_size_connection_type(data);
        break;

    case connector_request_id_config_link_speed:
        APP_DEBUG("app_config_handler:connector_config_link_speed\n");
        status = app_get_invalid_size_link_speed(data);
        break;

    case connector_request_id_config_phone_number:
        APP_DEBUG("app_config_handler:connector_config_phone_number\n");
        status = app_get_invalid_size_phone_number(data);
        break;

    case connector_request_id_config_tx_keepalive:
        APP_DEBUG("app_config_handler:connector_config_tx_keepalive\n");
        status = app_get_invalid_size_tx_keepalive_interval(data);
        break;

    case connector_request_id_config_rx_keepalive:
        APP_DEBUG("app_config_handler:connector_config_rx_keepalive\n");
        status = app_get_invalid_size_rx_keepalive_interval(data);
        break;

    case connector_request_id_config_wait_count:
        APP_DEBUG("app_config_handler:connector_config_wait_count\n");
        status = app_get_invalid_size_wait_count(data);
        break;

    case connector_request_id_config_ip_addr:
        APP_DEBUG("app_config_handler:connector_config_ip_addr\n");
        status = app_get_invalid_size_ip_address(data);
        break;

    case connector_request_id_config_error_status:
        APP_DEBUG("app_config_handler:connector_config_error_status\n");
        status = app_config_error(data);
        break;

    case connector_request_id_config_firmware_facility:
        APP_DEBUG("app_config_handler:connector_config_firmware_support\n");
        status = app_get_invalid_size_firmware_support(data);
        break;

    case connector_request_id_config_data_service:
        APP_DEBUG("app_config_handler:connector_config_data_service_support\n");
        status = app_get_invalid_size_data_service_support(data);
        break;

    case connector_request_id_config_file_system:
        status = app_get_invalid_size_file_system_support(data);
        break;

    case connector_request_id_config_remote_configuration:
        status = app_get_invalid_size_remote_configuration_support(data);
        break;

    case connector_request_id_config_max_transaction:
        APP_DEBUG("app_config_handler:connector_config_max_message_transactions\n");
        status = app_get_invalid_size_max_message_transactions(data);
        break;

    case connector_request_id_config_device_id_method:
        status = app_get_invalid_size_device_id_method(data);
        break;

     case connector_request_id_config_network_tcp:
         status = app_start_network_tcp(data);
         break;

     case connector_request_id_config_imei_number:
         status = app_get_invalid_size_imei_number(data);
         break;

     case connector_request_id_config_wan_type:
         status = app_get_invalid_size_wan_type(data);
         break;

     case connector_request_id_config_esn:
         status = app_get_invalid_size_esn(data);
         break;

     case connector_request_id_config_meid:
         status = app_get_invalid_size_meid(data);
         break;

     case connector_request_id_config_identity_verification:
         status = app_get_invalid_size_identity_verification(data);
         break;

     case connector_request_id_config_password:
         status = app_get_invalid_size_password(data);;
         break;

    default:
        APP_DEBUG("connector_invalid_size_config_callback: unknown configuration request= %d\n", request_id);
        break;
    }

    return status;
}

