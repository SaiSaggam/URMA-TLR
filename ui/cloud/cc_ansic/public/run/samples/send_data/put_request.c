/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "connector_api.h"
#include "platform.h"

#define SEND_DATA_SIZE  64
typedef struct
{
    char const * data_ptr;
    size_t bytes;
} client_data_t;

connector_status_t app_send_put_request(connector_handle_t handle)
{
    connector_status_t status = connector_no_resource;
    static connector_request_data_service_send_t header; /* Cloud Connector will hold this until we get a response/error callback */
    static char const file_path[] = "test/test.txt";
    static char const file_type[] = "text/plain";
    static char const buffer[] = "Connector data service sample\n";
    client_data_t * const app_data = malloc(sizeof *app_data);

    if (app_data == NULL)
        goto done;

    app_data->data_ptr = buffer;
    app_data->bytes = strlen(buffer);
    header.transport = connector_transport_tcp;
    header.option = connector_data_service_send_option_append;
    header.path  = file_path;
    header.content_type = file_type;
    header.user_context = app_data; /* will be returned in all subsequent callbacks */

    status = connector_initiate_action(handle, connector_initiate_send_data, &header);

    APP_DEBUG("Status: %d, file: %s\n", status, file_path);

    if (status != connector_success)
        free(app_data);

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

            APP_DEBUG("Received %s response from Device Cloud\n", (resp_ptr->response == connector_data_service_send_response_success) ? "success" : "error");
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Device Cloud response %s\n", resp_ptr->hint);
            }

            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * const status_ptr = cb_data;
            client_data_t * const app_data = status_ptr->user_context;

            APP_DEBUG("Data service status: %d\n", status_ptr->status);

            free(app_data);
            break;
        }

        default:
            APP_DEBUG("Unexpected command: %d\n", request_id);
            break;
    }

    return status;
}

