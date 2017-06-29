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
#include <unistd.h>
#include "connector_api.h"
#include "platform.h"

connector_status_t app_send_ping(connector_handle_t handle)
{
    connector_status_t status;
    static connector_sm_send_ping_request_t request;
    static uint32_t ping_request_id;
    
    int dummy_context = 0;

    request.transport = connector_transport_udp;
    request.user_context = &dummy_context;
    request.response_required = request.response_required ? connector_false : connector_true;
    if (request.response_required)
        request.timeout_in_seconds = CONNECTOR_SM_TIMEOUT_TX;
    request.request_id = &ping_request_id;

    status = connector_initiate_action(handle, connector_initiate_ping_request, &request);
    APP_DEBUG("Sent ping [%d].\n", status);
    if (status == connector_success)
    {
        static size_t test_cases = 0;

        if (test_cases < 2)
        {
            static connector_sm_cancel_request_t cancel_request;

            cancel_request.transport = request.transport;
            cancel_request.request_id = *request.request_id;

            APP_DEBUG("Trying to cancel the ping request\n");
            do
            {
                status = connector_initiate_action(handle, connector_initiate_session_cancel, &cancel_request);
                if (status == connector_service_busy)
                    usleep(2);
                else
                {
                    APP_DEBUG("connector_initiate_session_cancel returned %d\n", status);
                }
            } while (status == connector_service_busy);
        }

        test_cases++;
    }

    return status;
}

typedef struct
{
    connector_request_data_service_send_t header;
    char const * data_ptr;
    size_t bytes;
    uint32_t request_id;
} client_data_t;

connector_status_t app_send_data(connector_handle_t handle)
{
    connector_status_t status = connector_no_resource;
    static char const buffer[] = "iDigi sm udp dvt for device data\n";
    static char const file_path[] = "test/sm_udp_neg.txt";
    static client_data_t app_data[CONNECTOR_SM_UDP_MAX_SESSIONS];
    static size_t test_cases = 0;
    static connector_bool_t response_needed = connector_true;
    client_data_t * const app_ptr = &app_data[test_cases % CONNECTOR_SM_UDP_MAX_SESSIONS];
    connector_request_data_service_send_t * header_ptr = &app_ptr->header;

    app_ptr->data_ptr = buffer;
    app_ptr->bytes = strlen(buffer);
    header_ptr->transport = connector_transport_udp;
    header_ptr->option = connector_data_service_send_option_overwrite;
    header_ptr->response_required = response_needed;
    if (header_ptr->response_required)
        header_ptr->timeout_in_seconds = CONNECTOR_SM_TIMEOUT_TX;
    header_ptr->path = (test_cases % 3) ? NULL : file_path;
    header_ptr->user_context = app_ptr; /* will be returned in all subsequent callbacks */
    header_ptr->request_id = &app_ptr->request_id;

    status = connector_initiate_action(handle, connector_initiate_send_data, header_ptr);
    APP_DEBUG("Status: Send %s [%d]\n", file_path, status);
    if (status == connector_success)
    {
        if (test_cases < 2)
        {
            static connector_sm_cancel_request_t cancel_request;

            cancel_request.transport = header_ptr->transport;
            cancel_request.request_id = *header_ptr->request_id;

            APP_DEBUG("Trying to cancel the send data request\n");
            do
            {
                status = connector_initiate_action(handle, connector_initiate_session_cancel, &cancel_request);
                if (status == connector_service_busy)
                    usleep(2);
                else
                {
                    APP_DEBUG("connector_initiate_session_cancel returned %d\n", status);
                }
            } while (status == connector_service_busy);
        }

        response_needed = response_needed ? connector_false : connector_true;
        test_cases++;
    }

    return status;
}

