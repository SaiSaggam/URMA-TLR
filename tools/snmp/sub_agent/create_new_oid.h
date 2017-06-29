#ifndef __CREATE_NEW_OID_H__
#define __CREATE_NEW_OID_H__

#include "config_table.h"
#include "net-snmp-config.h"
#include "net-snmp-includes.h"
#include "agent/net-snmp-agent-includes.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


int create_new_oid(oid const * const fixed_oid_prefix,
                    uint32_t fixed_oid_prefix_length,
                    oid * const new_oid,
                    uint32_t * const new_oid_length,
                    config_mgr_group_type_t const group_type,
                    size_t const group_id,
                    bool const is_table_group,
                    size_t const instance,
                    size_t const element_id,
                    bool const is_index_element);


#endif /* __CREATE_NEW_OID_H__ */
