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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdbool.h>

struct timer_context_st;
typedef struct timer_context_st timer_context_st;
struct timer_id_st;
typedef struct timer_id_st timer_id_st;

timer_id_st * create_timer(timer_context_st * const context, void (*callback)(void * data), void * data);
void start_timer(timer_id_st * const timer_id, unsigned int millisecs);
int stop_timer(timer_id_st * const timer_id);
int destroy_timer(timer_id_st * timer_id);
timer_context_st * open_timer_context(void);
bool timer_is_running(timer_id_st * const timer_id);
int seconds_remaining(timer_id_st * const timer_id);

#endif /*  __TIMER_H__ */
