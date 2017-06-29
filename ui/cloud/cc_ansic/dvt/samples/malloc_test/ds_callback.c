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

unsigned int put_file_active_count = 0;

#if (defined CONNECTOR_DATA_SERVICE)

#define INITIAL_WAIT_COUNT      4

#define DS_FILE_NAME_LEN  20
#define DS_DATA_SIZE  (1024 * 16)

static char ds_buffer[DS_DATA_SIZE];

typedef struct
{
    connector_request_data_service_send_t header;
    char file_path[DS_FILE_NAME_LEN];
    size_t bytes_sent;
    size_t file_length_in_bytes;
    char * file_data;
    int index;
} ds_record_t;

static bool first_time = true;

size_t put_request_size = 0;

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

connector_status_t send_put_request(connector_handle_t handle, int index)
{

    connector_status_t status = connector_success;
    char filename[DS_FILE_NAME_LEN];
    static int fileindex = 0;

    if (put_file_active_count >= DS_MAX_USER)
    {
        status = connector_invalid_data_range;
        goto done;
    }

    if (firmware_download_started != 0)
    {
        status = connector_service_busy;
        goto done;
    }


    if (first_time)
    {
        int i;

        for (i = 0; i < DS_DATA_SIZE; i++)
            ds_buffer[i] = 0x41 + (rand() % 0x3B);
        first_time = false;
    }
    fileindex = (fileindex > 9) ? 0 : fileindex +1;
    sprintf(filename, "test/dvt%d.txt", fileindex);
    status = send_file(handle, index, filename, ds_buffer, (rand() % (DS_DATA_SIZE +1)));

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

            APP_DEBUG("app_put_request_handler (status): %s session %p done with status = %d\n", user->file_path, (void *)user, error_ptr->status);
            ASSERT(user != NULL);
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


/* supported targets */
static char const device_request_target[] = "data_target";
static char const device_request_malloc_target[] = "malloc_target";
static char const device_request_invalid_response_target[] = "iik_cancel_response_target";
static char const request_terminate_target[] = "request_terminate";


#define DEVICE_REPONSE_COUNT    2

typedef struct device_request_handle {
    void * session;
    char * response_data;
    size_t length_in_bytes;
    char * target;
    unsigned int count;
} device_request_handle_t;

static unsigned int device_request_active_count = 0;

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_info)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * device_request = target_info->user_context;

    if (strcmp(target_info->target, request_terminate_target) == 0)
    {
        connector_status_t ccode;

        APP_DEBUG("process_device_request_target: terminate iik (active session = %d)\n", put_file_active_count);
        ccode = app_terminate_tcp_transport(connector_handle);
        if (ccode != connector_success)
        {
            APP_DEBUG("process_device_request_target: app_terminate_tcp_transport error %d\n", ccode);
        }
        else
        {
            status = connector_callback_error;
            goto done;
        }
    }
    device_request = malloc(sizeof *device_request);
    if (device_request == NULL)
    {
        /* no memeory stop IIK */
        APP_DEBUG("process_device_request_target: malloc fails for device request on %s target\n", target_info->target);
        status = connector_callback_error;
        goto done;
    }
    put_request_size += sizeof *device_request;

    device_request->length_in_bytes = 0;
    device_request->response_data = NULL;
    device_request->count = 0;
    device_request->target = NULL;
    device_request_active_count++;

    target_info->user_context = device_request;


    if (strcmp(target_info->target, device_request_malloc_target) == 0)
    {
        /* testing regular process target */
        write_usage_file("Data Service");
        device_request->target = (char *)device_request_malloc_target;
    }
    else
    if (strcmp(target_info->target, device_request_target) == 0)
    {
        /* testing regular process target */
        device_request->target = (char *)device_request_target;
    }
    else if (strcmp(target_info->target, device_request_invalid_response_target) == 0)
    {
        /* testing to return error in response callback */
        device_request->target = (char *)device_request_invalid_response_target;
    }
    else
    {
        /* testing to return unrecognized status */
        APP_DEBUG("process_device_request_target: unrecognized target = %s\n", target_info->target);
        status = connector_callback_error;
        goto done;
    }

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
    APP_DEBUG("process_device_request_data: handle %p target = \"%s\" data length = %lu total length = %lu\n",
                                 (void *)device_request,
                                 device_request->target,
                                 (unsigned long int)receive_data->bytes_used,
                                 (unsigned long int)device_request->length_in_bytes);

    if (!receive_data->more_data)
    {   /* No more chunk. let's setup response data */
        /* don't care about what target in here */
        device_request->response_data = ds_buffer;
        device_request->length_in_bytes = (rand() % (DS_DATA_SIZE +1));
        device_request->count = DEVICE_REPONSE_COUNT;
        APP_DEBUG("process_device_request_data: total response length = %" PRIsize "\n", (device_request->length_in_bytes * device_request->count));
    }
    return status;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const device_request = reply_data->user_context;

    if (reply_data == NULL) goto done;

    if (device_request->target == device_request_invalid_response_target)
    {
        /* cancel before sending any response data */
        APP_DEBUG("process_device_response: handle %p cancel\n", (void *)device_request);
        status = connector_callback_error;
        goto done;
    }

    if (device_request->response_data != NULL)
    {
        size_t const bytes = (device_request->length_in_bytes < reply_data->bytes_available) ? device_request->length_in_bytes : reply_data->bytes_available;

        APP_DEBUG("process_device_response: handle %p total length = %lu send_byte %lu\n",
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
    else goto done;

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
    goto done;

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
        case connector_data_service_status_session_error:
            APP_DEBUG("app_process_device_request_status: session error %d\n",
                       status_data->session_error);
            break;
        default:
            APP_DEBUG("app_process_device_request_status: error %d\n",
                        status_data->status);
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

#endif /* (defined CONNECTOR_DATA_SERVICE) */
