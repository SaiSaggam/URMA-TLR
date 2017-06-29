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

#include "dsl_timer.h"
#include "dsl_context.h"
#include "dsl_event.h"
#include "dsl_monitor.h"
#include "dsl_log.h"
#include "timer.h"
#include "tlr_common.h"

#include <stdbool.h>

static void dsl_timer_callback(void * const data)
{
    dsl_timer_t const * const timer = data;

	dsl_debug("timer '%s' expired\n", timer->name);

	dsl_monitor_send_event(DSL_MONITOR_INSTANCE, timer->event_code, NULL, 0);
}

bool dsl_timer_initialise(timer_context_st * const timer_context, dsl_timer_t * const timer, char const * const name, dsl_event_code_t const event_code)
{
    bool timer_initialised = false;

    timer->id = create_timer(timer_context, dsl_timer_callback, timer);
    if (timer->id == NULL)
    {
        dsl_error("timer '%s' initialisation failed\n", name);
        goto done;
    }

    timer->event_code = event_code;
    timer->name = name;

    timer_initialised = true;

done:
    return timer_initialised;
}

void dsl_timer_start(dsl_timer_t * const timer, size_t const milliseconds)
{
    start_timer(timer->id, milliseconds);

	dsl_debug("timer '%s' started, expiring in %u seconds\n", timer->name, (unsigned)milliseconds_to_seconds(milliseconds, NULL));
}

void dsl_timer_stop(dsl_timer_t * const timer)
{
    if (stop_timer(timer->id) == 0)
    {
	    dsl_debug("timer '%s' stopped\n", timer->name);
    }
}

bool dsl_timer_is_running(dsl_timer_t * const timer)
{
    return timer_is_running(timer->id);
}
