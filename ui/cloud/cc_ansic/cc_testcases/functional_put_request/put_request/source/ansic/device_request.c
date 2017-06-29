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

/* Import the test functions to execute in the test cases list */
extern connector_status_t app_test_case_put_request_send_option_overwrite(char * data_buffer, size_t length_in_bytes, char const * target);
extern connector_status_t app_test_case_put_request_send_option_append(char * data_buffer, size_t length_in_bytes, char const * target);
extern connector_status_t app_test_case_put_request_send_option_archive(char * data_buffer, size_t length_in_bytes, char const * target);



/* Internal test structure to save and manage the device request */
typedef struct device_request_handle {
    char * payload;                       // Contains the data that will be sent in the response
    char const * target;                  // Contains the target
    size_t length_in_bytes;               // Stores the received target
} device_request_handle_t;

/* List of known targets for this test case */
static char * const device_request_targets[] = {
    "Data point",
    "test_put_request_send_option_overwrite",
    "test_put_request_send_option_append",
    "test_put_request_send_option_archive",
    "test_put_request_busy_state",
    "test_put_request_cancel_at_start",
    "test_put_request_cancel_in_middle_first_call",
    "test_put_request_cancel_in_middle_final_call",
    "test_put_request_timeout"
};
/* Internal test structure to create a list of arguments obtained from the device request payload */
typedef struct parsed_payload{
    char * element;
    void * next;
} parsed_payload_t;


/* Auxiliar functions to manage the payload */
parsed_payload_t * getParsedPayload(const char * payload, char delimiter);
void freeQueue(parsed_payload_t * queue);











/* This function is called only one time for each target (First step) */
static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const target_data)
{
    connector_callback_status_t status = connector_callback_continue;

    APP_DEBUG("Device request data: target = \"%s\"\n", target_data->target);

    device_request_handle_t * device_request = target_data->user_context;

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
    for(unsigned int i = 0; i < ARRAY_SIZE(device_request_targets); i++){

        if(strcmp(target_data->target, device_request_targets[i]) == 0){
            /* If it is a known target, fill the test structure */
            APP_DEBUG("Target data: %s\n", target_data->target);
            device_request->length_in_bytes = 0;
            device_request->payload = NULL;
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

    if(device_request->target != NULL)
    {
        /*
        * Next lines process the packet data and add them to "device_request->payload"
        */
        device_request->length_in_bytes += receive_data->bytes_used;
        device_request->payload = realloc(device_request->payload, device_request->length_in_bytes);
        memcpy(device_request->payload + device_request->length_in_bytes - receive_data->bytes_used, receive_data->buffer, receive_data->bytes_used);

        /* No more chunk */
        /* Sets bytes_sent counter to 0 and resets the firstPacket variable */
        if (!receive_data->more_data)
        {
            /* ONLY FOR DEBUG */
            /*APP_DEBUG("Total Payload received = \"%.*s\" for %s\n", (unsigned int)device_request->length_in_bytes, device_request->payload, device_request->target);*/
            APP_DEBUG("Total Payload received with length = \"%d\" for %s\n", (unsigned int)device_request->length_in_bytes, device_request->target);
        }
    }
    else
    {
        status = connector_callback_error;

        if (device_request != NULL && device_request->payload != NULL)
        {
            free(device_request->payload);
            device_request->payload = NULL;
        }
    }

    return status;
}



/* This function is called several times, one for each packet to send in the response */
static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_data)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const device_request = reply_data->user_context;
    size_t bytes_used = 0;
    void const * buffer = NULL;

    if(device_request->target == NULL)
    {
        // If the target isn't supported, sends the message below
        static char const unsupported_target_text[] = "Target is not supported";
        buffer = unsupported_target_text;
        bytes_used = sizeof unsupported_target_text -1;
    }
    else
    {
        static char const request_successful[] = "Launch successful";
        buffer = request_successful;
        bytes_used = sizeof request_successful -1;
    }

    /* Send the buffer content */
    ASSERT(reply_data->bytes_available >= bytes_used);

    /* let's copy the response data to service_response buffer */
    memcpy(reply_data->buffer, buffer, bytes_used);
    reply_data->bytes_used = bytes_used;


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
                    (void *) status_data->user_context);
            break;
        case connector_data_service_status_session_error:
            APP_DEBUG("app_process_device_request_status: handle %p session error %d\n",
                    (void *) status_data->user_context, status_data->session_error);
            break;
        default:
            APP_DEBUG("app_process_device_request_status: handle %p session error %d\n",
                        (void *)status_data->user_context, status_data->status);
            break;
        }
    }

    return status;
}






