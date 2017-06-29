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

#include "vpn_monitor.h"
#include "debug.h"
#include "vpn_config.h"
#include "count_sas.h"
#include "ipsec.h"
#include "tlr_common.h"
#include "digiutils.h"
#include "daemonize.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <messages.h>
#include <message_codes.h>
#include <timer.h>
#include <event_manager.h>
#include <tlr_print.h>
#include <tlr_common.h>
#include <interfaces.h>
#include <parsing.h>
#include <pthread.h>

// FIXME - should get the number of VPN instances from the config manager code
#define NUM_VPN_MONITOR_CONTEXTS	32 // We can have 32 ipsec tunnels

#define IS_VALID_VPN_INSTANCE(instance)	INSTANCE_IS_VALID((instance), NUM_VPN_MONITOR_CONTEXTS)

#define VPN_MONITOR_EVENT_QUEUE_SIZE 200

#define MAX_CONNECTING_TIME_MILLISECS 30000
#define MAX_DISCONNECTING_TIME_MILLISECS 5000
#define INTERCONNECT_TIME_MILLISECS 3000


/*
 * The events that can affect the state machine
*/
typedef enum vpn_events_t
{
    /* private events*/
    CONNECTING_TIMEOUT_EVENT = first_private_message,
    DISCONNECTING_TIMEOUT_EVENT,
    INTERCONNECT_TIMEOUT_EVENT,
    VPN_PING_RESPONSE_TMR_EVENT,

    /* global events */
    CONNECT_MESSAGE_EVENT = connect_message,
    DISCONNECT_MESSAGE_EVENT = disconnect_message,
    CONNECTED_MESSAGE_EVENT = connected_message,
    DISCONNECTED_MESSAGE_EVENT = disconnected_message,
    UPDATE_MESSAGE_EVENT = update_message,
	PING_MESSAGE_EVENT = ping_message
} vpn_events_t;


typedef struct vpn_monitor_event_t
{
	vpn_events_t code;
	size_t instance;
	void const * info;
    size_t info_size;

} vpn_monitor_event_t;

static int send_vpn_monitor_event(size_t const instance, int const event, void * * const info, size_t const info_size);


static vpn_tunnel_context_st vpn_context[NUM_VPN_MONITOR_CONTEXTS];

// 1 global event manager for all contexts..
static	event_manager_st * event_manager;
/* the name specified in connected/disconnected messages to interface manager */
static char const * vpn_monitor_name;

/* callbacks from the timer thread that occur when the associated timer expires
 * Note that the callbacks are called from within the context of the timer thread .
 * They would typically send an event back to the appropriate instance of the monitor.
 * These callbacks should be written to take as little time as possible to execute.
 */
static void connecting_timeout(void * data)
{
	vpn_tunnel_context_st * const context = data;

	send_vpn_monitor_event(context->instance, CONNECTING_TIMEOUT_EVENT, NULL, 0);
}

static void disconnecting_timeout(void * data)
{
	vpn_tunnel_context_st * const context = data;

	send_vpn_monitor_event(context->instance, DISCONNECTING_TIMEOUT_EVENT, NULL, 0);
}

static void interconnect_timeout(void * data)
{
	vpn_tunnel_context_st * const context = data;

	send_vpn_monitor_event(context->instance, INTERCONNECT_TIMEOUT_EVENT, NULL, 0);
}


static void vpn_ping_reponse_timeout(void * data)
{
    vpn_tunnel_context_st * const context = data;

	send_vpn_monitor_event(context->instance, VPN_PING_RESPONSE_TMR_EVENT, NULL, 0);
}


static void start_connecting_timer(vpn_tunnel_context_st * const context, size_t milliseconds)
{
    start_timer(context->connecting_timer_id, milliseconds);
    context->connecting_timer_running = true;
}

