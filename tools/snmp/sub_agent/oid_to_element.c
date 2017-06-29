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
#include "snmp_utils.h"
#include "snmp_agent_debug.h"
#include "oid_to_parameter.h"
#include "oid_to_group.h"
#include "index_element.h"
#include "find_element.h"

static const size_t min_oid_length_for_get_next_table = 14;
static const size_t min_oid_length_for_get_next = 11;

static const size_t oid_length_for_get_request = 12;

static int const element_id_difference_from_group_oid = SNMP_ELEMENT_OFFSET_FROM_GROUP_OID;
static int const element_id_difference_from_table_oid = SNMP_ELEMENT_OFFSET_FROM_TABLE_OID;

static int const element_offset_in_group = SNMP_ELEMENT_OFFSET_IN_GROUP;
static int const element_offset_in_table = SNMP_ELEMENT_OFFSET_IN_TABLE;

static int const index_offset_in_oid=12;

static bool is_index_oid(oid const * const oid)
{
    return oid[index_offset_in_oid] == 1;
}

static const size_t instance_oid_offset_for_group = 11;

static bool is_last_oid_zero_in_group(oid const * const oid, uint32_t const oid_len)
{
    return oid[instance_oid_offset_for_group] == 0;
}

config_mgr_element_t *snmp_get_next_element(oid const * const oid, uint32_t const oid_len,
										config_mgr_group_t const ** group,
									 config_mgr_group_type_t * const group_type,
									 uint32_t * instance)
{

	config_mgr_group_t   const *pgroup = *group;
	config_mgr_element_t *element = NULL;

	if (is_a_table(pgroup))
	{
        if (oid_len < min_oid_length_for_get_next_table)
        {
			element = snmp_get_index_config_element();
			*instance = 1;
        }
        else
        {
			element = snmp_find_next_element_in_table(oid, oid_len, group, group_type, instance);
        }
    }
	else
	{
        if (oid_len < min_oid_length_for_get_next)
        {
			element = &pgroup->elements.element[0];
			*instance = 1;
        }
        else
        {
			element = snmp_find_next_element_in_group(oid, oid_len, group, group_type, instance);
        }
    }

	return element;
}

config_mgr_element_t *snmp_get_element(oid const * const oid, uint32_t const oid_len,
								  config_mgr_group_t const * pgroup,
								  uint32_t *instance)
{
	config_mgr_element_t *element = NULL;
	uint32_t element_id;
	int      suboid;

	if (is_a_table(pgroup))
	{
        if (oid_len != min_oid_length_for_get_next_table)
        {
			goto end;
        }
        suboid = element_offset_in_table;
        element_id = oid[suboid++] - element_id_difference_from_table_oid;
        *instance  = oid[suboid];
	}
	else
	{
        if (oid_len != oid_length_for_get_request)
        {
			goto end;
        }

        if (!is_last_oid_zero_in_group(oid, oid_len))
        {
            goto end;
        }

		suboid = element_offset_in_group;
		element_id = oid[suboid++] - element_id_difference_from_group_oid;
		*instance = 1;
	}

	if (is_index_oid(oid))
    {
        element = snmp_get_index_config_element();
    }
	else
	{
	    if (element_id >= pgroup->elements.count)
	    {
	        goto end;
	    }

	    element = &pgroup->elements.element[element_id];
	}

end:
	return element;
}


