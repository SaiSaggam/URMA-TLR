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

#ifndef _CONNECTOR_API_H
#define _CONNECTOR_API_H

#if (defined CONNECTOR_CONST_PROTECTION)
#define CONST
#undef CONNECTOR_CONST_PROTECTION
#else
#if (defined CONST)
#define CONNECTOR_CONST_STORAGE CONST
#undef CONST
#endif
#define CONST const
#endif

/**
 * Current version of Cloud Connector that application is using or built for.
 *
 * Note current version number denotes:  1.1.0.0
 *                                       | | | |
 *                                       v v v v
 *                                    0x01010000
 */
#define CONNECTOR_VERSION    0x02020000

#include "connector_config.h"

/* NOTE: The configuration macro validation below must stay in this place, which is
         after connector_config.h and before defining module specific data types. */

#if (defined CONNECTOR_FILE_SYSTEM_HAS_LARGE_FILES)
#define CONNECTOR_SUPPORTS_64_BIT_INTEGERS
#endif

#if !((defined CONNECTOR_TRANSPORT_TCP) || (defined CONNECTOR_TRANSPORT_UDP) || (defined CONNECTOR_TRANSPORT_SMS))
#define CONNECTOR_TRANSPORT_TCP
#endif

#if ((defined CONNECTOR_FIRMWARE_SERVICE) || (defined CONNECTOR_FILE_SYSTEM) || (defined CONNECTOR_RCI_SERVICE))
#if !(defined CONNECTOR_TRANSPORT_TCP)
#define CONNECTOR_TRANSPORT_TCP
#endif
#endif

#define CONNECTOR_TRANSPORT_COUNT ((defined CONNECTOR_TRANSPORT_TCP) + (defined CONNECTOR_TRANSPORT_UDP) + (defined CONNECTOR_TRANSPORT_SMS))

#if (defined CONNECTOR_DATA_POINTS) && !(defined CONNECTOR_DATA_SERVICE)
#define CONNECTOR_DATA_SERVICE
#endif

#if ((defined CONNECTOR_TRANSPORT_UDP) || (defined CONNECTOR_TRANSPORT_SMS))
#define CONNECTOR_SHORT_MESSAGE
#endif

#include "connector_types.h"

#define ARRAY_SIZE(array)  (sizeof array/sizeof array[0])

/**
 * @defgroup connector_bool_t Connector Boolean Type
 * @{
 */
/**
 * Connector Boolean Type
 *
 */
typedef enum {
    connector_false,    /**< False */
    connector_true      /**< True */
} connector_bool_t;
/**
* @}
*/

#include "api/connector_api_network.h"

 /**
 * @defgroup connector_port_numbers Device Cloud Port
 * Numbers @{
 */
/**
 * Port number to connect to Device Cloud
 *
 * This port number is for connector_network_open callback to make
 * a connection to Device Cloud.
 *
 * @see CONNECTOR_SSL_PORT
 * @see CONNECTOR_UDP_PORT
 * @see connector_network_open
 */
#define CONNECTOR_PORT       3197   /**< Port number used to connect to Device Cloud. */

/**
 * Port number to make a secure connection to Device Cloud
 *
 * This port number is for connector_network_open callback to make
 * a secured connection to Device Cloud.
 *
 * @see CONNECTOR_PORT
 * @see CONNECTOR_UDP_PORT
 * @see connector_network_open
 */
#define CONNECTOR_SSL_PORT   3199   /**< Secured port number used to connect to Device Cloud. */

/**
 * Device Cloud UDP port number
 *
 * This port number is for connector_network_open callback to communicate with
 * the Device Cloud over UDP.
 *
 * @see CONNECTOR_PORT
 * @see CONNECTOR_SSL_PORT
 * @see connector_network_open
 */
#define CONNECTOR_UDP_PORT       3297   /**< UDP port number used to communicate with Device Cloud. */
/**
* @}
*/

 /**
 * @defgroup Cloud_status_returns Cloud Connector Status values
 * @{
 */
 /**
 * Status returned by Cloud Connector API calls.
 */
