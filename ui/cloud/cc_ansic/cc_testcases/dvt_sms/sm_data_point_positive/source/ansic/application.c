/*
 * Copyright (c) 2012, 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "connector_api.h"
#include "platform.h"

extern connector_callback_status_t app_data_point_handler(connector_request_id_data_service_t const request, void  * const data);
extern connector_callback_status_t app_sm_handler(connector_request_id_sm_t const request, void  * const data);
extern connector_status_t app_send_data_point(connector_handle_t handle);
extern connector_bool_t app_dp_test_running(void);

connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    connector_bool_t type;

    UNUSED_ARGUMENT(class_id);

    switch (status)
    {
           /* if either Device Cloud or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
        case connector_close_status_device_stopped:
        case connector_close_status_abort:
             type = connector_false;
             break;

       /* otherwise it's an error and we want to retry */
       default:
             type = connector_true;
             break;
    }

    return type;
}

connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
                                                   connector_request_id_t const request_id,
                                                   void * const data, void * const context)
{
    connector_callback_status_t   status = connector_callback_unrecognized;

    UNUSED_ARGUMENT(context);

    switch (class_id)
    {
    case connector_class_id_config:
        status = app_config_handler(request_id.config_request, data);
        break;

    case connector_class_id_operating_system:
        status = app_os_handler(request_id.os_request, data);
        break;

#if defined CONNECTOR_TRANSPORT_TCP
    case connector_class_id_network_tcp:
        status = app_network_tcp_handler(request_id.network_request, data);
        break;
#endif

#if defined CONNECTOR_TRANSPORT_UDP
    case connector_class_id_network_udp:
        status = app_network_udp_handler(request_id.network_request, data);
        break;
#endif

#if defined CONNECTOR_TRANSPORT_SMS
    case connector_class_id_network_sms:
        status = app_network_sms_handler(request_id.network_request, data);
        break;
#endif

    case connector_class_id_data_point:
        status = app_data_point_handler(request_id.data_point_request, data);
        break;

    case connector_class_id_short_message:
        status = app_sm_handler(request_id.sm_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    default:
        status = connector_callback_unrecognized;
        /* not supported */
        break;
    }

    return status;
}

int application_run(connector_handle_t handle)
{
    connector_status_t status;
    unsigned int const seed = (unsigned int)time(NULL);

    APP_DEBUG("Data point positive DVT using seed %u\n", seed);
    srand(seed);

    do
    {
        status = app_send_data_point(handle);

        switch (status)
        {
            case connector_init_error:
            case connector_service_busy:
            case connector_unavailable:
                status = connector_success;
                APP_DEBUG(".");
                sleep(20);
                break;

            case connector_success:
                sleep(10);
                break;

            case connector_idle:
                sleep(20);
                break;

            default:
                APP_DEBUG("SM send failed [%d]\n", status);
                break;
        }

    } while(app_dp_test_running());

    return 1;
}
