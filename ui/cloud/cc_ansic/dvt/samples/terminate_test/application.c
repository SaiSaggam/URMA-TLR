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

char terminate_file_content[32];

extern connector_status_t connector_run_thread_status;


extern connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request,
                                                  void * const data);
extern connector_callback_status_t app_firmware_handler(connector_request_id_firmware_t const request,
                                                  void * const data);

extern connector_status_t send_put_request(connector_handle_t handle, int index);
extern connector_status_t send_file(connector_handle_t handle, int index, char * const filename,
                                char * const content, size_t content_length);

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

    case connector_class_id_data_service:
        status = app_data_service_handler(request_id.data_service_request, data);
        break;

    case connector_class_id_firmware:
        status = app_firmware_handler(request_id.firmware_request, data);
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

int application_run(connector_handle_t handle)
{
    connector_status_t status;
    int index = 0;

    size_t file_length = file_length = strlen(terminate_file_content);

     usleep(5 * 1000000);

     while (terminate_flag == device_request_terminate_done  && file_length > 0)
     {

         status = send_file(handle, 255, (char * const)TERMINATE_TEST_FILE, (char * const)terminate_file_content, file_length);
         if (status != connector_success)
         {
             APP_DEBUG("application_run: unable to send terminate_test.txt %d\n", status);
             usleep(2 * 1000000);
         }
         else
         {
             /* assume it's done sending */
             APP_DEBUG("application_run: sent terminate_test.txt\n");
             break;
         }
     }

    for (;connector_run_thread_status != connector_device_terminated;)
    {
        status = send_put_request(handle, index);

        switch (status)
        {
        case connector_init_error:
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
            goto done;
        default:
            break;
        }
    }

done:
    return 0;
}


