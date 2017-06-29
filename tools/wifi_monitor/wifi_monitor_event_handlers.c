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

#include "wifi_monitor_event_handlers.h"
#include "wifi_monitor_init.h"
#include "wifi_monitor_config.h"
#include "wifi_monitor_timers.h"
#include "wifi_monitor_ifconfig_commands.h"
#include "wifi_monitor_log.h"
#include "messages.h"
#include "tlr_assert.h"
#include "tlr_print.h"
#include "wifi_radio.h"

#include <stdlib.h>

static bool send_message_to_interface_manager(message_codes_t const message_code, size_t const interface_instance)
{
	int result = false;
	char * buffer = NULL;

	if (asprintf(&buffer, "%s %zd", wifi_interface_name_get(), interface_instance) == -1)
	{
		wifi_monitor_debug("could not prepare message\n");
		goto done;
	}

	if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, message_code, buffer, NULL) != 0)
	{
		wifi_monitor_debug("could not send message to interface manager\n");
		goto done;
	}

	result = true;

done:
	free(buffer);

	return result;
}

static void wifi_update_configuration(wifi_context_st * const context)
{
    wifi_common_elements_t * const new_config = wifi_get_configuration(context->instance);

    if (wifi_config_check_for_change(context->instance, context->config, new_config))
    {
        /* replace the old config with the new one. */
        wifi_free_configuration(context->config);
        context->config = new_config;
        /* update the system with the latest settings. */
        wifi_config_has_changed(context, context->config);
    }
    else
    {
        wifi_free_configuration(new_config);
    }
}

void wifi_handle_update_event(wifi_context_st * const context)
{
    wifi_update_configuration(context);
}

void wifi_handle_ping_response_timeout_event(wifi_context_st * const context)
{
    if (context->saved_ping_response_info != NULL)
    {
        ping_info_st const * const ping_info = context->saved_ping_response_info;

        (void)send_pong_message(ping_info->pong_destination, ping_info->message_id);
        free((void *)context->saved_ping_response_info);
        context->saved_ping_response_info = NULL;
    }
}

void wifi_handle_ping_message_event(wifi_context_st * const context, wifi_monitor_event_t * const event)
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
        wifi_ping_response_timer_start(context, pinfo->please_respond_time);
    }
    // else must have previously received a PING request and are still waiting for the timer to elapse
}

void wifi_handle_connect_event(wifi_context_st * const context)
{
    size_t const instance = context->instance;

    context->desired_state = wifi_state_connected;

    switch (context->current_state)
    {
        case wifi_state_disconnected:
        {
            if (wifi_radio_connect(instance))
            {
                wifi_handle_connected_event(context);
            }
            else
            {
                wifi_handle_disconnected_event(context);
            }
            break;
        }
        case wifi_state_connected:
            send_message_to_interface_manager(connected_message, instance);
            break;
    }
}

void wifi_handle_disconnect_event(wifi_context_st * const context)
{
    size_t const instance = context->instance;

    context->desired_state = wifi_state_disconnected;

    switch (context->current_state)
    {
        case wifi_state_disconnected:
            send_message_to_interface_manager(disconnected_message, instance);
            break;
        case wifi_state_connected:
        {
            wifi_radio_disconnect(instance);
            wifi_handle_disconnected_event(context);
            break;
        }
    }
}

void wifi_handle_connected_event(wifi_context_st * const context)
{
    if (context->current_state != wifi_state_connected)
    {
        tlr_event_log(tlr_log_notice, "apply wifi", "wifi %zd SSID %s started\n", context->instance,
                      context->config != NULL ? context->config->wifi_param_ssid : "");
        context->current_state = wifi_state_connected;
        send_message_to_interface_manager(connected_message, context->instance);
    }
}

void wifi_handle_disconnected_event(wifi_context_st * const context)
{
    if (context->current_state != wifi_state_disconnected)
    {
        tlr_event_log(tlr_log_notice, "apply wifi", "wifi %zd SSID %s stopped\n", context->instance,
                      context->config != NULL ? context->config->wifi_param_ssid : "");
        context->current_state = wifi_state_disconnected;
        send_message_to_interface_manager(disconnected_message, context->instance);
    }
}

