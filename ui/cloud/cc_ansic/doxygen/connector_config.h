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
 /**
  * @file
  *  @brief Cloud Connector configuration options
  *
  */
#ifndef __CONNECTOR_CONFIG_H_
#define __CONNECTOR_CONFIG_H_


/**
* @defgroup connector_config_options Cloud Connector Configuration Options
* @{
*
* @brief Defines the major blocks of functionality used in Cloud Connector
*/

/**
 *  When defined, the system is little endian.
 *
 *  @note For Big endian systems, this line should be commented out.
 */
#define CONNECTOR_LITTLE_ENDIAN

/**
 * When defined, Cloud Connector private library includes debug support.  Cloud Connector state
 * and debug data will be displayed using the @ref connector_debug_vprintf. See @ref debug_routine
 * for its implementation. Macro error testing is compiled and implemented using the
 * @ref ASSERT macro.
 *
 * This debug feature can be eliminated in a product release, which will greatly reduce memory codespace and
 * RAM needs, by commenting this line out in connector_config.h:
 *
 * @code
 * #define CONNECTOR_DEBUG
 * @endcode
 *
 * To this:
 * @code
 * //#define CONNECTOR_DEBUG
 * @endcode
 *
 * @see connector_debug_printf
 * @see ASSERT
 */
#define CONNECTOR_DEBUG

/**
 * When defined, Cloud Connector private library includes the @ref firmware_download
 * "Firmware Download Service".
 *
 * To disable the @ref firmware_download "Firmware Download Service" feature, comment
 * this line out in connector_config.h:
 *
 * @code
 * #define CONNECTOR_FIRMWARE_SERVICE
 * @endcode
 *
 * To this:
 * @code
 * //#define CONNECTOR_FIRMWARE_SERVICE
 * @endcode
 *
 * @see @ref firmware_support
 */
#define CONNECTOR_FIRMWARE_SERVICE

/**
 * When defined, Cloud Connector includes the @ref zlib "compression" support used with the
 * @ref data_service.
 *
 * By default, compression support is disabled.  To enable Optional Data Compression Support,
 * change this line in connector_config.h:
 * By default, optional @ref zlib "compression" support is disabled.
 *
 * To enable @ref zlib "compression", uncomment the define in connector_config.h:
 *
 * @code
 * //#define CONNECTOR_COMPRESSION
 * @endcode
 *
 * To this:
 * @code
 * #define CONNECTOR_COMPRESSION
 * @endcode
 *
 * @note When included, this requires the @ref zlib "zlib" library.
 *
 * @see @ref data_service
 * @see @ref CONNECTOR_DATA_SERVICE
 * @see @ref CONNECTOR_FILE_SYSTEM
 * @see @ref CONNECTOR_RCI_SERVICE
 *
 */
#define CONNECTOR_COMPRESSION

/**
 * If defined, Cloud Connector includes the @ref data_service.
 * To disable the @ref data_service feature, comment this line out in connector_config.h:
 *
 * @code
 * #define CONNECTOR_DATA_SERVICE
 * @endcode
 *
 * To this:
 * @code
 * //#define CONNECTOR_DATA_SERVICE
 * @endcode
 *
 * @see @ref data_service_support
 * @see @ref zlib
 * @see @ref CONNECTOR_COMPRESSION
 */
#define CONNECTOR_DATA_SERVICE

/**
* If defined, Cloud Connector includes the @ref data_point.
* To disable the @ref data_point feature, comment this line out in connector_config.h:
*
* @code
* #define CONNECTOR_DATA_POINTS
* @endcode
*
* To this:
* @code
* //#define CONNECTOR_DATA_POINTS
* @endcode
*
* @see @ref data_point
* @see @ref zlib
* @see @ref CONNECTOR_COMPRESSION
*/
#define CONNECTOR_DATA_POINTS

/**
 * If defined, Cloud Connector includes the @ref file_system.
 * To enable the @ref file_system feature, uncomment this line in connector_config.h:
 *
 * @code
 * // #define CONNECTOR_FILE_SYSTEM
 * @endcode
 *
 * To this:
 * @code
 * #define CONNECTOR_FILE_SYSTEM
 * @endcode
 *
 * @see @ref file_system_support
 * @see @ref zlib
 * @see @ref CONNECTOR_COMPRESSION
 */
#define CONNECTOR_FILE_SYSTEM

/**
 * If defined, Cloud Connector includes the @ref rci_service.
 * To disable the @ref rci_service feature, comment this line out in connector_config.h:
 *
 * @code
 * #define CONNECTOR_RCI_SERVICE
 * @endcode
 *
 * To this:
 * @code
 * // #define CONNECTOR_RCI_SERVICE
 * @endcode
 *
 * @see @ref rci_service
 * @see @ref app_get_remote_configuration_support()
 * @see @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH
 * @see @ref zlib
 * @see @ref CONNECTOR_COMPRESSION
 */
