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
#include "messages.h"
#include "message_codes.h"
#include "tlr_print.h"
#include "interfaces.h"
#include "ethregs.h"
#include "parsing.h"
#include "daemonize.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <pthread.h>

int temp_read_config_eth_state(size_t instance);

#define NUM_ETH_MONITOR_CONTEXTS	4 	// Numbered/ referred to as from 1..4.
#define ETHIDX_TO_INSTANCE(index)	((index)+1)
#define INSTANCE_TO_ETHIDX(inst)	((inst>0)?inst-1:0)
#define IS_VALID_ETHINSTANCE(instance)	((instance) > 0 && (instance) <= NUM_ETH_MONITOR_CONTEXTS)
#define ETH_MONITOR_EVENT_QUEUE_SIZE 200
#define FIRST_POLL_TIME		4000
#define POLL_TIME			1000


#define MONITOR_PERIOD_SECONDS	10

/*
 * The events that can affect the ethernet state machine
*/
typedef enum eth_events_t
{
	INITIALISE_EVENT,
	POLL_TMR_EVENT,	/* we need to look at the ETH ports periodically to work out if they are up/down */
    CONNECT_MESSAGE_EVENT = connect_message,
    DISCONNECT_MESSAGE_EVENT = disconnect_message,
    CONNECTED_MESSAGE_EVENT = connected_message,
    DISCONNECTED_MESSAGE_EVENT = disconnected_message,
    UPDATE_MESSAGE_EVENT = update_message,
	PING_MESSAGE_EVENT = ping_message,
    JOINED_BRIDGE_MESSAGE_EVENT = joined_bridge_message,
    LEFT_BRIDGE_MESSAGE_EVENT = left_bridge_message,
    ASSIGNED_TO_WAN_MESSAGE = assigned_to_wan_message,
    REMOVED_FROM_WAN_MESSAGE = removed_from_wan_message,
	PING_RESPONSE_TMR_EVENT
} eth_events_t;


typedef struct eth_monitor_event_t
{
	eth_events_t code;
	size_t 		 instance;
	void const * info;
    size_t info_size;

} eth_monitor_event_t;

static eth_context_t eth_context[NUM_ETH_MONITOR_CONTEXTS];

// 1 global event manager for all contexts..
static	event_manager_st * event_manager;

static int send_msg_to_ifman(eth_context_t * const context, message_codes_t const code)
{
	int ret = EXIT_FAILURE;
	char msg[40];
	char const * const ifname = interface_get_name_from_type(eth_interface);

	if (ifname == NULL)
		goto ERROR;

	snprintf(msg, sizeof msg, "%s %d", ifname, context->instance);
	if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, code, msg, NULL))
	{
		tlr_printf(tlr_response_format_ascii, "eth_monitor: send_message() to interface_manager failed\n");
		tlr_event_log(tlr_log_error, "eth_monitor", "eth_monitor: send_message() to interface_manager failed");
	}
	else
		ret = EXIT_SUCCESS;

ERROR:
	return ret;
}


