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

#include "eth_monitor.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "config_mgr.h"
#include "ethphy.h"
#include "ethregs.h"


/********************
	Functions
********************/

static int get_eth_link_status(int port, bool * linkup)
{
	int     phy_stat;


	if (reg_read(port, PHY_STAT, &phy_stat) < 0)
	{
		return -1;
	}

	if ((phy_stat & PHY_STAT_LINKUP) == 0)
		// not connected
		*linkup = false;
	else
		*linkup = true;
	return 0;
}

void poll_eth_status(eth_context_t * const context)
{

	bool 	link_state;
	int 	port = context->instance;

	if (get_eth_link_status(port, &link_state) == 0)
	{
		if (context->link_state != link_state)
		{
			// Change in link status
			DEBUG("get_eth_link_status(%d): new link_state=%d, old= %d genstate:%d d_eth_state:%d\n", port, link_state, context->link_state, context->state, context->desired_eth_state);
			if (link_state == true && context->desired_eth_state == true)
			{
				if (eth_do_connect_action(context) == 0)
				{
					set_eth_interface_state(context, state_eth_up);
				}
			}
			context->link_state = link_state;   // remember the link state.
		}
		if (link_state == false && context->state == state_eth_up)
		{
			if (eth_do_disconnect_action(context) == 0)
			{
				set_eth_interface_state(context, state_eth_down);
			}
		}
	}

}