#define CONNECTOR_RCI_SERVICE

/**
 * When @ref CONNECTOR_NO_MALLOC is defined, this defines the maximum content length in bytes of an element's value for the @ref rci_service.
 *
 * @code
 * #define CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH 256
 * @endcode
 *
 * @see @ref CONNECTOR_RCI_SERVICE
 */
#define CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH    256

/**
* If defined, Cloud Connector includes the @ref cli_support.
* To disable the @ref cli_support feature, comment this line out in connector_config.h:
*
* @code
* #define CONNECTOR_SM_CLI
* @endcode
*
* To this:
* @code
* //#define CONNECTOR_SM_CLI
* @endcode
*
* @see @ref shortmessaging
* @see @ref zlib
* @see @ref CONNECTOR_COMPRESSION
*/
#define CONNECTOR_SM_CLI

/**
 * This is used to define the maximum length in bytes of the full file path on the device, supported by the @ref file_system.
 * This length includes an ending null-character.
 *
 * For example, if the device receives a request to list the directory '/mydir' and the '/mydir' directory has an entry
 * 'myfile', the full path to the directory entry is '/mydir/myfile'. This full path '/mydir/myfile' has a length of 14 bytes,
 * including an ending null-character.
 *
 * @note System limitation: CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH must be <= 460
 *
 * See @ref CONNECTOR_FILE_SYSTEM to enable or disable the service.
 *
 * @code
 * #define CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH 256
 * @endcode
 *
 * @see @ref CONNECTOR_FILE_SYSTEM
 * @see @ref file_system_readdir "Read a directory" callback
 */
#define CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH   256

/**
 * When defined, Cloud Connector private library does not use dynamic memory allocations and
 * static memory buffers are used instead. This eliminates the possibility of memory fragmentation.
 *
 * When no dynamic RAM option is used, @ref 
 * CONNECTOR_MSG_MAX_TRANSACTION should be defined and it must
 * be less or equal to 32. The default is 1. 
 *  
 * When no dynamic RAM option is used, @ref 
 * CONNECTOR_NO_MALLOC_MAX_SEND_SESSIONS should be defined and 
 * it must be less or equal to 32. The default is 1. 
 *
 * @note If using @ref rci_support, then @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH must be defined.
 *
 * To enable no dynamic RAM feature, uncomment this line in connector_config.h:
 *
 * @code
 * //#define CONNECTOR_NO_MALLOC
 * @endcode
 *
 * To this:
 * @code
 * #define CONNECTOR_NO_MALLOC
 * @endcode
 *
 */
#define CONNECTOR_NO_MALLOC

/**
 * Maximum number of simultaneous send data sessions over @ref 
 * CONNECTOR_TRANSPORT_TCP "TCP transport" if @ref 
 * CONNECTOR_NO_MALLOC is defined. 
 *  
 * The default is 1. Must be less or equal to 32.
 *  
 * If @ref CONNECTOR_NO_MALLOC is not defined, the number of
 * simultaneous send data sessions is unlimited.
 *  
 */
#define CONNECTOR_NO_MALLOC_MAX_SEND_SESSIONS 1

/**
 * If defined, Cloud Connector includes the TCP transport.
 * To disable this feature, comment this line out in connector_config.h:
 *
 * @code
 * #define CONNECTOR_TRANSPORT_TCP
 * @endcode
 *
 * To this:
 * @code
 * // #define CONNECTOR_TRANSPORT_TCP
 * @endcode
 *
 * @see @ref network_tcp_start
 * @see @ref app_start_network_tcp()
 */
#define CONNECTOR_TRANSPORT_TCP

/**
* If defined, Cloud Connector includes the UDP transport.
* To disable this feature, comment this line out in connector_config.h:
*
* @code
* #define CONNECTOR_TRANSPORT_UDP
* @endcode
*
* To this:
* @code
* // #define CONNECTOR_TRANSPORT_UDP
* @endcode
*
* @see @ref network_udp_start
* @see @ref app_start_network_udp()
*/
#define CONNECTOR_TRANSPORT_UDP

/**
* If defined, Cloud Connector includes the SMS transport.
* To disable this feature, comment this line out in connector_config.h:
*
* @code
* #define CONNECTOR_TRANSPORT_SMS
* @endcode
*
* To this:
* @code
* // #define CONNECTOR_TRANSPORT_SMS
* @endcode
*
* @see @ref network_sms_start
* @see @ref app_start_network_sms()
*/
#define CONNECTOR_TRANSPORT_SMS