typedef enum {
   connector_success,               /**< No error. */
   connector_init_error,            /**< Cloud Connector was not initialized. */
   connector_invalid_data_size,     /**< Callback returned configuration with invalid size. */
   connector_invalid_data_range,    /**< Callback returned configuration that is out of range. */
   connector_invalid_data,          /**< Callback returned invalid data. Callback returned a NULL data. */
   connector_keepalive_error,       /**< Cloud Connector did not receive keepalive messages. Device Cloud may be offline. */
   connector_bad_version,           /**< Invalid firmware version number. Incorrect firmware version number used in Remote Configuration (RCI) */
   connector_device_terminated,     /**< Cloud Connector was terminated by user via connector_initiate_action call.
                                     All memory is freed and connector_init must be called to reStart Cloud Connector. */
   connector_service_busy,          /**< Someone else is using the same service or the device is busy. */
   connector_invalid_response,      /**< Received invalid response from Device Cloud. */
   connector_no_resource,           /**< Lack of resource */
   connector_unavailable,           /**< Not available to perform the specified action. */
   connector_idle,                  /**< Cloud Connector is idling. Cloud Connector has no message to process.
                                     Other task should be processed. */
   connector_working,               /**< Cloud Connector has processed a message and should be called at the earliest possible time. */
   connector_pending,               /**< Cloud Connector is waiting to process a message or a callback to complete. This allows
                                     Cloud Connector to relinquish for other task to run. */
   connector_active,                /**< Cloud Connector is processing a message and it should be called at the earliest possible time. */
   connector_abort,                 /**< Cloud Connector was aborted and terminated either Cloud Connector encountered fatal error or callback aborted Cloud Connector.
                                     All memory is freed and connector_init must be called to reStart Cloud Connector. */
   connector_device_error,          /**< Close Callback returns error. If connector_step or connector_run is called again, it will re-establish the connection. */

   connector_exceed_timeout,        /**< Callback exceeded timeout value before it returned. */
   connector_invalid_payload_packet, /**< Cloud Connector received invalid payload message. */
   connector_open_error             /**< Open callback was not successfully opened and returned @ref connector_callback_error.
                                     Cloud Connector must be called again to continue running. It will reconnect if
                                     it's initially setup to connect automatically. */
} connector_status_t;
/**
* @}
*/

/**
* @defgroup connector_class_id_t Class IDs
* @{
*/
/**
* Class Id for which class is used in the application's callback.
* It tells class id for the request id passed to the application's callback.
*/
typedef enum {
    connector_class_id_config,           /**< Configuration Class Id */
    connector_class_id_operating_system, /**< Operating System Class Id */
    connector_class_id_firmware,         /**< Firmware Facility Class Id */
    connector_class_id_data_service,     /**< Data Service Class Id */
    connector_class_id_remote_config,    /**< Remote Configuration Class ID */
    connector_class_id_file_system,      /**< File System Class Id */
    connector_class_id_network_tcp,      /**< TCP Network Class ID */
    connector_class_id_network_udp,      /**< UDP Network Class ID */
    connector_class_id_network_sms,      /**< SMS Network Class ID */
    connector_class_id_status,           /**< Class ID for all status */
    connector_class_id_short_message,    /**< Short message specific class ID */
    connector_class_id_data_point        /**< Data point class ID */
} connector_class_id_t;
/**
* @}
*/

/**
* @defgroup connector_session_error_t Connector session error codes
* @{
*/
/**
* Error values returned from lower communication layer. It can be either from Device Cloud or from
* Connector. These are errors originated from a layer where compression/decompression, resource
* allocation and state handling takes place.
*/
typedef enum
{
    connector_session_error_none,                   /**< Success */
    connector_session_error_fatal,                  /**< Generally represents internal, unexpected error */
    connector_session_error_invalid_opcode,         /**< Opcode used in the message is invalid/unsupported */
    connector_session_error_format,                 /**< Packet is framed incorrectly */
    connector_session_error_session_in_use,         /**< Session with same ID is already in use */
    connector_session_error_unknown_session,        /**< Session is not opened or already closed */
    connector_session_error_compression_failure,    /**< Failed during compression of the data to send */
    connector_session_error_decompression_failure,  /**< Failed during decompression of the received data */
    connector_session_error_memory,                 /**< Malloc failed, try to restrict the number of active sessions */
    connector_session_error_send,                   /**< Send socket error */
    connector_session_error_cancel,                 /**< Used to force termination of a session */
    connector_session_error_busy,                   /**< Either the cloud or the connector is busy processing */
    connector_session_error_ack,                    /**< Invalid ack count */
    connector_session_error_timeout,                /**< Session timed out */
    connector_session_error_no_service,             /**< Requested service is not supported */
    connector_session_error_count                   /**< Maximum error count value, new value goes before this element */
} connector_session_error_t;
/**
* @}
*/


