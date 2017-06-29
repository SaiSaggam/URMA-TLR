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
#include <stdio.h>
#include <stdlib.h>
#include "sockapi.h"
#include "iam.hh"
#include "iam_netos.hh"
#include "naerrno.h"
#include "boardParams.h"

#include "connector_config.h"
#include "connector_debug.h"
#include "connector_api.h"
#include "platform.h"


/* iDigi Connector Configuration routines */

/*
 * Routine to get the IP address, you will need to modify this routine for your 
 * platform.
 */
static connector_callback_status_t app_get_ip_address(uint8_t const ** ip_address, size_t * const size)
{
    NaIamIfAddress_t iam_addr;
    connector_callback_status_t  status = connector_callback_abort;
    static struct in_addr  ip_addr;

    NaIamStatus_t iam_status = customizeIamGetIfAddress(MY_IFNAME, AF_INET, &iam_addr);

    if (iam_status != NA_IAM_STATUS_SUCCESS)
    {
        APP_DEBUG("customizeIamGetIfAddress failed\n");
        goto error;
    }
    
    if (iam_addr.ipAddress.ss_family != AF_INET || iam_addr.ipAddress.addr.ipv4.sin_addr.s_addr == 0)
    {
        APP_DEBUG("IP address not configured\n");
        goto error;
    }

    APP_DEBUG("get_ip_address: Looking for current device IP address: found [%d] entries\n", 1);


    /* Fill in the size and IP address */
    ip_addr = iam_addr.ipAddress.addr.ipv4.sin_addr;

    APP_DEBUG("get_ip_address: Interface name [%s]\tIP Address [%s]\n", MY_IFNAME, inet_ntoa(ip_addr));

    *size       = sizeof ip_addr.s_addr;
    *ip_address = (uint8_t *)&ip_addr.s_addr;
    status = connector_callback_continue;

error:

    return status;
}



