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
#include "config_mgr.h"
#include "oid_to_group.h"

#define FIRST_OID_IN_ENTERPRISE_MIB 8  // Skip over the first 8 sub-oids

static int const first_oid_in_enterprise_mib = FIRST_OID_IN_ENTERPRISE_MIB;
static size_t const min_oid_len_for_a_group = FIRST_OID_IN_ENTERPRISE_MIB + 1;

static int const oid_offset_from_group_type = SNMP_GROUP_TYPE_OFFSET_FROM_OID;
static int const oid_offset_from_group_id = SNMP_GROUP_OFFSET_FROM_OID;

static bool is_group_in_mib(config_mgr_group_t const * const group)
{
    return ((group != NULL) && group->in_mib
            && (group->access != config_mgr_access_no_access));
}

static config_mgr_group_t const *get_next_group_in_group_type(config_mgr_group_type_t const group_type, size_t const group_id)
{
    config_mgr_group_t const *next_group = NULL;
    size_t const max_group_count =  tlr_get_num_groups(group_type);
    size_t next_group_id = group_id +1;

    while (next_group_id < max_group_count)
    {
        next_group = tlr_get_group_ptr(next_group_id, 1, group_type);

        if (is_group_in_mib(next_group))
        {
            break;
        }
        else
        {
            next_group = NULL;
        }
        next_group_id++;
    }

    return next_group;
}

static bool is_end_of_set_groups(config_mgr_group_t const * const next_group,
                                 config_mgr_group_type_t const * const group_type)
{
    return (next_group == NULL) && (*group_type == config_mgr_set_group);
}

static config_mgr_group_t const *const get_next_group_in_config_table(config_mgr_group_t const * const group,
														  config_mgr_group_type_t * const group_type)
{
    config_mgr_group_t const *next_group = get_next_group_in_group_type(*group_type, group->id);

    if (is_end_of_set_groups(next_group, group_type))
    {
        *group_type = config_mgr_get_group;
        next_group = get_next_group_in_group_type(*group_type, 0);
    }

    return next_group;
}

static config_mgr_group_t const * const get_next_group(config_mgr_group_t const * const group,
									     config_mgr_group_type_t * const group_type)
{
	config_mgr_group_t  const * next_group;

	if (INVALID_GROUP_TYPE(*group_type))
	{
		next_group = NULL;
        goto done;
	}

	next_group = get_next_group_in_config_table(group, group_type);

done:
	return next_group;
}

static config_mgr_group_t const * get_first_group_included_in_mib(config_mgr_group_type_t const group_type)
{
    /* Get the first group of a type (config/show) that is included in the MIB.
     * We'll work under the assumption that there will be at least one group of the specified type
     * that is included in the mib.
     */
    config_mgr_group_t const * group;
    size_t group_id;

    group_id = 0;
    do
    {
        group = tlr_get_group_ptr(group_id, 1, group_type);
        group_id++;
    }
    while(group != NULL && !group->in_mib);

    return group;
}

static config_mgr_group_t const * get_first_group(oid const * const oid,
											uint32_t const oid_len,
											config_mgr_group_type_t * const group_type)
{
	config_mgr_group_t const *group=NULL;

	if (oid_len == min_oid_len_for_a_group)
	{
		*group_type = oid[first_oid_in_enterprise_mib] - oid_offset_from_group_type;
	}
	else
	{
        // Default to the set group since it's first
		*group_type = config_mgr_set_group;
	}

	if (INVALID_GROUP_TYPE(*group_type))
	{
		goto error;
	}

	group = get_first_group_included_in_mib(*group_type);

error:
	return group;
}

config_mgr_group_t const * const get_group_from_oid(oid const * const oid,
										uint32_t const oid_len,
										config_mgr_group_type_t * const group_type)
{
	config_mgr_group_t const *group = NULL;
	int suboid = first_oid_in_enterprise_mib;
	int group_id;

	*group_type = oid[suboid++] - oid_offset_from_group_type;

	if (INVALID_GROUP_TYPE(*group_type))
	{
		goto error;
	}

	group_id = oid[suboid++] - oid_offset_from_group_id;

	group = tlr_get_group_ptr(group_id, 1, *group_type);
    if (group != NULL && !group->in_mib)
    {
        group = NULL;
    }

error:
	return group;
}

config_mgr_group_t const * snmp_get_next_group(oid const * const oid, uint32_t const oid_len,
		config_mgr_group_type_t * const group_type)
{
	config_mgr_group_t const *next_group = NULL;

	if (oid_len <= min_oid_len_for_a_group)
	{
		next_group = get_first_group(oid, oid_len, group_type);
	}
	else
	{
		config_mgr_group_t const *this_group = get_group_from_oid(oid, oid_len, group_type);

		if (this_group == NULL) goto error;

		next_group = get_next_group(this_group, group_type);
	}

error:
    return next_group;
}

config_mgr_group_t const * snmp_get_group(oid const * const oid, uint32_t const oid_len,
		config_mgr_group_type_t * const group_type)
{
	config_mgr_group_t const *group;

	if (oid_len <= min_oid_len_for_a_group)
	{
		group = get_first_group(oid, oid_len, group_type);
	}
	else
	{
		group = get_group_from_oid(oid, oid_len, group_type);
	}

	return group;
}











