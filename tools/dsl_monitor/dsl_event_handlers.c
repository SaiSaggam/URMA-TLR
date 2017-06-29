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

#include "dsl_event_handlers.h"
#include "dsl_context.h"
#include "dsl_event.h"
#include "dsl_config.h"
#include "dsl_modem.h"
#include "dsl_monitor.h"
#include "dsl_ppp.h"
#include "dsl_log.h"
#include "dsl_timer.h"
#include "messages.h"
#include "timer.h"
#include "tlr_common.h"

#include <stdlib.h>

#define MAX_CONNECTING_TIME_MILLISECONDS    seconds_to_milliseconds(30)
#define MAX_DISCONNECTING_TIME_MILLISECONDS	seconds_to_milliseconds(30)

static void do_connect_action(dsl_context_t * const context);
static void do_disconnect_action(dsl_context_t * const context);

static bool send_message_to_interface_manager(message_codes_t const message_code, size_t instance)
{
    bool message_sent = false;
	char * buffer = NULL;

    if (asprintf(&buffer, "%s %zd", dsl_monitor_interface_name(), instance) == -1 || buffer == NULL)
    {
    	dsl_error("could not prepare message\n");
    	goto done;
    }

    if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, message_code, buffer, NULL) != 0)
    {
    	dsl_error("could not send message to interface manager\n");
    	goto done;
    }

    message_sent = true;

done:
    free(buffer);

    return message_sent;
}

static void set_state(dsl_context_t * const context, dsl_state_t const new_state)
{
#if DSL_LOG_MODE
	dsl_state_t const old_state = context->current_state;
#endif

	dsl_debug("state change from '%s' to '%s'\n", dsl_state_name_from_value(old_state), dsl_state_name_from_value(new_state));

	context->current_state = new_state;

	switch (new_state)
	{
		case dsl_state_disconnected:
	        if (dsl_timer_is_running(&context->connecting_timer))
	        {
                dsl_timer_stop(&context->connecting_timer);
			}

	        if (dsl_timer_is_running(&context->delay_up_timer))
	        {
                dsl_timer_stop(&context->delay_up_timer);
			}

            if (dsl_timer_is_running(&context->disconnecting_timer))
            {
    			dsl_timer_stop(&context->disconnecting_timer);
            }

			if (context->notified_state != dsl_state_disconnected)
			{
				send_message_to_interface_manager(disconnected_message, context->instance);
				context->notified_state = dsl_state_disconnected;
				dsl_modem_config->is_ppp_up = false;
			}

			if (context->is_line_up && context->desired_state == dsl_state_connected)
			{
				do_connect_action(context);
			}
			break;

		case dsl_state_connecting:
            // Nothing to do.
            break;

        case dsl_state_connected_delay_up:
            if (dsl_timer_is_running(&context->connecting_timer))
            {
			    dsl_timer_stop(&context->connecting_timer);
            }

            dsl_timer_start(&context->delay_up_timer, seconds_to_milliseconds(context->delay_up_timer_period));
            break;

		case dsl_state_connected:
			if (context->notified_state != dsl_state_connected)
			{
				send_message_to_interface_manager(connected_message, context->instance);
				context->notified_state = dsl_state_connected;
				dsl_modem_config->is_ppp_up = true;
			}
			break;

		case dsl_state_disconnecting:
            if (dsl_timer_is_running(&context->delay_up_timer))
            {
			    dsl_timer_stop(&context->delay_up_timer);
            }
			break;
	}
}

static void do_connect_action(dsl_context_t * const context)
{
	while (dsl_modem_config->changed_mode || dsl_modem_config->changed_vpi_vci_encapsulation)
	{
		dsl_debug("waiting 1 second for DSL chipset configuration to be pushed\n");
		sleep(1);
	}

	dsl_ppp_start(context);

	dsl_timer_start(&context->connecting_timer, MAX_CONNECTING_TIME_MILLISECONDS);

	set_state(context, dsl_state_connecting);
}

static void do_disconnect_action(dsl_context_t * const context)
{
	dsl_ppp_stop(context);

	dsl_timer_start(&context->disconnecting_timer, MAX_DISCONNECTING_TIME_MILLISECONDS);

	set_state(context, dsl_state_disconnecting);
}

void dsl_event_line_up_handler(dsl_context_t * const context)
{
    context->is_line_up = true;

    switch (context->current_state)
    {
        case dsl_state_disconnected:
            if (context->desired_state == dsl_state_connected)
            {
        		do_connect_action(context);
            }
            break;
        case dsl_state_connecting:
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
        case dsl_state_disconnecting:
            // Do nothing.
            break;
    }
}

