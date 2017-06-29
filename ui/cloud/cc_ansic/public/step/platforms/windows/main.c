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
#include <Windows.h>
#include "connector_api.h"
#include "platform.h"
#pragma comment(lib, "ws2_32.lib")

int __cdecl main(int argc, char **argv)
{
	WSADATA wsaData;
    connector_handle_t connector_handle;

	/* Initialize Winsock */
	if (WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		APP_DEBUG("WSAStartup failed: %d\n", WSAGetLastError());
		goto done;
	}
    APP_DEBUG("main: Starting iDigi\n");
    connector_handle = connector_init(app_connector_callback, NULL);
    if (connector_handle == NULL)
    {
        APP_DEBUG("main: connector_init() failed\n");
        goto done;
    }

    /* Sample program control loop */
    for(;;)
    {
        connector_status_t status;
        /*
         * Example of using connector_step(), we run a portion of the iDigi Connector then run
         * a portion of the users application.
         */
        status = connector_step(connector_handle);
        if (status != connector_idle && status != connector_working && status != connector_pending &&
            status != connector_active && status != connector_open_error)
        {
            /* We exclude connector_receive_error, connector_send_error, &
             * connector_connect_error. We want to reconnect iDigi Device Cloud
             * even if connector_step returns error in receive, send, or connect.
             */
            APP_DEBUG("main: connector_step() failed\n");
            break;
        }
        if (application_step(connector_handle) != 0)
        {
            APP_DEBUG("main: application_step() failed\n");
            goto done;
        }
		app_os_yield(&status);
    }

done:

	WSACleanup();
    return 1;
}


