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

extern connector_handle_t connector_handle;
extern connector_callback_status_t app_os_malloc(size_t const size, void ** ptr);
extern connector_callback_status_t app_os_free(void * const ptr);

int initiate_connector_stop_from_send_data = 0;

#define DS_MAX_USER   5
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
} ds_record_t;

unsigned int put_file_active_count = 0;

connector_status_t send_dummy_test_file(connector_handle_t handle)
{
    connector_status_t status = connector_success;
    char filename[DS_FILE_NAME_LEN];
    static int fileindex = 0;
    static int buffer_initialized = 0;

    if (!buffer_initialized)
    {
        int i;

        for (i = 0; i < DS_DATA_SIZE; i++)
            ds_buffer[i] = 0x41 + (rand() % 0x3B);
        buffer_initialized = 1;
    }

    if (put_file_active_count >= DS_MAX_USER)
    {
        status = connector_invalid_data_range;
        goto done;
    }

    fileindex = (fileindex > 9) ? 0 : fileindex +1;
    sprintf(filename, "test/dvt%d.txt", fileindex);

    {
        connector_status_t status = connector_success;
        static char file_type[] = "text/plain";
        ds_record_t * send_file_data;
        size_t content_length = rand() % (DS_DATA_SIZE + 1);

        {
            connector_callback_status_t const connector_status = app_os_malloc(sizeof *send_file_data, (void **)&send_file_data);
            if (connector_status != connector_callback_continue|| send_file_data == NULL)
            {
                APP_DEBUG("FAILED: malloc failed\n");
                status = connector_no_resource;
                goto done;
            }
        }

        sprintf(send_file_data->file_path, "%s", filename);
        send_file_data->header.option = connector_data_service_send_option_overwrite;
        send_file_data->header.path = send_file_data->file_path;
        send_file_data->header.content_type = file_type;
        send_file_data->header.transport = connector_transport_tcp;
        send_file_data->bytes_sent = 0;
        send_file_data->file_data = ds_buffer;
        send_file_data->file_length_in_bytes = content_length;
        send_file_data->header.user_context = send_file_data;

        status = connector_initiate_action(handle, connector_initiate_send_data, &send_file_data->header);
        if (status == connector_success)
        {
            APP_DEBUG("send_dummy_test_file: %p %s length %" PRIsize "\n", (void *)send_file_data, send_file_data->file_path, send_file_data->file_length_in_bytes);
        }
        else
        {
            app_os_free(send_file_data);
            goto done;
        }
        put_file_active_count++;

    }

done:
    return status;
}

connector_callback_status_t app_data_send_handler(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * const send_ptr = cb_data;
            ds_record_t * const user = send_ptr->user_context;

            if (initiate_connector_stop_from_send_data)
            {
                APP_DEBUG("app_data_send_handler: Terminating CC from 'connector_request_id_data_service_send_data' callback\n");
                connector_initiate_action(connector_handle, connector_initiate_terminate, NULL);
            }

            {
                send_ptr->bytes_used = user->file_length_in_bytes - user->bytes_sent;

                if (send_ptr->bytes_used > send_ptr->bytes_available)
                {
                    send_ptr->bytes_used = send_ptr->bytes_available;
                }

                memcpy(send_ptr->buffer, &user->file_data[user->bytes_sent], send_ptr->bytes_used);

                user->bytes_sent += send_ptr->bytes_used;
                if (user->bytes_sent == user->file_length_in_bytes)
                {
                    send_ptr->more_data = connector_false;
                }
                else
                {
                    send_ptr->more_data = connector_true;
                }
            }

            break;
        }

        case connector_request_id_data_service_send_response:
        {
            connector_data_service_send_response_t * const resp_ptr = cb_data;
            ds_record_t * const user = resp_ptr->user_context;
            unsigned long current_time;

            app_os_get_system_time(&current_time);
            APP_DEBUG("app_data_send_handler: (response) %s %p\n", user->file_path, (void *)user);

            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("app_data_send_handler: server response %s\n", resp_ptr->hint);
            }
            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * const error_ptr = cb_data;
            ds_record_t * const user = error_ptr->user_context;

            APP_DEBUG("app_data_send_handler (status): %s session %p completed with error %d\n", user->file_path, (void *)user, error_ptr->status);
            ASSERT(user != NULL);
            app_os_free(user);
            put_file_active_count--;

            break;
        }

        default:
            APP_DEBUG("app_data_send_handler: Unexpected request ID: %d\n", request_id);
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
        status = app_data_send_handler(request_id, data);
        break;
    case connector_request_id_data_service_receive_target:
    case connector_request_id_data_service_receive_data:
    case connector_request_id_data_service_receive_status:
    case connector_request_id_data_service_receive_reply_data:
    case connector_request_id_data_service_receive_reply_length:
    default:
        APP_DEBUG("app_data_service_handler: Request not supported: %d\n", request_id);
        break;
    }
    return status;
}
