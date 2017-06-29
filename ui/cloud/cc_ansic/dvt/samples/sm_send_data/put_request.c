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
#include <time.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"


#if defined APP_USE_SM_UDP
const connector_transport_t app_transport = connector_transport_udp;

#elif defined APP_USE_SM_SMS
const connector_transport_t app_transport = connector_transport_sms;
#endif

typedef struct
{
    char const * data_ptr;
    size_t bytes;
    uint32_t request_id;
} client_data_t;

static client_data_t * app_send_data = NULL;

#ifdef APP_USE_SM_UDP
connector_status_t app_send_ping(connector_handle_t handle)
{
    static connector_sm_send_ping_request_t request;
    connector_status_t status;

    request.transport = connector_transport_udp;
    request.user_context = &request;
    request.response_required = connector_true;
    request.timeout_in_seconds = CONNECTOR_SM_TIMEOUT_TX;
    request.request_id = NULL;

    status = connector_initiate_action(handle, connector_initiate_ping_request, &request);

    return status;
}
#endif

connector_bool_t app_put_request_complete(void)
{
    return (app_send_data == NULL) ? connector_true : connector_false;
}

connector_status_t app_cancel_put_request(connector_handle_t handle)
{
    connector_status_t status = connector_success;

    if (app_send_data != NULL)
    {
        static connector_sm_cancel_request_t request;

       request.transport = app_transport;
       request.request_id = app_send_data->request_id;

       APP_DEBUG("Previous data send pending, cancel it\n");
       status = connector_initiate_action(handle, connector_initiate_session_cancel, &request);
       if (status == connector_success)
           status = connector_service_busy;
       else
           APP_DEBUG("connector_initiate_session_cancel returned %d\n", status);
    }
    return status;
}

connector_status_t app_send_put_request(connector_handle_t handle, connector_bool_t response_needed)
{
    connector_status_t status;
    static connector_request_data_service_send_t header; /* Cloud connector will hold this until we get a response/error callback */
    static char const file_type[] = "text/plain";
    static char const buffer[] = "iDigi sm_send_data sample data\n";

    if (app_send_data != NULL)
    {
       status = connector_service_busy;
       goto done;
    }

    app_send_data = malloc(sizeof *app_send_data);
    if (app_send_data == NULL)
    {
        status = connector_no_resource;
        goto done;
    }

    app_send_data->data_ptr = buffer;
    app_send_data->bytes = strlen(buffer);

    header.option = connector_data_service_send_option_overwrite;
    header.response_required = response_needed;
    if (header.response_required)
        headertimeout_in_seconds = CONNECTOR_SM_TIMEOUT_TX;
    header.path = response_needed ? "test/sm_data.txt" : "test/sm_data_noresp.txt";
    header.transport = app_transport;
    header.content_type = file_type;
    header.user_context = app_send_data;
    header.request_id = &app_send_data->request_id;

    status = connector_initiate_action(handle, connector_initiate_send_data, &header);

    APP_DEBUG("Status: %d, file: %s\n", status, header.path);

    if (status != connector_success)
    {
        free(app_send_data);
        app_send_data = NULL;
    }

done:
    return status;
}

connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * send_ptr = cb_data;
            client_data_t * const app_data = send_ptr->user_context;

            ASSERT(app_data != NULL);
            send_ptr->bytes_used = (send_ptr->bytes_available > app_data->bytes) ? app_data->bytes : send_ptr->bytes_available;

            memcpy(send_ptr->buffer, app_data->data_ptr, send_ptr->bytes_used);
            app_data->data_ptr += send_ptr->bytes_used;
            app_data->bytes -= send_ptr->bytes_used;
            send_ptr->more_data = (app_data->bytes > 0) ? connector_true : connector_false;

            break;
        }

        case connector_request_id_data_service_send_response:
        {
            connector_data_service_send_response_t * const resp_ptr = cb_data;

            APP_DEBUG("Received %s response from server\n", (resp_ptr->response == connector_data_service_send_response_success) ? "success" : "error");
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Server response %s\n", resp_ptr->hint);
            }
            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * const error_ptr = cb_data;
            client_data_t * const app_data = error_ptr->user_context;

            APP_DEBUG("Data service error: %d\n", error_ptr->status);

            free(app_data);
            app_send_data = NULL;
            break;
        }

        case connector_request_id_data_service_send_length:
        {
            connector_data_service_length_t * const len_ptr = cb_data;
            client_data_t * const app_ptr = len_ptr->user_context;

            len_ptr->total_bytes = app_ptr->bytes;
            break;
        }

        default:
            APP_DEBUG("Unexpected send data command in SM: %d\n", request_id);
            break;
    }

    return status;
}

connector_callback_status_t app_sm_handler(connector_request_id_sm_t const request, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request)
    {
        case connector_request_id_sm_ping_request:
        {
            connector_sm_receive_ping_request_t * const ping_request = data;

            APP_DEBUG("Received ping request. response %s needed\n", ping_request->response_required ? "is" : "is not");
            break;
        }

        case connector_request_id_sm_ping_response:
        {
            connector_sm_ping_response_t * const ping_resp = data;

            APP_DEBUG("Received ping response [%d].\n", ping_resp->status);
            break;
        }

        case connector_request_id_sm_opaque_response:
        {
            connector_sm_opaque_response_t * const response = data;

            APP_DEBUG("Received %" PRIsize " opaque bytes on id %d\n", response->bytes_used, response->id);
            break;
        }

        case connector_request_id_sm_more_data:
        {
            connector_sm_more_data_t * const more_data = data;

            APP_DEBUG("More SM data is waiting on %s in Etherios Device Cloud\n", (more_data->transport == connector_transport_udp) ? "UDP" : "SMS");
            break;
        }

        case connector_request_id_sm_request_connect:
        {
            connector_sm_request_connect_t * const request_connect = data;
            request_connect->allow = connector_true;
            APP_DEBUG("connector_request_id_sm_request_connect called %s...\n", request_connect->allow ? "ALLOWING" : "REJECTING");

            status = connector_callback_continue;
            break;
        }

        default:
            APP_DEBUG("Request not supported in this sample: %d\n", request);
            break;
    }

    return status;
}

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
            APP_DEBUG("Status request not supported: %d\n", request);
            break;
    }

    return status;
}