/**
 * This is used to enable support for file sizes larger than 2
 * gigabyte in file system listing requests. On linux platform 
 * the following must be added to Makefile: 
 *  
 * CFLAGS += D_FILE_OFFSET_BITS=64 
 *
 * @code
 * #define CONNECTOR_FILE_SYSTEM_HAS_LARGE_FILES
 * @endcode
 *
 * @see @ref file_system_stat callback 
 * @see @ref file_system_stat_dir_entry callback  
 * @see @ref CONNECTOR_FILE_SYSTEM 
 */
#define CONNECTOR_FILE_SYSTEM_HAS_LARGE_FILES

/**
* If @ref CONNECTOR_TRANSPORT_UDP or @ref CONNECTOR_TRANSPORT_SMS are defined, Cloud Connector will use 
* the define below to enable multipart message functionality. If not set, Cloud Connector will only allow 
* transmission and reception of single segment messages.
* @see @ref shortmessaging
* @see @ref CONNECTOR_TRANSPORT_UDP
* @see @ref CONNECTOR_TRANSPORT_SMS
*/
#define CONNECTOR_SM_MULTIPART

/**
* If @ref CONNECTOR_TRANSPORT_UDP is defined, Cloud Connector will use the define below to set the maximum Short Messaging over UDP sessions active at a time.
* If not set, Cloud Connector will call @ref connector_request_id_config_sm_udp_max_sessions configuration callback.
*
* @see @ref sm_udp_max_sessions
* @see @ref shortmessaging
* @see @ref CONNECTOR_TRANSPORT_UDP
*/
#define CONNECTOR_SM_UDP_MAX_SESSIONS                  4

/**
* If @ref CONNECTOR_TRANSPORT_SMS is defined, Cloud Connector will use the define below to set the maximum Short Messaging over SMS sessions active at a time.
* If not set, Cloud Connector will call @ref connector_request_id_config_sm_sms_max_sessions configuration callback.
*
* @see @ref sm_sms_max_sessions
* @see @ref shortmessaging
* @see @ref CONNECTOR_TRANSPORT_SMS
*/
#define CONNECTOR_SM_SMS_MAX_SESSIONS                  4

/**
* If @ref CONNECTOR_TRANSPORT_UDP is defined, Cloud Connector will use the define below to set the maximum Short Message segments used per session during message reception 
* This define does not affect the number of segments for transmission which is unlimitted, except for Data Points, see @ref CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS. 
* If not set, Cloud Connector will call @ref connector_request_id_config_sm_udp_max_rx_segments configuration callback.
* User needs to increase this value if SM over UDP is expected to receive larger data. 
* @ref CONNECTOR_SM_MULTIPART must be defined in order to configure this parameter to a value bigger than 1.
*
* @see @ref sm_udp_max_rx_segments
* @see @ref shortmessaging
* @see @ref CONNECTOR_TRANSPORT_UDP
*/
#define CONNECTOR_SM_UDP_MAX_RX_SEGMENTS               4

/**
* If @ref CONNECTOR_TRANSPORT_SMS is defined, Cloud Connector will use the define below to set the maximum Short Message segments used per session during message reception 
* This define does not affect the number of segments for transmission which is unlimitted, except for Data Points, see @ref CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS. 
* If not set, Cloud Connector will call @ref connector_request_id_config_sm_sms_max_rx_segments configuration callback.
* User needs to increase this value if SM over UDP is expected to receive larger data. 
* @ref CONNECTOR_SM_MULTIPART must be defined in order to configure this parameter to a value bigger than 1.
*
* @see @ref sm_sms_max_rx_segments
* @see @ref shortmessaging
* @see @ref CONNECTOR_TRANSPORT_UDP
*/
#define CONNECTOR_SM_SMS_MAX_RX_SEGMENTS               4

/**
* If @ref CONNECTOR_TRANSPORT_UDP or @ref CONNECTOR_TRANSPORT_SMS are defined and your application
* sends Data Points over one of these transports, Cloud Connector will use the define below to allocate
* a buffer to process the @ref connector_initiate_data_point requests
* that use SMS or UDP. If both are defined, the buffer will be allocated with the maximum value.
* This define does not affect the number of segments for reception (see @ref CONNECTOR_SM_UDP_MAX_RX_SEGMENTS and @ref CONNECTOR_SM_SMS_MAX_RX_SEGMENTS).
* Each Data Point is sent in CSV format (one per line, '\\n' terminated). The default order is:\n\n
* DATA, TIMESTAMP, QUALITY, DESCRIPTION, LOCATION, DATATYPE, UNITS, FORWARDTO \n\n
* All fields are sent, even empty ones (i.e.: data,1,,,99)
* For more information, please refer to the <a href="http://ftp1.digi.com/support/documentation/html/90002008">Device Cloud Programming Guide</a> 
* section "Data Streams", subsection "CSV".\n
* If this macro is not defined, Cloud Connector will use the default value of 1.
* @ref CONNECTOR_SM_MULTIPART must be defined in order to configure this parameter to a value bigger than 1.
*
* @see @ref shortmessaging
* @see @ref data_point
* @see @ref CONNECTOR_TRANSPORT_UDP
* @see @ref CONNECTOR_TRANSPORT_SMS
*/
#define CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS      4

