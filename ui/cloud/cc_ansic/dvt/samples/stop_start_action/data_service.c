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

#include <stdlib.h>
#include <stdbool.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"

typedef struct device_request_handle {
    connector_request_id_data_service_t request;
    char const * response_data;
    device_request_target_t target;
    size_t length_in_bytes;
    connector_transport_t transport;
    connector_bool_t response_required;
} device_request_handle_t;

static unsigned int device_request_active_count = 0;

#define enum_to_case(name)  case name:  result = #name;             break

static char const * device_request_to_string(device_request_target_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(device_request_not_support);
        enum_to_case(device_request_invalid_data);
        enum_to_case(device_request_stop_connector);
        enum_to_case(device_request_start_connector);
        enum_to_case(device_request_stop_terminate_connector);
        enum_to_case(device_request_app_stop_connector);
        enum_to_case(device_request_app_start_connector);
        enum_to_case(device_request_stop_all_transports);
        enum_to_case(device_request_abort_device_request);
        enum_to_case(device_request_abort_stop_callback);
        enum_to_case(device_request_terminate_connector);
    }
    return result;
}

char const * transport_to_string(connector_transport_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        #if (defined CONNECTOR_TRANSPORT_TCP)
        enum_to_case(connector_transport_tcp);
        #endif
        #if (defined CONNECTOR_TRANSPORT_UDP)
        enum_to_case(connector_transport_udp);
        #endif
        #if (defined CONNECTOR_TRANSPORT_SMS)
        enum_to_case(connector_transport_sms);
        #endif
        enum_to_case(connector_transport_all);
    }
    return result;
}

static connector_status_t device_request_action(device_request_handle_t * const client_device_request)
{
    connector_status_t status = connector_success;

    switch (client_device_request->target)
    {
        case device_request_invalid_data:
        case device_request_not_support:
            break;

        case device_request_stop_connector:
        APP_DEBUG("device_request_action: stop %s the connector (connector_wait_sessions_complete)\n", transport_to_string(client_device_request->transport));
        connector_close_status = connector_close_status_device_stopped;
        {
            connector_initiate_stop_request_t const request_data = {client_device_request->transport, connector_wait_sessions_complete, NULL};

            stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
            status = connector_initiate_action(connector_handle, connector_initiate_transport_stop, &request_data);
            if (status != connector_success)
            {
                APP_DEBUG("device_request_action: connector_initiate_action for connector_initiate_transport_stop returns error %d\n", status);
                goto done;
            }

            break;
        }
        case device_request_start_connector:
        APP_DEBUG("device_request_action: start %s connector\n", transport_to_string(client_device_request->transport));
        {
            status = connector_initiate_action(connector_handle, connector_initiate_transport_start, &client_device_request->transport);
            if (status != connector_success)
            {
                APP_DEBUG("device_request_action: connector_initiate_action for connector_initiate_transport_start returns error %d\n", status);
                goto done;
            }

            break;
        }
        case device_request_stop_terminate_connector:
        APP_DEBUG("device_request_action: stop %s connector immediately and terminate\n", transport_to_string(client_device_request->transport));
        connector_close_status = connector_close_status_device_stopped;
        {
            connector_initiate_stop_request_t const request_data = {client_device_request->transport, connector_stop_immediately, NULL};

            stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
            status = connector_initiate_action(connector_handle, connector_initiate_transport_stop, &request_data);
            if (status != connector_success)
            {
                APP_DEBUG("device_request_action: connector_initiate_action for connector_initiate_transport_stop returns error %d\n", status);
                goto done;
            }
            break;
        }
        case device_request_app_stop_connector:
            APP_DEBUG("device_request_action: application stop %s connector\n", transport_to_string(client_device_request->transport));
            break;

        case device_request_app_start_connector:
            APP_DEBUG("device_request_action: application start %s connector\n", transport_to_string(client_device_request->transport));
            break;

        case device_request_stop_all_transports:
            APP_DEBUG("device_request_action: stop All connector transport\n");
            connector_close_status = connector_close_status_device_stopped;
            {
                connector_initiate_stop_request_t const request_data = {connector_transport_all, connector_wait_sessions_complete, NULL};

                stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
                status = connector_initiate_action(connector_handle, connector_initiate_transport_stop, &request_data);
                if (status != connector_success)
                {
                    APP_DEBUG("device_request_action: connector_initiate_action for connector_initiate_transport_stop returns error %d\n", status);
                    goto done;
                }
                break;
            }
        case device_request_abort_stop_callback:
            APP_DEBUG("device_request_action: abort at %s stop callback\n", transport_to_string(client_device_request->transport));
            connector_close_status = connector_close_status_device_stopped;
            stop_callback_status = connector_callback_abort;
            {
                /* call initiate action to stop and let the close callback to return abort */
                connector_initiate_stop_request_t const request_data = {client_device_request->transport, connector_wait_sessions_complete, NULL};

                stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
                status = connector_initiate_action(connector_handle, connector_initiate_transport_stop, &request_data);
                if (status != connector_success)
                {
                    APP_DEBUG("device_request_action: connector_initiate_action for connector_initiate_transport_stop returns error %d\n", status);
                    goto done;
               }
                break;
            }

        case device_request_abort_device_request:
            break;

        case device_request_terminate_connector:
        APP_DEBUG("device_request_action: stop connector immediately and terminate\n");
        connector_close_status = connector_close_status_device_terminated;
        {
            status = connector_initiate_action(connector_handle, connector_initiate_terminate, NULL);
            if (status != connector_success)
            {
                APP_DEBUG("device_request_action: connector_initiate_action for connector_initiate_terminate returns error %d\n", status);
                goto done;
            }
            break;
        }
    }

    initiate_action.target = client_device_request->target;
    initiate_action.transport = client_device_request->transport;
    APP_DEBUG("device_request_action: initiate transport %d\n", initiate_action.transport);


done:
    return status;
}

