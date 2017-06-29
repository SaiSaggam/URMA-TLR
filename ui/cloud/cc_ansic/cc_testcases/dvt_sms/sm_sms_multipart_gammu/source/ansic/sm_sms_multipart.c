/*
 * Copyright (c) 2011, 2012, 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <unistd.h>
#include <time.h>
#include "connector_api.h"
#include "platform.h"

typedef struct
{
    #define SM_MAX_FILE_PATH_SIZE 32
    connector_request_data_service_send_t header;
    char file_path[SM_MAX_FILE_PATH_SIZE];
    char const * data_ptr;
    size_t total_bytes;
    size_t bytes_sent;
} client_data_t;

extern char * sm_dvt_random_buffer;
extern size_t sm_dvt_buffer_size;
connector_bool_t sm_dvt_buffer_initialized = connector_false;

connector_status_t app_send_ping(connector_handle_t handle)
{
    connector_status_t status;
    static connector_sm_send_ping_request_t request; /* Cloud connector will hold this until send completes */

    request.transport = connector_transport_sms;
    request.user_context = NULL;
    request.response_required = request.response_required ? connector_false : connector_true;
    if (request.response_required)
        request.timeout_in_seconds = CONNECTOR_SM_TX_TIMEOUT;
    request.request_id = NULL;

    status = connector_initiate_action(handle, connector_initiate_ping_request, &request);
    APP_DEBUG("Sent ping [%d].\n", status);


    /* Added to avoid deadlock due to a connector_init_error */
    static int counter_connector_init_error_send_ping = 0;
    if(status == connector_init_error)
    {
        counter_connector_init_error_send_ping++;

        if(counter_connector_init_error_send_ping>10)
        {
            APP_DEBUG("ABORT TEST DUE TO A connector_init_error in app_send_ping *********************\n");
            exit(0);
        }
    }
    /* Added to avoid deadlock due to a connector_init_error */


    return status;
}

connector_status_t app_send_data(connector_handle_t handle)
{
    static int test_cases = 0;
    static client_data_t app_data[CONNECTOR_SM_SMS_MAX_SESSIONS];
    static connector_bool_t response_needed = connector_true;
    connector_status_t status = connector_no_resource;
    client_data_t * const app_ptr = &app_data[test_cases % CONNECTOR_SM_SMS_MAX_SESSIONS];
    connector_request_data_service_send_t * const header_ptr = &app_ptr->header;

    ASSERT(sm_dvt_random_buffer != NULL);

	if (sm_dvt_buffer_initialized == connector_false)
		return connector_service_busy;

    sleep(5);

    snprintf(app_ptr->file_path, sizeof app_ptr->file_path, "test/sm_sms_mp%d.txt", test_cases  % CONNECTOR_SM_SMS_MAX_SESSIONS);

    app_ptr->data_ptr = sm_dvt_random_buffer;
    app_ptr->total_bytes = sm_dvt_buffer_size;
    app_ptr->bytes_sent = 0;
    header_ptr->transport = connector_transport_sms;
    header_ptr->option = connector_data_service_send_option_overwrite;
    header_ptr->response_required = response_needed;
    if (header_ptr->response_required)
        header_ptr->timeout_in_seconds = CONNECTOR_SM_TX_TIMEOUT;
    header_ptr->path  = app_ptr->file_path;
    header_ptr->user_context = app_ptr; /* will be returned in all subsequent callbacks */
    header_ptr->request_id = NULL;

    status = connector_initiate_action(handle, connector_initiate_send_data, header_ptr);
    APP_DEBUG("Status: %d, file: %s\n", status, header_ptr->path);

    if (status == connector_success)
    {
        response_needed = response_needed ? connector_false : connector_true;
        test_cases++;
    }


    /* Added to avoid deadlock due to a connector_init_error */
    static int counter_connector_init_error_send_data = 0;

    if(status == connector_init_error)
    {
        counter_connector_init_error_send_data++;

        if(counter_connector_init_error_send_data>10)
        {
            APP_DEBUG("ABORT TEST DUE TO A connector_init_error in app_send_data *********************\n");
            exit(0);
        }
    }
    /* Added to avoid deadlock due to a connector_init_error */


    return status;
}

