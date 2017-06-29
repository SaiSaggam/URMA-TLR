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

#ifndef __SNMP_AGENT_DEBUG_H__
#define __SNMP_AGENT_DEBUG_H__

#include "snmp_agent.h"
#include "snmp_utils.h"

// 0 = send all messages to syslog (normal operation)
// 1 = send all messages to console (during development)
// To run the sub agent stand alone:
// 1. killall sub_agent
// 2. ./sub_agent -Dsub_agent -x tcp:localhost:1705 -f
#if !defined(SUBAGENT_LOG_TO_CONSOLE)  /* may be defined in the Makefile */
#define SUBAGENT_LOG_TO_CONSOLE 0
#endif

#if SUBAGENT_LOG_TO_CONSOLE
#include <stdio.h>
#define logfunc(l, x...) do { printf(SUBAGENT_PROCESS_NAME ": %s: ", __func__ ); printf(x); } while (0);
#else
#include <syslog.h>
#define logfunc syslog
#endif

#define snmp_alert(x...)		do { logfunc(LOG_ALERT, x); } while (0)
#define snmp_critical(x...)		do { logfunc(LOG_CRIT, x); } while (0)
#define snmp_error(x...)		do { logfunc(LOG_ERR, x); } while (0)
#define snmp_warning(x...)	    do { logfunc(LOG_WARNING, x); } while (0)
#define snmp_notice(x...)	    do { logfunc(LOG_NOTICE, x); } while (0)
#define snmp_info(x...)			do { logfunc(LOG_INFO, x); } while (0)
#define snmp_debug(x...)		do { logfunc(LOG_DEBUG, x); } while (0)

void snmp_print_get_request(netsnmp_variable_list const * const var);
void snmp_print_set_request(netsnmp_variable_list const * const var);
void snmp_print_get_next_request(netsnmp_variable_list const * const var);
void snmp_print_parameter_debug_info(parameter_t const * const parameter);

#endif /* __SNMP_AGENT_DEBUG_H__ */
