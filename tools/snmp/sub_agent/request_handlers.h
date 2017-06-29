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

#ifndef __REQUEST_HANDLERS_H__
#define __REQUEST_HANDLERS_H__

#include "net-snmp-config.h"
#include "net-snmp-includes.h"
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/table.h>
#include <net-snmp/agent/instance.h>
#include <net-snmp/agent/table_data.h>
#include <net-snmp/agent/table_dataset.h>

int handle_get_request(netsnmp_variable_list * var);
int handle_get_next_request(netsnmp_variable_list * var);
int handle_set_request_commit(netsnmp_variable_list const * const var);
int handle_set_request_begin(netsnmp_variable_list const * const var);
#endif /* __REQUEST_HANDLERS_H__ */
