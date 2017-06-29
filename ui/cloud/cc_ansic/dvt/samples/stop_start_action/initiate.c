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
#include <unistd.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"

initiate_action_t initiate_action = {device_request_not_support, connector_transport_all};

int initiate_run(connector_handle_t handle)
{
//    connector_status_t status = connector_success;
    connector_initiate_stop_request_t  request_data = {connector_transport_tcp, connector_stop_immediately, NULL};

//    if (connector_run_thread_status == connector_device_terminated) goto done;


#if 0
    APP_DEBUG("initiate_run: Start TCP\n");
    request_data.transport = connector_transport_tcp;

    do {
        status = connector_initiate_action(handle, connector_initiate_transport_start, &request_data.transport);
        if (status != connector_success)
        {
            APP_DEBUG("initiate_run: connector_initiate_action for connector_initiate_transport_start returns error %d\n", status);
            sleep(1);
        }
    } while (status == connector_service_busy);

    /* start TCP again */
    status = connector_initiate_action(handle, connector_initiate_transport_start, &request_data.transport);
    if (status != connector_success)
    {
        /* should be ok since it's already started */
        APP_DEBUG("initiate_run: connector_initiate_action for connector_initiate_transport_start returns error %d\n", status);
        ASSERT(0);
    }
#endif

    for (;connector_run_thread_status != connector_device_terminated;)
    {
        switch (initiate_action.target)
        {
            case device_request_app_stop_connector:
                APP_DEBUG("Initiate_run: Stop  %s connector immediately (active put request = %d)\n", transport_to_string(request_data.transport), put_file_active_count[initiate_action.transport]);
                connector_close_status = connector_close_status_device_stopped;
                request_data.transport = initiate_action.transport;
                stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
                {
                    connector_status_t const status = connector_initiate_action(handle, connector_initiate_transport_stop, &request_data);
                    if (status != connector_success)
                    {
                        APP_DEBUG("initiate_run: connector_initiate_action for connector_initiate_transport_stop returns error %d\n", status);
                    }
                    else
                    {
#if (TRANSPORT_COUNT == 1)
                        initiate_action.target = device_request_app_start_connector;
#else
                        initiate_action.target = (initiate_action.transport == connector_transport_all) ? device_request_app_start_connector : device_request_not_support;
#endif
                    }
                    break;
                }
            case device_request_app_start_connector:
                sleep(1);
                APP_DEBUG("Initiate_run: Start Etherios Connector %s\n", transport_to_string(initiate_action.transport));

                {
                    connector_status_t status;
                    connector_initiate_stop_request_t  request_data = {connector_transport_all, connector_stop_immediately, NULL};

                    request_data.transport = (initiate_action.transport == connector_transport_all) ? connector_transport_tcp : initiate_action.transport;


                    for  (;request_data.transport <= initiate_action.transport; request_data.transport++)
                    {
                        int initiate_action = 0;
#if (defined CONNECTOR_TRANSPORT_TCP)
                        if (request_data.transport == connector_transport_tcp) initiate_action = 1;
#endif
 #if (defined CONNECTOR_TRANSPORT_UDP)
                        if (request_data.transport == connector_transport_udp) initiate_action = 1;
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
                        if (request_data.transport == connector_transport_sms) initiate_action = 1;
#endif
                        if (initiate_action)
                        {
                            APP_DEBUG("initiate_run: Start %s\n", transport_to_string(request_data.transport));
                            request_data.user_context = &request_data.transport;
                            do {
                                status = connector_initiate_action(handle, connector_initiate_transport_start, &request_data);
                                if (status != connector_success && status != connector_service_busy)
                                {
                                    APP_DEBUG("application_ping: connector_initiate_action for connector_initiate_transport_start returns error %d\n", status);
                                    ASSERT(0);
                                }
                            } while (status == connector_service_busy);
                        }
                    }
                }
                initiate_action.target = device_request_not_support;
                break;

            case device_request_stop_connector:
            case device_request_stop_all_transports:
#if (TRANSPORT_COUNT == 1)
                APP_DEBUG("Initiate_run: Let reStart Etherios Connector\n");
                initiate_action.target = device_request_app_start_connector;
#else
                APP_DEBUG("initiate_run: transport %s\n", transport_to_string(initiate_action.transport));
                initiate_action.target = (initiate_action.transport == connector_transport_all) ? device_request_app_start_connector : device_request_not_support;
#endif
                break;

            case device_request_stop_terminate_connector:
                APP_DEBUG("Initiate_run: Terminate connector\n");
                connector_close_status = connector_close_status_device_terminated;
                {
                    connector_status_t const status = connector_initiate_action(handle, connector_initiate_terminate, NULL);
                    if (status != connector_success)
                    {
                        APP_DEBUG("initiate_run: connector_initiate_action for connector_initiate_terminate returns error %d\n", status);
                        ASSERT(0);
                    }
                    else
                    {
                        APP_DEBUG("initiate_run: connector_initiate_action for connector_initiate_terminate\n");
                        terminate_flag = device_request_terminate_done;
                        initiate_action.target = device_request_not_support;
                        sleep(2);
                        goto done;
                    }

                    break;
                }
            case device_request_not_support:
                break;

            default:
                initiate_action.target = device_request_not_support;
                break;
        }
        sleep(1);
    }

done:
    APP_DEBUG("initiate_run: exit\n");
   return 0;
}
