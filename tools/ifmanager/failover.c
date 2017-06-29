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
#include "failover.h"
#include "failover_debug.h"
#include "failover_config.h"
#include "ifmanager.h"
#include "route.h"
#include <strlcpy.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include "interfaces.h"
#include "config_mgr.h"
#include <tlr_common.h>
#include <sys/time.h>


#define MAX_PROBE_RESPONSE_TIME_SECCONDS 1000
#define IS_VALID_INDEX(index)           ((index) < failover_contexts_count)

tlr_log_t failover_debug_priority = tlr_log_debug;

static unsigned int failover_contexts_count = 0;
static failover_info_t * failover_contexts = NULL;

/*
 * These strings are only referenced sometimes, and generate errors if they're not. Only declare
 * them if necessary.
 */
#if !defined(_CUNIT_) || IFMANAGER_DEBUG
static char const * failover_state_strings[] =
{
    FOREACH_FAILOVER_STATE(GENERATE_STRING)
};
#endif


// These mappings are also used by the routing routines.
static void failover_stop(failover_info_t * const failover);
static void failover_start(failover_info_t * const failover);
static void failover_timeout_start(failover_info_t * const failover);
static void failover_probe_start(failover_info_t * const failover);
static void failover_handle_connected_state_to(failover_info_t * const failover);
static void failover_handle_connected_state_from(failover_info_t * const failover);


typedef struct failover_name_mapping_t
{
    interface_info_st info;
    char *ifname;
} failover_name_mapping;


static const failover_name_mapping failover_interface_mappings[] =
{
    [config_failover_from_none] =
    {
        .info.interface = invalid_interface, //first_valid_interface,
        .info.instance = 0
    },
    [config_failover_from_lan1] =
    {
        .info.interface = bridge_interface,
        .info.instance = 1,
        .ifname = "lan1"
    },
    [config_failover_from_lan2] =
    {
        .info.interface = bridge_interface,
        .info.instance = 2,
        .ifname = "lan2"
    },
    [config_failover_from_lan3] =
    {
        .info.interface = bridge_interface,
        .info.instance = 3,
        .ifname = "lan3"
    },
    [config_failover_from_lan4] =
    {
        .info.interface = bridge_interface,
        .info.instance = 4,
        .ifname = "lan4"
    },
    [config_failover_from_lan5] =
    {
        .info.interface = bridge_interface,
        .info.instance = 5,
        .ifname = "lan5"
    },
    [config_failover_from_lan6] =
    {
        .info.interface = bridge_interface,
        .info.instance = 6,
        .ifname = "lan6"
    },
    [config_failover_from_lan7] =
    {
        .info.interface = bridge_interface,
        .info.instance = 7,
        .ifname = "lan7"
    },
    [config_failover_from_lan8] =
    {
        .info.interface = bridge_interface,
        .info.instance = 8,
        .ifname = "lan8"
    },
    [config_failover_from_lan9] =
    {
        .info.interface = bridge_interface,
        .info.instance = 9,
        .ifname = "lan9"
    },
    [config_failover_from_lan10] =
    {
        .info.interface = bridge_interface,
        .info.instance = 10,
        .ifname = "lan10"
    },
    [config_failover_from_dsl] =
    {
        .info.interface = dsl_interface,
        .info.instance = 1,
        .ifname = "dsl"
    },
    [config_failover_from_cellular1] =
    {
        .info.interface = cellular_interface,
        .info.instance = 1,
        .ifname = "cellular1"
    },
    [config_failover_from_cellular2] =
    {
        .info.interface = cellular_interface,
        .info.instance = 2,
        .ifname = "cellular2"
    },

    /* add more interfaces here */
};

/* called at init time and for testing purposes */
STATIC_FUNCTION void failover_set_number_of_contexts(size_t const contexts_count)
{
    failover_contexts_count = contexts_count;
}

/* called at init time and for testing purposes */
STATIC_FUNCTION void failover_set_contexts(failover_info_t * const contexts)
{
    failover_contexts = contexts;
}

static void start_probe_timeout_timer(failover_info_t * const failover)
{
    start_timer(failover->timer_probe_id, seconds_to_milliseconds(failover->config.probe_timeout));
    failover->timer_probe_running = true;
}

static void stop_probe_timeout_timer(failover_info_t * const failover)
{
    if (failover->timer_probe_running)
    {
        stop_timer(failover->timer_probe_id);
        failover->timer_probe_running = false;
    }
}

