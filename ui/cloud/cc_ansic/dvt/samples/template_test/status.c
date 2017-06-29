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

static connector_callback_status_t app_tcp_status(connector_tcp_status_t const * const status)
{

    switch (*status)
    {
    case connector_tcp_communication_started:
        keepalive_missed_count = 0;
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
