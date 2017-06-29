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
#include <net.h>
#include "connector_debug.h"
#include <connector_api.h>
#include <platform.h>
#include "connector_config.h"

#define  APP_IP_ADDR_STR_UNKNOWN                        "xxx.xxx.xxx.xxx"

/*
 * Routine to get the IP address, you will need to modify this routine for your
 * platform.
 */
static connector_callback_status_t app_get_ip_address(connector_config_ip_address_t * const config_ip)
{
    connector_callback_status_t status = connector_callback_abort;

    NET_IF_NBR if_nbr = NetIF_GetDflt();
    NET_IP_ADDRS_QTY  addr_ip_tbl_qty;
    NET_IP_ADDR addr_ip_tbl[NET_IP_CFG_IF_MAX_NBR_ADDR];
    static NET_IP_ADDR addr_ip;
    CPU_CHAR addr_ip_str[NET_ASCII_LEN_MAX_ADDR_IP];
    NET_ERR err_net;

    addr_ip_tbl_qty = sizeof(addr_ip_tbl) / sizeof(NET_IP_ADDR);

    (void)NetIP_GetAddrHost((NET_IF_NBR        ) if_nbr,
                            (NET_IP_ADDR      *)&addr_ip_tbl[0],
                            (NET_IP_ADDRS_QTY *)&addr_ip_tbl_qty,
                            (NET_ERR          *)&err_net);
    switch (err_net) {
        case NET_IP_ERR_NONE:
             status = connector_callback_continue;
             break;

            case NET_IF_ERR_INVALID_IF:
            case NET_IP_ERR_NULL_PTR:
            case NET_IP_ERR_ADDR_CFG_IN_PROGRESS:
            case NET_IP_ERR_ADDR_TBL_SIZE:
            case NET_IP_ERR_ADDR_NONE_AVAIL:
            default:
                (void)Str_Copy_N((CPU_CHAR *)&addr_ip_str[0],
                                 (CPU_CHAR *) APP_IP_ADDR_STR_UNKNOWN,
                                 (CPU_SIZE_T) sizeof(addr_ip_str));
                 break;
    }

    NetASCII_IP_to_Str((NET_IP_ADDR) addr_ip_tbl[0],
                       (CPU_CHAR  *) addr_ip_str,
                       (CPU_BOOLEAN) DEF_NO,
                       (NET_ERR   *)&err_net);
    APP_DEBUG("get_ip_address: IP Address: %s\n\n", addr_ip_str);

    addr_ip =  NET_UTIL_HOST_TO_NET_32(addr_ip_tbl[0]);

    config_ip->ip_address_type = connector_ip_address_ipv4;
    config_ip->address = (uint8_t *)&addr_ip;

    return status;
}

connector_callback_status_t app_get_mac_addr(connector_config_pointer_data_t * const config_mac)
{
    static uint8_t device_mac_addr[NET_IF_ETHER_ADDR_SIZE] = {0};
    NET_IF_NBR if_nbr;
    NET_IF *pif;
    NET_IF_DATA_802x  *p_if_data;
    NET_ERR err_net;
    connector_callback_status_t status = connector_callback_abort;

    if_nbr = NetIF_GetDflt();

    pif = NetIF_Get (if_nbr, &err_net);
    if (err_net != NET_IF_ERR_NONE)
        goto done;

    p_if_data = (NET_IF_DATA_802x *)pif->IF_Data;
    NET_UTIL_VAL_COPY(  device_mac_addr,
                        &p_if_data->HW_Addr[0],
                        NET_IF_802x_ADDR_SIZE);

    config_mac->data = (uint8_t *)device_mac_addr;
    status = connector_callback_continue;

done:
    return status;
}