static char const device_request_done[] = "Initiate action done";
static char const device_request_unable_to_processed[] = "Initiate action error";
static char const device_request_not_supported_target[] = "Target not supported";
static char const device_request_not_supported_data[] = "Invalid data";
static char const device_device_no_memory[] = "No memory";

static size_t const device_request_max_response_length = (sizeof device_request_done > sizeof device_request_unable_to_processed) ? sizeof device_request_done : sizeof device_request_unable_to_processed;

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_info)
{
    /* supported target name */
    static char const stop_target[] = "stop_connector";
    static char const start_target[] = "start_connector";
    static char const stop_terminate_target[] = "stop_terminate_connector";
    static char const app_stop_target[] = "application_stop_connector";
    static char const app_start_target[] = "application_start_connector";
    static char const stop_all_transports_target[] = "stop_all_transports";
    static char const abort_device_request[] = "abort_device_request";
    static char const abort_stop_callback[] = "abort_stop_callback";
    static char const terminate_connector[] = "terminate_connector";

    connector_callback_status_t result = connector_callback_continue;
    device_request_target_t target_type = device_request_not_support;

    device_request_handle_t * device_request = target_info->user_context;

    ASSERT(target_info->target != NULL);

    if (device_request == NULL)
    {

        if (strcmp(target_info->target, stop_target) == 0)
        {
            target_type = device_request_stop_connector;
        }
        else if (strcmp(target_info->target, start_target) == 0)
        {
            target_type = device_request_start_connector;
        }
        else if (strcmp(target_info->target, stop_terminate_target) == 0)
        {
            target_type = device_request_stop_terminate_connector;
        }
        else if (strcmp(target_info->target, app_stop_target) == 0)
        {
            target_type = device_request_app_stop_connector;
        }
        else if (strcmp(target_info->target, app_start_target) == 0)
        {
            target_type = device_request_app_start_connector;
        }
        else if (strcmp(target_info->target, stop_all_transports_target) == 0)
        {
            target_type = device_request_stop_all_transports;
        }
        else if (strcmp(target_info->target, abort_device_request) == 0)
        {
            connector_close_status = connector_close_status_abort;
            result = connector_callback_abort;
#if 0
            {
                /* call initiate action to stop and let the close callback to return abort */
                connector_initiate_stop_request_t const request_data = {transport_type, connector_wait_sessions_complete, NULL};

                stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
                connector_status_t const status = connector_initiate_action(connector_handle, connector_initiate_transport_stop, &request_data);
                if (status != connector_success)
                {
                    APP_DEBUG("app_process_device_request: connector_initiate_action for connector_initiate_transport_stop returns error %d\n", status);
                }
            }
#endif
            goto done;
        }
        else if (strcmp(target_info->target, abort_stop_callback) == 0)
        {
            target_type = device_request_abort_stop_callback;
        }
        else if (strcmp(target_info->target, terminate_connector) == 0)
        {
            target_type = device_request_terminate_connector;
        }

        /* 1st chunk of device request so let's allocate memory for it
         * and setup user_context for the client_device_request.
         */
        {
            void * ptr;

            connector_callback_status_t const is_ok = app_os_malloc(sizeof * device_request, &ptr);
            if (is_ok != connector_callback_continue|| ptr == NULL)
            {
                /* no memeory so cancel this request */
                APP_DEBUG("app_process_device_request_target: malloc fails for device request target = %s\n", target_info->target);
                result = connector_callback_error;
                goto done;
            }

            device_request = ptr;
        }
        device_request->length_in_bytes = 0;
        device_request->response_data = NULL;
        device_request->target = target_type;
        device_request->transport = connector_transport_all;
        device_request->response_required = target_info->response_required;

         /* setup the user_context for our device request data */
         target_info->user_context = device_request;
         device_request_active_count++;
    }

done:
    return result;
}

