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

 /**
  * @file
  *  @brief Configuration routines for the Cloud Connector.
  *
  */
#include <stdio.h>
#include <ctype.h>

#include "connector_config.h"
#include "connector_api.h"
#include "connector_debug.h"
#include "platform.h"

/**
 * @brief   Get the IP address of the device
 *
 * This routine assigns a pointer to the IP address of the device.
 *
 * @param [out] config_ip  Callback returns pointer to the IP address and type of IP Address.
 *
 * @retval connector_callback_continue  IP address was successfully returned
 * @retval connector_callback_abort     Could not get IP address and abort Cloud Connector.
 *
 * @see @ref ip_address API Configuration Callback
 */
static connector_callback_status_t app_get_ip_address(connector_config_ip_address_t * const config_ip)
{
    UNUSED_ARGUMENT(config_ip);
    return connector_callback_continue;
}

/**
 * @brief   Get the MAC address of the device
 *
 * This routine returns a pointer to the MAC address of the device,
 *
 * @param [out] config_mac  Callback returns pointer to memory containing the device's MAC address
 *
 * @retval connector_callback_continue  MAC address was successfully returned
 * @retval connector_callback_abort     Could not get the MAC address and abort Cloud Connector.
 *
 * @see @ref mac_address API Configuration Callback
 */
static connector_callback_status_t app_get_mac_addr(connector_config_pointer_data_t * const config_mac)
{
    UNUSED_ARGUMENT(config_mac);
    return connector_callback_continue;
}

/**
 * @brief   Load the Cloud Connector's device ID
 *
 * Device IDs are a globally unique 16-octet value identifier for Device Cloud clients.
 * This function is called to provide the Cloud Connector with a Device ID, it is up to the implementation to decide
 * which source to use to generate this unique identifier. There are three valid sources to generate the Device ID:
 *      - Device's MAC address
 *      - Device's modem's IMEI
 *      - Device's MEID
 *      - Device's ESN
 * 
 * When any of the previous sources are available, Cloud Connector can ask Device Cloud to assign the Device
 * a unique randomly generated Device ID for it. This value must be saved into non-volatile memory (e.g.: file, EEPROM, NVRAM, etc.)
 * and returned every time this is called again, because the autoprovisioning feature must be used only once per Device.
 * To use this feature, the returned Device ID must be a buffer with all its bytes set to 0x00, this will make Cloud Connector
 * to get the Device ID from Device Cloud and then call function @ref app_save_device_id so the user saves to a non-volatile
 * storage that provisioned value.
 * @note If Device Cloud provides a Device ID, it automatically adds that Device ID to the Device Cloud
 * account which @ref vendor_id is provided.
 * @note Provisioning a Device ID can only be done by TCP transport.
 *
 * @param [out] config_device_id  Callback returns pointer to memory containing the device ID
 *
 * @retval connector_callback_continue  Device ID was successfully returned.
 * @retval connector_callback_abort     Could not get the device ID and abort Cloud Connector.
 *
 * @see @ref device_id API Configuration Callback
 */
static connector_callback_status_t app_load_device_id(connector_config_pointer_data_t * const config_device_id)
{
    UNUSED_ARGUMENT(config_device_id);
    return connector_callback_continue;
}

/**
 * @brief   Save the Cloud Connector's Device ID
 *
 * This routine is called when a zeroed Device ID is provided in @ref app_load_device_id.
 * In this function the provided Device ID must be saved to a non-volatile storage to be read in future
 * access by @ref app_load_device_id function.
 * @note Provision a Device ID can only be done by TCP transport.
 * @param [in] config_device_id  pointer to memory containing the device ID to be stored in non-volatile storage.
 *
 * @retval connector_callback_continue  Device ID was successfully written.
 * @retval connector_callback_abort     Could not set the device ID, abort Cloud Connector.
 *
 * @see @ref device_id API Configuration Callback
 */
static connector_callback_status_t app_save_device_id(connector_config_pointer_data_t * const config_device_id)
{
    UNUSED_ARGUMENT(config_device_id);
    return connector_callback_continue;
}

