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

#include "cellular_config.h"
#include "cellular_monitor.h"
#include "cellular_state.h"

#include "cellular_modem.h"
#include "unittest_support.h"

#include <stdlib.h>
#include <string.h>
#include "monitor_debug.h"
#include "cellular_led.h"

extern unsigned int cellular_alloc_event_count;

//extern size_t cellular_led_heartbeat;
//tlr_log_t cellular_debug_priority = tlr_log_nolog;
tlr_log_t cellular_debug_priority = tlr_log_debug;

char const * interface_name;    // the name of the interface we're monitoring
size_t interface_instance_max;

static cellular_modem_status_t * cellular_modem_status = NULL;

#define IS_VALID_CELLULAR_INSTANCE(instance) INSTANCE_IS_VALID((instance), (interface_instance_max))

/* An array of the various state handlers. There must be one for each state */
static const state_handler_fn cellular_state_handlers[] =
{
    CELLULAR_FOREACH_STATE(CELLULAR_GENERATE_FUNC)
};

static event_manager_st * cellular_monitor_event_manager = NULL;
cellular_context_t * cellular_contexts = NULL;

static void cell_ping_response_tmr_event_handler(cellular_context_t * const context);

static char const * cellular_state_string_get(cellular_state_t const state)
{
    char const * state_string;

    if (state < cellular_state_strings_count)
    {
        state_string = cellular_state_strings[state];
    }
    else
    {
        state_string = "";
    }
    return state_string;
}

void show_cellular_debug(size_t const instance)
{
    size_t i = (instance <= interface_instance_max) ? INSTANCE_TO_INDEX(instance) : 0;

    cellular_context_t const * const context = &cellular_contexts[i];
    cellular_connect_status_t status = cellular_get_connection_status(context->csman_fd, context->index_3g);
    size_t index_usb;

    cellular_get_usb_index(context->csman_fd, context->index_3g, &index_usb);

    monitor_debug("USB index                = %zu\n", index_usb);
    monitor_debug("Modem CSID Status        = %d\n", status);
    //monitor_debug("LED Heartbeat            = %zu\n", cellular_led_heartbeat);
    monitor_debug("Heartbeat                = %zu\n", context->modem_status->heartbeat);
    monitor_debug("Modem connect status     = %d\n", context->modem_status->connect_status);
    monitor_debug("Modem desired status     = %d\n", context->modem_status->desired_status);
    monitor_debug("Active SIM               = %zu\n", context->modem_status->active_sim);
    monitor_debug("status_timer_count       = %zu\n", context->status_timer_count);
    monitor_debug("Instance                 = %zu\n", context->instance);
    monitor_debug("State                    = \"%s\" (%d)\n", cellular_state_string_get(context->current_state), context->current_state);
    monitor_debug("Restart_count            = %zu\n", context->restart_count);
    monitor_debug("sent_act_in              = %d\n", context->had_sent_act_in);
    monitor_debug("desired_state            = %d\n", context->desired_state);
    monitor_debug("connecting_timer_running = %d\n", context->connecting_timer_running);
    monitor_debug("disconnecting_timer_running = %d\n", context->disconnecting_timer_running);
    monitor_debug("csman_fd                 = %d\n", context->csman_fd);
    monitor_debug("cellular_alloc_event_count  = %d\n", cellular_alloc_event_count);

}

/* callbacks from the timer thread that occur when the associated timer expires
 * Note that the callbacks are called from within the context of the timer thread .
 * They would typically send an event back to the appropriate instance of the monitor.
 * These callbacks should be written to take as little time as possible to execute.
 */
static void connecting_timeout(void * data)
{
    cellular_context_t * const context = data;

//    monitor_debug("connecting_timeout: %p (%d)\n", context, context->instance);
    send_monitor_event(context, cellular_message_connect_timeout, NULL, 0);
}

static void disconnecting_timeout(void * data)
{
    cellular_context_t * const context = data;

    send_monitor_event(context, cellular_message_disconnect_timeout, NULL, 0);
}

#ifdef CELLULAR_MODEM_STATUS_TIMER
static void modem_status_timeout(void * data)
{
    cellular_context_t * const context = data;
    context->status_timer_count++;
    send_monitor_event(context, cellular_message_modem_status_timeout, NULL, 0);
}
#endif

static void cell_ping_reponse_timeout(void * data)
{
    cellular_context_t * const context = data;

	send_monitor_event(context, CELL_PING_RESPONSE_TMR_EVENT, NULL, 0);
}

