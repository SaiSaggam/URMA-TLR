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
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "connector_api.h"
#include "platform.h"

static unsigned int sample_number;

connector_status_t app_send_put_request(connector_handle_t handle)
{
    connector_status_t status = connector_success;
    static connector_request_data_service_send_t header;
    static char file_path[] = "keepalive.txt";
    static char file_type[] = "text/plain";

    header.path  = file_path;
    header.content_type = file_type;
    header.transport = connector_transport_tcp;
    header.user_context = &header; /* will be returned in all subsequent callbacks */


    status = connector_initiate_action(handle, connector_initiate_send_data, &header);
    APP_DEBUG("Status: %d, file: %s\n", status, file_path);

    return status;
}
#define BUFFER_SIZE 64

connector_callback_status_t app_put_request_handler(connector_request_id_data_service_t const request_id, void * const cb_data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * const send_ptr = cb_data;
            char buffer[BUFFER_SIZE];

            snprintf(buffer, BUFFER_SIZE, "keepalive DVT success\n");
            size_t const bytes = strlen(buffer);

            send_ptr->bytes_used = bytes;

            ASSERT (send_ptr->bytes_used <= send_ptr->bytes_available);

            memcpy(send_ptr->buffer, buffer, send_ptr->bytes_used);

            send_ptr->more_data = connector_false;
            sample_number++;
            break;
        }

        case connector_request_id_data_service_send_response:
        {
            connector_data_service_send_response_t * const resp_ptr = cb_data;
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("app_put_request_handler: server response %s\n", resp_ptr->hint);
            }

            APP_DEBUG("app_put_request_handler (response): status = %d\n", resp_ptr->response);
            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * const error_ptr = cb_data;

            APP_DEBUG("app_put_request_handler (status): %d\n", error_ptr->status);
            break;
        }

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
    default:
        APP_DEBUG("app_data_service_handler: Request not supported: %d\n", request_id);
        break;
    }
    return status;
}


