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

#include <psptypes.h>
#include <ipcfg.h>
#include <connector_api.h>
#include <platform.h>
#include <bele.h>
#include "connector_config.h"

#define IPCFG_default_enet_device       0

extern _enet_address device_mac_addr; /* defined in main.c */

/*
 * Routine to get the IP address, you will need to modify this routine for your
 * platform.
 */
static connector_callback_status_t app_get_ip_address(connector_config_ip_address_t * const config_ip)
{
    IPCFG_IP_ADDRESS_DATA ip_data;
    connector_callback_status_t const status = ipcfg_get_ip(IPCFG_default_enet_device, &ip_data) ? connector_callback_continue : connector_callback_abort;
    static uint32_t val;

    APP_DEBUG("get_ip_address: IP Address: %d.%d.%d.%d\n\n", IPBYTES(ip_data.ip));
    StoreBE32(&val, ip_data.ip);
    
    config_ip->address = (uint8_t *)&val;
    config_ip->ip_address_type = connector_ip_address_ipv4;

    return status;
}

connector_callback_status_t app_get_mac_addr(connector_config_pointer_data_t * const config_mac)
{
    connector_callback_status_t status = connector_callback_abort;

    ipcfg_get_mac(IPCFG_default_enet_device, device_mac_addr);
    config_mac->data = device_mac_addr;

    status = connector_callback_continue;

    return status;
}


static connector_callback_status_t app_get_device_id(connector_config_pointer_data_t * const config_device_id)
{
#if defined(CONNECTOR_CUSTOMIZE_GET_DEVICE_ID_METHOD)
	return app_custom_get_device_id(id);
#else	
    #define DEVICE_ID_LENGTH    16
    static uint8_t device_id[DEVICE_ID_LENGTH] = {0};
    connector_callback_status_t const status = ipcfg_get_mac(IPCFG_default_enet_device, device_mac_addr) ? connector_callback_continue : connector_callback_abort;

    APP_DEBUG("get_device_id: MAC Address: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\r\n",
            device_mac_addr[0], device_mac_addr[1], device_mac_addr[2],
            device_mac_addr[3], device_mac_addr[4], device_mac_addr[5]);

    /* This sample uses the MAC address to format the device ID */
    device_id[8] = device_mac_addr[0];
    device_id[9] = device_mac_addr[1];
    device_id[10] = device_mac_addr[2];
    device_id[11] = 0xFF;
    device_id[12] = 0xFF;
    device_id[13] = device_mac_addr[3];
    device_id[14] = device_mac_addr[4];
    device_id[15] = device_mac_addr[5];

    config_device_id->data = device_id;

    return status;
#endif
}

#if !(defined CONNECTOR_CONNECTION_TYPE)
static connector_callback_status_t app_get_connection_type(connector_connection_type_t ** const type)
{
    /* Return pointer to connection type */
    static connector_connection_type_t  device_connection_type = connector_connection_type_lan;

    *type = &device_connection_type;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND)
static connector_callback_status_t app_get_link_speed(connector_config_link_speed_t * const config_link)
{
    config_link->speed = 0;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_CLOUD_URL)
#if !(defined CLOUD_URL_LENGTH)
#define CLOUD_URL_LENGTH   64
#endif
char connector_cloud_url[CLOUD_URL_LENGTH] = {0};   /* Filled by connector_config.c */
static connector_callback_status_t app_get_device_cloud_url(connector_config_pointer_string_t * const config_url)
{
    config_url->string = (char *)connector_cloud_url;
    config_url->length = sizeof connector_cloud_url -1;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_VENDOR_ID)
uint32_t device_vendor_id = 0x00000000;   /* Filled by connector_config.c */
static connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor)
{
    config_vendor->id  =  device_vendor_id;

    return connector_callback_continue;
}
#endif

