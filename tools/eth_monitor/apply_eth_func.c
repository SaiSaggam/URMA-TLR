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

#include "eth_monitor.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "config_mgr.h"
#include "ethphy.h"
#include "ethregs.h"
#include "apply_eth.h"
#include <libifuptime_defs.h>
#include "tlr_print.h"
#include "dhcp_client.h"

#if defined(UNIT_TEST)
#define STATIC
#else
#define STATIC static
#endif

static char const OFF_STRING[] = "off";
/********************
	Functions
********************/
STATIC int run_command(char * command)
{
	if (system(command) != 0)
		return -1;
	return 0;
}

STATIC void print_error(char * message)
{
	tlr_debug("Apply Error: %s.\n", message);
}

bool is_interface_up(int const instance)
{
	// Return true if interface is up.
	// We look for the uptime file which the daemon 'nlmon' is in charge of.
	char f_uptime[CMD_LEN];
	struct stat fileStat;

	snprintf(f_uptime, sizeof f_uptime, "%s/eth%d", INTERFACE_UPTIME_BASE_DIRECTORY, instance);
	if (stat(f_uptime, &fileStat) < 0)
		return false;
	else
		return true;
}

int temp_read_config_eth_state(size_t instance)
{
	char 	value[VAL_LEN];
	config_status_t const status = tlr_get_element_value(config_eth, instance, config_eth_state,
														 value, sizeof value,
														 config_mgr_element_type_string, config_mgr_access_level_read_only);
	if (status == config_status_success)
	{
		if (strcmp(value, "off") == 0 || strcmp(value, "on-demand") == 0)
			return 0;
		else
			return 1;
	}

	return 0;
}

STATIC bool read_wan_dhcp_config(eth_context_t * const context)
{
	bool dhcp_enabled = false;
    unsigned int on_off = 0;

    if (context->wan_instance != ETH_NOT_ASSIGNED_TO_A_WAN)
    {
        config_status_t const read_status = tlr_get_element_value(config_wan, context->wan_instance,
                                                            config_wan_dhcp, (void *)&on_off,
                                                            sizeof(on_off), config_mgr_element_type_on_off,
                                                            config_mgr_access_level_read_only);

        if (read_status == config_status_success)
        {
            dhcp_enabled = (on_off != 0);
        }
        else
        {
            tlr_debug("read_wan_dhcp_config couldn't read dhcp setting: %d\n", read_status);
        }
    }

    return dhcp_enabled;
}

STATIC int eth_read_uci(eth_context_t * const context)
{
    config_eth_t const * const parameters = context->parameters;
    int const instance = context->instance;
    char ** values = context->values;
	int ret = -1;
	config_group_t group = config_eth;
	config_mgr_access_level_t const access_level = config_mgr_access_level_read_only;

	tlr_handle_t handle = tlr_config_open(group, instance, access_level);
	if (handle == NULL)
	{
		print_error("cannot open uci file");
		goto ERROR;
	}
	config_status_t config_status;

	for (int i = 0; i < eth_count; i++)
	{
		values[i] = malloc(VAL_LEN * sizeof(char));
		config_status = tlr_config_read(handle, parameters[i], values[i], VAL_LEN * sizeof(char));
		if (config_status != config_status_success)
		{
			print_error("cannot read uci file");
			goto ERROR;
		}
	}

    context->dhcp_client_enabled = read_wan_dhcp_config(context);

	if (context->desired_eth_state)
	{
	    tlr_debug("eth_read_uci: forcing values[eth_state] to on because desired_eth_state is true.\n");
		strcpy(context->values[eth_state], "on");
	}
	else
	{
	    tlr_debug("eth_read_uci: forcing values[eth_state] to off because desired_eth_state is false.\n");
		strcpy(context->values[eth_state], "off");
	}

	ret = 0;
ERROR:
	if (handle != NULL)
		tlr_config_close(handle);
	return ret;
}

