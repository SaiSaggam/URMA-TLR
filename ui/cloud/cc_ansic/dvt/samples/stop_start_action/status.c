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

#if (defined CONNECTOR_TRANSPORT_UDP) || (defined CONNECTOR_TRANSPORT_SMS)
#define enum_to_case(name)  case name:  result = #name;             break

static char const * session_status_to_string(connector_session_status_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_session_status_success);
        enum_to_case(connector_session_status_complete);
        enum_to_case(connector_session_status_cancel);
        enum_to_case(connector_session_status_timeout);
        enum_to_case(connector_session_status_no_resource);
        enum_to_case(connector_session_status_cloud_error);
        enum_to_case(connector_session_status_device_error);
        enum_to_case(connector_session_status_internal_error);
    }
    return result;
}
#endif

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

connector_callback_status_t stop_callback_status = connector_callback_continue;
unsigned stop_transport_count[connector_transport_all + 1] = {0};

connector_callback_status_t app_status_handler(connector_request_id_status_t const request, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;


    switch (request)
    {
    case connector_request_id_status_tcp:
        status = app_tcp_status(data);
        break;
    case connector_request_id_status_stop_completed:
    {
        connector_initiate_stop_request_t const * const stop_request = data;
        status = stop_callback_status;
        stop_callback_status = connector_callback_continue;
        if (status == connector_callback_abort) initiate_action.target = device_request_not_support;

        stop_transport_count[stop_request->transport]--;
        APP_DEBUG("app_status_handler: +++++ %s [%d] +++++ connector_request_id_status_stop_completed\n", transport_to_string(stop_request->transport), stop_transport_count[stop_request->transport]);

        if (stop_request->user_context != NULL)
        {
            connector_status_t const action_status = connector_initiate_action(connector_handle, connector_initiate_transport_start, &stop_request->transport);
            if (action_status != connector_success)
            {
                APP_DEBUG("app_status_handler: connector_initiate_action for connector_initiate_transport_start returns error %d\n", action_status);
                status =  (action_status == connector_service_busy) ? connector_callback_busy : connector_callback_abort;
            }
        }
        break;
    }
#if (defined CONNECTOR_TRANSPORT_UDP) || (defined CONNECTOR_TRANSPORT_SMS)
    case connector_status_ping_response:
    {
        connector_message_status_response_t const * const status_response = data;

        APP_DEBUG("app_status_handler: Received ping response [%s].\n", session_status_to_string(status_response->status));
        break;
    }

    case connector_status_ping_request:
    {
        connector_session_status_t * const status_request = data;

//        *status_request = connector_session_status_device_error; // connector_success;
        APP_DEBUG("app_status_handler: Received ping request %d\n", *status_request);
        break;
    }
#endif
    default:
        /* unsupported */
        APP_DEBUG("app_status_handler: unsupported request %d\n", request);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}