static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const receive_data)
{
    static char const udp_transport[] = "UDP";
    static char const sms_transport[] = "SMS";
    static char const tcp_transport[] = "TCP";

    connector_callback_status_t result = connector_callback_continue;
    device_request_handle_t * device_request = receive_data->user_context;

    connector_transport_t transport_type = connector_transport_all;

    {
        char * transport = (char *)receive_data->buffer;

        if (memcmp(transport, udp_transport, receive_data->bytes_used) == 0)
        {
#if (defined CONNECTOR_TRANSPORT_UDP)
            transport_type = connector_transport_udp;
#endif
        }
        else if (memcmp(transport, sms_transport,receive_data->bytes_used) == 0)
        {
#if (defined CONNECTOR_TRANSPORT_SMS)
            transport_type = connector_transport_sms;
#endif
        }
        else if (memcmp(transport, tcp_transport, receive_data->bytes_used) == 0)
        {
#if (defined CONNECTOR_TRANSPORT_TCP)
            transport_type = connector_transport_tcp;
#endif
        }
        else
        {
            APP_DEBUG("app_process_device_request_data: invalid data  %" PRIsize " \"%.*s\"\n", receive_data->bytes_used, (int)receive_data->bytes_used, transport);
            device_request->target = device_request_invalid_data;
            result = connector_callback_error;
        }

    }

    APP_DEBUG("app_process_device_request_data: %p request %s on %s previous %s\n", (void *) device_request,
            device_request_to_string(device_request->target), transport_to_string(transport_type), device_request_to_string(initiate_action.target));

    if (!device_request->response_required)
    {
        device_request->transport = transport_type;

        if ((result == connector_callback_continue) && (device_request_action(device_request) == connector_service_busy))
        {
            result = connector_callback_busy;
            goto done;
        }
    }

done:
    return result;
}

