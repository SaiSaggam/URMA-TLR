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

#include "ifmanager.h"
#include "failover.h"
#include <event_manager.h>
#include <messages.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <daemonize.h>
#include <unistd.h>

#include "unittest_support.h"

STATIC_FUNCTION int if_manager_start(void)
{
	int result = EXIT_FAILURE;
	message_thread_st * message_context;
	timer_context_st * timer_context;

    /* timer thread must be started before the state machine */
	timer_context = open_timer_context();
	if (timer_context == NULL)
	{
		goto done;
	}

    /* handle fail-over */
    if (ifm_init(timer_context) != 0)
    {
        goto done;
    }

	/* handle messages */
	message_context = open_messaging_context(interface_manager, ifm_message_callback);
	if (message_context == NULL)
	{
		goto done;
	}

	daemon_confirm_is_running();

	result = EXIT_SUCCESS;

#ifndef _CUNIT_
    ifm_info("ready!\n");

    /* this will block until all child threads end */
	pthread_exit(&result);

	/* shouldn't get here */
	ifm_error("exited!\n");
#endif

done:
    /* should only get here on error */
	return result;
}

#ifndef _CUNIT_
int main(int argc, char * argv[])
{
    return start_daemon(argc, argv, if_manager_start, true);
}
#endif
