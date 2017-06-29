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

#include <stdlib.h>
#include <stdio.h>
#include "ifmanager.h"
#include "tlr_print.h"
#include "config_mgr.h"
#include "config_def.h"
#include "failover.h"
#include "route.h"
#include "ifmwan.h"
#include "unittest_support.h"


#define INVALID_ETHERNET_INTERFACE  0

struct ifm_wan_config_struct
{
    config_wan_interface_t interface;
    bool dhcp_on;
};

STATIC struct ifm_wan_config_struct * wan_config = NULL;
STATIC size_t wan_count = 0;

STATIC size_t translate_wan_interface_to_ethernet_instance(config_wan_interface_t const interface)
{
    size_t ethernet_instance;

    switch(interface)
    {
        case config_wan_interface_eth1:
            ethernet_instance = 1;
            break;
        case config_wan_interface_eth2:
            ethernet_instance = 2;
            break;
        case config_wan_interface_eth3:
            ethernet_instance = 3;
            break;
        case config_wan_interface_eth4:
            ethernet_instance = 4;
            break;
        default:
            ethernet_instance = INVALID_ETHERNET_INTERFACE;
            break;
    }

    return ethernet_instance;
}

static config_wan_interface_t translate_interface_and_instance_to_wan_interface(interfaces_t const interface, size_t instance)
{
    config_wan_interface_t result;

    switch (interface)
    {
        case eth_interface:
            switch (instance)
            {
                case 1:
                    result = config_wan_interface_eth1;
                    break;
                case 2:
                    result = config_wan_interface_eth2;
                    break;
                case 3:
                    result = config_wan_interface_eth3;
                    break;
                case 4:
                    result = config_wan_interface_eth4;
                    break;
                default:
                    result = config_wan_interface_none;
                    break;
            }
            break;
        case dsl_interface:
            result = config_wan_interface_dsl;
            break;
        case cellular_interface:
            switch (instance)
            {
                case 1:
                    result = config_wan_interface_cellular1;
                    break;
                case 2:
                    result = config_wan_interface_cellular2;
                    break;
                default:
                    result = config_wan_interface_none;
                    break;
            }
            break;
        default:
            result = config_wan_interface_none;
            break;
    }

    return result;
}

STATIC bool is_ethernet_interface(config_wan_interface_t const interface)
{
    return translate_wan_interface_to_ethernet_instance(interface) != INVALID_ETHERNET_INTERFACE;
}

STATIC config_wan_interface_t wan_read_interface(size_t const instance)
{
    config_wan_interface_t interface = config_wan_interface_none;

    config_status_t const read_status = tlr_get_element_value(config_wan, instance, config_wan_interface,
                                                (void *)&interface, sizeof(interface),
                                                config_mgr_element_type_enum,
                                                config_mgr_access_level_super);
    if (read_status == config_status_success)
    {
        ifm_debug("wan_read_interface interface = %u\n", interface);
    }
    else
    {
        ifm_debug("wan_read_interface couldn't read interface: %d\n", read_status);
        interface = config_wan_interface_none;
    }

    return interface;
}

STATIC bool wan_read_dhcp_client_state(size_t const instance)
{
    unsigned int on_off = 0;

    config_status_t const read_status = tlr_get_element_value(config_wan, instance,
                                                        config_wan_dhcp, (void *)&on_off,
                                                        sizeof(on_off), config_mgr_element_type_on_off,
                                                        config_mgr_access_level_super);
    if (read_status == config_status_success)
    {
        ifm_debug("wan_read_dhcp_client_state on_off = %u\n", on_off);
    }
    else
    {
        ifm_debug("wan_read_dhcp_client_state couldn't read dhcp setting: %d\n", read_status);
        on_off = 0;
    }

    return (on_off != 0);
}

STATIC int send_removed_message(size_t const wan_instance, config_wan_interface_t const old_interface)
{
    char * msg = NULL;
    int result = -1;
    size_t eth_instance = translate_wan_interface_to_ethernet_instance(old_interface);

    ifm_debug("send_removed_message(%zu, %u) called\n", wan_instance, old_interface);
	if (asprintf(&msg, "%zu", wan_instance) == -1)
	{
        ifm_debug("send_removed_message: Could not allocate memory in asprintf\n");
        goto error;
	}

    if (send_message(ethernet_monitor, eth_instance, removed_from_wan_message, msg, NULL) == -1)
    {
        ifm_debug("send_removed_message: Unable to send removed_from_wan_message to eth %zu", eth_instance);
        goto error;
    }
    result = 0;

error:
    free(msg);
    return result;
}

STATIC int send_assigned_message(size_t const wan_instance, config_wan_interface_t const new_interface)
{
    char * msg = NULL;
    int result = -1;
    size_t eth_instance = translate_wan_interface_to_ethernet_instance(new_interface);

    ifm_debug("send_assigned_message(%zu, %u) called\n", wan_instance, new_interface);
	if (asprintf(&msg, "%zu", wan_instance) == -1)
	{
        ifm_debug("send_assigned_message: Could not allocate memory in asprintf\n");
        goto error;
	}

    if (send_message(ethernet_monitor, eth_instance, assigned_to_wan_message, msg, NULL) == -1)
    {
        ifm_debug("send_assigned_message: Unable to send assigned_to_wan_message to eth %zu", eth_instance);
        goto error;
    }
    result = 0;

error:
    free(msg);
    return result;
}