/**
 * @brief   Get the Device Cloud's vendor ID
 *
 * This routine returns the vendor ID which is a unique code identifying
 * the manufacturer of a device. Vendor IDs are assigned to manufacturers by Device Cloud.
 *
 * @param [out] config_vendor  Callback writes 4-byte device ID
 *
 * @retval connector_callback_continue  Vendor ID was successfully returned.
 * @retval connector_callback_abort     Could not get the vendor ID and abort Cloud Connector.
 *
 * @see @ref vendor_id API Configuration Callback
 * @see @ref connector_vendor_id
 *
 * @note This routine is not needed if you define @b CONNECTOR_VENDOR_ID configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor)
{
    UNUSED_ARGUMENT(config_vendor);
    return connector_callback_continue;
}

/**
 * @brief   Get the device type
 *
 * This routine returns a pointer to the device type which is an iso-8859-1 encoded string.
 * This string should be chosen by the device manufacture as a name that uniquely
 * identifies this model of device to Device Cloud. When Device Cloud finds two devices
 * with the same device type, it can infer that they are the same product and
 * product scoped data may be shared among all devices with this device type.
 * A device's type cannot be an empty string, nor contain only whitespace.
 *
 * @param [out] config_device_type  Callback returns pointer to memory containing the device type and
 *                                  the length of the device type in bytes.
 *
 * @retval connector_callback_continue  Device type was successfully returned.
 * @retval connector_callback_abort     Could not get the device type and abort Cloud Connector.
 *
 * @see @ref device_type API Configuration Callback
 * @see @ref CONNECTOR_DEVICE_TYPE
 *
 * @note This routine is not needed if you define @ref CONNECTOR_DEVICE_TYPE configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_device_type(connector_config_pointer_string_t * const config_device_type)
{
    UNUSED_ARGUMENT(config_device_type);
    return connector_callback_continue;
}

/**
 * @brief   Get the Device Cloud URL
 *
 * This routine assigns a pointer to the ASCII null-terminated string of
 * Device Cloud FQDN, this is typically login.etherios.com.
 *
 * @param [out] config_url  Callback returns pointer to memory containing the URL and the length of the URL in bytes.
 *
 * @retval connector_callback_continue  The URL type was successfully returned.
 * @retval connector_callback_abort     Could not get the URL and abort Cloud Connector.
 *
 * @see @ref device_cloud_url API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_CLOUD_URL configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_device_cloud_url(connector_config_pointer_string_t * const config_url)
{
    UNUSED_ARGUMENT(config_url);
    return connector_callback_continue;
}

/**
 * @brief   Get the Device Cloud phone number where to send SMSs (Only used for SMS transport)
 *
 * This routine assigns a pointer to the ASCII null-terminated string of
 * Device Cloud phone number, this is typically 447786201216 if using world wide long codes or 32075 withing US.
 *
 * @param [out] config_phone  Callback returns pointer to memory containing the phone number and the length of the phone number in bytes.
 *
 * @retval connector_callback_continue  The phone number was successfully returned.
 * @retval connector_callback_abort     Could not get the phone number and abort Cloud Connector.
 *
 * @see @ref get_device_cloud_phone API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_CLOUD_PHONE configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_device_cloud_phone(connector_config_pointer_string_t * const config_phone)
{
    UNUSED_ARGUMENT(config_phone);
    return connector_callback_continue;
}

/**
 * @brief   Set the Device Cloud phone number where to send SMSs (Only used for SMS transport)
 *
 * This routine is used to set (not to get) the Device Cloud phone number where to send SMSs (Only used for SMS transport)
 *
 * @param [in] config_phone  Pointer passed to the Callback containing the Device Cloud phone number where to send
 *                SMSs in order to be stored in persistent storage. (Only used for SMS transport).
 *
 * @retval connector_callback_continue  The phone number was successfully stored.
 * @retval connector_callback_abort     Could not set the phone number and abort Cloud Connector.
 *
 * @see @ref set_device_cloud_phone API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_CLOUD_PHONE configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_set_device_cloud_phone(connector_config_pointer_string_t * const config_phone)
{
    UNUSED_ARGUMENT(config_phone);
    return connector_callback_continue;
    static char connector_cloud_phone[16] = "";	// empty: will require a provisioning message from the server for initialization

    if (config_phone->length > (sizeof connector_cloud_phone -1))
    {
        APP_DEBUG("app_set_device_cloud_phone: Not enough room to store cloud_phone.\n");
        return connector_callback_error;
    }

    strcpy(connector_cloud_phone, config_phone->string);

    /* Maybe user want to save connector_cloud_phone to persistent storage */

    return connector_callback_continue;
}