STATIC int eth_apply_mtu(int const instance, char const * const mtu)
{
	int ret = -1;
	char command[CMD_LEN];
	if (*mtu != '\0')
	{
		snprintf(command, sizeof command, "ifconfig eth%d mtu %s", instance, mtu);
		if (run_command(command) != 0)
		{
			print_error("unable to configure mtu");
			goto ERROR;
		}
	}
	ret = 0;
ERROR:
	return ret;
}

STATIC int eth_apply_state(int const instance, char const * const state)
{
	int ret = -1;
	char command[CMD_LEN];
	snprintf(command, sizeof command, "ifconfig eth%d %s", instance, (strcmp(state, "on") == 0) ? "up" : "down");
	if (run_command(command) != 0)
	{
		print_error("unable to set interface status");
		goto ERROR;
	}

	ret = 0;
ERROR:
	return ret;
}

STATIC int eth_apply(eth_context_t * const context)
{
	int ret = -1;
    static bool first_time[] = {true, true, true, true, true};
    int const instance = context->instance;
    char * const * const values = context->values;
    char * const * const last_values = context->last_values;
    bool want_dhcp = context->dhcp_client_enabled && (strcmp(values[eth_state], OFF_STRING) != 0);
    bool dhcp_is_running = DHCP_CLIENT_NO_PID != dhcp_client_get_pid(eth_interface, instance);

//	if (strcmp(values[eth_state], "off") == 0)
//	{
		if (first_time[instance] || strcmp(last_values[eth_state], values[eth_state]) != 0)
		{
			if (eth_apply_state(instance, values[eth_state]))
				goto ERROR;
			strcpy(last_values[eth_state], values[eth_state]);
            first_time[instance] = false;
            if (strcmp(values[eth_state], "off") == 0 || strcmp(values[eth_state], "on-demand") == 0)
                goto OK;
		}
//	}
	if ((strcmp(last_values[eth_speed], values[eth_speed]) != 0) || (strcmp(last_values[eth_duplex], values[eth_duplex]) != 0))
	{
		if (eth_apply_speed_duplex(instance, values[eth_speed], values[eth_duplex]))
			goto ERROR;
		strcpy(last_values[eth_speed], values[eth_speed]);
		strcpy(last_values[eth_duplex], values[eth_duplex]);
	}
	if (strcmp(last_values[eth_mtu], values[eth_mtu]) != 0)
	{
		if (eth_apply_mtu(instance, values[eth_mtu]))
			goto ERROR;
		strcpy(last_values[eth_mtu], values[eth_mtu]);
	}
OK:
    if ((want_dhcp) && (!dhcp_is_running))
    {
        tlr_debug("eth_apply:  Starting DHCP client on eth%d\n", instance);
        if (dhcp_client_start(eth_interface, instance) == -1)
        {
            tlr_debug("eth_apply:  Unable to start DHCP client on eth%d\n", instance);
            goto ERROR;
        }
    }
    else if ((!want_dhcp) && (dhcp_is_running))
    {
        tlr_debug("eth_apply:  Trying to stop DHCP\n");
        if (dhcp_client_stop(eth_interface, instance) == -1)
        {
            tlr_debug("eth_apply:  Unable to stop DHCP client on eth%d\n", instance);
            goto ERROR;
        }
    }
	ret = 0;
ERROR:
	return ret;
}

STATIC void read_stat_registers(int instance)
{
	int phy_stat, part_1000, adv_1000;
	int adv, partner, phy_con;
	reg_read(instance, PHY_STAT, &phy_stat);
	reg_read(instance, PHY_AUTOADVERT, &adv);
	reg_read(instance, PHY_AUTOPART, &partner);
	reg_read(instance, PHY_CON, &phy_con);
	reg_read(instance, PHY_1000BASET_CTRL, &adv_1000);
	reg_read(instance, PHY_1000BASET_STAT, &part_1000);
}

