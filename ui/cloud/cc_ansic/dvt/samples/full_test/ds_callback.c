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
#include "connector_dvt.h"

connector_status_t send_put_request(connector_handle_t handle, dvt_ds_t * const ds_info)
{
    connector_status_t status = connector_success;
    static char file_type[] = "text/plain";
    connector_request_data_service_send_t * header = NULL;

    ds_info->bytes_sent = 0;
    header = &ds_info->header;
    APP_DEBUG("Sending %s of length %" PRIsize "\n", ds_info->file_path, (ds_info->test_case != dvt_case_put_request_ds_zero) ? ds_info->file_size : 0);
    switch (ds_info->test_case)
    {
    case dvt_case_put_request_ds_append:
        header->option = connector_data_service_send_option_append;
        break;

    case dvt_case_put_request_ds_archive:
        header->option = connector_data_service_send_option_archive;
        break;

    case dvt_case_put_request_ds_both:
        header->option = connector_data_service_send_option_transient;
        break;

    default:
        header->option = connector_data_service_send_option_overwrite;
        break;
    }

    header->path  = ds_info->file_path;
    header->content_type = file_type;
    header->user_context = ds_info;
    header->transport = connector_transport_tcp;

    status = connector_initiate_action(handle, connector_initiate_send_data, header);
    if (status == connector_success)
        ds_info->state = dvt_state_request_progress;
    APP_DEBUG("send_put_request: %s status  %d\n", ds_info->file_path, status);

    return status;
}

connector_callback_status_t app_put_request_handler(connector_request_id_data_service_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    if (data == NULL)
    {
        APP_DEBUG("app_put_request_handler: Callback has NULL data\n");
        goto done;
    }

    switch (request_id)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * const send_ptr = data;
            dvt_ds_t * const ds_info = send_ptr->user_context;

            ASSERT(ds_info != NULL);
            switch (ds_info->test_case)
            {
                case dvt_case_put_request_ds_busy:
                {
                    static bool first_time = true;

                    if (first_time)
                    {
                        APP_DEBUG("busy\n");
                        status = connector_callback_busy;
                        first_time = false;
                        goto done;
                    }

                    break;
                }

                case dvt_case_put_request_ds_zero:
                    send_ptr->more_data = connector_false;
                    send_ptr->bytes_used = 0;
                    ds_info->state = dvt_state_request_complete;
                    goto done;

                case dvt_case_put_request_ds_cancel_start:
                    ds_info->state = dvt_state_request_error;
                    status = connector_callback_error;
                    goto done;

                default:
                    break;
            }

            send_ptr->bytes_used = ds_info->file_size - ds_info->bytes_sent;
            if (send_ptr->bytes_used > send_ptr->bytes_available)
                send_ptr->bytes_used = send_ptr->bytes_available;

            memcpy(send_ptr->buffer, &ds_info->file_buf[ds_info->bytes_sent], send_ptr->bytes_used);
            ds_info->bytes_sent += send_ptr->bytes_used;
            if (ds_info->bytes_sent >= ds_info->file_size)
            {
                switch (ds_info->test_case)
                {
                    case dvt_case_put_request_ds_timeout:
                        status = connector_callback_busy;
                        break;

                    case dvt_case_put_request_ds_cancel_middle:
                        ds_info->state = dvt_state_request_error;
                        status = connector_callback_error;
                        break;

                    default:
                        send_ptr->more_data = connector_false;
                        ds_info->state = dvt_state_request_complete;
                        break;
                }
            }
            else
                send_ptr->more_data = connector_true;

            break;
        }

        case connector_request_id_data_service_send_response:
        {
            connector_data_service_send_response_t * const resp_ptr = data;
            dvt_ds_t * const ds_info = resp_ptr->user_context;

            ASSERT(ds_info != NULL);
            if (ds_info->state != dvt_state_request_complete)
            {
                APP_DEBUG("connector_put_request_callback: got response before complete\n");
                ds_info->state = dvt_state_request_error;
                status = connector_callback_error;
                break;
            }

            APP_DEBUG("Received response for put %s: Received %s[%d] from Cloud\n", ds_info->file_path,
                      ((resp_ptr->response == connector_data_service_send_response_success) ? "success" : "error"), resp_ptr->response);
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Cloud response [%s]: %s\n", ds_info->file_path, resp_ptr->hint);
            }
            break;
        }

        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t * error_ptr = data;
            dvt_ds_t * const ds_info = error_ptr->user_context;

            ASSERT(ds_info != NULL);
            switch(error_ptr->status)
            {
                case connector_data_service_status_complete:
                    APP_DEBUG("Put request complete for %s\n", ds_info->file_path);
                    ds_info->state = dvt_state_response_complete;
                    break;
                default:
                    APP_DEBUG("Error response %d for %s\n", error_ptr->status, ds_info->file_path);
                    ds_info->state = dvt_state_request_error;
                    status = connector_callback_error;
                    break;
            }
            break;
        }

        default:
            APP_DEBUG("DvtError: Unexpected message type: %d\n", request_id);
            break;
    }

