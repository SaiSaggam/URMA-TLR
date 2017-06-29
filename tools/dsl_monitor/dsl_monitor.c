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

#include "dsl_monitor.h"
#include "dsl_event.h"
#include "dsl_ppp.h"
#include "dsl_modem.h"
#include "dsl_log.h"
#include "dsl_config.h"
#include "dsl_show.h"
#include "event_manager.h"
#include "messages.h"
#include "interfaces.h"
#include "timer.h"
#include "tlr_common.h"

#include <parsing.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define NUM_DSL_MONITOR_CONTEXTS	1
#define DSL_MONITOR_EVENT_QUEUE_SIZE 100
#define IS_VALID_DSL_INSTANCE(instance)	INSTANCE_IS_VALID((instance), (NUM_DSL_MONITOR_CONTEXTS))

static char const * dsl_interface_name;
static dsl_context_t contexts[NUM_DSL_MONITOR_CONTEXTS];

int dsl_monitor_start(timer_context_st * const timer_context)
{
	int result = -1;
	size_t i;

    dsl_interface_name = interface_get_name_from_type(dsl_interface);
    if (dsl_interface_name == NULL)
    {
        dsl_error("unknown interface type (%d)\n", dsl_interface);
        goto done;
    }

	for (i = 0; i < NUM_DSL_MONITOR_CONTEXTS; i++)
	{
		dsl_context_t * const context = &contexts[i];

		context->instance = INDEX_TO_INSTANCE(i);
        context->timer_context = timer_context; /* all contexts use the same timer context */
        context->current_state = dsl_state_disconnected;

        if (!dsl_timer_initialise(timer_context, &context->connecting_timer, "connecting", dsl_event_connecting_timeout) ||
            !dsl_timer_initialise(timer_context, &context->delay_up_timer, "delay-up", dsl_event_delay_up_timeout) ||
            !dsl_timer_initialise(timer_context, &context->disconnecting_timer, "disconnecting", dsl_event_disconnecting_timeout) ||
            !dsl_timer_initialise(timer_context, &context->ping_response_timer, "ping response", dsl_event_ping_response_timeout))
        {
            goto done;
        }

		context->event_manager = new_event_manager(context, DSL_MONITOR_EVENT_QUEUE_SIZE, dsl_event_handler);
        if (context->event_manager == NULL)
        {
        	dsl_error("failed to create event manager\n");
            goto done;
        }

        if (!dsl_config_initialise(context))
        {
        	goto done;
        }

        if (show_init(context) != 0)
        {
        	goto done;
        }
	}

	result = 0;

done:
	return result;
}

char const * dsl_monitor_interface_name(void)
{
    return dsl_interface_name;
}

int dsl_monitor_send_event(size_t const instance, int const code, void * * const info, size_t const info_size)
{
	int result = -1;
    dsl_context_t * context;

    if (!IS_VALID_DSL_INSTANCE(instance))
    	goto done;

    context = &contexts[INSTANCE_TO_INDEX(instance)];
	result = dsl_event_send_to_dsl_monitor(context->event_manager, code, info, info_size);

done:
	return result;
}
