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

#define APP_NUM_STREAMS          3
#define APP_POINTS_PER_STREAM    5

extern connector_callback_status_t app_data_point_handler(connector_request_id_data_point_t const request, void  * const data);
void app_setup_stream(connector_data_stream_t * stream, char * stream_id, char * units, connector_data_point_type_t const type, char * forward_to);
extern void app_update_point(void * const buffer, size_t const stream_index, size_t const point_index);
extern connector_status_t app_send_data(connector_handle_t handle, void * const buffer);
extern connector_request_data_point_t * app_allocate_data(size_t const stream_count, size_t const point_count);
extern void app_free_data(void * buffer, size_t const stream_count);

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

int application_step(connector_handle_t handle)
{
    connector_status_t status = connector_success;
    static connector_request_data_point_t * data_point = NULL;
    static size_t point_index = 0;
    static size_t busy_count = 0;
    static int app_wait = 0;
    int ret = 0;

    if (data_point == NULL)
    {
        /* This function is reentrant. Allocate only once. */
        data_point = app_allocate_data(APP_NUM_STREAMS, APP_POINTS_PER_STREAM);
        ASSERT(data_point != NULL);
        app_setup_stream(&data_point->stream[0], "cpu_usage", "%", connector_data_point_type_integer, NULL);
        app_setup_stream(&data_point->stream[1], "cpu_temperature", "Celsius degree", connector_data_point_type_float, NULL);
        app_setup_stream(&data_point->stream[2], "incremental", "Counts", connector_data_point_type_integer, NULL);
    }

    if (app_wait > 0)
    {
        static time_t last_time = 0;
        time_t current_time = time(NULL);

        if (current_time - last_time < 1)
        {
            goto done;
        }

        last_time = current_time;
        app_wait--;
    }
    else
    {
        if (point_index < APP_POINTS_PER_STREAM)
        {
            size_t stream_index;

            /* Collect a sample for each stream */            
            for(stream_index = 0; stream_index < APP_NUM_STREAMS; stream_index++)
                app_update_point(data_point, stream_index, point_index);
            point_index++;
            app_wait = 2;
        }
        else
        {
            /* Now it is time to send all collected samples to Device Cloud */
            status = app_send_data(handle, data_point);

            switch (status)
            {
                case connector_init_error:
                case connector_service_busy:
                case connector_unavailable:
                {
                    if (++busy_count > APP_POINTS_PER_STREAM)
                    {
                        goto error;
                    }
                    APP_DEBUG("Wait 5 seconds\n");
                    app_wait = 5;
                    break;
                }

                case connector_success:
                    point_index = 0;
                    busy_count = 0;
                    break;

                default:
                    APP_DEBUG("Failed to send data point multiple. status: %d\n", status);                   
                    goto error;
            }
        }
    }

    goto done;

error:
    app_free_data(data_point, APP_NUM_STREAMS);
    ret = -1;

done:
    return ret;
}
