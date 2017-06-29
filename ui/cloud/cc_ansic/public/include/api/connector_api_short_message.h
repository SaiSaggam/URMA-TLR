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

#ifndef CONNECTOR_API_SHORT_MESSAGE_H
#define CONNECTOR_API_SHORT_MESSAGE_H

#if (defined CONNECTOR_SHORT_MESSAGE)

#define SM_INVALID_REQUEST_ID   UINT32_C(~0x0)

#define SM_WAIT_FOREVER UINT32_C(0x0)

/**
* @defgroup connector_sm_send_ping_request_t connector_sm_send_ping_request_t
*
* @brief Data type used to send ping request.
* @{
*/
/**
* This data structure is used when the device initiates the ping request to Device Cloud. The context will be returned
* when Device Connector receives the ping response.
*
* @see connector_request_id_sm_ping_response
*/
typedef struct
{
    connector_transport_t transport;    /**< transport method to use */
    void * user_context;                /**< user context, will be returned in response callback */
    uint32_t * request_id;              /**< pointer to where to store the session's Request ID. This value is saved by by Cloud Connector after a successful connector_initiate_action()
                                             and might be used for canceling the session. Only valid for SM protocol. Set to NULL if not desired.
                                             See @ref connector_initiate_session_cancel*/

    connector_bool_t response_required; /**< set to connector_true if response is needed */
    unsigned long timeout_in_seconds;   /**< outgoing sessions timeout in seconds. Only valid for SM. Use SM_WAIT_FOREVER to wait forever for the complete request/response */
} connector_sm_send_ping_request_t;
/**
* @}
*/

/**
* @defgroup connector_sm_cancel_request_t  To cancel short message session.
* @{
*/
/**
* This data structure is used when the device initiates the cancel session request. This data
* structure is used in connector_initiate_action() with action ID connector_initiate_session_cancel and connector_initiate_session_cancel_all.
*
* @see connector_initiate_session_cancel
* @see connector_initiate_session_cancel_all
*/
typedef struct
{
    connector_transport_t transport; /**< transport method to use */
    uint32_t request_id;             /**< SM Request ID to be canceled, for connector_initiate_session_cancel_all this is ignored */
} connector_sm_cancel_request_t;
/**
* @}
*/

/**
* @defgroup connector_request_id_sm_t Short message specific request IDs
* @{
*/
/**
* Short message related callbacks will use these request IDs. The class ID associated with these request ID is
* connector_class_id_short_message and supported transport methods under this class are UDP and SMS.
*/
typedef enum
{
    connector_request_id_sm_ping_request,   /**< used when Cloud Connector receives ping request from Device Cloud */
    connector_request_id_sm_ping_response,  /**< used when Cloud Connector receives ping response from Device Cloud */
    connector_request_id_sm_cli_request,    /**< used when Cloud Connector receives CLI request from Device Cloud */
    connector_request_id_sm_cli_response,   /**< used to get the CLI response */
    connector_request_id_sm_cli_response_length,/**< called to get total CLI response length */
    connector_request_id_sm_cli_status,     /**< called when error occurs in CLI session */
    connector_request_id_sm_more_data,      /**< indicates pending messages are available on Device Cloud,
                                                User need to use new request (can be ping) to pull the pending messages from Device Cloud. */
    connector_request_id_sm_opaque_response,/**< Cloud Connector will use this to provide Device Cloud response for which
                                                there is no associated request */
    connector_request_id_sm_config_request, /**< used when Cloud Connector receives config request from Device Cloud. Used only if the transport method is SMS */
    connector_request_id_sm_request_connect /**< used when Cloud Connector receives a request to start TCP transport, user can allow or not this action. */
} connector_request_id_sm_t;
/**
* @}
*/

