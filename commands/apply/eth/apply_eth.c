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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config_def.h"
#include "messages.h"
#include "message_codes.h"
#include "tlr_print.h"
#include "interfaces.h"

#define INTERFACE_MANAGER_INSTANCE 1
#define MIN_INSTANCE	1
#define MAX_INSTANCE	4

/********************
	Functions
********************/

int run_command(char * command)
{
	if (system(command) != 0)
		return -1;
	return 0;
}


/********************
	Main Function
********************/

int main(int argc, char * argv[])
{
	int ret = EXIT_FAILURE;
	size_t instance = -1;
	char msg[40];
	char const * const config_group = get_config_group_name_from_id(config_eth);
	int 	senttoeth = 0;
	int 	senttoifman = 0;
	int c;

	while ((c = getopt (argc, argv, "i:u:a:")) != -1)
        {
            switch (c)
            {
			case 'i':
				instance = atoi(optarg);
                break;
			default:
			    /* ignore the rest for now */
			    break;
            }
        }
	if (instance < MIN_INSTANCE || instance > MAX_INSTANCE)
		goto ERROR;

	snprintf(msg, sizeof msg, "%s %d", config_group, instance);
	if (send_message(ethernet_monitor, instance, update_message, msg, NULL))
	{
		tlr_printf(tlr_response_format_ascii, "apply_eth: send_message() to ethernet_monitor failed\n");
		tlr_event_log(tlr_log_error, "apply_eth", "apply_eth: send_message() to ethernet_monitor failed");
	}
	else
		senttoeth = 1;
	if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, update_message, msg, NULL))
	{
		tlr_printf(tlr_response_format_ascii, "apply_eth: send_message() to interface_manager failed\n");
		tlr_event_log(tlr_log_error, "apply_eth", "apply_eth: send_message() to interface_manager failed");
	}
	else
		senttoifman = 1;

	if (senttoeth && senttoifman)
	{
		ret = EXIT_SUCCESS;
		tlr_event_log(tlr_log_info, "apply_eth", "apply_eth: sent update_message to ethernet_monitor and interface_manager");
	}
	else if (senttoeth)
		tlr_event_log(tlr_log_info, "apply_eth", "apply_eth: sent update_message to ethernet_monitor but failed to send to interface_manager");
	else if (senttoifman)
		tlr_event_log(tlr_log_info, "apply_eth", "apply_eth: sent update_message to interface_manager but failed to send to ethernet_monitor");

ERROR:
	return ret;
}