static void start_probe_interval_timer(failover_info_t * const failover)
{
    //failover_debug("start_probe_interval_timer: failover%zu running %s\n", failover->instance, failover->timer_probe_interval_running ? "yes" : "no");

    start_timer(failover->timer_probe_interval_id, seconds_to_milliseconds(failover->config.probe_interval));
    failover->timer_probe_interval_running = true;
}

static void stop_probe_interval_timer(failover_info_t * const failover)
{
    //failover_debug("stop_probe_interval_timer: failover%zu running %s\n", failover->instance, failover->timer_probe_interval_running ? "yes" : "no");
    if (failover->timer_probe_interval_running)
    {
        stop_timer(failover->timer_probe_interval_id);
        failover->timer_probe_interval_running = false;
    }
}

static void start_switch_primary_up_timer(failover_info_t * const failover)
{
    failover_debug("---->start_switch_primary_up_timer: failover%zu running %s\n", failover->instance, timer_is_running(failover->timer_switch_primary_up_id) ? "yes" : "no");
    start_timer(failover->timer_switch_primary_up_id, seconds_to_milliseconds(failover->config.switch_primary_up));
}

static void stop_switch_primary_up_timer(failover_info_t * const failover)
{
    failover_debug("<----stop_switch_primary_up_timer: failover%zu running %s\n", failover->instance, timer_is_running(failover->timer_switch_primary_up_id) ? "yes" : "no");
    stop_timer(failover->timer_switch_primary_up_id);
}

static void show_failover_debug(failover_info_t const * const failover)
{
#if IFMANAGER_DEBUG
 //   size_t i;
 //   for (i=0; i < failover_contexts_count; i++)
    {
//        failover_info_t * failover = &failover_contexts[i];

        ifm_if_config_t * const to = ifm_get_interface_entry(&failover->config.to);
        ifm_if_config_t * const from = ifm_get_interface_entry(&failover->config.from);

        failover_debug("failover%d: %s/"
                       "%s/"
                       "%s/"
                       "%s/"
                       "%s/"
                       "%s/"
                       "%s/"
                       "%s/"
                       "%s/",
                       failover->instance,
                       failover->is_started ? "Active" : "Inactive",
                       failover_state_strings[failover->state],
                       timer_is_running(failover->timer_id) ? "Passive timer" : "",
                       failover->timer_probe_interval_running ? "Probing interval timer" : "",
                       failover->timer_probe_running ? "Probe timer" : "",
                       timer_is_running(failover->timer_switch_primary_up_id) ? "switch-primary-up timer" : "",
                       failover->config.use_type == config_failover_use_passive ? "passive" : "ping",
                       from != NULL ? state_strings[from->current_state] : "None",
                       to != NULL ? state_strings[to->current_state] : "None");
    }
#endif
}

char const * const failover_get_interface_name(ifm_if_info_t const * const info)
{
    char *ifname=NULL;
    int i;

    if (info == NULL)
    {
        goto error;
    }

    for (i=1; i < ARRAY_SIZE(failover_interface_mappings); i++)
    {
        failover_name_mapping const * const pinfo = &failover_interface_mappings[i];

        if ((info->instance == pinfo->info.instance) && (info->interface == pinfo->info.interface))
        {
            ifname = pinfo->ifname;
            goto done;
        }

    }

    if (i == ARRAY_SIZE(failover_interface_mappings))
    {
        failover_debug("cannot find name for [%d] [%d]", info->instance, info->interface);
    }

error:
done:
    return ifname;
}

STATIC_FUNCTION int switch_interface(failover_info_t * const failover, ifm_if_info_t const * const iface)
{
    int result = -1;
    ifm_if_config_t * const ifptr = ifm_get_interface_entry(iface);

    if (ifptr == NULL)
    {
        failover_debug("unable to switch interface %s:%zu\n", iface->name, iface->instance);
        goto done;
    }

    switch (ifptr->config_state)
    {
        case ifm_config_state_on:
        case ifm_config_state_on_demand:
            failover->state = failover_state_switch;
            result = ifm_handle_state_change(ifptr, ifm_config_state_on);
            if (result == 0)
            {
                failover_debug("switch_interface: failover%d %s\n", failover->instance, failover_state_strings[failover->state]);
            }
            failover_warning("failover%zu %s interface from %s%zu to %s%zu\n",
                            failover->instance,
                            (result == 0) ? "switched" : "unable to switch",
                            failover->config.from.name, failover->config.from.instance,
                            iface->name, iface->instance);
            break;

        case ifm_config_state_off:
            failover->state = failover_state_no_switch;
            failover_debug("switch_interface: Unable to failover%zu %s%zu since it's in %s\n", failover->instance,
                            ifptr->info.name, ifptr->info.instance, state_strings[ifptr->config_state]);
            result = 0;
            break;
    }

done:
    return result;
}

