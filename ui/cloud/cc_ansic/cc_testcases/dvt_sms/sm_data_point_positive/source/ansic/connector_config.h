/*
 * Copyright (c) 2011, 2012, 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#ifndef __CONNECTOR_CONFIG_H_
#define __CONNECTOR_CONFIG_H_

#define CONNECTOR_LITTLE_ENDIAN
#define CONNECTOR_DEBUG
/* #define CONNECTOR_FIRMWARE_SERVICE */
/* #define CONNECTOR_COMPRESSION */
#define CONNECTOR_DATA_SERVICE
/* #define CONNECTOR_FILE_SYSTEM */
/* #define CONNECTOR_RCI_SERVICE */
#define CONNECTOR_TRANSPORT_TCP
#define CONNECTOR_TRANSPORT_UDP
#define CONNECTOR_TRANSPORT_SMS

#define CONNECTOR_DATA_POINTS
#define CONNECTOR_SUPPORTS_FLOATING_POINT
#define CONNECTOR_SUPPORTS_64_BIT_INTEGERS

#define CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH    256
#define CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH   256

#define CONNECTOR_SM_UDP_MAX_SESSIONS   20
#define CONNECTOR_SM_SMS_MAX_SESSIONS   20
#define CONNECTOR_SM_MULTIPART
#define CONNECTOR_SM_UDP_MAX_RX_SEGMENTS   16
#define CONNECTOR_SM_SMS_MAX_RX_SEGMENTS   16
#define CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS   16
#define CONNECTOR_SM_UDP_RX_TIMEOUT        SM_WAIT_FOREVER  /* In seconds, SM_WAIT_FOREVER: Wait forever for the complete request/response */
#define CONNECTOR_SM_SMS_RX_TIMEOUT        SM_WAIT_FOREVER  /* In seconds, SM_WAIT_FOREVER: Wait forever for the complete request/response */
#define CONNECTOR_SM_UDP_TX_TIMEOUT CONNECTOR_SM_UDP_RX_TIMEOUT /* Use same timeout as for RX */
#define CONNECTOR_SM_SMS_TX_TIMEOUT CONNECTOR_SM_SMS_RX_TIMEOUT /* Use same timeout as for RX */

/* #define CONNECTOR_NO_MALLOC */
#define CONNECTOR_NO_MALLOC_MAX_SEND_SESSIONS 10

#ifdef ENABLE_COMPILE_TIME_DATA_PASSING
#define CONNECTOR_DEVICE_TYPE                          "Linux Cloud Connector Sample"
#define CONNECTOR_CLOUD_URL                            "login.etherios.com"
#define CONNECTOR_TX_KEEPALIVE_IN_SECONDS              5
#define CONNECTOR_RX_KEEPALIVE_IN_SECONDS              75
#define CONNECTOR_WAIT_COUNT                           5
#define CONNECTOR_VENDOR_ID                            0x00000000
#define CONNECTOR_MSG_MAX_TRANSACTION                  1
#define CONNECTOR_CONNECTION_TYPE                      connector_connection_type_lan
#define CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND    0
#define CONNECTOR_WAN_PHONE_NUMBER_DIALED              "012345678"
#define CONNECTOR_FIRMWARE_SUPPORT
#define CONNECTOR_DATA_SERVICE_SUPPORT
#define CONNECTOR_FILE_SYSTEM_SUPPORT
#define CONNECTOR_REMOTE_CONFIGURATION_SUPPORT

#define CONNECTOR_DEVICE_ID_METHOD                      connector_device_id_method_auto
#define CONNECTOR_NETWORK_TCP_START                     connector_connect_auto
#define CONNECTOR_NETWORK_UDP_START                     connector_connect_auto
#define CONNECTOR_NETWORK_SMS_START                     connector_connect_auto
#define CONNECTOR_WAN_TYPE                              connector_wan_type_imei
#define CONNECTOR_IDENTITY_VERIFICATION                 connector_identity_verification_simple

#endif

#endif
