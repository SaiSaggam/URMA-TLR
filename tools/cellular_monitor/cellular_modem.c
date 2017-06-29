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
#include "cellular_modem.h"
#include "cellular_state.h"
#include <stdlib.h>

#include "monitor_debug.h"

extern bool init_3g_initialized;
unsigned int cellular_alloc_event_count= 0;

static int send_message_to_interface_manager(message_codes_t message_code, char const * const additional_info)
{
    int result;

    // currently always delivered to interface manager instance 1
    result = send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, message_code, additional_info, NULL);
    monitor_debug_printf("Send interface manager message %d %s (%d)\n", message_code, (additional_info != NULL) ? additional_info : "", result);

    return result;
}

static int send_event_with_interface_info_to_interface_manager(message_codes_t message_code, size_t interface_instance)
{
    char * additional_info = NULL;
    int temp;
    int result = -1;

    /* both of the messages sent to interface manager (connected/disconnected) need additional
     * info sent (i.e. interface name and instance)
     */
    temp = asprintf(&additional_info, "%s %zd", interface_name, interface_instance);

    if (temp == -1 || additional_info == NULL)
        goto done;
    result = send_message_to_interface_manager(message_code, additional_info);
    // TODO: error handling

done:
    free(additional_info);
    return result;
}

static int notify_cellular_to_connect(cellular_context_t * const context)
{
    int ret = -1;
    monitor_debug_printf(
            "notify_cellular_to_connect: Send CONNECT cellular %zu/%zu connect_message\n",
            context->instance, context->modem_status->active_sim);
    if (send_monitor_event(context, cellular_message_connect, NULL, 0) == 0)
    {
        context->modem_status->desired_status = cellular_connecting;
        ret = 0;
    }

    return ret;
}

static int notify_cellular_to_disconnect(cellular_context_t * const context)
{
    int ret = -1;
    monitor_debug_printf("notify_cellular_to_disconnect: Send DISCONNECT SIM%zu/%zu\n",
                         context->instance, context->modem_status->active_sim);

    if (send_monitor_event(context, cellular_message_disconnect, NULL, 0) == 0)
    {
        context->modem_status->desired_status = cellular_disconnecting;
        ret = 0;
    }

    return ret;
}

static void handle_modem_disconnected_status(cellular_context_t * const context, size_t const active_sim)
{

    /* See if this context wants to connect
     */
    if (context->desired_state)
    {
        if (context->instance != CELLULAR_1_INSTANCE)
        {
            /* if cellular 1 also wants to connect,
             * cellular 1 takes precedent
             */
            cellular_context_t * const cellular1 = &cellular_contexts[INSTANCE_TO_INDEX(CELLULAR_1_INSTANCE)];
            if (cellular1->desired_state)
                goto done;
        }

        {
            cellular_context_t * const active_context = &cellular_contexts[INSTANCE_TO_INDEX(active_sim)];

            switch (active_context->current_state)
            {
                case state_idle:
                case state_disconnected:
                case state_connect:
                    /* if current active sim is not connecting or connected,
                     * send connect message to connect this context.
                     */
                    notify_cellular_to_connect(context);
                    break;

                case state_connecting:
                    if (!is_current_context_active_sim(context, active_sim))
                    {
                        /*
                         * The modem status still says not connected while waiting active sim to be connected
                         * so need to send disconnect.
                         */
                        notify_cellular_to_disconnect(&cellular_contexts[INSTANCE_TO_INDEX(active_sim)]);
                    }
                    break;

                default:
                    /* wait until next state changed.
                     *
                     */
                    //monitor_debug("modem_status_handle: ACTIVE SIM%zu state %d\n", active_sim, active_context->current_state);
                    break;

            }

        }
    }

done:
    return;
}

static void handle_modem_connected_status(cellular_context_t * const context, size_t const active_sim)
{
    cellular_modem_status_t * const modem_status = context->modem_status;

    if (modem_status->desired_status != cellular_disconnecting)
    {
        /* see if this context wants to connect
         *
         */
        if (context->desired_state)
        {

            if ((!is_current_context_active_sim(context, active_sim)) &&
                (context->instance == CELLULAR_1_INSTANCE))
            {
                /* cellular 1 wants to connect so needs to disconnect the active sim
                 *
                 */
                cellular_context_t * const active_context = &cellular_contexts[INSTANCE_TO_INDEX(active_sim)];

                switch (active_context->current_state)
                {
                    case state_initialize:
                    case state_idle:
                    case state_disconnected:
                    case state_disconnecting:
                    case state_disconnect:
                        break;

                    case state_connect:
                    case state_connecting:
                    case state_connected:
                    case state_wait_modem:
                        monitor_debug_printf("modem_status_handle: Send DISCONNECT SIM%d/%zu  (switch to SIM%zu)\n",
                                        active_sim, context->modem_status->active_sim, context->instance);

                        notify_cellular_to_disconnect(&cellular_contexts[INSTANCE_TO_INDEX(active_sim)]);
                        break;

                }
            }
        }
        else if (is_current_context_active_sim(context, active_sim))
        {
            /* request to disconnect this context
             *
             */
            monitor_debug_printf("modem_status_handle: Send DISCONNECT SIM%d/%zu \n",
                            active_sim, context->modem_status->active_sim);
            notify_cellular_to_disconnect(&cellular_contexts[INSTANCE_TO_INDEX(active_sim)]);

        }
    }

}

