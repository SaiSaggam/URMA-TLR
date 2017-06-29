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

#include "failover_show.h"
#include "ifmanager.h"
#include "failover.h"
#include "plain_socket.h"
#include "config_mgr.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "messages.h"

void send_failover_status(ifm_config_t const * const context, int const socket_fd)
{
    int instance_in = 1;
    int const max_instance = failover_get_number_of_contexts();

    while (instance_in <= max_instance)
    {
        failover_info_t * info = failover_get_context(instance_in);
        if (info != NULL && info->valid == true)
        {
            ifm_if_config_t const * if_ptr_from = ifm_get_interface_entry(&info->config.from);
            ifm_if_config_t const * if_ptr_to = ifm_get_interface_entry(&info->config.to);
            int instance = info->instance;
            dprintf(socket_fd, "failover%d valid:%s\n", instance, "yes");
            if (if_ptr_from)
            {
                if (info->config.from.interface == dsl_interface)
                    dprintf(socket_fd, "failover%d primary:%s\n", instance, info->config.from.name);
                else
                    dprintf(socket_fd, "failover%d primary:%s%d\n", instance, info->config.from.name, (int)info->config.from.instance);
                if_ptr_from = ifm_get_interface_entry(&info->config.from);
                dprintf(socket_fd, "failover%d primary_status:%s\n", instance, (if_ptr_from->current_state == ifm_state_connected)? "Up":"Down");
            }
            else
            {
                dprintf(socket_fd, "failover%d primary:%s\n", instance, "none");
                dprintf(socket_fd, "failover%d primary_status:%s\n", instance, "Down");
            }

            if (if_ptr_to)
            {
                if (info->config.to.interface == dsl_interface)
                    dprintf(socket_fd, "failover%d secondary:%s\n", instance, info->config.to.name);
                else
                    dprintf(socket_fd, "failover%d secondary:%s%d\n", instance, info->config.to.name, (int)info->config.to.instance);
                dprintf(socket_fd, "failover%d secondary_status:%s\n", instance, (if_ptr_to->current_state == ifm_state_connected)? "Up":"Down");
            }
            else
            {
                dprintf(socket_fd, "failover%d secondary:%s\n", instance, "none");
                dprintf(socket_fd, "failover%d secondary_status:%s\n", instance, "Down");
            }

            switch (info->config.use_type)
            {
                case config_failover_use_passive:
                {
                    if (if_ptr_from && if_ptr_from->current_state == ifm_state_connected && info->state == failover_state_idle)
                    {
                        if (info->config.from.interface == dsl_interface)
                            dprintf(socket_fd, "failover%d current:%s\n", instance, info->config.from.name);
                        else
                            dprintf(socket_fd, "failover%d current:%s%d\n", instance, info->config.from.name, (int)info->config.from.instance);
                    }
                    else if (if_ptr_to && if_ptr_to->current_state == ifm_state_connected && info->state == failover_state_done)
                    {
                        if(info->config.to.interface == dsl_interface)
                            dprintf(socket_fd, "failover%d current:%s\n", instance, info->config.to.name);
                        else
                            dprintf(socket_fd, "failover%d current:%s%d\n", instance, info->config.to.name, (int)info->config.to.instance);
                    }
                    else
                        dprintf(socket_fd, "failover%d current:%s\n", instance, "none");

                        dprintf(socket_fd, "failover%d detection method:%s\n", instance, "passive");
                    break;
                }
                case config_failover_use_ping:
                {
                    //TODO: not working
                    if (info->current_iface != NULL)
                    {
                        if (info->current_iface->interface == dsl_interface )
                            dprintf(socket_fd, "failover%d current:%s\n", instance, info->current_iface->name);
                        else
                            dprintf(socket_fd, "failover%d current:%s%d\n", instance, info->current_iface->name, (int)info->current_iface->instance);
                    }
                    else
                        dprintf(socket_fd, "failover%d current:%s\n", instance, "none");

                        dprintf(socket_fd, "failover%d detection method:%s\n", instance, "probe");
                    break;
                }
            }

            dprintf(socket_fd, "failover%d last ping received:%d\n", instance, (int)(time(NULL) - info->last_successful_probe_timestamp.tv_sec));

            if (info->config.timeout_after && timer_is_running(info->timer_id) && info->current_iface != &info->config.to)
            {
                dprintf(socket_fd, "failover%d failing over in:%d\n", instance, seconds_remaining(info->timer_id));
            }

            if (info->config.switch_primary_up && timer_is_running(info->timer_switch_primary_up_id))
            {
                dprintf(socket_fd, "failover%d switch back in:%d\n", instance, seconds_remaining(info->timer_switch_primary_up_id));
            }
        }
        instance_in++;
    }

    dprintf(socket_fd, "\n");

    /* all done; close the socket */
    close(socket_fd);
}

int show_init(ifm_config_t * const context)
{
    plain_socket_context_st * plain_socket_context;

    // TODO: "FAILOVER_SHOW_SOCKET" needs to be centrally defined somewhere (see show_failover_work.c)
    plain_socket_context = open_plain_socket_context("FAILOVER_SHOW_SOCKET", true, ifm_show_failover_plain_socket_callback, context);
    if (plain_socket_context == NULL)
    {
        ifm_debug("show_failover failed to open plain socket context\n");
        goto error;
    }

    return EXIT_SUCCESS;

error:
    return EXIT_FAILURE;
}
