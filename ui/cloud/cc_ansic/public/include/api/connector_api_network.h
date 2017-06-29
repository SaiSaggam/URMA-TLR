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

#ifndef CONNECTOR_API_NETWORK_H
#define CONNECTOR_API_NETWORK_H


/**
* @defgroup connector_request_id_network_t Network Request IDs
* @{
*/
/**
* Network Request ID passed to the application's callback for network interface.
* The class id for this connector_request_id_network_t is
* connector_class_id_network_tcp,
* connector_class_id_network_udp.
* connector_class_id_network_sms.
*/
typedef enum {
    connector_request_id_network_open,     /**< Requesting callback to set up and make connection to Device Cloud */
    connector_request_id_network_send,     /**< Requesting callback to send data to Device Cloud */
    connector_request_id_network_receive,  /**< Requesting callback to receive data from Device Cloud */
    connector_request_id_network_close     /**< Requesting callback to close Device Cloud connection */
} connector_request_id_network_t;
/**
* @}
*/

/**
* @defgroup connector_network_open_t Network Open Data Structure
* @{
*/
/**
* Network open data structure for @ref connector_request_id_network_open callback which is called to open and connect to Device Cloud.
*/
typedef struct  {
    union
    {
        char const * CONST url;    /**< Pointer to Device Cloud URL. Used for TCP and UDP transports */
        char const * CONST phone;    /**< Pointer to Device Cloud Phone Number where to send SMSs. Used for SMS transport */
    } device_cloud;  /**< Device Cloud information */
    connector_network_handle_t handle;      /**< Application defined network handle associated with the connection */
} connector_network_open_t;
/**
* @}
*/

/**
* @defgroup connector_network_send_t Network Send Data Structure
* @{
*/
/**
* Send data structure for @ref connector_request_id_network_send callback which is called to send data to Device Cloud.
*/
typedef struct  {
    connector_network_handle_t CONST handle;    /**< Network handle associated with a connection through the connector_network_open callback */
    void const * CONST buffer;                  /**< Pointer to data to be sent */
    size_t CONST bytes_available;               /**< Number of bytes to send in the buffer */
    size_t bytes_used;                          /**< Number of bytes sent */
} connector_network_send_t;
/**
* @}
*/

/**
* @defgroup connector_network_receive_t Network Receive Request
* @{
*/
/**
* Read request structure for connector_request_id_network_receive callback which is called to receive
* a specified number of bytes data from Device Cloud.
*/
typedef struct  {
    connector_network_handle_t CONST handle;    /**< Network handle associated with a connection through the connector_network_open callback */
    void * CONST buffer;                        /**< Pointer to memory where callback writes received data to */
    size_t CONST bytes_available;               /**< Number of bytes available in the buffer */
    size_t bytes_used;                          /**< Number of bytes received and copied to the buffer */
} connector_network_receive_t;
/**
* @}
*/

/**
* @defgroup connector_close_status_t Connection Close Status Values
* @{
*/
/**
* Reasons for @ref connector_request_id_network_close callback which is called to close the connection to Device Cloud.
*/
typedef enum {
    connector_close_status_cloud_disconnected = 1,    /**< Cloud Connector received a disconnect from Device Cloud. */
    connector_close_status_cloud_redirected,          /**< Cloud Connector is redirected to different Device Cloud. */
    connector_close_status_device_terminated,          /**< Cloud Connector is terminated via @ref connector_initiate_action
                                                        Cloud Connector will terminate all active messages or requests and free all memory.
                                                        @ref connector_connect_auto_type_t returned status from the close callback will be ignored. */
    connector_close_status_device_stopped,             /**< Cloud Connector is stopped via @ref connector_initiate_action */
    connector_close_status_no_keepalive,               /**< Cloud Connector has not received keep alive messages from Device Cloud */
    connector_close_status_abort,                      /**< Cloud Connector is aborted either it encountered fatal error or callback aborted Cloud Connector.
                                                        Cloud Connector will terminate all active messages or requests and free all memory.
                                                        @ref connector_connect_auto_type_t returned status from the close callback will be ignored. */
    connector_close_status_device_error                /**< Cloud Connector received error from callback which requires to close the connection. */
} connector_close_status_t;
/**
* @}
*/

/**
* @defgroup connector_network_close_t Network Close Request
* @{
*/
/**
* Close request structure for @ref connector_request_id_network_close callback which is called to close the connection to Device Cloud.
*/
typedef struct  {
    connector_network_handle_t CONST handle;    /**< Network handle associated with a connection through the connector_network_open callback */
    connector_close_status_t  CONST status;     /**< Reason for closing the network handle */

    connector_bool_t  reconnect;                /**< connector_true - reconnect, connector_false - don't reconnect. This value is ignored if transport is closed by a @ref connector_initiate_transport_stop action. */
} connector_network_close_t;
/**
* @}
*/

#if !defined _CONNECTOR_API_H
#error  "Illegal inclusion of connector_api_network.h. You should only include connector_api.h in user code."
#endif

#else
#error  "Illegal inclusion of connector_api_network.h. You should only include connector_api.h in user code."
#endif