static eth_monitor_event_t * eth_alloc_handler_event(int code, size_t instance, void * * const info, size_t const info_size)
{
	eth_monitor_event_t * event;

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

static void eth_free_handler_event(eth_monitor_event_t * handler_event)
{
    if (handler_event->info != NULL)
        free((void *)handler_event->info);
	free(handler_event);
}

void set_eth_interface_state(eth_context_t * const context, state_t new_state)
{
    DEBUG("set eth_interface_state(%d):old=%d, new=%d\n", context->instance, context->state, new_state);
    if (context->state != new_state)
    {
        tlr_debug("eth%d state change %d -> %d\n", context->instance, context->state, new_state);
        context->state = new_state;
        if (send_msg_to_ifman(context, (new_state == state_eth_up) ? connected_message : disconnected_message) != EXIT_SUCCESS)
        {
            context->state = state_init;    // Stay in init so we keep trying to inform ifmanager if we cannot send it a message.
            tlr_debug("failed to send msg to ifman!!\n");
        }
    }
}

/*
 * eth_event_handler
 * Called whenever a new event is pulled off the message queue for this context
*/
static void eth_event_handler(void * const user_context, void * const data)
{
    eth_monitor_event_t * const handler_event = (eth_monitor_event_t *)data;
    eth_context_t * const context = &eth_context[INSTANCE_TO_ETHIDX(handler_event->instance)];
    interface_info_st 	* int_info;
    wan_info_st * wan_info;
    const char * eth_state_string[] = {"init", "up", "down"};

    switch (handler_event->code)
    {
        // Private events..
        case INITIALISE_EVENT:
            // set context->desired_eth_state according to config_eth_state (this is until ifmanager is working better)
            context->desired_eth_state = temp_read_config_eth_state(context->instance);
            context->link_state = true;

            start_timer(context->poll_timer_id, FIRST_POLL_TIME);      // We'll be periodically polling for eth status.
            break;
        case POLL_TMR_EVENT:
            // You come in here once per second to poll the eth interface
            if (context->state == state_init)
            {
                DEBUG("inst:%d. state init so checking iface\n", context->instance);
                set_eth_interface_state(context, is_interface_up(context->instance) ? state_eth_up : state_eth_down);
            }
            poll_eth_status(context);
            start_timer(context->poll_timer_id, POLL_TIME);      // So we come back and poll again in a second
            break;
            // System wide events...
        case UPDATE_MESSAGE_EVENT:
            DEBUG("ethmon got config update on instance:%d\n", (int)context->instance);
            tlr_debug("config update on instance:%d\n", (int)context->instance);
            eth_config_update(context);
            break;
        case CONNECT_MESSAGE_EVENT:
            DEBUG("connect_message on instance:%d\n", context->instance);
            tlr_debug("ethmon got connect_message on instance:%d\n", context->instance);
            tlr_debug("ethmon eth%d:  state = %s, link_state = %s\n", context->instance,
                        eth_state_string[context->state], context->link_state ? "true" : "false");
            context->desired_eth_state = true;
            if (context->state == state_eth_up)
            {
                send_msg_to_ifman(context, connected_message);       //Inform ifmanager it's connected
            }
            else if (context->link_state == true)
            {
                if (eth_do_connect_action(context) == 0)
                {
                    set_eth_interface_state(context, state_eth_up);
                }
            }
            break;
        case DISCONNECT_MESSAGE_EVENT:
            DEBUG("ethmon got disconnect_message on instance:%d context->state:%d\n", context->instance, context->state);
            tlr_debug("ethmon got disconnect_message on instance:%d\n", context->instance);
            context->desired_eth_state = false;
            if (context->state == state_eth_down)
            {
                DEBUG("already disconnect so sending disconnected\n");
                send_msg_to_ifman(context, disconnected_message);       //Inform ifmanager it's disconnected
            }
            else
            {
                if (eth_do_disconnect_action(context) == 0)
                {
                    set_eth_interface_state(context, state_eth_down);
                }
            }

            break;
        case CONNECTED_MESSAGE_EVENT:
            // An ETH interface has come up...
            if ((int_info = (interface_info_st *)handler_event->info) != NULL)
            {
                DEBUG("ethmon got connected_message on instance:%d regarding instance %d\n", context->instance, int_info->instance);
                tlr_debug("ethmon got connected_message int_info->instance = %d\n", int_info->instance);
                if (IS_VALID_ETHINSTANCE(int_info->instance))
                {
                    if (!eth_context[INSTANCE_TO_ETHIDX(int_info->instance)].desired_eth_state)
                    {
                        tlr_debug("ethmon received connected message when not trying to connect\n");
                    }
                    else
                    {
                        tlr_debug("ethmon calling set_eth_interface_state:%d\n", eth_context[INSTANCE_TO_ETHIDX(int_info->instance)].instance);
                        set_eth_interface_state(&eth_context[INSTANCE_TO_ETHIDX(int_info->instance)], state_eth_up);
                    }
                }
            }
            break;
        case DISCONNECTED_MESSAGE_EVENT:
            // An ETH interface has gone down.
            if ((int_info = (interface_info_st *)handler_event->info) != NULL)
            {
                DEBUG("ethmon got disconnected_message on instance:%d regarding instance %d\n", context->instance, int_info->instance);
                tlr_debug("ethmon got disconnected_message int_info->instance = %d\n", int_info->instance);
                if (IS_VALID_ETHINSTANCE(int_info->instance))
                {
                    if (eth_context[INSTANCE_TO_ETHIDX(int_info->instance)].desired_eth_state)
                    {
                        tlr_debug("ethmon received disconnected message when not trying to disconnect\n");
                    }
                    else
                    {
                        tlr_debug("ethmon calling set_eth_interface_state:%d\n", eth_context[INSTANCE_TO_ETHIDX(int_info->instance)].instance);
                        set_eth_interface_state(&eth_context[INSTANCE_TO_ETHIDX(int_info->instance)], state_eth_down);
                    }
                }
            }
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
                    start_timer(context->ping_reponse_timer_id, pinfo->please_respond_time);
                }
            }
            break;
        case JOINED_BRIDGE_MESSAGE_EVENT:
            DEBUG("ethmon got joined_bridge_message on instance:%d\n", context->instance);
            tlr_debug("ethmon got joined_bridge_message on instance:%d\n", context->instance);
            if (eth_do_joined_bridge_action(context) == 0)
            {
                send_msg_to_ifman(context, joined_bridge_message);
            }
            break;
        case LEFT_BRIDGE_MESSAGE_EVENT:
            DEBUG("ethmon got left_bridge_message on instance:%d\n", context->instance);
            tlr_debug("ethmon got left_bridge_message on instance:%d\n", context->instance);
            if (eth_do_left_bridge_action(context) == 0)
            {
                send_msg_to_ifman(context, left_bridge_message);
            }
            break;
        case ASSIGNED_TO_WAN_MESSAGE:
            DEBUG("ethmon got assigned_to_wan_message on instance:%d\n", context->instance);
            tlr_debug("ethmon got assigned_to_wan_message on instance:%d\n", context->instance);
            wan_info = (wan_info_st *) handler_event->info;
            if (wan_info != NULL)
            {
                eth_do_assigned_to_wan(context, wan_info);
            }
            break;
        case REMOVED_FROM_WAN_MESSAGE:
            DEBUG("ethmon got removed_from_wan_message on instance:%d\n", context->instance);
            tlr_debug("ethmon got removed_from_wan_message on instance:%d\n", context->instance);
            wan_info = (wan_info_st *) handler_event->info;
            if (wan_info != NULL)
            {
                eth_do_removed_from_wan(context, wan_info);
            }
            break;
        case PING_RESPONSE_TMR_EVENT:
            {
                if (context->p_response_info != NULL)
                {
                    (void)send_pong_message(context->p_response_info->pong_destination, context->p_response_info->message_id);
                    free((void *)context->p_response_info);
                    context->p_response_info = NULL;
                }
            }
            break;

        default:
            break;
    }

    eth_free_handler_event(data);
}