/**
* @defgroup connector_sm_receive_ping_request_t Data type used receive ping request.
* @{
*/
/**
* This data structure is used when the the callback is called with the connector_request_id_sm_ping_request.
* A ping request is received from Device Cloud and returning connector_callback_continue from the
* callback will result in success response to Device Cloud.
*
* @see connector_request_id_sm_ping_request
*/
typedef struct
{
    connector_transport_t CONST transport;      /**< transport method on which ping request is received */
    connector_bool_t CONST response_required;   /**< connector_true if Device Cloud wants response, no action is
                                                     needed by user. This is information purpose only */
} connector_sm_receive_ping_request_t;
/**
* @}
*/

/**
* @defgroup connector_sm_ping_response_t connector_sm_ping_response_t
* @brief Data type used to pass ping response.
* @{
*/
/**
* This data structure is used when the callback is called with the connector_request_id_sm_ping_response.
* A ping response is received from Device Cloud.
*
* @see connector_request_id_sm_ping_response
*/
typedef struct
{
    connector_transport_t CONST transport; /**< transport method used */
    void * user_context;    /**< user context passed in ping request connector_initiate_action call */

    enum
    {
        connector_sm_ping_status_success,   /**< success response received from Device Cloud */
        connector_sm_ping_status_complete,  /**< session completed successfully, response is not requested */
        connector_sm_ping_status_cancel,    /**< session cancelled by the user */
        connector_sm_ping_status_timeout,   /**< timed out waiting for a response from Device Cloud */
        connector_sm_ping_status_error      /**< internal error in Cloud Connector */
    } CONST status;  /**< response/status returned from Device Cloud/Cloud Connector */

} connector_sm_ping_response_t;
/**
* @}
*/

/**
* @defgroup connector_sm_cli_request_t connector_sm_cli_request_t
* @brief Data type used for cli request
* @{
*/
/**
* This data structure is used when the callback is called to pass the CLI request received from
* Device Cloud. The request ID associated with this structure is connector_request_id_sm_cli_request.
*
* @see connector_request_id_sm_cli_request
*
*/
typedef struct
{
    connector_transport_t CONST transport;  /**< transport method on which CLI is received */
    void * user_context;                    /**< user context */

    char const * CONST buffer;              /**< buffer contains CLI request */
    size_t CONST bytes_used;                /**< bytes filled in the buffer */
    connector_bool_t CONST response_required; /**< connector_true means response is needed by Device Cloud */
    connector_bool_t CONST more_data;       /**< connector_true means more request cli to follow */
} connector_sm_cli_request_t;

/**
* @}
*/

/**
* @defgroup connector_sm_cli_response_t connector_sm_cli_response_t
* Data type used to get CLI response
* @{
*/
/**
* This data structure is used when the callback is called to get the CLI response to Device Cloud.
* The associated request ID for this data structure is connector_request_id_sm_cli_response.
*
* @see connector_request_id_sm_cli_response
*
*/
typedef struct
{
    connector_transport_t CONST transport;  /**< transport method used */
    void * user_context;                    /**< the user context passed during CLI request callback */

    char * CONST buffer;                    /**< buffer pointer to write the CLI response to */
    size_t CONST bytes_available;           /**< total bytes available in buffer */
    size_t bytes_used;                      /**< bytes filled */
    connector_bool_t more_data;             /**< more CLI response chunk to send */
} connector_sm_cli_response_t;
/**
* @}
*/

/**
* @defgroup connector_sm_cli_response_length_t connector_sm_cli_response_length_t
* @brief Data type used to get CLI response
* @{
*/
/**
* This data structure is used when the callback is called to get the CLI response length.
* The associated request ID for this data structure is connector_request_id_sm_cli_response_length.
* This callback will be called just before calling connector_request_id_sm_cli_response.
*
* @see connector_request_id_sm_cli_response_length
*
*/
typedef struct
{
    connector_transport_t CONST transport; /**< transport method used */
    void * user_context;                   /**< user context */

    size_t total_bytes;                 /**< total CLI response bytes in to send */
} connector_sm_cli_response_length_t;
/**
* @}
*/

