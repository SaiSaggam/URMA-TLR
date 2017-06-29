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
#ifndef CONNECTOR_API_CONFIG_H
#define CONNECTOR_API_CONFIG_H

/**
* @defgroup connector_request_id_config_t Configuration Request IDs
* @{
*/
/**
* Configuration Request Id passed to the application's callback for requesting configuration data.
* The class id for this connector_request_id_config_t is connector_class_id_config.
*/
typedef enum {
    connector_request_id_config_device_id,         /**< Requesting callback to return a unique device id which is used to identify the device.*/
    connector_request_id_config_set_device_id,     /**< Requesting callback to set a new device ID given by Device Cloud (when provisioning is enabled) */
    connector_request_id_config_vendor_id,         /**< Requesting callback to return a unique vendor id identifying the manufacturer of a device. */
    connector_request_id_config_device_type,       /**< Requesting callback to return device type that identifies the type of the device  */
    connector_request_id_config_device_cloud_url,  /**< Requesting callback to return the Device Cloud URL which will be connected to */
    connector_request_id_config_get_device_cloud_phone,/**< Requesting callback to get the Device Cloud Phone Number where to send SMSs (Only used for SMS transport) */
    connector_request_id_config_set_device_cloud_phone,/**< Requesting callback to set the Device Cloud Phone Number where to send SMSs (Only used for SMS transport) */
    connector_request_id_config_device_cloud_service_id,/**< Requesting callback to return the Device Cloud Phone Number service-id (if used)
                                                             where to send SMSs (Only used for SMS transport) */
    connector_request_id_config_connection_type,   /**< Requesting callback to return LAN or WAN connection type */
    connector_request_id_config_mac_addr,          /**< Requesting callback to return device's MAC addresss */
    connector_request_id_config_link_speed,        /**< Requesting callback to return link speed for WAN connection type */
    connector_request_id_config_phone_number,      /**< Requesting callback to return phone number dialed for WAN connection type */
    connector_request_id_config_tx_keepalive,      /**< Requesting callback to return Device Cloud's Tx keep alive interval in seconds */
    connector_request_id_config_rx_keepalive,      /**< Requesting callback to return Device Cloud's Rx keep alive interval in seconds */
    connector_request_id_config_wait_count,        /**< Requesting callback to return the number of intervals of not receiving a keep alive message after which a connection should be considered lost. */
    connector_request_id_config_ip_addr,           /**< Requesting callback to return device's IP address */
    connector_request_id_config_error_status,      /**< Error status notification which tells callback that error is encountered. */
    connector_request_id_config_firmware_facility, /**< Requesting callback to return whether firmware facility is supported or not. */
    connector_request_id_config_data_service,      /**< Requesting callback to return whether data service is supported or not. */
    connector_request_id_config_file_system,       /**< Requesting callback to return whether file system is supported or not. */
    connector_request_id_config_remote_configuration, /**< Requesting callback to return whether remote configuration is supported or not. */
    connector_request_id_config_max_transaction,   /**< Requesting callback to obtain maximum messaging sessions supported by client. */
    connector_request_id_config_device_id_method,  /**< Requesting callback to obtain method on how device id is generated */
    connector_request_id_config_network_tcp,       /**< Requesting callback to return whether connection over TCP need to start now. */
    connector_request_id_config_network_udp,       /**< Requesting callback to return whether connection over UDP need to start now. */
    connector_request_id_config_network_sms,       /**< Requesting callback to return whether connection over SMS need to start now. */
    connector_request_id_config_identity_verification,     /**< Requesting callback to return identity verification form. */
    connector_request_id_config_password,          /**< Requesting callback to return password for the password identity verification form */
    connector_request_id_config_sm_udp_max_sessions,    /**< Requesting callback to obtain maximum UDP Short Messaging sessions active at once. */
    connector_request_id_config_sm_sms_max_sessions,    /**< Requesting callback to obtain maximum SMS Short Messaging sessions active at once. */
    connector_request_id_config_sm_udp_max_rx_segments, /**< Requesting callback to obtain maximum segments used per incoming UDP Short Messaging session. */
    connector_request_id_config_sm_sms_max_rx_segments, /**< Requesting callback to obtain maximum segments used per incoming SMS Short Messaging session. */
    connector_request_id_config_sm_udp_rx_timeout,      /**< Requesting callback to obtain timeout in seconds for incoming UDP Short Messaging sessions. */
    connector_request_id_config_sm_sms_rx_timeout,      /**< Requesting callback to obtain timeout in seconds for incoming SMS Short Messaging sessions. */
    connector_request_id_config_rci_descriptor_data     /**< Requesting callback to obtain Remote Configuration Interface descriptor data see @ref rci_descriptor_data. */
} connector_request_id_config_t;
/**
* @}
*/

/**
* @defgroup connector_config_pointer_data_t Data Pointer Configuration
* @{
*/
/**
* Application Binary Data Pointer Configuration for the following callbacks:
*   -# @ref connector_request_id_config_device_id
*   -# @ref connector_request_id_config_mac_addr
*
* @see @ref device_id
* @see @ref mac_address
*/
typedef struct {
    uint8_t * data;               /**< Application data pointer */
    size_t CONST bytes_required;  /**< Number of bytes required and needed on the application data pointer */
} connector_config_pointer_data_t;
/**
* @}
*/