/**
* @defgroup connector_tcp_status_t Status Reason Types
* @{
*/
/**
* TCP Status values
*
* These status values are used for an event notification.
* @see connector_status_tcp_status_t
*/
typedef enum {
    connector_tcp_communication_started,      /**< Cloud Connector has established connection with Device Cloud and starts communicating. */
    connector_tcp_keepalive_missed,           /**< Cloud Connector didn't receive a keep alive message within the specified keep alive interval @ref tx_keepalive. */
    connector_tcp_keepalive_restored          /**< Cloud Connector received a keep alive message after a keep alive message was not received.
                                         This is called after the callback has been previously notified with connector_miss_keepalive status. */
} connector_tcp_status_t;
/**
* @}
*/

/**
* @defgroup connector_status_tcp_event_t TCP Status Structure
* @{
*/
/**
* TCP status structure is used for @ref connector_request_id_status_tcp callback. See @ref status_tcp.
*
* This is used for an event notification.
*/
typedef struct {
    connector_tcp_status_t CONST status; /**< TCP status */
} connector_status_tcp_event_t;
/**
* @}
*/


/**
* @defgroup connector_request_id_status_t  Cloud Connector status
* status request IDs @{
*/
/**
* These request IDs are used by Cloud Connector to communicate its status change to the user.
* The class ID associated with these request ID is connector_class_id_status.
*/
typedef enum {
    connector_request_id_status_tcp,            /**< Used in a callback for Cloud Connector TCP status. The callback is called to notify the application that
                                                    TCP connection has been established, a keep-alive message was not received, or keep-alive message was received and restored.
                                                    @see connector_tcp_status_t */
    connector_request_id_status_stop_completed  /**< Used in a callback when Cloud Connector has stopped a transport running via @ref connector_initiate_action call with @ref connector_initiate_transport_stop. */

} connector_request_id_status_t;
/**
* @}
*/

/**
* @defgroup connector_initiate_request_t Initiate action enumeration
* @{
*/
/**
* Request IDs used in connector_initiate_action()
*/
typedef enum
{
    connector_initiate_transport_start, /**< Starts the specified (TCP, UDP or SMS) transport method. */
    connector_initiate_transport_stop,  /**< Stops the specified (TCP, UDP or SMS) transport method. It will not try to reconnect regardless the value returned in the @ref connector_close_status_t*/

    #if (defined CONNECTOR_DATA_SERVICE)
    connector_initiate_send_data,       /**< Initiates the action to send data to the Device Cloud, the data will be stored in a file on Device Cloud. */
    #endif

    #if (defined CONNECTOR_SHORT_MESSAGE)
    connector_initiate_ping_request,       /**< Sends a ping request to Device Cloud. Supported only under UDP and SMS transport methods. */
    connector_initiate_session_cancel,     /**< Initiates the action to cancel the session, can be used in case of timeout. Supported only under UDP and SMS transport methods. */
    connector_initiate_session_cancel_all,  /**< Initiates the action to cancel all active sessions. Supported only under UDP and SMS transport methods. */
    #endif

    #if (defined CONNECTOR_DATA_POINTS)
    connector_initiate_data_point_binary,  /**< Initiates the action to send a binary data point to Device Cloud. */
    connector_initiate_data_point,         /**< Initiates the action to send data points to Device Cloud. */
    #endif

    connector_initiate_terminate        /**< Terminates and stops Cloud Connector from running. */
} connector_initiate_request_t;
/**
* @}
*/

