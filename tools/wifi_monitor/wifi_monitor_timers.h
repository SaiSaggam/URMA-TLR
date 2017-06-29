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

#ifndef __WIFI_MONITOR_TIMERS_H__
#define __WIFI_MONITOR_TIMERS_H__

#include "wifi_monitor.h"
#include "timer.h"
#include <stddef.h>

void wifi_ping_response_timer_start(wifi_context_st * const context, size_t millisecs);
void wifi_ping_response_timer_stop(wifi_context_st * const context);

void wifi_timers_init(wifi_context_st * const context, timer_context_st * const timer_context);

#if defined(UNIT_TEST)
void ping_response_timer_callback(void * arg);
#endif

#endif /* __WIFI_MONITOR_TIMERS_H__ */
