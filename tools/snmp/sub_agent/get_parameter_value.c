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
#include "get_parameter_value.h"
#include "get_config_value.h"
#include "get_show_value.h"
#include "config_table.h"
#include "config_def.h"
#include "config_status.h"
#include "config_mgr.h"
#include "snmp_utils.h"
#include "snmp_agent_debug.h"
#include "index_element.h"

static int get_config_element(u_char const asn_type, parameter_t * const parameter,
                              uint8_t * * const buffer, size_t * const element_size)
{
    int retval = SNMP_ERR_NOSUCHNAME;

    switch (parameter->group_type)
    {
        case config_mgr_set_group:
            retval = get_config_group_element_value(parameter->group, parameter->instance, parameter->element, asn_type,
                                                    (void **)buffer, element_size);
            if (retval != SNMP_ERR_NOERROR)
            {
                goto error;
            }
            break;
        case config_mgr_get_group:
            retval = get_show_group_element_value(parameter->group, parameter->instance, parameter->element, asn_type,
                                                  (void **)buffer, element_size);
            if (retval != SNMP_ERR_NOERROR)
            {
                goto error;
            }
            break;
        default:
            retval = SNMP_ERR_NOSUCHNAME;
            snmp_debug("SNMP_ERR_NOSUCHNAME");
            goto error;
    }

    retval = SNMP_ERR_NOERROR;
error:
    return retval;
}

int snmp_get_parameter_value(parameter_t * const parameter, netsnmp_variable_list * const var)
{
    int       retval = SNMP_ERR_NOSUCHNAME;
    u_char    asn_type;
    size_t  element_size;
    uint8_t  * buffer = NULL;

    snmp_print_parameter_debug_info(parameter);

    if ((element_size = tlr_get_element_size(parameter->element)) == 0)
    {
        retval = SNMP_ERR_BADVALUE;
        snmp_debug("Invalid element size: SNMP_ERR_BADVALUE\n");
        goto error;
    }

    if ((asn_type = snmp_convert_tlr_type_to_asn_type(parameter->element->type)) == (u_char)MAX_SUBID)
    {
        retval = SNMP_ERR_BADVALUE;
        snmp_debug("snmp_convert_tlr_type_to_asn_type failed SNMP_ERR_BADVALUE\n");
        goto error;
    }

	if (snmp_is_index_element(parameter->element->name, asn_type))
	{
		buffer = snmp_get_index_element(parameter->instance);
		if (buffer == NULL)
		{
			goto error;
		}
		element_size = sizeof(int);
	}
	else
	{
		retval = get_config_element(asn_type, parameter, &buffer, &element_size);
		if (retval != SNMP_ERR_NOERROR)
		{
			goto error;
		}
	}

    if (snmp_set_var_typed_value(var, asn_type, buffer, element_size) != 0)
    {
        retval = SNMP_ERR_GENERR;
        snmp_debug("SNMP_ERR_GENERR: snmp_set_var_typed_value() failed\n");
        goto error;
    }

    retval = SNMP_ERR_NOERROR;
error:
    free(buffer);

    return retval;
}