static connector_callback_status_t app_process_cli_request(connector_sm_cli_request_t * const cli_request)
{
    connector_callback_status_t status = connector_callback_continue;
    static int send_busy = 1;
    static client_data_t app_data;

    if (send_busy)
    {
        APP_DEBUG("CLI request [%s] returning busy...\n", cli_request->buffer);
        status = connector_callback_busy;
        send_busy = 0;
        goto done;
    }

    APP_DEBUG("Executing %s.\n", cli_request->buffer);
    app_data.total_bytes = sm_dvt_buffer_size;
    app_data.bytes_sent = 0;
    app_data.data_ptr = sm_dvt_random_buffer;
    cli_request->user_context = &app_data;

done:
    return status;
}

static connector_callback_status_t app_process_cli_response(connector_sm_cli_response_t * const cli_response)
{
    connector_callback_status_t status = connector_callback_continue;
    client_data_t * const app_ptr = cli_response->user_context;
    static int send_busy = 1;

    if (send_busy)
    {
        APP_DEBUG("CLI response returning busy...\n");
        status = connector_callback_busy;
        send_busy = 0;
        goto done;
    }

    {
        size_t const bytes_remaining = app_ptr->total_bytes - app_ptr->bytes_sent;

        cli_response->bytes_used = (cli_response->bytes_available < bytes_remaining) ? cli_response->bytes_available : bytes_remaining;
        memcpy(cli_response->buffer, &app_ptr->data_ptr[app_ptr->bytes_sent], cli_response->bytes_used);
        app_ptr->bytes_sent += cli_response->bytes_used;
        cli_response->more_data = (app_ptr->bytes_sent < app_ptr->total_bytes) ? connector_true : connector_false;
        APP_DEBUG("Sending CLI response [%" PRIsize " bytes]\n", cli_response->bytes_used);
    }

done:
    return status;
}

static connector_callback_status_t app_process_cli_response_length(connector_sm_cli_response_length_t * const cli_length)
{
    connector_callback_status_t status = connector_callback_continue;
    client_data_t const * const app_ptr = cli_length->user_context;
    static int send_busy = 1;

    if (send_busy)
    {
        APP_DEBUG("CLI get length returning busy...\n");
        status = connector_callback_busy;
        send_busy = 0;
    }

    cli_length->total_bytes = app_ptr->total_bytes;

    return status;
}

static connector_callback_status_t app_handle_put_request(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * const send_ptr = cb_data;
            client_data_t * const app_ptr = send_ptr->user_context;
            size_t const bytes_remaining = app_ptr->total_bytes - app_ptr->bytes_sent;

            send_ptr->bytes_used = (send_ptr->bytes_available < bytes_remaining) ? send_ptr->bytes_available : bytes_remaining;

            memcpy(send_ptr->buffer, &app_ptr->data_ptr[app_ptr->bytes_sent], send_ptr->bytes_used);
            app_ptr->bytes_sent += send_ptr->bytes_used;
            send_ptr->more_data = (app_ptr->bytes_sent < app_ptr->total_bytes) ? connector_true : connector_false;

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
            connector_data_service_status_t * const status_ptr = cb_data;

            APP_DEBUG("Put request status: %d\n", status_ptr->status);
            switch (status_ptr->status)
            {
                case connector_data_service_status_complete:
                case connector_data_service_status_cancel:
                    break;

                case connector_data_service_status_timeout:
                {
                    static size_t timeout_count = 0;

                    if (timeout_count++ > CONNECTOR_SM_SMS_MAX_SESSIONS)
                    {
                        APP_DEBUG("SM Put request timeout error. No response from Device Cloud!");
                        status = connector_callback_abort;
                    }
                    break;
                }

                default:
                    APP_DEBUG("SM put request received session error %d\n", status_ptr->session_error);
                    status = connector_callback_abort;
                    break;
            }

            break;
        }

        case connector_request_id_data_service_send_length:
        {
            connector_data_service_length_t * const len_ptr = cb_data;
            client_data_t * const app_ptr = len_ptr->user_context;

            len_ptr->total_bytes = app_ptr->total_bytes;
            break;
        }

        default:
            APP_DEBUG("Unexpected request id in put request: %d\n", request_id);
            status = connector_callback_abort;
            break;
    }

    return status;
}