/**
* @defgroup.connector_callback_status_t Callback return status
* @{
*/
/**
* Return status from Cloud Connector callback
*/
typedef enum  {
    connector_callback_continue,        /**< Continues with no error */
    connector_callback_busy,            /**< Callback is busy */
    connector_callback_abort,           /**< Aborts Cloud Connector. Cloud Connector frees all memory. @ref connector_init must be called to reStart Cloud Connector. */
    connector_callback_unrecognized,    /**< Unsupported callback request */
    connector_callback_error            /**< Exits Cloud Connector when error is encountered. Cloud Connector will try reconnecting to Device Cloud
                                         if @ref connector_step or @ref connector_run is called again. */
} connector_callback_status_t;
/**
* @}
*/

/**
* @}
*/

/**
* @defgroup connector_transport_t  Transport layer type
* @{
*/
/**
* Cloud Connector will use the specified transport
* method when sending a request to Device Cloud.
*/
typedef enum
{
#if (defined CONNECTOR_TRANSPORT_TCP)
    connector_transport_tcp, /**< Use TCP. @ref CONNECTOR_TRANSPORT_TCP must be enabled. */
#endif
#if (defined CONNECTOR_TRANSPORT_UDP)
    connector_transport_udp, /**< Use UDP. @ref CONNECTOR_TRANSPORT_UDP must be enabled. */
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
    connector_transport_sms, /**< Use SMS. @ref CONNECTOR_TRANSPORT_SMS must be enabled. */
#endif
    connector_transport_all  /**< All transports. */
} connector_transport_t;
/**
* @}
*/

typedef char const * connector_json_t;
typedef char const * connector_geojson_t;

#if (defined CONNECTOR_RCI_SERVICE)
#include "connector_api_remote.h"
#endif

#include "api/connector_api_config.h"
#include "api/connector_api_firmware.h"
#include "api/connector_api_data_service.h"
#include "api/connector_api_data_point.h"
#include "api/connector_api_file_system.h"
#include "api/connector_api_short_message.h"
#include "api/connector_api_os.h"


/**
* @defgroup connector_request_id_t Request IDs
* @{
*/
/**
* Request IDs passed to callback identifying the type of request
* @see connector_class_id_t
*/
typedef union {
    connector_request_id_config_t config_request;               /**< Configuration request ID for configuration class */
    connector_request_id_os_t os_request;                       /**< Operating system request ID for operating system class */
    #if (defined CONNECTOR_FIRMWARE_SERVICE)
    connector_request_id_firmware_t firmware_request;           /**< Firmware request ID for firmware facility class */
    #endif
    #if (defined CONNECTOR_DATA_SERVICE)
    connector_request_id_data_service_t data_service_request;   /**< Data service request ID for data service class */
    #endif
    #if (defined CONNECTOR_DATA_POINTS)
    connector_request_id_data_point_t data_point_request;       /**< Data service request ID for data service class */
    #endif
    #if (defined CONNECTOR_RCI_SERVICE)
    connector_request_id_remote_config_t remote_config_request; /**< Remote configuration request ID for remote configuration service class */
    #endif
    #if (defined CONNECTOR_FILE_SYSTEM)
    connector_request_id_file_system_t   file_system_request;   /**< File system request ID for file system class */
    #endif
    connector_request_id_network_t  network_request;            /**< Network request ID for network TCP class, network UDP class and network SMS class */
    connector_request_id_status_t status_request;               /**< Status request ID for status class */
    #if (defined CONNECTOR_SHORT_MESSAGE)
    connector_request_id_sm_t sm_request;                       /**< Short message request ID for short message class */
    #endif
    int int_value;                                             /**< For debugging purposes. */
} connector_request_id_t;
/**
* @}
*/

/**
* @defgroup connector_handle_t Cloud Connector Handle
* @{
*/
/**
*
* Cloud Connector Handle type that is used throughout Cloud Connector APIs, this is used by the application
* to store context information about a connections, for example this could
* be used to store a file descriptor or a pointer to a structure.
*/
#define connector_handle_t void *
/**
* @}
*/

