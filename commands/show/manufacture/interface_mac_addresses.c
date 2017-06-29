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
#include "interface_mac_addresses.h"
#include "interface_mac_addresses_private.h"
#include "show_manufacture_mem.h"
#include "mac_address.h"
#include "tlr_print.h"
#include "interfaces.h"
#include "tlr_common.h"

#include <stddef.h>
#include <stdlib.h>

static size_t get_num_interfaces(config_group_t const config_group)
{
    size_t count;
    config_mgr_group_t const * const group = tlr_get_group_ptr(config_group, 0, config_mgr_set_group);

    if (group == NULL)
    {
        count = 0;
        goto done;
    }

    count = group->instances;

done:
    return count;
}

static interface_mac_address_st * alloc_interface_mac_addresses(char const * const interface_name,
                                                                size_t const num_interfaces)
{
    interface_mac_address_st * interface_mac_addresses;

    interface_mac_addresses = show_manufacture_calloc(1, sizeof *interface_mac_addresses);
    if (interface_mac_addresses == NULL)
    {
        goto done;
    }
    interface_mac_addresses->base_interface_name = interface_name;
    interface_mac_addresses->num_interfaces = num_interfaces;
    interface_mac_addresses->mac_addresses = show_manufacture_calloc(num_interfaces, sizeof *interface_mac_addresses->mac_addresses);
    if (interface_mac_addresses->mac_addresses == NULL)
    {
        interface_mac_addresses_free(interface_mac_addresses);
        interface_mac_addresses = NULL;
        goto done;
    }

done:
    return interface_mac_addresses;
}

static void read_mac_addresses(interface_mac_address_st * const interface_mac_addresses)
{
    size_t index;

    for (index = 0; index < interface_mac_addresses->num_interfaces; index++)
    {
        size_t const instance = INDEX_TO_INSTANCE(index);

        interface_mac_addresses->mac_addresses[index] = get_mac_address(interface_mac_addresses->base_interface_name, instance);
        if (interface_mac_addresses->mac_addresses[index] == NULL)
        {
            tlr_debug("failed to get %s %zd MAC address\n", interface_mac_addresses->base_interface_name, instance);
        }
    }
}

static void free_mac_addresses(char * * const mac_addresses, size_t const num_mac_addresses)
{
    size_t index;

    if (mac_addresses != NULL)
    {
        for (index = 0; index < num_mac_addresses; index++)
        {
            show_manufacture_free(mac_addresses[index]);
        }
        show_manufacture_free(mac_addresses);
    }
}

static void write_mac_address(FILE * const fp,
                              size_t const instance,
                              char const * const label,
                              char const * const interface_label,
                              char const * const sub_label,
                              char const * const mac_address)
{
    fprintf(fp, " %s %s%zd", label, interface_label, instance);
    if (sub_label != NULL)
    {
        fprintf(fp, " %s", sub_label);
    }
    fprintf(fp, " MAC=%s\n", mac_address);
}

interface_mac_address_st * interface_mac_addresses_alloc(config_group_t const config_group)
{
    interface_mac_address_st * interface_mac_addresses;
    char const * interface_name;
    size_t num_interfaces;

    /* get_num_interfaces() will return 0 if this interface type is not supported on this
     * hardware.
     */
    num_interfaces = get_num_interfaces(config_group);
    if (num_interfaces == 0)
    {
        /* Don't log an error as it's entirely legitimate for num_interfaces to == 0 on some
         * hardware.
         */
        interface_mac_addresses = NULL;
        goto done;
    }

    interface_name = interface_get_name_from_config_group(config_group);
    if (interface_name == NULL)
    {
        tlr_debug("Unable to get interface name for config_group %d\n", config_group);
        interface_mac_addresses = NULL;
        goto done;
    }

    interface_mac_addresses = alloc_interface_mac_addresses(interface_name, num_interfaces);
    if (interface_mac_addresses == NULL)
    {
        tlr_debug("Unable to obtain storage for MAC addresses\n");
        interface_mac_addresses = NULL;
        goto done;
    }

    read_mac_addresses(interface_mac_addresses);

done:

    return interface_mac_addresses;
}

void interface_mac_addresses_free(interface_mac_address_st * const interface_mac_addresses)
{
    if (interface_mac_addresses != NULL)
    {
        free_mac_addresses(interface_mac_addresses->mac_addresses, interface_mac_addresses->num_interfaces);
        show_manufacture_free(interface_mac_addresses);
    }
}

void write_interface_mac_addresses(FILE * const fp,
                                   interface_mac_address_st const * const mac_addresses,
                                   char const * * const label_to_print,
                                   char const * const empty_label,
                                   char const * const interface_label,
                                   char const * const sub_label)
{
    char const * label = *label_to_print;

    if (mac_addresses != NULL)
    {
        unsigned int index;

        for (index = 0; index < mac_addresses->num_interfaces; index++, label = empty_label)
        {
            write_mac_address(fp,
                              INDEX_TO_INSTANCE(index),
                              label,
                              interface_label,
                              sub_label,
                              mac_addresses->mac_addresses[index]);
        }
    }

    /* Update the label to print. */
    *label_to_print = label;
}

char const * get_mac_address_from_mac_addresses(interface_mac_address_st const * const mac_addresses, unsigned int const instance)
{
    char const * mac_address;
    unsigned int const index = INSTANCE_TO_INDEX(instance);

    if (mac_addresses == NULL)
    {
        mac_address = NULL;
        goto done;
    }
    if (index >= mac_addresses->num_interfaces)
    {
        mac_address = NULL;
        goto done;
    }
    mac_address = mac_addresses->mac_addresses[index];

done:
    return mac_address;
}


