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
#include "connector_api.h"
#include "platform.h"

static connector_callback_status_t app_tcp_status(connector_status_tcp_event_t const * const tcp_event)
{
    /* We don't want to see first missed and restored keepalive debug printf.
     * Keepalive sometimes missed and restored almost at the same time.
     */
    static size_t keepalive_missed_count = 0;

    switch (tcp_event->status)
    {
    case connector_tcp_communication_started:
        keepalive_missed_count = 0;
        APP_DEBUG("connector_tcp_communication_started\n");
        break;
    case connector_tcp_keepalive_missed:
        if (keepalive_missed_count > 0)
            APP_DEBUG("connector_tcp_keepalive_missed\n");
        keepalive_missed_count++;
        break;
    case connector_tcp_keepalive_restored:
        keepalive_missed_count--;
        if (keepalive_missed_count > 0)
            APP_DEBUG("connector_tcp_keepalive_restored\n");
        break;
    }

    return connector_callback_continue;
}

connector_callback_status_t app_status_handler(connector_request_id_status_t const request_id, void  * const data)
{
    connector_callback_status_t status = connector_callback_continue;


    switch (request_id)
    {
    case connector_request_id_status_tcp:
        status = app_tcp_status(data);
        break;
    case connector_request_id_status_stop_completed:
        APP_DEBUG("app_status_handle: connector_request_id_status_stop_completed\n");
        break;
    default:
        /* unsupported */
        APP_DEBUG("app_status_handler unsupported request %d\n", request_id);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}