/**
* @defgroup connector_config_error_status_t Error Status
* @{
*/
/**
* Error status structure for @ref connector_request_id_config_error_status callback which
* is called when Cloud Connector encounters an error.
* @see connector_request_id_config_t
*/
typedef struct  {
    connector_class_id_t CONST class_id;         /**< Class ID which Cloud Connector encounters error with */
    connector_request_id_t CONST request_id;     /**< Request ID which Cloud Connector encounters error with */
    connector_status_t CONST status;             /**< Error status */
} connector_config_error_status_t;
/**
* @}
*/

/**
* @defgroup connector_stop_condition_t Condition used on @ref connector_initiate_transport_stop in @ref connector_initiate_action
* @{
*/
/**
* This is used to tell how Cloud Connector handles all active sessions before stopping.
*
*/
typedef enum {
    connector_stop_immediately,             /**< Cancel all active sessions and stop the transport of Cloud Connector immediately */
    connector_wait_sessions_complete         /**< Stop the transport of Cloud Connector after all active sessions complete */
} connector_stop_condition_t;
/**
* @}
*/

/**
* @defgroup connector_initiate_stop_request_t Data type used to stop a transport of Cloud Connector
* @{
*/
/**
* This data structure is used on @ref connector_initiate_transport_stop in @ref connector_initiate_action API
* to stop a transport.
*
*/
typedef struct
{
    connector_transport_t transport;        /**< Transport will be stopped:
                                            - @ref connector_transport_tcp
                                            - @ref connector_transport_udp
                                            - @ref connector_transport_sms
                                            - @ref connector_transport_all */

    connector_stop_condition_t condition;   /**< Condition to stop the transport:
                                             - @ref connector_stop_immediately
                                             - @ref connector_wait_sessions_complete */

    void * user_context;                /**< User's defined context that will be passed to @ref connector_request_id_status_stop_completed after the transport of  Cloud Connector has stopped running. */

} connector_initiate_stop_request_t;
/**
* @}
*/


 /**
 * @defgroup connector_callback_t Application-defined callback
 * @{
 * @b Include: connector_api.h
 *
 * @brief   This is Cloud Connector Application
 *          defined callback.
 *
 * An application must define this function and pass this to Connector during the
 * connector_init() call.  This callback is used to pass configuration data and exchange system
 * services with the Connector state machine.  The callback
 * return value is then used to guide Connector how to
 * proceed after completion of the application callback.
 *
 */
 /**
 * @param class_id              This is a grouping or category of callback functions.  Each class_id contains a number of related request_id's.
 * @param request_id            The request ID defines the specific callback being requested.
 * @param data                  Points to specific structure for a given class ID and request ID.
 * @param context               The same context passed to @ref connector_init "connector_init()" as "context" argument.
 *
 * @retval  connector_callback_continue The callback completed successfully and Cloud Connector should continue
 *                                      it's process.
 * @retval  connector_callback_busy     The callback could not complete the operation within the allowable time frame.
 *                                      Do not advance the Cloud Connector state and recall this callback at some
 *                                      later time.
 * @retval  connector_callback_error    An application level error occurred while processing the callback.
 * @retval  connector_callback_abort    The application is requesting Cloud Connector to abort execution.  This will
 *                                      cause connector_run() or connector_step() to terminate with status @ref connector_abort.
 * @retval  connector_callback_unrecognized An unsupported and unrecognized callback was received.  The application does not
 *                                      support this request.  This should be implemented in the application to allow for
 *                                      graceful recovery and upgrade-ability to future Cloud Connector releases.
 *
 * @see connector_callback_status_t
 * @see connector_init()
 */
typedef connector_callback_status_t (* connector_callback_t) (connector_class_id_t const class_id, connector_request_id_t const request_id, void * const data, void * const context);
/**
* @}
*/

 /**
 * @defgroup connector_init Initialize Cloud Connector.
 *@{
 * @b Include: connector_api.h
 */