/**
* If @ref CONNECTOR_TRANSPORT_UDP is defined, Cloud Connector will use the define below to set the timeout (in seconds) for all incoming UDP sessions. 
* If not set, Cloud Connector will call @ref connector_request_id_config_sm_udp_rx_timeout configuration callback.
* It can be set to SM_WAIT_FOREVER for infinite wait time but it is not recommended as delivery is not guaranteed in Short Messaging.
* 
* @see @ref sm_udp_rx_timeout
* @see @ref shortmessaging
* @see @ref CONNECTOR_TRANSPORT_UDP
* @see @ref CONNECTOR_TRANSPORT_SMS
*/
#define CONNECTOR_SM_UDP_RX_TIMEOUT                    30

/**
* If @ref CONNECTOR_TRANSPORT_SMS is defined, Cloud Connector will use the define below to set the timeout (in seconds) for all incoming SMS sessions.
* If not set, Cloud Connector will call @ref connector_request_id_config_sm_sms_rx_timeout configuration callback.
* It can be set to SM_WAIT_FOREVER for infinite wait time but it is not recommended as delivery is not guaranteed in Short Messaging.
* 
* @see @ref sm_sms_rx_timeout
* @see @ref shortmessaging
* @see @ref CONNECTOR_TRANSPORT_UDP
* @see @ref CONNECTOR_TRANSPORT_SMS
*/
#define CONNECTOR_SM_SMS_RX_TIMEOUT                    (30 * 60)

/**
 * When defined, the Cloud Connector compilation will expect the C99 stdint.h header file,
 * even though we're setup for an C89 environment.
 *
 * This define should get added to your makefile:
 *
 * @code
 * CFLAGS +=-std=c89 -DCONNECTOR_HAS_STDINT_HEADER
 * @endcode
 *
 * @see @ref errors_due_to_C89_and_stdint
 * @see @ref resolving_compilation_issues
 *
 */
#define CONNECTOR_HAS_STDINT_HEADER

/**
 * When defined, the Cloud Connector compilation will expect the compiler has float support by:
 *    - float and double types are supported. 
 *    - "%f" and "%lf" format specifiers are supported.
 *
 */
#define CONNECTOR_SUPPORTS_FLOATING_POINT

/**
 * When defined, the Cloud Connector compilation will expect the compiler has 64 bit integers support by:
 *    - int64_t and uint64_t types are supported. 
 *    - "%lld" and "%" PRId64 format specifiers are supported.
 *
 */
#define CONNECTOR_SUPPORTS_64_BIT_INTEGERS

/**
 * Sets the outbound packets' maximum size in UDP transport (it has no effect on SM/SMS or EDP/TCP).
 * This change only has effect if no multipacket messaging is enabled (@ref CONNECTOR_SM_MULTIPART).
 * If it is not defined the default value of 1472 is used. This value is used for buffers declaration
 * so it might help to save RAM. If you do not know which value to use, leave it undefined.
 *
 */
#define SM_PACKET_SIZE_UDP        1472

/**
 * @}*/

#ifdef ENABLE_COMPILE_TIME_DATA_PASSING

