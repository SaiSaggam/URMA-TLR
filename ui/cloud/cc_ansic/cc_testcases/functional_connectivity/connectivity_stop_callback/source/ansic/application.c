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

/******** Modified for testing ********/
#include "application.h"
/* Global flag to force the reconnection */
connector_bool_t reconnect_transport = connector_false;
/* Global flag to exit the application thread */
connector_bool_t terminate_app_thread = connector_false;
/******** End of Modifications for testing ********/

extern connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id, void * const data);



connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    connector_bool_t type;

    UNUSED_ARGUMENT(class_id);

    switch (status)
    {
        /******** Modified for testing ********/
        /* if either Device Cloud or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
            /* Set the global flag to finish the application thread */
            terminate_app_thread = connector_true;
            type = connector_false;
            break;

        case connector_close_status_device_stopped:
            /* Set the global flag to reconnect automatically */
            reconnect_transport = connector_true;
            type = connector_true;
            break;

        case connector_close_status_abort:
            type = connector_false;
            break;

        /* otherwise it's an error and we want to retry */
        default:
            type = connector_true;
            break;

        /******** End of Modifications for testing ********/
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


/******** Modified for testing ********/
connector_handle_t connector_handle;
/******** End of Modifications for testing ********/

int application_run(connector_handle_t handle)
{

    /******** Modified for testing ********/
    connector_handle = handle;

    for (;;)
    {

        if (reconnect_transport)
        {
            APP_DEBUG("application_run: relaunch the tcp transport for the next test in 5 seconds...\n");
            sleep(5);

            connector_transport_t const transport = connector_transport_tcp;
            connector_status_t action_status;

            action_status = connector_initiate_action(handle, connector_initiate_transport_start, &transport);

            if (action_status == connector_success)
            {
                reconnect_transport = connector_false;
            }
        }

        if (terminate_app_thread)
        {
            APP_DEBUG("application_run: application thread exits\n");
            break;
        }

        /* This sleep is needed to run the sample because without it does not work */
        sleep(1);

    }

    /******** End of Modifications for testing ********/

   return 0;
}
