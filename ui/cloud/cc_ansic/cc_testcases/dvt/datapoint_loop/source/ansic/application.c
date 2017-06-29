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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "connector_api.h"
#include "platform.h"

extern connector_callback_status_t app_data_point_handler(connector_request_id_data_point_t const request, void  * const data);
extern connector_status_t app_start_test_case_datapoints_loop(connector_handle_t handlerForConnector, size_t const numberOfLoops, size_t const numberStreams, size_t const numberPointsPerStream);



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

static connector_callback_status_t app_tcp_status(connector_tcp_status_t const * const status)
{
    switch (*status)
    {
    case connector_tcp_communication_started:
        APP_DEBUG("connector_tcp_communication_started\n");
        break;
    case connector_tcp_keepalive_missed:
        APP_DEBUG("connector_tcp_keepalive_missed\n");
        break;
    case connector_tcp_keepalive_restored:
        APP_DEBUG("connector_tcp_keepalive_restored\n");
        break;
    }

    return connector_callback_continue;
}




connector_callback_status_t app_status_handler(connector_request_id_status_t const request, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request)
    {
        case connector_request_id_status_tcp:
            status = app_tcp_status(data);
            break;

        case connector_request_id_status_stop_completed:
            APP_DEBUG("connector_restore_keepalive\n");
            break;

        default:
            APP_DEBUG("Status request not supported in sm_udp: %d\n", request);
            break;
    }

    return status;
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

#if (defined CONNECTOR_TRANSPORT_TCP)
    case connector_class_id_network_tcp:
        status = app_network_tcp_handler(request_id.network_request, data);
        break;
#endif

    /* Define callback to manage the datapoints calls */
    case connector_class_id_data_point:
        status = app_data_point_handler(request_id.data_point_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    default:
        status = connector_callback_unrecognized;
        /* not supported */
        break;
    }

    return status;
}




int application_run(connector_handle_t handlerForConnector)
{
    /* Generate entropy for random values, only one call per run */
    srand(((unsigned int)time(NULL)));

    sleep(20);

    APP_DEBUG("****************************** INIT application.c\n");

    size_t const numberOfLoops = 25;
    size_t const numberPointsPerStream = 200;
    size_t const numberStreams = 1;

    /* Launch the test case to send DataPoints */
    connector_status_t status = app_start_test_case_datapoints_loop(handlerForConnector, numberOfLoops, numberStreams, numberPointsPerStream);

    switch (status)
    {
        case connector_init_error:
        case connector_service_busy:
        case connector_unavailable:
        {
            APP_DEBUG("Data point sample is exited! ERROR\n");
            return 1;
        }
        case connector_success:
        {
            APP_DEBUG("Data point sample is exited! DONE\n");
            return 0;
        }
        default:
        {
            APP_DEBUG("Data point sample is exited! UNKNOWN\n");
            return 1;
        }
    }


}
