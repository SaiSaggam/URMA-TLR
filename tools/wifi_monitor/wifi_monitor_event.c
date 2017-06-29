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

#include "wifi_monitor.h"
#include "wifi_monitor_event.h"
#include "wifi_monitor_init.h"
#include "wifi_monitor_log.h"
#include "wifi_monitor_event_handlers.h"
#include "wifi_monitor_event_alloc.h"
#include "wifi_radio.h"
#include "unittest_support.h"
#include <stdlib.h>

static event_manager_st * wifi_event_manager;

/*
 * Called whenever a new wifi monitor event is received (or a wifi5g monitor event)
*/
STATIC_FUNCTION void wifi_monitor_event_handler(void * const user_context, void * const data)
{
    wifi_monitor_event_t * const event = (wifi_monitor_event_t *)data;
    wifi_context_st * const context = wifi_get_context(event->instance);

    if (context == NULL)
    {
        /*
         * This might happen if somebody sends a message to wifi monitor specifying an invalid
         * instance number (e.g. using the sendmsg application).
        */
        wifi_monitor_debug("Couldn't locate context for %s instance %u", wifi_interface_name_get(), event->instance);
        goto done;
    }

    switch (event->code)
    {
        // Private events
        case PING_RESPONSE_TIMER_EVENT:
            wifi_handle_ping_response_timeout_event(context);
            break;


            // System wide events...
        case UPDATE_MESSAGE_EVENT:
            wifi_handle_update_event(context);
            break;
        case CONNECT_MESSAGE_EVENT:
            wifi_handle_connect_event(context);
            break;
        case DISCONNECT_MESSAGE_EVENT:
            wifi_handle_disconnect_event(context);
            break;
        case CONNECTED_MESSAGE_EVENT:
            wifi_handle_connected_event(context);
            break;
        case DISCONNECTED_MESSAGE_EVENT:
            wifi_handle_disconnected_event(context);
            break;
        case PING_MESSAGE_EVENT:
            wifi_handle_ping_message_event(context, event);
            break;
    }

done:
    wifi_monitor_event_free(event);
}

/*
 * Used by other threads (e.g. messaging thread) to deliver events to an instance of this monitor.
 * If the monitor wishes to retain the 'info' at *info it should set *info to NULL to indicate to
 * the messaging code that it doesn't need to free it. The monitor then becomes responsible fro
 * freeing that memory.
 */
int send_event_to_wifi_monitor(size_t const instance, int const event_code, void * * const info, size_t const info_size)
{
    int result;
    wifi_monitor_event_t * event;

    event = wifi_monitor_event_alloc(event_code, instance, info, info_size);
    if (event == NULL)
    {
        result = -1;
        goto done;
    }

    if (send_event_to_event_manager(wifi_event_manager, event) != 0)
    {
        result = -1;
        goto done;
    }
    result = 0;

done:
    if (result != 0)
    {
        wifi_monitor_event_free(event);
    }

    return result;

}

bool wifi_monitor_event_init(size_t const event_queue_size)
{
    wifi_event_manager = new_event_manager(NULL, event_queue_size, wifi_monitor_event_handler);
    return (wifi_event_manager != NULL);
}


