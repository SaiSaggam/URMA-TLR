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
#include <event_manager.h>
#include <messages.h>
#include <stdlib.h>
#include <string.h>
#include "config_status.h"
#include "config_def.h"
#include "config_mgr.h"
#include "digiutils.h"
#include "unittest_support.h"
#include "dns_handle.h"
#include "route.h"

// Add any default routes for this interface
STATIC_FUNCTION int add_default_routes(char const * const ifname)
{
    int ret = -1;
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);

    if (ifname == NULL)
    {
        goto error;
    }

    if (config_defroute < table_ptr->count)
    {
        config_mgr_group_t * const group_ptr = &table_ptr->groups[config_defroute];
        size_t i;

        for (i=1; i <= group_ptr->instances; i++)
        {
            char interface[64];

            config_status_t read_status = tlr_get_element_value(config_defroute, i,
                                          config_defroute_interface,
                                          (char * const)interface, sizeof interface,
                                          config_mgr_element_type_string,
                                          config_mgr_access_level_read_only);

            if (read_status == config_status_success)
            {
                // See if the route can be used on this interface
                if (does_interface_match_default_route(ifname, i) &&
                   (is_default_route_valid(i)))
                {
                    // Try to apply this route.
                    apply_default_route(i);
                }
            }
            else
            {
                tlr_debug("Adding default route could not read interface field %d\n", read_status);
                goto error;
            }
        }
    }

    ret = 0;
error:
    return ret;
}

// Determine which static routes to apply for this interface
STATIC_FUNCTION int add_static_routes(char const * const ifname)
{
    int ret = -1;
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);

    if (ifname == NULL)
    {
        goto error;
    }

    if (config_route < table_ptr->count)
    {
        config_mgr_group_t * const group_ptr = &table_ptr->groups[config_route];
        size_t i;

        for (i=1; i <= group_ptr->instances; i++)
        {
            char interface[64];

            config_status_t read_status = tlr_get_element_value(config_route, i,
                                          config_route_interface,
                                          interface, sizeof interface,
                                          config_mgr_element_type_string,
                                          config_mgr_access_level_read_only);

            if (read_status == config_status_success)
            {
                if (does_interface_match_static_route(ifname, i) &&
                    is_static_route_valid(i))
                {
                    apply_static_route(i);
                }
            }
            else
            {
                tlr_debug("Adding static route could not read interface field %d\n", read_status);
                goto error;
            }
        }
    }

    ret = 0;
error:
    return ret;
}

// An interface just came up, apply any routes
int ifm_apply_routes(char const * const ifname)
{
    int ret = -1;

    if (ifname == NULL)
    {
        tlr_debug("ifm_apply_routes name is null");
        goto error;
    }

    if (add_default_routes(ifname) < 0)
    {
        tlr_debug("add_default_routes: could not add static route for %s", ifname);
    }

    if (add_static_routes(ifname) < 0)
    {
         goto error;
    }

    ret = 0;
error:
    return ret;
}

// A configuration change has been made to a route
int ifm_update_route(ifm_update_info_t const * const info)
{
    int ret = -1;

    tlr_debug("ifm_update_route group %d instance %zu",info->config_group, info->instance);

    if (info->config_group == config_defroute)
    {
        tlr_debug("ifm_update_route: configuring default route for %zu\n", info->instance);
        if (update_default_route(info->instance) < 0)
        {
            tlr_debug("ifm_update_route: configuring default route failed for %zu\n", info->instance);
            goto error;
        }
    }
    else if (info->config_group == config_route)
    {
        tlr_debug("ifm_update_route: configuring static route for %zu\n", info->instance);
        if (update_static_route(info->instance) < 0)
        {
            tlr_debug("ifm_update_route: configuring static route failed for %zu\n", info->instance);
            goto error;
        }
    }
    else
    {
        tlr_debug("ifm_update_route: unsupported group id %d\n", info->config_group);
        goto error;
    }

    ret = 0;
error:
    return ret;
}


