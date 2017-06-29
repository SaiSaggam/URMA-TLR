/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#include "dsl_monitor.h"
#include "dsl_message.h"
#include "dsl_modem.h"
#include "dsl_ppp.h"
#include "dsl_log.h"
#include "event_manager.h"
#include "timer.h"
#include "messages.h"
#include "daemonize.h"
#include "tlr_print.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <syslog.h>
#include <libgen.h>
#include <limits.h>

static int start(void)
{
	int result;
	message_thread_st * message_context = NULL;
	timer_context_st * timer_context;
	pthread_t modem_thread_id;

    /* timer thread must be started before the state machine */
	timer_context = open_timer_context();
	if (timer_context == NULL)
	{
		dsl_error("could not open timer context\n");
		result = EXIT_FAILURE;
		goto done;
	}

    if (!dsl_ppp_thread_init())
    {
		dsl_error("could not create PPP thread\n");
		result = EXIT_FAILURE;
		goto done;
    }

	/* start the monitor */
	if (dsl_monitor_start(timer_context) != 0)
	{
		dsl_error("could not start monitor\n");
		result = EXIT_FAILURE;
		goto done;
	}

	/* handle messages */
	message_context = open_messaging_context(dsl_monitor, dsl_message_callback);
	if (message_context == NULL)
	{
		dsl_error("could not open messaging context\n");
		result = EXIT_FAILURE;
		goto done;
	}

	/* create thread that will communicate with the DSL chipset */
    if (pthread_create(&modem_thread_id, NULL, dsl_modem_thread, NULL) != 0)
    {
    	dsl_error("could not create modem thread\n");
    	result = EXIT_FAILURE;
    	goto done;
    }

	daemon_confirm_is_running();

	result = EXIT_SUCCESS;

	/* this will block until all child threads end */
	pthread_exit(&result);

	/* shouldn't get here */
	dsl_error("exited!\n");

done:
	/* should only get here on error */
	if (message_context != NULL)
		free(message_context);
	return result;
}

int main(int argc, char * argv[])
{
	return start_daemon(argc, argv, start, true);
}
