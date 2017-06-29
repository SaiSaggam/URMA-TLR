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
    no_delay_receive,
    start_delay_receive,
    delay_receive_started,
    delay_receive_done
} delay_receive_state_t;

typedef enum {
    no_reboot_received,
    reboot_received,
    receive_reset,
    receive_error,
    send_error,
    reboot_timeout
} reboot_state_t;

typedef enum {
    config_test_invalid_data,
    config_test_invalid_size,
    config_test_none
} config_test_t;


typedef enum
{
    connector_connector_stop,
    connector_connector_start
} connector_connector_status_t;

extern connector_callback_status_t app_config_error(connector_config_error_status_t const * const error_data);
extern connector_callback_status_t app_invalid_size_config_handler(connector_request_id_config_t const request_id,
                                              void * const data);
extern connector_callback_status_t app_invalid_data_config_handler(connector_request_id_config_t const request_id,
                                              void * const data);

extern void write_python_result_file(char *file_buffer);

/* extern delay_receive_state_t delay_receive_state;
extern reboot_state_t reboot_state;
extern int reboot_errno; */

extern connector_device_id_method_t device_id_method;
extern connector_connector_status_t connector_connector_status;
extern connector_connection_type_t  device_connection_type;
extern connector_wan_type_t connector_wan_type;
extern connector_identity_verification_t connector_identity_verification;
extern connector_connect_auto_type_t connector_connect_tcp;
extern config_test_t config_test[];

extern char python_file_buffer[256];

/* extern unsigned int put_file_active_count; */

extern connector_handle_t connector_handle;

#endif /* APPLICATION_H_ */
