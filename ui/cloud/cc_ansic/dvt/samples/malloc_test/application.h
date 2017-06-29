
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

#define DS_MAX_USER 1

#define TERMINATE_TEST_FILE "terminate_test.txt"
#define HEAP_USAGE_FILE     "./dvt/heap_usage.txt"

extern connector_handle_t connector_handle;
extern unsigned int put_file_active_count;
extern int firmware_download_started;
extern size_t total_malloc_size;
extern size_t total_max_malloc_size;
extern connector_status_t connector_run_thread_status;

extern void clear_stack_size(void);
void write_usage_file(char const * text);

#if (defined CONNECTOR_DATA_SERVICE)
extern connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id,
                                                  void * const data);
extern connector_status_t send_put_request(connector_handle_t handle, int index);
#endif

#if (defined CONNECTOR_FIRMWARE_SERVICE)
extern connector_callback_status_t app_firmware_handler(connector_request_id_firmware_t const request_id,
                                                  void * const data);
#endif

extern connector_status_t app_terminate_tcp_transport(connector_handle_t handle);

#endif /* APPLICATION_H_ */