/**
 * @defgroup connector_config_data_options Hard Coded Application Configuration options
 * @{
 *
 *  @brief Hard Coded Application Configuration options is an optional method where configuration is pulled from hardcode
 *  defined at compile time instead of the @ref connector_callback_t "Application callback" method.
 *
 *  If the following configurations in @ref connector_config.h are defined, the code to make the @ref connector_callback_t "application callbacks"
 *  is uncompiled and the hard-coded values are used instead:
 *
 * @code
 *     #define CONNECTOR_DEVICE_TYPE                          "Cloud Connector Linux Sample"
 *     #define CONNECTOR_CLOUD_URL                            "login.etherios.com"
 *     #define CONNECTOR_TX_KEEPALIVE_IN_SECONDS              90
 *     #define CONNECTOR_RX_KEEPALIVE_IN_SECONDS              60
 *     #define CONNECTOR_WAIT_COUNT                           5
 *     #define CONNECTOR_VENDOR_ID                            0x00000000
 *     #define CONNECTOR_MSG_MAX_TRANSACTION                  1
 *     #define CONNECTOR_CONNECTION_TYPE                      connector_connection_type_lan
 *     #define CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND    0
 *     #define CONNECTOR_WAN_PHONE_NUMBER_DIALED              "0123456789"
 *     #define CONNECTOR_FIRMWARE_SUPPORT
 *     #define CONNECTOR_DATA_SERVICE_SUPPORT
 *     #define CONNECTOR_FILE_SYSTEM_SUPPORT
 *     #define CONNECTOR_REMOTE_CONFIGURATION_SUPPORT
 *     #define CONNECTOR_DEVICE_ID_METHOD                     connector_device_id_method_auto
 *     #define CONNECTOR_NETWORK_TCP_START                    connector_connect_auto
 *     #define CONNECTOR_NETWORK_UDP_START                    connector_connect_auto
 *     #define CONNECTOR_NETWORK_SMS_START                    connector_connect_auto
 *     #define CONNECTOR_WAN_TYPE                             connector_wan_type_imei
 *     #define CONNECTOR_IDENTITY_VERIFICATION                connector_identity_verification_simple
 *     #define CONNECTOR_CLOUD_PHONE                          "447786201216"
 *     #define CONNECTOR_CLOUD_SERVICE_ID                     "" /*empty: No shared-code used *\/
 * @endcode
 *
 * This option is useful for reducing Cloud Connector code space for those applications that are sensitive to memory usage.  The following
 * table describes the each define and the callback cross reference:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Configuration Defines</td>
 * <th class="title">Description</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_DEVICE_TYPE @htmlonly</td>
 * <td>Device type in iso-8859-1 encoded string to identify the device.
 * See @endhtmlonly @ref device_type @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_CLOUD_URL @htmlonly </td>
 * <td>Device Cloud FQDN (typically, login.etherios.com or login.etherios.co.uk).
 * See @endhtmlonly @ref device_cloud_url @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_TX_KEEPALIVE_IN_SECONDS @htmlonly </td>
 * <td>TX Keepalive interval in seconds between 5 and 7200 seconds.
 * See @endhtmlonly @ref tx_keepalive @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_RX_KEEPALIVE_IN_SECONDS @htmlonly </td>
 * <td>RX Keepalive interval in seconds between 5 and 7200 seconds.
 * See @endhtmlonly @ref rx_keepalive @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_WAIT_COUNT @htmlonly </td>
 * <td>Maximum number of consecutive missing keepalive messages for a
 * connection to be considered lost.  Valid range is greater than 1 and less than 64.
 * See @endhtmlonly @ref wait_count @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_VENDOR_ID @htmlonly </td>
 * <td>Vendor ID from Device Cloud Account.
 * See @endhtmlonly @ref vendor_id @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_MSG_MAX_TRANSACTION @htmlonly </td>
 * <td>Maximum simultaneous transactions for data service receiving message.
 * See @endhtmlonly @ref max_msg_transactions @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_CONNECTION_TYPE @htmlonly </td>
 * <td> @endhtmlonly @ref connector_connection_type_lan @htmlonly for LAN connection or
 * @endhtmlonly @ref connector_connection_type_wan @htmlonly WAN connection.
 * See @endhtmlonly @ref connection_type @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND @htmlonly </td>
 * <td>Link speed for WAN connection.
 * See @endhtmlonly @ref link_speed @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_WAN_PHONE_NUMBER_DIALED @htmlonly </td>
 * <td>String representation of a WAN cellular phone connection.
 * See @endhtmlonly @ref phone_number @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_FIRMWARE_SUPPORT @htmlonly </td>
 * <td>If defined it enables firmware download capability.
 * See @endhtmlonly @ref firmware_support @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_DATA_SERVICE_SUPPORT @htmlonly </td>
 * <td>If defined it enables data service capability.
 * See @endhtmlonly @ref data_service_support @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_FILE_SYSTEM_SUPPORT @htmlonly </td>
 * <td>If defined it enables file system capability.
 * See @endhtmlonly @ref file_system_support @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_REMOTE_CONFIGURATION_SUPPORT @htmlonly </td>
 * <td>If defined it enables remote configuration capability.
 * See @endhtmlonly @ref rci_support @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_NETWORK_TCP_START @htmlonly </td>
 * <td>If defined it enables and starts TCP transport.
 * See @endhtmlonly @ref network_tcp_start @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_NETWORK_UDP_START @htmlonly </td>
 * <td>If defined it enables and starts UDP transport.
 * See @endhtmlonly @ref network_udp_start @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_NETWORK_SMS_START @htmlonly </td>
 * <td>If defined it enables and starts SMS transport.
 * See @endhtmlonly @ref network_sms_start @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_IDENTITY_VERIFICATION @htmlonly </td>
 * <td> @endhtmlonly @ref identity_verification @htmlonly for
 * simple or password identity verification form.
 * See @endhtmlonly @ref identity_verification @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_CLOUD_PHONE @htmlonly </td>
 * <td>Device Cloud Phone Number where to send SMSs (Only used for SMS transport).
 * See @endhtmlonly @ref get_device_cloud_phone and @ref set_device_cloud_phone @htmlonly</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_CLOUD_SERVICE_ID @htmlonly </td>
 * <td>Device Cloud Service ID (if required) used during SMS transport.
 * See @endhtmlonly @ref device_cloud_service_id @htmlonly</td>
 * </tr>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 *
 *
 */
