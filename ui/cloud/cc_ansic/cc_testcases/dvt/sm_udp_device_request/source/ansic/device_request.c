/*
 * Copyright (c) 2012, 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <stdlib.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"

/* supported target (path) names */
static char const device_request_target[] = "myTarget";
static char const device_response_data[]  = "My device response data";
static char const null_target_response[]  = "Empty target in SM device request";
static char const unexpected_target_response[] = "Unexpected target in SM device request";

typedef enum {
    device_request_response_status_ok,
    device_request_response_status_unsupported_target
} device_request_response_status_t;

typedef struct device_request_handle {
    char const * response_data;
    char const * target;
    size_t length_in_bytes;
    device_request_response_status_t status;
} device_request_handle_t;

static unsigned int device_request_active_count;

#ifdef CONNECTOR_TRANSPORT_UDP
connector_bool_t app_ping_pending = connector_false;

connector_status_t app_send_ping(connector_handle_t handle)
{
    connector_status_t status;
    static connector_sm_send_ping_request_t request; /* Cloud connector will hold this until reply received or send completes */
    static uint32_t ping_request_id;
    
    if (app_ping_pending)
    {
        static connector_sm_cancel_request_t cancel_request;

        cancel_request.transport = connector_transport_udp;
        cancel_request.request_id = *request.request_id;

        APP_DEBUG("Previous ping pending, cancel it\n");
        status = connector_initiate_action(handle, connector_initiate_session_cancel, &cancel_request);
        if (status == connector_success)
            status = connector_service_busy;
        else
            APP_DEBUG("connector_initiate_session_cancel returned %d\n", status);
        goto done;
    }

    request.transport = connector_transport_udp;
    request.user_context = NULL;
    request.response_required = connector_true;
    request.request_id = &ping_request_id;
    request.timeout_in_seconds = CONNECTOR_SM_TX_TIMEOUT;
    
    status = connector_initiate_action(handle, connector_initiate_ping_request, &request);
    if (status == connector_success)
        app_ping_pending = connector_true;
    else
        app_ping_pending = connector_false;

    APP_DEBUG("Sent ping [%d].\n", status);

done:
    return status;
}
#endif

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * device_request = target_data->user_context;

    APP_DEBUG("Device request data: target = \"%s\"\n", target_data->target);

    if (target_data->user_context == NULL)
    {

        /* 1st chunk of device request so let's allocate memory for it
         * and setup user_context for the client_device_request.
         */
        {
            void * ptr;

            ptr = malloc(sizeof *device_request);
            if (ptr == NULL)
            {
                /* no memeory so cancel this request */
                APP_DEBUG("app_process_device_request: malloc fails for device request \"%s\" target\n", target_data->target);
                status = connector_callback_error;
                goto done;
            }

            device_request = ptr;
            target_data->user_context = ptr;
        }
    }

    /* check for supported target */
    if ((target_data->target == NULL) || (target_data->target[0] == '\0'))
    {
        device_request->status = device_request_response_status_ok;
        device_request->response_data = null_target_response;
        device_request->target = NULL;
    }
    else if (strcmp(target_data->target, device_request_target) != 0)
    {
        /* unsupported target so let's cancel it */
        device_request->status = device_request_response_status_unsupported_target;
        device_request->response_data = unexpected_target_response;
        status = connector_callback_error;
    }
    else
    {
        device_request->status = device_request_response_status_ok;
        device_request->response_data = device_response_data;
        device_request->target = device_request_target;
     }

     device_request->length_in_bytes = strlen(device_request->response_data);
     device_request_active_count++;

done:
    return status;
}

static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const receive_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const device_request = receive_data->user_context;

    ASSERT(device_request != NULL);

    {
        /* prints device request data */
        char * device_request_data = (char *)receive_data->buffer;
        APP_DEBUG("Device request data: received data = \"%.*s\" for %s\n", (int)receive_data->bytes_used,
                    device_request_data, device_request->target);
    }

    if (!receive_data->more_data)
    {
        APP_DEBUG("Last chunk of device request (SM)\n");
    }

    return status;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t status = connector_callback_continue;

    device_request_handle_t * const device_request = reply_data->user_context;
    size_t bytes_used = 0;
    char const * buffer = NULL;

    if (device_request == NULL)
    {
        static char const no_memory_text[] = "No memory";

        /* no memory from target process */
        buffer = no_memory_text;
        bytes_used = sizeof no_memory_text -1;
    }
    else
    {
        switch (device_request->status)
        {
            case device_request_response_status_unsupported_target:
            {
                buffer = device_request->response_data;
                bytes_used = device_request->length_in_bytes;
                break;
            }

            case device_request_response_status_ok:
                /* get number of bytes written to the client data buffer */
                bytes_used = (reply_data->bytes_available < device_request->length_in_bytes) ? reply_data->bytes_available : device_request->length_in_bytes;
                buffer = device_request->response_data;

                device_request->response_data += bytes_used;
                device_request->length_in_bytes -= bytes_used;

                if (device_request->length_in_bytes > 0) reply_data->more_data = connector_true;
                break;
        }
    }

    if (buffer != NULL)
    {
        APP_DEBUG("Device response data: send response data = %.*s\n", (int)bytes_used, buffer);

        ASSERT(reply_data->bytes_available >= bytes_used);

            /* let's copy the response data to service_response buffer */
        memcpy(reply_data->buffer, buffer, bytes_used);
        reply_data->bytes_used = bytes_used;
    }

    return status;
}

static connector_callback_status_t app_process_device_request_status(connector_data_service_status_t const * const status_data)
{
    connector_callback_status_t status = connector_callback_continue;

    device_request_handle_t * const device_request = status_data->user_context;

    if (device_request != NULL)
    {

        switch (status_data->status)
        {
        case connector_data_service_status_session_error:
            APP_DEBUG("app_process_device_request_error: handle %p session error %d\n",
                       (void *) device_request, status_data->session_error);
            break;
        default:
            APP_DEBUG("app_process_device_request_error: handle %p session error %d\n",
                        (void *)device_request, status_data->status);
            break;
        }

        device_request_active_count--;
        free(device_request);
    }

    return status;
}

static connector_callback_status_t app_process_device_request_total_length(connector_data_service_length_t * const length_ptr)
{
    device_request_handle_t * const device_request = length_ptr->user_context;

    length_ptr->total_bytes = device_request->length_in_bytes;

    return connector_callback_continue;
}

connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_service_receive_target:
            status = app_process_device_request_target(data);
            break;
        case connector_request_id_data_service_receive_data:
            status = app_process_device_request_data(data);
            break;
        case connector_request_id_data_service_receive_status:
            status = app_process_device_request_status(data);
            break;
        case connector_request_id_data_service_receive_reply_data:
            status = app_process_device_request_response(data);
            break;
        case connector_request_id_data_service_receive_reply_length:
            status = app_process_device_request_total_length(data);
            break;
        default:
            APP_DEBUG("app_data_service_handler (SM): unknown request id type %d for connector_request_id_data_service\n", request_id);
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

            app_ping_pending = connector_false;
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