static connector_callback_status_t app_process_cli_request(connector_sm_cli_request_t * const cli_request)
{
    connector_callback_status_t status = connector_callback_continue;
    static char response_string[] = "Time: Day Mon DD HH:MM:SS YYYY ";
    static client_data_t app_data;
    time_t const cur_time = time(NULL);
    static int send_busy = 1;

    if (send_busy)
    {
        APP_DEBUG("CLI request [%s] returning busy...\n", cli_request->buffer);
        status = connector_callback_busy;
        send_busy = 0;
        goto done;
    }

    APP_DEBUG("Executing %s.\n", cli_request->buffer);

    app_data.bytes = snprintf(response_string, sizeof response_string, "Time: %s", ctime(&cur_time));
    app_data.data_ptr = response_string;
    cli_request->user_context = &app_data;

done:
    return status;
}

static connector_callback_status_t app_process_cli_response(connector_sm_cli_response_t * const cli_response)
{
    connector_callback_status_t status = connector_callback_continue;
    client_data_t const * const app_ptr = cli_response->user_context;
    static int send_busy = 1;

    if (send_busy)
    {
        APP_DEBUG("CLI response returning busy...\n");
        status = connector_callback_busy;
        send_busy = 0;
        goto done;
    }

    cli_response->bytes_used = (cli_response->bytes_available > app_ptr->bytes) ? app_ptr->bytes : cli_response->bytes_available;
    memcpy(cli_response->buffer, app_ptr->data_ptr, cli_response->bytes_used);
    APP_DEBUG("Sending CLI response, %s\n", cli_response->buffer);

done:
    return status;
}

static connector_callback_status_t app_process_cli_response_length(connector_sm_cli_response_length_t * const cli_length)
{
    connector_callback_status_t status = connector_callback_continue;
    client_data_t const * const app_ptr = cli_length->user_context;

    cli_length->total_bytes = app_ptr->bytes;

    return status;
}

static connector_callback_status_t app_handle_put_request(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;
    static size_t test_case = 0;

    switch (request_id)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * const send_ptr = cb_data;
            client_data_t * const app_ptr = send_ptr->user_context;

            if (!(test_case % 2))
            {
                APP_DEBUG("SM negative -> put request -> send_data: returning busy...\n");
                status = connector_callback_busy;
                goto done;
            }

            send_ptr->bytes_used = (send_ptr->bytes_available < app_ptr->bytes) ? send_ptr->bytes_available : app_ptr->bytes;

            memcpy(send_ptr->buffer, app_ptr->data_ptr, send_ptr->bytes_used);
            app_ptr->data_ptr += send_ptr->bytes_used;
            app_ptr->bytes -= send_ptr->bytes_used;
            send_ptr->more_data = (app_ptr->bytes > 0) ? connector_true : connector_false;

            break;
        }

        case connector_request_id_data_service_send_response:
        {
            connector_data_service_send_response_t * const resp_ptr = cb_data;

            if (!(test_case % 2))
            {
                APP_DEBUG("SM negative -> put request -> send_response: returning busy...\n");
                status = connector_callback_busy;
                goto done;
            }

            APP_DEBUG("Received %s response from server\n", (resp_ptr->response == connector_data_service_send_response_success) ? "success" : "error");
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Server response %s\n", resp_ptr->hint);
            }

            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * const status_ptr = cb_data;

            APP_DEBUG("Put request status: %d\n", status_ptr->status);

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
            APP_DEBUG("Unexpected request id in put request: %d\n", request_id);
            status = connector_callback_abort;
            break;
    }

done:
    test_case++;
    return status;
}

