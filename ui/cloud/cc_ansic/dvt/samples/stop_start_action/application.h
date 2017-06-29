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

#if (defined CONNECTOR_TRANSPORT_UDP) || (defined CONNECTOR_TRANSPORT_SMS)
#define APP_USE_SM
#endif

#if (defined CONNECTOR_TRANSPORT_UDP)
#define UDP_T   1
#else
#define UDP_T   0
#endif

#if (defined CONNECTOR_TRANSPORT_SMS)
#define SMS_T   1
#else
#define SMS_T   0
#endif

#if (defined CONNECTOR_TRANSPORT_TCP)
#define TCP_T   1
#else
#define TCP_T   0
#endif

#define TRANSPORT_COUNT TCP_T + UDP_T + SMS_T

#define TRANSPORT_ERROR_FILE "transport_error.txt"
#define TERMINATE_TEST_FILE "terminate_test.txt"

typedef enum {
    device_request_not_support,
    device_request_invalid_data,
    device_request_stop_connector,
    device_request_start_connector,
    device_request_stop_terminate_connector,
    device_request_app_stop_connector,
    device_request_app_start_connector,
    device_request_stop_all_transports,
    device_request_abort_device_request,
    device_request_abort_stop_callback,
    device_request_terminate_connector
} device_request_target_t;


typedef enum {
    device_request_idle,
    device_request_terminate,
    device_request_terminate_start,
    device_request_terminate_in_application,
    device_request_terminate_in_application_start,
    device_request_terminate_done
} terminate_flag_t;

#define DS_MAX_USER   5
#define DS_DATA_SIZE  (1024 * 16)

#define DS_FILE_NAME_LEN  20


typedef struct
{
    connector_request_id_data_service_t request;
    connector_request_data_service_send_t header;
    char file_path[DS_FILE_NAME_LEN];
    size_t bytes_sent;
    size_t file_length_in_bytes;
    char * file_data;
    int index;
    unsigned long initiate_time;
    unsigned long first_data_time;
    unsigned long last_data_time;
} ds_record_t;

typedef struct {
    device_request_target_t target;
    connector_transport_t transport;
} initiate_action_t;

extern initiate_action_t initiate_action;

extern connector_handle_t connector_handle;
extern terminate_flag_t terminate_flag;
extern unsigned int put_file_active_count[];
extern connector_status_t connector_run_thread_status;
extern char terminate_file_content[];
extern connector_callback_status_t stop_callback_status;
extern connector_close_status_t connector_close_status;
extern char connector_close_status_text[];
extern unsigned stop_transport_count[];

extern void initialize_send_data(void);

extern void malloc_sem_init(void);
extern connector_callback_status_t app_os_malloc(size_t const size, void ** ptr);
extern void app_os_free(void * const ptr);

extern int application_udp(connector_handle_t handle);
extern int application_ping(connector_handle_t handle);
extern int initiate_run(connector_handle_t handle);

extern connector_status_t send_file(connector_handle_t handle, int index, char * const filename, char * const content, size_t content_length, connector_transport_t const transport);
extern connector_status_t send_put_request(connector_handle_t handle, int index, connector_transport_t const transport);

extern char const * transport_to_string(connector_transport_t const value);

extern device_request_target_t device_request_target;

#endif /* APPLICATION_H_ */