/**
 * When defined, this string is hardcode for the @ref device_type instead of the application framework
 * function @ref app_get_device_type (called via the @ref connector_request_id_config_device_type @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_device_type when CONNECTOR_DEVICE_TYPE is defined.
 *
 * @see @ref device_type
 * @see @ref app_get_device_type
 */
#define CONNECTOR_DEVICE_TYPE                             "Cloud Connector Linux Sample"

/**
 * When defined, this string hardcode for the @ref device_cloud_url (typically, login.etherios.com or login.etherios.co.uk) instead of the application framework
 * function @ref app_get_device_cloud_url (called via the @ref connector_request_id_config_device_cloud_url @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_device_cloud_url when CONNECTOR_CLOUD_URL is defined.
 *
 * @see @ref device_cloud_url
 * @see @ref app_get_device_cloud_url()
 */
#define CONNECTOR_CLOUD_URL                               "login.etherios.com"

/**
 * When defined, this string hardcode for the @ref tx_keepalive instead of the application framework
 * function @ref  app_get_tx_keepalive_interval() (called via the @ref connector_request_id_config_tx_keepalive @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref  app_get_tx_keepalive_interval() when CONNECTOR_TX_KEEPALIVE_IN_SECONDS is defined.
 *
 * @see @ref tx_keepalive
 * @see @ref  app_get_tx_keepalive_interval()
 */
#define CONNECTOR_TX_KEEPALIVE_IN_SECONDS                 90

/**
 * When defined, this string hardcode for the @ref rx_keepalive instead of the application framework
 * function @ref app_get_rx_keepalive_interval() (called via the @ref connector_request_id_config_rx_keepalive @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_rx_keepalive_interval() when CONNECTOR_RX_KEEPALIVE_IN_SECONDS is defined.
 *
 * @see @ref rx_keepalive
 * @see @ref app_get_rx_keepalive_interval()
 */
#define CONNECTOR_RX_KEEPALIVE_IN_SECONDS                 60

/**
 * When defined, this string hardcode for the @ref wait_count instead of the application framework
 * function @ref app_get_wait_count() (called via the @ref connector_request_id_config_wait_count @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_wait_count() when CONNECTOR_WAIT_COUNT is defined.
 *
 * @see @ref wait_count
 * @see @ref app_get_wait_count()
 */
#define CONNECTOR_WAIT_COUNT                              5

/**
 * When defined, this 4 byte value is hardcode for the @ref vendor_id instead of the application framework
 * function @ref app_get_vendor_id() (called via the @ref connector_request_id_config_vendor_id @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_vendor_id() when CONNECTOR_VENDOR_ID is defined.
 *
 * @see @ref vendor_id
 * @see @ref app_get_vendor_id()
 * @see @ref connector_vendor_id "Obtaining an Device Cloud Vendor ID"
 */
#define CONNECTOR_VENDOR_ID                            0x00000000

/**
 * When defined, this string hardcode for the @ref max_msg_transactions instead of the application framework
 * function @ref app_get_max_message_transactions() (called via the @ref connector_request_id_config_max_transaction @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_max_message_transactions() when CONNECTOR_MSG_MAX_TRANSACTION is defined.
 *
 * @see @ref max_msg_transactions
 * @see @ref app_get_max_message_transactions()
 */
#define CONNECTOR_MSG_MAX_TRANSACTION                  1

/**
 * When defined, this string hardcode for the @ref connection_type instead of the application framework
 * function @ref app_get_connection_type() (called via the @ref connector_request_id_config_connection_type @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_connection_type() when CONNECTOR_CONNECTION_TYPE is defined.  The
 * values for this define are limited to @ref connector_connection_type_lan or @ref connector_connection_type_wan.
 *
 * @see @ref connection_type
 * @see @ref app_get_connection_type()
 * @see @ref connector_connection_type_t
 */
