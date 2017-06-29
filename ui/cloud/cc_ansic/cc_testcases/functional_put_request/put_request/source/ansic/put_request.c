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

#define SEND_DATA_SIZE  64

/* Import function to manage the device request calls */
extern connector_callback_status_t app_data_service_device_request_handler(connector_request_id_data_service_t const request_id, void * const data);

/* Import the pointer to the connector handler instance */
extern connector_handle_t CLOUD_HANDLER;

typedef struct
{
    char const * data_ptr;
    size_t bytes;
    char const * file_path;
    char const * target;
} client_data_t;





/**** TEST_CASE OPTION OVERWRITE *****/
connector_status_t app_test_case_put_request_send_option_overwrite(char * data_buffer, size_t length_in_bytes, char const * target)
{
    connector_status_t status = connector_no_resource;
    static connector_request_data_service_send_t header; /* Cloud Connector will hold this until we get a response/error callback */
    static char const file_path[] = "test/test_overwrite.txt";
    static char const file_type[] = "text/plain";

    /* Create an internal structure with the file content */
    client_data_t * const app_data = malloc(sizeof *app_data);

    if (app_data == NULL)
        goto done;

    app_data->data_ptr = data_buffer; /* Save the payload to use in the test case */
    app_data->bytes = length_in_bytes; /* total length in bytes of the payload */
    app_data->file_path = file_path; /* Save the file path where the file will be uploaded */
    app_data->target = target; /* Save the device request target to known the test case */


    /* Compound the structure to send the file to Device Cloud */
    header.transport = connector_transport_tcp;
    header.option = connector_data_service_send_option_overwrite;
    header.path  = file_path;
    header.content_type = file_type;
    header.user_context = app_data; /* will be returned in all subsequent callbacks */


    /* Call to connector to start the send data action */
    status = connector_initiate_action(CLOUD_HANDLER, connector_initiate_send_data, &header);
    if (status == connector_init_error)
    {
        static int print_once = 1;

        if (print_once)
            print_once = 0;
        else
            goto error;
    }

    APP_DEBUG("Status: %d, file: %s\n", status, file_path);
    if (status == connector_success)
        goto done;

error:
    free(app_data);

done:
    return status;
}



/**** TEST_CASE OPTION APPEND *****/
connector_status_t app_test_case_put_request_send_option_append(char * data_buffer, size_t length_in_bytes, char const * target)
{
    connector_status_t status = connector_no_resource;
    static connector_request_data_service_send_t header; /* Cloud Connector will hold this until we get a response/error callback */
    static char const file_path[] = "test/test_append.txt";
    static char const file_type[] = "text/plain";

    /* Create an internal structure with the file content */
    client_data_t * const app_data = malloc(sizeof *app_data);

    if (app_data == NULL)
        goto done;

    app_data->data_ptr = data_buffer; /* Save the payload to use in the test case */
    app_data->bytes = length_in_bytes; /* total length in bytes of the payload */
    app_data->file_path = file_path; /* Save the file path where the file will be uploaded */
    app_data->target = target; /* Save the device request target to known the test case */


    /* Compound the structure to send the file to Device Cloud */
    header.transport = connector_transport_tcp;
    header.option = connector_data_service_send_option_append;
    header.path  = file_path;
    header.content_type = file_type;
    header.user_context = app_data; /* will be returned in all subsequent callbacks */


    /* Call to connector to start the send data action */
    status = connector_initiate_action(CLOUD_HANDLER, connector_initiate_send_data, &header);
    if (status == connector_init_error)
    {
        static int print_once = 1;

        if (print_once)
            print_once = 0;
        else
            goto error;
    }

    APP_DEBUG("Status: %d, file: %s\n", status, file_path);
    if (status == connector_success)
        goto done;

error:
    free(app_data);

done:
    return status;
}




/**** TEST_CASE OPTION ARCHIVE *****/
connector_status_t app_test_case_put_request_send_option_archive(char * data_buffer, size_t length_in_bytes, char const * target)
{
    connector_status_t status = connector_no_resource;
    static connector_request_data_service_send_t header; /* Cloud Connector will hold this until we get a response/error callback */
    static char const file_path[] = "test/test_archive.txt";
    static char const file_type[] = "text/plain";

    /* Create an internal structure with the file content */
    client_data_t * const app_data = malloc(sizeof *app_data);

    if (app_data == NULL)
        goto done;

    app_data->data_ptr = data_buffer; /* Save the payload to use in the test case */
    app_data->bytes = length_in_bytes; /* total length in bytes of the payload */
    app_data->file_path = file_path; /* Save the file path where the file will be uploaded */
    app_data->target = target; /* Save the device request target to known the test case */


    /* Compound the structure to send the file to Device Cloud */
    header.transport = connector_transport_tcp;
    header.option = connector_data_service_send_option_archive;
    header.path  = file_path;
    header.content_type = file_type;
    header.user_context = app_data; /* will be returned in all subsequent callbacks */


    /* Call to connector to start the send data action */
    status = connector_initiate_action(CLOUD_HANDLER, connector_initiate_send_data, &header);
    if (status == connector_init_error)
    {
        static int print_once = 1;

        if (print_once)
            print_once = 0;
        else
            goto error;
    }

    APP_DEBUG("Status: %d, file: %s\n", status, file_path);
    if (status == connector_success)
        goto done;

error:
    free(app_data);

done:
    return status;
}














