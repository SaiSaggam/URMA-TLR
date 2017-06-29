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

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdbool.h>

typedef enum {
    device_request_not_support,
    device_request_wan_type_target
} device_request_target_t;

extern connector_handle_t connector_handle;
extern unsigned int put_file_active_count;
extern connector_status_t connector_run_thread_status;
extern connector_wan_type_t wan_type;
extern char * wan_id_string;

extern connector_status_t app_terminate_tcp_transport(connector_handle_t handle);
extern int initiate_run(connector_handle_t handle);
extern connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id,
                                                  void * const data);

extern device_request_target_t device_request_target;

#endif /* APPLICATION_H_ */
