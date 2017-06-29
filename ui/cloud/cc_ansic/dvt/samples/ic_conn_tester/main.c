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
#include <Windows.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"

int __cdecl main(int argc, char **argv)
{
	WSADATA wsaData;
    connector_handle_t connector_handle;
    int result = -1;

    /* Initialize Winsock */
	if (WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		APP_PRINTF("WSAStartup failed: %d\n", WSAGetLastError());
		goto done;
	}

    if (!application_init(argc, argv))
    {
        goto done;
    }

    APP_DEBUG("main: Starting iDigi\n");
    connector_handle = connector_init(app_connector_callback, NULL);
    if (connector_handle == NULL)
    {
        APP_DEBUG("main: connector_init() failed\n");
        APP_ALT_PRINTF("\nInitialization failed\n");
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
        if (status != status != connector_success && status != connector_idle &&
			status != connector_working && status != connector_pending && status != connector_active)
        {
            APP_DEBUG("\nmain: connector_step() failed\n");
            break;
        }
        result = application_step(connector_handle);

        if (result != 0)
            goto done;

		app_os_yield(&status);
    }

done:
    if (argc < 3)
    {
        result = 0;
    }
    else
    {
        result = result > 0 ? 0: 1;
        print_test_result(result);
    }
    application_cleanup();
	WSACleanup();
    return result;
}