static connector_callback_status_t app_process_device_request_reply_length(connector_data_service_length_t * const reply_data)
{
    connector_callback_status_t result = connector_callback_continue;

    device_request_handle_t * const device_request = reply_data->user_context;

    if (device_request == NULL)
    {
        reply_data->total_bytes = sizeof device_device_no_memory -1;
    }
    else
    {
        switch (device_request->target)
        {
            case device_request_not_support:
                reply_data->total_bytes = sizeof device_request_not_supported_target -1;
                break;

            case device_request_invalid_data:
                reply_data->total_bytes = sizeof device_request_not_supported_data -1;
                break;
            default:
                reply_data->total_bytes = device_request_max_response_length;
                break;
        }
    }

    return result;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t result = connector_callback_continue;
    device_request_handle_t * const device_request = reply_data->user_context;

    if (device_request == NULL)
    {
        size_t const device_device_no_memory_lenght = sizeof device_device_no_memory -1;

        ASSERT(reply_data->bytes_available >= device_device_no_memory_lenght);

        memcpy(reply_data->buffer, device_device_no_memory, device_device_no_memory_lenght);
        reply_data->bytes_used = device_device_no_memory_lenght;
        goto done;
    }


    if (device_request->length_in_bytes == 0)
    {
        char * response_message_data = (char *)device_request_done;

        if (initiate_action.target != device_request_not_support)
        {
            /* busy doing previous initiate_action */
            result = connector_callback_busy;
            goto done;
        }

        switch (device_request->target)
        {
            case device_request_not_support:
                response_message_data = (char *)device_request_not_supported_target;
                break;

            case device_request_invalid_data:
                response_message_data = (char *)device_request_not_supported_data;
                break;

            case device_request_abort_device_request:
                /* should not come here. Abort in the request */
                ASSERT(0);
                break;

            default:
            {
                connector_status_t const status = device_request_action(device_request);
                if (status == connector_service_busy)
                {
                    result = connector_callback_busy;
                    goto done;
                }
                if (status != connector_success)
                {
                    response_message_data = (char *)device_request_unable_to_processed;
                }
            }

        }

        APP_DEBUG("app_process_device_request_response: device_request_target = %s\n", device_request_to_string(initiate_action.target));
        device_request->response_data = response_message_data;
        device_request->length_in_bytes = strlen(response_message_data);
    }

    {
        /* get number of bytes written to the client data buffer */
        size_t const bytes = (device_request->length_in_bytes < reply_data->bytes_available) ? device_request->length_in_bytes : reply_data->bytes_available;

        /* let's copy the response data to service_response buffer */
        memcpy(reply_data->buffer, device_request->response_data, bytes);

        device_request->response_data += bytes;
        device_request->length_in_bytes -= bytes;

        reply_data->more_data = (device_request->length_in_bytes == 0) ? connector_false : connector_true;
        reply_data->bytes_used = bytes;

    }

done:
    return result;
}

static connector_callback_status_t app_process_device_request_status(connector_data_service_status_t const * const status_data)
{
    connector_callback_status_t status = connector_callback_continue;

    device_request_handle_t * const device_request = status_data->user_context;

    if (device_request != NULL)
    {

        switch (status_data->status)
        {
        case connector_data_service_status_complete:
            APP_DEBUG("app_process_device_request_status: handle %p session completed\n",
                    (void *) device_request);
            break;
        case connector_data_service_status_session_error:
            APP_DEBUG("app_process_device_request_status: handle %p session error %d\n",
                       (void *) device_request, status_data->session_error);
            break;
        default:
            APP_DEBUG("app_process_device_request_status: handle %p error %d\n",
                        (void *)device_request, status_data->status);
            break;
        }

        device_request_active_count--;
        put_file_active_count[device_request->transport]--;
        app_os_free(device_request);
    }

    return status;
}

