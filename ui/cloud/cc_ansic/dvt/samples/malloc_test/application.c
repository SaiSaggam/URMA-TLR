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
#include "connector_config.h"

connector_status_t app_terminate_tcp_transport(connector_handle_t handle)
{

    return connector_initiate_action(handle, connector_initiate_terminate, NULL);

}

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

#if (defined CONNECTOR_DATA_SERVICE)
    case connector_class_id_data_service:
        status = app_data_service_handler(request_id.data_service_request, data);
        break;
#endif

#if (defined CONNECTOR_FIRMWARE_SERVICE)
    case connector_class_id_firmware:
        status = app_firmware_handler(request_id.firmware_request, data);
        break;
#endif

#if (defined CONNECTOR_FILE_SYSTEM)
    case connector_class_id_file_system:
        status = app_file_system_handler(request_id.file_system_request, data);
        break;
#endif

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    default:
        /* not supported */
        break;
    }
    clear_stack_size();

    return status;
}

int application_run(connector_handle_t handle)
{
#if defined CONNECTOR_DATA_SERVICE && DS_MAX_USER > 0
    int index = 0;
    int stop_calling = 0;

    while (!stop_calling)
    {
        connector_status_t const status = send_put_request(handle, index);

        switch (status)
        {
        case connector_init_error:
            if (connector_run_thread_status == connector_device_terminated)
            {
                stop_calling = 1;
                break;
            }
        case connector_service_busy:
           #define SLEEP_ONE_SECOND  (1 * 1000000)
            usleep(SLEEP_ONE_SECOND);
            break;

        case connector_success:
            index++;
            break;

        case connector_invalid_data_range:
            index = 0;

            #define SLEEP_BETWEEN_TESTS   (5 * 1000000)
            usleep(SLEEP_BETWEEN_TESTS);
            break;

        case connector_device_terminated:
        default:
            stop_calling = 1;
            break;
        }
    }
#else
    UNUSED_ARGUMENT(handle);
#endif
    return 0;
}