static void stop_connecting_timer(vpn_tunnel_context_st * const context)
{
    if (context->connecting_timer_running)
    {
        stop_timer(context->connecting_timer_id);
        context->connecting_timer_running = false;
    }
}

static void start_interconnect_timer(vpn_tunnel_context_st * const context, size_t milliseconds)
{
    start_timer(context->interconnect_timer_id, milliseconds);
    context->interconnect_timer_running = true;
}

static void start_disconnecting_timer(vpn_tunnel_context_st * const context, size_t milliseconds)
{
    start_timer(context->disconnecting_timer_id, milliseconds);
    context->disconnecting_timer_running = true;
}

static void stop_disconnecting_timer(vpn_tunnel_context_st * const context)
{
    if (context->disconnecting_timer_running)
    {
        stop_timer(context->disconnecting_timer_id);
        context->disconnecting_timer_running = false;
    }
}




static int send_msg_to_ifman(vpn_tunnel_context_st * const context, message_codes_t const code)
{
	int ret;
	char * msg = NULL;
    int temp;

    // XXX libraryize this?

    temp = asprintf(&msg, "%s %d", vpn_monitor_name, context->instance);
    if (temp == -1)
    {
        ret = -1;
        goto done;
    }
	if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, code, msg, NULL))
	{
		tlr_printf(tlr_response_format_ascii, "%s monitor: send_message() to interface_manager failed\n", vpn_monitor_name);
		tlr_event_log(tlr_log_error, "vpn_monitor", "%s monitor: send_message() to interface_manager failed", vpn_monitor_name);
        ret = -1;
        goto done;
	}

    ret = 0;

done:
    free(msg);
	return ret;
}


static vpn_monitor_event_t * alloc_handler_event(int code, size_t instance, void * * const info, size_t const info_size)
{
	vpn_monitor_event_t * event;

	event = calloc(1, sizeof *event);
	if (event == NULL)
		goto done;

	event->code = code;
	event->instance = instance;
    if (info != NULL)
    {
		/* claim the memory passed with the event. It'll get freed when the event is freed */
		event->info = *info;
        *info = NULL;
    }
    event->info_size = info_size;
done:
	return event;
}

static void free_handler_event(vpn_monitor_event_t * handler_event)
{
    if (handler_event->info != NULL)
        free((void *)handler_event->info);
	free(handler_event);
}

/* Call when we detect that a tunnel has gone up/down.
 * Don't call when we're setting the state to connecting/disconnecting though
 */
static void set_vpn_interface_state_up_down(vpn_tunnel_context_st * const context, vpn_monitor_state_t new_state)
{
	DEBUG("instance: %zd: old state:%d, new state:%d\n", context->instance, context->actual_vpn_state, new_state);
	tlr_debug("set vpn %zd interface state:old state: %d, new state: %d\n", context->instance, context->actual_vpn_state, new_state);
    /* only inform interface manager when there has been a change of state */
    if (context->actual_vpn_state != new_state)
	{
		DEBUG("instance: %zd: state changed. sending msg with state:%d and saving newstate\n", context->instance, new_state);
		tlr_debug("state changed\n");
        /* XXX the monitor will send a 'disconnected' message to the interface manager if it changes
         * state from init to down.
         * This occurs at startup time when the vpn monitor receives its first 'update' message
         * because the monitor will perform a disconnect operation if the desired state is down at
         * the time the update message is received.
         * Should we bother to send the 'disconnected' message when changing from 'init' to 'down'?
         * As things stand, the interface manager winds up entering a disconnected message into the
         * logs as a result of this state change.
         */

		if (send_msg_to_ifman(context, (new_state == state_tunnel_up) ? connected_message : disconnected_message) != 0)
		{
			context->actual_vpn_state = state_init;    // Stay in init so we keep trying to inform ifmanager if we cannot send it a message.
			DEBUG("failed to send msg to ifman!\n");
		}
        else
        {
            context->actual_vpn_state = new_state;
        }
    }
}

