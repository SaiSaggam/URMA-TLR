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

#include "bridge_monitor_config.h"
#include "bridge_monitor_config_commands.h"
#include "bridge_interfaces.h"
#include "bridge_monitor_log.h"
#include "bridge_monitor.h"
#include "bridge_interfaces_update.h"
#include "bridge_dns.h"
#include "messages.h"
#include "str_replace_char.h"

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>


static bool element_ip_address_or_mask_changed(size_t const instance, char const * const ip_address_string, char const * const mask_string)
{
    bool ip_address_and_mask_set;

    // Setting the ip address has the side effect of bringing the interface up,
    // so only do this if the interface is already connected.
    if (!bridge_is_connected(instance))
    {
        ip_address_and_mask_set = false;
        goto done;
    }

    ip_address_and_mask_set = bridge_interface_set_ip_address_and_mask(instance, ip_address_string, mask_string);

done:
    return ip_address_and_mask_set;
}

static bool element_dhcp_client_changed(size_t const instance, char const * const dhcp_string, bool * const dhcp_client_has_been_shut_down)
{
    bool new_dhcp_client_configuration_has_been_set = true;
    char const dhcp_off_keyword[] = "off";

    if (bridge_is_connected(instance))
    {
        /*
         * If the bridge is already running and someone changes the DHCP configuration
         * then start or stop the client here.
         */
        new_dhcp_client_configuration_has_been_set = bridge_interface_set_dhcp_client_to_desired_state(instance, dhcp_string);
        *dhcp_client_has_been_shut_down = strcmp(dhcp_string, dhcp_off_keyword) == 0;
    }
    else
    {
        /*
         * If the bridge is not running, then do nothing and say we handled it.
         * The DHCP client will be started if necessary when the interface is
         * brought up.
         */
        new_dhcp_client_configuration_has_been_set = true;
    }

    return new_dhcp_client_configuration_has_been_set;
}


static bool element_mtu_changed(size_t const instance, char const * const mtu_string)
{
    return bridge_interface_set_mtu(instance, mtu_string);
}

static bool element_dns_changed(size_t const instance, char const * const dns1, char const * const dns2)
{
    bool dns_servers_set;

    // Setting the ip address has the side effect of bringing the interface up,
    // so only do this if the interface is already connected.
    if (!bridge_is_connected(instance))
    {
        dns_servers_set = false;
        goto done;
    }

    dns_servers_set = bridge_set_dns_servers(instance, dns1, dns2);

done:
    return dns_servers_set;
}

bool bridge_config_check_for_change(size_t const instance, config_st * const config, config_st * const new_config)
{
    bool config_changed = false;
    size_t index;
    size_t const old_config_count = config_get_element_count(config);
    size_t const new_config_count = config_get_element_count(new_config);

    assert(old_config_count == new_config_count);

    /*
     * Done this way, we'll get a compile time error whenever a new parameter is added, which is
     * much better than this function getting out of date or forgotten as soon as a new parameter is
     * added.
     */
    bridgemon_debug("bridge_config_check_for_change called for lan%u\n", instance);
    for (index = 0; index < new_config_count; index++)
    {
        char const * const old_value = config_get_element_value(config, index);
        char const * const new_value = config_get_element_value(new_config, index);
        bool const element_changed = strcmp(old_value, new_value) != 0;
        bool element_change_handled = false;

        if (!element_changed)
        {
            continue;
        }

        bridgemon_debug("element id %zd has changed (%s->%s)\n", index, old_value, new_value);

        switch ((config_lan_t)index)
        {
            /* insignificant parameters */
            case config_lan_state:
            case config_lan_description:
                element_change_handled = true;
                break;

            /* significant parameters */
            case config_lan_dns1:
            case config_lan_dns2:
            {
                char const * const new_dns1 = config_get_element_value(new_config, config_lan_dns1);
                char const * const new_dns2 = config_get_element_value(new_config, config_lan_dns2);
                element_change_handled = element_dns_changed(instance, new_dns1, new_dns2);
                break;
            }
            case config_lan_dhcp_client:
            {
                bool dhcp_client_has_been_shut_down = false;

                element_change_handled = element_dhcp_client_changed(instance, new_value, &dhcp_client_has_been_shut_down);
                if (dhcp_client_has_been_shut_down)
                {
                    char const * const mask_string = config_get_element_value(new_config, config_lan_mask);
                    char const * const ip_address_string = config_get_element_value(new_config, config_lan_ip_address);
                    (void) element_ip_address_or_mask_changed(instance, ip_address_string, mask_string);
                }
                break;
            }
            case config_lan_mtu:
                element_change_handled = element_mtu_changed(instance, new_value);
                break;

            case config_lan_interfaces:
                element_change_handled = bridge_interfaces_changed(instance, old_value, new_value);
                break;

            case config_lan_ip_address:
            {
                char const * const mask_string = config_get_element_value(new_config, config_lan_mask);
                element_change_handled = element_ip_address_or_mask_changed(instance, new_value, mask_string);
                break;
            }

            case config_lan_mask:
            {
                char const * const ip_address_string = config_get_element_value(new_config, config_lan_ip_address);
                element_change_handled = element_ip_address_or_mask_changed(instance, ip_address_string, new_value);
                break;
            }
        }

        if (element_change_handled)
        {
            config_changed = true;
        }
        else
        {
            config_set_element_value(new_config, index, old_value);
        }
    }

    return config_changed;
}


