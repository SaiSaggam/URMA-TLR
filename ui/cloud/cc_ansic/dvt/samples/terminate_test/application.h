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

#include "connector_api.h"

#define TERMINATE_TEST_FILE "terminate_test.txt"

typedef enum {
    device_request_idle,
    device_request_terminate,
    device_request_terminate_start,
    device_request_terminate_in_application,
    device_request_terminate_in_application_start,
    device_request_terminate_done
} terminate_flag_t;

extern connector_handle_t connector_handle;
extern terminate_flag_t terminate_flag;
extern unsigned int put_file_active_count;
extern int firmware_download_started;

extern connector_status_t app_terminate_tcp_transport(connector_handle_t handle);

#endif /* APPLICATION_H_ */