void modem_status_handler(cellular_context_t * const context, size_t active_sim)
{

    cellular_connect_status_t connect_status = cellular_get_connection_status(context->csman_fd, context->index_3g);
    cellular_modem_status_t * const modem_status = context->modem_status;

    cellular_connect_status_t const old_connect_status = modem_status->connect_status;

    modem_status->heartbeat++;
    modem_status->active_sim = active_sim;

    if (modem_status->connect_status != connect_status)
    {

        monitor_debug_printf("Instance %d: modem connect status %d -> %d in \"%s\"\n",
                      context->instance, modem_status->connect_status, connect_status,
                      (context->current_state < cellular_state_strings_count) ? cellular_state_strings[context->current_state] : "");

        modem_status->connect_status = connect_status;

        switch (connect_status)
        {
            case cellular_disconnected:
                if (is_current_context_active_sim(context, active_sim))
                {
                    if (context->connection_established)
                    {
                        monitor_info("Cellular%zu is disconnected\n", active_sim);
                    }


                    /* TODO: should start out-of-service timer
                     *  before it's really disconnected.
                     */
                    monitor_debug_printf("modem_status_handle: Send DISCONNECTED SIM%d/%zu \n",
                                  active_sim, context->modem_status->active_sim);
                    if (send_monitor_event(context, disconnected_message, NULL, 0) < 0)
                    {
                        modem_status->connect_status = old_connect_status;
                        goto done;
                    }
                    modem_status->desired_status = cellular_disconnected;
                    context->connection_established = false;
                }
                break;
            case cellular_connected:
                if (is_current_context_active_sim(context, active_sim))
                {
                    monitor_info("Cellular%zu is connected\n", active_sim);
                    monitor_debug_printf("modem_status_handle: Send CONNECTED SIM%zu/%zu \n",
                                  active_sim, context->modem_status->active_sim);

                    if (send_monitor_event(context, connected_message, NULL, 0) < 0)
                    {
                        modem_status->connect_status = old_connect_status;
                        goto done;
                    }

                    modem_status->desired_status = cellular_connected;
                    context->connection_established = true;
                }
                break;

            case cellular_connecting:
                break;

            default:
                if (is_current_context_active_sim(context, active_sim))
                {
                    monitor_debug_printf("modem_status_handle: Send DISCONNECT SIM%d/%zu (disconnecting)\n",
                                  active_sim, context->modem_status->active_sim);
                    if (send_monitor_event(context, cellular_message_disconnect, NULL, 0) < 0)
                    {
                        modem_status->connect_status = old_connect_status;
                        goto done;
                    }
                    modem_status->desired_status = cellular_disconnecting;

                }
                break;
        }

    }

    if (active_sim == 0)
    {
        monitor_debug_printf("cellular%zu ACTIVE SIM = %d\n", context->instance, active_sim);
    }

    switch (connect_status)
    {
        case cellular_disconnecting:
            break;

        case cellular_disconnected:
            handle_modem_disconnected_status(context, active_sim);
            break;

        case cellular_connecting:
        case cellular_connected:
            handle_modem_connected_status(context, active_sim);
            break;
    }

    done:
    return;

}

static cellular_event_t * alloc_event(int code, size_t const instance, void * * const info, size_t const info_size)
{
    cellular_event_t * event;

    event = calloc(1, sizeof *event);
    if (event == NULL)
        goto done;

    event->code = code;
    event->instance = instance;
    if (info != NULL)
    {
        /* claim the memory passed with the event */
        event->info = *info;
        *info = NULL;
    }
    event->info_size = info_size;
    cellular_alloc_event_count++;
done:
    return event;
}

void free_event(cellular_event_t * handler_event)
{
    if (handler_event->info != NULL)
    {
        free((void *) handler_event->info);
    }
    free(handler_event);
    cellular_alloc_event_count--;

}

int send_connected_event_to_interface_manager(size_t interface_instance)
{
    return send_event_with_interface_info_to_interface_manager(connected_message, interface_instance);
}

int send_disconnected_event_to_interface_manager(size_t interface_instance)
{
    return send_event_with_interface_info_to_interface_manager(disconnected_message, interface_instance);
}

/*
 *  send_monitor_event
 *  messages to context are delivered by calling this function. code is one of the defined values in
 *  events.h. data is event code specific, and may be NULL
 */
int send_monitor_event(cellular_context_t * const context, int event, void * * const info, size_t const info_size)
{
    int result = -1;
    cellular_event_t * handler_event;

    handler_event = alloc_event(event, context->instance, info, info_size);
    if (handler_event == NULL)
    {
        monitor_debug_printf("alloc_event failed\n");
        goto done;
    }
    result = send_event_to_event_manager(context->event_manager, handler_event);
    if (result != 0)
    {
        monitor_debug_printf("Unable to send monitor event\n");
        free_event(handler_event);
    }
done:
    return result;
}

bool is_current_context_active_sim(cellular_context_t const * const context, size_t const active_sim)
{
    return (active_sim == context->instance);
}