static void vpn_start_tunnel(vpn_tunnel_context_st * const context)
{
    if (ipsec_start_tunnel(context) == 0)
    {
        context->actual_vpn_state = state_tunnel_connecting;
        start_connecting_timer(context, MAX_CONNECTING_TIME_MILLISECS);
    }
}

static void vpn_stop_tunnel(vpn_tunnel_context_st * const context)
{
    if (ipsec_stop_tunnel(context) == 0)
    {
        context->actual_vpn_state = state_tunnel_disconnecting;
        start_disconnecting_timer(context, MAX_DISCONNECTING_TIME_MILLISECS);
    }
}

static int vpn_handle_update_message(vpn_tunnel_context_st * const context)
{
    int ret = -1;
    bool config_has_changed;

    if (config_update(&context->config, context->instance, &config_has_changed) == 0)
    {
        if (config_has_changed)
        {
            DEBUG("config has changed on instance:%zd\n", context->instance);
            // This script determines changes in configuration, and updates them on the running daemon.
            // Currently established connections are not affected
            // XXX what's the difference between a reload and an update, apart from that an update
            // is specific to a particular tunnel ?
            if (ipsec_reload_configuration() < 0)
            {
                tlr_error("failed to update strongswan configuration");
                goto error;
            }
            if (ipsec_update_tunnel_configuration(context) != 0)
            {
                tlr_debug("error updating configuration for %s\n", context->name);
                DEBUG("error updating configuration for %s\n", context->name);
                goto error;
            }

            if (context->desired_vpn_state)
            {
                DEBUG("instance %zd got update and desired state is up", context->instance);
                switch (context->actual_vpn_state)
                {
                    case state_tunnel_interconnect_delay:
                    case state_tunnel_disconnecting:
                        break;
                    case state_init:
                    case state_tunnel_down:
                        vpn_start_tunnel(context);
                        break;
                    case state_tunnel_connecting:
                    case state_tunnel_up:
                        /*
                         * It looks like the new config isn't used until all old SAs are removed,
                         * so I think we need to do a vpn_stop_tunnel and the next vpn_start_tunnel
                         * will used the new config
                        */
                        vpn_stop_tunnel(context);
                        break;
                }
            }
            else
            {
                /* safest just to issue a down command */
                DEBUG("instance %zd got update and desired state is down", context->instance);
                vpn_stop_tunnel(context);
            }
        }
    }

    ret = 0;
error:
    return ret;
}

static void vpn_handle_connect_request(vpn_tunnel_context_st * const context)
{
    /* called when the desired state is up */
    switch (context->actual_vpn_state)
    {
        case state_init:
        case state_tunnel_down:
            vpn_start_tunnel(context);
            break;
        case state_tunnel_interconnect_delay:  /* ignore connect requests while still in this delay period */
            break;
        case state_tunnel_disconnecting:
            stop_disconnecting_timer(context);
            /* should be able to execute an up command */
            vpn_start_tunnel(context);
            break;
        case state_tunnel_connecting:
            break;
        case state_tunnel_up:
            send_msg_to_ifman(context, connected_message);       // Inform ifmanager it's connected
            break;
    }
}

static void vpn_handle_disconnect_message(vpn_tunnel_context_st * const context)
{
    switch (context->actual_vpn_state)
    {
        case state_init:
        case state_tunnel_down:
            send_msg_to_ifman(context, disconnected_message);       //Inform ifmanager it's disconnected
            break;
        case state_tunnel_disconnecting:    /* already disconnecting */
            break;
        case state_tunnel_interconnect_delay:  /* nothing to do */
            break;
        case state_tunnel_connecting:
            stop_connecting_timer(context);
            /* drop through */
        case state_tunnel_up:
            vpn_stop_tunnel(context);
            break;
    }
}

static void vpn_handle_connected_message(vpn_tunnel_context_st * const context)
{
    stop_connecting_timer(context);
    set_vpn_interface_state_up_down(context, state_tunnel_up);
}

