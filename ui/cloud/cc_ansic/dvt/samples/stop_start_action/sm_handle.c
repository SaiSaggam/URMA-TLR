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
#include <time.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"

#if (defined APP_USE_SM)
connector_callback_status_t app_sm_handler(connector_request_id_sm_t const request, void * const data)
{
#if (defined APP_USE_SM)
    connector_callback_status_t status = connector_callback_continue;

    UNUSED_ARGUMENT(request_data);
    UNUSED_ARGUMENT(request_length);
    UNUSED_ARGUMENT(response_length);

    switch (request)
    {
#if 0
        case connector_sm_total_length:
        {
            size_t * const length = response_data;
//            ds_record_t const * const app_ptr = request_data;

            *length = DS_DATA_SIZE; // app_ptr->file_length_in_bytes;
            APP_DEBUG("app_sm_handle: connector_sm_total_length %" PRIsize "\n", *length);
            break;
        }

        case connector_sm_session_complete:
        {
            connector_message_status_response_t const * const msg_resp = request_data;
            if (msg_resp->status != connector_success)
            {
                APP_DEBUG("connector_sm_session_complete with %d status\n", msg_resp->status);
                if (msg_resp->error_text != NULL) APP_DEBUG("error message %s\n", msg_resp->error_text);
            }

            APP_DEBUG("connector_sm_send_complete %p\n", msg_resp->user_context);
            if (msg_resp->user_context != NULL)
            {
                connector_request_id_data_service_t  * const request_id = (connector_request_id_data_service_t *)msg_resp->user_context;

                switch (*request_id)
                {
                    case connector_data_service_put_request:
                    {
                        ds_record_t * const user = (ds_record_t *)msg_resp->user_context;

                        if (put_file_active_count[user->header.transport] == 0)
                        {
                            ASSERT(0);
                        }
                        put_file_active_count[user->header.transport]--;
                        app_os_free((void *)msg_resp->user_context);
                        break;
                    }
                    case connector_data_service_device_request:
                        break;
                    default:
                        break;
                }
            }
            break;
        }
#endif
        case connector_sm_opaque_response:
        {
            connector_sm_opaque_response_t * const response = response_data;

            APP_DEBUG("Received %" PRIsize " opaque bytes on id %d\n", response->bytes, response->id);
            break;
        }
#if (defined CONNECTOR_SM_CLI)
        case connector_sm_cli_request:
            break;

        case connector_sm_cli_response:
            break;
#endif

        case connector_sm_more_data:  /* battery operated device */
            APP_DEBUG("connector_sm_more_data\n");
            break;

#if (defined CONNECTOR_TRANSPORT_SMS)
        case connector_request_id_sm_config_request:
        case connector_request_id_sm_config_response:
            break;
#endif

        default:
            APP_DEBUG("Request not supported in this sample: %d\n", request);
            status = connector_callback_unrecognized;
            break;

    }
#else
    connector_callback_status_t status = connector_callback_unrecognized;

    UNUSED_ARGUMENT(request);
    UNUSED_ARGUMENT(request_data);
    UNUSED_ARGUMENT(request_length);
    UNUSED_ARGUMENT(response_data);
    UNUSED_ARGUMENT(response_length);

#endif

    return status;
}
#endif