static int disconnect_interface(failover_info_t * const failover, ifm_if_info_t const * const iface)
{
    int result = -1;
    ifm_if_config_t * const ifptr = ifm_get_interface_entry(iface);

    if (ifptr == NULL)
    {
        failover_debug("unable to disconnect interface %s:%zu\n", iface->name, iface->instance);
        goto done;
    }

    switch (ifptr->config_state)
    {
        case ifm_config_state_on_demand:
            result = ifm_handle_state_change(ifptr, ifm_config_state_on_demand);
            failover_info("disconnect_interface: failover%zu %s %s%zu\n",
                            failover->instance,
                            (result == 0) ? "disconnecting" : "unable to disconnect",
                            iface->name, iface->instance);
            break;

        case ifm_config_state_off:
        case ifm_config_state_on:
            failover_debug("disconnect_interface: failover%zu not disconnecting %s%zu since it's in %s\n", failover->instance,
                            iface->name, iface->instance, state_strings[ifptr->config_state]);
            result = 0;
            break;
    }

done:
    return result;
}

static int handle_passive_timeout(failover_info_t * const failover)
{
    int result;
    ifm_if_config_t * const ifptr = ifm_get_interface_entry(&failover->config.from);

    if (ifptr == NULL)
    {
        result = -1;
        failover_debug("interface entry not found %s %zu\n", failover->config.from.name, failover->config.from.interface);
        goto done;
    }

    switch (ifptr->current_state)
    {
        case ifm_state_connected:
            result = 0;
            break;
        default:
            /* Linux should already have removed routes from the disconnected interface */
            result = switch_interface(failover, &failover->config.to);
            break;
    }

    failover_stop(failover);

done:
    return result;

}

static int handle_active_timeout(failover_info_t * const failover)
{

    int result = 0;

    ifm_if_config_t * const ifptr = ifm_get_interface_entry(&failover->config.from);

    if (ifptr == NULL)
    {
        result = -1;
        failover_debug("interface entry not found %s %zu\n", failover->config.from.name, failover->config.from.instance);
        goto done;
    }

    ifm_delete_routes(failover_get_interface_name(&failover->config.from));
    failover->current_iface = NULL;

    switch (failover->state)
    {
        case failover_state_idle:
            break;
        case failover_state_start:
        case failover_state_done:
        case failover_state_switch:
        case failover_state_no_switch:
        {
            ifm_if_config_t * const to_ifptr = ifm_get_interface_entry(&failover->config.to);
            if (to_ifptr == NULL)
            {
                result = -1;
                failover_debug("interface entry not found %s %zu\n", failover->config.to.name, failover->config.to.instance);
                goto done;
            }

            switch (to_ifptr->current_state)
            {
                case ifm_state_connected:  /* a connect request has already been sent */
                    failover->state = failover_state_switch;
                    failover_handle_connected_state_to(failover);
                    break;
                case ifm_state_connecting:  /* a connect request has already been sent */
                    break;
                case ifm_state_init:
                case ifm_state_disconnecting:
                case ifm_state_disconnected:
                    result = switch_interface(failover, &failover->config.to);
                    break;
            }
            failover->state = failover_state_done;
        }
            break;
    }

    //failover_timeout_start(failover);

done:
    return result;
}

int failover_timeout_timer_timeout(failover_info_t * const failover)
{
    int result = -1;

    failover_debug("---> TIMEOUT TIMER TIMEOUT: failover%zu %s%zu %d\n", failover->instance,
                    failover->config.from.name, failover->config.from.instance,
                    failover->config.use_type);

    switch (failover->config.use_type)
    {
        case config_failover_use_passive:
            result = handle_passive_timeout(failover);
            break;
        case config_failover_use_ping:
            result = handle_active_timeout(failover);
            break;
    }

    return result;
}

int failover_probe_interval_timeout(failover_info_t * const failover)
{

    int result;

    ifm_if_config_t * const ifptr = ifm_get_interface_entry(&failover->config.from);

    if (ifptr == NULL)
    {
        result = -1;
        failover_debug("interface entry not found %s %zu\n", failover->config.from.name, failover->config.from.interface);
        goto done;
    }
    //failover_debug(">>> PROBE INTERVAL TIMER: failover%zu %s%zu in %s\n", failover->instance,
    //                failover->config.from.name, failover->config.from.instance,
    //                state_strings[ifptr->current_state]);

    failover->timer_probe_interval_running = false;
    failover_probe_start(failover);

    result = 0;

done:
    return result;
}