int eth_apply_speed_duplex(int const instance, char const * const speed, char const * const duplex)
{

    int ret = -1;
    if (*speed == '\0')
        goto BYPASS;
    if (*duplex == '\0')
        goto BYPASS;

    config_eth_speed_t speed_want = config_eth_speed_auto;
    config_eth_duplex_t duplex_want = config_eth_duplex_auto;
    int adv, phy_con, ctrl_1000;
    int adv_want, ctrl_1000_want;

    if (strcmp(speed, "10") == 0)
        speed_want = config_eth_speed_10;
    else if (strcmp(speed, "100") == 0)
        speed_want = config_eth_speed_100;
    else if (strcmp(speed, "1000") == 0)
        speed_want = config_eth_speed_1000;

    if (strcmp(duplex, "half") == 0)
        duplex_want = config_eth_duplex_half;
    else if (strcmp(duplex, "full") == 0)
        duplex_want = config_eth_duplex_full;

    if (reg_read(instance, PHY_AUTOADVERT, &adv) < 0 ||
        reg_read(instance, PHY_1000BASET_CTRL, &ctrl_1000) < 0 ||
        reg_read(instance, PHY_CON, &phy_con) < 0)
        goto BYPASS;


    adv_want = adv | PHY_AUTOADV_100HALFDUP | PHY_AUTOADV_100FULLDUP | PHY_AUTOADV_10FULLDUP | PHY_AUTOADV_10HALFDUP;
    ctrl_1000_want = ctrl_1000 | PHY_CTRL_1000HALF | PHY_CTRL_1000FULL;

    switch (speed_want)
    {
    case config_eth_speed_10:
        adv_want &= ~(PHY_AUTOADV_100HALFDUP | PHY_AUTOADV_100FULLDUP);
        ctrl_1000_want &= ~(PHY_CTRL_1000HALF | PHY_CTRL_1000FULL);
        break;
    case config_eth_speed_100:
        adv_want &= ~(PHY_AUTOADV_10HALFDUP | PHY_AUTOADV_10FULLDUP);
        ctrl_1000_want &= ~(PHY_CTRL_1000HALF | PHY_CTRL_1000FULL);
        break;
    case config_eth_speed_1000:
        adv_want &= ~(PHY_AUTOADV_100HALFDUP | PHY_AUTOADV_100FULLDUP | PHY_AUTOADV_10HALFDUP | PHY_AUTOADV_10FULLDUP);
        break;
    case config_eth_speed_auto:
        break;
    }

    switch (duplex_want)
    {
    case config_eth_duplex_half:
        adv_want &= ~(PHY_AUTOADV_100FULLDUP | PHY_AUTOADV_10FULLDUP);
        ctrl_1000_want &= ~(PHY_CTRL_1000FULL);
        break;
    case config_eth_duplex_full:
        adv_want &= ~(PHY_AUTOADV_100HALFDUP | PHY_AUTOADV_10HALFDUP);
        ctrl_1000_want &= ~(PHY_CTRL_1000HALF);
        break;
    case config_eth_duplex_auto:
        break;
    }

    if (adv_want != adv || ctrl_1000_want != ctrl_1000)
    {
        sleep(1);
        reg_write(instance, PHY_AUTOADVERT, adv_want);
        reg_write(instance, PHY_1000BASET_CTRL, ctrl_1000_want);
        phy_con |= PHY_CON_RESTARTNEG;
        reg_write(instance, PHY_CON, phy_con);
    }

    read_stat_registers(instance);

BYPASS:
    ret = 0;
    // ERROR:
    return ret;
}

