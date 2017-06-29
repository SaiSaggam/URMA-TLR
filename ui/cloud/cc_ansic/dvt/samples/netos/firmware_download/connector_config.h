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
#ifndef __CONNECTOR_CONFIG_H_
#define __CONNECTOR_CONFIG_H_

/* #define CONNECTOR_LITTLE_ENDIAN */
#define CONNECTOR_DEBUG
#define CONNECTOR_FIRMWARE_SERVICE
/* #define CONNECTOR_COMPRESSION */
/* #define CONNECTOR_DATA_SERVICE */
/* #define CONNECTOR_FILE_SYSTEM */
/* #define CONNECTOR_RCI_SERVICE */
#define CONNECTOR_TRANSPORT_TCP
/* #define CONNECTOR_TRANSPORT_UDP */
/* #define CONNECTOR_TRANSPORT_SMS */

#define CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH    256
#define CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH   256

/* #define CONNECTOR_NO_MALLOC */

#ifdef ENABLE_COMPILE_TIME_DATA_PASSING
#define CONNECTOR_DEVICE_TYPE                          "Etherios Cloud Connector NET+OS Sample"
#define CONNECTOR_CLOUD_URL                            "my.idigi.com"
#define CONNECTOR_TX_KEEPALIVE_IN_SECONDS              5
#define CONNECTOR_RX_KEEPALIVE_IN_SECONDS              75
#define CONNECTOR_WAIT_COUNT                           5
#define CONNECTOR_VENDOR_ID                            0x00000000
#define CONNECTOR_MSG_MAX_TRANSACTION                  1
#define CONNECTOR_CONNECTION_TYPE                      connector_lan_connection_type
#define CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND    0
#define CONNECTOR_WAN_PHONE_NUMBER_DIALED              "012345678"
#define CONNECTOR_FIRMWARE_SUPPORT
#define CONNECTOR_DATA_SERVICE_SUPPORT
#define CONNECTOR_FILE_SYSTEM_SUPPORT
#define CONNECTOR_REMOTE_CONFIGURATION_SUPPORT

#define CONNECTOR_DEVICE_ID_METHOD                      connector_auto_device_id_method
#define CONNECTOR_NETWORK_TCP_START                     connector_auto_connect
#define CONNECTOR_WAN_TYPE                              connector_imei_wan_type
#define CONNECTOR_IDENTITY_VERIFICATION                 connector_simple_identity_verification

#endif

#endif