static connector_callback_status_t app_handle_device_request(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_service_receive_target:
        {
            static client_data_t app_data;
            connector_data_service_receive_target_t * const target_ptr = cb_data;

            APP_DEBUG("Received device request for target %s.\n", target_ptr->target);
            app_data.data_ptr = sm_dvt_random_buffer;
            app_data.total_bytes = sm_dvt_buffer_size;
            app_data.bytes_sent = 0;
            target_ptr->user_context = &app_data;
            break;
        }

        case connector_request_id_data_service_receive_data:
        {
            connector_data_service_receive_data_t * const recv_ptr = cb_data;
            client_data_t * const app_ptr = recv_ptr->user_context;

            //APP_DEBUG("Received data in the request: %s\n", recv_ptr->buffer );

			memcpy((void *)&app_ptr->data_ptr[app_ptr->bytes_sent], recv_ptr->buffer, recv_ptr->bytes_used);

            if (recv_ptr->more_data)
                app_ptr->bytes_sent += recv_ptr->bytes_used;
            else
			{		
				app_ptr->total_bytes = app_ptr->bytes_sent + recv_ptr->bytes_used;
				ASSERT(sm_dvt_buffer_size >= app_ptr->total_bytes);
				sm_dvt_buffer_size = app_ptr->total_bytes;
				sm_dvt_buffer_initialized = connector_true;

                app_ptr->bytes_sent = 0;			
			}

            APP_DEBUG("Received %" PRIsize " request bytes, on SM multipart\n", recv_ptr->bytes_used);
            break;
        }

        case connector_request_id_data_service_receive_reply_data:
        {
            connector_data_service_receive_reply_data_t * const reply_ptr = cb_data;
            client_data_t * const app_ptr = reply_ptr->user_context;
            size_t const bytes_remaining = app_ptr->total_bytes - app_ptr->bytes_sent;

            reply_ptr->bytes_used = (reply_ptr->bytes_available < bytes_remaining) ? reply_ptr->bytes_available : bytes_remaining;
            memcpy(reply_ptr->buffer, &app_ptr->data_ptr[app_ptr->bytes_sent], reply_ptr->bytes_used);
            app_ptr->bytes_sent += reply_ptr->bytes_used;
            reply_ptr->more_data = (app_ptr->bytes_sent < app_ptr->total_bytes) ? connector_true : connector_false;

            APP_DEBUG("Sending %" PRIsize " response bytes, on SM multipart\n", reply_ptr->bytes_used);
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

            len_ptr->total_bytes = app_ptr->total_bytes;
            break;
        }

        default:
            APP_DEBUG("Unexpected type in device request: %d\n", request_id);
            status = connector_callback_abort;
            break;
    }

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

            APP_DEBUG("Received ping response [%d].\n", ping_resp->status);
            switch (ping_resp->status)
            {
                case connector_sm_ping_status_success:
                case connector_sm_ping_status_complete:
                case connector_sm_ping_status_cancel:
                    break;

                case connector_sm_ping_status_timeout:
                {
                    static size_t timeout_count = 0;

                    if (timeout_count++ > CONNECTOR_SM_SMS_MAX_SESSIONS)
                    {
                        APP_DEBUG("Ping request timeout error. No response from Device Cloud!\n");
                        status = connector_callback_abort;
                    }
                    break;
                }

                default:
                    APP_DEBUG("app_sm_handler (multipart): received error [%d]!\n", ping_resp->status);
                    status = connector_callback_abort;
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
            static size_t test_case = 0;

            APP_DEBUG("Received %" PRIsize " opaque bytes on id %d\n", response->bytes_used, response->id);
            switch (test_case)
            {
                case 0:
                    status = connector_callback_busy;
                    break;

                case 1:
                    break;

                case 2:
                    status = connector_callback_error;
                    break;

                default:
                    break;
            }

            test_case++;
            break;
        }

        case connector_request_id_sm_more_data:
        {
            connector_sm_more_data_t * const more_data = data;

            APP_DEBUG("More SM data is waiting on %s in Etherios Device Cloud\n", (more_data->transport == connector_transport_sms) ? "SMS" : "UDP");
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
            APP_DEBUG("Request not supported in this sample: %d\n", request);
            break;
    }

    return status;
}
