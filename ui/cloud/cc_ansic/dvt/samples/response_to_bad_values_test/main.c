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

#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>
#include "connector_api.h"
#include "platform.h"
#include <unistd.h>
#include <errno.h>
#include "application.h"

connector_handle_t connector_handle;

pthread_t connector_thread;
pthread_t application_thread;

connector_status_t connector_run_thread_status = connector_success;

void * connector_run_thread(void * arg)
{
    connector_run_thread_status = connector_run(arg);
    APP_DEBUG("connector_run thread exits %d\n", connector_run_thread_status);

    pthread_exit(arg);
}


void * application_run_thread(void * arg)
{
    connector_status_t status;

    APP_DEBUG("application_run_thread starts\n");

    status = application_run(arg);

    APP_DEBUG("application_run thread exits %d\n", status);

    pthread_exit(arg);
}

void print_error(int error_no, char * const error_message)
{
    errno = error_no;
    perror("Error:");
    perror(error_message);

    ASSERT(0);
}

int main (void)
{
    int rc = 0;
    int bad_callback = 0xf00000;
    static int first_time = 0;
    unsigned long callback;

#if (defined CONNECTOR_WAN_PHONE_NUMBER_DIALED)
    config_test[connector_request_id_config_phone_number] = config_test_none;
#endif

#if (defined CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND)
    config_test[connector_request_id_config_link_speed] = config_test_none;
#endif

#if (defined CONNECTOR_MSG_MAX_TRANSACTION)
    config_test[connector_request_id_config_max_transaction] = config_test_none;
#endif


#if (defined CONNECTOR_DEVICE_ID_METHOD) || (defined CONNECTOR_WAN_TYPE)
    config_test[connector_request_id_config_meid] = config_test_none;
    config_test[connector_request_id_config_esn] = config_test_none;
    config_test[connector_request_id_config_imei_number] = config_test_none;
#endif


    for (;;)
    {
        APP_DEBUG("Start Etherios Connector\n");

        if (first_time == 1)
        {
            first_time = 0;
            callback = (unsigned long)bad_callback;
            connector_handle = connector_init((connector_callback_t) callback, NULL);
        }
        else
        {
            connector_handle = connector_init(app_connector_callback, NULL);
        }

        if (connector_handle != NULL)
        {
            if (connector_connect_tcp == connector_connect_manual)
            {
                connector_status_t status;

                status =  connector_initiate_action(connector_handle, connector_initiate_terminate, NULL);
                if (status != connector_success)
                {
                    APP_DEBUG("connector_run_thread: connector_initiate_terminate error %d\n", status);
                    break;
                }
            }

            {
                int ccode;
                ccode = pthread_create(&connector_thread, NULL, connector_run_thread, connector_handle);
                if (ccode != 0)
                {
                    APP_DEBUG("thread_create() error on connector_process_thread %d\n", ccode);
                    goto done;
                }

                ccode = pthread_create(&application_thread, NULL, application_run_thread, connector_handle);
                if (ccode != 0)
                {
                    APP_DEBUG("thread_create() error on application_run_thread %d\n", ccode);
                    goto done;
                }

                pthread_join(connector_thread, NULL);
                pthread_join(application_thread, NULL);
            }

            snprintf(python_file_buffer, sizeof(python_file_buffer), "connector_run thread terminated\n");
            write_python_result_file(python_file_buffer);

            APP_DEBUG("Etherios Cloud Connector terminated\n");


        }
        else
        {
            snprintf(python_file_buffer, sizeof(python_file_buffer), "connector_init failed\n");
            write_python_result_file(python_file_buffer);

            APP_DEBUG("Unable to initialize the connector\n");
        }

        if (config_test[connector_request_id_config_device_id] == config_test_none)
        {
            connector_identity_verification = connector_identity_verification_password;
        }

        if ((config_test[connector_request_id_config_phone_number] == config_test_none) &&
            (config_test[connector_request_id_config_link_speed] == config_test_none))
        {
            device_connection_type = connector_connection_type_lan;
        }

        if (config_test[connector_request_id_config_password] == config_test_none)
        {
            connector_identity_verification = connector_identity_verification_simple;
            connector_connect_tcp = connector_connect_auto;
        }

        if (config_test[connector_request_id_config_meid] == config_test_none)
        {
            device_id_method = connector_device_id_method_manual;
        }
        else if (config_test[connector_request_id_config_esn] == config_test_none)
        {
            connector_wan_type = connector_wan_type_meid;
        }
        else if (config_test[connector_request_id_config_imei_number] == config_test_none)
        {
            connector_wan_type = connector_wan_type_esn;
        }

        connector_run_thread_status = connector_success;
    }
done:
    return rc;
}