done:
    return status;
}

/* supported targets */
typedef enum
{
    dvt_case_ds_valid_target,
    dvt_case_ds_invalid_target,
    dvt_case_ds_cancel_request,
    dvt_case_ds_cancel_in_middle,
    dvt_case_ds_cancel_response,
    dvt_case_ds_busy_response,
    dvt_case_ds_busy_request,
    dvt_case_ds_not_handle,
    dvt_case_ds_zero_byte,
    dvt_case_ds_timeout_response,
    dvt_case_ds_start_put_request,
    dvt_case_ds_query_put_request,
    dvt_case_ds_last
} dvt_case_ds_t;

typedef struct device_request_handle
{
    dvt_case_ds_t test_case;
    char * target;
    size_t length_in_bytes;
    char * data;
    size_t bytes_sent;
} device_request_handle_t;

static device_request_handle_t device_request_targets[] =
{
    {dvt_case_ds_valid_target, "valid target", 0, NULL, 0},
    {dvt_case_ds_invalid_target, "invalid target", 0, NULL, 0},
    {dvt_case_ds_cancel_request, "cancel request", 0, NULL, 0},
    {dvt_case_ds_cancel_in_middle, "cancel in middle", 0, NULL, 0},
    {dvt_case_ds_cancel_response, "cancel response", 0, NULL, 0},
    {dvt_case_ds_busy_request, "busy request", 0, NULL, 0},
    {dvt_case_ds_busy_response, "busy response", 0, NULL, 0},
    {dvt_case_ds_not_handle, "not handle", 0, NULL, 0},
    {dvt_case_ds_zero_byte, "zero byte data", 0, NULL, 0},
    {dvt_case_ds_timeout_response, "timeout response", 0, NULL, 0},
    {dvt_case_ds_start_put_request, "start put request", 0, NULL, 0},
    {dvt_case_ds_query_put_request, "query put request", 0, NULL, 0}
};

#define DVT_DS_MAX_DATA_SIZE  0x8000

static void start_put_request(device_request_handle_t * target_info, uint8_t const * const data_path, size_t const path_bytes)
{
    dvt_ds_t * const ds_info = &data_service_info;

    if (ds_info->state != dvt_state_init)
    {
        APP_DEBUG("start_put_request: unexpected state %d for %s\n", ds_info->state, ds_info->file_path);
        goto done;
    }

    if (path_bytes >= sizeof ds_info->file_path)
    {
        APP_DEBUG("start_put_request: unexpected data length %" PRIsize "\n", path_bytes);
        goto done;
    }


    memcpy(ds_info->file_path, data_path, path_bytes);
    ds_info->file_path[path_bytes] = '\0';
    ds_info->test_case = atoi(&ds_info->file_path[strlen("test/dvt_ds")]);
    target_info->data = ds_info->file_path;

done:
    return;
}

