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
****************************************************************************/

#include "cellular_config.h"
#include "cellular_state.h"
#include "cellular_modem.h"
#include "unittest_support.h"
#include "digiutils.h"

#include "monitor_debug.h"

//#if (MONITOR_DEBUG != 0) || (MONITOR_PRINTF != 0)
#if 1
char const * cellular_state_strings[] =
{
    CELLULAR_FOREACH_STATE(GENERATE_STRING)
};

char const * cellular_message_strings[] =
{
    CELLULAR_FOREACH_MESSAGE(GENERATE_STRING)
};

size_t const cellular_state_strings_count = ARRAY_SIZE(cellular_state_strings);
size_t const cellular_message_strings_count = ARRAY_SIZE(cellular_message_strings);

#else
char const * cellular_state_strings[0];
char const * cellular_message_strings[0];

size_t const cellular_state_strings_count = 0;
size_t const cellular_message_strings_count = 0;

#endif

STATIC_FUNCTION void do_wait_modem_action(cellular_context_t * const context);

void restart_usb_control(void)
{
    system_command("/usr/etc/init.d/Cellular/usb-control.sh off");
    system_command("/usr/etc/init.d/Cellular/usb-control.sh on&");
}

STATIC_FUNCTION int update_cellular_parameter_csid(void)
{
    return system_command("/usr/etc/init.d/Cellular/getParam.sh");
}

STATIC_FUNCTION int start_dualsim_switch(size_t const index_3g, size_t sim)
{
    return system_command("/usr/sbin/dualsim_switch %d -s %d &", index_3g, sim);
}

