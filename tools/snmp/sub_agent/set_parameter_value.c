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

#include "set_parameter_value.h"
#include "net-snmp-config.h"
#include "snmp_agent_debug.h"
#include "snmp_utils.h"
#include "config_mgr.h"
#include "tlr_common.h"
#include "digiutils.h"
#include <arpa/inet.h>

static int find_tlr_enum_index_from_snmp_value(long const value, parameter_t const * const parameter)
{
    int index = -1;
    config_mgr_element_enum_limit_t *limit_ptr = parameter->element->value_limit;
    const config_mgr_element_enum_t *enum_value = limit_ptr->value;

    for (index = 0; index < limit_ptr->count; index++)
    {
        if (value == enum_value[index].value)
        {
            break;
        }
    }
    if (index == limit_ptr->count)
    {
        snmp_debug("Invalid enum value %ld\n", value);
        index = -1;
    }

    return index;
}

static int set_enum_element_value(parameter_t const * const parameter, netsnmp_variable_list const * const var)
{
    int retval = -1;
    config_mgr_element_enum_limit_t *limit_ptr = parameter->element->value_limit;
    const config_mgr_element_enum_t *enum_value = limit_ptr->value;
    config_status_t status;
    long value = *var->val.integer;
    int index;

    if ((index = find_tlr_enum_index_from_snmp_value(value, parameter)) < 0)
    {
        goto error;
    }

    char const * const enum_string_name = enum_value[index].name;

    status = tlr_set_element_value(parameter->group->id,
                                   parameter->instance, parameter->element->id,
                                   enum_string_name, config_mgr_access_level_super);

    if (status != config_status_success)
    {
        snmp_debug("setting enum tlr_set_element_value failed [%d]", status);
        goto error;
    }

    retval = 0;
error:
    return retval;
}

static int set_native_element_value(parameter_t const * const parameter, netsnmp_variable_list const * const var)
{
    int retval = -1;
    void *ptr_value = var->val.string;
    tlr_handle_t tlr_handle;

    tlr_handle = tlr_config_open(parameter->group->id, parameter->instance, config_mgr_access_level_super);
    if (tlr_handle == NULL)
    {
        snmp_debug("tlr_config_open failed\n");
        goto error;
    }

    if (tlr_config_write_native(tlr_handle, parameter->element->id, ptr_value) != config_status_success)
    {
        snmp_debug("tlr_config_write_native failed\n");
        goto error;
    }

    retval = 0;

error:
    if (tlr_handle != NULL)
    {
        tlr_config_close(tlr_handle);
    }
    return retval;
}

static int set_ip_address_value(parameter_t const * const parameter, netsnmp_variable_list const * const var)
{
    int retval = -1;
    config_status_t status;
    char *binary_ip_address_string = (char *)var->val.string;
    char ip_address_string_buffer[INET6_ADDRSTRLEN];

    if (convert_ip_address_to_string_from_binary(binary_ip_address_string, ip_address_string_buffer,
        sizeof ip_address_string_buffer) < 0)
    {
        goto error;
    }

    status = tlr_set_element_value(parameter->group->id,
                                   parameter->instance, parameter->element->id,
                                   ip_address_string_buffer,
                                   config_mgr_access_level_super);

    if (status != config_status_success)
    {
        goto error;
    }
    retval = 0;
error:
    return retval;
}

int snmp_set_parameter_value(parameter_t const * const parameter, netsnmp_variable_list const * const var)
{
    int retval = SNMP_ERR_GENERR;
    int set_element_return_value = -1;
    config_mgr_element_type_t type = parameter->element->type;

    snmp_print_parameter_debug_info(parameter);

    switch (type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_enum_multi:
        case config_mgr_element_type_uint32:
        case config_mgr_element_type_int32:
        case config_mgr_element_type_counter32:
        case config_mgr_element_type_counter64:
        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
            set_element_return_value = set_native_element_value(parameter, var);
            break;

        case config_mgr_element_type_enum:
            set_element_return_value = set_enum_element_value(parameter, var);
            break;

        case config_mgr_element_type_ipaddr:
            set_element_return_value = set_ip_address_value(parameter, var);
            break;

        case config_mgr_element_type_none:
        case config_mgr_element_type_index:
        case config_mgr_element_type_interface:
            snmp_debug("unsupported element type [%d]\n", type);
            break;
    }

    if (set_element_return_value < 0)
    {
        snmp_debug("failed setting: set_element_return_value [%d]\n", set_element_return_value);
        goto error;
    }

    retval = SNMP_ERR_NOERROR;
error:
    return retval;
}