/**
* @defgroup connector_config_vendor_t  Vendor ID Configuration
* @{
*/
/**
* Vendor ID configuration for @ref connector_request_id_config_vendor_id callback
*
* @see @ref vendor_id
*
*/
typedef struct {
    uint32_t id;    /**< Vendor ID */
} connector_config_vendor_id_t;
/**
* @}
*/


/**
* @defgroup connector_config_name_data_t String Pointer Configuration
* @{
*/
/**
* Application String Pointer Configuration for the following callbacks:
*
*   -# @ref connector_request_id_config_device_type
*   -# @ref connector_request_id_config_device_cloud_url
*   -# @ref connector_request_id_config_phone_number
*   -# @ref connector_request_id_config_password
*
* @see @ref device_type
* @see @ref device_cloud_url
* @see @ref phone_number
* @see @ref password
*
*/
typedef struct {
    char * string;  /**< Application string pointer */
    size_t length;  /**< Number of bytes in the application string pointer */
} connector_config_pointer_string_t;
/**
* @}
*/

/**
* @defgroup connector_connection_type_t Connection Types
* @{
*/
/**
* Connection type configuration of the device
*
* @see connector_config_connection_type_t
*/
typedef enum {
   connector_connection_type_lan,   /**< LAN connection type for Ethernet or WiFi */
   connector_connection_type_wan    /**< WAN connection type for PPP over Modem */
} connector_connection_type_t;
/**
* @}
*/

/**
* @defgroup connector_config_connection_type_t Connection Type Configuration
* @{
*/
/**
* Device connection type configuration for @ref connector_request_id_config_connection_type callback
*
* @see @ref connection_type
*
*/
typedef struct {
    connector_connection_type_t type; /**< Device connection type */
} connector_config_connection_type_t;
/**
* @}
*/


/**
* @defgroup connector_config_link_speed_t  Link Speed Configuration
* @{
*/
/**
* Device link speed configuration for WAN connection type for @ref connector_request_id_config_link_speed callback
*
* @see @ref link_speed
* @see @ref connection_type
*/
typedef struct {
    uint32_t speed;        /**< Device link speed  */
} connector_config_link_speed_t;
/**
* @}
*/


/**
* @defgroup connector_config_keepalive_t Keep-Alive Interval Configuration
* @{
*/
/**
* Keep-alive interval configuration used for the following callbacks:
*   -# @ref connector_request_id_config_tx_keepalive
*   -# @ref connector_request_id_config_rx_keepalive
*
* @see @ref tx_keepalive
* @see @ref rx_keepalive
*
*/
typedef struct {
    uint16_t interval_in_seconds;       /**< Device keep-alive interval */
} connector_config_keepalive_t;
/**
* @}
*/


/**
* @defgroup connector_config_wait_count_t Wait Count Configuration
* @{
*/
/**
* Device wait count configuration for @ref connector_request_id_config_wait_count callback
*
* @see @ref wait_count
* @see @ref tx_keepalive
*/
typedef struct {
    uint16_t count;       /**< wait count */
} connector_config_wait_count_t;
/**
* @}
*/

/**
* @defgroup connector_config_ip_address_type_t IP Address Configuration Types
* @{
*/
/**
* Device IP address types
*
* @see connector_config_ip_address_t
*/
typedef enum {
    connector_ip_address_ipv4,  /**< IPv4 address type */
    connector_ip_address_ipv6   /**< IPv6 address type */
} connector_config_ip_address_type_t;
/**
* @}
*/

/**
* @defgroup connector_config_ip_addr_t Device IP Address Configuration
* @{
*/
/**
* Device IP Address Configuration for @ref connector_request_id_config_ip_addr callback
*
* @see @ref ip_address
*/
typedef struct {
    uint8_t * address;        /**< Device's IP address */
    connector_config_ip_address_type_t ip_address_type; /**< IPv4 or IPv6 device IP address type */
} connector_config_ip_address_t;
/**
* @}
*/


/**
* @defgroup connector_config_support_status_t Service Supported Configuration
* @{
*/
/**
* Service supported status configuration for the following callbacks:
*   -# @ref connector_request_id_config_firmware_facility
*   -# @ref connector_request_id_config_data_service
*   -# @ref connector_request_id_config_file_system
*
* @see @ref firmware_support
* @see @ref data_service_support
* @see @ref file_system_support
* @see @ref rci_support
*
*/
typedef struct {
    connector_bool_t supported; /**< Application service supported status */
} connector_config_supported_t;
/**
* @}
*/


/**
* @defgroup connector_connect_auto_type_t Network Connection Action Types
* @{
*/
/**
* Network connection action used to tell Cloud Connector to automatic or manual connect.
*
* @see connector_config_connect_type_t
*/
typedef enum {

    connector_connect_auto,     /**< Automatic connect to Device Cloud */
    connector_connect_manual   /**< Manual connect to Device Cloud */
} connector_connect_auto_type_t;
/**
* @}
*/