#if !defined(CONNECTOR_TX_KEEPALIVE_IN_SECONDS)
/* Keep alives are from the prospective of the server */
/* This keep alive is sent from the server to the device */
static connector_callback_status_t app_get_tx_keepalive_interval(uint16_t const ** const interval, size_t * const size)
{
#define DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS     90
    /* Return pointer to Tx keepalive interval in seconds */
    static uint16_t device_tx_keepalive_interval = DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS;

    *interval = &device_tx_keepalive_interval;
    *size = sizeof device_tx_keepalive_interval;

    return connector_callback_continue;
}
#endif

#if !defined(CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
/* This keep alive is sent from the device to the server  */
static connector_callback_status_t app_get_rx_keepalive_interval(uint16_t const ** const interval, size_t * const size)
{
#define DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS     60
    /* Return pointer to Rx keepalive interval in seconds */
    static uint16_t device_rx_keepalive_interval = DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS;

    *interval = &device_rx_keepalive_interval;
    *size = sizeof device_rx_keepalive_interval;

    return connector_callback_continue;
}
#endif

#if !defined(CONNECTOR_WAIT_COUNT)
static connector_callback_status_t app_get_wait_count(uint16_t const ** const count, size_t * const size)
{
#define DEVICE_WAIT_COUNT     5
    /*
     * Return pointer to wait count (number of times not receiving Tx keepalive
     * from server is allowed).
     */
    static uint16_t device_wait_count = DEVICE_WAIT_COUNT;

    *count = &device_wait_count;
    *size = sizeof device_wait_count;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_FIRMWARE_SERVICE) && !(defined CONNECTOR_FIRMWARE_SUPPORT)
static connector_callback_status_t app_get_firmware_support(connector_config_supported_t * const config_status)
{
    config_status->supported = connector_true;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_DATA_SERVICE) && !(defined CONNECTOR_DATA_SERVICE_SUPPORT)
static connector_callback_status_t app_get_data_service_support(connector_config_supported_t * const config_status)
{
    config_status->supported = connector_true;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_FILE_SYSTEM) && !(defined CONNECTOR_FILE_SYSTEM_SUPPORT)
static connector_callback_status_t app_get_file_system_support(connector_config_supported_t * const config_status)
{
    config_status->supported = connector_true;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_RCI_SERVICE) && !(defined CONNECTOR_REMOTE_CONFIGURATION_SUPPORT)
static connector_callback_status_t app_get_remote_configuration_support(connector_config_supported_t * const config_status)
{
    config_status->supported = connector_true;

    return connector_callback_continue;
}
#endif

#if ((defined CONNECTOR_DATA_SERVICE) || (defined CONNECTOR_FILE_SYSTEM) || (defined CONNECTOR_RCI_SERVICE)) && !(defined CONNECTOR_MSG_MAX_TRANSACTION)
static connector_callback_status_t app_get_max_message_transactions(unsigned int * const transCount)
{
    #define CONNECTOR_MAX_MSG_TRANSACTIONS   1

    *transCount = CONNECTOR_MAX_MSG_TRANSACTIONS;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_DEVICE_ID_METHOD)
static connector_callback_status_t app_get_device_id_method(connector_config_device_id_method_t * const config_device)
{
    config_device->method = connector_device_id_method_auto;

    return connector_callback_continue;
}
#endif

static connector_callback_status_t app_start_network_tcp(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = connector_connect_auto;
    return connector_callback_continue;
}

#if !(defined CONNECTOR_IDENTITY_VERIFICATION)
static connector_callback_status_t app_get_identity_verification(connector_config_identity_verification_t * const config_identity)
{
    config_identity->type = connector_identity_verification_simple;

    return connector_callback_continue;
}
#endif

static connector_callback_status_t app_get_password(connector_config_pointer_string_t * const config_password)
{
    static  char const connector_password[] = "";

    /* Return pointer to password. */
    config_password->string = (char *)connector_password;
    config_password->length = sizeof connector_password - 1;

    return connector_callback_continue;
}
/* End of Connector configuration routines */

#ifdef CONNECTOR_DEBUG
/*
 * This routine is called when a configuration error is encountered by the Cloud Connector.
 * This is currently used as a debug tool for finding configuration errors.
 */