/**
 * @brief This API is called initially at startup to allocate and initialize Cloud Connector.
 *
 * This function takes the @ref iik_callback "application-defined callback" function pointer as
 * it's only argument.  This callback is used by Cloud Connector to communicate with the
 * application environment.  This function must be called prior to other Cloud Connector APIs,
 * like connector_step(), connector_run(), and connector_initiate_action().
 *
 * @param [in] callback  Callback function that is used to
 *        interface between the application and Cloud Connector.
 * @param [in] context  User-controlled context passed to "callback". Set to NULL if not used.
 *
 * @retval NULL         An error occurred and Cloud Connector was unable to initialize.
 * @retval "Not NULL"   Success.  A Handle was returned for subsequent Cloud Connector calls.
 *
 * Example Usage:
 * @code
 *    connector_handle = connector_init(application_callback);
 * @endcode
 *
 * @see connector_handle_t
 * @see connector_callback_t
 */
connector_handle_t connector_init(connector_callback_t const callback, void * const context);
/**
* @}
*/

/**
 * @defgroup connector_step Step Routine
 * @{
 * @b Include: connector_api.h
 */
/**
 * @brief   Executes a small portion of Cloud Connector in a non-blocking call.
 *
 * This non-blocking function is used to execute a portion Cloud Connector after it's been
 * successfully initialized with an connector_init() call.  This function performs a sequence of
 * operations or events and then returns.  This allows applications to perform other tasks,
 * especially in non-threading environment.  For more information on the differences between
 * connector_run() and connector_step() see the @ref threading "Cloud Connector threading model".
 *
 * This function is similar to connector_run() except this is the non-blocking variant.  This function would
 * normally be made from system control loop or state machine.  This function must be repeatedly made to
 * maintain Cloud Connector state.
 *
 * See Cloud Connector @ref threading "threading model" section for more information on the differences
 * between connector_run() and connector_step().
 *
 * @param [in] handle  Handle returned from connector_init
 *
 * @retval connector_init_error             Cloud Connector was not properly initialized.  The application requires a
 *                                      call to connector_init() prior to calling this function.
 * @retval connector_abort                  Cloud Connector was aborted by callback function.
 * @retval connector_invalid_data_size      An @ref iik_callback "application callback" returned an invalid response_length.
 *                                      This is an application callback defect and should be corrected.  To locate the
 *                                      faulty callback, enable @ref CONNECTOR_DEBUG.  For more information on
 *                                      debugging, see the section on @ref debug_routine "how to implement debug support".
 * @retval connector_invalid_data_range     An @ref iik_callback "application callback" returned invalid response_data.
 *                                      This is an application callback defect and should be corrected.  To locate the
 *                                      faulty callback, enable @ref CONNECTOR_DEBUG.  For more information on
 *                                      debugging, see the section on @ref debug_routine "how to implement debug support".
 * @retval connector_invalid_payload_packet Cloud Connector received invalid payload message.
 * @retval connector_keepalive_error        Cloud Connector did not receive keep alive messages. Device Cloud may be offline.
 * @retval connector_bad_version            Device Cloud rejected version number.
 * @retval connector_exceed_timeout         Callback exceeded timeout value before it returned.
 * @retval connector_unsupported_security   Cloud Connector received a packet with unsupported security.
 * @retval connector_invalid_data           An @ref iik_callback "application callback" returned unexpected NULL response_data.
 *                                      This is an application callback defect and should be corrected.  To locate the
 *                                      faulty callback, enable @ref CONNECTOR_DEBUG.  For more information on
 *                                      debugging, see the section on @ref debug_routine "how to implement debug support".
 * @retval connector_device_terminated      Cloud Connector was terminated by user via connector_initiate_action() call.
 * @retval connector_idle                   Cloud Connector is idling. Cloud Connector has no message to process and relinquishes other task execution.
 * @retval connector_working                Cloud Connector is processing a message and should be called at the earliest possible time.
 * @retval connector_pending                Cloud Connector is busy or waiting to process a message and relinquishes other task execution
 * @retval connector_active                 Cloud Connector is busy or waiting to process a message and should be called
 *                                      at the earliest possible time
 *
 *
 * Example Usage:
 * @code
 *     connector_status_t status;
 *     connector_handle_t connector_handle = connector_init(application_callback);
 *
 *     for (;;)
 *     {
 *          status = connector_step(connector_handle);
 *          other_system_tasks();
 *     }
 * @endcode
 *
 * @see connector_init()
 * @see connector_handle_t
 * @see connector_status_t
 * @see connector_run()
 */