int failover_probe_timeout(failover_info_t * const failover)
{

    int result = 0;

    //failover_debug(">>> PROBE TIMEOUT: failover%zu probe timeout: %s%zu\n", failover->instance,
    //                failover->config.from.name, failover->config.from.instance);

    failover->timer_probe_running = false;
    /*
     * This time expires when there has been no response to a probe
     * Free up the probe_handle. If a probe success event is received now, it will be ignored.
     */
    if (failover->probe_handle != NULL)
    {
        free_probe(failover->probe_handle);
        failover->probe_handle = NULL;
    }

    if (failover->config.switch_primary_up && timer_is_running(failover->timer_switch_primary_up_id))
    {
        stop_switch_primary_up_timer(failover);
        start_switch_primary_up_timer(failover);
    }

    return result;
}

int failover_switch_primary_up_timeout(failover_info_t * const failover)
{
    int result = -1;

    ifm_if_config_t * const ifptr = ifm_get_interface_entry(&failover->config.from);

    if (ifptr == NULL)
    {
        failover_debug("interface entry not found %s %zu\n", failover->config.from.name, failover->config.from.interface);
        goto done;
    }
    failover_debug(">>> SWITCH-PRIMARY-UP TIMER: failover%zu %s%zu in %s\n", failover->instance,
                    failover->config.from.name, failover->config.from.instance,
                    state_strings[ifptr->current_state]);

    failover_stop(failover);
    failover->state = failover_state_idle;
    failover->current_iface = &failover->config.from;
    ifm_apply_routes(failover_get_interface_name(&failover->config.from));
    disconnect_interface(failover, &failover->config.to);

    failover_warning("failover%zu switched interface from %s%zu to %s%zu\n",
                    failover->instance,
                    failover->config.to.name, failover->config.to.instance,
                    failover->config.from.name, failover->config.from.instance);

    if (failover->config.use_type == config_failover_use_ping)
        failover_start(failover);

    result = 0;

done:
    return result;
}

bool get_failover_interface(size_t interface_id, ifm_if_info_t * const if_info)
{
    bool ret = false;
    static size_t const table_size = ARRAY_SIZE(failover_interface_mappings);

    if (interface_id >= table_size) goto error;

    {
        failover_name_mapping const * const pinfo = &failover_interface_mappings[interface_id];

        if_info->instance = pinfo->info.instance;
        if_info->interface = pinfo->info.interface;
        if_info->name = interface_get_name_from_type(pinfo->info.interface);
    }

    ret = true;

error:
    return ret;
}

bool is_interface_info(ifm_if_info_t const * const src, ifm_if_info_t const * const dest)
{
    bool const match_instance = (src->instance == dest->instance);
    bool const match_interface = (src->interface == dest->interface);

    return (match_instance && match_interface);
}

static void failover_destroy_timer(failover_info_t * const context)
{
    failover_debug("failover_destroy_timer\n");
    if (context->timer_id != NULL)
    {
        destroy_timer(context->timer_id);
        context->timer_id = NULL;
    }

    if (context->timer_probe_id != NULL)
    {
        destroy_timer(context->timer_probe_id);
        context->timer_probe_id = NULL;
        context->timer_probe_running = false;
    }

    if (context->timer_probe_interval_id != NULL)
    {
        destroy_timer(context->timer_probe_interval_id);
        context->timer_probe_interval_id = NULL;
        context->timer_probe_interval_running = false;
    }

    if (context->timer_switch_primary_up_id != NULL)
    {
        destroy_timer(context->timer_switch_primary_up_id);
        context->timer_switch_primary_up_id = NULL;
    }
}