connector_callback_status_t app_data_service_device_request_handler(connector_request_id_data_service_t const request_id, void * const data)
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
            /* Call to manage the request status */
            status = app_process_device_request_status(data);

            /******************************************************/
            /*** Manage the actions requested by Device Request ***/
            /******************************************************/
            device_request_handle_t * const device_request = ((connector_data_service_status_t*)data)->user_context;

            if(device_request->target != NULL){
                /* If the target is supported, manage the payload and do the corresponding action */
//                 char target[100];
//                 sprintf(target, "%s", device_request->target);
                parsed_payload_t * parsedPayload = getParsedPayload(device_request->payload, ';');

                APP_DEBUG("Launch test for target '%s'\n", device_request->target);

                if(strcmp(device_request->target, "Data point") == 0){
                    /* If the target is Data point, we know that there are 2 elements in the string */
                    parsed_payload_t * auxiliar = parsedPayload;
                    char const * first = auxiliar->element;
                    int streams = atoi(first); /* Obtain an Integer from a char[] */

                    auxiliar = auxiliar->next;
                    char const * second = auxiliar->element;
                    int points = atoi(second); /* Obtain an Integer from a char[] */

                    APP_DEBUG("Data point with '%d' points and '%d' streams\n", points,streams);

                }
                /* Test cases from test_put_request_positive.py */
                else if(strcmp(device_request->target, "test_put_request_send_option_overwrite") == 0)
                {
                    app_test_case_put_request_send_option_overwrite(device_request->payload, device_request->length_in_bytes, device_request->target);
                }
                else if(strcmp(device_request->target, "test_put_request_send_option_append") == 0)
                {
                    app_test_case_put_request_send_option_append(device_request->payload, device_request->length_in_bytes, device_request->target);
                }
                else if(strcmp(device_request->target, "test_put_request_send_option_archive") == 0)
                {
                    app_test_case_put_request_send_option_archive(device_request->payload, device_request->length_in_bytes, device_request->target);
                }
                /* Test cases from test_put_request_force_errors.py */
                else if(strcmp(device_request->target, "test_put_request_busy_state") == 0)
                {
                    app_test_case_put_request_send_option_overwrite(device_request->payload, device_request->length_in_bytes, device_request->target);
                }
                else if(strcmp(device_request->target, "test_put_request_cancel_at_start") == 0)
                {
                    app_test_case_put_request_send_option_overwrite(device_request->payload, device_request->length_in_bytes, device_request->target);
                }
                else if(strcmp(device_request->target, "test_put_request_cancel_in_middle_first_call") == 0)
                {
                    app_test_case_put_request_send_option_overwrite(device_request->payload, device_request->length_in_bytes, device_request->target);
                }
                else if(strcmp(device_request->target, "test_put_request_cancel_in_middle_final_call") == 0)
                {
                    app_test_case_put_request_send_option_overwrite(device_request->payload, device_request->length_in_bytes, device_request->target);
                }
                else if(strcmp(device_request->target, "test_put_request_timeout") == 0)
                {
                    app_test_case_put_request_send_option_overwrite(device_request->payload, device_request->length_in_bytes, device_request->target);
                }



                /* Release the memory allocated for the queue */
                freeQueue(parsedPayload);
            }


            /* Free internal structure for device request */
            if(device_request != NULL){
//                 free(device_request->payload);
                free(device_request);
            }
            else{
                APP_DEBUG("device_request is null");
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

/* Creates a queue that contains each element between delimiters */
/* Returns a pointer to the first element of the queue */
parsed_payload_t * getParsedPayload(const char * payload, char delimiter){

    parsed_payload_t * head = malloc(sizeof(parsed_payload_t));
    parsed_payload_t * auxiliar = head;
    int elementSize = 0;
    unsigned int i = 0;

    while(i<strlen(payload)){

        if(payload[i] != delimiter){
            /* Element size stores how many characters have been found until the appearance of the delimiter */
            elementSize++;
        }
        else{
            /* When the delimiter is found, stores the value of the element and add a new item to the queue */
            auxiliar->element = malloc(elementSize+1);
            snprintf(auxiliar->element, elementSize+1, "%s", payload+i-elementSize);
            auxiliar->next = malloc(sizeof(parsed_payload_t));
            auxiliar = auxiliar->next;
            elementSize = 0;
        }
        i++;
    }

    // Last element. Set null the pointer to the next element
    auxiliar->element = malloc(elementSize+1);
    snprintf(auxiliar->element, elementSize+1, "%s", payload+i-elementSize);
    auxiliar->next = NULL;

    return head;
}

void freeQueue(parsed_payload_t * queue){

    parsed_payload_t * auxiliar = queue;

    while(auxiliar != NULL){
        free(auxiliar->element);
        auxiliar = auxiliar->next;
    }
    free(queue);
    queue = NULL;
}


