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


extern connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request,
                                                  void * const data);

extern connector_status_t send_put_request(connector_handle_t handle, int index);

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

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    default:
        /* not supported */
        break;
    }

    return status;
}

connector_handle_t connector_handle = NULL;

int application_run(connector_handle_t handle)
{
    connector_status_t status;
    int index = 0;

    connector_handle = handle;

    for (;;)
    {
        status = send_put_request(handle, index);

        switch (status)
        {
        case connector_init_error:
            index = 0;
            /* no break; */
        case connector_service_busy:
            sleep(1);
            break;

        case connector_success:
            index++;
            break;

        case connector_device_terminated:
            goto done;

        default:
            APP_DEBUG("application_run: send_put_request returns %d\n", status);
            sleep(1);
            break;
        }
    }

done:
    return 0;
}