STATIC int send_config_updated(config_wan_interface_t const interface)
{
    char const * const config_group = get_config_group_name_from_id(config_eth);
    size_t instance = translate_wan_interface_to_ethernet_instance(interface);
    int result = -1;
    char * msg = NULL;

    ifm_debug("send_config_updated(%u) called\n", interface);
    if (asprintf(&msg, "%s %zu", config_group, instance) == -1)
	{
        ifm_debug("send_config_updated: Could not allocate memory in asprintf\n");
        goto error;
	}
    if (send_message(ethernet_monitor, instance, update_message, msg, NULL))
    {
        ifm_debug("send_config_updated: Unable to send update_message to eth %zu", instance);
        goto error;
    }

    result = 0;

error:
    free(msg);
    return result;
}

int wan_handle_config_change(ifm_update_info_t const * const info)
{
    config_wan_interface_t new_interface;
    config_wan_interface_t old_interface;
    bool new_dhcp_on;
    bool old_dhcp_on;
    bool interface_changed;
    bool dhcp_changed;
    int result = -1;

    ifm_debug("wan_handle_config_change called: name = %s, instance = %zu\n", info->name, info->instance);

    if (INSTANCE_TO_INDEX(info->instance) > wan_count)
    {
        ifm_debug("wan_handle_config_change: wan instance = %zu, but wan_count = %zu", info->instance, wan_count);
        goto done;
    }

    new_interface = wan_read_interface(info->instance);
    old_interface = wan_config[INSTANCE_TO_INDEX(info->instance)].interface;
    new_dhcp_on = wan_read_dhcp_client_state(info->instance);
    old_dhcp_on = wan_config[INSTANCE_TO_INDEX(info->instance)].dhcp_on;
    interface_changed = old_interface != new_interface;
    dhcp_changed = old_dhcp_on != new_dhcp_on;

    wan_config[INSTANCE_TO_INDEX(info->instance)].interface = new_interface;
    wan_config[INSTANCE_TO_INDEX(info->instance)].dhcp_on = new_dhcp_on;

    if (interface_changed)
    {
        if (is_ethernet_interface(old_interface))
        {
            result = send_removed_message(info->instance, old_interface);
        }
        if (is_ethernet_interface(new_interface))
        {
            result = send_assigned_message(info->instance, new_interface);
        }
    }
    else if ((dhcp_changed) && (is_ethernet_interface(old_interface)))
    {
        result = send_config_updated(old_interface);
    }
    else
    {
        result = 0;
    }

done:
    return result;
}

STATIC int wan_set_default_route(interfaces_t const interface, size_t const instance, char const * const gateway, size_t const metric)
{
    int result = -1;
    char * command = NULL;

    if (asprintf(&command, "ip route add default via %s metric %zu dev %s%zu  2>&1", gateway, metric,
                 interface_get_name_from_type(interface), instance) < 0)
    {
        ifm_debug("wan_set_default_route: unable to allocate memory for command string\n");
        goto done;
    }

    if (route_add_error == ifm_execute_route_command(command))
    {
        ifm_debug("wan_set_default_route: command '%s' failed\n", command);
        goto done;
    }

    result = 0;

done:
    free(command);
    return result;
}

int wan_handle_gateway_assignment(dhcp_assigned_gateway_st const * const gateway_info)
{
    config_wan_interface_t wan_interface = translate_interface_and_instance_to_wan_interface(gateway_info->interface, gateway_info->instance);
    int result = 0;

    if (wan_interface != config_wan_interface_none)
    {
        unsigned int wan_index;

        for (wan_index = 0; wan_index < wan_count; wan_index++)
        {
            if (wan_config[wan_index].interface == wan_interface)
            {
                result = wan_set_default_route(gateway_info->interface, gateway_info->instance, gateway_info->gateway, INDEX_TO_INSTANCE(wan_index));
                break;
            }
        }
    }

    return result;
}

void wan_close(void)
{
    wan_count = 0;
    if (wan_config != NULL)
    {
        free(wan_config);
        wan_config = NULL;
    }
}

STATIC void wan_set_number_of_contexts(size_t const contexts_count)
{
    wan_count = contexts_count;
}

STATIC void wan_set_contexts(struct ifm_wan_config_struct * const contexts)
{
    wan_config = contexts;
}

int wan_init(void)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    size_t contexts_count;
    struct ifm_wan_config_struct * contexts;
    size_t wan_index;
    int result = -1;

    ifm_debug("wan_init called\n");

    contexts_count = table->groups[config_wan].instances;

    if (contexts_count == 0)
    {
        ifm_debug("wan_init: WAN not supported\n");
        goto done;
    }


    contexts = calloc(contexts_count, sizeof *contexts);
    if (contexts == NULL)
    {
        ifm_debug("wan_init: calloc failed for wan context)\n");
        goto done;
    }

    wan_set_number_of_contexts(contexts_count);
    wan_set_contexts(contexts);

    for (wan_index = 0; wan_index < contexts_count; wan_index++)
    {
        size_t const instance = INDEX_TO_INSTANCE(wan_index);

        contexts[wan_index].interface = wan_read_interface(instance);
        contexts[wan_index].dhcp_on = wan_read_dhcp_client_state(instance);
        if (is_ethernet_interface(contexts[wan_index].interface))
        {
            if (send_assigned_message(instance, contexts[wan_index].interface) != 0)
            {
                ifm_debug("wan_init: Unable to assign eth%zu to wan%zu\n",
                          translate_wan_interface_to_ethernet_instance(contexts[wan_index].interface),
                          instance);
            }
        }
    }

    result = 0;

done:
    if (result != 0)
    {
        wan_close();
    }
    return result;
}