STATIC int init_parameter_storage(eth_context_t * const context)
{
	int 	i;

	context->values = calloc(sizeof(char *), eth_count);
	context->parameters[eth_duplex] = config_eth_duplex;
	context->parameters[eth_speed] = config_eth_speed;
	context->parameters[eth_mtu] = config_eth_mtu;
	context->parameters[eth_state] = config_eth_state;
	if (context->values == NULL)
		goto init_ERROR;

	for (i = 0; i < eth_count; i++)
	{
		context->values[i] = calloc(sizeof(char), CMD_LEN);
		if (context->values[i] == NULL)
			goto init_ERROR;
	}
	context->last_values = calloc(sizeof(char *), eth_count);
	context->last_parameters[eth_duplex] = config_eth_duplex;
	context->last_parameters[eth_speed] = config_eth_speed;
	context->last_parameters[eth_mtu] = config_eth_mtu;
	context->last_parameters[eth_state] = config_eth_state;
	if (context->last_values == NULL)
		goto init_ERROR;

	for (i = 0; i < eth_count; i++)
	{
		context->last_values[i] = calloc(sizeof(char), CMD_LEN);
		if (context->last_values[i] == NULL)
			goto init_ERROR;
	}
	if (eth_read_uci(context))
        goto init_ERROR;

    context->desired_eth_state = (strcmp(context->values[eth_state], "on") == 0)? true:false;

	return 0;
init_ERROR:

	if (context->values != NULL)
	{
		for (i = 0; i < eth_count; i++)
			if (context->values[i] != NULL)
				free(context->values[i]);
		free(context->values);
		context->values = NULL;
	}
	if (context->last_values != NULL)
	{
		for (i = 0; i < eth_count; i++)
			if (context->last_values[i] != NULL)
				free(context->last_values[i]);
		free(context->last_values);
		context->last_values = NULL;
	}
	return -1;
}

STATIC int read_and_apply_config(eth_context_t * const context)
{
    int result;

    if (context->values == NULL && init_parameter_storage(context) != 0)
    {
        result = -1;
        goto error;
    }

    if (eth_read_uci(context) != 0)
    {
        result = -1;
        goto error;
    }

    if (eth_apply(context) != 0)
    {
        result = -1;
        goto error;
    }

    result = 0;

error:
    return result;
}

int eth_config_update (eth_context_t * const context)
{
    tlr_debug("eth_config_update (): desired_eth_state = %d\n", context->desired_eth_state);
    DEBUG("eth_config_update (): desired_eth_state = %d\n", context->desired_eth_state);

    return read_and_apply_config(context);
}

int eth_do_connect_action (eth_context_t * const context)
{
    bool const desired_eth_state = context->desired_eth_state;
    int result;

    tlr_debug("eth_do_connect_action(): desired_eth_state = %d\n", context->desired_eth_state);

    context->desired_eth_state = true;
    result = read_and_apply_config(context);
    context->desired_eth_state = desired_eth_state;

    return result;
}

int eth_do_assigned_to_wan(eth_context_t * const context, wan_info_st const * const wan_info)
{
    tlr_debug("eth_do_assigned_to_wan: wan = %zu\n", wan_info->wan_instance);

    context->wan_instance = wan_info->wan_instance;
    return read_and_apply_config(context);
}

int eth_do_removed_from_wan(eth_context_t * const context, wan_info_st const * const wan_info)
{
    tlr_debug("eth_do_removed_from_wan: wan = %zu\n", wan_info->wan_instance);

    context->wan_instance = ETH_NOT_ASSIGNED_TO_A_WAN;
    return read_and_apply_config(context);
}

int eth_do_disconnect_action (eth_context_t * const context)
{
    bool const desired_eth_state = context->desired_eth_state;
    int result;

    tlr_debug("eth_do_disconnect_action(): desired_eth_state = %d\n", context->desired_eth_state);

    context->desired_eth_state = false;
    result = read_and_apply_config(context);
    context->desired_eth_state = desired_eth_state;

    return result;
}

int eth_do_joined_bridge_action(eth_context_t * const context)
{
    int result;

    tlr_debug("eth_do_joined_bridge_action(): instance=%d\n", (int)context->instance);

    if (context->is_member_of_bridge)
    {
        result = 0;
        goto error;
    }

    context->is_member_of_bridge = true;
    result = read_and_apply_config(context);

error:
    return result;
}

int eth_do_left_bridge_action(eth_context_t * const context)
{
    int result;

    tlr_debug("eth_do_left_bridge_action(): instance=%d\n", (int)context->instance);

    if (!context->is_member_of_bridge)
    {
        result = 0;
        goto error;
    }

    context->is_member_of_bridge = false;
    result = read_and_apply_config(context);

error:
    return result;
}