connector_callback_status_t app_device_request_handler(connector_request_id_data_service_t const request_id, void * data)
{
    connector_callback_status_t status = connector_callback_unrecognized;

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
            app_process_device_request_reply_length(data);
            break;
        default:
            APP_DEBUG("app_device_request_handler: unknown request id type %d for connector_request_id_data_service\n", request_id);
            break;
    }

    return status;
}

connector_callback_status_t app_put_request_handler(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * const send_ptr = cb_data;
            ds_record_t * const user = send_ptr->user_context;

            {
                send_ptr->bytes_used = user->file_length_in_bytes - user->bytes_sent;

                if (send_ptr->bytes_used > send_ptr->bytes_available)
                    send_ptr->bytes_used = send_ptr->bytes_available;

                memcpy(send_ptr->buffer, &user->file_data[user->bytes_sent], send_ptr->bytes_used);
                if (user->bytes_sent == 0)
                {
                    app_os_get_system_time(&user->first_data_time);
                    APP_DEBUG("app_put_request_handler: (data request) %s %p transport %s\n", user->file_path, (void *)user, transport_to_string(user->header.transport));
                }

                user->bytes_sent += send_ptr->bytes_used;
                if (user->bytes_sent == user->file_length_in_bytes)
                {
                    app_os_get_system_time(&user->last_data_time);
                    send_ptr->more_data = connector_false;
                }
                else
                    send_ptr->more_data = connector_true;
            }

            break;
        }

        case connector_request_id_data_service_send_response:
        {
            connector_data_service_send_response_t * const resp_ptr = cb_data;
            ds_record_t * const user = resp_ptr->user_context;
            unsigned long current_time;

            app_os_get_system_time(&current_time);
            APP_DEBUG("app_put_request_handler: (response) %s %p\n", user->file_path, (void *)user);
            APP_DEBUG("app_put_request_handler: (response) time initiate = %lu\n", user->initiate_time);
            APP_DEBUG("app_put_request_handler: (response) time between initiate and 1st data  = %lu\n", (user->first_data_time - user->initiate_time));
            APP_DEBUG("app_put_request_handler: (response) time between 1st and last data = %lu\n", (user->last_data_time - user->first_data_time));
            APP_DEBUG("app_put_request_handler: (response) time between last data and response = %lu\n", (current_time - user->last_data_time));

            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("app_put_request_handler: server response %s\n", resp_ptr->hint);
            }

            /* should be done now */
            APP_DEBUG("app_put_request_handler (response): status = %d, %s done this session %p\n", resp_ptr->response, user->file_path, (void *)user);

            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * const error_ptr = cb_data;
            ds_record_t * const user = error_ptr->user_context;

            APP_DEBUG("app_put_request_handler (status): %s cancel this session %p\n", user->file_path, (void *)user);
            ASSERT(user != NULL);
            put_file_active_count[user->header.transport]--;
            app_os_free(user);

            break;
        }
        case connector_request_id_data_service_send_length:
        {
            connector_data_service_length_t * const resp_ptr = cb_data;
            ds_record_t * const user = resp_ptr->user_context;

            resp_ptr->total_bytes = user->file_length_in_bytes;

        }
            break;

        default:
            APP_DEBUG("app_put_request_handler: Unexpected request ID: %d\n", request_id);
            break;
    }

    return status;
}

connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id,
                                                  void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
    case connector_request_id_data_service_send_length:
    case connector_request_id_data_service_send_data:
    case connector_request_id_data_service_send_status:
    case connector_request_id_data_service_send_response:
        status = app_put_request_handler(request_id, data);
        break;
    case connector_request_id_data_service_receive_target:
    case connector_request_id_data_service_receive_data:
    case connector_request_id_data_service_receive_status:
    case connector_request_id_data_service_receive_reply_data:
    case connector_request_id_data_service_receive_reply_length:
        status = app_device_request_handler(request_id, data);
        break;
    default:
        APP_DEBUG("app_data_service_handler: Request not supported: %d\n", request_id);
        break;
    }
    return status;
}