static int failover_init_timer(failover_info_t * const failover)
{
    int result;

    if (failover->timer_id == NULL)
    {
        failover->timer_id = create_timer(failover->timer_context, ifm_timeout_callback, failover);
        if (failover->timer_id == NULL)
        {
            result = -1;
            failover_debug("failed to create timer for failover %d\n", failover->instance);
            goto done;
        }
    }

    if (failover->timer_probe_id == NULL)
    {
        failover->timer_probe_id = create_timer(failover->timer_context, ifm_probe_timeout_callback, failover);
        if (failover->timer_probe_id == NULL)
        {
            result = -1;
            failover_debug("failed to create probe timer for failover %d\n", failover->instance);
            goto done;
        }
    }

    if (failover->timer_probe_interval_id == NULL)
    {
        failover->timer_probe_interval_id = create_timer(failover->timer_context, ifm_probe_interval_timeout_callback, failover);
        if (failover->timer_probe_interval_id == NULL)
        {
            result = -1;
            failover_debug("failed to create probe interval timer for failover %d\n", failover->instance);
            goto done;
        }
    }

    if (failover->timer_switch_primary_up_id == NULL)
    {
        failover->timer_switch_primary_up_id = create_timer(failover->timer_context, ifm_switch_primary_up_timeout_callback, failover);
        if (failover->timer_switch_primary_up_id == NULL)
        {
            result = -1;
            failover_debug("failed to create switch-primary-up timer for failover %d\n", failover->instance);
            goto done;
        }
    }

    result = 0;

done:
    if (result != 0)
        failover_destroy_timer(failover);

    return result;
}

static void failover_timeout_start(failover_info_t * const failover)
{
    failover_debug("<--- TIMEOUT TIMER START: failover%zu running %s\n", failover->instance, timer_is_running(failover->timer_id) ? "yes" : "no");

    if (timer_is_running(failover->timer_id))
    {
        goto done;
    }

    start_timer(failover->timer_id, seconds_to_milliseconds(failover->config.timeout_after));
done:
    return;
}

static void failover_timeout_stop(failover_info_t * const failover)
{
    failover_debug("---> TIMEOUT TIMER STOP: failover%zu running %s\n", failover->instance, timer_is_running(failover->timer_id) ? "yes" : "no");
    stop_timer(failover->timer_id);
}

/*
 *   probe_success_callback
 *   Called by the probe thread when a response has been received.
 *   This callback should add an event onto the failover event queue
 *   because it is called from another thread
 *   The event handler will call failover_probe_success when it processes the message.
*/
static void probe_success_callback(void * arg, uint32_t probe_id)
{
    failover_info_t * const failover = arg;
    probe_success_info_st info;

    /* failover wants to know when the probe response was received */
    (void)gettimeofday(&info.timestamp, NULL);
    info.failover = failover;
    info.probe_id = probe_id;

    (void)ifm_send_event(ifm_event_code_probe_success, &info, sizeof info);

}

/* called by the event handler thread when processing a ifm_event_code_probe_success message */
int failover_probe_success(probe_success_info_st * const success_info)
{
    int result = 0;

    failover_info_t * const failover = success_info->failover;

    failover_debug("PROBE SUCCESS failover%d state %s ID: %d REQ: %d\n",
                   failover->instance,
                   failover_state_strings[failover->state],
                   success_info->probe_id,
                   failover->last_probe_request_id);

    if (failover->probe_handle != NULL)
    {
        if (failover->last_probe_request_id == success_info->probe_id)
        {
            //failover_debug ("PROBE ID matches expected value\n");
            /* save the timestamp of the last received probe response */
            failover->last_successful_probe_timestamp = success_info->timestamp;

            free_probe(failover->probe_handle);
            failover->probe_handle = NULL;

            /* switch back primary interface */
            {
                ifm_if_config_t * const ifptr = ifm_get_interface_entry(&failover->config.from);
                if (ifptr == NULL)
                {
                    result = -1;
                    failover_debug("interface entry not found %s %zu\n", failover->config.from.name, failover->config.from.instance);
                    goto done;
                }

                switch (ifptr->current_state)
                {
                    case ifm_state_connecting:  /* a connect request has already been sent */
                    case ifm_state_connected:  /* a connect request has already been sent */
                        if ((failover->current_iface == NULL) || (failover->current_iface != &failover->config.from))
                        {
                            if (failover->config.switch_primary_up == 0 || (timer_is_running(failover->timer_switch_primary_up_id) == false))
                                failover_handle_connected_state_from(failover);
                        }

                        //result = ifm_send_event(ifm_event_code_connected, &failover->config.from, sizeof failover->config.from);
                        break;
                    case ifm_state_init:
                    case ifm_state_disconnecting:
                    case ifm_state_disconnected:
                        failover->current_iface = NULL;
                        result = switch_interface(failover, &failover->config.from);
                        failover->state = failover_state_done;
                        break;
                }
            }

            /* need to stop the probe timeout timer. Another probe will get sent when the probe interval
             * timer expires next.
             */
            stop_probe_timeout_timer(failover);

            failover_timeout_stop(failover);
            failover_timeout_start(failover);
        }
        /* else must be a response to some probe that we've already timed out */
    }
    /* else there are no outstanding probes, so not expecting a repsonse */

done:
    return result;
}

