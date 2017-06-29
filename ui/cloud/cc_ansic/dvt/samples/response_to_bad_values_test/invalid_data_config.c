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

static connector_callback_status_t app_get_invalid_data_ip_address(connector_config_ip_address_t * const config_ip)
{
    static uint32_t invalid_data_ip_address = 0;

    config_ip->ip_address_type = connector_ip_address_ipv4;
    config_ip->address = (uint8_t *)&invalid_data_ip_address;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_mac_addr(connector_config_pointer_data_t * const config_mac)
{
//#error "Specify device MAC address for LAN connection"

    config_mac->data = NULL;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_data_device_id(connector_config_pointer_data_t * const config_device_id)
{

    config_device_id->data = NULL;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_vendor_id(connector_config_vendor_id_t * const config_vendor)
{
    static uint32_t const invalid_device_vendor_id = 0x00000000;

    config_vendor->id  =  invalid_device_vendor_id;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_device_type(connector_config_pointer_string_t * const config_device_type)
{

    config_device_type->string = NULL;
    config_device_type->length = 5;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_cloud_url(connector_config_pointer_string_t * const config_url)
{

    config_url->string = NULL;
    config_url->length = 5;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_data_connection_type(connector_config_connection_type_t * const config_connection)
{

    config_connection->type = (connector_connection_type_t) -1;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_data_link_speed(connector_config_link_speed_t * const config_link)
{
    /* there is no invalid data can be returned */
    config_link->speed = 0;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_phone_number(connector_config_pointer_string_t * const config_phone_number)
{
    /*
     * Return pointer to phone number for WAN connection type.
     */
    config_phone_number->string = NULL;
    config_phone_number->length = 5;

    return connector_callback_continue;
}

/* Keep alives are from the prospective of Device Cloud */
/* This keep alive is sent from Device Cloud to the device */
static connector_callback_status_t app_get_invalid_data_tx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    config_keepalive->interval_in_seconds = 0;

    return connector_callback_continue;
}

/* This keep alive is sent from the device to Device Cloud  */
static connector_callback_status_t app_get_invalid_data_rx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    config_keepalive->interval_in_seconds = 0;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_data_wait_count(connector_config_wait_count_t * const config_wait)
{
    config_wait->count = 0;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_firmware_support(connector_config_supported_t * const config_status)
{

    config_status->supported = (connector_bool_t)-1;
    return connector_callback_continue;

}

static connector_callback_status_t app_get_invalid_data_data_service_support(connector_config_supported_t * const config_status)
{
    config_status->supported = (connector_bool_t)-1;
    return connector_callback_continue;
}

static connector_callback_status_t  app_get_invalid_data_file_system_support(connector_config_supported_t * const config_status)
{
    config_status->supported = (connector_bool_t)-1;
    return connector_callback_continue;
}

static connector_callback_status_t  app_get_invalid_data_remote_configuration_support(connector_config_supported_t * const config_status)
{
    config_status->supported = (connector_bool_t)-1;
    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_max_message_transactions(connector_config_max_transaction_t * const config_max_transaction)
{
    /* no invalid data can be set */
    config_max_transaction->count = 0;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_data_device_id_method(connector_config_device_id_method_t * const config_device)
{

    config_device->method = (connector_device_id_method_t)-1;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_imei_number(connector_config_pointer_data_t * const config_imei)
{
    config_imei->data = NULL;

    return connector_callback_continue;
}

static connector_callback_status_t app_start_network_tcp(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = (connector_connect_auto_type_t)-1;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_wan_type(connector_config_wan_type_t * const config_wan)
{
    config_wan->type = (connector_wan_type_t)-1;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_data_esn(connector_config_pointer_data_t * const config_esn)
{
    config_esn->data = NULL;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_meid(connector_config_pointer_data_t * const config_meid)
{
    config_meid->data = NULL;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_invalid_data_identity_verification(connector_config_identity_verification_t * const config_identity)
{

    config_identity->type = (connector_identity_verification_t)-1;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_invalid_data_password(connector_config_pointer_string_t * const config_password)
{
    config_password->string = NULL;
    config_password->length = 0;

    return connector_callback_continue;
}


/* End of Cloud Connector configuration routines */

/*
 * Configuration callback routine.
 */
extern char const * app_config_class_to_string(connector_request_id_config_t const value);

connector_callback_status_t app_invalid_data_config_handler(connector_request_id_config_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_unrecognized;

    APP_DEBUG("app_invalid_data_config_handler: %s\n", app_config_class_to_string(request_id));

    switch (request_id)
    {
    case connector_request_id_config_device_id:
        status = app_get_invalid_data_device_id(data);
        break;

    case connector_request_id_config_mac_addr:
        status = app_get_invalid_data_mac_addr(data);
        break;

    case connector_request_id_config_vendor_id:
        status = app_get_invalid_data_vendor_id(data);
        break;

    case connector_request_id_config_device_type:
        status = app_get_invalid_data_device_type(data);
        break;

    case connector_request_id_config_device_cloud_url:
         status = app_get_invalid_data_cloud_url(data);
        break;

    case connector_request_id_config_connection_type:
        status = app_get_invalid_data_connection_type(data);
        break;

    case connector_request_id_config_link_speed:
        status = app_get_invalid_data_link_speed(data);
        break;

    case connector_request_id_config_phone_number:
        status = app_get_invalid_data_phone_number(data);
        break;

    case connector_request_id_config_tx_keepalive:
        status = app_get_invalid_data_tx_keepalive_interval(data);
        break;

    case connector_request_id_config_rx_keepalive:
        status = app_get_invalid_data_rx_keepalive_interval(data);
        break;

    case connector_request_id_config_wait_count:
         status = app_get_invalid_data_wait_count(data);
        break;

    case connector_request_id_config_ip_addr:
        status = app_get_invalid_data_ip_address(data);
        break;

    case connector_request_id_config_error_status:
        status = app_config_error(data);
        break;

    case connector_request_id_config_firmware_facility:
        status = app_get_invalid_data_firmware_support(data);
        break;

    case connector_request_id_config_data_service:
        status = app_get_invalid_data_data_service_support(data);
        break;

    case connector_request_id_config_file_system:
        status = app_get_invalid_data_file_system_support(data);
        break;

    case connector_request_id_config_remote_configuration:
        status = app_get_invalid_data_remote_configuration_support(data);
        break;

    case connector_request_id_config_max_transaction:
         status = app_get_invalid_data_max_message_transactions(data);
        break;

    case connector_request_id_config_device_id_method:
        status = app_get_invalid_data_device_id_method(data);
        break;

     case connector_request_id_config_network_tcp:
         status = app_start_network_tcp(data);
         break;

     case connector_request_id_config_imei_number:
         status = app_get_invalid_data_imei_number(data);
         break;

     case connector_request_id_config_wan_type:
         status = app_get_invalid_data_wan_type(data);
         break;

     case connector_request_id_config_esn:
         status = app_get_invalid_data_esn(data);
         break;

     case connector_request_id_config_meid:
         status = app_get_invalid_data_meid(data);
         break;

     case connector_request_id_config_identity_verification:
         status = app_get_invalid_data_identity_verification(data);
         break;

     case connector_request_id_config_password:
         status = app_get_invalid_data_password(data);
         break;

    default:
        APP_DEBUG("connector_config_callback: unknown configuration request= %d\n", request_id);
        break;
    }

    return status;
}

