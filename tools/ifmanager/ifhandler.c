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
#include <messages.h>
#include <stdlib.h>
#include <string.h>
#include "route.h"
#include "failover_show.h"
#include "dns_handle.h"
#include "ifmwan.h"

#ifdef _CUNIT_
int ifm_unit_test_result = 0;
#endif

/*
 * These strings are only referenced sometimes, and generate errors if they're not. Only declare
 * them if necessary.
 */
#if !defined(_CUNIT_) || IFMANAGER_DEBUG
static char const * event_code_strings[] =
{
        FOREACH_EVENT_CODE(GENERATE_STRING)
};
#endif

char const * state_strings[] =
{
        FOREACH_STATE(GENERATE_STRING)
};

STATIC_FUNCTION void ifm_free_event(ifm_event_t * const event)
{
    if (event != NULL)
    {
        free(event);
    }
}

STATIC_FUNCTION int ifm_event_connected(ifm_if_info_t const * const info)
{
    int result = -1;
	ifm_if_config_t * const ifptr = ifm_get_interface_entry(info);

    if (ifptr == NULL)
    {
        ifm_debug("%s %zu no entry found\n", info->name, info->instance);
        goto error;
    }

    ifm_debug("%s %zu %s >> connected\n", info->name, info->instance, state_strings[ifptr->current_state]);

    {
        ifm_current_state_t prev_state = ifptr->current_state;

        ifptr->current_state = ifm_state_connected;

        switch (prev_state)
        {
            //case ifm_state_connecting:
            case ifm_state_connected:
                break;
            default:
                failover_handler(info);
                break;
        }
    }

    /* TODO: active failover */
    if (!failover_inhibit_routes(info) && ifm_apply_routes(failover_get_interface_name(&ifptr->info)))
    {
        goto error;
    }

    result = 0;

error:
    return result;
}

STATIC_FUNCTION int ifm_event_disconnected(ifm_if_info_t const * const info)
{
    int result = -1;
    ifm_if_config_t * const ifptr = ifm_get_interface_entry(info);

    if (ifptr == NULL)
    {
        ifm_debug("%s %zu no entry found\n", info->name, info->instance);
        goto error;
    }

    ifm_debug("%s%zu %s >>> disconnected\n", info->name, info->instance, state_strings[ifptr->current_state]);

    {
        ifm_current_state_t prev_state = ifptr->current_state;

        ifptr->current_state = ifm_state_disconnected;

        switch (prev_state)
        {
            //case ifm_state_disconnecting:
            case ifm_state_disconnected:
                break;

            default:
                failover_handler(info);
                break;
        }

    }

    result = 0;

error:
    return result;
}

STATIC_FUNCTION int ifm_event_joined_bridge(ifm_if_info_t const * const info)
{
    int result;
    ifm_if_config_t * const ifptr = ifm_get_interface_entry(info);

    if (ifptr == NULL)
    {
        ifm_debug("%s %zu no entry found\n", info->name, info->instance);
        result = -1;
        goto error;
    }

    ifm_debug("%s%zu %s >>> joined_bridge\n", info->name, info->instance, state_strings[ifptr->current_state]);

    // Nothing to do; Linux automatically deletes static routes associated
    // with an unconfigured interface.

    result = 0;

error:
    return result;
}

STATIC_FUNCTION int ifm_event_left_bridge(ifm_if_info_t const * const info)
{
    int result;
    ifm_if_config_t * const ifptr = ifm_get_interface_entry(info);

    if (ifptr == NULL)
    {
        ifm_debug("%s %zu no entry found\n", info->name, info->instance);
        result = -1;
        goto error;
    }

    ifm_debug("%s%zu %s >>> left_bridge\n", info->name, info->instance, state_strings[ifptr->current_state]);

    result = ifm_apply_routes(failover_get_interface_name(&ifptr->info));

error:
    return result;
}

STATIC_FUNCTION int ifm_event_config_change(ifm_update_info_t const * const info)
{
    int result = -1;

    switch (info->config_group)
    {
        case config_wan:
            ifm_debug("update wan %d configuration\n", info->instance);
            result = wan_handle_config_change(info);
            break;
        case config_failover:
            ifm_debug("update failover %d configuration\n", info->instance);
            result = failover_handle_config_change(info);
            break;

        /* interface group configuration changes */
        case config_dsl:
            /* for debug only */
            {
                ifm_config_t * const ifm_config = ifm_get_context();
                ifm_if_config_t * ptr = ifm_config->if_list;

                while (ptr != NULL)
                {
                    ifm_debug("INTERFACE (%d) %s %d in %s\n", ptr->info.interface, ptr->info.name, ptr->info.instance, state_strings[ptr->current_state]);
                    ptr = ptr->pnext;
                }
            }
        case config_eth:
        case config_cellular:
        case config_ipsec:
        case config_lan:
        case config_wifi:
            ifm_debug("update config interface %d %zd\n", info->config_group, info->instance);
            result = ifm_handle_config_change(info);
            break;
        case config_defroute:
        case config_route:
            ifm_debug("update config route %zd\n", info->instance);
            result = ifm_update_route(info);
            break;
        default:    /* TODO: route, defroute etc */
            ifm_debug("got unexpected config change\n");

            break;
    }

    return result;
}

