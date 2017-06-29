/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/
#include "wifi_monitor_log.h"
#include "wifi_monitor_init.h"
#include "event_manager.h"
#include "timer.h"

#include <daemonize.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


static int wifi_monitor_daemon_thread(void)
{
    int result;

	/* start the monitor */
	if (!wifi_monitor_start())
	{
		wifi_monitor_debug("could not start monitor\n");
		result = EXIT_FAILURE;
		goto error;
	}

    daemon_confirm_is_running();

    result = EXIT_SUCCESS;

	/* this will block until all child threads end */
	pthread_exit(&result);

	/* shouldn't get here */
	wifi_monitor_debug("wifi_monitor exiting.\n");

error:

	return result;
}

int main(int argc, char * argv[])
{
	return start_daemon(argc, argv, wifi_monitor_daemon_thread, true);
}
