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

#include <stdlib.h>
#include "connector_api.h"
#include "platform.h"

/* supported target name */
static char const device_request_target[] = "myTarget";
static char const device_response_data[] = "My device response data";

typedef enum {
    device_request_response_status_ok,
    device_request_response_status_unsupported_target
} device_request_response_status_t;

typedef struct device_request_handle {
    char const * response_data;
    char const * target;
    size_t length_in_bytes;
    device_request_response_status_t status;
} device_request_handle_t;

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * device_request = target_data->user_context;

    APP_DEBUG("Device request data: target = \"%s\"\n", target_data->target);

    if (target_data->user_context == NULL)
    {

        /* 1st chunk of device request so let's allocate memory for it
         * and setup user_context for the client_device_request.
         */
        {
            void * ptr;

            ptr = malloc(sizeof *device_request);
            if (ptr == NULL)
            {
                /* no memory so cancel this request */
                APP_DEBUG("app_process_device_request: malloc fails for device request \"%s\" target\n", target_data->target);
                status = connector_callback_error;
                goto done;
            }

            device_request = ptr;
            target_data->user_context = ptr;
        }
    }

    /* check for supported target */
    if (strcmp(target_data->target, device_request_target) != 0)
    {
        /* unsupported target so let's cancel it */
        device_request->status = device_request_response_status_unsupported_target;
        status = connector_callback_error;
        goto done;
    }
    else
    {
        device_request->status = device_request_response_status_ok;
        device_request->length_in_bytes = 0;
        device_request->response_data = NULL;
        device_request->target = device_request_target;
     }

done:
    return status;
}

static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const receive_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const device_request = receive_data->user_context;

    ASSERT(device_request != NULL);

    {
        /* prints device request data */
        char * device_request_data = (char *)receive_data->buffer;
        APP_DEBUG("Device request data: received data = \"%.*s\" for %s\n", (int)receive_data->bytes_used,
                    device_request_data, device_request->target);
    }

    if (!receive_data->more_data)
    {   /* No more chunk */
        /* setup response data for this target */
        device_request->response_data = device_response_data;
        device_request->length_in_bytes = strlen(device_request->response_data);
    }


    return status;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t status = connector_callback_continue;

    device_request_handle_t * const device_request = reply_data->user_context;
    size_t bytes_used = 0;
    char const * buffer = NULL;

    if (device_request == NULL)
    {
        static char const no_memory_text[] = "No memory";

        /* no memory from target process */
        buffer = no_memory_text;
        bytes_used = sizeof no_memory_text -1;
    }
    else
    {
        switch (device_request->status)
        {
            case device_request_response_status_unsupported_target:
            {
                static char const unsupported_target_text[] = "Target is not supported";

                buffer = unsupported_target_text;
                bytes_used = sizeof unsupported_target_text -1;
                break;
            }

            case device_request_response_status_ok:
                /* get number of bytes written to the client data buffer */
                bytes_used = (reply_data->bytes_available < device_request->length_in_bytes) ? reply_data->bytes_available : device_request->length_in_bytes;
                buffer = device_request->response_data;

                device_request->response_data += bytes_used;
                device_request->length_in_bytes -= bytes_used;

                if (device_request->length_in_bytes > 0) reply_data->more_data = connector_true;
                break;
        }
    }

    if (buffer != NULL)
    {
        APP_DEBUG("Device response data: send response data = %.*s\n", (int)bytes_used, buffer);

        ASSERT(reply_data->bytes_available >= bytes_used);

            /* let's copy the response data to service_response buffer */
        memcpy(reply_data->buffer, buffer, bytes_used);
        reply_data->bytes_used = bytes_used;

    }

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
            APP_DEBUG("app_process_device_request_status: handle %p session error %d\n",
                        (void *)device_request, status_data->status);
            break;
        }

        free(device_request);
    }

    return status;
}

connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id,
                                                      void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

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
            APP_DEBUG("app_data_service_handler: unknown request id type %d for connector_request_id_data_service\n", request_id);
            break;
    }

    return status;
}

