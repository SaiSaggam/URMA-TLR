/*
 * Copyright (c) 2011, 2012, 2013 Digi International Inc.,
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

extern connector_callback_status_t app_sm_handler(connector_request_id_sm_t const request, void * const data);

connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    UNUSED_ARGUMENT(class_id);

    connector_bool_t type;

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

#if defined CONNECTOR_TRANSPORT_TCP
    case connector_class_id_network_tcp:
        status = app_network_tcp_handler(request_id.network_request, data);
        break;
#endif

    case connector_class_id_network_sms:
        status = app_network_sms_handler(request_id.network_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    case connector_class_id_short_message:
        status = app_sm_handler(request_id.sm_request, data);
        break;

    default:
        /* not supported */
        break;
    }

    return status;
}

int application_run(connector_handle_t handle)
{
    int return_status = 0;
    connector_status_t status;

    connector_initiate_stop_request_t  request_data = {connector_transport_sms, connector_stop_immediately, NULL};

    /* Let time to the test.py:test_check_proxy to start... if not the test harness deletes the console log */
    sleep(45);

    APP_DEBUG("application_run: Starting SMS transport\n");
    status = connector_initiate_action(handle, connector_initiate_transport_start, &request_data.transport);
    if (status != connector_success)
    {
        APP_DEBUG("application_run: connector_initiate_action for connector_initiate_transport_start returns status %d\n", status);
    }

    return return_status;
}

