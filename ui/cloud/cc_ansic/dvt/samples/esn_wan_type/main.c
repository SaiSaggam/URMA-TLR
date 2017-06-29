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

connector_status_t connector_run_thread_status = connector_success;

extern char terminate_file_content[];



void * connector_run_thread(void * arg)
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


void * application_run_thread(void * arg)
{
    int status;

    APP_DEBUG("application_run_thread starts\n");

    status = application_run(arg);

    APP_DEBUG("application_run thread exits %d\n", status);

    pthread_exit(arg);
}

int start_connector_thread(void)
{
    int ccode = -1;

    connector_handle = connector_init(app_connector_callback, NULL);
    if (connector_handle != NULL)
    {
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

int start_application_thread(void)
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

int main (void)
{
    int rc = 0;


    for (;;)
    {
        APP_DEBUG("Start Etherios Connector\n");

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

        pthread_join(connector_thread, NULL);
        pthread_join(application_thread, NULL);
    }

done:
    return rc;
}