static connector_callback_status_t app_get_device_id(connector_config_pointer_data_t * const config_device_id)
{
#if defined(CONNECTOR_CUSTOMIZE_GET_DEVICE_ID_METHOD)
    return app_custom_get_device_id(id, size);
#else
 #define DEVICE_ID_LENGTH    16

    static uint8_t device_id[DEVICE_ID_LENGTH] = {0};
    connector_callback_status_t status;
    connector_config_pointer_data_t config_mac = { 0, NET_IF_ETHER_ADDR_SIZE} ;

    /* Use the MAC address to format the device ID */
    //config_mac.bytes_required = (size_t)NET_IF_ETHER_ADDR_SIZE;
    status = app_get_mac_addr(&config_mac);
    if (status == connector_callback_continue)
    {
        device_id[8] = config_mac.data[0];
        device_id[9] = config_mac.data[1];
        device_id[10] = config_mac.data[2];
        device_id[11] = 0xFF;
        device_id[12] = 0xFF;
        device_id[13] = config_mac.data[3];
        device_id[14] = config_mac.data[4];
        device_id[15] = config_mac.data[5];

        config_device_id->data = device_id;
    }

    return status;
#endif
}

#if !(defined CONNECTOR_VENDOR_ID)
uint32_t device_vendor_id = 0x00000000;   /* Filled by connector_config.c */
static connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor)
{
    config_vendor->id  =  device_vendor_id;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_DEVICE_TYPE)
static connector_callback_status_t app_get_device_type(connector_config_pointer_string_t * const config_device_type)
{
    static char const device_type[] = "Linux Application";

    /* Return pointer to device type. */
    config_device_type->string = (char *)device_type;
    config_device_type->length = sizeof device_type -1;

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

#if !(defined CONNECTOR_CONNECTION_TYPE)
static connector_callback_status_t app_get_connection_type(connector_config_connection_type_t * const config_connection)
{

    /* Return pointer to connection type */
    config_connection->type = connector_connection_type_lan;

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

#if !(defined CONNECTOR_WAN_PHONE_NUMBER_DIALED)
static connector_callback_status_t app_get_phone_number(connector_config_pointer_string_t * const config_phone_number)
{
    /*
     * Return pointer to phone number for WAN connection type.
     */
    static char const phone_number[] ="000-000-0000";

    config_phone_number->string = (char *)phone_number;
    config_phone_number->length = sizeof phone_number -1;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_TX_KEEPALIVE_IN_SECONDS)
/* Keep alives are from the prospective of Device Cloud */
/* This keep alive is sent from Device Cloud to the device */
static connector_callback_status_t app_get_tx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{

#define DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS     90
    /* Return Tx keepalive interval in seconds */
    config_keepalive->interval_in_seconds = DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_RX_KEEPALIVE_IN_SECONDS)
/* This keep alive is sent from the device to the cloud  */
static connector_callback_status_t app_get_rx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
#define DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS     60
    /* Return Rx keepalive interval in seconds */
    config_keepalive->interval_in_seconds = DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_WAIT_COUNT)
static connector_callback_status_t app_get_wait_count(connector_config_wait_count_t * const config_wait)
{
#define DEVICE_WAIT_COUNT     5
    /*
     * Return wait count (number of times not receiving Tx keepalive
     * from Device Cloud is allowed).
     */
    config_wait->count = DEVICE_WAIT_COUNT;

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
static connector_callback_status_t app_get_max_message_transactions(connector_config_max_transaction_t * const config_max_transaction)
{
#define CONNECTOR_MAX_MSG_TRANSACTIONS   1

    config_max_transaction->count = CONNECTOR_MAX_MSG_TRANSACTIONS;

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

static void get_hex_digit(char str, uint8_t * const value)
{

    if (ASCII_IsDig(str))
    {
        *value = str - '0';
    }
    else if (ASCII_IsDig(str))
    {
        int const hex = ASCII_ToLower(str);
        *value = (hex - 'a') + 10;
    }
    else
    {
        APP_DEBUG("get_hex_digit: invalid digit %c\n", str);
    }

    return;
}

static connector_callback_status_t app_get_imei_number(connector_config_pointer_data_t * const config_imei)
{
#define APP_IMEI_LENGTH 8

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
                uint8_t value;
                get_hex_digit(app_imei_number_string[i], &value);
                app_imei_number[index] += (value << ((uint8_t)n * 4));
                n++;
            }
        }
        index--;
    }

    config_imei->data = app_imei_number;
    ASSERT(config_imei->bytes_required == sizeof app_imei_number);
    return connector_callback_continue;
}

