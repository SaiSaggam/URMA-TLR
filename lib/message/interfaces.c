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

#include "interfaces.h"
#include <string.h>
#include "tlr_common.h"

struct interface_mappings_st
{
    config_group_t config_group;
    char const * const name;
};

typedef struct interface_mappings_st interface_mappings_st;

static const interface_mappings_st interface_mappings[] =
{
    [cellular_interface] =
    {
        .config_group = config_cellular,
        .name = "cellular"
    },
    [dsl_interface] =
    {
        .config_group = config_dsl,
        .name = "dsl"
    },
    [eth_interface] =
    {
        .config_group = config_eth,
        .name = "eth"
    },
    [bridge_interface] =
    {
        .config_group = config_lan,
        .name = "lan"
    },
    [wifi_interface] =
    {
        .config_group = config_wifi,
        .name = "wifi"
    },
    [wifi5g_interface] =
    {
        .config_group = config_wifi5g,
        .name = "wifi5g"
    },
    [vpn_interface] =
    {
       .config_group = config_ipsec,
       .name = "ipsec"	// needs to match what is specified in the apply_ipsec script
    },
};


// TODO: if we ever get more than a few interfaces it might be worth using a different lookup method.

interfaces_t interface_get_type_from_name(char const * const name)
{
    int i;
    interfaces_t interface_type = invalid_interface;

    for (i = 0; i < ARRAY_SIZE(interface_mappings); i++)
    {
        if (interface_mappings[i].name != NULL && strcmp(name, interface_mappings[i].name) == 0)
        {
            interface_type = i;
            break;
        }
    }

    return interface_type;
}

char const * interface_get_name_from_type(interfaces_t const type)
{
    char const * interface_name = NULL;

    if (type < ARRAY_SIZE(interface_mappings))
    {
        interface_name = interface_mappings[type].name;
    }

    return interface_name;
}

interfaces_t interface_get_type_from_config_group(config_group_t const config_group)
{
    int i;
    int interface_type = invalid_interface;

    for (i = 0; i < ARRAY_SIZE(interface_mappings); i++)
    {
        interface_mappings_st const * const interface_mapping = &interface_mappings[i];

        if (interface_mapping->name != NULL && interface_mapping->config_group == config_group)
        {
            interface_type = i;
            break;
        }
    }

    return interface_type;
}

char const * interface_get_name_from_config_group(config_group_t const config_group)
{
    int i;
    char const * interface_name = NULL;

    for (i = 0; i < ARRAY_SIZE(interface_mappings); i++)
    {
        interface_mappings_st const * const interface_mapping = &interface_mappings[i];

        if (interface_mapping->name != NULL && interface_mapping->config_group == config_group)
        {
            interface_name = interface_mapping->name;
            break;
        }
    }

    return interface_name;
}
