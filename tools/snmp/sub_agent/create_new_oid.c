#include "create_new_oid.h"
#include "snmp_utils.h"

static int const config_table_sub_oid = 1;
static int const config_table_entry_sub_oid = 1;
static int const oid_offset_from_group_type = SNMP_GROUP_TYPE_OFFSET_FROM_OID;
static int const oid_offset_from_group_id = SNMP_GROUP_OFFSET_FROM_OID;
static int const enterprise_mib_fixed_oids_length = 8;
static int const sub_oid_for_index_element = 1;
static int const offset_to_next_element_in_table = 2;
static int const offset_to_next_element = 1;

int create_new_oid(oid const * const fixed_oid_prefix,
                   uint32_t fixed_oid_prefix_length,
                   oid * const new_oid,
                   uint32_t * const new_oid_length,
                   config_mgr_group_type_t const group_type,
                   size_t const group_id,
                   bool const is_table_group,
                   size_t const instance,
                   size_t const element_id,
                   bool const is_index_element)
{
    uint32_t suboid;
    int result;

    if (fixed_oid_prefix_length < enterprise_mib_fixed_oids_length)
    {
        result = -1;
        goto done;
    }

    for (suboid = 0; suboid < enterprise_mib_fixed_oids_length; suboid++)
    {
        new_oid[suboid] = fixed_oid_prefix[suboid];
    }
    new_oid[suboid++] = group_type + oid_offset_from_group_type;
    new_oid[suboid++] = group_id  + oid_offset_from_group_id;

    if (is_table_group)
    {
        new_oid[suboid++] = config_table_sub_oid;
        new_oid[suboid++] = config_table_entry_sub_oid;
        if (is_index_element)
        {
            new_oid[suboid++] = sub_oid_for_index_element;
        }
        else
        {
            new_oid[suboid++] = element_id + offset_to_next_element_in_table;
        }
        new_oid[suboid++] = instance;
    }
    else
    {
        new_oid[suboid++] = element_id + offset_to_next_element;
        new_oid[suboid++] = 0;
    }

    *new_oid_length = suboid;
    result = 0;

done:
    return result;
}


