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
#include "connector_api.h"
#include "platform.h"

connector_status_t app_send_put_request(connector_handle_t handle)
{
    connector_status_t status = connector_success;
    static connector_data_service_put_request_t header;
    static char file_path[] = "test/test.txt";
    static char file_type[] = "text/plain";

    header.flags = CONNECTOR_DATA_PUT_APPEND;
    header.path  = file_path;
    header.content_type = file_type;

    status = connector_initiate_action(handle, connector_initiate_send_data, &header);
    APP_DEBUG("Status: %d, file: %s\n", status, file_path);

    return status;
}

connector_callback_status_t app_data_service_handler(connector_data_service_request_t const request,
                                                  void const * const request_data, size_t const request_length,
                                                  void * response_data, size_t * const response_length)
{
    connector_callback_status_t status = connector_callback_continue;
    connector_data_service_msg_request_t const * const put_request = request_data;
    connector_data_service_msg_response_t * const put_response = response_data;

    UNUSED_ARGUMENT(request_length);
    UNUSED_ARGUMENT(response_length);

    if ((put_request == NULL) || (put_response == NULL))
    {
         APP_DEBUG("Invalid request_data [%p] or response_data[%p]\n", request_data, response_data);
         goto done;
    }

    if (request == connector_data_service_put_request)
    {
        switch (put_request->message_type)
        {
        case connector_data_service_type_need_data:
            {
                connector_data_service_block_t * const message = put_response->client_data;
                char const buffer[] = "iDigi data service sample\n";
                size_t const bytes = strlen(buffer);

                if (message->length_in_bytes > bytes)
                    message->length_in_bytes = bytes;

                memcpy(message->data, buffer, message->length_in_bytes);
                message->flags = CONNECTOR_MSG_LAST_DATA | CONNECTOR_MSG_FIRST_DATA;
                put_response->message_status = connector_msg_error_none;
            }
            break;

        case connector_data_service_type_have_data:
            {
                connector_data_service_block_t * const message = put_request->server_data;
    
                APP_DEBUG("Received %s response from server\n", ((message->flags & CONNECTOR_MSG_RESP_SUCCESS) != 0) ? "success" : "error");
                if (message->length_in_bytes > 0) 
                {
                    char * const data = message->data;
    
                    data[message->length_in_bytes] = '\0';
                    APP_DEBUG("Server response %s\n", data);
                }
            }
            break;

        case connector_data_service_type_error:
            {
                connector_data_service_block_t * const message = put_request->server_data;
                connector_msg_error_t const * const error_value = message->data;

                APP_DEBUG("Data service error: %d\n", *error_value);
            }
            break;

        default:            
            APP_DEBUG("Unexpected command: %d\n", request);
            break;
        }
    }
    else
    {
        APP_DEBUG("Request not supported in this sample: %d\n", request);
    }

done:
    return status;
}



