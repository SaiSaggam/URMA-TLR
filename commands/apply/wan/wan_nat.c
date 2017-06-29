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
#include "wan_nat.h"
#include <stdlib.h>
#include <string.h>
#include "config_mgr.h"
#include "tlr_print.h"
#include "iptables.h"

static int get_nat_enabled(size_t const instance, bool * const is_nat_enabled)
{
    int result = -1;
    unsigned int on_off_value = 0;

    config_status_t const status = tlr_get_element_value(config_wan, instance, config_wan_nat,
                                                         &on_off_value, sizeof on_off_value,
                                                         config_mgr_element_type_on_off, config_mgr_access_level_super);

    if (status != config_status_success)
    {
        tlr_error("Could not read wan nat value for instance [%zu]\n", instance);
        goto error;
    }

    *is_nat_enabled = (on_off_value == 1);
    result = 0;

error:
    return result;
}

static bool is_wan_interface_configured(char const * const interface_name)
{
    bool wan_interface_is_configured;
    config_wan_interface_t wan_interface;
    config_status_t status;

    status = tlr_config_string_to_value(config_wan, config_wan_interface, interface_name, &wan_interface);
    if (status != config_status_success)
    {
        tlr_error("Error mapping interface name (%s) to value\n", interface_name);
        wan_interface_is_configured = false;
        goto done;
    }
    wan_interface_is_configured = (wan_interface != config_wan_interface_none);

done:
    return wan_interface_is_configured;
}

static int configured_wan_interface_name_alloc(size_t const instance, char * * const interface_name)
{
    int result = -1;
    config_status_t const read_status = tlr_get_element_alloc_value(config_wan, instance,
                                                              config_wan_interface,
                                                              (void **)interface_name,
                                                              config_mgr_element_type_string,
                                                              config_mgr_access_level_super);

    if (read_status != config_status_success)
    {
        tlr_error("Could not read wan interface for instance [%zu]\n", instance);
        goto error;
    }

    result = 0;

error:
    return result;
}

static void configured_wan_interface_name_free(char * const interface_name)
{
    free(interface_name);
}

static void enable_disable_nat(char const * const interface_name, bool const nat_enabled)
{
    if (nat_enabled)
    {
        iptables_nat_on(interface_name);
    }
    else
    {
        iptables_nat_off(interface_name);
    }
}

int apply_wan_nat_settings(size_t const instance)
{
    int result = -1;
    bool nat_enabled;
    char * interface_name = NULL;

    if (get_nat_enabled(instance, &nat_enabled) < 0)
    {
        goto done;
    }

    if (configured_wan_interface_name_alloc(instance, &interface_name) < 0)
    {
        goto done;
    }

    if (!is_wan_interface_configured(interface_name))
    {
        /* Nothing to do as there is no interface configured. */
        result = 0;
        goto done;
    }

    enable_disable_nat(interface_name, nat_enabled);

    result = 0;

done:
    configured_wan_interface_name_free(interface_name);

    return result;
}