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
#include "bridge_monitor_event.h"
#include "bridge_monitor_init.h"
#include "bridge_monitor_log.h"
#include "bridge_monitor_event_handlers.h"
#include "bridge_monitor_event_alloc.h"
#include "unittest_support.h"
#include <stdlib.h>

static event_manager_st * bridge_event_manager;

/*
 * Called whenever a new bridge monitor event is received
*/
STATIC_FUNCTION void bridgemon_event_handler(void * const user_context, void * const data)
{
    bridge_monitor_event_t * const event = (bridge_monitor_event_t *)data;
    bridge_context_st * const context = bridge_get_context(event->instance);

    if (context == NULL)
    {
        /*
         * This might happen if somebody sends a message to bridge monitor specifying an invalid
         * instance number (e.g. using the sendmsg application).
        */
        bridgemon_warning("Couldn't locate context for %s instance %u", bridge_interface_name_get(), event->instance);
        goto done;
    }

    switch (event->code)
    {
        // Private events
        case INITIALISE_EVENT:
            bridge_handle_initialise_event();
            break;
        case PING_RESPONSE_TIMER_EVENT:
            bridge_handle_ping_response_timeout_event(context);
            break;


            // System wide events...
        case UPDATE_MESSAGE_EVENT:
            bridge_handle_update_event(context);
            break;
        case CONNECT_MESSAGE_EVENT:
            bridge_handle_connect_event(context);
            break;
        case DISCONNECT_MESSAGE_EVENT:
            bridge_handle_disconnect_event(context);
            break;
        case CONNECTED_MESSAGE_EVENT:
            bridge_handle_connected_event(context);
            break;
        case DISCONNECTED_MESSAGE_EVENT:
            bridge_handle_disconnected_event(context);
            break;
        case PING_MESSAGE_EVENT:
            bridge_handle_ping_message_event(context, event);
            break;
    }

done:
    bridgemon_event_free(data);
}

/*
 * Used by other threads (e.g. messaging thread) to deliver events to an instance of this monitor.
 * If the monitor wishes to retain the 'info' at *info it should set *info to NULL to indicate to
 * the messaging code that it doesn't need to free it. The monitor then becomes responsible fro
 * freeing that memory.
 */
int send_event_to_bridge_monitor(size_t const instance, int const event_code, void * * const info, size_t const info_size)
{
    int result;
    bridge_monitor_event_t * event;

    //bridgemon_debug("got message for bridge monitor code %d\n", event_code);

    event = bridgemon_event_alloc(event_code, instance, info, info_size);
    if (event == NULL)
    {
        result = -1;
        goto done;
    }

    if (send_event_to_event_manager(bridge_event_manager, event) != 0)
    {
        result = -1;
        goto done;
    }
    result = 0;

done:
    if (result != 0)
    {
        bridgemon_event_free(event);
    }

    return result;

}

bool bridge_event_init(size_t const event_queue_size)
{
    bool event_manager_setup;

    bridge_event_manager = new_event_manager(NULL, event_queue_size, bridgemon_event_handler);

    if (bridge_event_manager == NULL)
    {
        event_manager_setup = false;
    }
    else
    {
        event_manager_setup = true;
    }

    return event_manager_setup;
}


