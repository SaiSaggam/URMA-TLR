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

terminate_flag_t terminate_flag = device_request_idle;
char terminate_file_content[100] = "\0";

connector_close_status_t connector_close_status;
char connector_close_status_text[100] = "\0";

extern connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request, void * const data);
#if (defined APP_USE_SM)
extern connector_callback_status_t app_sm_handler(connector_request_id_sm_t const request, void * const data);
#endif
#define enum_to_case(name)  case name:  result = #name;             break

static char const * close_status_to_string(connector_close_status_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_close_status_cloud_disconnected);
        enum_to_case(connector_close_status_cloud_redirected);
        enum_to_case(connector_close_status_device_terminated);
        enum_to_case(connector_close_status_device_stopped);
        enum_to_case(connector_close_status_no_keepalive);
        enum_to_case(connector_close_status_abort);
        enum_to_case(connector_close_status_device_error);
    }
    return result;
}

static char const * app_class_to_string(connector_class_id_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_class_id_config);
        enum_to_case(connector_class_id_operating_system);
        enum_to_case(connector_class_id_firmware);
        enum_to_case(connector_class_id_data_service);
        enum_to_case(connector_class_id_remote_config);
        enum_to_case(connector_class_id_file_system);
        enum_to_case(connector_class_id_network_tcp);
        enum_to_case(connector_class_id_network_udp);
        enum_to_case(connector_class_id_network_sms);
        enum_to_case(connector_class_id_status);
        enum_to_case(connector_class_id_short_message);
        enum_to_case(connector_class_id_data_point);
    }
    return result;
}

connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    connector_bool_t type;

    UNUSED_ARGUMENT(class_id);

    APP_DEBUG("app_connector_reconnect: %s close status %s\n", app_class_to_string(class_id), close_status_to_string(status));

    switch (status)
    {
           /* if either Device Cloud or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
        case connector_close_status_device_stopped:
        case connector_close_status_abort:
             type = connector_false;
             if (status != connector_close_status)
             {
                 connector_status_t action_status;
                 APP_DEBUG("app_connector_reconnect: terminating. unexpected close status %s is not expected %s\n", close_status_to_string(status), close_status_to_string(connector_close_status));

                 snprintf(connector_close_status_text, sizeof connector_close_status_text, "%s is not expected %s\n", close_status_to_string(status), close_status_to_string(connector_close_status));
                 action_status = connector_initiate_action(connector_handle, connector_initiate_terminate, NULL);
                 if (action_status != connector_success)
                 {
                     APP_DEBUG("app_connector_reconnect: connector_initiate_action for connector_initiate_terminate returns error %d\n", status);
                     ASSERT(0);
                 }
             }
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
#if (defined CONNECTOR_TRANSPORT_TCP)
    case connector_class_id_network_tcp:
        status = app_network_tcp_handler(request_id.network_request, data);
        break;
#endif

    case connector_class_id_data_service:
        status = app_data_service_handler(request_id.data_service_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

#if (defined APP_USE_SM)
    case connector_class_id_short_message:
        status = app_sm_handler(request_id.sm_request, data);
        break;
#endif

#if (defined CONNECTOR_TRANSPORT_UDP)
    case connector_class_id_network_udp:
        status = app_network_udp_handler(request_id.network_request, data);
        break;
#endif

    default:
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}

void write_transport_error_file(connector_handle_t handle)
{
#define TRANSPORT_ERROR_FORMAT "<%s>%d</%s>"

    static char stop_transport_error_content[128];

    connector_transport_t transport;
    char * file_content = stop_transport_error_content;
    size_t file_length = 0;

    for (transport = connector_transport_tcp; transport <= connector_transport_all; transport++)
    {
        if (stop_transport_count[transport] > 0)
        {
            size_t const length_avail = sizeof stop_transport_error_content - file_length;

            file_length += snprintf(file_content, length_avail, TRANSPORT_ERROR_FORMAT, transport_to_string(transport), stop_transport_count[transport], transport_to_string(transport));
            file_content += file_length;
            stop_transport_count[transport] = 0;
        }
    }

/*    if (file_length > 0) */
    {
        connector_status_t status;

        file_content = stop_transport_error_content;

#if (defined CONNECTOR_TRANSPORT_TCP)
         transport = connector_transport_tcp;
#elif (defined CONNECTOR_TRANSPORT_UDP)
         transport = connector_transport_udp;
#elif (defined CONNECTOR_TRANSPORT_SMS)
         transport = connector_transport_sms;
#endif

        do {
            status = send_file(handle, 255, (char * const)TRANSPORT_ERROR_FILE, (char * const)file_content, file_length, transport);
            if (status != connector_success)
            {
                APP_DEBUG("write_transport_error_file: unable to send %s (%s) %d\n", TRANSPORT_ERROR_FILE, transport_to_string(transport), status);
                sleep(2);
            }
            else
            {
                /* assume it's done sending */
                APP_DEBUG("write_transport_error_file: sent %s (%s) size %" PRIsize " \n", TRANSPORT_ERROR_FILE, transport_to_string(transport), file_length);
                break;
            }
        } while (status != connector_device_terminated);
    }

}
int application_run(connector_handle_t handle)
{
    connector_status_t status;
#if (defined CONNECTOR_TRANSPORT_TCP)
    int index = 0;
#endif
    size_t file_length = strlen(terminate_file_content);

    {
        connector_initiate_stop_request_t  request_data = {(connector_transport_t)(connector_transport_all + 1), connector_stop_immediately, NULL};

        if (file_length == 0)
        {
            /* invalid transport */
            status = connector_initiate_action(handle, connector_initiate_transport_start, &request_data.transport);
            if (status == connector_success)
            {
                APP_DEBUG("application_ping: connector_initiate_action for connector_initiate_transport_start returns ok on invalid transport\n");
                ASSERT(0);
            }
        }

        for  (request_data.transport = connector_transport_tcp; request_data.transport < connector_transport_all; request_data.transport++)
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
                connector_status_t status;
                APP_DEBUG("application_ping: Stop %s\n", transport_to_string(request_data.transport));
                request_data.user_context = handle;
                stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
                do {
                    status = connector_initiate_action(handle, connector_initiate_transport_stop, &request_data);
                    if (status != connector_success)
                    {
                        APP_DEBUG("application_ping: connector_initiate_action for connector_initiate_transport_stop returns error %d\n", status);
                        if (status != connector_service_busy)
                        {
                            ASSERT(0);
                        }
                    }
                } while (status == connector_service_busy);
            }
        }
    }

