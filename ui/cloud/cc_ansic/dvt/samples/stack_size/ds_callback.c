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
#define DS_DATA_SIZE  (1024 * 2)

static char ds_buffer[DS_DATA_SIZE];

typedef struct
{
    connector_request_data_service_send_t header;
    char file_path[DS_FILE_NAME_LEN];
    size_t bytes_sent;
    size_t file_length_in_bytes;
    char * file_data;
    int index;
    unsigned long initiate_time;
    unsigned long first_data_time;
    unsigned long last_data_time;
} ds_record_t;

unsigned int put_file_active_count = 0;
static bool first_time = true;

connector_status_t send_file(connector_handle_t handle, int index, char * const filename, char * const content, size_t content_length)
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
    app_os_get_system_time(&user->initiate_time);

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

connector_status_t send_put_request(connector_handle_t handle)
{

    connector_status_t status = connector_success;
    char filename[DS_FILE_NAME_LEN];
    static int fileindex = 0;

    if (put_file_active_count >= DS_MAX_USER)
    {
        status = connector_invalid_data_range;
        goto done;
    }


    if (first_time)
    {
        int i;

        for (i = 0; i < DS_DATA_SIZE; i++)
            ds_buffer[i] = 0x41 + (rand() % 0x3B);
        first_time = false;
    }
    fileindex = (fileindex > 9) ? 0 : put_file_active_count +1;
    sprintf(filename, "test/dvt%d.txt", put_file_active_count);
    status = send_file(handle, put_file_active_count, filename, ds_buffer, (rand() % (DS_DATA_SIZE +1)));

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

            ASSERT(user != NULL);
            {
                send_ptr->bytes_used = user->file_length_in_bytes - user->bytes_sent;

                if (send_ptr->bytes_used > send_ptr->bytes_available)
                    send_ptr->bytes_used = send_ptr->bytes_available;

                memcpy(send_ptr->buffer, &user->file_data[user->bytes_sent], send_ptr->bytes_used);
                if (user->bytes_sent == 0)
                {
                    app_os_get_system_time(&user->first_data_time);
                    APP_DEBUG("app_put_request_handler: (data request) %s %p\n", user->file_path, (void *)user);
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

            ASSERT(user != NULL);

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

            ASSERT(user != NULL);

            APP_DEBUG("app_put_request_handler (status): %s session %p done with status = %d\n", user->file_path, (void *)user, error_ptr->status);
            free(user);
            put_file_active_count--;

            break;
        }

        default:
            APP_DEBUG("app_put_request_handler: Unexpected request ID: %d\n", request_id);
            break;
    }

    return status;
}

static char const request_terminate_target[] = "request_terminate";


#define DEVICE_REPONSE_COUNT    1

typedef struct device_request_handle {
    void * session;
    char * response_data;
    size_t length_in_bytes;
    unsigned int count;
    char target[64];
} device_request_handle_t;

static unsigned int device_request_active_count = 0;

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_info)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * device_request = target_info->user_context;

    APP_DEBUG("app_process_device_request_target: Start device request: %s\n", target_info->target);

    if (strcmp(target_info->target, request_terminate_target) == 0)
    {
        connector_status_t action_status;

        APP_DEBUG("app_process_device_request_target: terminate IC (active session = %d)\n", put_file_active_count);
        action_status = connector_initiate_action(connector_handle, connector_initiate_terminate, NULL);
        if (action_status != connector_success)
        {
            APP_DEBUG("process_device_request: connector_initiate_terminate error %d\n", action_status);
        }
        status = connector_callback_error;
        goto done;
    }
    else
    {
        write_stack_info_action("Device Request");
    }

    {
        size_t const device_request_size = sizeof *device_request;
        size_t const target_length = strlen(target_info->target);

        ASSERT(target_length < sizeof device_request->target);

        device_request = malloc(device_request_size);
        if (device_request == NULL)
        {
            /* no memeory stop IIK */
            APP_DEBUG("app_process_device_request_target: malloc fails for device request on %s target\n", target_info->target);
            status = connector_callback_error;
            goto done;
        }
        memcpy(device_request->target, target_info->target, target_length);
        device_request->target[target_length] = '\0';
    }

    device_request->length_in_bytes = 0;
    device_request->response_data = NULL;
    device_request->count = 0;
    device_request_active_count++;
    target_info->user_context = device_request;

    if (first_time)
    {
        int i;

        for (i = 0; i < DS_DATA_SIZE; i++)
            ds_buffer[i] = 0x41 + (rand() % 0x3B);
        first_time = false;
    }

done:
    return status;

}

static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const receive_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * device_request = receive_data->user_context;

    ASSERT(device_request != NULL);

    device_request->length_in_bytes += receive_data->bytes_used;
    APP_DEBUG("app_process_device_request_data: handle %p target = \"%s\" data length = %lu total length = %lu\n",
                                 (void *)device_request,
                                 device_request->target,
                                 (unsigned long int)receive_data->bytes_used,
                                 (unsigned long int)device_request->length_in_bytes);

    if (!receive_data->more_data)
    {
        device_request->response_data = ds_buffer;
        device_request->length_in_bytes = DS_DATA_SIZE;
        device_request->count = DEVICE_REPONSE_COUNT;
    }

    return status;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const device_request = reply_data->user_context;

    if (device_request == NULL)
    {
        goto done;
    }

    {
        size_t const bytes = (device_request->length_in_bytes < reply_data->bytes_available) ? device_request->length_in_bytes : reply_data->bytes_available;

        APP_DEBUG("app_process_device_request_response: handle %p total length = %lu send_byte %lu\n",
                                    (void *)device_request,
                                    (unsigned long int)device_request->length_in_bytes,
                                    (unsigned long int)bytes);

        /* let's copy the response data to service_response buffer */
        memcpy(reply_data->buffer, device_request->response_data, bytes);
        device_request->response_data += bytes;
        device_request->length_in_bytes -= bytes;

        reply_data->bytes_used = bytes;
        reply_data->more_data = (device_request->length_in_bytes == 0 && device_request->count == 1) ? connector_false : connector_true;
   }

    if (device_request->length_in_bytes == 0)
    {
        device_request->count--;
        if (device_request->count > 0)
        {
            /* setup more data to be sent */
            device_request->response_data = ds_buffer;
            device_request->length_in_bytes = (rand() % (DS_DATA_SIZE +1));
        }
    }

done:
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
        free(device_request);
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