STATIC_FUNCTION int switch_sim(int const fd, size_t const index_3g, size_t const sim)
{
    int ret;

    /* if something is wrong with 3g code, then just let connecting timer times out */
    if ((cellular_set_active_sim(fd, index_3g, sim) < 0)
        || (update_cellular_parameter_csid() < 0)
        || (start_dualsim_switch(index_3g, sim) < 0))
    {
        monitor_error("ERROR on Switching SIM\n");
        ret = -1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

STATIC_FUNCTION void do_connect_action(cellular_context_t * const context)
{

    if (!is_current_context_active_sim(context, context->modem_status->active_sim))
    {
        monitor_debug("--- TRY TO DO CONNECT BUT INCORRECT SIM%zu to SIM%zu in state %d\n",
                        context->modem_status->active_sim, context->instance, context->current_state);
        do_wait_modem_action(context);
        goto done;
    }

    monitor_debug("--- DO CONNECT ACTION SIM%zu\n", context->instance);

    if ((cellular_get_usb_index(context->csman_fd, context->index_3g, &context->index_usb) < 0)
       || (system_command("3g-action stop %zu %zu", context->index_usb, context->index_3g) < 0)
        || (update_cellular_parameter_csid() < 0)
        || (system_command("3g-action start %zu %zu&", context->index_usb, context->index_3g) < 0))
    {
        monitor_error("Error unable to start 3G\n");
//        restart_usb_control();
//        set_state(context, state_idle);
        start_timer(context->connecting_timer_id, WAIT_MODEM_TIME_IN_MILLISECS);
        context->connecting_timer_running = true;
    }
    else
    {
        context->restart_count++;
        set_state(context, state_connecting);
        start_timer(context->connecting_timer_id, MAX_CONNECTING_TIME_MILLISECS);
        context->connecting_timer_running = true;
    }

done:
    return;
}

static void do_disconnect_action(cellular_context_t * const context)
{
    size_t const active_sim = context->modem_status->active_sim;

    if (is_current_context_active_sim(context, active_sim))
    {
        cellular_connect_status_t const connect_status = context->modem_status->connect_status;
        switch (connect_status)
        {
            case cellular_disconnected:
                monitor_debug("--- DO DISCONNECT ACTION SIM%zu but it's already in %d\n",
                              context->instance, connect_status);
                /* no break */
            case cellular_disconnecting:
            case cellular_connecting:
            case cellular_connected:
                monitor_debug("--- DO DISCONNECT ACTION SIM%zu\n", context->instance);
                if ((cellular_get_usb_index(context->csman_fd, context->index_3g, &context->index_usb) < 0)
                    || (system_command("3g-action stop %zu %zu&", context->index_usb, context->index_3g) < 0))
                {
                    monitor_info("Error unable to stop 3G\n");
                    //restart_usb_control();
                    set_state(context, state_idle);
                }
                else
                {
                    start_timer(context->disconnecting_timer_id, MAX_DISCONNECTING_TIME_MILLISECS);
                    context->disconnecting_timer_running = true;
                    set_state(context, (connect_status == cellular_disconnecting) ? state_disconnected : state_disconnecting);
                }
                break;
        }
    }
    else
    {
        monitor_debug_printf("active sim%d. Bring down sim%zu\n", active_sim, context->instance);
        set_state(context, state_idle);
    }
}

STATIC_FUNCTION void do_wait_modem_action(cellular_context_t * const context)
{
    get_cellular_data(context->instance, &context->config_data);

    set_state(context, state_wait_modem);
    start_timer(context->connecting_timer_id, WAIT_MODEM_TIME_IN_MILLISECS);
    context->connecting_timer_running = true;

    return;
}

static void do_update_config_action(cellular_context_t * const context)
{
    cellular_config_status_t const status = get_cellular_data(context->instance, &context->config_data);
    switch (status)
    {
        case cellular_config_no_changed:
            break;
        case cellular_config_changed:
            do_disconnect_action(context);
            break;
        case cellular_config_error:
            break;
    }
}


/*
 * init_monitor_context
*  Called when the state machine switches to the idle state. We might read the configuration here
*  and perhaps connect an interface if we have enough information and the interface manager desires
*  the interface to be up.
 */
STATIC_FUNCTION void init_monitor_context(cellular_context_t * const context)
{
    context->had_sent_act_in = false;
}

void set_state(cellular_context_t * const context, cellular_state_t const new_state)
{

    context->current_state = new_state;

    switch (new_state)
    {
        case state_idle:

            if ((context->had_sent_act_in) && (!context->desired_state))
                send_disconnected_event_to_interface_manager(context->instance);

            init_monitor_context(context);
            break;
        case state_connected:
            if (!context->had_sent_act_in)
            {
                context->had_sent_act_in = true;
                send_connected_event_to_interface_manager(context->instance);
            }
            break;
        case state_disconnect:
            context->desired_state = false;
            context->had_sent_act_in = true;
            break;
        case state_connect:
            context->desired_state = true;
            break;
        case state_disconnected:
        case state_connecting:
        case state_disconnecting:
        case state_wait_modem:
        case state_initialize:
            break;
    }

}

/*
   this handler is called whenever an event is received and we're in the INITIALISE state
   It responds to the events as appropriate for this state.
 */
void state_initialize_handler(cellular_context_t * const context, void * data)
{
    cellular_event_t * const handler_event = (cellular_event_t *)data;

    switch (handler_event->code)
    {
        case cellular_message_initialize:
            get_cellular_data(context->instance, &context->config_data);

#ifdef CELLULAR_MODEM_STATUS_TIMER
            start_timer(context->modem_status_timer_id, MODEM_STATUS_TIME_IN_MILLISECS);
            context->modem_status_timer_running = true;
#endif
            set_state(context, state_idle);
            break;
        default:
            break;
    }
}

/*
   this handler is called whenever an event is received and we're in the IDLE state
   It responds to the events as appropriate for this state.
 */
void state_idle_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = (cellular_event_t *)data;

    switch (handler_event->code)
    {
        case connect_message:
            set_state(context, state_connect);
            break;

        case cellular_message_connect:
            do_wait_modem_action(context);
            break;

        case disconnect_message:
            /* got a disconnect request when not connected. Just send a disconnected event to interface
             * manager to let it know we're down
             */
            send_disconnected_event_to_interface_manager(context->instance);
            break;

        default:
            monitor_debug_printf("state_idle_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}


void state_connect_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = (cellular_event_t *)data;

    switch (handler_event->code)
    {
        case cellular_message_connect:
            do_wait_modem_action(context);
            break;

        case disconnect_message:
            set_state(context, state_disconnect);

            /* no break; */
        case cellular_message_disconnect:
            /* we have not start 3g-action yet so just
             * set back to idle state.
             */
            if (context->connecting_timer_running == true)
            {
                stop_timer(context->connecting_timer_id);
                context->connecting_timer_running = false;
            }
            set_state(context, state_idle);
            break;

        default:
            monitor_debug_printf("state_connect_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}

void state_wait_modem_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = data;

    switch (handler_event->code)
    {
        case cellular_message_connect_timeout:
            {
                size_t const active_sim = context->modem_status->active_sim;
                bool usb_modem_ready = false;

                context->connecting_timer_count++;
                if (context->connecting_timer_count >= 90)
                {
                    /* restart usb and connecting timer */
                    monitor_error("Error unable to switch (usb ready %d sim %zu)\n",
                                    cellular_get_usb_ready(context->csman_fd, context->index_3g),
                                    active_sim);

                    //restart_usb_control();
                    context->connecting_timer_count = 0;
                }
                else if (!is_current_context_active_sim(context, context->modem_status->active_sim))
                {
                    monitor_debug("--- switching SIM%zu (current SIM%zu) usb ready %d\n", context->instance, active_sim, cellular_get_usb_ready(context->csman_fd, context->index_3g));

                    /* if something is wrong with 3g code, then just let connecting-timer times out */
                    if (switch_sim(context->csman_fd, context->index_3g, context->instance) == 0)
                    {
                        cellular_get_active_sim(context->csman_fd, context->index_3g, &context->modem_status->active_sim);
                    }

                }
                else
                {

                    context->connecting_timer_running = false;

                    /* Timeout waiting for modem */
                    if (init_3g_initialized)
                    {
                        usb_modem_ready = cellular_get_usb_ready(context->csman_fd, context->index_3g);

                        if ((!usb_modem_ready) ||
                            (context->modem_status->active_sim != cellular_get_switch_sim_card(context->csman_fd, context->index_3g)))
                        {
                            /* make sure modem is ready and dualsim_switch is done */
                            usb_modem_ready = false;
                        }
                        else
                        {
                            do_connect_action(context);
                            context->connecting_timer_count = 0;
                        }
                    }
                }
                if (!usb_modem_ready)
                {
                    start_timer(context->connecting_timer_id, WAIT_MODEM_TIME_IN_MILLISECS);
                    context->connecting_timer_running = true;
                }
                break;
            }
        case disconnect_message:
            set_state(context, state_disconnect);

            /* no break; */
        case cellular_message_disconnect:
            /* we have not start 3g-action yet so just
             * set back to idle state.
             */
            if (context->connecting_timer_running == true)
            {
                stop_timer(context->connecting_timer_id);
                context->connecting_timer_running = false;
            }
            set_state(context, state_idle);
            break;

        default:
            monitor_debug_printf("state_wait_modem_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}

void state_connecting_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = data;

    switch (handler_event->code)
    {
        case disconnect_message:
            if (context->connecting_timer_running == true)
            {
                stop_timer(context->connecting_timer_id);
                context->connecting_timer_running = false;
            }
            set_state(context, state_disconnect);
            send_monitor_event(context, cellular_message_disconnect, NULL, 0);
            break;

        case cellular_message_connect_timeout:
            /* Timeout waiting for interface to connect */
            switch (context->modem_status->connect_status)
            {
                case cellular_disconnected:
                    do_connect_action(context);
                    break;
                default:
                    break;
            }
            start_timer(context->connecting_timer_id, MAX_CONNECTING_TIME_MILLISECS);
            break;

        case cellular_message_disconnect:
            if (context->connecting_timer_running == true)
            {
                stop_timer(context->connecting_timer_id);
                context->connecting_timer_running = false;
            }
            do_disconnect_action(context);
            break;

        case connected_message:
            /* interface now connected. stop the connecting timer. */
            if (context->connecting_timer_running == true)
            {
                stop_timer(context->connecting_timer_id);
                context->connecting_timer_running = false;
                set_state(context, state_connected);
            }
            break;
        case disconnected_message:
            /* keep trying to connect.
             */
            monitor_debug_printf("state_connecting_handler: retry connecting %d <= %d\n", context->restart_count, context->config_data.connection_attempts);
            do_connect_action(context);
            break;

        case update_message:
            do_update_config_action(context);
            break;

        default:
            monitor_debug_printf("state_connecting_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}

void state_connected_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = data;

    switch (handler_event->code)
    {
        case disconnect_message:
            set_state(context, state_disconnect);
            break;

        case cellular_message_disconnect:
            do_disconnect_action(context);
            break;

        case connect_message:
            /* a bit unexpected, but just send back a connected message */
            //context->desired_state = true;
            send_connected_event_to_interface_manager(context->instance);
            context->had_sent_act_in = true;
            break;

        case disconnected_message:
            /* interface has disconnected.
             */
            /* keep trying to connect.
             */
            do_connect_action(context);
            break;

        case update_message:
            do_update_config_action(context);
            break;

        default:
            monitor_debug_printf("state_connecting_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}

void state_disconnect_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = data;

    switch (handler_event->code)
    {
        case cellular_message_disconnect:
            do_disconnect_action(context);
            break;

        case connect_message:
            set_state(context, state_connect);
            break;

        case disconnect_message:
        case cellular_message_disconnect_timeout:
        case disconnected_message:
            state_disconnected_handler(context, data);
            break;

        default:
            monitor_debug_printf("state_disconnect_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}

void state_disconnecting_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = data;

    switch (handler_event->code)
    {
        case cellular_message_disconnect:
            do_disconnect_action(context);
            break;

        case disconnect_message:
        case cellular_message_disconnect_timeout:
        case disconnected_message:
            state_disconnected_handler(context, data);
            break;

        case connect_message:
            set_state(context, state_connect);
            break;

        default:
            monitor_debug_printf("state_disconnecting_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}

void state_disconnected_handler(cellular_context_t * const context, void * const data)
{
    cellular_event_t * const handler_event = data;

    switch (handler_event->code)
    {
        case cellular_message_disconnect_timeout:
            monitor_debug("state_disconnected_handle: disconnect timer time out");
            context->disconnecting_timer_running = false;
            do_disconnect_action(context);
            break;
        case disconnected_message:
        case disconnect_message:
            if (context->disconnecting_timer_running)
            {
                context->disconnecting_timer_running = false;
                stop_timer(context->disconnecting_timer_id);
            }
            if ((context->desired_state) && (context->had_sent_act_in))
                send_disconnected_event_to_interface_manager(context->instance);

            set_state(context, state_idle);
            break;

        case connect_message:
            set_state(context, state_connect);
            break;

        default:
            monitor_debug_printf("state_disconnected_handler %zu: event %d not handled\n", context->instance, handler_event->code);
            break;
    }
}