static void prepare_put_request_response(device_request_handle_t * target_info, connector_data_service_receive_reply_data_t * const reply)
{
    dvt_ds_t * const ds_info = &data_service_info;
    static char const ds_error[] = "error";
    char const * resp = ds_error;

    switch (target_info->test_case)
    {
    case dvt_case_ds_start_put_request:
        resp = ds_info->file_path;
        ds_info->state = dvt_state_request_start;
        break;

    case dvt_case_ds_query_put_request:
        {
            switch (ds_info->state)
            {
            case dvt_state_request_start:
            case dvt_state_request_progress:
            case dvt_state_request_complete:
                {
                    static char const ds_wait[] = "wait";

                    resp = ds_wait;
                }
                break;

            case dvt_state_response_complete:
                {
                    static char const ds_done[] = "done";

                    resp = ds_done;
                    ds_info->state = dvt_state_init;
                }
                break;

            default:
                ds_info->state = dvt_state_init;
                break;
            }
        }
        break;

    default:
        ds_info->state = dvt_state_init;
        break;
    }

    APP_DEBUG("%s: Sending response [%s] state[%d]\n", ds_info->file_path, resp, ds_info->state);
    reply->bytes_used = strlen(resp);
    ASSERT(reply->bytes_used < reply->bytes_available);
    memcpy(reply->buffer, resp, reply->bytes_used);
}

static device_request_handle_t * get_test_case_record (char const * const target)
{
    dvt_case_ds_t test_case;
    device_request_handle_t * record = NULL;

    for (test_case = dvt_case_ds_valid_target; test_case < dvt_case_ds_last; test_case++)
    {
        if (!strcmp(target, device_request_targets[test_case].target))
        {
            record = &device_request_targets[test_case];
            break;
        }
    }

    return record;
}

static connector_callback_status_t app_process_device_request_data(connector_data_service_receive_data_t * const data_ptr)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * target_info = data_ptr->user_context;

    if (target_info == NULL)
    {
        APP_DEBUG("app_process_device_request_data: couldn't find the target\n");
        goto cancel;
    }

    switch (target_info->test_case)
    {
        case dvt_case_ds_invalid_target:
            goto cancel;

        case dvt_case_ds_cancel_request:
            if (target_info->length_in_bytes > 0)
                goto cancel;
            break;

        case dvt_case_ds_busy_response:
        {
            static bool first_time = true;

            if (first_time && (target_info->length_in_bytes > 0))
            {
                APP_DEBUG("busy\n");
                status = connector_callback_busy;
                first_time = false;
                goto done;
            }

            break;
        }

        case dvt_case_ds_start_put_request:
        {
            dvt_ds_t * const ds_info = &data_service_info;

            start_put_request(target_info, data_ptr->buffer, data_ptr->bytes_used);
            APP_DEBUG("Received start %s\n", ds_info->file_path);

            goto done;
        }

        case dvt_case_ds_query_put_request:
        {
            dvt_ds_t * const ds_info = &data_service_info;

            APP_DEBUG("Received query %s\n", ds_info->file_path);
            if (memcmp(data_ptr->buffer, ds_info->file_path, data_ptr->bytes_used))
            {
                APP_DEBUG("Expected/received %s mismatch\n", (char *)data_ptr->buffer);
                ds_info->state = dvt_state_request_error;
            }

            goto done;
        }

        default:
            break;
    }

    if (target_info->data == NULL)
    {
        target_info->data = malloc(DVT_DS_MAX_DATA_SIZE);
        if (target_info->data == NULL)
        {
            APP_DEBUG("process_device_request: malloc failed %s\n", target_info->target);
            goto cancel;
        }
    }

    if ((data_ptr->bytes_used > 0) && ((target_info->length_in_bytes + data_ptr->bytes_used) < DVT_DS_MAX_DATA_SIZE))
    {
        memcpy(&target_info->data[target_info->length_in_bytes], data_ptr->buffer, data_ptr->bytes_used);
        target_info->length_in_bytes += data_ptr->bytes_used;
    }

    goto done;

