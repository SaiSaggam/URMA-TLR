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

#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <stdio.h>
#include <bsp_api.h>
#include <tx_api.h>


#define APP_DEBUG  printf

#define UNUSED_ARGUMENT(x)     ((void)x)

#ifndef in_addr_t
typedef uint32_t in_addr_t;
#endif

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

#define MY_IFNAME   "eth0"

/*#define MY_DEVICE_VENDOR_ID {0x01, 0x00, 0x00, 0x23}*/
/*#define MY_SERVER_URL       "test.idigi.com"*/
#define MY_DEVICE_VENDOR_ID {0x01, 0x00, 0x01, 0x37}
#define MY_SERVER_URL       "my.idigi.com"
#define MY_IMEI_NUMBER      "000000-00-000000-0"

extern connector_callback_status_t app_connector_callback(connector_class_id_t const class_id, connector_request_id_t const request_id,
                                    void const * const request_data, size_t const request_length,
                                    void * response_data, size_t * const response_length);


/* Callbacks for this platform */
extern connector_callback_status_t app_os_handler(connector_os_request_t const request,
                           void const * const request_data, size_t const request_length,
                           void * response_data, size_t * const response_length);

extern connector_callback_status_t app_network_handler(connector_network_request_t const request,
                         void const * const request_data, size_t const request_length,
                         void * response_data, size_t * const response_length);

extern connector_callback_status_t app_config_handler(connector_config_request_t const request,
                                              void const * const request_data,
                                              size_t const request_length,
                                              void * response_data,
                                              size_t * const response_length);

extern connector_callback_status_t app_file_system_handler(connector_file_system_request_t const request,
                                                void const * const request_data,
                                                size_t const request_length,
                                                void * const response_data,
                                                size_t * const response_length);

extern int application_run(connector_handle_t handle);

extern connector_callback_status_t app_os_get_system_time(unsigned long * const uptime);


#define APP_SSL_CA_CERT   "./public/certificates/idigi-ca-cert-public.crt"


#endif /* _PLATFORM_H */