static uint32_t get_next_probe_id(void)
{
    static uint32_t probe_id;
    uint32_t next_probe_id;

    next_probe_id = probe_id++;

    return next_probe_id;
}

static int failover_probe_action_start(failover_info_t * const failover)
{
    // XXX Do we only ever probe the 'from' interface?
    char const * interface_name;
    uint32_t const probe_id = get_next_probe_id();

    interface_name = failover_get_interface_name(&failover->config.from);

    /* clean up any rference to an old probe */
    if (failover->probe_handle != NULL)
    {
        free_probe(failover->probe_handle);
    }
    failover->probe_handle = send_probe(
                                        failover->config.probe_host,
                                        interface_name,
                                        probe_id,
                                        failover->config.probe_size,
                                        probe_success_callback,
                                        failover,
                                        failover->config.probe_timeout);

    failover->last_probe_request_id = probe_id;

    return (failover->probe_handle != NULL) ? 0 : -1;
}

static void failover_probe_stop(failover_info_t * const failover)
{
    //failover_debug("failover_probe_stop: failover%zu stop\n", failover->instance);

    if (failover->probe_handle != NULL)
    {
        free_probe(failover->probe_handle);
        failover->probe_handle = NULL;
    }
    stop_probe_interval_timer(failover);
    stop_probe_timeout_timer(failover);

}


static void failover_probe_start(failover_info_t * const failover)
{
    //failover_debug("failover_probe_start: failover%zu start\n", failover->instance);

    if (failover_probe_action_start(failover) < 0) goto done;

    start_probe_interval_timer(failover);

    if (!failover->timer_probe_running)
    {
        start_probe_timeout_timer(failover);
    }
done:
    return;
}


void failover_start(failover_info_t * const failover)
{


#if 0
    if (!failover->is_started)
    {
        if (failover_init_timer(failover) != 0)
            goto done;
    }
#endif

    switch (failover->state)
    {
        case failover_state_idle:
        case failover_state_done:
            failover_info("failover%zu starts %s\n", failover->instance, failover_state_strings[failover->state]);

            switch (failover->config.use_type)
            {
                case config_failover_use_passive:
                    failover_probe_stop(failover);
                    failover_timeout_start(failover);
                    failover->is_started = true;
                    failover->state = failover_state_start;
                    break;
                case config_failover_use_ping:
                    if (failover->state == failover_state_idle)
                    {
                        failover_probe_start(failover);
                        failover_timeout_start(failover);
                        failover->is_started = true;
                        failover->state = failover_state_start;
                    }
                    break;
            }
            break;
        case failover_state_start:
        case failover_state_switch:
        case failover_state_no_switch:
            break;
    }
}

static void failover_stop(failover_info_t * const failover)
{
    failover_info("failover%zu stopped %s\n", failover->instance, failover_state_strings[failover->state]);

    switch (failover->state)
    {
        case failover_state_done:
            failover->state = failover_state_idle;
            break;
        case failover_state_idle:
            break;
        case failover_state_start:
        case failover_state_switch:
        case failover_state_no_switch:
            if (failover->is_started)
            {
                failover_timeout_stop(failover);
                failover_probe_stop(failover);
                failover->is_started = false;
                //failover->timer_running = false;
                //failover->timer_probe_running = false;
                //failover->timer_probe_interval_running = false;
                //failover_destroy_timer(failover);
                failover->state = failover_state_done;
            }
            else
                failover->state = failover_state_idle;

            break;
    }
}

static void failover_handle_connected_state_to(failover_info_t * const failover)
{

    failover_debug("CONNECTED_STATE_TO: %s current_iface %p\n", failover_state_strings[failover->state], failover->current_iface);
    switch (failover->config.use_type)
    {
        case config_failover_use_ping:
            /* break; */
        case config_failover_use_passive:
            switch (failover->state)
            {
                case failover_state_idle:
                case failover_state_start:
                case failover_state_no_switch:
                    break;
                case failover_state_done:
                case failover_state_switch:
                    failover->current_iface = &failover->config.to;
                    break;
            }
            break;
    }
}
static void failover_handle_disconnected_state_to(failover_info_t * const failover)
{
    switch (failover->config.use_type)
    {
        case config_failover_use_ping:
            break;
        case config_failover_use_passive:
            break;

    }

}


