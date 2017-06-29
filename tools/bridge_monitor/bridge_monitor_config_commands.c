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

#include "bridge_monitor_config_commands.h"
#include "bridge_monitor_init.h"
#include "bridge_monitor_log.h"
#include "bridge_monitor.h"
#include "bridge_dns.h"
#include "digiutils.h"

#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


#define IFCONFIG_EXECUTABLE "/sbin/ifconfig"
#define UDHCPC_EXECUTABLE   "/sbin/udhcpc"

static int bridge_execute_ifconfig_up_down_command(size_t const instance, bool do_connect)
{
	bool ret;
    char * command = NULL;
    char const * direction = do_connect ? "up" : "down";
    int command_result;

    command_result = system_command_with_log(BRIDGEMON_PROCESS_NAME, IFCONFIG_EXECUTABLE" %s%zu %s", bridge_interface_name_get(), instance, direction);
    if (command_result == -1)
    {
        bridgemon_error("unable to set %s%zu interface state %s\n", bridge_interface_name_get(), instance, direction);
        ret = false;
        goto error;
    }

	ret = true;

error:
    free(command);
	return ret;
}

bool bridge_connect_interface(size_t const instance)
{
    bool result;

    bridgemon_debug("connect %s%zu\n", bridge_interface_name_get(), instance);
    result = bridge_execute_ifconfig_up_down_command(instance, true);

    return result;
}

bool bridge_disconnect_interface(size_t const instance)
{
    bool result;

    bridgemon_debug("disconnect %s%zu\n", bridge_interface_name_get(), instance);

    result = bridge_execute_ifconfig_up_down_command(instance, false);

    return result;
}

bool bridge_interface_set_mtu(size_t const instance, char const * const mtu_string)
{
    bool mtu_set;

    if (system_command_with_log(BRIDGEMON_PROCESS_NAME, IFCONFIG_EXECUTABLE" %s%zu mtu %s", bridge_interface_name_get(), instance, mtu_string) == -1)
    {
        mtu_set = false;
        goto done;
    }
    mtu_set = true;

done:
    return mtu_set;
}

bool bridge_interface_set_ip_address_and_mask(size_t const instance, char const * const ip_address_string, char const * const mask_string)
{
    bool ip_address_and_mask_set;
    char const * const null_ip_address_string = "0.0.0.0";
    char const * ip_address_to_set;

    if (*ip_address_string == '\0')
    {
        ip_address_to_set = null_ip_address_string;
    }
    else
    {
        ip_address_to_set = ip_address_string;
    }

    if (system_command_with_log(BRIDGEMON_PROCESS_NAME, IFCONFIG_EXECUTABLE" %s%zu %s netmask %s", bridge_interface_name_get(), instance, ip_address_to_set, mask_string) == -1)
    {
        ip_address_and_mask_set = false;
        goto done;
    }

    ip_address_and_mask_set = true;

done:
    return ip_address_and_mask_set;
}

bool start_dhcp_client(size_t const instance, char const * const pid_filename)
{
    bool client_started = false;

    bridgemon_debug("Starting DHCP client on lan%u", instance);
    if (system_command_with_log(BRIDGEMON_PROCESS_NAME, UDHCPC_EXECUTABLE" -i %s%zu -p %s -s /usr/share/udhcpc/tlr_dhcp_client_script.sh &", bridge_interface_name_get(), instance, pid_filename) == 0)
    {
        client_started = true;
    }
    return client_started;
}

void stop_dhcp_client(size_t const instance, pid_t pid, char const * const pid_filename)
{
    int const SIG_RELEASE_DHCP_LEASE = SIGUSR2;

    bridgemon_debug("Stopping DHCP client on lan%u", instance);
    if (kill(pid, SIG_RELEASE_DHCP_LEASE))
    {
        bridgemon_debug("Unable to release DHCP lease");
    }
    usleep(10000);
    if (kill(pid, SIGKILL))
    {
        bridgemon_debug("Unable to kill udhcpc process");
    }
    unlink(pid_filename);
    delete_dhcp_nameservers_file(instance);
}