/**
 * @brief   Get the Device Cloud Service ID (if used) where to send SMSs (Only used for SMS transport)
 *
 * This routine assigns a pointer to the ASCII null-terminated string of
 * Device Cloud Service Id, this is typically empty if using world wide long codes or idgp withing US.
 *
 * @param [out] config_service_id  Callback returns pointer to memory containing the Service Id and the length of the Service Id in bytes.
 *
 * @retval connector_callback_continue  The Service Id was successfully returned.
 * @retval connector_callback_abort     Could not get the Service Id and abort Cloud Connector.
 *
 * @see @ref device_cloud_service_id API Configuration Callback
 *
 * @note This routine is not needed if you define @ref CONNECTOR_CLOUD_SERVICE_ID configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_device_cloud_service_id(connector_config_pointer_string_t * const config_service_id)
{
    UNUSED_ARGUMENT(config_service_id);
    return connector_callback_continue;
}

/**
 * @brief   Get the connection type
 *
 * This routine specifies the connection type as @ref connector_connection_type_lan or
 * @ref connector_connection_type_wan.
 *
 * @param [out] config_connection  Pointer to @ref connector_config_connection_type_t where callback writes the connection type,
 *
 * @retval connector_callback_continue  The connection type was successfully returned.
 * @retval connector_callback_abort     Could not get connection type and abort Cloud Connector.
 *
 * @see @ref connection_type API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_CONNECTION_TYPE configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_connection_type(connector_config_connection_type_t * const config_connection)
{
    UNUSED_ARGUMENT(config_connection);
    return connector_callback_continue;
}

/**
 * @brief   Get the link speed
 *
 * This routine returns the link speed for WAN connection type. If connection type is LAN,
 * Cloud Connector will not request link speed configuration.
 *
 * @param [out] config_link Pointer to @ref connector_config_link_speed_t where callback writes 4-byte link speed.
 *
 * @retval connector_callback_continue  The link speed was successfully returned.
 * @retval connector_callback_abort     Could not get the link speed and abort Cloud Connector
 *
 * @see @ref link_speed API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_link_speed(connector_config_link_speed_t * const config_link)
{
    UNUSED_ARGUMENT(config_link);
    return connector_callback_continue;
}

/**
 * @brief   Get the WAN phone number
 *
 * This routine assigns the phone number dialed for WAN connection type,
 * including any dialing prefixes. It's a variable length, non null-terminated string.
 * If connection type is LAN, Cloud Connector will not request phone number.
 *
 * @param [out] config_phone_number  Pointer to @ref connector_config_pointer_string_t where callback writes
 *                                   the phone number and the length of the phone number in bytes.
 *
 * @retval connector_callback_continue  The phone number was successfully returned.
 * @retval connector_callback_abort     Could not get the phone number and abort Cloud Connector.
 *
 * @see @ref phone_number API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_WAN_PHONE_NUMBER_DIALED configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_phone_number(connector_config_pointer_string_t * const config_phone_number)
{
    UNUSED_ARGUMENT(config_phone_number);
    return connector_callback_continue;
}

/**
 * @brief   Get the TX keepalive interval
 *
 * This routine assigns the TX keep alive interval in seconds. This indicates how
 * often Device Cloud sends a keep alive message to the device to verify the
 * device is still operational. Keep alive messages are from the prospective of Device Cloud,
 * this keepalive is sent from Device Cloud to the device. The value must be between 5 and 7200 seconds.
 *
 * @param [out] config_keepalive  Pointer to @ref connector_config_keepalive_t where callback writes the keep alive interval in seconds.
 *
 * @retval connector_callback_continue  The keep alive interval was successfully returned.
 * @retval connector_callback_abort     Could not get the keep alive interval and abort Cloud Connector.
 *
 * @see @ref tx_keepalive API Configuration Callback
 *
 * @note This routine is not needed if you define @ref CONNECTOR_TX_KEEPALIVE_IN_SECONDS configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_tx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    UNUSED_ARGUMENT(config_keepalive);
    return connector_callback_continue;
}

/**
 * @brief   Get the RX keepalive interval
 *
 * This routine assigns the RX keep alive interval in seconds. This indicates how
 * often Cloud Connector device sends keep alive messages to Device Cloud. Keepalive
 * messages are from the prospective of Device Cloud, this keep alive is sent from the
 * device to Device Cloud. The value must be between 5 and 7200 seconds.
 *
 * @param [out] config_keepalive  Pointer to @ref connector_config_keepalive_t where callback writes the keep alive interval in seconds.
 *
 * @retval connector_callback_continue  The keep alive interval was successfully returned.
 * @retval connector_callback_abort     Could not get the keep alive interval and abort Cloud Connector.
 *
 * @see @ref rx_keepalive API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_RX_KEEPALIVE_IN_SECONDS configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_rx_keepalive_interval(connector_config_keepalive_t * const config_keepalive)
{
    UNUSED_ARGUMENT(config_keepalive);
    return connector_callback_continue;
}

/**
 * @brief   Get the wait count
 *
 * This routine assigns the number of times that not receiving a keep alive message
 * from Device Cloud will indicate that the connection is considered lost.
 * This value must be between 2 to 64 counts.
 *
 * @param [out] config_wait  Pointer to @ref connector_config_wait_count_t where callback writes the wait count
 *
 * @retval connector_callback_continue  The wait count was successfully returned.
 * @retval connector_callback_abort     Could not get the wait count and abort Cloud Connector.
 *
 * @see @ref wait_count API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_WAIT_COUNT configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_wait_count(connector_config_wait_count_t * const config_wait)
{
    UNUSED_ARGUMENT(config_wait);
    return connector_callback_continue;
}

/**
 * @brief   Get firmware update support
 *
 * This routine tells Cloud Connector whether firmware update capability is supported or not.
 * If firmware update is not supported, callback for connector_class_id_firmware
 * class will not be executed.
 *
 * @param [out] config_status  Pointer to @ref connector_config_supported_t where callback writes connector_true if firmware update is supported or
 *                            connector_false  if firmware update is not supported.
 *
 * @retval connector_callback_continue  The firmware update support was successfully returned.
 * @retval connector_callback_abort     Could not get the firmware update support and abort Cloud Connector.
 *
 * @see @ref firmware_support API Configuration Callback
 *
 * @note This routine is not called if you define @b CONNECTOR_FIRMWARE_SUPPORT configuration in @ref connector_config.h.
 * @note This CONNECTOR_FIRMWARE_SUPPORT indicates application supports firmware download. See @ref connector_config_data_options
 *
 * @note See @ref CONNECTOR_FIRMWARE_SERVICE to include firmware access facility code in Cloud Connector.
 */
