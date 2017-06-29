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
#ifndef OID_TO_ELEMENT_H
#define OID_TO_ELEMENT_H

#include "snmp_utils.h"
#include "config_def.h"

config_mgr_element_t *snmp_get_next_element(oid const * const oid, uint32_t oid_len,
									config_mgr_group_t const ** group,
									config_mgr_group_type_t *group_type,
									uint32_t *instance);

config_mgr_element_t *snmp_get_element(oid const * const oid, uint32_t oid_len,
								  config_mgr_group_t const * group,
								  uint32_t *instance);

#endif /* SNMP_ELEMENT_H */
