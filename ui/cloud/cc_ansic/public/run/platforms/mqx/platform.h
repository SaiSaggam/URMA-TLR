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

#define APP_DEBUG   log_printf

#define UNUSED_ARGUMENT(x)     ((void)x)
#define IPCFG_default_enet_device       0

/* Callbacks for this platform */
connector_callback_status_t app_os_handler(connector_request_id_os_t const request, void * const data);
connector_callback_status_t app_network_tcp_handler(connector_request_id_network_t const request_id, void * const data);
connector_callback_status_t app_config_handler(connector_request_id_config_t const request_id, void * const data);
#if defined (CONNECTOR_RCI_SERVICE)
connector_callback_status_t app_remote_config_handler(connector_request_id_remote_config_t const request_id, void * const data);
#endif
int application_run(connector_handle_t handle);
extern connector_callback_status_t app_get_server_url(connector_config_pointer_string_t * const config_url);
extern connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor_id);
extern connector_callback_status_t app_get_device_type(connector_config_pointer_string_t * const config_device_type);
extern connector_callback_status_t app_get_device_id_method(connector_config_device_id_method_t * const config_device);
extern connector_callback_status_t app_custom_get_mac_addr(unsigned char * config_mac);
extern connector_callback_status_t app_get_mac_addr(connector_config_pointer_data_t * const config_mac);
extern connector_callback_status_t app_os_get_system_time(unsigned long * const uptime);
extern unsigned long ic_create_task(unsigned long const index_number, unsigned long const parameter);
extern unsigned long ic_destroy_task(unsigned long const task_id);
void log_printf(const char * const msg, ...);

#if defined CONNECTOR_FIRMWARE_SERVICE
extern connector_callback_status_t app_firmware_handler(connector_request_id_firmware_t const request_id, void * const data);
#endif
#if defined CONNECTOR_FILE_SYSTEM
extern connector_callback_status_t app_file_system_handler(connector_request_id_file_system_t const request, void * const data);
#endif

#endif