static connector_callback_status_t app_get_firmware_support(connector_config_supported_t * const config_status)
{
    UNUSED_ARGUMENT(config_status);
    return connector_callback_continue;
}

/**
 * @brief  Get data service support
 *
 * This routine tells Cloud Connector whether the data service facility is supported or not.
 * If you plan on sending data to/from Device Cloud return connector_true.
 *
 * @param [out] config_status  Pointer to @ref connector_config_supported_t where callback writes connector_true if data service is supported or
 *                            connector_false  if data service is not supported.
 *
 * @retval connector_callback_continue  The data service support was successfully returned.
 * @retval connector_callback_abort     Could not get the data service support and abort Cloud Connector.
 *
 * @see @ref data_service_support API Configuration Callback
 *
 * @note This routine is not called if you define @b CONNECTOR_DATA_SERVICE_SUPPORT configuration in @ref connector_config.h.
 * @note This CONNECTOR_DATA_SERVICE_SUPPORT indicates application supports data service. See @ref connector_config_data_options
 *
 * @note See @ref CONNECTOR_DATA_SERVICE to include data service code in Cloud Connector.
 * @note See @ref CONNECTOR_COMPRESSION for data service transferring compressed data.
 */
static connector_callback_status_t app_get_data_service_support(connector_config_supported_t * const config_status)
{
    UNUSED_ARGUMENT(config_status);
    return connector_callback_continue;
}