connector_status_t connector_step(connector_handle_t const handle);
/**
* @}
*/


 /**
 * @defgroup connector_run Run routine
 * @{
 * @b Include: connector_api.h
 */
/**
 * @brief   Runs Cloud Connector as a blocking call.
 *
 * This blocking function is typically used in a separate thread (in a multi-threaded environment) to run
 * Cloud Connector after it's been successfully initialized with an connector_init() call.  This function
 * only returns when Cloud Connector encounters an unexpected error.  This function is similar to
 * connector_step() except this is the blocking variant.
 *
 * See Cloud Connector @ref threading "Threading Model section" for more information on the differences
 * between connector_run() and connector_step().
 *
 * @param [in] handle                   Handle returned from connector_init() call.
 *
 * @retval connector_init_error             Cloud Connector was not properly initialized.  The application requires a
 *                                      call to connector_init() prior to calling this function.
 * @retval connector_abort                  Cloud Connector was aborted by callback function.
 * @retval connector_invalid_data_size      An @ref iik_callback "application callback" returned an invalid response_length.
 *                                      This is an application callback defect and should be corrected.  To locate the
 *                                      faulty callback, enable @ref CONNECTOR_DEBUG.  For more information on
 *                                      debugging, see the section on @ref debug_routine "how to implement debug support".
 * @retval connector_invalid_data_range     An @ref iik_callback "application callback" returned invalid response_data.
 *                                      This is an application callback defect and should be corrected.  To locate the
 *                                      faulty callback, enable @ref CONNECTOR_DEBUG.  For more information on
 *                                      debugging, see the section on @ref debug_routine "how to implement debug support".
 * @retval connector_keepalive_error        Cloud Connector did not receive keep alive messages. Device Cloud may be offline.
 * @retval connector_bad_version            Device Cloud rejected version number.
 * @retval connector_exceed_timeout         Callback exceeded timeout value before it returned.
 * @retval connector_unsupported_security   Cloud Connector received a packet with unsupported security.
 * @retval connector_invalid_data           An @ref iik_callback "application callback" returned unexpected NULL response_data.
 *                                      This is an application callback defect and should be corrected.  To locate the
 *                                      faulty callback, enable @ref CONNECTOR_DEBUG.  For more information on
 *                                      debugging, see the section on @ref debug_routine "how to implement debug support".
 * @retval connector_device_error           Close Callback returns
 *                                      error. If connector_step or connector_run is called again, it
 *                                      will re-establish the connection.
 * @retval connector_device_terminated      Cloud Connector was terminated by user via connector_initiate_action call.
 *
 * Example Usage:
 * @code
 *     connector_status_t status;
 *     connector_handle_t connector_handle = connector_init(application_callback);
 *
 *     status = connector_run(connector_handle);
 * @endcode
 *
 * @see connector_init()
 * @see connector_handle_t
 * @see connector_status_t
 * @see connector_step()
 */
connector_status_t connector_run(connector_handle_t const handle);
/**
* @}
*/


 /**
 * @defgroup connector_initiate_action Initiate Action
 * @{
 * @b Include: connector_api.h
 */
