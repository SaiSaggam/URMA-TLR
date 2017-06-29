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
#include "get_show_value.h"
#include "read_show_element.h"
#include "snmp_utils.h"
#include "snmp_agent_debug.h"

int get_show_group_element_value(config_mgr_group_t const * const group,
                                 size_t instance,
                                 config_mgr_element_t const * const element,
                                 u_char const asn_type,
                                 void * * const value_buffer,
                                 size_t * const element_size)
{
    tlr_handle_t handle;
    int result;

    handle = tlr_status_open(group->id, instance);
    if (handle == NULL)
    {
        result = SNMP_ERR_GENERR;
        goto error;
    }

    switch (asn_type)
    {
        case ASN_OCTET_STR:
        case ASN_IPADDRESS:
        {
            int const read_as_string_result = read_show_element_as_string(handle, element->id, value_buffer);

            if (read_as_string_result != SNMP_ERR_NOERROR)
            {
                result = read_as_string_result;
                goto error;
            }
            break;
        }
        case ASN_INTEGER:
        case ASN_BOOLEAN:
        case ASN_UNSIGNED:
        case ASN_COUNTER:
        case ASN_COUNTER64:
        {
            int const read_show_element_result = read_show_element(handle, element, value_buffer);

            if (read_show_element_result != SNMP_ERR_NOERROR)
            {
                result = read_show_element_result;
                goto error;
            }
            break;
        }
        default:
            snmp_debug("Got SNMP request with unsupported ASN type (0x%x)", asn_type);
            result = SNMP_ERR_GENERR;
            goto error;

    }

    if (snmp_adjust_element(asn_type, element_size, value_buffer) < 0)
    {
        result = SNMP_ERR_GENERR;
    	goto error;
    }

    result = SNMP_ERR_NOERROR;

error:

    tlr_status_close(handle);

    return result;
}



