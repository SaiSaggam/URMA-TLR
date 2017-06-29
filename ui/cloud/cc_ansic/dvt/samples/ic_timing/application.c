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
#include <time.h>
#include <errno.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"
#include "connector_config.h"

connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    connector_bool_t type;

    UNUSED_ARGUMENT(class_id);

    switch (status)
    {
           /* if either Device Cloud or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
        case connector_close_status_device_stopped:
        case connector_close_status_abort:
             type = connector_false;
             break;

       /* otherwise it's an error and we want to retry */
       default:
             type = connector_true;
             break;
    }

    return type;
}

connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
                                                   connector_request_id_t const request_id,
                                                   void * const data, void * const context)
{
    connector_callback_status_t   status = connector_callback_unrecognized;

    UNUSED_ARGUMENT(context);

    struct timespec start_time;
    struct timespec end_time;

    get_time(&start_time);
    callback_count++;

//    APP_DEBUG("app_connector_callback: class %d id %d\n", class_id, request_id.config_request);

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

    case connector_class_id_firmware:
        status = app_firmware_handler(request_id.firmware_request, data);
        break;

    case connector_class_id_file_system:
        status = app_file_system_handler(request_id.file_system_request, data);
        break;

    case connector_class_id_remote_config:
        status = app_remote_config_handler(request_id.remote_config_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    default:
        status = connector_callback_unrecognized;
        /* not supported */
        break;
    }

    get_time(&end_time);

    total_elapsed_time(&callback_time, start_time, end_time);

    return status;
}

int application_step(connector_handle_t handle)
{
#if defined CONNECTOR_DATA_SERVICE
    send_put_request(handle);
#else
    UNUSED_ARGUMENT(handle);
#endif
    return 0;
}

