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

static int const element_id_difference_from_group_oid = SNMP_ELEMENT_OFFSET_FROM_GROUP_OID;
static int const element_id_difference_from_table_oid = SNMP_ELEMENT_OFFSET_FROM_TABLE_OID;

static int const element_offset_in_group = SNMP_ELEMENT_OFFSET_IN_GROUP;
static int const element_offset_in_table = SNMP_ELEMENT_OFFSET_IN_TABLE;

static bool more_instances_left_in_table(config_mgr_group_t const * const pgroup, uint32_t const instance)
{
    return instance < pgroup->instances;
}

static bool more_elements_left_in_group(config_mgr_group_t const * const pgroup, int const element_id)
{
    return (element_id + 1) < pgroup->elements.count;
}

config_mgr_group_t const * goto_next_group(const oid* const oid,
        const uint32_t oid_len, config_mgr_group_type_t * const group_type,
        uint32_t *const instance,
        int *element_id, bool *is_index_element)
{
    const config_mgr_group_t *next_group = snmp_get_next_group(oid, oid_len, group_type);

    if (next_group == NULL)
    {
        //printf("no next group\n");
        goto end;
    }

    //printf("going to next group %s\n", next_group->name);

    *instance   = 1;
    *element_id = 0;

    if (is_a_table(next_group))
    {
        *is_index_element = true;
    }

end:
    return next_group;
}

static oid element_oid_value;

config_mgr_element_t *snmp_find_next_element_in_table(oid const * const oid, uint32_t const oid_len,
											config_mgr_group_t const **group,
											config_mgr_group_type_t * const group_type,
											uint32_t * const instance)
{
	int suboid;
	int element_id;
	config_mgr_group_t   const *pgroup = *group;
	config_mgr_element_t *element = NULL;
	bool is_index_element = false;

	suboid = element_offset_in_table;
	element_oid_value = oid[suboid++];

	element_id = element_oid_value - element_id_difference_from_table_oid;
	*instance  = oid[suboid];

	//printf("snmp_find_next_element_in_table element id %d group %s count %d\n", element_id, pgroup->name, pgroup->elements.count);

    if (more_instances_left_in_table(pgroup, *instance))
	{
		(*instance)++;
	    if (element_oid_value == 1)
	    {
	        // This is an index element and we are going to the next index element.
	        is_index_element = true;
	    }
	}
    else if (more_elements_left_in_group( pgroup, element_id))
	{
		element_id++;
		*instance = 1;
	}
	else
	{
        pgroup = goto_next_group(oid, oid_len, group_type,
                instance, &element_id, &is_index_element);
        if (pgroup == NULL)
        {
            *group = NULL;
            goto end;
        }
        *group = pgroup;
	}

    if (is_index_element)
    {
        element = snmp_get_index_config_element();
    }
    else
    {
        element = &pgroup->elements.element[element_id];
    }

    //printf("next_element_in_table: group %s name %s instance %d\n", (pgroup)->name, element->name, *instance);

end:
	return element;
}

// Find the next element in the group (not a table)
config_mgr_element_t *snmp_find_next_element_in_group(oid const * const oid, uint32_t const oid_len,
												config_mgr_group_t const **group,
												config_mgr_group_type_t * const group_type,
												uint32_t * instance)
{
	uint32_t element_id;
	config_mgr_group_t  const *pgroup = *group;
	config_mgr_element_t *element = NULL;

	element_id = oid[element_offset_in_group] - element_id_difference_from_group_oid;

	if (more_elements_left_in_group(pgroup, element_id))
	{
		element_id++;
        element = &pgroup->elements.element[element_id];
	}
	else
	{
		config_mgr_group_t const *next_group = snmp_get_next_group(oid, oid_len, group_type);

		if (next_group == NULL)
		{
			*group = NULL;
			goto end;
		}

		*group = next_group;
		element_id = 0;

		if (is_a_table(next_group))
		{
		    element = snmp_get_index_config_element();
		}
		else
		{
		    element = &next_group->elements.element[element_id];
		}
	}
    *instance  = 1;
    //printf("next_element_in_group: group %s name %s instance %d\n", (*group)->name, element->name, *instance);

end:
	return element;
}