/*
 * monitor_event_handler
 * Called whenever a new event is pulled off the message queue for this context
*/
STATIC_FUNCTION void monitor_event_handler(void * const user_context, void * const data)
{
    cellular_event_t * const event = data;
    cellular_context_t * const context = &cellular_contexts[IS_VALID_CELLULAR_INSTANCE(event->instance) ? INSTANCE_TO_INDEX(event->instance) : 0];

    switch (event->code)
    {
        case ping_message:
        {
            ping_info_st const * const pinfo = event->info;

            if (pinfo->please_respond_time == 0)
                (void)send_pong_message(pinfo->pong_destination, pinfo->message_id);
            else if (context->p_response_info == NULL)
            {
                context->p_response_info = pinfo;
                event->info = NULL;
                start_timer(context->cell_pingmsg_reponse_timer_id, pinfo->please_respond_time);
            }
            break;
        }
        case CELL_PING_RESPONSE_TMR_EVENT:
            cell_ping_response_tmr_event_handler(context);
            break;
#ifdef CELLULAR_MODEM_STATUS_TIMER
        case cellular_message_modem_status_timeout:
            monitor_cellular_status(context);
            break;
#endif
        case connect_message:
        case disconnect_message:
            monitor_debug_printf("Cellular%zu received \"%s\" message.\n", context->instance, (event->code == connect_message) ? "connect" : "disconnect");
            /* no break; */
        default:
            if (context->current_state < ARRAY_SIZE(cellular_state_handlers))
            {
                cellular_state_handlers[context->current_state](context, event);
            }
            else
            {
                monitor_debug_printf("Invalid current state %d. Event (%d) not handled\n", context->current_state, event->code);
            }
            break;
    }

    free_event(event);
}


static void cell_ping_response_tmr_event_handler(cellular_context_t * const context)
{
    if (context->p_response_info != NULL)
    {
        (void)send_pong_message(context->p_response_info->pong_destination, context->p_response_info->message_id);
        free((void *)context->p_response_info);
        context->p_response_info = NULL;
    }
}

static void close_cellular_context(cellular_context_t * const context)
{
    if (context->connecting_timer_id != NULL)
    {
        destroy_timer(context->connecting_timer_id);
    }

    if (context->disconnecting_timer_id != NULL)
    {
        destroy_timer(context->disconnecting_timer_id);
    }

    cellular_close_status(context->csman_fd);
}

void close_cellular_monitor(void)
{
    if (cellular_contexts != NULL)
    {
        size_t i;

        for (i=0; i < interface_instance_max; i++)
        {
            close_cellular_context(&cellular_contexts[i]);
        }
        free(cellular_contexts);
    }
    cellular_contexts = NULL;

    if (cellular_modem_status != NULL)
    {
        free(cellular_modem_status);
        cellular_modem_status = NULL;
    }
}

int init_cellular_monitor(timer_context_st * const timer_context)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    size_t i;
    int result;

    cellular_all_leds_off();
    if (table->groups[config_cellular].instances == 0)
    {
        monitor_info("Not supported interface type (%d)\n", cellular_interface);
        result = -1;
        goto done;
    }

    interface_name = interface_get_name_from_type(cellular_interface);
    if (interface_name == NULL)
    {
        monitor_error("monitoring unknown interface type (%d)\n",cellular_interface);
        result = -1;
        goto done;
    }

    monitor_debug("Interface name = %s\n", interface_name);


    interface_instance_max = table->groups[config_cellular].instances;

    cellular_contexts = calloc(table->groups[config_cellular].instances, sizeof *cellular_contexts);
    if (cellular_contexts == NULL)
    {
        monitor_error("calloc failed for cellular interface (%d id)\n", cellular_interface);
        result = -1;
        goto done;
    }

    cellular_modem_status = calloc(interface_instance_max, sizeof *cellular_modem_status);
    if (cellular_modem_status == NULL)
    {
        monitor_error("calloc failed cellular modem status\n");
        result = -1;
        goto done;
    }

    for (i = 0; i < interface_instance_max; i++)
    {
        cellular_modem_status_t * const modem_status = &cellular_modem_status[i];

        modem_status->active_sim = 0;
        modem_status->connect_status = cellular_disconnected;
        modem_status->desired_status = cellular_disconnected;
        modem_status->heartbeat = 0;
    }

    memset(cellular_contexts, 0, (interface_instance_max * sizeof *cellular_contexts));

    for (i = 0; i < interface_instance_max; i++)
    {
        cellular_context_t * const context = &cellular_contexts[i];

        /* initialise the context for this state machine */
        context->instance = INDEX_TO_INSTANCE(i);
        context->index_3g = CELLULAR_INDEX_3G; /* fix 3G index from AMIT */
        context->timer_context = NULL;
        context->current_state = state_initialize;
        context->restart_count = 0;
        context->status_timer_count = 0;
        context->connection_established = false;
        context->p_response_info = NULL;
        context->csman_fd = -1;

        context->config_data.preferred_mode = config_cellular_preferred_mode_auto;

        context->modem_status = &cellular_modem_status[i];

        context->timer_context = timer_context; /* all contexts use the same timer context. */

        context->connecting_timer_id = create_timer(context->timer_context, connecting_timeout, context);
        if (context->connecting_timer_id == NULL)
        {
            monitor_error("create connecting-timer failed\n");
            result = -1;
            goto done;
        }

        context->disconnecting_timer_id = create_timer(context->timer_context, disconnecting_timeout, context);
        if (context->disconnecting_timer_id == NULL)
        {
            monitor_error("create disconnecting-timer failed\n");
            result = -1;
            goto done;
        }

#ifdef CELLULAR_MODEM_STATUS_TIMER
        context->modem_status_timer_id = create_timer(context->timer_context, modem_status_timeout, context);
        if (context->modem_status_timer_id == NULL)
        {
            monitor_error("create modem_status-timer failed\n");
            result = -1;
            goto done;
        }
#endif
        context->cell_pingmsg_reponse_timer_id = create_timer(context->timer_context, cell_ping_reponse_timeout, context);
        if (context->cell_pingmsg_reponse_timer_id == NULL)
        {
            monitor_error("create ping response-timer failed\n");
            result = -1;
            goto done;
        }



    }

    monitor_debug_printf("interface_instance_max = %zu (%zu)\n", interface_instance_max, cellular_modem_status[0].active_sim);
    result = 0;