/*
 *  send_eth_monitor_event
 *  messages to context are delivered by calling this function. code is one of the defined values in
 *  demo_events.h. data is event code specific, and may be NULL
 */
static int send_eth_monitor_event(size_t const instance, int event, void * * const info, size_t const info_size)
{
	int result = -1;
	eth_monitor_event_t * handler_event;

	handler_event = eth_alloc_handler_event(event, instance, info, info_size);
	if (handler_event == NULL)
		goto done;
    result = send_event_to_event_manager(event_manager, handler_event);
    if (result != 0)
    {
		eth_free_handler_event(handler_event);
		DEBUG("could not send event to event manager\n");
    }

done:
	return result;
}



/*
 * sendEthMonitorEvent
 * messages to context are delivered by calling this function.
 */
static int sendEthMonitorEvent(size_t const instance, int const event, void * * const info, size_t const info_size)
{
	int result = -1;

    if (IS_VALID_ETHINSTANCE(instance))
		result = send_eth_monitor_event(instance, event, info, info_size);

	return result;
}


static void poll_timeout(void * data)
{
	eth_context_t * const context = data;

	send_eth_monitor_event(context->instance, POLL_TMR_EVENT, NULL, 0);
}

static void ping_reponse_timeout(void * data)
{
	eth_context_t * const context = data;

	send_eth_monitor_event(context->instance, PING_RESPONSE_TMR_EVENT, NULL, 0);
}


int startEthMonitor(timer_context_st * const timer_context)
{
	int result;
	size_t i;

	event_manager = new_event_manager(NULL, ETH_MONITOR_EVENT_QUEUE_SIZE, eth_event_handler);
	for (i = 0; i < NUM_ETH_MONITOR_CONTEXTS; i++)
	{
		eth_context_t * const context = &eth_context[i];

		/* initialise the context for this state machine */
		context->instance = ETHIDX_TO_INSTANCE(i);
        context->timer_context = timer_context; /* all contexts use the same timer context. */
		context->poll_timer_id = create_timer(context->timer_context, poll_timeout, context);
		context->ping_reponse_timer_id = create_timer(context->timer_context, ping_reponse_timeout, context);
		context->wan_instance = ETH_NOT_ASSIGNED_TO_A_WAN;
        if (event_manager == NULL)
        {
            result = -1;
            goto done;
        }

		send_eth_monitor_event(context->instance, INITIALISE_EVENT, NULL, 0);
	}
	result = 0;

done:

	return result;
}

static int parseEthMessage(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size)
{
    int result;

    if (message_code == query_connect)
    {
        result = IS_VALID_ETHINSTANCE(instance) ? 0 : -1;

        if (result == 0)
        {
            eth_context_t * const context = &eth_context[INSTANCE_TO_ETHIDX(instance)];
            bool const is_connected = context->desired_eth_state && context->link_state && context->state == state_eth_up;

            // Query's we deal with straight away..
            dprintf(fd, is_connected ? "connected." : "disconnected.");
        }
    }
    else
    {
        result = sendEthMonitorEvent(instance, message_code, info, info_size);
    }

    return result;
}

int eth_monitor(void)
{

	int result;
	message_thread_st * message_thread_context;
	timer_context_st * timer_context;

	memset(&eth_context[0], 0, sizeof eth_context);

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
	if (startEthMonitor(timer_context) != 0)
	{
		result = EXIT_FAILURE;
		goto done;
	}

	/*
	    start the message handling thread. This must be done after starting the state machines so
	    that events received over the messaging socket aren't delivered into a non-existent event
		queue
	*/
	message_thread_context = open_messaging_context(ethernet_monitor, parseEthMessage);
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
	tlr_debug("eth_monitor exiting due to error");
	return result;
}

