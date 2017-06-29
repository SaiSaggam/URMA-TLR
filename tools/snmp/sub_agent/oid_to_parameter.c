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
#include "oid_to_parameter.h"
#include "oid_to_group.h"
#include "oid_to_element.h"
#include "create_new_oid.h"
#include "snmp_utils.h"
#include "index_element.h"


/*
 * Returns 0 if it finds the next parameter, 1 if there are no more parameters and -1 if there is an error
 */
static int get_next_parameter_from_oid(oid const * const orig_oid, uint32_t const oid_len, parameter_t * const parameter)
{
    int  retval = -1;

    if ((parameter->group = snmp_get_group(orig_oid, oid_len, &parameter->group_type)) == NULL)
    {
        goto done;
    }

    if ((parameter->element = snmp_get_next_element(orig_oid, oid_len, &parameter->group,
                                                    &parameter->group_type, &parameter->instance)) == NULL)
    {
        /* If there are no more parameters, then the group pointer is set to NULL */
        if (parameter->group == NULL)
        {
            retval = 1;
        }

        goto done;
    }
    retval = 0;
   // printf("get_next_parameter_from_oid: group %s name %s instance %d\n", parameter->group->name, parameter->element->name,
   //         parameter->instance);
done:
    return retval;
}

/*
 * Returns 0 if it finds the next parameter, 1 if there are no more parameters and -1 if there is an error
 */
int oid_to_next_parameter(oid const * const orig_oid, uint32_t const oid_len, parameter_t * const parameter)
{
    int retval = get_next_parameter_from_oid(orig_oid, oid_len, parameter);

    if (retval != 0)
    {
        goto done;
    }

    bool const is_index_element = parameter->element == snmp_get_index_config_element();

    retval = create_new_oid(orig_oid,
                   oid_len,
                   parameter->oid,
                   &parameter->oid_length,
                   parameter->group_type,
                   parameter->group->id,
                   is_a_table(parameter->group),
                   parameter->instance,
                   parameter->element->id,
                   is_index_element);

done:
    return retval;
}


/*
 * Converts the given OID into a group, instance and element ID
 */
int oid_to_parameter(oid const * const oid, uint32_t const oid_len, parameter_t * const parameter)
{
    int retval = -1;

    if ((parameter->group = snmp_get_group(oid, oid_len, &parameter->group_type)) == NULL)
    {
        goto end;
    }

    if ((parameter->element = snmp_get_element(oid, oid_len, parameter->group,
                                               &parameter->instance)) == NULL)
    {
        goto end;
    }

    retval = 0;

end:
    return retval;
}






