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

#include "wifi_monitor_timers.h"
#include "wifi_monitor_event.h"
#include "unittest_support.h"

STATIC_FUNCTION void ping_response_timer_callback(void * const arg)
{
    wifi_context_st * const context = (wifi_context_st *)arg;

    send_event_to_wifi_monitor(context->instance, PING_RESPONSE_TIMER_EVENT, NULL, 0);
}

static void wifi_timer_init(wifi_timer_info_st * const timer_info,
                              timer_context_st * const timer_context,
                              void (* const timer_callback)(void * arg),
                              void * const arg)
{
    timer_info->timer_id = create_timer(timer_context, timer_callback, arg);
}

static void wifi_timer_start(wifi_timer_info_st * timer_info, size_t millisecs)
{
    start_timer(timer_info->timer_id, millisecs);
}

static void wifi_timer_stop(wifi_timer_info_st * timer_info)
{
    stop_timer(timer_info->timer_id);
}

void wifi_ping_response_timer_start(wifi_context_st * const context, size_t millisecs)
{
    wifi_timer_start(&context->ping_response_timer, millisecs);
}

void wifi_ping_response_timer_stop(wifi_context_st * const context)
{
    wifi_timer_stop(&context->ping_response_timer);
}

void wifi_timers_init(wifi_context_st * const context, timer_context_st * const timer_context)
{
    context->timer_context = timer_context;

    wifi_timer_init(&context->ping_response_timer, context->timer_context, ping_response_timer_callback, context);
}

