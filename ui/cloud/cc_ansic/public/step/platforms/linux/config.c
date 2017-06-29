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
#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <ctype.h>
#include "connector_config.h"
#include "connector_debug.h"
#include "connector_api.h"
#include "platform.h"

/* Cloud Connector Configuration routines */

/*
 * Routine to get the IP address, you will need to modify this routine for your
 * platform.
 */
static connector_callback_status_t app_get_ip_address(connector_config_ip_address_t * const config_ip)
{
    #define MAX_INTERFACES      128

    int             fd = -1;
    connector_callback_status_t  status = connector_callback_abort;
    char            *buf = malloc(MAX_INTERFACES*sizeof(struct ifreq));
    struct ifconf   conf;
    static struct in_addr  ip_addr; /* Used to store the IP address */

    if (buf == NULL)
    {
        APP_DEBUG("get_ip_address malloc failed\n");
        goto error;
    }
    conf.ifc_len = MAX_INTERFACES*sizeof(struct ifreq);
    conf.ifc_buf = buf;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        goto error;
    }

    if( ioctl(fd, SIOCGIFCONF , &conf) == -1)
    {
        APP_DEBUG("get_ip_address: Error using ioctl SIOCGIFCONF.\n");
        goto error;
    }

    {
        unsigned int    entries = 0;
        unsigned int    i;

        entries = (unsigned int)conf.ifc_len / sizeof(struct ifreq);

        APP_DEBUG("get_ip_address: Looking for current device IP address: found [%d] entries\n", entries);

        if (entries == 0)
        {
            goto error;
        }

        for( i = 0; i < entries; i++)
        {
            struct ifreq * req = &conf.ifc_req[i];
            struct sockaddr_in * sa = cast_for_alignment(struct sockaddr_in *, &req->ifr_addr);

            APP_DEBUG("get_ip_address: %d: Interface name [%s]\tIP Address [%s]\n", i+1, req->ifr_name, inet_ntoa(sa->sin_addr));
            if (sa->sin_addr.s_addr && sa->sin_addr.s_addr != htonl(INADDR_LOOPBACK))
            {
                ip_addr = sa->sin_addr;
                break;
            }
        }
    }

    /* Fill in the size and IP address */
    config_ip->ip_address_type = connector_ip_address_ipv4;
    config_ip->address = (uint8_t *)&ip_addr.s_addr;
    status = connector_callback_continue;

error:
    if (fd != -1)
    {
        close(fd);
    }
    if (buf != NULL)
    {
        free(buf);
    }
    return status;
}