/**
 * @brief   Get file system support
 *
 * This routine tells Cloud Connector whether the file system facility is supported or not.
 * If you plan to access device files from Device Cloud return connector_true.
 *
 * @param [out] config_status  Pointer to @ref connector_config_supported_t where callback writes connector_true if file system is supported or
 *                            connector_false  if file system is not supported.
 *
 * @retval connector_callback_continue  The file system support was successfully returned.
 * @retval connector_callback_abort     Could not get the file system support and abort Cloud Connector.
 *
 * @see @ref file_system_support API Configuration Callback
 *
 * @note This routine is not called if you define @b CONNECTOR_FILE_SYSTEM_SUPPORT configuration in @ref connector_config.h.
 * @note This CONNECTOR_FILE_SYSTEM_SUPPORT indicates application supports file system. See @ref connector_config_data_options
 *
 * @note See @ref CONNECTOR_FILE_SYSTEM to include file system code in Cloud Connector.
 * @note See @ref CONNECTOR_COMPRESSION for file system transferring compressed data.
 */
static connector_callback_status_t app_get_file_system_support(connector_config_supported_t * const config_status)
{
    UNUSED_ARGUMENT(config_status);
    return connector_callback_continue;
}

/**
 * @brief   Get the remote configuration support
 *
 * This routine tells Cloud Connector whether the remote configuration service is supported or not.
 * If you plan on accessing device data configurations through Device Cloud return connector_true.
 *
 * @param [out] config_status  Pointer to @ref connector_config_supported_t where callback writes connector_true if remote configuration is supported or
 *                            connector_false  if remote configuration is not supported.
 *
 * @retval connector_callback_continue  The remote configuration support was successfully returned.
 * @retval connector_callback_abort     Could not get the remote configuration support and abort Cloud Connector.
 *
 * @note @b CONNECTOR_RCI_SERVICE must be defined in connector_config.h
 * @note This routine is not called if you define @b CONNECTOR_REMOTE_CONFIGURATION_SUPPORT configuration in @ref connector_config.h.
 * @note This CONNECTOR_REMOTE_CONFIGURATION_SUPPORT indicates application supports remote configuration. See @ref connector_config_data_options
 *
 */
static connector_callback_status_t app_get_remote_configuration_support(connector_config_supported_t * const config_status)
{
    UNUSED_ARGUMENT(config_status);
    return connector_callback_continue;
}

/**
 * @brief   Get maximum transactions
 *
 * This routine tells Cloud Connector the maximum simultaneous transactions for data service, file system, and
 * remote_config to receive messages from  Device Cloud.
 *
 * @param [out] config_max_transaction  Pointer to @ref connector_config_max_transaction_t where callback writes the maximum simultaneous transaction.
 *                           Writes 0 for unlimited transactions.
 *
 * @retval connector_callback_continue  The maximum simultaneous transactions was successfully returned.
 * @retval connector_callback_abort     Could not get the maximum simultaneous transactions and abort Cloud Connector.
 *
 * @see @ref max_msg_transactions API Configuration Callback
 *
 * @note This routine is not needed if you define @b CONNECTOR_MSG_MAX_TRANSACTION configuration in @ref connector_config.h.
 * See @ref connector_config_data_options
 */
static connector_callback_status_t app_get_max_message_transactions(connector_config_max_transaction_t * const config_max_transaction)
{
    UNUSED_ARGUMENT(config_max_transaction);
    return connector_callback_continue;
}