static void failover_handle_connected_state_from(failover_info_t * const failover)
{
    failover_debug("CONNECTED_STATE_FROM: failover%d %s CONNECTED_STATE current_iface %p\n",
                    failover->instance, failover_state_strings[failover->state],
                    failover->current_iface);

    switch (failover->config.use_type)
    {
        case config_failover_use_ping:
            /* For probing, connected.
             * Let's start failover timer for probing
             * Apply any routes
             */
            switch (failover->state)
            {
                case failover_state_start:
                case failover_state_no_switch:
                    stop_switch_primary_up_timer(failover);
                    failover->current_iface = &failover->config.from;
                    ifm_apply_routes(failover_get_interface_name(&failover->config.from));
                    failover->state = failover_state_idle;
                    failover_start(failover);
                    break;
                case failover_state_done:
                    if (failover->config.switch_primary_up)
                        start_switch_primary_up_timer(failover);
                    else
                    {
                        failover->current_iface = &failover->config.from;
                        ifm_apply_routes(failover_get_interface_name(&failover->config.from));
                        failover->state = failover_state_start;
                        failover_start(failover);
                    }
                    break;
                    /* no break */
                case failover_state_idle:
                    failover->current_iface = &failover->config.from;
                    failover_start(failover);
                    break;
                case failover_state_switch:
                    ifm_delete_routes(failover_get_interface_name(&failover->config.from));
                    break;
            }
            break;
        case config_failover_use_passive:
            /* For passive,
             * Stop failover if failover already started and interface is back on
             */
            switch (failover->state)
            {
                case failover_state_start:
                    failover_stop(failover);
                    failover->state = failover_state_idle;
                    break;
                case failover_state_done:
                    start_switch_primary_up_timer(failover);
                    break;
                case failover_state_idle:
                case failover_state_switch:
                case failover_state_no_switch:
                    break;
            }
            break;
    }
}

static void failover_handle_disconnected_state_from(failover_info_t * const failover)
{
    failover_debug("DISCONNECTED_STATE: %s current_iface %p\n", failover_state_strings[failover->state], failover->current_iface);

    switch (failover->config.use_type)
    {
        case config_failover_use_ping:
            switch (failover->state)
            {
                case failover_state_idle:
                case failover_state_start:
                case failover_state_no_switch:
                    stop_switch_primary_up_timer(failover);
                    failover->current_iface = NULL;
                    break;
                case failover_state_switch:
                case failover_state_done:
                    stop_switch_primary_up_timer(failover);
                    break;
            }
            break;
        case config_failover_use_passive:
            switch (failover->state)
            {
                case failover_state_idle:
                    failover_start(failover);
                    failover->current_iface = NULL;
                    break;
                case failover_state_done:
                    stop_switch_primary_up_timer(failover);
                    break;
                case failover_state_start:
                case failover_state_switch:
                case failover_state_no_switch:
                    break;
            }
            break;

    }

}

static void update_failover(failover_info_t * const failover)
{

    show_failover_debug(failover);

    if (!failover->config.enabled)
    {
        goto error;
    }

    {

        //failover_event_process(failover);

 //       if (failover->config.enabled)
        {
            ifm_if_config_t * to = ifm_get_interface_entry(&failover->config.to);
            ifm_if_config_t * from = ifm_get_interface_entry(&failover->config.from);

            if (to == NULL || from == NULL) goto error;

            switch (failover->config.use_type)
            {
                case config_failover_use_ping:
                    if (STRING_IS_EMPTY(failover->config.probe_host))
                        goto error;
                    break;
                case config_failover_use_passive:
                    break;

            }

            /* check failover from state */
            switch (from->config_state)
            {
                case ifm_config_state_off:
                case ifm_config_state_on_demand:
                    failover_debug("update_failover: failover%d: invalid state for %s %d\n",
                                    failover->instance, failover->config.from.name, failover->config.from.instance);
                    goto error;
                case ifm_config_state_on:
                    break;
            }

            /* check failover to state */
            switch (to->config_state)
            {
                case ifm_config_state_off:
                    failover_debug("update_failover: failover%d: invalid state for %s %d\n",
                                    failover->instance, failover->config.to.name, failover->config.to.instance);
                    goto error;
                case ifm_config_state_on_demand:
                case ifm_config_state_on:
                    break;
            }
#if 1
            failover_debug("update_failover: failover%zu: %s %zu current state %s\n",
                            failover->instance,
                            failover->config.from.name, failover->config.from.instance,
                            state_strings[from->current_state]);
#endif
            show_failover_debug(failover);

            failover->valid = true;


            switch (from->current_state)
            {
                case ifm_state_connected:
                    failover_handle_connected_state_from(failover);
                    break;
                case ifm_state_init:
                    break;
                case ifm_state_connecting:
                case ifm_state_disconnecting:
                case ifm_state_disconnected:
                    failover_handle_disconnected_state_from(failover);
                    break;
            }

            switch (to->current_state)
            {
                case ifm_state_connected:
                    failover_handle_connected_state_to(failover);
                    break;
                case ifm_state_init:
                    break;
                case ifm_state_connecting:
                case ifm_state_disconnecting:
                case ifm_state_disconnected:
                    failover_handle_disconnected_state_to(failover);
                    break;
            }

            goto done;
        }
    }

error:
    failover_stop(failover);
    failover->state = failover_state_idle;
    failover->valid = false;

done:
    return;
}


