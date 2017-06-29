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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "interfaces.h"
#include "messages.h"
#include "config_mgr.h"
#include "failover_show.h"
#include "ifmwan.h"

static ifm_config_t ifm_config;

STATIC_FUNCTION int ifm_get_state(config_group_t const group_id, size_t const instance, ifm_config_state_t * const state)
{
    int result = -1;
    ssize_t element_id;
    config_status_t status;
    ifm_config_state_t if_state;
    char value[sizeof "on-demand"];  /* @TODO: Use API to get native value */

    element_id = tlr_get_element_id(group_id, "state");
    if (element_id < 0) goto error;

    status = tlr_get_element_value(group_id, instance, element_id,
                                   value, sizeof value,
                                   config_mgr_element_type_string, config_mgr_access_level_read_only);
    if (status != config_status_success) goto error;

    // Turn the string value into an integer matching the element id.
    status = tlr_config_string_to_value(group_id, element_id, value, &if_state);
    if (status != config_status_success) goto error;

    *state = if_state;
    result = 0;

error:
    return result;
}

STATIC_FUNCTION ifm_if_config_t *create_interface_entry(ifm_if_info_t const * const info)
{
    ifm_if_config_t * const ptr = calloc(1, sizeof *ptr);

    if (ptr == NULL)
    {
        goto done;
    }

    ifm_debug("adding new interface entry for %s %zu\n", info->name, info->instance);

    ptr->info = *info;
    ptr->current_state = ifm_state_init;
    ptr->config_state = ifm_config_state_off;

    ptr->pnext = ifm_config.if_list;
    ifm_config.if_list = ptr;

done:
    return ptr;
}

int ifm_handle_state_change(ifm_if_config_t * const ptr, ifm_config_state_t const new_state)
{
    int result = -1;

    ifm_debug("%s %zu states: current[%d], configured[%d], new[%d]\n", ptr->info.name, ptr->info.instance, ptr->current_state, ptr->config_state, new_state);
    switch (new_state)
    {
        case ifm_config_state_on:
            switch (ptr->current_state)
            {
                case ifm_state_connecting:  /* a connect request has already been sent */
                    result = 0;
                    break;

                default:
                    if (ptr->current_state != ifm_state_connected)
                    {
                        ptr->current_state = ifm_state_connecting;
                    }
                    result = send_message_to_interface_monitor(ptr->info.interface, ptr->info.instance, connect_message, NULL);
                    ifm_debug("send connect to %s %zu %s.\n", ptr->info.name, ptr->info.instance, (result < 0) ? "failed" : "success");
                    break;
            }
            break;

        case ifm_config_state_on_demand:
        case ifm_config_state_off:
            switch (ptr->current_state)
            {
                case ifm_state_disconnecting:   /* a disconnect request has already been sent */
                    result = 0;
                    break;

                default:
                    if (ptr->current_state != ifm_state_disconnected)
                    {
                        ptr->current_state = ifm_state_disconnecting;
                    }
                    result = send_message_to_interface_monitor(ptr->info.interface, ptr->info.instance, disconnect_message, NULL);
                    ifm_debug("send disconnect to %s %zu %s.\n", ptr->info.name, ptr->info.instance, (result < 0) ? "failed" : "success");
                    break;
            }
            break;
    }

    return result;
}

ifm_if_config_t * ifm_get_interface_entry(ifm_if_info_t const * const info)
{
    ifm_if_config_t * ptr = ifm_config.if_list;

    while (ptr != NULL)
    {
        bool const interface_matched = (ptr->info.interface == info->interface);
        bool const instance_matched = (ptr->info.instance == info->instance);

        if (interface_matched && instance_matched)
            break;

        ptr = ptr->pnext;
    }
#if 0
    if (ptr == NULL)
    {
        ifm_debug("interface entry not found %s %zu.\n", info->name, info->instance);
    }
#endif
    return ptr;
}

int ifm_handle_config_change(ifm_update_info_t const * const info)
{
    int result;
    ifm_config_state_t configured_state;
    ifm_if_info_t if_info;

    if_info.instance = info->instance;
    if_info.interface = interface_get_type_from_config_group(info->config_group);
    if (if_info.interface == invalid_interface)
    {
        ifm_debug("couldn't get interface from config group %u\n", info->config_group);
        result = -1;
        goto error;
    }
    if_info.name = interface_get_name_from_type(if_info.interface);

    ifm_if_config_t * ptr = ifm_get_interface_entry(&if_info);

    if (ptr == NULL)
    {
        ptr = create_interface_entry(&if_info);
        if (ptr == NULL)
        {
            result = -1;
            goto error;
        }
    }

    if (ifm_get_state(info->config_group, info->instance, &configured_state) != 0)
    {
        result = -1;
        goto error;
    }

    if (ptr->config_state == configured_state)
    {
        result = 0;
        goto error;
    }

    result = ifm_handle_state_change(ptr, configured_state);

    if (result == 0)
    {
        ptr->config_state = configured_state;
        failover_handler(&if_info);
    }

error:
    ifm_info("update %s %zu %s\n", info->name, info->instance, (result == 0) ? "success" : "failed");
    return result;
}

int ifm_init(timer_context_st * const context)
{
    int result;
    size_t const queue_size = 64;

    //ifm_config.timer_context = context;
    ifm_config.if_list = NULL;

    ifm_config.event_manager = new_event_manager(context, queue_size, ifm_event_handler);
    if (ifm_config.event_manager == NULL)
    {
        ifm_error("failed to create event manager\n");
        result = -1;
        goto error;
    }

    if (failover_init(context) != 0)
    {
        ifm_error("failed to initialize failover\n");
        result = -1;
        goto error;
    }

    if (wan_init() != 0)
    {
        ifm_error("failed to initialize wan\n");
        result = -1;
        goto error;
    }

    if (show_init(ifm_get_context()) != 0)
    {
        ifm_error("failed to create failover_show plain socket\n");
        result = -1;
        goto error;
    }

    result = 0;

error:
    return result;
}

ifm_config_t * ifm_get_context(void)
{
    return &ifm_config;
}