static void vpn_handle_disconnected_message(vpn_tunnel_context_st * const context)
{
    /* It looks like the updown script is called whenever an SA is removed. So if there happen to be
     * multiple SAs installed and one set gets removed, we get a disconnected message even though
     * there is still a tunnel to the remote peer.
     * So before we act upon the disconnected message we count up the number of installed SAs and
     * only set the state to down once they've _all_ been removed.
     */
    int sa_count;

    sa_count = count_installed_sas(context->name);
    if (sa_count == 0)
    {
        if (context->actual_vpn_state != state_tunnel_interconnect_delay)
        {
            stop_connecting_timer(context);
            stop_disconnecting_timer(context);
            set_vpn_interface_state_up_down(context, state_tunnel_down);

            /* now we wait for a while before attempting to reconnect */
            start_interconnect_timer(context, INTERCONNECT_TIME_MILLISECS);
            context->actual_vpn_state = state_tunnel_interconnect_delay;
        }
    }
}

static void handle_connecting_timeout(vpn_tunnel_context_st * const context)
{
    context->connecting_timer_running = false;
    switch (context->actual_vpn_state)
    {
        case state_tunnel_connecting:
            context->actual_vpn_state = state_tunnel_down;
            if (context->desired_vpn_state)
            {
                /* interface manager wants the interface up, so try again */
                vpn_handle_connect_request(context);
            }
            break;
        case state_init:
        case state_tunnel_down:
        case state_tunnel_interconnect_delay:  /* ignore connect requests while still in this delay period */
        case state_tunnel_disconnecting:
        case state_tunnel_up:
            break;
    }
}

static void handle_disconnecting_timeout(vpn_tunnel_context_st * const context)
{
    context->disconnecting_timer_running = false;
    switch (context->actual_vpn_state)
    {
        case state_tunnel_disconnecting:
        {
            int sa_count;

            sa_count = count_installed_sas(context->name);
            if (sa_count > 0)
            {
                /* if still in the disconnecting state, try and get the tunnel down again */
                if (!context->desired_vpn_state)
                {
                    vpn_stop_tunnel(context);
                }
            }
            else
            {
                context->actual_vpn_state = state_tunnel_down;
                if (context->desired_vpn_state)
                {
                    /* interface manager wants the interface up, so try again */
                    vpn_handle_connect_request(context);
                }
            }
            break;
        }
        case state_tunnel_connecting:
        case state_init:
        case state_tunnel_down:
        case state_tunnel_interconnect_delay:  /* ignore connect requests while still in this delay period */
        case state_tunnel_up:
            break;
    }
}

static void handle_interconnect_timeout(vpn_tunnel_context_st * const context)
{
    context->interconnect_timer_running = false;
    switch (context->actual_vpn_state)
    {
        case state_tunnel_interconnect_delay:  /* ignore connect requests while still in this delay period */
            context->actual_vpn_state = state_tunnel_down;
            if (context->desired_vpn_state)
            {
                vpn_handle_connect_request(context);
            }
            break;
        case state_tunnel_disconnecting:
        case state_tunnel_connecting:
        case state_init:
        case state_tunnel_down:
        case state_tunnel_up:
            break;
    }

}

