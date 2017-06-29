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

#include <stdio.h>
#include "connector_debug.h"

#define APP_DEBUG   printf

#define UNUSED_ARGUMENT(x)     ((void)x)

/* GCC emits a warning message for alignment issues when casting. Normally we fix these by reorganizing our structures.
   In some cases these structures are in system header files and cannot be changed. This macro should be used in those situations. */
#define cast_for_alignment(cast, ptr)   ((cast) ((void *) (ptr)))

extern int app_get_edp_conn_ip_address(connector_config_ip_address_t * const config_ip);

extern connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
                                                          connector_request_id_t const request_id,
                                                          void * const data);

/* Callbacks for this platform */
extern connector_callback_status_t app_os_handler(connector_request_id_os_t const request,
                                                  void * const data);

extern connector_callback_status_t app_network_tcp_handler(connector_request_id_network_t const request,
                                                           void * const data);

extern connector_callback_status_t app_network_udp_handler(connector_request_id_network_t const request,
                                                           void * const data);

extern connector_callback_status_t app_network_sms_handler(connector_request_id_network_t const request,
                                                           void * const data);

extern connector_callback_status_t app_config_handler(connector_request_id_config_t const request,
                                                      void * const data);

#if (defined CONNECTOR_FILE_SYSTEM)
extern connector_callback_status_t app_file_system_handler(connector_request_id_file_system_t const request,
                                                           void * const data);
#endif

extern int application_step(connector_handle_t handle);

extern connector_callback_status_t app_os_get_system_time(unsigned long * const uptime);

extern connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status);
extern connector_callback_status_t app_status_handler(connector_request_id_status_t const request,
                                           void * const data);

connector_callback_status_t app_os_yield(connector_status_t const * const status);

#if (defined RUNNIG_FROM_BASE)
#define APP_SSL_CA_CERT   "./public/certificates/Digi_Int-ca-cert-public.crt"
#else
#define APP_SSL_CA_CERT   "../../../../public/certificates/Digi_Int-ca-cert-public.crt"
#endif


#endif
