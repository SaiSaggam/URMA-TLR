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
#include "connector_api.h"
#include "platform.h"
#include <unistd.h>

/* List of supported targets */
static char * const device_request_targets[] = {
    "myTarget",
    "cancel request",
    "cancel in the middle",
    "cancel response",
    "busy request",
    "busy response",
    "timeout response"
};

/* Internal test structure to save and manage the device request */
typedef struct device_request_handle {
    char * response_data;                       // Contains the data that will be sent in the response
    char const * target;                        // Contains the target
    size_t length_in_bytes;                     // Stores the received target
    size_t bytes_sent;                          // Counter for bytes sent. Starts in 0 and increases in each response iteration
} device_request_handle_t;






/* This function is called only one time for each target (First step) */
static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_data)
{
    connector_callback_status_t status = connector_callback_continue;
    /* For each target we create a new Test structure */
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
            device_request->response_data = NULL;
            target_data->user_context = ptr;
        }
    }

    /* check for supported target */
    for(unsigned int i = 0; i < ARRAY_SIZE(device_request_targets); i++){

        if(strcmp(target_data->target, device_request_targets[i]) == 0){
            /* If it is a known target, fill the test structure */
            APP_DEBUG("Target data: %s\n", target_data->target);
            device_request->length_in_bytes = 0;
            device_request->target = device_request_targets[i];
            goto done;
        }
    }

    /* Target not supported, so assign device_request_unsupported_target and send an error */
    device_request->target = NULL;
    status = connector_callback_error;

done:
    return status;
}





/* This function is called several times, one for each received packet */
static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const receive_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const device_request = receive_data->user_context;

    ASSERT(device_request != NULL);

    if(device_request->target == NULL){
        // Don't process the payload
        goto cancel;
    }

    else if(strcmp(device_request->target, "cancel request") == 0){

        if(device_request->length_in_bytes > 0)
        // Once the connector has started to process the payload, cancel it
        goto cancel;
    }

    else if(strcmp(device_request->target, "busy request") == 0){

        // Forces the connector to stay busy for a moment during processing the payload
        static connector_bool_t first_time = connector_true;
        if (first_time && device_request->length_in_bytes > 0)
        {
            APP_DEBUG("Connector busy. Please wait...\n");
            status = connector_callback_busy;
            first_time = connector_false;
            goto done;
        }
    }



    /*
    * Next lines process the packet data and add them to "device_request->response_data"
    */
    device_request->length_in_bytes += receive_data->bytes_used;
    if ( device_request->length_in_bytes > 0 )
    {   /* Only if we have new data to append we do it */
        device_request->response_data = realloc(device_request->response_data, device_request->length_in_bytes);
        memcpy(device_request->response_data + device_request->length_in_bytes - receive_data->bytes_used, receive_data->buffer, receive_data->bytes_used);
    }

    /* No more chunks */
    /* Sets bytes_sent counter to 0 */
    if (!receive_data->more_data)
    {
        device_request->bytes_sent = 0;
        APP_DEBUG("Total Payload received = \"%.*s\" for %s\n", (unsigned int)device_request->length_in_bytes, device_request->response_data, device_request->target);
    }
    goto done;

cancel:
    status = connector_callback_error;

    if (device_request != NULL && device_request->response_data != NULL)
    {
        free(device_request->response_data);
        device_request->response_data = NULL;
    }

done:
    return status;
}



/* This function is called several times, one for each packet to send in the response */
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
        goto done;
    }
    else
    {

        /* Take different actions for each target */
        if(device_request->target == NULL){
            // If the target isn't supported, sends the message below
            static char const unsupported_target_text[] = "Target is not supported";
            buffer = unsupported_target_text;
            bytes_used = sizeof unsupported_target_text -1;
            goto send;
        }

        else if(strcmp(device_request->target, "cancel in the middle") == 0){
            // If the target is "cancel in the middle", simply cancels the response
            goto cancel;
        }

        else if(strcmp(device_request->target, "busy response") == 0){
            // Forces the connector to stay busy for a moment during processing the response
            static connector_bool_t first_time = connector_true;
            if (first_time && device_request->bytes_sent > device_request->length_in_bytes/2)
            {
                APP_DEBUG("Connector busy. Wait till response is sent...\n");
                status = connector_callback_busy;
                first_time = connector_false;
                goto done;
            }
        }

        else if(strcmp(device_request->target, "cancel response") == 0){
            // If the target is "cancel response", cancels the response once it's processed more than half of the data
            if (device_request->bytes_sent > device_request->length_in_bytes/2)
                goto cancel;
        }

        else if(strcmp(device_request->target, "timeout response") == 0){
            // Forces the connector to stay busy till timeout is reached
            if (device_request->bytes_sent > device_request->length_in_bytes/2)
            {
                status = connector_callback_busy;
                goto done;
            }
        }




        if(device_request->response_data != NULL)
        {
            /* Gets the number of bytes that will be sent in the current iteration */
            /* If bytes_available in response writer < bytes left in the response, send the bytes available */
            if (reply_data->bytes_available <  (device_request->length_in_bytes - device_request->bytes_sent) )
            {
                bytes_used = reply_data->bytes_available;
            }
            else
            {
                bytes_used = device_request->length_in_bytes - device_request->bytes_sent;
            }

            /* response_data pointer goes forward the number of bytes sent in the iteration */
            buffer = device_request->response_data + device_request->bytes_sent;

            /* update the internal counter for the bytes sent */
            device_request->bytes_sent += bytes_used;

            /* if it is not the last chunk enable the flag for more_data */
            if (device_request->bytes_sent < device_request->length_in_bytes)
            {
                reply_data->more_data = connector_true;
            }
        }
    }

send:
    if (buffer != NULL)
    {
        /* Print the buffer content */
        //APP_DEBUG("Device response data: send response data = %.*s\n", (int)bytes_used, buffer);

        ASSERT(reply_data->bytes_available >= bytes_used);

        /* let's copy the response data to service_response buffer */
        memcpy(reply_data->buffer, buffer, bytes_used);
        reply_data->bytes_used = bytes_used;
    }

    goto done;


cancel:
    status = connector_callback_error;
done:
    return status;
}




/* this function is called at the final of the process */
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

            connector_data_service_status_t * const data_ptr = data;
            device_request_handle_t * const device_request = data_ptr->user_context;

            if(device_request != NULL){
                if(device_request->response_data != NULL){
                    free(device_request->response_data);
                    device_request->response_data = NULL;
                }
                free(device_request);
            }
            else{
                APP_DEBUG("NULL POINTER in internal device_request structure!!!\n");
            }
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