/**
 * @brief  Start Network TCP
 *
 * This routine tells Cloud Connector whether it automatic or manual starts TCP.
 * You need to call connector_initiate_action with connector_initiate_transport_start to start TCP if it manual starts TCP.
 *
 * @param [out] config_connect  Pointer connector_config_connect_type_t where callback writes connector_connect_auto to automatic start TCP connection or
 *                            connector_connect_manual to manually start TCP connection.
 *
 * @retval connector_callback_continue  TCP network connection was successfully started.
 * @retval connector_callback_abort     Abort Cloud Connector.
 *
 * @see @ref network_tcp_start API Configuration Callback
 *
 * @note This routine is not called if you define @b CONNECTOR_TRANSPORT_TCP and @b CONNECTOR_NETWORK_TCP_START configuration in @ref connector_config.h.
 * @note This CONNECTOR_TRANSPORT_TCP indicates application supports network TCP.
 *
 * @note See @ref CONNECTOR_TRANSPORT_TCP to include TCP code in Cloud Connector.
 */
static connector_callback_status_t app_start_network_tcp(connector_config_connect_type_t * const config_connect)
{
    UNUSED_ARGUMENT(config_connect);
    return connector_callback_continue;
}

/**
 * @brief  Start Network UDP
 *
 * This routine tells Cloud Connector whether it automatically or manually starts UDP.
 * You need to call connector_initiate_action with connector_initiate_transport_start to start UDP if manually is selected
 * when this callback is called.
 *
 * @param [out] config_connect  Pointer connector_config_connect_type_t where callback writes connector_connect_auto to
 *                              automatically start UDP connection or connector_connect_manual to manually start UDP connection.
 *
 *
 * @retval connector_callback_continue  UDP configuration was successfully set to start automatically or manually.
 * @retval connector_callback_abort     Abort Cloud Connector.
 *
 * @see @ref network_udp_start API Configuration Callback
 *
 * @note This routine is not called if you define @b CONNECTOR_TRANSPORT_UDP configuration in @ref connector_config.h.
 * @note This CONNECTOR_TRANSPORT_UDP indicates application supports network UDP.
 *
 * @note See @ref CONNECTOR_TRANSPORT_UDP to include UDP code in Cloud Connector.
*/
static connector_callback_status_t app_start_network_udp(connector_config_connect_type_t * const config_connect)
{
    UNUSED_ARGUMENT(config_connect);
    return connector_callback_continue;
}

/**
 * @brief  Start Network SMS
 *
 * This routine tells Cloud Connector whether it automatically or manually starts SMS.
 * You need to call connector_initiate_action with connector_initiate_transport_start to start SMS if manually is selected
 * when this callback is called.
 *
 * @param [out] config_connect  Pointer connector_config_connect_type_t where callback writes connector_connect_auto to
 *                              automatically start SMS connection or connector_connect_manual to manually start SMS connection.
 *
 *
 * @retval connector_callback_continue  SMS configuration was successfully set to start automatically or manually.
 * @retval connector_callback_abort     Abort Cloud Connector.
 *
 * @see @ref network_sms_start API Configuration Callback
 *
 * @note This routine is not called if you define @b CONNECTOR_TRANSPORT_SMS configuration in @ref connector_config.h.
 * @note This CONNECTOR_TRANSPORT_SMS indicates application supports network SMS.
 *
 * @note See @ref CONNECTOR_TRANSPORT_SMS to include SMS code in Cloud Connector.
*/
static connector_callback_status_t app_start_network_sms(connector_config_connect_type_t * const config_connect)
{
    UNUSED_ARGUMENT(config_connect);
    return connector_callback_continue;
}

/**
 * @brief   Get identity verification form
 *
 * This routine returns identity verification form which allows the device to determine that it is
 * communicating with Device Cloud, and allows Device Cloud to determine and
 * verify the Device ID of the device it is communicating with for TCP transport.
 *
 * @note The identity verification form in this function should be referenced by devicesecurity setting
 * when @ref rci_service is supported and devicesecurity setting is defined in configuration file (see @ref rci_tool).
 *
 * @note If password identity verification form is used, @ref app_get_password will be called to obtain
 * the password.
 *
 * @param [out] config_identity  Pointer to @ref connector_config_identity_verification_t where callback writes the identity verification form.
 *
 * @retval connector_callback_continue  The identity verification form was successfully returned.
 * @retval connector_callback_abort     Could not get the identity verification form and abort Cloud Connector.
 *
 * @see @ref app_get_password API Callback
 *
 */