#define CONNECTOR_CONNECTION_TYPE                      connector_connection_type_lan

/**
 * When defined, this string hardcode for the @ref link_speed instead of the application framework
 * function @ref app_get_link_speed() (called via the @ref connector_request_id_config_link_speed @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_link_speed() when CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND is defined.
 *
 * @see @ref link_speed
 * @see @ref app_get_link_speed()
 */
#define CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND    0

/**
 * When defined, this string hardcode for the @ref phone_number instead of the application framework
 * function @ref app_get_phone_number() (called via the @ref connector_request_id_config_phone_number @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_phone_number() when CONNECTOR_WAN_PHONE_NUMBER_DIALED is defined.
 *
 * @see @ref phone_number
 * @see @ref app_get_phone_number()
 */
#define CONNECTOR_WAN_PHONE_NUMBER_DIALED              "0123456789"

/**
 * When defined, this string hardcode for the @ref firmware_support instead of the application framework
 * function @ref app_get_firmware_support() (called via the @ref connector_request_id_config_firmware_facility @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_firmware_support() when CONNECTOR_FIRMWARE_SUPPORT is defined.
 * @note This define is only used when @ref CONNECTOR_FIRMWARE_SERVICE is defined in @ref connector_config.h.
 *
 * @see @ref firmware_support
 * @see @ref app_get_firmware_support()
 * @see @ref CONNECTOR_FIRMWARE_SERVICE
 */
#define CONNECTOR_FIRMWARE_SUPPORT

/**
 * When defined, this string hardcode for the @ref data_service_support instead of the application framework
 * function @ref app_get_data_service_support() (called via the @ref connector_request_id_config_data_service @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_data_service_support() when CONNECTOR_DATA_SERVICE_SUPPORT is defined.
 * @note This define is only used when @ref CONNECTOR_DATA_SERVICE is defined in @ref connector_config.h.
 *
 * @see @ref data_service_support
 * @see @ref app_get_data_service_support()
 * @see @ref CONNECTOR_DATA_SERVICE
 */
#define CONNECTOR_DATA_SERVICE_SUPPORT

/**
 * When defined, this string hardcode for the @ref file_system_support instead of the application framework
 * function @ref app_get_file_system_support() (called via the @ref connector_request_id_config_file_system @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_file_system_support() when CONNECTOR_FILE_SYSTEM_SUPPORT is defined.
 * @note This define is only used when @ref CONNECTOR_FILE_SYSTEM is defined in @ref connector_config.h.
 *
 * @see @ref file_system_support
 * @see @ref app_get_file_system_support()
 * @see @ref CONNECTOR_FILE_SYSTEM
 */
#define CONNECTOR_FILE_SYSTEM_SUPPORT


/**
 * When defined, this string hardcode for the remote_configuration_support instead of the application framework
 * function @ref app_get_remote_configuration_support() (called via the @ref connector_request_id_config_remote_configuration @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_remote_configuration_support() when CONNECTOR_REMOTE_CONFIGURATION_SUPPORT is defined.
 * @note This define is only used when @ref CONNECTOR_RCI_SERVICE is defined a non-zero in @ref connector_config.h.
 *
 * @see @ref rci_support
 * @see @ref app_get_remote_configuration_support()
 * @see @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH
 */
#define CONNECTOR_REMOTE_CONFIGURATION_SUPPORT

/**
 * When defined, this string hardcode for the @ref network_tcp_start instead of the application framework
 * function @ref app_start_network_tcp() (called via the @ref connector_request_id_config_network_tcp @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_start_network_tcp() when CONNECTOR_NETWORK_TCP_START is defined. The
 * values for this define are limited to @ref connector_connect_auto or @ref connector_connect_manual.
 * @note This define is only used when @ref CONNECTOR_TRANSPORT_TCP is defined in @ref connector_config.h.
 *
 * @see @ref network_tcp_start
 * @see @ref connector_connect_auto_type_t
 * @see @ref app_start_network_tcp()
 * @see @ref CONNECTOR_TRANSPORT_TCP
 */
#define CONNECTOR_NETWORK_TCP_START                 connector_connect_auto

/**
 * When defined, this string hardcode for the @ref network_udp_start instead of the application framework
 * function @ref app_start_network_udp() (called via the @ref connector_request_id_config_network_udp @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_start_network_udp() when CONNECTOR_NETWORK_UDP_START is defined. The
 * values for this define are limited to @ref connector_connect_auto or @ref connector_connect_manual.
 * @note This define is only used when @ref CONNECTOR_TRANSPORT_UDP is defined in @ref connector_config.h.
 *
 * @see @ref network_udp_start
 * @see @ref connector_connect_auto_type_t
 * @see @ref app_start_network_udp()
 * @see @ref CONNECTOR_TRANSPORT_UDP
 */
