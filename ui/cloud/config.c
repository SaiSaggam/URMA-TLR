/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
****************************************************************************/
#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include "connector_config.h"
#include "connector_debug.h"
#include "connector_api.h"
#include "platform.h"

#include "ip_helper.h"
#include "odm_specific.h"
#include "config_mgr.h"
#include "cc_status.h"

/* Cloud Connector Configuration routines */

/* need to move this declaration to appropriate header in cc_ansic */
extern connector_callback_status_t app_get_interface_ip_address(uint8_t ** ip_address, size_t * size);

/*
 * Get the IP address of the connected interface. Use network_tcp_ssl.c to store the address.
 */
static connector_callback_status_t app_get_ip_address(connector_config_ip_address_t * const config_ip)
{
    size_t len;
    connector_callback_status_t status = app_get_interface_ip_address(&config_ip->address, &len);

    config_ip->ip_address_type = connector_ip_address_ipv4;

    return status;
}

static connector_callback_status_t app_get_mac_addr(connector_config_pointer_data_t * const config_mac)
{
    config_mac->data = cc_get_mac_addr();

    return connector_callback_continue;
}

static connector_callback_status_t app_load_device_id(connector_config_pointer_data_t * const config_device_id)
{
    config_device_id->data = cc_get_device_id();

    return connector_callback_continue;
}

static connector_callback_status_t app_save_device_id(connector_config_pointer_data_t * const config_device_id)
{
    return connector_callback_continue;
}

static connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor)
{
    uint32_t const device_vendor_id = 0xFE000009;

    config_vendor->id = device_vendor_id;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_device_type(connector_config_pointer_string_t * const config_device_type)
{
    config_device_type->string = cc_get_device_type();
    config_device_type->length = strlen(config_device_type->string);

    return connector_callback_continue;
}

static connector_callback_status_t app_get_device_cloud_url(connector_config_pointer_string_t * const config_url)
{
    config_url->string = cc_get_cloud_url();
    config_url->length = strlen(config_url->string);

    return connector_callback_continue;
}

/* Configure the phone number of the server where to send SMSs.
 * Will be updated if a SMSs provisioning message arrives from the server.
 * If set to nothing, will require a provisioning message from the server for initialization.
 */
static char connector_cloud_phone[] = "447786201216"; /* phone number corresponding to login.etherios.com */

static connector_callback_status_t app_get_device_cloud_phone(connector_config_pointer_string_t * const config_phone)
{

    config_phone->string = (char *)connector_cloud_phone;
    config_phone->length = sizeof connector_cloud_phone - 1;

    return connector_callback_continue;
}

static connector_callback_status_t app_set_device_cloud_phone(connector_config_pointer_string_t * const config_phone)
{
    if (config_phone->length > (sizeof connector_cloud_phone -1))
    {
        APP_DEBUG("app_set_device_cloud_phone: Not enough room to store cloud_phone.\n");
        return connector_callback_error;
    }

    strcpy(connector_cloud_phone, config_phone->string);

    /* Maybe user want to save connector_cloud_phone to persistent storage */

    return connector_callback_continue;
}

/* Service-Id used to communicate with the server through SMS
 * if empty, not shared-code used (default when using long codes)
 * When using shared-codes within US you may need to use "idgp" 
 */
static char connector_cloud_service_id[] = "";  /* empty: No shared-code used */

static connector_callback_status_t app_get_device_cloud_service_id(connector_config_pointer_string_t * const config_service_id)
{

    config_service_id->string = (char *)connector_cloud_service_id;
    config_service_id->length = strlen(connector_cloud_service_id);

    return connector_callback_continue;
}

static connector_callback_status_t app_get_connection_type(connector_config_connection_type_t * const config_connection)
{
    /* Return pointer to connection type */
    config_connection->type = connector_connection_type_lan;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_link_speed(connector_config_link_speed_t * const config_link)
{
    config_link->speed = 0;

    return connector_callback_continue;
}

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

static uint16_t get_ka_interval()
{
#define DEVICE_KEEPALIVE_INTERVAL_IN_SECONDS  60
    uint16_t ka_interval = DEVICE_KEEPALIVE_INTERVAL_IN_SECONDS;
    unsigned int * ka_seconds = NULL;
    bool const is_cellular = false; /* @TODO: find out IF name from IP address */
    config_cloud_t const element = is_cellular ? config_cloud_keepalive_cellular : config_cloud_keepalive;

    if (tlr_get_element_alloc_value(config_cloud, 1, element, (void **)&ka_seconds, config_mgr_element_type_uint32, config_mgr_access_level_read_only) == config_status_success)
    {
        ka_interval = (uint16_t)(*ka_seconds);
    }

    free(ka_seconds);

    return ka_interval;
}

/* Keep alives are from the prospective of Device Cloud */
/* This keep alive is sent from Device Cloud to the device */
static connector_callback_status_t app_get_tx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    /* Return Tx keepalive interval in seconds */
    config_keepalive->interval_in_seconds = get_ka_interval();

    return connector_callback_continue;
}

/* This keep alive is sent from the device to Device Cloud  */
static connector_callback_status_t app_get_rx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    config_keepalive->interval_in_seconds = get_ka_interval();

    return connector_callback_continue;
}

