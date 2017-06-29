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

#ifndef __DSL_TIMER_H__
#define __DSL_TIMER_H__

#include "dsl_event.h"
#include "timer.h"

#include <stdbool.h>

typedef struct
{
    timer_id_st * id;
    dsl_event_code_t event_code;
    char const * name;
} dsl_timer_t;

bool dsl_timer_initialise(timer_context_st * const timer_context, dsl_timer_t * const timer, char const * const name, dsl_event_code_t const event_code);
void dsl_timer_start(dsl_timer_t * const timer, size_t const milliseconds);
void dsl_timer_stop(dsl_timer_t * const timer);
bool dsl_timer_is_running(dsl_timer_t * const timer);

#endif // __DSL_TIMER_H__
