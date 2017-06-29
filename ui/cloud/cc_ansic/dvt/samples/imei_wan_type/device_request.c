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


extern connector_callback_status_t app_os_malloc(size_t const size, void ** ptr);
extern connector_callback_status_t app_os_free(void * const ptr);

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
    int index;
    unsigned long initiate_time;
    unsigned long first_data_time;
    unsigned long last_data_time;
} ds_record_t;

unsigned int put_file_active_count = 0;
static bool first_time = true;
size_t put_request_size = 0;

connector_status_t send_file(connector_handle_t handle, int index, char * const filename, char * const content, size_t content_length)
{

    connector_status_t status = connector_success;
    static char file_type[] = "text/plain";
    ds_record_t * user;

    {
        void * ptr;

        connector_callback_status_t const is_ok = app_os_malloc(sizeof *user, &ptr);
        if (is_ok != connector_callback_continue|| ptr == NULL)
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
        app_os_free(user);
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

            app_os_get_system_time(&current_time);
            APP_DEBUG("app_put_request_handler: (response) %s %p\n", user->file_path, (void *)user);
            APP_DEBUG("app_put_request_handler: (response) time initiate = %lu\n", user->initiate_time);
            APP_DEBUG("app_put_request_handler: (response) time between initiate and 1st data  = %lu\n", (user->first_data_time - user->initiate_time));
            APP_DEBUG("app_put_request_handler: (response) time between 1st and last data = %lu\n", (user->last_data_time - user->first_data_time));
            APP_DEBUG("app_put_request_handler: (response) time between last data and response = %lu\n", (current_time - user->last_data_time));

            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("app_put_request_handler: Device Cloud response %s\n", resp_ptr->hint);
            }

            /* should be done now */
            APP_DEBUG("app_put_request_handler (response): status = %d, %s done this session %p\n", resp_ptr->response, user->file_path, (void *)user);
            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * const error_ptr = cb_data;
            ds_record_t * const user = error_ptr->user_context;

            APP_DEBUG("app_put_request_handler (status): %s close this session %p, status = %d\n", user->file_path, (void *)user, error_ptr->status);
            ASSERT(user != NULL);
            app_os_free(user);
            put_file_active_count--;

            break;
        }

        default:
            APP_DEBUG("app_put_request_handler: Unexpected request ID: %d\n", request_id);
            break;
    }

    return status;
}

/* supported target name */
static char const wan_type_target[] = "wan_type";

device_request_target_t device_request_target = device_request_not_support;

typedef struct device_request_handle {
    void * device_handle;
    char const * response_data;
    device_request_target_t target;
    size_t length_in_bytes;
} device_request_handle_t;

static unsigned int device_request_active_count = 0;

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * device_request = target_data->user_context;

    ASSERT(target_data->target != NULL);

    APP_DEBUG("Device request data: target = \"%s\"\n", target_data->target);

    if (target_data->user_context == NULL)
    {
        /* 1st chunk of device request so let's allocate memory for it
         * and setup user_context for the client_device_request.
         */
       void * ptr;

        connector_callback_status_t const ccode = app_os_malloc(sizeof *device_request, &ptr);
        if (ccode != connector_callback_continue || ptr == NULL)
        {
            APP_DEBUG("process_device_request: malloc fails for device request on %s target\n", target_data->target);
            status = connector_callback_error;
            goto done;
        }

        device_request = ptr;
        target_data->user_context = ptr;

    }

    /* check for supported target */
    if (strcmp(target_data->target, wan_type_target) != 0)
    {
        /* unsupported target so let's cancel it */
        device_request->target = device_request_not_support;
        status = connector_callback_error;
     }
    else
    {
        device_request->target = device_request_wan_type_target;
    }

    device_request->length_in_bytes = 0;
    device_request->response_data = NULL;
    device_request_active_count++;

done:
    return status;
}

static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const receive_data)
{
    connector_callback_status_t status = connector_callback_continue;

    UNUSED_ARGUMENT(receive_data);
    ASSERT(receive_data->user_context != NULL);

    /* don't care about the data */
    return status;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t status = connector_callback_continue;

    device_request_handle_t * const device_request = reply_data->user_context;

    if (device_request == NULL)
    {
        static char const no_memory_text[] = "No memory";
        char * const buffer = (char *)no_memory_text;

        reply_data->bytes_used = sizeof no_memory_text -1;
        ASSERT(reply_data->bytes_used <= reply_data->bytes_available);

        memcpy(reply_data->buffer, buffer, reply_data->bytes_used);
        goto done;
    }

    if (device_request->length_in_bytes == 0)
    {
        static char const device_request_done[] = "Device request success";
        static char const device_request_unable_to_processed[] = "Device request unsupported";

        char * response_message_data = (char *)device_request_done;

        switch (device_request->target)
        {
            case device_request_not_support:
                response_message_data = (char *)device_request_unable_to_processed;
                break;

            case device_request_wan_type_target:
            {
                #define WAN_ID_LENGTH            18
                #define VALUE_TO_STRING(value)   # value
                #define MACRO_TO_STRING(macro)   VALUE_TO_STRING(macro)

                #define WAN_TYPE_FORMAT "<type>%d</type><id>%s</id>"

                static char wan_type_response[sizeof WAN_TYPE_FORMAT + sizeof(INT_MAX) + WAN_ID_LENGTH];

                ASSERT(strlen(wan_type_response) <= WAN_ID_LENGTH);
                sprintf(wan_type_response, WAN_TYPE_FORMAT, wan_type, wan_id_string);
                APP_DEBUG("%s\n", wan_type_response);
                response_message_data = (char *)wan_type_response;
                break;
            }
        }

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

    APP_DEBUG("Device response data: send response data = %.*s\n", (int)reply_data->bytes_used, reply_data->buffer);

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