#if !(defined CONNECTOR_NETWORK_TCP_START)
static connector_callback_status_t app_start_network_tcp(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = connector_connect_auto;
    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_NETWORK_UDP_START)
static connector_callback_status_t app_start_network_udp(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = connector_connect_auto;
    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_NETWORK_SMS_START)
static connector_callback_status_t app_start_network_sms(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = connector_connect_auto;
    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_WAN_TYPE)
static connector_callback_status_t app_get_wan_type(connector_config_wan_type_t * const config_wan)
{

    config_wan->type = connector_wan_type_imei;

    return connector_callback_continue;
}
#endif

static connector_callback_status_t app_get_esn(connector_config_pointer_data_t * const config_esn)
{
#define APP_ESN_HEX_LENGTH 4

    /* Each nibble corresponds a decimal digit.
     * Most upper nibble must be 0.
     */
    static char const app_esn_hex_string[] = "00000000";
    static uint8_t app_esn_hex[APP_ESN_HEX_LENGTH] = {0};
    int i = sizeof app_esn_hex_string -1;
    int index = sizeof app_esn_hex -1;

    while (i > 0)
    {
        int n = 0;

        app_esn_hex[index] = 0;

        /* loop 2 times here for 2 digits (1 bytes) */
        while (n < 2 && i > 0)
        {
            i--;
            if (app_esn_hex_string[i] != '-')
            {
                uint8_t value;
                get_hex_digit(app_esn_hex_string[i], &value);
                app_esn_hex[index] += (value << ((uint8_t)n * 4));
                n++;
            }
        }
        index--;
    }

    config_esn->data = app_esn_hex;
    ASSERT(config_esn->bytes_required == sizeof app_esn_hex);

    return connector_callback_continue;
}

static connector_callback_status_t app_get_meid(connector_config_pointer_data_t * const config_meid)
{
#define APP_MEID_HEX_LENGTH 7

    /* Each nibble corresponds a decimal digit.
     * Most upper nibble must be 0.
     */
    static char const app_meid_hex_string[] = "00000000000000";
    static uint8_t app_meid_hex[APP_MEID_HEX_LENGTH] = {0};
    int i = sizeof app_meid_hex_string -1;
    int index = sizeof app_meid_hex -1;

    while (i > 0)
    {
        int n = 0;

        app_meid_hex[index] = 0;

        /* loop 2 times here for 2 digits (1 bytes) */
        while (n < 2 && i > 0)
        {
            i--;
            if (app_meid_hex_string[i] != '-')
            {
                uint8_t value;
                get_hex_digit(app_meid_hex_string[i], &value);
                app_meid_hex[index] += (value << ((uint8_t)n * 4));
                n++;
            }
        }
        index--;
    }

    config_meid->data = app_meid_hex;
    ASSERT(config_meid->bytes_required == sizeof app_meid_hex);

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
    config_password->length = sizeof connector_password -1;

    return connector_callback_continue;
}

/* End of Etherios Cloud Connector configuration routines */
#if (defined CONNECTOR_DEBUG)

#define enum_to_case(name)  case name:  result = #name;             break

static char const * app_class_to_string(connector_class_id_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_class_id_config);
        enum_to_case(connector_class_id_operating_system);
        enum_to_case(connector_class_id_firmware);
        enum_to_case(connector_class_id_data_service);
        enum_to_case(connector_class_id_remote_config);
        enum_to_case(connector_class_id_file_system);
        enum_to_case(connector_class_id_network_tcp);
        enum_to_case(connector_class_id_network_udp);
        enum_to_case(connector_class_id_network_sms);
        enum_to_case(connector_class_id_status);
        enum_to_case(connector_class_id_short_message);
        enum_to_case(connector_class_id_data_point);
    }
    return result;
}

