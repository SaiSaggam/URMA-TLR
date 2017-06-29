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

#ifndef __DSL_CONTEXT_H__
#define __DSL_CONTEXT_H__

#include "dsl_state.h"
#include "dsl_timer.h"
#include "timer.h"
#include "event_manager.h"
#include "parsing.h"
#include "lr54leds.h"

#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>
#include <inttypes.h>

typedef struct
{
	char * value;
	size_t size;
} dsl_element_t;

typedef struct
{
	size_t instance;					// monitor instance; note: this is 1-based, not 0-based

	event_manager_st * event_manager;

    timer_context_st * timer_context;

	dsl_timer_t connecting_timer;
	dsl_timer_t disconnecting_timer;

	dsl_timer_t delay_up_timer;
    uint32_t delay_up_timer_period;

	bool is_line_up;

    dsl_state_t desired_state;
    dsl_state_t current_state;
    dsl_state_t notified_state;

    dsl_element_t * config;

	dsl_timer_t ping_response_timer;
    ping_info_st const * saved_ping_response_info;

    config_state_dsl_admin_status_t admin_status;
    config_state_dsl_oper_status_t oper_status;

    led_state_t current_led_state;
} dsl_context_t;

#endif /*__DSL_CONTEXT_H__*/

