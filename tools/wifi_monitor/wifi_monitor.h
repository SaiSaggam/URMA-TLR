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

#ifndef __WIFI_MONITOR_H__
#define __WIFI_MONITOR_H__

#include "wifi_monitor_radio_config_commands.h"
#include "timer.h"
#include "parsing.h"
#include <stddef.h>
#include <stdbool.h>

#define FIRST_WIFI_INSTANCE 1
#ifdef RADIO_24
#define WIFI_MONITOR_INTERFACE wifi_interface
#endif

#ifdef RADIO_5G
#define WIFI_MONITOR_INTERFACE wifi5g_interface
#endif


typedef struct wifi_context_st wifi_context_st;
typedef struct wifi_timer_info_st wifi_timer_info_st;

typedef enum wifi_state_t
{
    wifi_state_disconnected,
    wifi_state_connected,
} wifi_state_t;

struct wifi_timer_info_st
{
    timer_id_st * timer_id;
};

struct wifi_context_st
{
    size_t instance;

    wifi_common_elements_t * config;

    timer_context_st * timer_context;

    wifi_state_t desired_state;
    wifi_state_t current_state;

    ping_info_st const * saved_ping_response_info;

    wifi_timer_info_st ping_response_timer;
};

wifi_context_st * wifi_get_context(size_t const instance);

#endif /* __WIFI_MONITOR_H__ */