static connector_callback_status_t app_get_identity_verification(connector_config_identity_verification_t * const config_identity)
{
    UNUSED_ARGUMENT(config_identity);
    return connector_callback_continue;
}

/**
 * @brief   Get password for password identity verification form
 *
 * This routine returns the password which Device Cloud verifies the password to
 * its stored password when password identity verification form is used. See @ref app_get_identity_verification.
 *
 * @param [out] config_password  Pointer to @ref connector_config_pointer_string_t where callback returns pointer to
 *                               password and the length of the password in bytes.
 *
 * @retval connector_callback_continue  The password was successfully returned.
 * @retval connector_callback_abort     Could not get the password and abort Cloud Connector.
 *
 * @see @ref app_get_identity_verification API Callback
 *
 */
static connector_callback_status_t app_get_password(connector_config_pointer_string_t * const config_password)
{
    UNUSED_ARGUMENT(config_password);
    return connector_callback_continue;
}

/**
 * @brief Get maximum active sessions for SM over UDP
 *
 * This routine returns the number of active sessions available for SM over UDP.
 *
 * @param [out] config_max_sessions  Pointer to  @ref connector_config_sm_max_sessions_t where callback returns the value.
 *
 * @retval connector_callback_continue  Requested data was successfully returned.
 * @retval connector_callback_abort     Requested data was not set and abort Cloud Connector.
 *
 */
static connector_callback_status_t app_get_sm_udp_max_sessions(connector_config_sm_max_sessions_t * const config_max_sessions)
{
    UNUSED_ARGUMENT(config_max_sessions);
    return connector_callback_continue;
}

/**
 * @brief Get maximum active sessions for SM over SMS
 *
 * This routine returns the number of active sessions available for SM over SMS.
 *
 * @param [out] config_max_sessions  Pointer to  @ref connector_config_sm_max_sessions_t where callback returns the value.
 *
 * @retval connector_callback_continue  Requested data was successfully returned.
 * @retval connector_callback_abort     Requested data was not set and abort Cloud Connector.
 *
 */
static connector_callback_status_t app_get_sm_sms_max_sessions(connector_config_sm_max_sessions_t * const config_max_sessions)
{
    UNUSED_ARGUMENT(config_max_sessions);
    return connector_callback_continue;
}

/**
 * @brief Get maximum incoming segments for SM over UDP
 *
 * This routine returns the number of segments in which the incoming data in SM over UDP might be split.
 *
 * @param [out] config_max_rx_segments  Pointer to  @ref connector_config_sm_max_rx_segments_t where callback returns the value.
 *
 * @retval connector_callback_continue  Requested data was successfully returned.
 * @retval connector_callback_abort     Requested data was not set and abort Cloud Connector.
 *
 */
static connector_callback_status_t app_get_sm_udp_max_rx_segments(connector_config_sm_max_rx_segments_t * const config_max_rx_segments)
{
    UNUSED_ARGUMENT(config_max_rx_segments);
    return connector_callback_continue;
}

/**
 * @brief Get maximum incoming segments for SM over SMS
 *
 * This routine returns the number of segments in which the incoming data in SM over SMS might be split.
 *
 * @param [out] config_max_rx_segments  Pointer to  @ref connector_config_sm_max_rx_segments_t where callback returns the value.
 *
 * @retval connector_callback_continue  Requested data was successfully returned.
 * @retval connector_callback_abort     Requested data was not set and abort Cloud Connector.
 *
 */
static connector_callback_status_t app_get_sm_sms_max_rx_segments(connector_config_sm_max_rx_segments_t * const config_max_rx_segments)
{
    UNUSED_ARGUMENT(config_max_rx_segments);
    return connector_callback_continue;
}

/**
 * @brief Get maximum timeout for incoming SM sessions over UDP
 *
 * This routine returns how many seconds should elapse before dropping an incomplete incoming message over UDP. It can be set to SM_WAIT_FOREVER for 
 * infinite wait time but it is not recommended as delivery is not guaranteed in Short Messaging.
 *
 * @param [out] config_rx_timeout  Pointer to  @ref connector_config_sm_rx_timeout_t where callback returns the value.
 *
 * @retval connector_callback_continue  Requested data was successfully returned.
 * @retval connector_callback_abort     Requested data was not set and abort Cloud Connector.
 *
 */