STATIC int ifm_event_dhcp_assigned_gateway(dhcp_assigned_gateway_st const * const gateway_info)
{
    return wan_handle_gateway_assignment(gateway_info);
}

void ifm_event_handler(void * const user_context, void * const data)
{
    ifm_event_t * const event = data;
    int result = -1;

    switch (event->code)
    {
        case ifm_event_code_connected:
        {
            ifm_if_info_t const * const pinfo = event->info;

            ifm_debug("EVENT %s for %s %zu\n", event_code_strings[event->code], pinfo->name, pinfo->instance);
            result = ifm_event_connected(pinfo);
            if (create_dns_link() < 0)
            {
                tlr_debug("create dns: could not create dns links");
            }
            break;
        }

        case ifm_event_code_disconnected:
        {
            ifm_if_info_t const * const pinfo = event->info;

            ifm_debug("EVENT %s for %s %zu\n", event_code_strings[event->code], pinfo->name, pinfo->instance);
            result = ifm_event_disconnected(pinfo);
            if (create_dns_link() < 0)
            {
                tlr_debug("create dns: could not create dns links");
            }
            break;
        }

        case ifm_event_code_update:
        {
            ifm_update_info_t const * const pinfo = event->info;

            ifm_debug("EVENT %s for %s %zu\n", event_code_strings[event->code], pinfo->name, pinfo->instance);
            result = ifm_event_config_change(pinfo);
            break;
        }

        case ifm_event_code_timer_timeout:
        {
            failover_event_info_t * const event_info = event->info;

            ifm_debug("EVENT %s for failover%zu\n", event_code_strings[event->code], event_info->instance);
            result = failover_timeout_timer_timeout(event_info->failover);
            break;
        }
        case ifm_event_code_probe_interval_timeout:
        {
            failover_event_info_t * const event_info = event->info;

            ifm_debug("EVENT %s for failover%zu\n", event_code_strings[event->code], event_info->instance);
            result = failover_probe_interval_timeout(event_info->failover);
            break;
        }
        case ifm_event_code_probe_timeout:
        {
            failover_event_info_t * const event_info = event->info;

            ifm_debug("EVENT %s for failover%zu \n", event_code_strings[event->code], event_info->instance);

            result = failover_probe_timeout(event_info->failover);
            break;
        }
        case ifm_event_code_probe_success:
        {
            probe_success_info_st * const success_info = (probe_success_info_st *) event->info;

            //ifm_debug("EVENT %s for failover %zu FAILOVER\n", event_code_strings[event->code], success_info->failover->config.instance);
            result = failover_probe_success(success_info);
            break;
        }
        case ifm_event_code_show:
        {
            ifm_config_t const * const context = ifm_get_context();
            int const socket_fd = * (int const *) event->info;

            ifm_debug("EVENT %d for %s:%zu\n", event->code, "failover show plain socket", socket_fd);
            if (socket_fd >= 0)
            {
                send_failover_status(context, socket_fd);
                result = 0;
            }
            break;
        }
        case ifm_event_code_switch_primary_up_timeout:
        {
            failover_event_info_t * const event_info = event->info;

            ifm_debug("EVENT %s for failover%zu\n", event_code_strings[event->code], event_info->instance);
            result = failover_switch_primary_up_timeout(event_info->failover);
            break;
        }
        case ifm_event_code_joined_bridge:
        {
            ifm_if_info_t const * const pinfo = event->info;

            ifm_debug("EVENT %s for %s %zu\n", event_code_strings[event->code], pinfo->name, pinfo->instance);
            result = ifm_event_joined_bridge(pinfo);
            break;
        }

        case ifm_event_code_left_bridge:
        {
            ifm_if_info_t const * const pinfo = event->info;

            ifm_debug("EVENT %s for %s %zu\n", event_code_strings[event->code], pinfo->name, pinfo->instance);
            result = ifm_event_left_bridge(pinfo);
            break;
        }

        case ifm_event_code_dhcp_assigned_gateway:
        {
            dhcp_assigned_gateway_st const * const gateway_info = event->info;

            ifm_debug("EVENT %s for %s %zu, gateway = %s\n", event_code_strings[event->code],
                      interface_get_name_from_type(gateway_info->interface), gateway_info->instance,
                      gateway_info->gateway);
            result = ifm_event_dhcp_assigned_gateway(gateway_info);
            break;
        }
    }

    ifm_free_event(event);

#ifdef _CUNIT_
    ifm_unit_test_result = result;
#endif

    if (result != 0)
    {
        ifm_debug("couldn't process event\n");
    }
}
