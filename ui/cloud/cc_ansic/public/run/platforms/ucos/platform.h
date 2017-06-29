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
#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "connector_api.h"

#if (defined CONNECTOR_DEBUG)
#define APP_DEBUG   printf
#else
#define APP_DEBUG
#endif

#define UNUSED_ARGUMENT(x)     ((void)x)

/* Callbacks for this platform */
connector_callback_status_t app_os_handler(connector_request_id_os_t const request, void * const data);
connector_callback_status_t app_network_tcp_handler(connector_request_id_network_t const request_id, void * const data);
connector_callback_status_t app_network_udp_handler(connector_request_id_network_t const request_id, void * const data);
connector_callback_status_t app_network_sms_handler(connector_request_id_network_t const request_id, void * const data);
connector_callback_status_t app_config_handler(connector_request_id_config_t const request_id, void * const data);
#if defined (CONNECTOR_RCI_SERVICE)
connector_callback_status_t app_remote_config_handler(connector_request_id_remote_config_t const request_id, void * const data);
#endif
#if defined (CONNECTOR_FILE_SYSTEM)
connector_callback_status_t app_file_system_handler(connector_request_id_file_system_t const request, void * const data);
#endif
#if defined (CONNECTOR_FIRMWARE_SERVICE)
connector_callback_status_t app_firmware_handler(connector_request_id_firmware_t const request_id, void * const data);
#endif
int application_run(connector_handle_t handle);

#endif