static connector_callback_status_t app_get_wait_count(connector_config_wait_count_t * const config_wait)
{
    connector_callback_status_t status = connector_callback_error;
    unsigned int * wait_count = NULL;

    if (tlr_get_element_alloc_value(config_cloud, 1, config_cloud_keepalive_count, (void **)&wait_count, config_mgr_element_type_uint32, config_mgr_access_level_read_only) != config_status_success)
        goto done;

    config_wait->count = (uint16_t)(*wait_count);
    status = connector_callback_continue;

done:
    free(wait_count);
    return status;
}


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

static connector_callback_status_t app_start_network_tcp(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = connector_connect_auto;
    return connector_callback_continue;
}

static connector_callback_status_t app_start_network_udp(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = connector_connect_auto;
    return connector_callback_continue;
}

static connector_callback_status_t app_start_network_sms(connector_config_connect_type_t * const config_connect)
{
    config_connect->type = connector_connect_auto;
    return connector_callback_continue;
}

static connector_callback_status_t app_get_identity_verification(connector_config_identity_verification_t * const config_identity)
{

    config_identity->type = connector_identity_verification_simple;

    return connector_callback_continue;
}

static connector_callback_status_t app_get_password(connector_config_pointer_string_t * const config_password)
{
    static  char const connector_password[] = "";

    /* Return pointer to password. */
    config_password->string = (char *)connector_password;
    config_password->length = sizeof connector_password -1;

    return connector_callback_continue;
}

#if (defined CONNECTOR_TRANSPORT_UDP) && !(defined CONNECTOR_SM_UDP_MAX_SESSIONS)
static connector_callback_status_t app_get_sm_udp_max_sessions(connector_config_sm_max_sessions_t * const config_max_sessions)
{
#define APP_MAX_SM_UDP_SESSIONS      32
    /*
     * Return max sessions for UDP transport 
     */
    config_max_sessions->max_sessions = APP_MAX_SM_UDP_SESSIONS;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_TRANSPORT_SMS) && !(defined CONNECTOR_SM_SMS_MAX_SESSIONS)
static connector_callback_status_t app_get_sm_sms_max_sessions(connector_config_sm_max_sessions_t * const config_max_sessions)
{
#define APP_MAX_SM_SMS_SESSIONS      32
    /*
     * Return max sessions for SMS transport 
     */
    config_max_sessions->max_sessions = APP_MAX_SM_SMS_SESSIONS;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_TRANSPORT_UDP) && !(defined CONNECTOR_SM_UDP_MAX_RX_SEGMENTS)