/**
* @defgroup connector_config_connect_status_t Network Connection Action Type Configuration
* @{
*/
/**
* Connection action which is used to to tell the connector to make connection to
* Device Cloud automatically or manually. It's used for the following callbacks:
*   -# @ref connector_request_id_config_network_tcp
*   -# @ref connector_request_id_config_network_udp
*   -# @ref connector_request_id_config_network_sms
*
* @see @ref network_tcp_start
* @see @ref network_udp_start
* @see @ref network_sms_start
*/
typedef struct {
    connector_connect_auto_type_t type;   /**< Connection action */
} connector_config_connect_type_t;
/**
* @}
*/


/**
* @defgroup connector_identity_verification_t  Identity Verification Form Types
* @{
*/
/**
 * Identify verification tells the connector to use simple or password identity verification for TCP connection.
 *
 * @see connector_config_identity_verification_t
 */
typedef enum {
    connector_identity_verification_simple,         /**< Callback returns this form for simple identity verification. Cloud Connector just transmits the
                                                     device ID and the Device Cloud URL. */
    connector_identity_verification_password        /**< Callback returns this form for password identity verification. Cloud Connector exchanges a pre-shared
                                                     secret password with Device Cloud. The password must match the password set in Device Cloud.
                                                     @note Cloud Connector will call @ref connector_request_id_config_password callback for the password. */
} connector_identity_verification_t;
/**
* @}
*/


/**
* @defgroup connector_config_identity_verification_t Device Identity Verification Configuration
* @{
*/
/**
* Identify verification configuration for @ref connector_request_id_config_identity_verification callback.
* It tells the connector to use simple or password identity verification for TCP connection.
*
* @see @ref identity_verification
* @see @ref password
* @see @ref connector_request_id_config_password
**/
typedef struct {
    connector_identity_verification_t type;   /**< Identity verification type */

} connector_config_identity_verification_t;
/**
* @}
*/


/**
* @defgroup connector_config_max_transaction_t Device Maximum Transaction Configuration
* @{
*/
/**
* Device maximum transaction configuration for @ref connector_request_id_config_max_transaction callback.
*
* @see @ref max_msg_transactions
**/
typedef struct {
    uint8_t count;   /**< Maximum transaction count */
} connector_config_max_transaction_t;
/**
* @}
*/

/**
* @defgroup connector_config_sm_max_sessions_t Short Messaging Maximum Sessions
* @{
*/
/**
* Transport maximum active sessions at once for @ref connector_request_id_config_sm_udp_max_sessions and @ref connector_request_id_config_sm_sms_max_sessions callbacks.
*
* @see @ref sm_udp_max_sessions
* @see @ref sm_sms_max_sessions
**/
typedef struct {
    size_t max_sessions;  /**< maximum active sessions */
} connector_config_sm_max_sessions_t;
/**
* @}
*/

/**
* @defgroup connector_config_sm_max_rx_segments_t Short Messaging Maximum incoming segments
* @{
*/
/**
* Maximum number of incoming segments (packets) expected for the transport. Because incoming segments can arrive at any order, Cloud Connector allocates the maximum
* and sort the incoming segments it appropriately before processing. Used for @ref connector_request_id_config_sm_udp_max_rx_segments and @ref connector_request_id_config_sm_sms_max_rx_segments callbacks.
*
* @see @ref sm_udp_max_rx_segments
* @see @ref sm_sms_max_sessions
**/
typedef struct {
    size_t max_rx_segments;   /**< Maximum number of incoming segments (packets) */
} connector_config_sm_max_rx_segments_t;
/**
* @}
*/

/**
* @defgroup connector_config_sm_rx_timeout_t Short Messaging RX timeout
* @{
*/
/**
* Timeout for incoming Short Messaging sessions. Used for @ref connector_request_id_config_sm_udp_rx_timeout and @ref connector_request_id_config_sm_sms_rx_timeout callbacks.
*
* @see @ref sm_udp_rx_timeout
* @see @ref sm_sms_rx_timeout
**/
typedef struct {
    size_t rx_timeout;  /**< Timeout for incoming Short Messaging sessions */
} connector_config_sm_rx_timeout_t;
/**
* @}
*/

/**
* @defgroup connector_config_rci_descriptor_data_t Remote Configuration Interface descriptor data
* @{
*/
/**
* This structure is used by Cloud Connector to load the internal data when @ref rci_service is enabled. It is used for @ref connector_request_id_config_rci_descriptor_data callback.
* Except that you know exactly what you are doing, the "rci_data" pointer must be set to "rci_descriptor_data", which is autogenerated by @ref rci_tool and visible including connector_api.h
* header file.
*
* @see @ref rci_descriptor_data
**/
typedef struct {
    struct connector_remote_config_data const * rci_data;  /**< pointer to Cloud Connector's internal RCI data structure */
} connector_config_rci_descriptor_data_t;
/**
* @}
*/

#if !defined _CONNECTOR_API_H
#error  "Illegal inclusion of connector_api_config.h. You should only include connector_api.h in user code."
#endif

#else
#error  "Illegal inclusion of connector_api_config.h. You should only include connector_api.h in user code."
#endif