static connector_callback_status_t app_handle_device_request(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;
    static size_t test_case = 0;

    switch (request_id)
    {
        case connector_request_id_data_service_receive_target:
        {
            static client_data_t app_data;
            static char const device_response_data[] = "My response data for device request data\n";
            connector_data_service_receive_target_t * const target_ptr = cb_data;

            if (!(test_case % 2))
            {
                APP_DEBUG("SM negative -> device request -> recv_target: returning busy...\n");
                status = connector_callback_busy;
                goto done;
            }

            APP_DEBUG("Received device request for target %s.\n", target_ptr->target);
            app_data.data_ptr = device_response_data;
            app_data.bytes = strlen(device_response_data);
            target_ptr->user_context = &app_data;
            break;
        }

        case connector_request_id_data_service_receive_data:
        {
            connector_data_service_receive_data_t * const recv_ptr = cb_data;

            if (!(test_case % 3))
            {
                APP_DEBUG("SM negative -> device request -> recv_data: returning busy...\n");
                status = connector_callback_busy;
                goto done;
            }

            APP_DEBUG("Data: \"%.*s\".\n", (int)recv_ptr->bytes_used, (char *)recv_ptr->buffer);
            break;
        }

        case connector_request_id_data_service_receive_reply_data:
        {
            connector_data_service_receive_reply_data_t * const reply_ptr = cb_data;
            client_data_t * const app_ptr = reply_ptr->user_context;

            if (!(test_case % 2))
            {
                APP_DEBUG("SM negative -> device request -> recv_reply: returning busy...\n");
                status = connector_callback_busy;
                goto done;
            }

            reply_ptr->bytes_used = (reply_ptr->bytes_available < app_ptr->bytes) ? reply_ptr->bytes_available : app_ptr->bytes;
            memcpy(reply_ptr->buffer, app_ptr->data_ptr, reply_ptr->bytes_used);
            app_ptr->bytes -= reply_ptr->bytes_used;
            app_ptr->data_ptr += reply_ptr->bytes_used;
            reply_ptr->more_data = (app_ptr->bytes > 0) ? connector_true : connector_false;
            break;
        }

        case connector_request_id_data_service_receive_status:
        {
            connector_data_service_status_t * const status_ptr = cb_data;

            APP_DEBUG("SM device request session is closed, status[%d]\n", status_ptr->status);
            break;
        }

        case connector_request_id_data_service_receive_reply_length:
        {
            connector_data_service_length_t * const len_ptr = cb_data;
            client_data_t * const app_ptr = len_ptr->user_context;

            len_ptr->total_bytes = app_ptr->bytes;
            break;
        }

        default:
            APP_DEBUG("Unexpected type in device request: %d\n", request_id);
            status = connector_callback_abort;
            break;
    }

done:
    test_case++;
    return status;
}

connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request)
    {
        case connector_request_id_data_service_send_length:
        case connector_request_id_data_service_send_data:
        case connector_request_id_data_service_send_status:
        case connector_request_id_data_service_send_response:
            status = app_handle_put_request(request, data);
            break;

        default:
            status = app_handle_device_request(request, data);
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

            switch (ping_resp->status)
            {
                case connector_sm_ping_status_success:
                    APP_DEBUG("Received ping response.\n");
                    break;

                case connector_sm_ping_status_complete:
                    APP_DEBUG("Received ping complete.\n");
                    break;

                case connector_sm_ping_status_cancel:
                    APP_DEBUG("Received ping cancel.\n");
                    break;

                default:
                    APP_DEBUG("Received ping error [%d].\n", ping_resp->status);
                    break;
            }

            break;
        }

        case connector_request_id_sm_cli_request:
            status = app_process_cli_request(data);
            break;

        case connector_request_id_sm_cli_response:
            status = app_process_cli_response(data);
            break;

        case connector_request_id_sm_cli_response_length:
            status = app_process_cli_response_length(data);
            break;

        case connector_request_id_sm_cli_status:
        {
            connector_sm_cli_status_t * const status_ptr = data;

            switch (status_ptr->status)
            {
                case connector_sm_cli_status_success:
                    APP_DEBUG("Received CLI status: success\n");
                    break;
                case connector_sm_cli_status_cancel:
                    APP_DEBUG("Received CLI status: cancel\n");
                    break;
                case connector_sm_cli_status_error:
                    APP_DEBUG("Received CLI status: error\n");
                    break;
            }
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
            APP_DEBUG("Status request not supported in sm_udp: %d\n", request);
            break;
    }

    return status;
}
