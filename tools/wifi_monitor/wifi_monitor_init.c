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

#include "wifi_monitor_init.h"
#include "wifi_monitor.h"
#include "wifi_monitor_log.h"
#include "wifi_monitor_message.h"
#include "wifi_monitor_event.h"
#include "wifi_monitor_timers.h"
#include "wifi_radio.h"
#include "config_mgr.h"
#include "interfaces.h"
#include "tlr_common.h"

#include <stdlib.h>

#define WIFI_MONITOR_EVENT_QUEUE_SIZE 200
#define WIFI_INSTANCE_IS_VALID(instance)	INSTANCE_IS_VALID((instance), (wifi_monitor_count))

static char const * wifi_interface_name;    // wifi or wifi5g depending... use this in messages to ifman
static unsigned int wifi_monitor_count;
static wifi_context_st * * contexts;



static wifi_context_st * wifi_context_create(size_t const instance, timer_context_st * const timer_context)
{
    wifi_context_st * const context = calloc(1, sizeof *context);

    if (context == NULL)
    {
        goto done;
    }
    context->instance = instance;
    // XXX should prod the interface manager to tell us the desired state.
    context->desired_state = wifi_state_disconnected;

    wifi_timers_init(context, timer_context);

done:
    return context;
}

static bool wifi_contexts_create(size_t const num_contexts, timer_context_st * const timer_context)
{
    bool all_contexts_setup;
    int i;

    contexts = calloc(num_contexts, sizeof *contexts);
    if (contexts == NULL)
    {
        wifi_monitor_debug("Memory shortage\n");
        all_contexts_setup = false;
        goto done;
    }

    for (i = 0; i < num_contexts; i++)
    {
        contexts[i] = wifi_context_create(INDEX_TO_INSTANCE(i), timer_context);
        if (contexts[i] == NULL)
        {
			int 	j;
			for (j = 0; j < i; j++)
			{
				free(contexts[j]);
			}
			wifi_monitor_debug("Memory shortage\n");
            free(contexts);
            contexts = NULL;
            all_contexts_setup = false;
            goto done;
        }
    }

    all_contexts_setup = true;

done:
    return all_contexts_setup;
}

static void wifi_contexts_destroy(void)
{
    if (contexts != NULL)
    {
        int i;

        for (i = 0; i < wifi_monitor_count; i++)
        {
            free(contexts[i]);
        }
        free(contexts);
    }
}

wifi_context_st * wifi_get_context(size_t const instance)
{
    wifi_context_st * context;

    if (!WIFI_INSTANCE_IS_VALID(instance))
    {
        context = NULL;
        goto done;
    }
    size_t const index = INSTANCE_TO_INDEX(instance);
    context = contexts[index];

done:
    return context;
}

unsigned int wifi_get_num_contexts(void)
{
    return wifi_monitor_count;
}

char const * wifi_interface_name_get(void)
{
    return wifi_interface_name;
}

char const * wifi_process_name_get(void)
{
    return WIFI_MONITOR_PROCESS_NAME;
}

static void wifi_monitor_cleanup(void)
{
    wifi_contexts_destroy();
    wifi_messaging_teardown();
}


bool wifi_monitor_start(void)
{
	int wifi_monitor_started;
    timer_context_st * timer_context;

    // WIFI_MONITOR_INTERFACE is a define which should be either wifi_interface or wifi5g_interface
    wifi_interface_name = interface_get_name_from_type(WIFI_MONITOR_INTERFACE);
    if (wifi_interface_name == NULL)
    {
        wifi_monitor_debug("unknown interface type (%d)\n", wifi_interface);
        wifi_monitor_started = false;
        goto done;
    }

    timer_context = open_timer_context();
    if (timer_context == NULL)
    {
        wifi_monitor_debug("could not open timer context\n");
        wifi_monitor_started = false;
        goto done;
    }

    wifi_monitor_count = wifi_radio_interface_count_from_config();
    if (wifi_monitor_count == 0)
    {
        wifi_monitor_started = false;
        goto done;
    }

    if (!wifi_monitor_event_init(WIFI_MONITOR_EVENT_QUEUE_SIZE))
    {
        wifi_monitor_started = false;
        goto done;
    }

    if (!wifi_contexts_create(wifi_monitor_count, timer_context))
    {
        wifi_monitor_started = false;
        goto done;
    }

    /*
     * Once all contexts have been set up we can start listening for messages from other processes
     */
    if (!wifi_messaging_setup(WIFI_MONITOR_INTERFACE))
    {
        wifi_monitor_started = false;
        goto done;
    }

    wifi_monitor_started = true;

done:
    if (!wifi_monitor_started)
    {
        wifi_monitor_cleanup();
    }

	return wifi_monitor_started;
}