static connector_callback_status_t app_config_error(connector_config_error_status_t const * const error_data)
{

    connector_callback_status_t result = connector_callback_continue;

    APP_DEBUG("app_config_error: Class: %s (%d) ", app_class_to_string(error_data->class_id), error_data->class_id);

    switch (error_data->class_id)
    {
    case connector_class_id_config:
        APP_DEBUG("Request: %s (%d) ", app_config_class_to_string(error_data->request_id.config_request), error_data->request_id.config_request);
        break;
    case connector_class_id_network_tcp:
    case connector_class_id_network_udp:
    case connector_class_id_network_sms:
        APP_DEBUG("Request: %s (%d) ", app_network_class_to_string(error_data->request_id.network_request), error_data->request_id.network_request);
        break;
    case connector_class_id_operating_system:
        APP_DEBUG("Request: %s (%d) ", app_os_class_to_string(error_data->request_id.os_request), error_data->request_id.os_request);
        break;

#if (defined CONNECTOR_FIRMWARE_SERVICE)
    case connector_class_id_firmware:
        APP_DEBUG("Request: %s (%d) ", app_firmware_class_to_string(error_data->request_id.firmware_request), error_data->request_id.firmware_request);
        break;
#endif

#if (defined CONNECTOR_DATA_SERVICE)
    case connector_class_id_data_service:
        APP_DEBUG("Request: %s (%d) ", app_data_service_class_to_string(error_data->request_id.data_service_request), error_data->request_id.data_service_request);
        break;
#endif

#if (defined CONNECTOR_DATA_POINT)
    case connector_class_id_data_point:
        APP_DEBUG("Request: %s (%d) ", app_data_point_class_to_string(error_data->request_id.data_point_request), error_data->request_id.data_point_request);
        break;
#endif

#if (defined CONNECTOR_FILE_SYSTEM)
    case connector_class_id_file_system:
        APP_DEBUG("Request: %s (%d) ", app_file_system_class_to_string(error_data->request_id.file_system_request), error_data->request_id.file_system_request);
           break;
#endif

#if (defined CONNECTOR_RCI_SERVICE)
    case connector_class_id_remote_config:
        APP_DEBUG("Request: %s (%d) ", app_remote_config_class_to_string(error_data->request_id.remote_config_request), error_data->request_id.remote_config_request);
           break;
#endif

    case connector_class_id_status:
        APP_DEBUG("Request: %s (%d) ", app_status_class_to_string(error_data->request_id.status_request), error_data->request_id.status_request);
        break;

#if (defined CONNECTOR_SHORT_MESSAGE)
    case connector_class_id_short_message:
        APP_DEBUG("Request: %s (%d) ", app_sm_class_to_string(error_data->request_id.sm_request), error_data->request_id.sm_request);
        break;
#endif

    default:
        APP_DEBUG("unknown class id = %d ", error_data->class_id);
        break;
    }

    APP_DEBUG("Error status: %s (%d)\n", app_status_error_to_string(error_data->status), error_data->status);

    return result;
}
#endif

/*
 * Configuration callback routine.
 */
