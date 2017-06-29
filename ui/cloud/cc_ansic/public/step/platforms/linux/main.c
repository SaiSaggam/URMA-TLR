/*
 * Copyright (c) 2014 Digi International Inc.,
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
#include <unistd.h>
#include "connector_api.h"
#include "platform.h"

int main (void)
{
    int status=EXIT_FAILURE;
    connector_handle_t connector_handle;

    APP_DEBUG("Start Cloud Connector for Embedded\n");
    connector_handle = connector_init(app_connector_callback, NULL);
    if (connector_handle == NULL)
    {
        APP_DEBUG("Unable to initialize the connector\n");
        goto done;
    }

    /* Sample program control loop */
    for(;;)
    {
        connector_status_t status;
        /*
         * Example of using connector_step(), we run a portion of Cloud Connector then run
         * a portion of the users application.
         */
        status = connector_step(connector_handle);
        if (status != connector_idle && status != connector_working && status != connector_pending &&
            status != connector_active && status != connector_open_error)
         {
            /* We want to reconnect to Device Cloud
             * even if connector_step returns error in receive, send, or connect.
             */
            APP_DEBUG("main: connector_step() failed %d\n", status);
            break;
        }
        if (application_step(connector_handle) != 0)
        {
            APP_DEBUG("main: application_step() failed\n");
            goto done;
        }
        if (status == connector_idle)
        {
            usleep(1000);
        }
    }

done:
    return status;
}