/*
 * vpn_event_handler Called whenever a new event is pulled off the message queue for this context
*/
static void vpn_event_handler(void * const user_context, void * const data)
{
    vpn_monitor_event_t * const handler_event = (vpn_monitor_event_t *)data;


    if (!IS_VALID_VPN_INSTANCE(handler_event->instance))
    {
        DEBUG("got event with invalid instance (%zd)\n", handler_event->instance);
        goto done;
    }
    vpn_tunnel_context_st * const context = &vpn_context[INSTANCE_TO_INDEX(handler_event->instance)];

    switch (handler_event->code)
    {
        // Private events
        case CONNECTING_TIMEOUT_EVENT:
            DEBUG("%s monitor instance %zd timeout waiting for connect. Desired state: %s actual state: %d\n", vpn_monitor_name, handler_event->instance, context->desired_vpn_state ? "up" : "down", context->actual_vpn_state);
            handle_connecting_timeout(context);
            break;

        case DISCONNECTING_TIMEOUT_EVENT:
            DEBUG("%s monitor instance %zd timeout waiting for disconnect. Desired state: %s actual state: %d\n", vpn_monitor_name, handler_event->instance, context->desired_vpn_state ? "up" : "down", context->actual_vpn_state);
            handle_disconnecting_timeout(context);
            break;

        case INTERCONNECT_TIMEOUT_EVENT:
            /* if the desired state is up, try connecting again */
            DEBUG("%s monitor instance %zd interconnect timer elapsed. Desired state: %s actual state: %d\n", vpn_monitor_name, handler_event->instance, context->desired_vpn_state ? "up" : "down", context->actual_vpn_state);
            handle_interconnect_timeout(context);
            break;

        case VPN_PING_RESPONSE_TMR_EVENT:
            if (context->p_response_info != NULL)
            {
                (void)send_pong_message(context->p_response_info->pong_destination, context->p_response_info->message_id);
                free((void *)context->p_response_info);
                context->p_response_info = NULL;
            }
            break;

        // System wide events...
        case UPDATE_MESSAGE_EVENT:
        {
            DEBUG("%s monitor got config update on instance:%zd\n", vpn_monitor_name, context->instance);
            tlr_debug("config update on instance:%zd\n", context->instance);
            vpn_handle_update_message(context);
            break;
        }
        case CONNECT_MESSAGE_EVENT:
        {
            DEBUG("VPN monitor %zd got request to connect tunnel\n", context->instance);
            tlr_debug("VPN monitor %zd got connect_message\n", context->instance);
            context->desired_vpn_state = true;
            vpn_handle_connect_request(context);
            break;
        }
        case DISCONNECT_MESSAGE_EVENT:
            DEBUG("VPN monitor %zd got request to disconnect tunnel\n", context->instance);
            tlr_debug("VPN monitor %zd got disconnect_message\n", context->instance);
            context->desired_vpn_state = false;
            vpn_handle_disconnect_message(context);
            break;
        case CONNECTED_MESSAGE_EVENT:
            DEBUG("VPN monitor %zd got connected message and desired state is '%s'\n", context->instance, context->desired_vpn_state ? "up" : "down");
            vpn_handle_connected_message(context);
            break;
        case DISCONNECTED_MESSAGE_EVENT:
            DEBUG("VPN monitor %zd got disconnected message and desired state is '%s'\n", context->instance, context->desired_vpn_state ? "up" : "down");
            vpn_handle_disconnected_message(context);
            break;
        case PING_MESSAGE_EVENT:
        {
            ping_info_st const * const pinfo = handler_event->info;

            if (pinfo->please_respond_time == 0)
            {
                (void)send_pong_message(pinfo->pong_destination, pinfo->message_id);
            }
            else if (context->p_response_info == NULL)
            {
                context->p_response_info = pinfo;
                handler_event->info = NULL;
                start_timer(context->vpn_pingmsg_reponse_timer_id, pinfo->please_respond_time);
            }
        }
            break;
    }

done:
    free_handler_event(data);

}



/*
 *  send_vpn_monitor_event
 *  messages to the specified vpn monitor instance are delivered by calling this function. 'code'
 *  should match one of the defined values in vpn_events_t. 'info' is event code specific, and may
 *  be NULL. 'info_size' indicates the amount of memory pointed to by 'info'
 */