#define CONNECTOR_NETWORK_UDP_START                 connector_connect_auto

/**
 * When defined, this string hardcode for the @ref network_sms_start instead of the application framework
 * function @ref app_start_network_sms() (called via the @ref connector_request_id_config_network_sms @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_start_network_sms() when CONNECTOR_NETWORK_SMS_START is defined. The
 * values for this define are limited to @ref connector_connect_auto or @ref connector_connect_manual.
 * @note This define is only used when @ref CONNECTOR_TRANSPORT_SMS is defined in @ref connector_config.h.
 *
 * @see @ref network_sms_start
 * @see @ref connector_connect_auto_type_t
 * @see @ref app_start_network_sms()
 * @see @ref CONNECTOR_TRANSPORT_SMS
 */
#define CONNECTOR_NETWORK_SMS_START                 connector_connect_auto

/**
 * When defined, this string hardcode for the @ref identity_verification instead of the application framework
 * function @ref app_get_identity_verification() (called via the @ref connector_request_id_config_identity_verification @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_identity_verification() when CONNECTOR_IDENTITY_VERIFICATION is defined.  The
 * values for this define are limited to @ref connector_identity_verification_simple, or @ref connector_identity_verification_password.
 *
 * @see @ref identity_verification
 * @see @ref app_get_identity_verification()
 * @see @ref connector_identity_verification_t
 */
#define CONNECTOR_IDENTITY_VERIFICATION                 connector_identity_verification_simple

/**
 * When defined, this string hardcode for the @ref get_device_cloud_phone @ref set_device_cloud_phone instead of the application framework
 * function @ref app_get_device_cloud_phone (called via the @ref connector_request_id_config_get_device_cloud_phone @ref connector_callback_t "callback" in config.c)
 * and function @ref app_set_device_cloud_phone (called via the @ref connector_request_id_config_set_device_cloud_phone @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_device_cloud_phone and app_set_device_cloud_phone when CONNECTOR_CLOUD_PHONE is defined.
 *
 * @see @ref get_device_cloud_phone and @ref set_device_cloud_phone
 * @see @ref app_get_device_cloud_phone() and app_set_device_cloud_phone()
 */
#define CONNECTOR_CLOUD_PHONE                               "447786201216"

/**
 * When defined, this string hardcode for the @ref device_cloud_service_id instead of the application framework
 * function @ref app_get_device_cloud_service_id (called via the @ref connector_request_id_config_device_cloud_service_id @ref connector_callback_t "callback" in config.c).
 *
 * @note There is no need to implement or port @ref app_get_device_cloud_service_id when CONNECTOR_CLOUD_SERVICE_ID is defined.
 *
 * @see @ref device_cloud_service_id
 * @see @ref app_get_device_cloud_service_id()
 */
#define CONNECTOR_CLOUD_SERVICE_ID                          "" /* empty: No shared-code used */

/**
 * This macro is for memory and TCP transport optimizations only. When defined, this value is used internally to allocate incoming buffers for EDP processing. If it is not
 * defined, the default value of 1460 is used.
 * @note This value is used for buffers declaration so it has direct impact on RAM usage. If you do not know which value to use, leave it undefined.
 * @see @ref MSG_RECV_WINDOW_SIZE
 */
#define MSG_MAX_RECV_PACKET_SIZE         1460

/**
 * This macro is for memory and TCP transport optimizations only. When defined, this value is used internally to allocate outgoing buffers for EDP processing. If it is not
 * defined, the default value of 1460 is used. 
 * @note This value is used for buffers declaration so it has direct impact on RAM usage. If you do not know which value to use, leave it undefined.
 */
#define MSG_MAX_SEND_PACKET_SIZE         1460

/**
 * This macro is for TCP transport optimization only. When defined, this value is used for the incoming window size in EDP's Messaging facility (used by Data Service, Data Points,
 * RCI and File System). This flow control mechanism is used regardless the TCP layer in order to be able to handle more than one concurrent request. This value tells Device Cloud
 * how many bytes Cloud Connector can receive without sending back an ACK message.
 * If not set the default value of 4 times MSG_MAX_RECV_PACKET_SIZE is used.
 * 
 * @see @ref MSG_MAX_RECV_PACKET_SIZE
 */
#define MSG_RECV_WINDOW_SIZE         (4 * MSG_MAX_RECV_PACKET_SIZE)

/**
* @}
*/

#endif

#endif
