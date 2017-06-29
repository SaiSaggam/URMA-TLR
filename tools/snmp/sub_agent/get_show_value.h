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

#ifndef __GET_SHOW_VALUE_H__
#define __GET_SHOW_VALUE_H__

#include <stddef.h>
#include "config_def.h"
#include "config_table.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

int get_show_group_element_value(config_mgr_group_t const * const group,
                                 size_t instance,
                                 config_mgr_element_t const * const element,
                                 u_char const asn_type,
                                 void * * const value_buffer,
                                 size_t * const element_size);

#endif /* __GET_SHOW_VALUE__ */