#if 0
    {
        connector_transport_t  transport;

        for  (transport = connector_transport_tcp; transport < connector_transport_all; transport++)
        {
            int initiate_action = 0;
#if (defined CONNECTOR_TRANSPORT_TCP)
            if (transport == connector_transport_tcp) initiate_action = 1;
#endif
#if (defined CONNECTOR_TRANSPORT_UDP)
            if (transport == connector_transport_udp) initiate_action = 1;
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
            if (transport == connector_transport_sms) initiate_action = 1;
#endif
            if (initiate_action)
            {
                APP_DEBUG("application_ping: Start %s\n", transport_to_string(transport));
                do {
                    status = connector_initiate_action(handle, connector_initiate_transport_start, &transport);
                    if (status != connector_success && status != connector_service_busy)
                    {
                        APP_DEBUG("application_ping: connector_initiate_action for connector_initiate_transport_start returns error %d\n", status);
                        ASSERT(0);
                    }
                } while (status == connector_service_busy);
            }
        }
    }
#endif


    APP_DEBUG("application_run: terminate_flag = %d file_length = %" PRIsize "\n", terminate_flag, file_length);

     while (file_length > 0)
     {
         connector_transport_t transport;
#if (defined CONNECTOR_TRANSPORT_TCP)
         transport = connector_transport_tcp;
#elif (defined CONNECTOR_TRANSPORT_UDP)
         transport = connector_transport_udp;
#elif (defined CONNECTOR_TRANSPORT_SMS)
         transport = connector_transport_sms;
#endif

         status = send_file(handle, 255, (char * const)TERMINATE_TEST_FILE, (char * const)terminate_file_content, file_length, transport);
         if (status != connector_success)
         {
             APP_DEBUG("application_run: unable to send terminate_test.txt %d\n", status);
             sleep(2);
         }
         else
         {
             /* assume it's done sending */
             terminate_flag = device_request_idle;
             APP_DEBUG("application_run: sent terminate_test.txt\n");
             APP_DEBUG("application_run: terminate_flag = device_request_idle\n");
             break;
         }
     }

     if (file_length > 0) write_transport_error_file(handle);

#if (defined CONNECTOR_TRANSPORT_TCP)
    for (;connector_run_thread_status != connector_device_terminated;)
    {
        status = send_put_request(handle, index, connector_transport_tcp);

        switch (status)
        {
        case connector_init_error:
        case connector_service_busy:
            break;

        case connector_success:
            index++;
            break;

        case connector_invalid_data_range:
            index = 0;
            sleep(5);
            break;

        case connector_device_terminated:
            goto done;

        default:
            break;
        }
        sleep(1);
    }

done:
#endif
    APP_DEBUG("application_run: exit\n");
    return 0;
}

int application_udp(connector_handle_t handle)
{
#if (defined CONNECTOR_TRANSPORT_UDP)
    connector_status_t status;
    int index = 0;

    for (;connector_run_thread_status != connector_device_terminated;)
    {
        status = send_put_request(handle, index, connector_transport_udp);

        switch (status)
        {
        case connector_init_error:
        case connector_service_busy:
            break;

        case connector_success:
            index++;
            break;

        case connector_invalid_data_range:
            index = 0;
            sleep(5);
            break;

        case connector_device_terminated:
            goto done;

        default:
            break;
        }

        sleep(1);
    }
done:
#else
    UNUSED_ARGUMENT(handle);
#endif
    APP_DEBUG("application_udp: exit\n");
    return 1;
}


#if (defined CONNECTOR_TRANSPORT_UDP)

connector_status_t app_send_ping(connector_handle_t handle)
{
    static connector_message_status_request_t ping_request; /* Cloud connector will hold this until reply received or send completes */
    connector_status_t status;

    ping_request.transport = connector_transport_udp;
    ping_request.user_context = NULL;
    ping_request.flags = CONNECTOR_DATA_RESPONSE_NOT_NEEDED;
    status = connector_initiate_action(handle, connector_initiate_ping_request, &ping_request);
    APP_DEBUG("Sent ping [%d].\n", status);

    return status;
}
#endif


int application_ping(connector_handle_t handle)
{

#if (defined CONNECTOR_TRANSPORT_UDP)
    for (;connector_run_thread_status != connector_device_terminated;)
    {
        connector_status_t status = app_send_ping(handle);
        switch (status)
        {
            case connector_service_busy:
                break;

            case connector_success:
                sleep(5);
                break;

            default:
                APP_DEBUG("Send data failed [%d]\n", status);
                break;
        }
        sleep(1);

    }

#else
    UNUSED_ARGUMENT(handle);
#endif
    APP_DEBUG("application_ping: exit\n");
    return 1;
}