void dsl_event_line_down_handler(dsl_context_t * const context)
{
    context->is_line_up = false;

    switch (context->current_state)
    {
        case dsl_state_disconnected:
            // Do nothing.
            break;
        case dsl_state_connecting:
			dsl_ppp_stop(context);
            set_state(context, dsl_state_disconnected);
            break;
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
			dsl_ppp_stop(context);
			set_state(context, dsl_state_disconnected);
            break;
        case dsl_state_disconnecting:
			set_state(context, dsl_state_disconnected);
            break;
    }
}

void dsl_event_connecting_timeout_handler(dsl_context_t * const context)
{
    switch (context->current_state)
    {
        case dsl_state_connecting:
			dsl_ppp_stop(context);
			do_connect_action(context);
            break;
        case dsl_state_disconnected:
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
        case dsl_state_disconnecting:
            // Do nothing.
            break;
    }
}

void dsl_event_delay_up_timeout_handler(dsl_context_t * const context)
{
    switch (context->current_state)
    {
        case dsl_state_connected_delay_up:
            set_state(context, dsl_state_connected);
            break;
        case dsl_state_disconnected:
        case dsl_state_connecting:
        case dsl_state_connected:
        case dsl_state_disconnecting:
            // Do nothing.
            break;
    }
}

void dsl_event_disconnecting_timeout_handler(dsl_context_t * const context)
{
    switch (context->current_state)
    {
        case dsl_state_disconnecting:
			set_state(context, dsl_state_disconnected);
            break;
        case dsl_state_disconnected:
        case dsl_state_connecting:
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
            // Do nothing.
            break;
    }
}

void dsl_event_ping_response_timeout_handler(dsl_context_t * const context)
{
    if (context->saved_ping_response_info != NULL)
    {
        send_pong_message(context->saved_ping_response_info->pong_destination, context->saved_ping_response_info->message_id);
        free((void *)context->saved_ping_response_info);
        context->saved_ping_response_info = NULL;
    }
}

void dsl_event_connect_message_handler(dsl_context_t * const context)
{
    context->desired_state = dsl_state_connected;

    switch (context->current_state)
    {
        case dsl_state_disconnected:
			if (context->is_line_up)
			{
				do_connect_action(context);
			}
            break;
        case dsl_state_connecting:
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
        case dsl_state_disconnecting:
            // Do nothing.
            break;
    }
}

void dsl_event_disconnect_message_handler(dsl_context_t * const context)
{
    context->desired_state = dsl_state_disconnected;

    switch (context->current_state)
    {
        case dsl_state_connecting:
    		dsl_ppp_stop(context);
			set_state(context, dsl_state_disconnected);
            break;
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
			do_disconnect_action(context);
            break;
        case dsl_state_disconnected:
        case dsl_state_disconnecting:
            // Do nothing.
            break;
    }
}

void dsl_event_connected_message_handler(dsl_context_t * const context)
{
    switch (context->current_state)
    {
        case dsl_state_disconnected:
			if (context->desired_state == dsl_state_connected)
			{
				set_state(context, dsl_state_connected_delay_up);
			}
			else
			{
				dsl_ppp_stop(context);
			}
            break;
        case dsl_state_connecting:
			set_state(context, dsl_state_connected_delay_up);
            break;
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
            // Do nothing.
            break;
        case dsl_state_disconnecting:
			dsl_ppp_stop(context);
            break;
    }
}

void dsl_event_disconnected_message_handler(dsl_context_t * const context)
{
    switch (context->current_state)
    {
        case dsl_state_disconnected:
            // Do nothing.
            break;
        case dsl_state_connecting:
            // For some reason we get a spurious 'disconnected' message
            // shortly before PPP connects. We have to ignore this (rather
            // than return to dsl_state_disconnected), otherwise the PPP link
            // endless cycles up and down and doesn't stay connected.
            break;
        case dsl_state_connected_delay_up:
        case dsl_state_connected:
        case dsl_state_disconnecting:
			set_state(context, dsl_state_disconnected);
            break;
    }
}

void dsl_event_update_message_handler(dsl_context_t * const context)
{
    dsl_config_update(context);
}

void dsl_event_ping_message_handler(dsl_context_t * const context, dsl_event_t * const event)
{
    ping_info_st const * const info = event->info;

    if (info->please_respond_time == 0)
    {
        send_pong_message(info->pong_destination, info->message_id);
    }
    else if (context->saved_ping_response_info == NULL)
    {
        context->saved_ping_response_info = info;
        event->info = NULL;
        dsl_timer_start(&context->ping_response_timer, info->please_respond_time);
    }
    // else must have previously received a PING request and are still waiting for the timer to elapse
}

