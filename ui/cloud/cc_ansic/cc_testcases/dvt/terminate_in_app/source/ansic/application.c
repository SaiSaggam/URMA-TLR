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
#include <unistd.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"
#include <sched.h>

connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id, void * const data);

connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
                                                   connector_request_id_t const request_id,
                                                   void * const data, void * const context)
{
    connector_callback_status_t   status = connector_callback_unrecognized;

    UNUSED_ARGUMENT(context);

    switch (class_id)
    {
    case connector_class_id_config:
        status = app_config_handler(request_id.config_request, data);
        break;

    case connector_class_id_operating_system:
        status = app_os_handler(request_id.os_request, data);
        break;

    case connector_class_id_network_tcp:
        status = app_network_tcp_handler(request_id.network_request, data);
        break;

    case connector_class_id_data_service:
        status = app_data_service_handler(request_id.data_service_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    default:
        /* not supported */
        break;
    }

    return status;
}

extern int tcp_started;
extern size_t total_malloc_size;

extern connector_status_t connector_status;
extern int initiate_connector_stop_from_send_data;

connector_status_t send_dummy_test_file(connector_handle_t handle);

int application_run(connector_handle_t handle)
{
    connector_status_t status;

    while (!tcp_started)
    {
        sched_yield();
    }

    APP_DEBUG("TCP connection started\n");

    sleep(20);
    APP_DEBUG("Sending file 1\n");
    do {
        status = send_dummy_test_file(handle);
    } while (status != connector_success);

    sleep(1);
    APP_DEBUG("Sending file 2\n");
    do {
        status = send_dummy_test_file(handle);
    } while (status != connector_success);

    APP_DEBUG("Sending file 2\n");
    do {
        status = send_dummy_test_file(handle);
    } while (status != connector_success);


    APP_DEBUG("Calling 'connector_initiate_terminate'\n");

    connector_initiate_action(handle, connector_initiate_terminate, NULL);

    while (connector_status != connector_device_terminated)
    {
        sched_yield();
    }

    APP_DEBUG("CC closed\n");
    if (total_malloc_size == 0)
    {
        APP_DEBUG("SUCCESS: no memory leak detected\n");
    }
    else
    {
        APP_DEBUG("FAILED: Memory leak detected!!\n");
    }

    for(;;);
    return 0;
}