done:
    if (result < 0)
    {
        close_cellular_monitor();
    }

    return result;
}

int start_cellular_monitor(void)
{
    size_t i;
    int result;

    monitor_debug("start_cellular_monitor\n");

    cellular_monitor_event_manager = new_event_manager(NULL, MONITOR_EVENT_QUEUE_SIZE, monitor_event_handler);
    if (cellular_monitor_event_manager == NULL)
    {
        monitor_error("create event manager failed\n");
        result = -1;
        goto done;
    }

    if (cellular_contexts == NULL)
    {
        monitor_error("cellular contexts not initialized\n");
        result = -1;
        goto done;
    }

    for (i = 0; i < interface_instance_max; i++)
    {
        cellular_context_t * const context = &cellular_contexts[i];
        context->event_manager = cellular_monitor_event_manager;

        /*
           send an INITIALISE event to get the monitor running.
           Be sure to do this after creating the new event manager
         */
        send_monitor_event(context, cellular_message_initialize, NULL, 0);
    }

    result = 0;

done:
    if (result < 0)
    {
        close_cellular_monitor();
    }

    return result;
}

/*
 * send_message_to_cellular_monitor
 * used by other threads (e.g. messaging thread) to deliver events to an instance of this monitor
 * Note that unless we return -1, info will need to be freed by the consumer (if not NULL) after
 * handling the event.
 */
int send_message_to_cellular_monitor(size_t const instance, int const event, void * * const info, size_t const info_size)
{
    int result = -1;

    if ((cellular_contexts != NULL) && IS_VALID_CELLULAR_INSTANCE(instance))
    {
        result = send_monitor_event(&cellular_contexts[INSTANCE_TO_INDEX(instance)], event, info, info_size);
        if (result < 0)
        {
            monitor_debug_printf("send_message_to_cellular_monitor: send message to monitor failed %d\n", event);
        }

    }

    return result;
}

#ifdef CELLULAR_MODEM_STATUS_TIMER
void monitor_cellular_status(cellular_context_t * context)
{

    static bool context_initialized = false;

    if (!context_initialized || !init_3g_initialized)
    {
        if ((interface_instance_max > 0) && (cellular_contexts != NULL))
        {
            context_initialized = true;
        }
        goto ret;
    }

    if (!CELLULAR_VALID_CSMAN_FD(context->csman_fd))
    {
        context->csman_fd = cellular_open_status();

        if (!CELLULAR_VALID_CSMAN_FD(context->csman_fd))
        {
            goto ret;
        }
        monitor_debug("cellular status: cellular%zu csman_fd = %d\n", context->instance, context->csman_fd);
    }

    {
        size_t active_sim;

        if (cellular_get_active_sim(context->csman_fd, context->index_3g, &active_sim) == 0)
        {
            modem_status_handler(context, active_sim);
        }
        else
        {
            monitor_debug_printf("cellular status: active sim = %d (not ready)\n", active_sim);
        }

    }


ret:

    start_timer(context->modem_status_timer_id, MODEM_STATUS_TIME_IN_MILLISECS);
    context->modem_status_timer_running = true;

    return;

}
#else
void monitor_cellular_status(cellular_context_t * const context)
{

    size_t active_sim;

    if (!CELLULAR_VALID_CSMAN_FD(context->csman_fd))
    {
        context->csman_fd = cellular_open_status();

        if (!CELLULAR_VALID_CSMAN_FD(context->csman_fd))
        {
            goto ret;
        }
    }

    if (cellular_get_active_sim(context->csman_fd, context->index_3g, &active_sim) == 0)
    {
        modem_status_handler(context, active_sim);
    }
    else
    {
        monitor_debug_printf("cellular status: active sim = %d (not ready)\n", active_sim);
    }


    //cellular_update_leds(active_sim,  context);

done:
    return;

}
#endif
