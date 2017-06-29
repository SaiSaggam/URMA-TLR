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
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "read_show_element.h"
#include "config_mgr.h"

int read_show_element_as_string(tlr_handle_t handle, size_t element_id, void * * buffer)
{
    int result;
    char const * element_value;
    config_status_t get_element_status;

    get_element_status = tlr_status_get_string_ptr(handle, element_id, &element_value);
    if (get_element_status != config_status_success)
    {
        result = SNMP_ERR_NOSUCHNAME;
        goto error;
    }

    *buffer = strdup(element_value);
    if (*buffer == NULL)
    {
        result = SNMP_ERR_GENERR;
        goto error;
    }

    result = SNMP_ERR_NOERROR;

error:
    return result;
}

int read_show_element(tlr_handle_t handle, config_mgr_element_t const * const element, void * * buffer)
{
    int result;
    size_t required_element_size;
    config_status_t get_element_status;

    required_element_size = tlr_get_element_size(element);
    *buffer = malloc(required_element_size);
    if (*buffer == NULL)
    {
        result = SNMP_ERR_GENERR;
        goto error;
    }

    get_element_status = tlr_status_read(handle, element->id, *buffer, required_element_size);
    if (get_element_status != config_status_success)
    {
        result = SNMP_ERR_NOSUCHNAME;
        goto error;
    }

    result = SNMP_ERR_NOERROR;

error:
    return result;
}