static connector_callback_status_t app_get_sm_udp_max_rx_segments(connector_config_sm_max_rx_segments_t * const config_max_rx_segments)
{
#define APP_MAX_SM_UDP_RX_SEGMENTS      1
    /*
     * Return max rx segments for UDP transport 
     */
    config_max_rx_segments->max_rx_segments = APP_MAX_SM_UDP_RX_SEGMENTS;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_TRANSPORT_SMS) && !(defined CONNECTOR_SM_SMS_MAX_RX_SEGMENTS)
static connector_callback_status_t app_get_sm_sms_max_rx_segments(connector_config_sm_max_rx_segments_t * const config_max_rx_segments)
{
#define APP_MAX_SM_SMS_RX_SEGMENTS      1
    /*
     * Return max rx segments for SMS transport 
     */
    config_max_rx_segments->max_rx_segments = APP_MAX_SM_SMS_RX_SEGMENTS;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_TRANSPORT_UDP) && !(defined CONNECTOR_SM_UDP_RX_TIMEOUT)
static connector_callback_status_t app_get_sm_udp_rx_timeout(connector_config_sm_rx_timeout_t * const config_rx_timeout)
{
#define APP_UDP_RX_TIMEOUT SM_WAIT_FOREVER /* In seconds, SM_WAIT_FOREVER: Wait forever for the complete request/response */
    /*
     * Return rx_timeout for UDP transport 
     */
    config_rx_timeout->rx_timeout = APP_UDP_RX_TIMEOUT;

    return connector_callback_continue;
}
#endif

#if (defined CONNECTOR_TRANSPORT_SMS) && !(defined CONNECTOR_SM_SMS_RX_TIMEOUT)
static connector_callback_status_t app_get_sm_sms_rx_timeout(connector_config_sm_rx_timeout_t * const config_rx_timeout)
{
#define APP_SMS_RX_TIMEOUT SM_WAIT_FOREVER /* In seconds, SM_WAIT_FOREVER: Wait forever for the complete request/response */
    /*
     * Return rx_timeout for SMS transport 
     */
    config_rx_timeout->rx_timeout = APP_SMS_RX_TIMEOUT;

    return connector_callback_continue;
}
#endif

/* End of Cloud Connector configuration routines */
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
        enum_to_case(connector_request_id_config_set_device_id);
        enum_to_case(connector_request_id_config_vendor_id);
        enum_to_case(connector_request_id_config_device_type);
        enum_to_case(connector_request_id_config_device_cloud_url);
        enum_to_case(connector_request_id_config_get_device_cloud_phone);
        enum_to_case(connector_request_id_config_set_device_cloud_phone);
        enum_to_case(connector_request_id_config_device_cloud_service_id);
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
        enum_to_case(connector_request_id_config_network_tcp);
        enum_to_case(connector_request_id_config_network_udp);
        enum_to_case(connector_request_id_config_network_sms);
        enum_to_case(connector_request_id_config_identity_verification);
        enum_to_case(connector_request_id_config_password);
        enum_to_case(connector_request_id_config_sm_udp_max_sessions);
        enum_to_case(connector_request_id_config_sm_sms_max_sessions);
        enum_to_case(connector_request_id_config_sm_udp_max_rx_segments);
        enum_to_case(connector_request_id_config_sm_sms_max_rx_segments);
        enum_to_case(connector_request_id_config_sm_udp_rx_timeout);
        enum_to_case(connector_request_id_config_sm_sms_rx_timeout);
        enum_to_case(connector_request_id_config_rci_descriptor_data);
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
#if (defined RCI_LEGACY_COMMANDS)
        enum_to_case(connector_request_id_remote_config_do_command);
        enum_to_case(connector_request_id_remote_config_reboot);
        enum_to_case(connector_request_id_remote_config_set_factory_def);
#endif
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
        enum_to_case(connector_request_id_sm_request_connect);
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
 * This routine is called when a configuration error is encountered by Cloud Connector.
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