/**
* @defgroup connector_sm_cli_status_t connector_sm_cli_status_t
* @brief Data type used to pass CLI status.
* @{
*/
/**
* This data structure is used when the callback is called to indicate the
* termination of CLI session. The associated request ID for this data structure is
* connector_request_id_sm_cli_status.
*
* @see connector_request_id_sm_cli_status
*
*/
typedef struct
{
    connector_transport_t CONST transport;  /**< transport method used */
    void * user_context;                    /**< user context */

    enum
    {
        connector_sm_cli_status_success, /**< session finished successfully */
        connector_sm_cli_status_cancel, /**< cancelled by the user */
        connector_sm_cli_status_error   /**< error occurred while preparing the response */
    } CONST status; /**< CLI session termination status */

} connector_sm_cli_status_t;
/**
* @}
*/

/**
* @defgroup connector_sm_opaque_response_t  connector_sm_opaque_response_t
* @brief Opaque response from Device Cloud
* @{
*/
/**
* This data structure is used when the callback is called to pass Device Cloud response for which the
* associated request is not present. It may be cancelled by the user or by Cloud Connector after sending the request.
* The request ID associated with this data structure is connector_request_id_sm_opaque_response.
*
* @see connector_request_id_sm_opaque_response
*
*/
typedef struct
{
    connector_transport_t CONST transport;  /**< transport method on which opaque response is received */
    uint32_t CONST id;                      /**< can be used to keep track of a multipart response */
    void const * CONST data;                /**< pointer to opaque response */
    size_t CONST bytes_used;                /**< number of bytes available in the data */
    connector_bool_t CONST error;           /**< connector_true means error response is received from Device Cloud */
} connector_sm_opaque_response_t;
/**
* @}
*/

/**
* @defgroup connector_sm_more_data_t  connector_sm_more_data_t
* @brief Indicates pending data in Device Cloud.
* @{
*/
/**
* This data structure is used when the callback is called with the connector_request_id_more_data.
* This indicates that more messages are pending in Device Cloud for the device. User need
* to send request (can be ping) to retrieve pending messages.
*
* @see connector_request_id_more_data
*/
typedef struct
{
    connector_transport_t CONST transport; /**< transport method on which pending data can be received */
} connector_sm_more_data_t;
/**
* @}
*/

/**
* @defgroup connector_sm_receive_config_request_t Data type used receive config request.
* @{
*/
/**
* This data structure is used when the the callback is called with the connector_request_id_sm_config_request.
* A config request is received from Device Cloud and returning connector_callback_continue from the
* callback will result in success response to Device Cloud.
*
* @see connector_request_id_sm_config_request
*/
typedef struct
{
    connector_transport_t CONST transport;      /**< transport method on which config request is received */

    char * phone_number;                        /**< ascii rendering of the Device Cloud phone number (eg. "32075") */
    char * service_id;                          /**< service id may be empty if shared codes are not being used */
    connector_bool_t CONST response_required;   /**< connector_true if Device Cloud wants response, no action is
                                                     needed by user. This is information purpose only */
} connector_sm_receive_config_request_t;
/**
* @}
*/

/**
* @defgroup connector_sm_request_connect_t Received a Short Message command to start TCP transport.
* @{
*/
/**
* This data structure is used when the the callback is called with the connector_request_id_sm_request_connect.
* User may allow or not Cloud Connector to start TCP transport.
*
* @see connector_request_id_sm_request_connect
*/
typedef struct
{
    connector_transport_t CONST transport;  /**< transport from which the request is received */
    connector_bool_t allow;                 /**< if set to connector_true, TCP transport will start immediately. */
} connector_sm_request_connect_t;
/**
* @}
*/

#endif

#if !defined _CONNECTOR_API_H
#error  "Illegal inclusion of connector_api_short_message.h. You should only include connector_api.h in user code."
#endif
#else
#error  "Illegal inclusion of connector_api_short_message.h. You should only include connector_api.h in user code."
#endif
