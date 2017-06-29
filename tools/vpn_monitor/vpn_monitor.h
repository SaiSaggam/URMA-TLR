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

#ifndef __VPN_MONITOR_H__
#define __VPN_MONITOR_H__

#include "vpn_config.h"
#include <stddef.h>
#include <stdbool.h>
#include "parsing.h"
#include "timer.h"

typedef enum vpn_monitor_state_t
{
    state_init,         /* context has just started */
    state_tunnel_down,
    state_tunnel_connecting,
    state_tunnel_disconnecting,
    state_tunnel_interconnect_delay,   /* a delay between disconnect and connect attempts */
    state_tunnel_up
} vpn_monitor_state_t;

#define VPN_NAME_LEN 64

/* there will be one context per vpn tunnel configuration */
typedef struct vpn_tunnel_context_st
{
    size_t instance;        /* which monitor instance is this. NB This is 1-based, not 0-based */
    char name[VPN_NAME_LEN];
    bool desired_vpn_state;
    vpn_monitor_state_t actual_vpn_state;
    timer_context_st * timer_context;
    timer_id_st * connecting_timer_id;
    bool connecting_timer_running;
    timer_id_st * disconnecting_timer_id;
    bool disconnecting_timer_running;
    timer_id_st * interconnect_timer_id;
    bool interconnect_timer_running;
    timer_id_st * vpn_pingmsg_reponse_timer_id;
    ping_info_st       const *p_response_info;
    config_st * config;
} vpn_tunnel_context_st;

int vpn_monitor_thread(void);

#endif /* __VPN_MONITOR_H__ */