static char const * app_config_class_to_string(connector_request_id_config_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_config_device_id);
        enum_to_case(connector_request_id_config_vendor_id);
        enum_to_case(connector_request_id_config_device_type);
        enum_to_case(connector_request_id_config_device_cloud_url);
        enum_to_case(connector_request_id_config_connection_type);
        enum_to_case(connector_request_id_config_mac_addr);
        enum_to_case(connector_request_id_config_link_speed);
        enum_to_case(connector_request_id_config_phone_number);
        enum_to_case(connector_request_id_config_tx_keepalive);
        enum_to_case(connector_request_id_config_rx_keepalive);
        enum_to_case(connector_request_id_config_wait_count);
        enum_to_case(connector_request_id_config_ip_addr);
        enum_to_case(connector_request_id_config_error_status);
        enum_to_case(connector_request_id_config_firmware_facility);
        enum_to_case(connector_request_id_config_data_service);
        enum_to_case(connector_request_id_config_file_system);
        enum_to_case(connector_request_id_config_remote_configuration);
        enum_to_case(connector_request_id_config_max_transaction);
        enum_to_case(connector_request_id_config_device_id_method);
        enum_to_case(connector_request_id_config_imei_number);
        enum_to_case(connector_request_id_config_network_tcp);
        enum_to_case(connector_request_id_config_network_udp);
        enum_to_case(connector_request_id_config_network_sms);
        enum_to_case(connector_request_id_config_wan_type);
        enum_to_case(connector_request_id_config_esn);
        enum_to_case(connector_request_id_config_meid);
        enum_to_case(connector_request_id_config_identity_verification);
        enum_to_case(connector_request_id_config_password);
    }
    return result;
}

static char const * app_network_class_to_string(connector_request_id_network_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_network_open);
        enum_to_case(connector_request_id_network_send);
        enum_to_case(connector_request_id_network_receive);
        enum_to_case(connector_request_id_network_close);
    }
    return result;
}

static char const * app_os_class_to_string(connector_request_id_os_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_os_malloc);
        enum_to_case(connector_request_id_os_free);
        enum_to_case(connector_request_id_os_realloc);
        enum_to_case(connector_request_id_os_system_up_time);
        enum_to_case(connector_request_id_os_yield);
        enum_to_case(connector_request_id_os_reboot);
    }
    return result;
}

#if (defined CONNECTOR_FIRMWARE_SERVICE)
static char const * app_firmware_class_to_string(connector_request_id_firmware_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_firmware_target_count);
        enum_to_case(connector_request_id_firmware_info);
        enum_to_case(connector_request_id_firmware_download_start);
        enum_to_case(connector_request_id_firmware_download_data);
        enum_to_case(connector_request_id_firmware_download_complete);
        enum_to_case(connector_request_id_firmware_download_abort);
        enum_to_case(connector_request_id_firmware_target_reset);
    }
    return result;
}
#endif

#if (defined CONNECTOR_RCI_SERVICE)
static char const * app_remote_config_class_to_string(connector_request_id_remote_config_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_remote_config_session_start);
        enum_to_case(connector_request_id_remote_config_action_start);
        enum_to_case(connector_request_id_remote_config_group_start);
        enum_to_case(connector_request_id_remote_config_group_process);
        enum_to_case(connector_request_id_remote_config_group_end);
        enum_to_case(connector_request_id_remote_config_action_end);
        enum_to_case(connector_request_id_remote_config_session_end);
        enum_to_case(connector_request_id_remote_config_session_cancel);
    }
    return result;
}
#endif

#if (defined CONNECTOR_FILE_SYSTEM)
static char const * app_file_system_class_to_string(connector_request_id_file_system_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_file_system_open);
        enum_to_case(connector_request_id_file_system_read);
        enum_to_case(connector_request_id_file_system_write);
        enum_to_case(connector_request_id_file_system_lseek);
        enum_to_case(connector_request_id_file_system_ftruncate);
        enum_to_case(connector_request_id_file_system_close);
        enum_to_case(connector_request_id_file_system_remove);
        enum_to_case(connector_request_id_file_system_stat);
        enum_to_case(connector_request_id_file_system_stat_dir_entry);
        enum_to_case(connector_request_id_file_system_opendir);
        enum_to_case(connector_request_id_file_system_readdir);
        enum_to_case(connector_request_id_file_system_closedir);
        enum_to_case(connector_request_id_file_system_get_error);
        enum_to_case(connector_request_id_file_system_session_error);
        enum_to_case(connector_request_id_file_system_hash);
    }
    return result;
}
#endif