static connector_callback_status_t app_get_sm_udp_rx_timeout(connector_config_sm_rx_timeout_t * const config_rx_timeout)
{
    UNUSED_ARGUMENT(config_rx_timeout);
    return connector_callback_continue;
}

/**
 * @brief Get maximum timeout for incoming SM sessions over SMS
 *
 * This routine returns how many seconds should elapse before dropping an incomplete incoming message over SMS. It can be set to SM_WAIT_FOREVER for 
 * infinite wait time but it is not recommended as delivery is not guaranteed in Short Messaging.
 *
 * @param [out] config_rx_timeout  Pointer to  @ref connector_config_sm_rx_timeout_t where callback returns the value.
 *
 * @retval connector_callback_continue  Requested data was successfully returned.
 * @retval connector_callback_abort     Requested data was not set and abort Cloud Connector.
 *
 */
static connector_callback_status_t app_get_sm_sms_rx_timeout(connector_config_sm_rx_timeout_t * const config_rx_timeout)
{
    UNUSED_ARGUMENT(config_rx_timeout);
    return connector_callback_continue;
}

/* End of Cloud Connector configuration routines */
/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @cond Doxygen_Suppress
 *
 */
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
/**
 * @endcond
 */

/**
 * @brief   Error status notification
 *
 * This routine is called when Cloud Connector encounters an error. This is used as
 * a debug tool for finding configuration or keep alive error.
 *
 * The error_data argument contains class id, request id, and error status.
 *
 * @note If @ref CONNECTOR_DEBUG is not defined in connector_config.h, Cloud Connector will
 * not call this callback to notify any error encountered.
 *
 * @retval connector_callback_continue  No Error.
 * @retval connector_callback_abort     Abort Cloud Connector.
 *                                  @note This function may be called again to free other pointer even if it has returned connector_callback_abort.
 *
 * @see @ref error_status API Configuration Callback
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

#if (defined CONNECTOR_RCI_SERVICE)
/**
 * @brief   Load Remote Configuration Interface internal data.
 *
 * Cloud Connector calls this routine when @ref rci_service is enabled and in use to load all the information automatically generated by @ref rci_tool. By default
 * that information is available at "rci_descriptor_data" pointer, available when including "connector_api.h". Except that you know what you are doing, is the only value
 * to be used.
 *
 * The error_data argument contains class id, request id, and error status.
 *
 * @retval connector_callback_continue  No Error.
 * @retval connector_callback_abort     Abort Cloud Connector.
 *                                  @note This function may be called again to free other pointer even if it has returned connector_callback_abort.
 *
 * @see @ref error_status API Configuration Callback
 */
static connector_callback_status_t app_load_rci_descriptor_data(connector_config_rci_descriptor_data_t * const config_rci_descriptor_data)
{
    connector_callback_status_t status = connector_callback_continue;

    config_rci_descriptor_data->rci_data = rci_descriptor_data; /* Autogenerated in remote_config.c by ConfigGenerator.jar tool */
    return status;
}
#endif

/**
 * @cond DEV
 */
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

    case connector_request_id_config_error_status:
        status = app_config_error(data);
        break;

    case connector_request_id_config_firmware_facility:
        status = app_get_firmware_support(data);
        break;

    case connector_request_id_config_data_service:
        status = app_get_data_service_support(data);
        break;

    case connector_request_id_config_file_system:
        status = app_get_file_system_support(data);
        break;

    case connector_request_id_config_remote_configuration:
        status = app_get_remote_configuration_support(data);
        break;

    case connector_request_id_config_max_transaction:
        status = app_get_max_message_transactions(data);
        break;

     case connector_request_id_config_network_tcp:
         status = app_start_network_tcp(data);
         break;

     case connector_request_id_config_network_udp:
         status = app_start_network_udp(data);
         break;

     case connector_request_id_config_network_sms:
         status = app_start_network_sms(data);
         break;

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
/**
 * @endcond
 */