cancel:
    status = connector_callback_error;

    if (target_info != NULL && target_info->data != NULL)
    {
        free(target_info->data);
        target_info->data = NULL;
    }

done:
    return status;
}

static connector_callback_status_t app_process_device_request_response(connector_data_service_receive_reply_data_t * const data_ptr)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const target_info = data_ptr->user_context;

    ASSERT(target_info != NULL);

    switch (target_info->test_case)
    {
        case dvt_case_ds_busy_response:
        {
            static bool first_time = true;

            if (first_time && (target_info->bytes_sent > 0))
            {
                APP_DEBUG("busy\n");
                status = connector_callback_busy;
                first_time = false;
                goto done;
            }
        }
        break;

        case dvt_case_ds_cancel_in_middle:
            goto cancel;

        case dvt_case_ds_cancel_response:
            if (target_info->bytes_sent > target_info->length_in_bytes/2)
                goto cancel;
            break;

        case dvt_case_ds_timeout_response:
            if (target_info->bytes_sent > target_info->length_in_bytes/2)
            {
                status = connector_callback_busy;
                goto done;
            }
            break;

        case dvt_case_ds_not_handle:
            goto cancel;

        case dvt_case_ds_start_put_request:
        case dvt_case_ds_query_put_request:
            prepare_put_request_response(target_info, data_ptr);
            data_ptr->more_data = connector_false;
            goto done;

        default:
            break;
    }

    if (target_info->data != NULL)
    {
        data_ptr->bytes_used = target_info->length_in_bytes - target_info->bytes_sent;
        if (data_ptr->bytes_used > data_ptr->bytes_available)
            data_ptr->bytes_used =  data_ptr->bytes_available;

        /* let's copy the response data to service_response buffer */
        if (data_ptr->bytes_used > 0)
        {
            memcpy(data_ptr->buffer, &target_info->data[target_info->bytes_sent], data_ptr->bytes_used);
            target_info->bytes_sent += data_ptr->bytes_used;
        }
        else
            APP_DEBUG("Sending 0 bytes\n");

        if (target_info->bytes_sent < target_info->length_in_bytes)
            data_ptr->more_data = connector_true;
    }

    goto done;

cancel:
    status = connector_callback_error;
done:
    return status;
}

static connector_callback_status_t app_process_device_request_target(connector_data_service_receive_target_t * const data_ptr)
{
    connector_callback_status_t status = connector_callback_continue;
    device_request_handle_t * const target_info = get_test_case_record(data_ptr->target);

    if (target_info != NULL)
    {
        target_info->bytes_sent = 0;
        target_info->length_in_bytes = 0;
        data_ptr->user_context = target_info;
        APP_DEBUG("process_device_request_target: receive target %s\n", data_ptr->target);
    }
    else
    {
        APP_DEBUG("process_device_request_target: unknown target %s\n", data_ptr->target);
        status = connector_callback_error;
    }

    return status;
}

static connector_callback_status_t app_process_device_request_status(connector_data_service_status_t const * const status_data)
{
    connector_callback_status_t status = connector_callback_continue;

    device_request_handle_t * const target_info = status_data->user_context;


    switch (status_data->status)
    {
    case connector_data_service_status_complete:
        APP_DEBUG("app_process_device_request_status: session completed\n");
        break;
    case connector_data_service_status_session_error:
        APP_DEBUG("app_process_device_request_status: session error %d\n",
                    status_data->session_error);
        break;
    default:
        APP_DEBUG("app_process_device_request_status: error %d\n",
                    status_data->status);
        break;
    }

    if (target_info->test_case != dvt_case_ds_start_put_request && target_info != NULL && target_info->data != NULL)
    {
        free(target_info->data);
        target_info->data = NULL;
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


connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id, void * const data)
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
