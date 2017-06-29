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

#include "wifi_monitor_ifconfig_commands.h"
#include "wifi_monitor_init.h"
#include "wifi_monitor_log.h"
#include "wifi_monitor.h"
#include "digiutils.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


#define IFCONFIG_EXECUTABLE "/sbin/ifconfig"

static bool wifi_execute_ifconfig_up_down_command(size_t const instance, bool const do_connect)
{
	bool result;
    char const * const direction = do_connect ? "up" : "down";
    int command_result;
    char const * const interface_name = wifi_interface_name_get();
    char const * const process_name = wifi_process_name_get();

    command_result = system_command_with_log(process_name, IFCONFIG_EXECUTABLE" %s%zu %s", interface_name, instance, direction);
    if (command_result == -1)
    {
        wifi_monitor_error("unable to set %s%zu interface state %s\n", interface_name, instance, direction);
        result = false;
        goto error;
    }

	result = true;

error:
	return result;
}

bool wifi_connect_interface(size_t const instance)
{
    return wifi_execute_ifconfig_up_down_command(instance, true);
}

bool wifi_disconnect_interface(size_t const instance)
{
    return wifi_execute_ifconfig_up_down_command(instance, false);
}