#if (defined CONNECTOR_DATA_SERVICE)
static char const * app_data_service_class_to_string(connector_request_id_data_service_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_data_service_send_length);
        enum_to_case(connector_request_id_data_service_send_data);
        enum_to_case(connector_request_id_data_service_send_status);
        enum_to_case(connector_request_id_data_service_send_response);
        enum_to_case(connector_request_id_data_service_receive_target);
        enum_to_case(connector_request_id_data_service_receive_data);
        enum_to_case(connector_request_id_data_service_receive_status);
        enum_to_case(connector_request_id_data_service_receive_reply_length);
        enum_to_case(connector_request_id_data_service_receive_reply_data);
    }
    return result;
}
#endif

#if (defined CONNECTOR_DATA_POINTS)
static char const * app_data_point_class_to_string(connector_request_id_data_point_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_data_point_binary_response);
        enum_to_case(connector_request_id_data_point_binary_status);
        enum_to_case(connector_request_id_data_point_response);
        enum_to_case(connector_request_id_data_point_status);
    }
    return result;
}
#endif

static char const * app_status_class_to_string(connector_request_id_status_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_status_tcp);
        enum_to_case(connector_request_id_status_stop_completed);
    }
    return result;
}

#if (defined CONNECTOR_SHORT_MESSAGE)
static char const * app_sm_class_to_string(connector_request_id_sm_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_request_id_sm_ping_request);
        enum_to_case(connector_request_id_sm_ping_response);
        enum_to_case(connector_request_id_sm_cli_request);
        enum_to_case(connector_request_id_sm_cli_response);
        enum_to_case(connector_request_id_sm_cli_response_length);
        enum_to_case(connector_request_id_sm_cli_status);
        enum_to_case(connector_request_id_sm_more_data);
        enum_to_case(connector_request_id_sm_opaque_response);
        enum_to_case(connector_request_id_sm_config_request);
    }
    return result;
}
#endif

static char const * app_status_error_to_string(connector_status_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_success);
        enum_to_case(connector_init_error);
        enum_to_case(connector_abort);
        enum_to_case(connector_invalid_data_size);
        enum_to_case(connector_invalid_data_range);
        enum_to_case(connector_keepalive_error);
        enum_to_case(connector_invalid_data);
        enum_to_case(connector_device_terminated);
        enum_to_case(connector_service_busy);
        enum_to_case(connector_invalid_response);
        enum_to_case(connector_no_resource);
        enum_to_case(connector_unavailable);
        enum_to_case(connector_idle);
        enum_to_case(connector_working);
        enum_to_case(connector_pending);
        enum_to_case(connector_active);
        enum_to_case(connector_device_error);
        enum_to_case(connector_open_error);

        enum_to_case(connector_invalid_payload_packet);
        enum_to_case(connector_bad_version);
        enum_to_case(connector_exceed_timeout);

    }
    return result;
}

/*
 * This routine is called when a configuration error is encountered by Etherios Cloud Connector.
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

#if (defined CONNECTOR_DATA_POINTS)
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

#if !(defined CONNECTOR_WAN_PHONE_NUMBER_DIALED)
    case connector_request_id_config_phone_number:
        status = app_get_phone_number(data);
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
         status = app_get_imei_number(data);
         break;

#if !(defined CONNECTOR_NETWORK_TCP_START)
     case connector_request_id_config_network_tcp:
         status = app_start_network_tcp(data);
         break;
#endif

#if !(defined CONNECTOR_NETWORK_UDP_START)
     case connector_request_id_config_network_udp:
         status = app_start_network_udp(data);
         break;
#endif

#if !(defined CONNECTOR_NETWORK_SMS_START)
     case connector_request_id_config_network_sms:
         status = app_start_network_sms(data);
         break;
#endif

#if !(defined CONNECTOR_WAN_TYPE)
     case connector_request_id_config_wan_type:
         status = app_get_wan_type(data);
         break;
#endif

     case connector_request_id_config_esn:
         status = app_get_esn(data);
         break;

     case connector_request_id_config_meid:
         status = app_get_meid(data);
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