int failover_handle_config_change(ifm_update_info_t const * const info)
{
    int result;
    failover_info_t * const failover = failover_get_context(info->instance);

    if (failover == NULL)
    {
        result = -1;
        goto done;
    }

    failover_config_status_t const status = failover_get_config(failover->instance, &failover->config);

    switch (status)
    {
        case failover_config_status_changed:
            failover_stop(failover);
            failover->state = failover_state_idle;
            failover->current_iface = NULL;
            break;

        case failover_config_status_error:
            result = -1;
            goto done;

        case failover_config_status_unchanged:
            show_failover_debug(failover);
            break;
    }

    update_failover(failover);

    result = 0;

done:
    return result;
}

void failover_close(void)
{
    free(failover_contexts);
}

int failover_init(timer_context_st * const timer_context)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    size_t i;
    int result;
    size_t contexts_count;
    failover_info_t * contexts;


    contexts_count = table->groups[config_failover].instances;

    if (contexts_count == 0)
    {
        failover_debug("Failover not supported\n");
        result = -1;
        goto done;
    }


    contexts = calloc(contexts_count, sizeof *contexts);
    if (contexts == NULL)
    {
        failover_debug("calloc failed for failover context)\n");
        result = -1;
        goto done;
    }

    failover_set_number_of_contexts(contexts_count);
    failover_set_contexts(contexts);

    for (i = 0; i < contexts_count; i++)
    {
        failover_info_t * const failover = &contexts[i];

        /* initialise the context for this state machine */
        failover->instance = INDEX_TO_INSTANCE(i);
        failover->config.to.interface = invalid_interface;
        failover->config.from.interface = invalid_interface;
        failover->timer_context = timer_context; /* all contexts use the same timer context. */
        failover->timer_probe_running = false;
        failover->timer_probe_running = false;
        failover->timer_id = NULL;
        failover->timer_probe_id = NULL;
        failover->timer_probe_interval_id = NULL;
        failover->timer_switch_primary_up_id = NULL;
        failover->current_iface = NULL;
        failover->state = failover_state_idle;
        if (failover_init_timer(failover) != 0)
        {
            result = -1;
            goto done;
        }
    }

    result = 0;

done:
    if (result < 0)
        failover_close();

    return result;
}

void failover_handler(ifm_if_info_t const * const iface)
{
    size_t i;

    for (i=0; i < failover_contexts_count; i++)
    {
        failover_info_t * const failover = &failover_contexts[i];

        if ((failover->config.from.name != NULL && is_interface_info(&failover->config.from, iface))
            || (failover->config.to.name != NULL && is_interface_info(&failover->config.to, iface)))
            update_failover(failover);
    }

    return;
}

bool failover_inhibit_routes(ifm_if_info_t const * const iface)
{
    size_t i;
    bool result = false;

    for (i = 0; i < failover_contexts_count; i++)
    {
        failover_info_t * const failover = &failover_contexts[i];

        if (failover->config.from.name != NULL && is_interface_info(&failover->config.from, iface) && timer_is_running(failover->timer_switch_primary_up_id))
        {
            result = true;
            break;
        }
    }

    return result;
}

failover_info_t * const failover_get_context(size_t const instance)
{
    failover_info_t * failover;
    size_t const index = INSTANCE_TO_INDEX(instance);

    if (!IS_VALID_INDEX(index))
    {
        failover = NULL;
        goto done;
    }
    failover = &failover_contexts[index];

done:
    return failover;
}

size_t const failover_get_number_of_contexts(void)
{
    return failover_contexts_count;
}


