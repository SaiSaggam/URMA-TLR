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
#include "connector_api.h"
#include "platform.h"
#include "application.h"

char * status_to_str(connector_status_t arg)
{
    return "";
}

connector_connect_auto_type_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    connector_connect_auto_type_t type;

    UNUSED_ARGUMENT(class_id);

    switch (status)
    {
           /* if either Device Cloud or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
        case connector_close_status_device_stopped:
        case connector_close_status_abort:
             type = connector_connect_manual;
             break;

       /* otherwise it's an error and we want to retry */
       default:
             type = connector_connect_auto;
             break;
    }

    return type;
}

connector_callback_status_t app_connector_callback(connector_class_id_t const class_id, connector_request_id_t const request_id,
                                    void const * const request_data, size_t const request_length,
                                    void * response_data, size_t * const response_length)
{
    connector_callback_status_t   status = connector_callback_unrecognized;

    switch (class_id)
    {
    case connector_class_config:
        status = app_config_handler(request_id.config_request, request_data, request_length, response_data, response_length);
        break;
    case connector_class_operating_system:
        status = app_os_handler(request_id.os_request, request_data, request_length, response_data, response_length);
        break;
    case connector_class_network_tcp:
    case connector_class_id_network_udp:
        status = app_network_handler(class_id, request_id.network_request, request_data, request_length, response_data, response_length);
        break;
    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, request_data, request_length, response_data, response_length);
        break;
    default:
        /* not supported */
        break;
    }
    return status;
}

int application_step(connector_handle_t handle)
{
    int result = -1;
    static unsigned long started;
    unsigned long close_cnt, open_fail_cnt, total_missed_ka;

    connector_os_status_type_t os_status = (connector_os_status_type_t) -1;

    UNUSED_ARGUMENT(handle);

    if (started == 0)
        app_os_get_system_time(&started);

    {
        unsigned long miss_ka_time = app_get_stats(missed_ka_time_id);
        if (miss_ka_time > 0)
        {
            unsigned long now;
            app_os_get_system_time(&now);
            if ((now - miss_ka_time) > 2)
            {
                APP_PRINTF("connector_miss_keepalive\n");
                app_inc_stats(missed_ka_id);
                app_set_stats(missed_ka_time_id, 0);
            }
        }
    }

    close_cnt = app_get_stats(closed_conn_id);
    open_fail_cnt = app_get_stats(open_conn_fail_id);
    total_missed_ka = app_get_stats(missed_ka_id);

     if (close_cnt > 0)
     {
        connector_status_t status = app_get_stats(close_reason_id);
        if (status == connector_keepalive_error)
        {
            if (total_missed_ka < DEVICE_WAIT_COUNT)
                app_set_stats(missed_ka_id, DEVICE_WAIT_COUNT);
        }
     }

     if (close_cnt < APP_CONNECT_FAIL_CNT && open_fail_cnt < APP_CONNECT_FAIL_CNT)
     {
        unsigned long now;
        app_os_get_system_time(&now);
        result = (now - started) > APP_STABLE_TIME ? 1 : 0;
     }
     return result;
}
