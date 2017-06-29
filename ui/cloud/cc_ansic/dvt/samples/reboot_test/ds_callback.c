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

#define DS_MAX_USER   1
#define DS_FILE_NAME_LEN  20

typedef struct
{
    connector_request_data_service_send_t header;
    char file_path[DS_FILE_NAME_LEN];
    size_t bytes_sent;
    size_t file_length_in_bytes;
    char * file_data;
    int index;
 } ds_record_t;

unsigned int put_file_active_count = 0;

static connector_status_t send_file(connector_handle_t handle, int index, char * const filename, char * const content, size_t content_length)
{

    connector_status_t status = connector_success;
    static char file_type[] = "text/plain";
    ds_record_t * user;

    {
        void * ptr;

        ptr = malloc(sizeof *user);
        if (ptr == NULL)
        {
            /* no memeory stop Cloud Connector */
            APP_DEBUG("send_put_request: malloc fails\n");
            status = connector_no_resource;
            goto done;
        }
        user = ptr;
    }

    sprintf(user->file_path, "%s", filename);
    user->header.option = connector_data_service_send_option_overwrite;
    user->header.path  = user->file_path;
    user->header.content_type = file_type;
    user->header.user_context = user;
    user->header.transport = connector_transport_tcp;
    user->bytes_sent = 0;
    user->file_data = content;
    user->file_length_in_bytes = content_length;
    user->index = index;

    status = connector_initiate_action(handle, connector_initiate_send_data, &user->header);
    if (status == connector_success)
    {
        APP_DEBUG("send_file: %p %s length %" PRIsize "\n", (void *)user, user->file_path, user->file_length_in_bytes);
        put_file_active_count++;
    }
    else
    {
        free(user);
    }

done:
    return status;
}

connector_status_t send_put_request(connector_handle_t handle, char * const filename, char * const content)
{

    connector_status_t status = connector_success;

    if (put_file_active_count >= DS_MAX_USER)
    {
        status = connector_service_busy;
        goto done;
    }

    status = send_file(handle, put_file_active_count, filename, content, strlen(content));

done:
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
                    APP_DEBUG("app_put_request_handler: (data request) %s %p\n", user->file_path, (void *)user);
                }

                user->bytes_sent += send_ptr->bytes_used;
                if (user->bytes_sent == user->file_length_in_bytes)
                {
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

           APP_DEBUG("app_put_request_handler: (response) %s %p\n", user->file_path, (void *)user);

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

            if (user != NULL)
            {
                APP_DEBUG("app_put_request_handler (status): %s session %p done with status = %d\n", user->file_path, (void *)user, error_ptr->status);
                ASSERT(user != NULL);
                free(user);
                put_file_active_count--;
            }
            else
            {
                APP_DEBUG("app_put_request_handler (status): NO USER CONTEXT active put file count = %d\n", put_file_active_count);
            }
            break;
        }

        default:
            APP_DEBUG("app_put_request_handler: Unexpected request ID: %d\n", request_id);
            break;
    }

    return status;
}

/* we only supported 1 target */
static char const request_reboot_ready[] = "request_reboot_ready";

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_info)
{
    connector_callback_status_t status = connector_callback_continue;

    if (strcmp(target_info->target, request_reboot_ready) == 0)
    {
        /* cause to delay calling receive */
        if (delay_receive_state == no_delay_receive)
            delay_receive_state = start_delay_receive;
        else
            APP_DEBUG("app_process_device_request_target: %s already started\n", request_reboot_ready);
    }
    else
    {
        /* testing to return unrecognized status */
        APP_DEBUG("process_device_request: unrecognized target = \"%s\"\n", target_info->target);
        status = connector_callback_error;
    }
    /* don't care any data in the request */
    return status;
}

static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const receive_data)
{
    connector_callback_status_t status = connector_callback_continue;

    UNUSED_ARGUMENT(receive_data);
    /* don't care any data in the request */
    return status;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t status = connector_callback_continue;
    /* user just lets us know that reboot request is about to start.
     * just respond so he knows we are connected and ready.
     */
    reply_data->bytes_used = 0; /* no data */

    return status;
}

static connector_callback_status_t app_process_device_request_status(connector_data_service_status_t const * const status_data)
{
    connector_callback_status_t status = connector_callback_continue;


    switch (status_data->status)
    {
    case connector_data_service_status_complete:
        APP_DEBUG("app_process_device_request_status: session completed\n");
        break;
    case connector_data_service_status_session_error:
        APP_DEBUG("app_process_device_request_error: session error %d\n",
                   status_data->session_error);
        break;
    default:
        APP_DEBUG("app_process_device_request_error: error %d\n",
                   status_data->status);
        break;
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
        default:
            APP_DEBUG("app_device_request_handler: unknown request id type %d for connector_request_id_data_service\n", request_id);
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