#if (defined CONNECTOR_RCI_SERVICE)
static connector_callback_status_t app_load_rci_descriptor_data(connector_config_rci_descriptor_data_t * const config_rci_descriptor_data)
{
    connector_callback_status_t status = connector_callback_continue;

    config_rci_descriptor_data->rci_data = rci_descriptor_data; /* Autogenerated in remote_config.c by ConfigGenerator.jar tool */
    return status;
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
        status = app_load_device_id(data);
        break;

    case connector_request_id_config_set_device_id:
        status = app_save_device_id(data);
        break;

    case connector_request_id_config_mac_addr:
        status = app_get_mac_addr(data);
        break;

    case connector_request_id_config_vendor_id:
        status = app_get_vendor_id(data);
        break;

    case connector_request_id_config_device_type:
        status = app_get_device_type(data);
        break;

    case connector_request_id_config_device_cloud_url:
        status = app_get_device_cloud_url(data);
        break;

    case connector_request_id_config_get_device_cloud_phone:
        status = app_get_device_cloud_phone(data);
        break;

    case connector_request_id_config_set_device_cloud_phone:
        status = app_set_device_cloud_phone(data);
        break;

    case connector_request_id_config_device_cloud_service_id:
        status = app_get_device_cloud_service_id(data);
        break;

    case connector_request_id_config_connection_type:
        status = app_get_connection_type(data);
        break;

    case connector_request_id_config_link_speed:
        status = app_get_link_speed(data);
        break;

    case connector_request_id_config_phone_number:
        status = app_get_phone_number(data);
       break;

    case connector_request_id_config_tx_keepalive:
        status = app_get_tx_keepalive_interval(data);
        break;

    case connector_request_id_config_rx_keepalive:
        status = app_get_rx_keepalive_interval(data);
        break;

    case connector_request_id_config_wait_count:
        status = app_get_wait_count(data);
        break;

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

     case connector_request_id_config_network_tcp:
         status = app_start_network_tcp(data);
         break;

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

     case connector_request_id_config_identity_verification:
         status = app_get_identity_verification(data);
         break;

     case connector_request_id_config_password:
         status = app_get_password(data);
         break;

#if (defined CONNECTOR_TRANSPORT_UDP) && !(defined CONNECTOR_SM_UDP_MAX_SESSIONS)
    case connector_request_id_config_sm_udp_max_sessions:
        status = app_get_sm_udp_max_sessions(data);
        break;
#endif

#if (defined CONNECTOR_TRANSPORT_SMS) && !(defined CONNECTOR_SM_SMS_MAX_SESSIONS)
    case connector_request_id_config_sm_sms_max_sessions:
        status = app_get_sm_sms_max_sessions(data);
        break;
#endif

#if (defined CONNECTOR_TRANSPORT_UDP) && !(defined CONNECTOR_SM_UDP_MAX_RX_SEGMENTS)
    case connector_request_id_config_sm_udp_max_rx_segments:
        status = app_get_sm_udp_max_rx_segments(data);
        break;
#endif

#if (defined CONNECTOR_TRANSPORT_SMS) && !(defined CONNECTOR_SM_SMS_MAX_RX_SEGMENTS)
    case connector_request_id_config_sm_sms_max_rx_segments:
        status = app_get_sm_sms_max_rx_segments(data);
        break;
#endif

#if (defined CONNECTOR_TRANSPORT_UDP) && !(defined CONNECTOR_SM_UDP_RX_TIMEOUT)
    case connector_request_id_config_sm_udp_rx_timeout:
        status = app_get_sm_udp_rx_timeout(data);
        break;
#endif

#if (defined CONNECTOR_TRANSPORT_SMS) && !(defined CONNECTOR_SM_SMS_RX_TIMEOUT)
    case connector_request_id_config_sm_sms_rx_timeout:
        status = app_get_sm_sms_rx_timeout(data);
        break;
#endif

#if (defined CONNECTOR_RCI_SERVICE)
    case connector_request_id_config_rci_descriptor_data:
         status = app_load_rci_descriptor_data(data);
         break;
#endif

    default:
        APP_DEBUG("app_config_callback: unknown configuration request= %d\n", request_id);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}

