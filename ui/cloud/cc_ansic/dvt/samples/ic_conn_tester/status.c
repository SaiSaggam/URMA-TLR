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

unsigned long miss_ka_time;

static connector_callback_status_t app_tcp_status(connector_status_tcp_event_t const * const tcp_event)
{
    switch (tcp_event->status)
    {
    case connector_tcp_communication_started:
        APP_PRINTF("connector_start_communicating\n");
        break;

    case connector_tcp_keepalive_missed:
        //APP_PRINTF("connector_miss_keepalive\n");
        {
            unsigned long miss_ka_time;
            app_os_get_system_time(&miss_ka_time);
            app_set_stats(missed_ka_time_id, miss_ka_time);
        }
        break;

    case connector_tcp_keepalive_restored:
        {
            static unsigned long old_missed_ka_cnt;
            unsigned long missed_ka_cnt;
            missed_ka_cnt = app_get_stats(missed_ka_id);
            if (missed_ka_cnt > old_missed_ka_cnt)
                APP_PRINTF("connector_restore_keepalive\n");
            old_missed_ka_cnt = missed_ka_cnt;
            app_set_stats(missed_ka_time_id, 0);
        }
        break;
    }
    return connector_callback_continue;
}

connector_callback_status_t app_status_handler(connector_status_request_t const request,
                                           void const * const request_data, size_t const request_length,
                                           void * response_data, size_t * const response_length)
{
    connector_callback_status_t status = connector_callback_continue;

    UNUSED_ARGUMENT(response_data);
    UNUSED_ARGUMENT(request_length);
    UNUSED_ARGUMENT(response_length);

    switch (request)
    {
    case connector_status_tcp:
        status = app_tcp_status(request_data);
        break;
    case connector_status_stop_completed:
        APP_DEBUG("app_status_handler connector_status_stop_completed\n");
        break;
    default:
        /* unsupported */
        APP_DEBUG("app_status_handler unsupported request %d\n", request);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}