/****************************************************/
/****** MANAGE ALL CALLBACKS FROM DATA SERVICE ******/
/****************************************************/
connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        /*************************************************************/
        /*** Cases for manage the Device request from Device Cloud ***/
        /*************************************************************/
        case connector_request_id_data_service_receive_target:
        case connector_request_id_data_service_receive_data:
        case connector_request_id_data_service_receive_status:
        case connector_request_id_data_service_receive_reply_data:
        case connector_request_id_data_service_receive_reply_length:
        {
            status = app_data_service_device_request_handler(request_id, data);
            break;
        }


        /* Put request */
        case connector_request_id_data_service_send_data:
        {
//             APP_DEBUG("Entra en app_data_service_handler->connector_request_id_data_service_send_data...\n");
            /* Get the internal structure and the response structure */
            connector_data_service_send_data_t * send_ptr = data;
            client_data_t * const app_data = send_ptr->user_context;

            ASSERT(app_data != NULL);

            /****************************************************/
            /* Manage the different test cases to force errors  */
            /* before process the data, if not we skip this step */
            /****************************************************/

            if(strcmp(app_data->target, "test_put_request_busy_state") == 0)
            {
                /* This static integer only is initialized one time */
                static int busy_counter = 0;
                busy_counter++;

                /* Only the first 3 times we send a busy */
                if ( busy_counter < 4 )
                {
                    APP_DEBUG("test_put_request_busy_state: return a busy state...\n");
                    status = connector_callback_busy;
                    goto done;
                }

            }
            else if(strcmp(app_data->target, "test_put_request_cancel_at_start") == 0)
            {
                APP_DEBUG("test_put_request_cancel_at_start: return an error on the start...\n");
                status = connector_callback_error;
                goto done;

            }
            else if(strcmp(app_data->target, "test_put_request_timeout") == 0)
            {
                /* This static integer only is initialized one time */
                static int timeout_counter = 0;
                timeout_counter++;

                if ( timeout_counter > 10)
                {
                    if( (timeout_counter % 5000000) == 0 )
                    {
                        APP_DEBUG("test_put_request_timeout: return a busy state...\n");
                    }
                    status = connector_callback_busy;
                    goto done;
                }
                else
                {
                    /* We send some data chunks before start with the busy state */
                    APP_DEBUG("test_put_request_timeout: process the call, waiting for start the busy state...\n");
                }

            }






            /***************************************************/
            /* Proces an standard request without force errors */
            /***************************************************/

            /* Identify how many bytes we will send in this call */
            if (send_ptr->bytes_available > app_data->bytes)
            {
                send_ptr->bytes_used = app_data->bytes;
            }
            else
            {
                send_ptr->bytes_used = send_ptr->bytes_available;
            }

            /* Copy from the internal structure to the response buffer */
            memcpy(send_ptr->buffer, app_data->data_ptr, send_ptr->bytes_used);
            app_data->data_ptr += send_ptr->bytes_used;
            app_data->bytes -= send_ptr->bytes_used;

            /* Identify if we need to send more data */
            if( app_data->bytes > 0 )
            {
                send_ptr->more_data = connector_true;
            }
            else
            {
                send_ptr->more_data = connector_false;
            }





            /****************************************************/
            /* Manage the different test cases to force errors  */
            /* after process the data, if not we skip this step */
            /****************************************************/

            if(strcmp(app_data->target, "test_put_request_cancel_in_middle_first_call") == 0)
            {
                APP_DEBUG("test_put_request_cancel_in_middle_first_packet: return an error on the first call...\n");
                /* in the first call we send an error */
                status = connector_callback_error;
            }
            else if(strcmp(app_data->target, "test_put_request_cancel_in_middle_final_call") == 0)
            {
                if ( send_ptr->more_data )
                {
                    /* If it is not the last data chunk, we process it */
                    APP_DEBUG("test_put_request_cancel_in_middle_final_call: process the call, waiting for the last call...\n");
                }
                else
                {
                    /* If it is the last data chunk, we return an error */
                    APP_DEBUG("test_put_request_cancel_in_middle_final_call: return an error on the last call...\n");
                    status = connector_callback_error;
                }

            }


            break;
        }

        case connector_request_id_data_service_send_response:
        {
            APP_DEBUG("Entra en app_data_service_handler->connector_request_id_data_service_send_response...\n");
            connector_data_service_send_response_t * const resp_ptr = data;

            APP_DEBUG("Received %s response from Device Cloud\n", (resp_ptr->response == connector_data_service_send_response_success) ? "success" : "error");
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Device Cloud response %s\n", resp_ptr->hint);
            }

            break;
        }

        case connector_request_id_data_service_send_status:
        {
            APP_DEBUG("Entra en app_data_service_handler->connector_request_id_data_service_send_status...\n");
            connector_data_service_status_t * const error_ptr = data;
            client_data_t * const app_data = error_ptr->user_context;

            APP_DEBUG("Data service error: %d\n", error_ptr->status);
            if ( error_ptr->status == 0 )
            {
                APP_DEBUG("Data service for '%s' was successfully\n", app_data->file_path);
            }

            free(app_data);
            break;
        }

        default:
            APP_DEBUG("Unexpected command: %d\n", request_id);
            break;
    }

done:
    return status;
}

