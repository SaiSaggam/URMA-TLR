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

#include <pthread.h>
#include "connector_api.h"
#include "platform.h"
#include <unistd.h>
#include <errno.h>

#include "application.h"

connector_handle_t connector_handle;

pthread_t connector_thread;
pthread_t application_thread;
pthread_t initiate_thread;
pthread_t udp_thread;
pthread_t ping_thread;

connector_status_t connector_run_thread_status = connector_success;

static void * connector_run_thread(void * arg)
{

    APP_DEBUG("connector_run_thread starts pid = %d \n", getpid());
    connector_run_thread_status = connector_success;

    for (;;)
    {
        connector_run_thread_status = connector_run(arg);
        if (connector_run_thread_status != connector_open_error) break;
    }
    APP_DEBUG("connector_run thread exits %d\n", connector_run_thread_status);

    connector_run_thread_status = connector_device_terminated;

    pthread_exit(arg);
}


static void * application_run_thread(void * arg)
{
    int status;

    APP_DEBUG("application_run_thread starts\n");

    status = application_run(arg);

    APP_DEBUG("application_run thread exits %d\n", status);

    pthread_exit(arg);
}

static void * application_udp_thread(void * arg)
{
    int status;

    APP_DEBUG("application_udp_thread starts\n");

    status = application_udp(arg);

    APP_DEBUG("application_udp_thread thread exits %d\n", status);

    pthread_exit(arg);
}

static void * application_ping_thread(void * arg)
{
    int status;

    APP_DEBUG("application_ping_thread starts\n");

    status = application_ping(arg);

    APP_DEBUG("application_ping_thread exits %d\n", status);

    pthread_exit(arg);
}

static void * initiate_run_thread(void * arg)
{
    int status;

    APP_DEBUG("initiate_run_thread starts\n");

    status = initiate_run(arg);

    APP_DEBUG("initiate_run_thread thread exits %d\n", status);

    pthread_exit(arg);
}

void print_error(int error_no, char * const error_message)
{
    errno = error_no;
    perror("Error:");
    perror(error_message);
    ASSERT(0);
}

static int start_connector_thread(void)
{
    int ccode = -1;

    connector_handle = connector_init(app_connector_callback, NULL);
    if (connector_handle != NULL)
    {
        connector_initiate_stop_request_t request_data = {connector_transport_all, connector_stop_immediately, NULL};
        connector_status_t  status;

        connector_close_status = connector_close_status_device_stopped;
        stop_transport_count[request_data.transport]++;  /* counter which is used to ensure the status_complete callback is called */
        status =  connector_initiate_action(connector_handle, connector_initiate_transport_stop, &request_data);
        if (status != connector_success)
        {
            ASSERT(0);
        }

        ccode = pthread_create(&connector_thread, NULL, connector_run_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on connector_run_thread %d\n", ccode);
            connector_run_thread_status = connector_device_terminated;
        }
    }
    else
    {
        APP_DEBUG("Unable to initialize the connector\n");
    }

    return ccode;
}

static int start_application_thread(void)
{
    int ccode = 0;

    if (connector_handle != NULL)
    {
        ccode = pthread_create(&application_thread, NULL, application_run_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on application_run_thread %d\n", ccode);
        }
    }

    return ccode;
}

static int start_application_udp_thread(void)
{
    int ccode = 0;

    if (connector_handle != NULL)
    {
        ccode = pthread_create(&udp_thread, NULL, application_udp_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on application_run_thread %d\n", ccode);
        }
    }

    return ccode;
}

static int start_application_ping_thread(void)
{
    int ccode = 0;

    if (connector_handle != NULL)
    {
        ccode = pthread_create(&ping_thread, NULL, application_ping_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on application_run_thread %d\n", ccode);
        }
    }

    return ccode;
}

static int start_initiate_thread(void)
{
    int ccode = 0;

    if (connector_handle != NULL)
    {
        ccode = pthread_create(&initiate_thread, NULL, initiate_run_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on application_run_thread %d\n", ccode);
        }
    }

    return ccode;
}


int main (void)
{
    extern size_t total_malloc_size;

    int rc = 0;

    malloc_sem_init();
    initialize_send_data();

    for (;;)
    {

        rc = start_connector_thread();
        if (rc != 0)
        {
           goto done;
        }

        rc = start_application_thread();
        if (rc != 0)
        {
           goto done;
        }

        rc = start_initiate_thread();
        if (rc != 0)
        {
           goto done;
        }

        rc = start_application_udp_thread();
        if (rc != 0)
        {
           goto done;
        }

        rc = start_application_ping_thread();
        if (rc != 0)
        {
           goto done;
        }

        pthread_join(connector_thread, NULL);
        pthread_join(application_thread, NULL);
        pthread_join(initiate_thread, NULL);
        pthread_join(udp_thread, NULL);
        pthread_join(ping_thread, NULL);

        if (connector_run_thread_status == connector_device_terminated)
        {
            strcpy(terminate_file_content, "terminate_ok");
            APP_DEBUG("connector_run has been terminated by connector_initiate_terminate\n");
        }
        if (total_malloc_size != 0)
        {
            connector_transport_t transport;

            /* terminate Cloud Connector so it will not reconnect to Device Cloud */
            APP_DEBUG("total malloc memory = %" PRIsize " after all threads are canceled\n", total_malloc_size);
            if (connector_run_thread_status == connector_device_terminated)
            {
                strcpy(terminate_file_content, "terminate_memory_leak");
                APP_DEBUG("Error: connector_run has been terminated by connector_initiate_terminate but total malloc memory is not 0 after all threads are canceled\n");
            }
            for (transport = connector_transport_tcp; transport < connector_transport_all; transport++)
            {
                if (put_file_active_count[transport] > 0)
                {
                    APP_DEBUG("Error: %d active put requests on %s\n", put_file_active_count[transport], transport_to_string(transport));
                }
            }
        }

        if (strlen(connector_close_status_text) > 0)
        {
            strcpy(terminate_file_content, connector_close_status_text);
            connector_close_status_text[0] ='\0';
        }
        /* continue and reconnect Device Cloud so python test will not fail */
        sleep(2);

        connector_run_thread_status = connector_success;

    }

done:
    return rc;
}
