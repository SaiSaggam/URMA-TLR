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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include "nlmon_debug.h"

/*
   This application is run whenever a state change is detected on an interface.
   Before the application is run, two environment variables are set up:
   IFACE: The name of the interface that has changed state
   IFACE_STATE: the new state of the interface ("up" or "down")
*/
#define NOTIFICATION_APPLICATION	"/usr/etc/network/statechange"

static void setup_child_environment(char  const * const interface_name, int const interface_state)
{
	/* set the environment variables expected by our supported commands */
	setenv("IFACE", interface_name, 1 );
	setenv("IFACE_STATE", interface_state != 0 ? "up" : "down", 1 );

}

static pid_t fork_and_execute_command(char *executable, char **args, char const * const interface_name, int const interface_state)
{
	pid_t child_pid;

	child_pid = fork();

	if (child_pid == 0)
	{
		/* child process */

		setup_child_environment(interface_name, interface_state);

		if (execv(executable, args) < 0)
		{
			DEBUG_PRINT("failed to run %s\n", executable);
		}
		_exit(1);
		/* we don't get here unless the executable failed to run */
    }
    else if (child_pid < 0)
	{
		DEBUG_PRINT("fork failed\n");
	}
	else
	{
		/* parent process. Nothing to do. */
	}

    return child_pid;
}

static int wait_for_command_to_complete(pid_t child_pid)
{
	int exit_status;

	waitpid(child_pid, &exit_status, 0);

	return exit_status;
}

static int execute_command(char * executable, char **args, char const * const interface_name, int const interface_state)
{
	int result;
	pid_t child_pid;

	child_pid = fork_and_execute_command(executable, args, interface_name, interface_state);

	result = wait_for_command_to_complete(child_pid);

	return result;
}

/*
	inform_system_of_interface_state_change
   attempt to run our notification application passing the interface name and state as environment variables
   so that the system can respond to these state changes
   We don't care if we're unable to run the application; we just do our best.
*/
void inform_system_of_interface_state_change(char const * const interface_name, int const interface_state)
{
	char * args[2];
	char * arg0 = strdup(NOTIFICATION_APPLICATION);
	args[0] = basename(arg0);
	args[1] = NULL;

	execute_command(NOTIFICATION_APPLICATION, args, interface_name, interface_state);
	free(arg0);
}