static int send_vpn_monitor_event(size_t const instance, int const event, void * * const info, size_t const info_size)
{
	int result = -1;
	vpn_monitor_event_t * handler_event;

	handler_event = alloc_handler_event(event, instance, info, info_size);
	if (handler_event == NULL)
		goto done;
    result = send_event_to_event_manager(event_manager, handler_event);
    if (result != 0)
    {
		free_handler_event(handler_event);
		DEBUG("could not send event to event manager\n");
    }

done:
	return result;
}

int start_vpn_monitor(timer_context_st * const timer_context)
{
	int result;
	size_t i;

    if (ipsec_start_strongswan() < 0)
    {
        result = -1;
        goto done;
    }
    event_manager = new_event_manager(NULL, VPN_MONITOR_EVENT_QUEUE_SIZE, vpn_event_handler);
    if (event_manager == NULL)
    {
        result = -1;
        goto done;
    }

    for (i = 0; i < ARRAY_SIZE(vpn_context); i++)
	{
		vpn_tunnel_context_st * const context = &vpn_context[i];

		/* initialise the context for this state machine */
		context->instance = INDEX_TO_INSTANCE(i);
        context->timer_context = timer_context; /* all contexts use the same timer context. */
        context->connecting_timer_id = create_timer(context->timer_context, connecting_timeout, context);
        if (context->connecting_timer_id == NULL)
        {
            DEBUG("failed to create connecting timer\n");
            result = -1;
            goto done;
        }
        context->disconnecting_timer_id = create_timer(context->timer_context, disconnecting_timeout, context);
        if (context->disconnecting_timer_id == NULL)
        {
            DEBUG("failed to create disconnecting timer\n");
            result = -1;
            goto done;
        }
        context->interconnect_timer_id = create_timer(context->timer_context, interconnect_timeout, context);
        if (context->interconnect_timer_id == NULL)
        {
            DEBUG("failed to create interconnect timer\n");
            result = -1;
            goto done;
        }
        context->vpn_pingmsg_reponse_timer_id = create_timer(context->timer_context, vpn_ping_reponse_timeout, context);
        if (context->vpn_pingmsg_reponse_timer_id == NULL)
        {
            DEBUG("failed to create pingmsg response timer\n");
            result = -1;
            goto done;
        }

        snprintf(context->name, sizeof context->name, "ipsec%d", context->instance);
        if (config_init(&context->config, context->instance, config_ipsec) < 0)
        {
            DEBUG("could not initialize configuration\n");
            result = -1;
            goto done;
        }
    }

    result = 0;

done:

	return result;
}

static int handle_message_to_vpn_monitor(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size)
{
    int result;

    result = send_vpn_monitor_event(instance, message_code, info, info_size);

    return result;
}

int vpn_monitor_thread(void)
{

	int result;
	message_thread_st * message_thread_context;
	timer_context_st * timer_context;

    vpn_monitor_name = interface_get_name_from_type(vpn_interface);
    if (vpn_monitor_name == NULL)
    {
        result = EXIT_FAILURE;
        goto done;
    }

    /* timer thread must be started before the state machine */
	timer_context = open_timer_context();
	if (timer_context == NULL)
	{
		result = EXIT_FAILURE;
		goto done;
	}

	/*
		now start the state machine, which should run in a thread, allowing multiple state
		machines to run
	*/
	if (start_vpn_monitor(timer_context) != 0)
	{
		result = EXIT_FAILURE;
		goto done;
	}

	/*
	    start the message handling thread. This must be done after starting the state machines so
	    that events received over the messaging socket aren't delivered into a non-existent event
		queue
	*/
	message_thread_context = open_messaging_context(vpn_monitor, handle_message_to_vpn_monitor);
	if (message_thread_context == NULL)
	{
		result = EXIT_FAILURE;
		goto done;
	}

    daemon_confirm_is_running();

	result = EXIT_SUCCESS;

	/* this will block until all child threads end */
	pthread_exit(&result);


	/* shouldn't get here */

done:
	/* should only get here on error */
	tlr_debug("vpn monitor exiting due to error");
	return result;
}
