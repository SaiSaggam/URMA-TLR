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
#ifndef SNMP_UTILS_H
#define SNMP_UTILS_H

#include <stdint.h>
#include "config_def.h"
#include "config_table.h"

#ifdef config_error
#undef config_error
#endif

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#define INVALID_GROUP_TYPE(group_type) (((group_type) != config_mgr_set_group) && ((group_type) != config_mgr_get_group))

typedef struct
{
	config_mgr_group_type_t   group_type;
	config_mgr_group_t       const *group;
	config_mgr_element_t     *element;
	uint32_t                  instance;
	oid                       oid[MAX_OID_LEN];
	uint32_t                  oid_length;
} parameter_t;

u_char snmp_convert_tlr_type_to_asn_type(config_mgr_element_type_t element_type);
int snmp_adjust_element(u_char const asn_type, size_t * const element_size, void * * const buffer);
bool is_a_table(config_mgr_group_t const * const pgroup);
int convert_ip_address_to_string_from_binary(char const * const binary_ip_address, char * const ip_address_string_buffer,
        size_t const str_len);

#define SNMP_GROUP_OFFSET_FROM_OID 1
#define SNMP_ELEMENT_OFFSET_FROM_GROUP_OID 1
#define SNMP_ELEMENT_OFFSET_FROM_TABLE_OID 2
#define SNMP_GROUP_TYPE_OFFSET_FROM_OID 1

#define SNMP_ELEMENT_OFFSET_IN_GROUP 10
#define SNMP_ELEMENT_OFFSET_IN_TABLE 12

#endif /* SNMP_UTILS_H */
