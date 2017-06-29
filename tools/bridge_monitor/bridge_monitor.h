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

#ifndef __BRIDGE_MONITOR_H__
#define __BRIDGE_MONITOR_H__

#include "timer.h"
#include "parsing.h"
#include "config_utils.h"
#include <stddef.h>
#include <stdbool.h>

#define FIRST_BRIDGE_INSTANCE 1
typedef struct bridge_context_st bridge_context_st;
typedef struct bridge_timer_info_st bridge_timer_info_st;

typedef enum bridge_state_t
{
    bridge_state_disconnected,
    bridge_state_connected,
} bridge_state_t;

struct bridge_timer_info_st
{
    timer_id_st * timer_id;
    bool timer_running;
};

struct bridge_context_st
{
    size_t instance;

    config_st * config;

    timer_context_st * timer_context;

    bridge_state_t desired_state;
    bridge_state_t current_state;

    ping_info_st const * saved_ping_response_info;

    bridge_timer_info_st ping_response_timer;
};

bridge_context_st * bridge_get_context(size_t const instance);

#endif /* __BRIDGE_MONITOR_H__ */
