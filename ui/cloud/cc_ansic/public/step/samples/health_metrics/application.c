/*
 * Copyright (c) 2014 Digi International Inc.,
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

#include "health_metrics_api.h"

extern connector_callback_status_t app_remote_config_handler(connector_request_id_remote_config_t const request_id, void * const data);

extern health_metrics_config_t health_metrics_config;

volatile connector_bool_t tcp_transport_started = connector_false;

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

connector_callback_status_t send_data_service_handle_callback(connector_request_id_data_service_t const ds_request_id, void * const data)
{
    connector_callback_status_t status;

    switch (ds_request_id)
    {
        case connector_request_id_data_service_send_data:
            status = dev_health_handle_data_callback(data);
            break;

        case connector_request_id_data_service_send_response:
            status = dev_health_handle_response_callback(data);
            break;

        case connector_request_id_data_service_send_status:
            status = dev_health_handle_status_callback(data);
            break;

        #if (defined CONNECTOR_SHORT_MESSAGE)
        case connector_request_id_data_service_send_length:
            status = dev_health_handle_length_callback(data);
            break;
        #endif

        default:
            status = connector_callback_unrecognized;
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

    case connector_class_id_network_tcp:
        status = app_network_tcp_handler(request_id.network_request, data);
        break;

    case connector_class_id_data_service:
        status = send_data_service_handle_callback(request_id.data_service_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    case connector_class_id_remote_config:
        status = app_remote_config_handler(request_id.remote_config_request, data);
        break;

    default:
        /* not supported */
        break;
    }
    return status;
}

int application_step(connector_handle_t handle)
{
    static health_metrics_data_t health_metrics_data;
    static connector_bool_t data_init = connector_false;
    int const return_status = 0;

    if (!data_init)
    {
        health_metrics_config.eth.metrics = connector_true;
        health_metrics_config.eth.sample_rate = 1;
        health_metrics_config.mobile.metrics = connector_true;
        health_metrics_config.mobile.sample_rate = 1;
        health_metrics_config.sys.metrics = connector_true;
        health_metrics_config.sys.sample_rate = 1;
        health_metrics_config.report_rate = 1;
        
        data_init = connector_true;
    }

    if (!tcp_transport_started)
    {
        goto done;
    }

    health_metrics_report_step(&health_metrics_config, &health_metrics_data, handle);
   
done:
    return return_status;
}

