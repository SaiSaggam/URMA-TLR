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

#include "bridge_monitor.h"
#include "bridge_monitor_init.h"
#include "bridge_monitor_event.h"
#include "bridge_monitor_config.h"
#include "bridge_monitor_timers.h"
#include "bridge_monitor_config_commands.h"
#include "bridge_monitor_log.h"
#include "bridge_dns.h"
#include "messages.h"
#include <stdlib.h>
#include <assert.h>

static int send_message_to_interface_manager(message_codes_t const message_code, size_t interface_instance)
{
    int result = EXIT_FAILURE;
	char * buffer = NULL;

    if (asprintf(&buffer, "%s %zd", bridge_interface_name_get(), interface_instance) == -1 ||
    	buffer == NULL)
    {
    	bridgemon_error("could not prepare message\n");
    	goto done;
    }

    if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, message_code, buffer, NULL) != 0)
    {
    	bridgemon_error("could not send message to interface manager\n");
    	goto done;
    }

	result = EXIT_SUCCESS;

done:
	if (buffer != NULL)
		free(buffer);

    return result;
}

static void bridge_context_init(size_t const instance)
{
    bridge_context_st * const context = bridge_get_context(instance);

    assert(context != NULL);

    context->config = config_alloc(config_lan);

    if (context->config == NULL)
    {
        bridgemon_error("failed to alloc %s %zu config at init time\n", bridge_interface_name_get(), context->instance);
    }
}

/*
 * Cycle through all contexts and perform initialisation work
 * e.g. load configuration.
 */
void bridge_handle_initialise_event(void)
{
    size_t num_contexts = bridge_get_num_contexts();
    int index;

    for (index = 0; index < num_contexts; index++)
    {
        size_t instance = index + FIRST_BRIDGE_INSTANCE;

        bridge_context_init(instance);
    }
}

static void bridge_update_configuration(bridge_context_st * const context)
{
    bool config_has_changed;

    if (config_update_and_compare_with_previous(&context->config,
                                                context->instance,
                                                bridge_config_check_for_change,
                                                &config_has_changed) == 0)
    {
        bridgemon_debug("config for %s %zu has %schanged\n", bridge_interface_name_get(), context->instance, config_has_changed ? "" : "not ");
    }
    else
    {
        bridgemon_error("config update for %s %zu has failed\n", bridge_interface_name_get(), context->instance);
    }
}

void bridge_handle_update_event(bridge_context_st * const context)
{
    bridge_update_configuration(context);
}

void bridge_handle_ping_response_timeout_event(bridge_context_st * const context)
{
    bridge_ping_response_timer_expired(context);
    if (context->saved_ping_response_info != NULL)
    {
        ping_info_st const * const ping_info = context->saved_ping_response_info;

        (void)send_pong_message(ping_info->pong_destination, ping_info->message_id);
        free((void *)context->saved_ping_response_info);
        context->saved_ping_response_info = NULL;
    }
}

void bridge_handle_ping_message_event(bridge_context_st * const context, bridge_monitor_event_t * const event)
{
    ping_info_st const * const pinfo = event->info;

    if (pinfo->please_respond_time == 0)
    {
        (void)send_pong_message(pinfo->pong_destination, pinfo->message_id);
    }
    else if (context->saved_ping_response_info == NULL)
    {
        context->saved_ping_response_info = pinfo;
        event->info = NULL;
        bridge_ping_response_timer_start(context, pinfo->please_respond_time);
    }
    // else must have previously received a PING request and are still waiting for the timer to elapse
}

void bridge_handle_connect_event(bridge_context_st * const context)
{
    size_t instance = context->instance;

    bridgemon_debug("%s %zu has got connect request\n", bridge_interface_name_get(), instance);

    context->desired_state = bridge_state_connected;

    switch (context->current_state)
    {
        case bridge_state_disconnected:
        {
            bool const interface_connected = bridge_connect_interface(instance);
            bool add_interfaces;

            if (interface_connected)
            {
                add_interfaces = bridge_check_if_dhcp_client_needed(instance);
            }
            else
            {
                add_interfaces = false;
            }
            if (add_interfaces && bridge_config_add_interfaces(instance))
            {
                context->current_state = bridge_state_connected;
                bridge_update_configuration(context);
                send_message_to_interface_manager(connected_message, instance);
            }
            break;
        }
        case bridge_state_connected:
            send_message_to_interface_manager(connected_message, instance);
            break;
    }
}

void bridge_handle_disconnect_event(bridge_context_st * const context)
{
    size_t instance = context->instance;

    bridgemon_debug("%s %zu has got disconnect request in state %u\n", bridge_interface_name_get(), context->instance, context->current_state);

    context->desired_state = bridge_state_disconnected;

    switch (context->current_state)
    {
        case bridge_state_disconnected:
            send_message_to_interface_manager(disconnected_message, instance);
            break;
        case bridge_state_connected:
        {
            bool interface_disconnected;

            bridge_interface_set_dhcp_client_off(instance);

            interface_disconnected = bridge_disconnect_interface(instance);

            bridge_refresh_dns_configuration(instance);

            if (interface_disconnected && bridge_config_delete_interfaces(instance))
            {
                context->current_state = bridge_state_disconnected;
                send_message_to_interface_manager(disconnected_message, instance);
            }
            break;
        }
    }
}

void bridge_handle_connected_event(bridge_context_st * const context)
{
    bridgemon_debug("%s %zu has connected\n", bridge_interface_name_get(), context->instance);
}

void bridge_handle_disconnected_event(bridge_context_st * const context)
{
    bridgemon_debug("%s %zu has disconnected\n", bridge_interface_name_get(), context->instance);
    // XXX should the monitor attempt to reconnect?
    /* will this ever occurr unexpectedly (i.e. not as a result of a config change)? It's not like
     * these interfaces connect to something like cellular, which might disconnect of their own
     * accord.
     */
}

