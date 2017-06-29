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

#include "snmp_agent_debug.h"
#include <stdio.h>

#define SNMP_DEBUG_BUFFER_SIZE 256

void snmp_print_get_request(netsnmp_variable_list const * const var)
{
    char buffer[SNMP_DEBUG_BUFFER_SIZE];
    int result = snprint_variable(buffer, sizeof buffer, var->name, var->name_length, var);

    if (result != -1)
    {
        snmp_debug("GET REQUEST: ");
        snmp_debug(buffer);
    }
}

void snmp_print_get_next_request(netsnmp_variable_list const * const var)
{
    char buffer[SNMP_DEBUG_BUFFER_SIZE];
    int result = snprint_variable(buffer, sizeof buffer, var->name, var->name_length, var);

    if (result != -1)
    {
        snmp_debug("GET NEXT REQUEST: ");
        snmp_debug(buffer);
    }
}

void snmp_print_set_request(netsnmp_variable_list const * const var)
{
    char buffer[SNMP_DEBUG_BUFFER_SIZE];
    int result = snprint_variable(buffer, sizeof buffer, var->name, var->name_length, var);

    if (result != -1)
    {
        snmp_debug("SET REQUEST: ");
        snmp_debug(buffer);
    }
}

void snmp_print_parameter_debug_info(parameter_t const * const parameter)
{
    snmp_debug("Group Name: [%s] Element Name [%s] Instance %d %s\n", parameter->group->name,
               parameter->element->name, parameter->instance,
               parameter->group_type == config_mgr_set_group ? "Config" : "Show");
}

