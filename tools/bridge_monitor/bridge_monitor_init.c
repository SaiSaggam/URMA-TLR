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

#include "bridge_monitor_init.h"
#include "bridge_monitor.h"
#include "bridge_monitor_log.h"
#include "bridge_monitor_message.h"
#include "bridge_monitor_event.h"
#include "bridge_monitor_timers.h"
#include "bridge_interfaces.h"
#include "config_mgr.h"
#include "interfaces.h"
#include "tlr_common.h"

#include <stdlib.h>

#define BRIDGE_MONITOR_EVENT_QUEUE_SIZE 200
#define BRIDGE_INSTANCE_IS_VALID(instance)	INSTANCE_IS_VALID((instance), (bridge_monitor_count))

static char const * bridge_interface_name;
static size_t bridge_monitor_count;
static bridge_context_st * * contexts;

/**
 * Called at init time to obtain the number of bridge interfaces from the configuration
 *
 * @author chris (1/20/2016)
 *
 * @return size_t
 * @param void
 *
 */
static size_t get_interface_count_from_config(void)
{
    size_t count;
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);

    count = table->groups[config_lan].instances;

    return count;
}

static bridge_context_st * bridge_context_create(size_t const instance, timer_context_st * const timer_context)
{
    bridge_context_st * const context = calloc(1, sizeof *context);

    if (context == NULL)
    {
        goto done;
    }
    context->instance = instance;
    // XXX should prod the interface manager to tell us the desired state.
    context->desired_state = bridge_state_disconnected;

    bridge_timers_init(context, timer_context);

done:
    return context;
}

static bool bridge_contexts_create(size_t num_contexts, timer_context_st * timer_context)
{
    bool all_contexts_setup;
    int i;

    contexts = calloc(num_contexts, sizeof *contexts);
    if (contexts == NULL)
    {
        bridgemon_error("Memory shortage\n");
        all_contexts_setup = false;
        goto done;
    }

    for (i = 0; i < num_contexts; i++)
    {
        contexts[i] = bridge_context_create(INDEX_TO_INSTANCE(i), timer_context);
        if (contexts[i] == NULL)
        {
            bridgemon_error("Memory shortage\n");
            all_contexts_setup = false;
            goto done;
        }
    }

    all_contexts_setup = true;

done:
    return all_contexts_setup;
}

static void bridge_contexts_destroy(void)
{
    if (contexts != NULL)
    {
        int i;

        for (i = 0; i < bridge_monitor_count; i++)
        {
            free(contexts[i]);
        }
        free(contexts);
    }
}

bridge_context_st * bridge_get_context(size_t const instance)
{
    bridge_context_st * context;

    if (!BRIDGE_INSTANCE_IS_VALID(instance))
    {
        context = NULL;
        goto done;
    }
    size_t index = INSTANCE_TO_INDEX(instance);
    context = contexts[index];

done:
    return context;
}

size_t bridge_get_num_contexts(void)
{
    return bridge_monitor_count;
}

char const * bridge_interface_name_get(void)
{
    return bridge_interface_name;
}

static void bridge_monitor_cleanup(void)
{
    bridge_contexts_destroy();
    bridge_messaging_teardown();
}

bool bridge_monitor_start(void)
{
	int bridge_monitor_started;
    timer_context_st * timer_context;

    bridge_interface_name = interface_get_name_from_type(bridge_interface);

    timer_context = open_timer_context();
    if (timer_context == NULL)
    {
        bridgemon_error("could not open timer context\n");
        bridge_monitor_started = false;
        goto done;
    }

    bridge_monitor_count = get_interface_count_from_config();
    if (bridge_monitor_count == 0)
    {
        bridgemon_error("No %s interfaces supported\n", bridge_interface_name_get());
        bridge_monitor_started = false;
        goto done;
    }

    if (bridge_interface_name == NULL)
    {
        bridgemon_error("unknown interface type (%d)\n", bridge_interface);
        bridge_monitor_started = false;
        goto done;
    }

    if (!bridge_interfaces_create(FIRST_BRIDGE_INSTANCE, bridge_monitor_count))
    {
        bridge_monitor_started = false;
        goto done;
    }

    if (!bridge_event_init(BRIDGE_MONITOR_EVENT_QUEUE_SIZE))
    {
        bridge_monitor_started = false;
        goto done;
    }

    if (!bridge_contexts_create(bridge_monitor_count, timer_context))
    {
        bridge_monitor_started = false;
        goto done;
    }

    /*
     * before we start listening for messages, place an INITIALISE event on the event queue so
     * that is sure to be the first event processed.
     * Note that the event also has to be set _after_ the contexts have been created as the event
     * handler starts processing events as soon as they arrive, and the handler assumes that the
     * contexts already exist.
     */
    send_event_to_bridge_monitor(FIRST_BRIDGE_INSTANCE, INITIALISE_EVENT, NULL, 0);

    /*
     * Once all contexts have been set up we can start listening for messages from other processes
     */
    if (!bridge_messaging_setup())
    {
        bridge_monitor_started = false;
        goto done;
    }

    bridge_monitor_started = true;

done:
    if (bridge_monitor_started == false)
    {
        bridge_monitor_cleanup();
    }

	return bridge_monitor_started;
}