connector_callback_status_t app_config_handler(connector_request_id_config_t const request_id, void * const data)
{
    connector_callback_status_t status;


    switch (request_id)
    {
    case connector_request_id_config_device_id:
        status = app_get_device_id(data);
        break;

    case connector_request_id_config_mac_addr:
        status = app_get_mac_addr(data);
        break;

#if !(defined CONNECTOR_VENDOR_ID)
    case connector_request_id_config_vendor_id:
        status = app_get_vendor_id(data);
        break;
#endif

#if !(defined CONNECTOR_DEVICE_TYPE)
    case connector_request_id_config_device_type:
        status = app_get_device_type(data);
        break;
#endif

#if !(defined CONNECTOR_CLOUD_URL)
    case connector_request_id_config_device_cloud_url:
        status = app_get_device_cloud_url(data);
        break;
#endif

#if !(defined CONNECTOR_CONNECTION_TYPE)
    case connector_request_id_config_connection_type:
        status = app_get_connection_type(data);
        break;
#endif

#if !(defined CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND)
    case connector_request_id_config_link_speed:
        status = app_get_link_speed(data);
        break;
#endif

#if !(defined CONNECTOR_TX_KEEPALIVE_IN_SECONDS)
    case connector_request_id_config_tx_keepalive:
        status = app_get_tx_keepalive_interval(data);
        break;
#endif

#if !(defined CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
    case connector_request_id_config_rx_keepalive:
        status = app_get_rx_keepalive_interval(data);
        break;
#endif

#if !(defined CONNECTOR_WAIT_COUNT)
    case connector_request_id_config_wait_count:
        status = app_get_wait_count(data);
        break;
#endif

    case connector_request_id_config_ip_addr:
        status = app_get_ip_address(data);
        break;

#ifdef CONNECTOR_DEBUG
    case connector_request_id_config_error_status:
        status = app_config_error(data);
        break;
#endif

#if (defined CONNECTOR_FIRMWARE_SERVICE) && !(defined CONNECTOR_FIRMWARE_SUPPORT)
    case connector_request_id_config_firmware_facility:
        status = app_get_firmware_support(data);
        break;
#endif

#if (defined CONNECTOR_DATA_SERVICE) && !(defined CONNECTOR_DATA_SERVICE_SUPPORT)
    case connector_request_id_config_data_service:
        status = app_get_data_service_support(data);
        break;
#endif

#if (defined CONNECTOR_FILE_SYSTEM) && !(defined CONNECTOR_FILE_SYSTEM_SUPPORT)
    case connector_request_id_config_file_system:
        status = app_get_file_system_support(data);
        break;
#endif

#if (defined CONNECTOR_RCI_SERVICE) && !(defined CONNECTOR_REMOTE_CONFIGURATION_SUPPORT)
    case connector_request_id_config_remote_configuration:
        status = app_get_remote_configuration_support(data);
        break;
#endif

#if ((defined CONNECTOR_DATA_SERVICE) || (defined CONNECTOR_FILE_SYSTEM) || (defined CONNECTOR_RCI_SERVICE)) && !(defined CONNECTOR_MSG_MAX_TRANSACTION)
    case connector_request_id_config_max_transaction:
        status = app_get_max_message_transactions(data);
        break;
#endif

#if !(defined CONNECTOR_DEVICE_ID_METHOD)
    case connector_request_id_config_device_id_method:
        status = app_get_device_id_method(data);
        break;
#endif

     case connector_request_id_config_imei_number:
         /* status = app_get_imei_number(data); TODO */
    	 status = connector_callback_continue;
         break;

#if !(defined CONNECTOR_NETWORK_TCP_START)
     case connector_request_id_config_network_tcp:
         status = app_start_network_tcp(data);
         break;
#endif

#if !(defined CONNECTOR_NETWORK_UDP_START)
     case connector_request_id_config_network_udp:
         /* status = app_start_network_udp(data); TODO */
         status = connector_callback_continue;
         break;
#endif

#if !(defined CONNECTOR_NETWORK_SMS_START)
     case connector_request_id_config_network_sms:
         /* status = app_start_network_sms(data); TODO */
    	 status = connector_callback_continue;
         break;
#endif

#if !(defined CONNECTOR_WAN_TYPE)
     case connector_request_id_config_wan_type:
         /* status = app_get_wan_type(data); TODO */
    	 status = connector_callback_continue;
         break;
#endif

     case connector_request_id_config_esn:
         /* status = app_get_esn(data); TODO */
    	 status = connector_callback_continue;
         break;

     case connector_request_id_config_meid:
         /* status = app_get_meid(data); TODO */
    	 status = connector_callback_continue;
         break;

#if !(defined CONNECTOR_IDENTITY_VERIFICATION)
     case connector_request_id_config_identity_verification:
         status = app_get_identity_verification(data);
         break;
#endif

     case connector_request_id_config_password:
         status = app_get_password(data);
         break;

    default:
        APP_DEBUG("app_config_callback: unknown configuration request= %d\n", request_id);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}
