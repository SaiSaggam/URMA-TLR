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

#include "net-snmp-config.h"
#include "request_handlers.h"
#include "snmp_agent_debug.h"
#include "oid_to_parameter.h"
#include "get_parameter_value.h"
#include "set_parameter_value.h"
#include "snmp_utils.h"
#include "config_mgr.h"
#include "tlr_common.h"
#include "apply_config.h"

int handle_get_request(netsnmp_variable_list *var)
{
	int retval = SNMP_ERR_GENERR;
	parameter_t  parameter;

	snmp_print_get_request(var);

	if (oid_to_parameter(var->name, var->name_length, &parameter) < 0)
	{
		retval = SNMP_ERR_NOSUCHNAME;
		goto error;
	}

	if ((retval = snmp_get_parameter_value(&parameter, var)) != SNMP_ERR_NOERROR)
	{
		goto error;
	}

	retval = SNMP_ERR_NOERROR;
error:
	return retval;
}

int handle_get_next_request(netsnmp_variable_list *var)
{
	int retval = SNMP_ERR_GENERR;
	parameter_t  parameter;
	int resp;

	snmp_print_get_next_request(var);

	if ((resp = oid_to_next_parameter(var->name, var->name_length, &parameter)) != 0)
	{
		if (resp == -1)
		{
			retval = SNMP_ERR_NOSUCHNAME;
		}
		else
		{
			retval = SNMP_ERR_NOERROR;
		}
		goto error;
	}

	if (snmp_set_var_objid(var, parameter.oid, parameter.oid_length) != 0)
	{
		goto error;
	}

	if ((retval = snmp_get_parameter_value(&parameter, var)) != SNMP_ERR_NOERROR)
	{
		snmp_debug("get_parameter_value failed\n");
		goto error;
	}

	retval = SNMP_ERR_NOERROR;
error:
	return retval;
}

int handle_set_request_begin(netsnmp_variable_list const * const var)
{
    int retval = SNMP_ERR_GENERR;
    parameter_t  parameter;

    snmp_print_set_request(var);

    if (oid_to_parameter(var->name, var->name_length, &parameter) < 0)
    {
        snmp_debug("oid_to_parameter failed\n");
        retval = SNMP_ERR_NOSUCHNAME;
        goto error;
    }
    if (snmp_set_parameter_value(&parameter, var) != SNMP_ERR_NOERROR)
    {
        snmp_debug("snmp_set_parameter_value failed\n");
        goto error;
    }
    retval = SNMP_ERR_NOERROR;
error:
    return retval;
}

int handle_set_request_commit(netsnmp_variable_list const * const var)
{
    int retval = SNMP_ERR_GENERR;
    parameter_t  parameter;

    snmp_print_set_request(var);

    if (oid_to_parameter(var->name, var->name_length, &parameter) < 0)
    {
        snmp_debug("oid_to_parameter failed\n");
        retval = SNMP_ERR_NOSUCHNAME;
        goto error;
    }

    if (snmp_apply_config(parameter.group->id, parameter.instance) < 0)
    {
        snmp_debug("snmp_apply_config failed\n");
        goto error;
    }

    retval = SNMP_ERR_NOERROR;
error:
    return retval;
}



