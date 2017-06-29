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

#ifndef __ETH_MONITOR_H__
#define __ETH_MONITOR_H__

#if !defined(ETH_MONITOR_DEBUG)
#define ETH_MONITOR_DEBUG 1
#endif

#include "config_def.h"
#include "apply_eth.h"
#include "parsing.h"
#include <event_manager.h>
#include <timer.h>
#include <messages.h>
#include <stdbool.h>

#define ETH_NOT_ASSIGNED_TO_A_WAN  0

typedef enum
{
	state_init,
	state_eth_up,
	state_eth_down,
} state_t;

typedef struct
{
	size_t instance;    /* which monitor instance is this. NB This is 1-based, not 0-based */
	state_t	state;
	bool 	desired_eth_state;
	bool	link_state;
	config_eth_t parameters[eth_count];
	config_eth_t last_parameters[eth_count];
	char ** values;
	char ** last_values;

	timer_context_st * timer_context;
	timer_id_st * poll_timer_id;
	timer_id_st * ping_reponse_timer_id;
    ping_info_st       const *p_response_info;
    bool    is_member_of_bridge;
    bool    dhcp_client_enabled;
    size_t  wan_instance;
} eth_context_t;

#if ETH_MONITOR_DEBUG
#define DEBUG(x...)		do { printf("eth_monitor: %s(): ", __func__); printf(x); printf("\n"); } while (0)
#define boolstr(b)		((b) ? "true" : "false")
#else
#define DEBUG(x...)		do { } while (0)
#endif


int eth_monitor(void);

int eth_config_update(eth_context_t * const context);
int eth_do_connect_action(eth_context_t * const context);
int eth_do_disconnect_action(eth_context_t * const context);
int eth_do_joined_bridge_action(eth_context_t * const context);
int eth_do_left_bridge_action(eth_context_t * const context);
void poll_eth_status(eth_context_t * const context);
void set_eth_interface_state(eth_context_t * const context, state_t new_state);
int eth_do_assigned_to_wan(eth_context_t * const context, wan_info_st const * const wan_info);
int eth_do_removed_from_wan(eth_context_t * const context, wan_info_st const * const wan_info);

#endif /* __ETH_MONITOR_H__ */