#define MAC_ADDR_LENGTH     6
static uint8_t const device_mac_addr[MAC_ADDR_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static connector_callback_status_t app_get_mac_addr(connector_config_pointer_data_t * const config_mac)
{
#error "Specify device MAC address for LAN connection"
    ASSERT(config_mac->bytes_required == MAC_ADDR_LENGTH);

    config_mac->data = (uint8_t *)device_mac_addr;

    return connector_callback_continue;
}

#define DEVICE_ID_LENGTH    16
#define DEVICE_ID_FILENAME  "device_id.cfg"

static uint8_t device_id[DEVICE_ID_LENGTH] = {0x00};

#define DEVICE_ID_SOURCE_MAC                0
#define DEVICE_ID_SOURCE_IMEI               1
#define DEVICE_ID_SOURCE_MEID               2
#define DEVICE_ID_SOURCE_ESN                3
#define DEVICE_ID_SOURCE_AUTOPROVISION      4

#define IMEI_DEVICE_ID_PREFIX   1
#define ESN_DEVICE_ID_PREFIX    2
#define MEID_DEVICE_ID_PREFIX   4

#if !(defined APPLICATION_DEVICE_ID_SOURCE)
#define APPLICATION_DEVICE_ID_SOURCE    DEVICE_ID_SOURCE_MAC
#endif

#define IMEI_LENGTH     8
#define MEID_LENGTH     8
#define ESN_LENGTH      4

static const uint8_t device_imei[IMEI_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t device_meid[MEID_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t device_esn[ESN_LENGTH] = {0x00, 0x00, 0x00, 0x00};

#if (APPLICATION_DEVICE_ID_SOURCE == DEVICE_ID_SOURCE_IMEI)
#error "Specify device device's IMEI value to generate Device ID and comment this line"
#elif (APPLICATION_DEVICE_ID_SOURCE == DEVICE_ID_SOURCE_MEID)
#error "Specify device device's MEID value to generate Device ID and comment this line"
#elif (APPLICATION_DEVICE_ID_SOURCE == DEVICE_ID_SOURCE_ESN)
#error "Specify device device's ESN value to generate Device ID and comment this line"
#elif (APPLICATION_DEVICE_ID_SOURCE > DEVICE_ID_SOURCE_AUTOPROVISION || APPLICATION_DEVICE_ID_SOURCE < DEVICE_ID_SOURCE_MAC)
#error "Specify APPLICATION_DEVICE_ID_SOURCE to a valid value"
#endif

static unsigned int const device_id_source = APPLICATION_DEVICE_ID_SOURCE;

static void get_device_id_file_full_path(char const * const file_name, char * const full_path_buffer, size_t buffer_size)
{
    char proc_path[256];
    pid_t pid = getpid();

    sprintf(proc_path, "/proc/%d/exe", pid);
    if (readlink(proc_path, full_path_buffer, buffer_size) == -1)
    {
        perror("readlink");
    }
    else
    {
        char * const executable_name = strrchr(full_path_buffer, '/');

        if (executable_name != NULL)
        {
            char * const file_name_destination = executable_name + 1;
            strcpy(file_name_destination, file_name);
        }
    }
}

static connector_bool_t file_exists(char const * const file_path)
{
    connector_bool_t file_exists = connector_false;

    if (access(file_path, F_OK) != -1)
    {
        file_exists = connector_true;
    }

    return file_exists;
}

static void read_device_id_from_file(char const * const file_path, uint8_t * const device_id_buffer, size_t const device_id_size)
{
    FILE * const file = fopen(file_path, "r");
    size_t bytes_read;

    ASSERT(file != NULL);
    bytes_read = fread(device_id_buffer, sizeof device_id[0], device_id_size, file);
    ASSERT(bytes_read == device_id_size);
    APP_DEBUG("read_device_id_from_file: read %" PRIsize " bytes from %s\n", bytes_read, file_path);
    fclose(file);
}

static void write_device_id_to_file(char const * const file_path, uint8_t * const device_id_buffer, size_t const device_id_size)
{
    FILE * const file = fopen(file_path, "w+");
    size_t bytes_written;

    bytes_written = fwrite(device_id_buffer, sizeof device_id_buffer[0], device_id_size, file);
    ASSERT(bytes_written == device_id_size);
    APP_DEBUG("write_device_id_to_file: wrote %" PRIsize " bytes to %s\n", bytes_written, file_path);
    fclose(file);
}

static connector_callback_status_t app_load_device_id(connector_config_pointer_data_t * const config_device_id)
{
    switch (device_id_source)
    {
        case DEVICE_ID_SOURCE_MAC:
            device_id[8] = device_mac_addr[0];
            device_id[9] = device_mac_addr[1];
            device_id[10] = device_mac_addr[2];
            device_id[11] = 0xFF;
            device_id[12] = 0xFF;
            device_id[13] = device_mac_addr[3];
            device_id[14] = device_mac_addr[4];
            device_id[15] = device_mac_addr[5];
            break;
        case DEVICE_ID_SOURCE_IMEI:
            device_id[1] = IMEI_DEVICE_ID_PREFIX;

            device_id[8] = device_imei[0];
            device_id[9] = device_imei[1];
            device_id[10] = device_imei[2];
            device_id[11] = device_imei[3];
            device_id[12] = device_imei[4];
            device_id[13] = device_imei[5];
            device_id[14] = device_imei[6];
            device_id[15] = device_imei[7];
            break;
        case DEVICE_ID_SOURCE_MEID:
            device_id[1] = MEID_DEVICE_ID_PREFIX;

            device_id[8] = device_meid[0];
            device_id[9] = device_meid[1];
            device_id[10] = device_meid[2];
            device_id[11] = device_meid[3];
            device_id[12] = device_meid[4];
            device_id[13] = device_meid[5];
            device_id[14] = device_meid[6];
            device_id[15] = device_meid[7];
            break;
        case DEVICE_ID_SOURCE_ESN:
            device_id[1] = ESN_DEVICE_ID_PREFIX;

            device_id[12] = device_esn[0];
            device_id[13] = device_esn[1];
            device_id[14] = device_esn[2];
            device_id[15] = device_esn[3];
            break;
        case DEVICE_ID_SOURCE_AUTOPROVISION:
        {
            char device_id_file_path[256];

            get_device_id_file_full_path(DEVICE_ID_FILENAME, device_id_file_path, sizeof device_id_file_path);

            if (file_exists(device_id_file_path))
            {
                read_device_id_from_file(device_id_file_path, device_id, sizeof device_id);
            }
            /* If Device ID is all zeroes then it will be autoprovisioned */
            break;
        }
        default:
            APP_DEBUG("Invalid device_id_source, please specify a valid one\n");
            ASSERT(connector_false);
    }

    config_device_id->data = device_id;

    return connector_callback_continue;
}

static connector_callback_status_t app_save_device_id(connector_config_pointer_data_t * const config_device_id)
{
    char device_id_file_path[PATH_MAX];

    get_device_id_file_full_path(DEVICE_ID_FILENAME, device_id_file_path, sizeof device_id_file_path);
    
    write_device_id_to_file(device_id_file_path, config_device_id->data, config_device_id->bytes_required);

    return connector_callback_continue;
}

#if !(defined CONNECTOR_VENDOR_ID)
static connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor)
{
#error  "Specify vendor id"

    static uint32_t const device_vendor_id = 0x00000000;
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
static connector_callback_status_t app_get_device_cloud_url(connector_config_pointer_string_t * const config_url)
{
    static  char const connector_cloud_url[] = "login.etherios.com";

    config_url->string = (char *)connector_cloud_url;
    config_url->length = sizeof connector_cloud_url - 1;

    return connector_callback_continue;
}
#endif

#if !(defined CONNECTOR_CLOUD_PHONE)

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
#endif

#if !(defined CONNECTOR_CLOUD_SERVICE_ID)

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
/* This keep alive is sent from the device to Device Cloud  */
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

/* Converts the first digit char ('0' to '9') to a nibble starting at index and working backwards. */
static unsigned int digit_to_nibble(char const * const string, int * const index)
{
    unsigned int nibble = 0;
    int current;
     
    for (current = *index; current >= 0; current--)
    {
        int const ch = string[current];
         
        if (isxdigit(ch))
        {
            if (isdigit(ch))
                nibble = ch - '0';
            else
                nibble = toupper(ch) - 'A' + 0xA;
            break;
        }
    }
    *index = current - 1;
 
    return nibble;
}
  
/* Parse a string with non-digit characters into an array (i.e.: 0123456-78-901234-5 will be stored in {0x01, 0x23, 0x45, 0x67, 0x89, 0x01, 0x23, 0x45}) */
static int str_to_uint8_array(char const * const str, uint8_t * const array, size_t const array_size)
{
    int i;
    int const string_length = strlen(str);
    int index = string_length - 1;
 
    for (i = array_size - 1; i >= 0; i--)
    {
        unsigned int const ls_nibble = digit_to_nibble(str, &index);
        unsigned int const ms_nibble = digit_to_nibble(str, &index);
 
        array[i] = (ms_nibble << 4) + ls_nibble;
    }
 
    return 0;
}

static connector_callback_status_t app_get_imei_number(connector_config_pointer_data_t * const config_imei)
{
#define APP_IMEI_LENGTH         8
#define APP_IMEI_STRING_LENGTH  (sizeof("000000-00-000000-0") - 1)
    /* Each nibble corresponds a decimal digit.
     * Most upper nibble must be 0.
     */
    char const app_imei_number_string[APP_IMEI_STRING_LENGTH] = "000000-00-000000-0";
    static uint8_t app_imei_number[APP_IMEI_LENGTH] = {0};
    
    str_to_uint8_array(app_imei_number_string, app_imei_number, sizeof app_imei_number);

    config_imei->data = app_imei_number;
    ASSERT(config_imei->bytes_required == sizeof app_imei_number);
    return connector_callback_continue;
}

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

static connector_callback_status_t app_get_esn(connector_config_pointer_data_t * const config_esn)
{
#define APP_ESN_HEX_LENGTH              4
#define APP_ESN_HEX_STRING_LENGTH       (sizeof("00000000") - 1)
    /* Each nibble corresponds a decimal digit.
     * Most upper nibble must be 0.
     */
    char const app_esn_hex_string[APP_ESN_HEX_STRING_LENGTH] = "00000000";
    static uint8_t app_esn_hex[APP_ESN_HEX_LENGTH] = {0};
    
    str_to_uint8_array(app_esn_hex_string, app_esn_hex, sizeof app_esn_hex);
    config_esn->data = app_esn_hex;
    ASSERT(config_esn->bytes_required == sizeof app_esn_hex);

    return connector_callback_continue;
}

static connector_callback_status_t app_get_meid(connector_config_pointer_data_t * const config_meid)
{
#define APP_MEID_HEX_LENGTH             7
#define APP_MEID_HEX_STRING_LENGTH      (sizeof("00000000000000") - 1)
    /* Each nibble corresponds a decimal digit.
     * Most upper nibble must be 0.
     */
    char const app_meid_hex_string[APP_MEID_HEX_STRING_LENGTH] = "00000000000000";
    static uint8_t app_meid_hex[APP_MEID_HEX_LENGTH] = {0};

    str_to_uint8_array(app_meid_hex_string, app_meid_hex, sizeof app_meid_hex);

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

#if !(defined CONNECTOR_CLOUD_PHONE)
    case connector_request_id_config_get_device_cloud_phone:
        status = app_get_device_cloud_phone(data);
        break;

    case connector_request_id_config_set_device_cloud_phone:
        status = app_set_device_cloud_phone(data);
        break;
#endif

#if !(defined CONNECTOR_CLOUD_SERVICE_ID)
    case connector_request_id_config_device_cloud_service_id:
        status = app_get_device_cloud_service_id(data);
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

#if !(defined CONNECTOR_IDENTITY_VERIFICATION)
     case connector_request_id_config_identity_verification:
         status = app_get_identity_verification(data);
         break;
#endif

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

