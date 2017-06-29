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
#include "config_table.h"
#include "config_def.h"
#include "config_status.h"
#include "config_mgr.h"
#include "get_config_value.h"
#include "snmp_agent_debug.h"

static config_mgr_element_type_t get_parameter_type_to_retrieve(config_mgr_element_type_t const type)
{
    config_mgr_element_type_t real_type;

    /*
     * For enum multi types, the MIB is defined to have these as strings. For all other types we
     * return the type defined in commands.conf.
     * XXX CN - How do we ensure that the asn_type matches the type that we get from the
     * configuration?
    */
    if (type == config_mgr_element_type_enum_multi)
    {
        real_type = config_mgr_element_type_string;
    }
    else
    {
        real_type = type;
    }

	return real_type;
}

int get_config_group_element_value(config_mgr_group_t const * const group,
                                    size_t instance,
                                    config_mgr_element_t const * const element,
                                    u_char const asn_type,
                                    void * * const buffer,
                                    size_t * const element_size)
{
    config_mgr_element_type_t type;
    config_status_t status;
    int ret = SNMP_ERR_NOSUCHNAME;

    type = get_parameter_type_to_retrieve(element->type);

    status = tlr_get_element_alloc_value(group->id, instance, element->id,
                                         buffer, type, element->access);
    switch (status)
    {
        case config_status_success:
            if (snmp_adjust_element(asn_type, element_size, buffer) < 0)
            {
                goto error;
            }
            break;

        case config_status_group_not_found:
        case config_status_element_not_found:
        case config_status_instance_not_found:
            snmp_debug("tlr_get_element_value() failed [%d]\n", status);
            goto error;

        default:
            snmp_debug("tlr_get_element_alloc_value failed status = [%d]\n", status);
            ret = SNMP_ERR_GENERR;
            goto error;
    }

    ret = SNMP_ERR_NOERROR;
error:
    return ret;
}