/**
 * @brief   Requests Cloud Connector to perform an asynchronous action.
 *
 * This function is called to initiate an Cloud Connector action.  It can be
 * used to initiate a send data sequence from the device to Device Cloud,
 * or to terminate Cloud Connector library.
 *
 * @param [in] handle  Handle returned from the connector_init() call.
 *
 * @param [in] request  Request action (one of the following):
 *                      @li @b connector_initiate_terminate:
 *                          Terminates and stops Cloud Connector from running.  This call closes all open network
 *                          Handles and frees all allocated memory.
 *                          If the application is using connector_step(), the next call to connector_step() terminates
 *                          Cloud Connector.  If a blocking call to connector_run() is still pending, this call
 *                          will eventually terminate that call.  Once Cloud Connector is terminated, the
 *                          Cloud Connector must restart by calling connector_init().
 *
 *                      @li @b connector_initiate_send_data:
 *                           This is used to trigger the send
 *                           data to Device Cloud. Only the
 *                           header information is passed by
 *                           this method. The actual data is
 *                           transferred through callbacks. The
 *                           data is stored in a specified file
 *                           on Device Cloud.
 *
 *                      @li @b connector_initiate_transport_start:
 *                          Starts the specified (@ref connector_transport_tcp
 *                          , @ref connector_transport_udp or @ref connector_transport_sms) transport method.
 *
 *                      @li @b connector_initiate_transport_stop:
 *                          Stops the Cloud Connector transport mechanism.  Note this call
 *                          triggers a @ref connector_request_id_status_stop_completed callback.
 *                          Transports include:
 *                              - @ref connector_transport_tcp - TCP transport
 *                              - @ref connector_transport_udp - UDP transport
 *                              - @ref connector_transport_sms - SMS transport
 *                              - @ref connector_transport_all - all transports.
 *
 *                      @li @b connector_initiate_data_point_binary:
 *                          Initiates the action to send a binary data point to Device Cloud.
 *
 *                      @li @b connector_initiate_data_point:
 *                          Initiates the action to send data points to Device Cloud.
 *
 *                      @li @b connector_initiate_ping_request:
 *                          Sends status message to the Device Cloud.  Supported for
 *                          @ref connector_transport_udp and @ref connector_transport_sms transports method only.
 *
 *                      @li @b connector_initiate_session_cancel:
 *                          Initiates the action to cancel a session.
 *
 *                      @li @b connector_initiate_session_cancel_all:
 *                          Initiates the action to cancel all sessions.
 *
 * @param [in] request_data  Pointer to Request data
 *                      @li @b connector_initiate_terminate:
 *                          Should be NULL.
 *                      @li @b connector_initiate_send_data:
 *                          Pointer to @ref connector_request_data_service_send_t "connector_request_data_service_send_t".
 *                      @li @b connector_initiate_transport_start:
 *                          Pointer to @ref connector_transport_t "connector_transport_t"
 *                      @li @b connector_initiate_transport_stop:
 *                          Pointer to @ref connector_initiate_stop_request_t "connector_initiate_stop_request_t"
 *                      @li @b connector_initiate_data_point_binary:
 *                          Pointer to @ref connector_request_data_point_binary_t "connector_request_data_point_binary_t"
 *                      @li @b connector_initiate_data_point:
 *                          Pointer to @ref connector_request_data_point_t "connector_request_data_point_t"
 *                      @li @b connector_initiate_ping_request:
 *                          Pointer to @ref connector_sm_send_ping_request_t "connector_sm_send_ping_request_t"
 *                      @li @b connector_initiate_session_cancel:
 *                          Pointer to @ref connector_sm_cancel_request_t "connector_sm_cancel_request_t"
 *
 * @retval connector_success              No error
 * @retval connector_init_error           Cloud Connector was not initialized or not connected to Device Cloud.
 * @retval connector_abort                Callback aborted Cloud Connector.
 * @retval connector_invalid_data         Invalid parameter
 * @retval connector_no_resource          Insufficient memory
 * @retval connector_service_busy         Cloud Connector is busy
 *
 * Example Usage:
 * @code
 *     static connector_request_data_service_send_t  file_info;
 *     :
 *     status = connector_initiate_action(handle, connector_initiate_send_data, &file_info);
 *     :
 *     :
 *     status = connector_initiate_action(connector_handle, connector_initiate_terminate, NULL);
 * @endcode
 *
 * @see connector_handle_t
 * @see connector_callback_t
 */
connector_status_t connector_initiate_action(connector_handle_t const handle, connector_initiate_request_t const request, void const * const request_data);
/**
* @}.
*/

#undef CONST
#if (defined CONNECTOR_CONST_STORAGE)
#define CONST CONNECTOR_CONST_STORAGE
#undef CONNECTOR_CONST_STORAGE
#endif

#endif