static connector_callback_status_t app_get_mac_addr(uint8_t const ** addr, size_t * const size)
{
    #define MAC_ADDR_LENGTH     6
    static uint8_t device_mac_addr[MAC_ADDR_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    connector_callback_status_t  status = connector_callback_continue;

    int result =  customizeGetMACAddress(device_mac_addr);
    if (result != 0)
    {
        APP_DEBUG("customizeGetMACAddress failed\n");
        status = connector_callback_abort;
        goto error;
    }

    *addr = device_mac_addr;
    *size = sizeof device_mac_addr;

error:
    return status;
}

static connector_callback_status_t app_get_device_id(uint8_t const ** id, size_t * const size)
{
    #define DEVICE_ID_LENGTH    16

    static uint8_t device_id[DEVICE_ID_LENGTH] = {0};
    uint8_t const * mac_addr;
    size_t mac_size;
    connector_callback_status_t status;

    /* This sample uses the MAC address to format the device ID */
    status = app_get_mac_addr(&mac_addr, &mac_size);
    if (status == connector_callback_continue)
    {
        device_id[8] = mac_addr[0];
        device_id[9] = mac_addr[1];
        device_id[10] = mac_addr[2];
        device_id[11] = 0xFF;
        device_id[12] = 0xFF;
        device_id[13] = mac_addr[3];
        device_id[14] = mac_addr[4];
        device_id[15] = mac_addr[5];

        *id   = device_id;
        *size = sizeof device_id;

    }

    return status;
}

static connector_callback_status_t app_get_vendor_id(uint8_t const ** id, size_t * const size)
{
#define VENDOR_ID_LENGTH    4
    static uint8_t const device_vendor_id[VENDOR_ID_LENGTH] = MY_DEVICE_VENDOR_ID;

    *id   =  device_vendor_id;
    *size = sizeof device_vendor_id;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_device_type(char const ** type, size_t * const size)
{
    static char const device_type[] = "NET+OS Application";

    /* Return pointer to device type. */
    *type = (char *)device_type;
    *size = sizeof device_type -1;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_connection_type(connector_connection_type_t const ** type)
{
    /* Return pointer to connection type */
    static connector_connection_type_t  device_connection_type = connector_lan_connection_type;

    *type = &device_connection_type;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_link_speed(uint32_t ** const speed, size_t * const size)
{
    UNUSED_ARGUMENT(speed);
    UNUSED_ARGUMENT(size);

    return connector_callback_continue;
}

static connector_callback_status_t app_get_phone_number(char ** const number, size_t * const size)
{
    /*
     * Return pointer to phone number for WAN connection type.
     */
    UNUSED_ARGUMENT(number);
    UNUSED_ARGUMENT(size);

    return connector_callback_continue;
}


static connector_callback_status_t app_get_server_url(char const ** url, size_t * const size)
{
    static  char const connector_server_url[] = MY_SERVER_URL;

    /* Return pointer to device type. */
    *url = connector_server_url;
    *size = sizeof connector_server_url -1;

    return connector_callback_continue;
}


/* Keep alives are from the prospective of the server */
/* This keep alive is sent from the server to the device */
static connector_callback_status_t app_get_tx_keepalive_interval(uint16_t const ** interval, size_t * const size)
{
#define DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS     90
    /* Return pointer to Tx keepalive interval in seconds */
    static uint16_t const device_tx_keepalive_interval = DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS;

    *interval = &device_tx_keepalive_interval;
    *size = sizeof device_tx_keepalive_interval;

    return connector_callback_continue;
}


/* This keep alive is sent from the device to the server  */
static connector_callback_status_t app_get_rx_keepalive_interval(uint16_t const ** interval, size_t * const size)
{
#define DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS     60
    /* Return pointer to Rx keepalive interval in seconds */
    static uint16_t const device_rx_keepalive_interval = DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS;

    *interval = &device_rx_keepalive_interval;
    *size = sizeof device_rx_keepalive_interval;

    return connector_callback_continue;
}


static connector_callback_status_t app_get_wait_count(uint16_t const ** count, size_t * const size)
{
#define DEVICE_WAIT_COUNT     5
    /*
     * Return pointer to wait count (number of times not receiving Tx keepalive
     * from server is allowed).
     */
    static uint16_t const device_wait_count = DEVICE_WAIT_COUNT;

    *count = &device_wait_count;
    *size = sizeof device_wait_count;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_firmware_support(connector_service_supported_status_t * const isSupported)
{
    *isSupported = connector_service_supported;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_data_service_support(connector_service_supported_status_t * const isSupported)
{
    *isSupported = connector_service_supported;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_file_system_support(connector_service_supported_status_t * const isSupported)
{
    *isSupported = connector_service_supported;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_remote_configuration_support(connector_service_supported_status_t * const isSupported)
{
    *isSupported = connector_service_supported;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_max_message_transactions(unsigned int * const transCount)
{
#define CONNECTOR_MAX_MSG_TRANSACTIONS   1

    *transCount = CONNECTOR_MAX_MSG_TRANSACTIONS;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_device_id_method(connector_device_id_method_t * const method)
{

    *method = connector_auto_device_id_method;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_imei_number(uint8_t ** const imei_number, size_t * size)
{
#define APP_IMEI_LENGTH 8

#error "Specify the IMEI number for WAN connection type if app_get_device_id_method returns connector_auto_device_id_method and app_get_device_id_method returns connector_imei_wan_type"
    /* Each nibble corresponds a decimal digit.
     * Most upper nibble must be 0.
     */
    static char const app_imei_number_string[] = "000000-00-000000-0";
    static uint8_t app_imei_number[APP_IMEI_LENGTH] = {0};
    int i = sizeof app_imei_number_string -1;
    int index = sizeof app_imei_number -1;

    while (i > 0)
    {
        int n = 0;

        app_imei_number[index] = 0;

        /* loop 2 times here for 2 digits (1 bytes) */
        while (n < 2 && i > 0)
        {
            i--;
            if (app_imei_number_string[i] != '-')
            {
                ASSERT(isdigit(app_imei_number_string[i]));
                app_imei_number[index] += ((app_imei_number_string[i] - '0') << (n * 4));
                n++;
            }
        }
        index--;
    }

    *imei_number = app_imei_number;
    *size = sizeof app_imei_number;

    APP_DEBUG("app_get_imei_number\n");

    return connector_callback_continue;
}

static connector_callback_status_t app_start_network_tcp(connector_auto_connect_type_t * const connect_type)
{
    *connect_type = connector_auto_connect;

    return connector_callback_continue;
}

/* End of Etherios Cloud Connector configuration routines */

/*
 * This routine is called when a configuration error is encountered by Etherios Cloud Connector.
 * This is currently used as a debug tool for finding configuration errors.
 */
static void app_config_error(connector_error_status_t const * const error_data)
{

    static char const * const error_status_string[] = {"connector_success", "connector_init_error",
                                                      "connector_abort",
                                                      "connector_invalid_data_size",
                                                      "connector_invalid_data_range",
                                                      "connector_invalid_payload_packet",
                                                      "connector_keepalive_error",
                                                      "connector_server_overload",
                                                      "connector_bad_version",
                                                      "connector_invalid_packet",
                                                      "connector_exceed_timeout",
                                                      "connector_unsupported_security",
                                                      "connector_invalid_data",
                                                      "connector_server_disconnected",
                                                      "connector_connect_error",
                                                      "connector_receive_error",
                                                      "connector_close_error",
                                                      "connector_device_terminated",
                                                      "connector_service_busy",
                                                      "connector_invalid_response"};

    static char const * config_request_string[] = { "connector_config_device_id",
                                                     "connector_config_vendor_id",
                                                     "connector_config_device_type",
                                                     "connector_config_server_url",
                                                     "connector_config_connection_type",
                                                     "connector_config_mac_addr",
                                                     "connector_config_link_speed",
                                                     "connector_config_phone_number",
                                                     "connector_config_tx_keepalive",
                                                     "connector_config_rx_keepalive",
                                                     "connector_config_wait_count",
                                                     "connector_config_ip_addr",
                                                     "connector_config_error_status",
                                                     "connector_config_firmware_facility",
                                                     "connector_config_data_service",
                                                     "connector_config_file_system",
                                                     "connector_config_remote_configuration",
                                                     "connector_config_max_transaction",
                                                     "connector_config_device_id_method",
                                                     "connector_config_imei_number"};

    static char const * const network_request_string[] = { "connector_network_open",
                                                          "connector_network_send",
                                                          "connector_network_receive",
                                                          "connector_network_close"
                                                          "connector_network_disconnected",
                                                          "connector_network_reboot"};

    static char const * const os_request_string[] = { "connector_os_malloc",
                                                     "connector_os_free",
                                                     "connector_os_system_up_time",
                                                     "connector_os_sleep"};

    static char const * const firmware_request_string[] = {"connector_firmware_target_count",
                                                          "connector_firmware_version",
                                                          "connector_firmware_code_size",
                                                          "connector_firmware_description",
                                                          "connector_firmware_name_spec",
                                                          "connector_firmware_download_request",
                                                          "connector_firmware_binary_block",
                                                          "connector_firmware_download_complete",
                                                          "connector_firmware_download_abort",
                                                          "connector_firmware_target_reset"};

    static char const * const data_service_string[] = {"connector_data_service_put_request",
                                          "connector_data_service_device_request"};

    static char const * const file_system_string[] = {"connector_file_system_open",
                                                       "connector_file_system_read",
                                                       "connector_file_system_write",
                                                       "connector_file_system_lseek",
                                                       "connector_file_system_ftruncate",
                                                       "connector_file_system_close",
                                                       "connector_file_system_rm",
                                                       "connector_file_system_stat",
                                                       "connector_file_system_opendir",
                                                       "connector_file_system_readdir",
                                                       "connector_file_system_closedir",
                                                       "connector_file_system_strerror",
                                                       "connector_file_system_msg_error",
                                                       "connector_file_system_hash"};

    static char const * const remote_config_string[]= { "connector_remote_config_session_start",
                                                        "connector_remote_config_session_end",
                                                        "connector_remote_config_action_start",
                                                        "connector_remote_config_action_end",
                                                        "connector_remote_config_group_start",
                                                        "connector_remote_config_group_end",
                                                        "connector_remote_config_group_process",
                                                        "connector_remote_config_session_cancel"};

    switch (error_data->class_id)
    {
    case connector_class_config:
        APP_DEBUG("app_config_error: Config - %s (%d)  status = %s (%d)\n", config_request_string[error_data->request_id.config_request],
                     error_data->request_id.config_request, error_status_string[error_data->status],error_data->status);
        break;
    case connector_class_network_tcp:
        APP_DEBUG("app_config_error: Network - %s (%d)  status = %s (%d)\n", network_request_string[error_data->request_id.network_request],
                     error_data->request_id.network_request, error_status_string[error_data->status],error_data->status);
        break;
    case connector_class_operating_system:
        APP_DEBUG("app_config_error: Operating System - %s (%d)  status = %s (%d)\n", os_request_string[error_data->request_id.os_request],
                     error_data->request_id.os_request, error_status_string[error_data->status],error_data->status);
        break;
    case connector_class_firmware:
        APP_DEBUG("app_config_error: Firmware facility - %s (%d)  status = %s (%d)\n",
                     firmware_request_string[error_data->request_id.firmware_request],
                     error_data->request_id.firmware_request,
                     error_status_string[error_data->status],error_data->status);
        break;
    case connector_class_id_data_service:
        APP_DEBUG("app_config_error: Data service - %s (%d)  status = %s (%d)\n",
                     data_service_string[error_data->request_id.data_service_request],
                     error_data->request_id.data_service_request,
                     error_status_string[error_data->status],error_data->status);
        break;
    case connector_class_id_file_system:
           APP_DEBUG("app_config_error: File system - %s (%d)  status = %s (%d)\n",
                        file_system_string[error_data->request_id.file_system_request],
                        error_data->request_id.file_system_request,
                        error_status_string[error_data->status],error_data->status);
           break;
    case connector_class_remote_config_service:
           APP_DEBUG("app_config_error: Remote configuration - %s (%d)  status = %s (%d)\n",
                   remote_config_string[error_data->request_id.remote_config_request],
                        error_data->request_id.remote_config_request,
                        error_status_string[error_data->status],error_data->status);
           break;
    default:
        APP_DEBUG("app_config_error: unsupport class_id = %d status = %d\n", error_data->class_id, error_data->status);
        break;
    }
}

/*
 * Configuration callback routine.
 */
connector_callback_status_t app_config_handler(connector_config_request_t const request,
                                              void const * const request_data,
                                              size_t const request_length,
                                              void * response_data,
                                              size_t * const response_length)
{
    connector_callback_status_t status;

    UNUSED_ARGUMENT(request_length);

    switch (request)
    {
    case connector_config_device_id:
        status = app_get_device_id(response_data, response_length);
        break;

    case connector_config_mac_addr:
        status = app_get_mac_addr(response_data, response_length);
        break;

    case connector_config_vendor_id:
        status = app_get_vendor_id(response_data, response_length);
        break;

    case connector_config_device_type:
        status = app_get_device_type(response_data, response_length);
        break;

    case connector_config_server_url:
        status = app_get_server_url(response_data, response_length);
        break;

    case connector_config_connection_type:
        status = app_get_connection_type(response_data);
        break;

    case connector_config_link_speed:
        status = app_get_link_speed(response_data, response_length);
        break;

    case connector_config_phone_number:
        status = app_get_phone_number(response_data, response_length);
       break;

    case connector_config_tx_keepalive:
        status = app_get_tx_keepalive_interval(response_data, response_length);
        break;

    case connector_config_rx_keepalive:
        status = app_get_rx_keepalive_interval(response_data, response_length);
        break;

    case connector_config_wait_count:
        status = app_get_wait_count(response_data, response_length);
        break;

    case connector_config_ip_addr:
        status = app_get_ip_address(response_data, response_length);
        break;

    case connector_config_error_status:
        app_config_error(request_data);
        status = connector_callback_continue;
        break;

    case connector_config_firmware_facility:
        status = app_get_firmware_support(response_data);
        break;

    case connector_config_data_service:
        status = app_get_data_service_support(response_data);
        break;

    case connector_config_file_system:
        status = app_get_file_system_support(response_data);
        break;

    case connector_config_remote_configuration:
        status = app_get_remote_configuration_support(response_data);
        break;

    case connector_config_max_transaction:
        status = app_get_max_message_transactions(response_data);
        break;

    case connector_config_device_id_method:
        status = app_get_device_id_method(response_data);
        break;

     case connector_config_imei_number:
         status = app_get_imei_number(response_data, response_length);
         break;

    default:
        APP_DEBUG("app_config_callback: unknown configuration request= %d\n", request);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}

