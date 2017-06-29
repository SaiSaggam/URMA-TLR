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
#include "sub_agent.h"
#include <syslog.h>
#include "snmp_agent.h"
#include "snmp_agent_debug.h"
#include "request_handlers.h"
#include "unittest_support.h"


static const oid const lr54_mib_oid[] = {1, 3, 6, 1, 4, 1, 332, 1};

STATIC_FUNCTION int sub_agent_handler(netsnmp_mib_handler *handler,
					netsnmp_handler_registration *reginfo,
					netsnmp_agent_request_info *reqinfo,
					netsnmp_request_info *requests)
{
	int ret;
	int request_result;

    (void)handler;
    (void)reginfo;

    while (requests)
	{
		netsnmp_variable_list * variable = requests->requestvb;

		switch (reqinfo->mode)
		{
			case MODE_GET:
			    request_result = handle_get_request(variable);
				break;

			case MODE_GETNEXT:
                request_result = handle_get_next_request(variable);
				break;

			case MODE_SET_ACTION:
                request_result = handle_set_request_begin(variable);
                break;

            case MODE_SET_COMMIT:
                request_result = handle_set_request_commit(variable);
                break;

            case MODE_SET_BEGIN:
			case MODE_SET_FREE:
			case MODE_SET_UNDO:
			case MODE_SET_RESERVE1:
			case MODE_SET_RESERVE2:
			    request_result = SNMP_ERR_NOERROR;
				break;

			default:
				snmp_debug("Unsupported mode %d\n", reqinfo->mode);
                request_result = SNMP_ERR_NOERROR;
				break;
		}

		if (request_result != SNMP_ERR_NOERROR)
		{
		    netsnmp_request_set_error(requests, request_result);
		}
		requests = requests->next;
	}

	ret = SNMP_ERR_NOERROR;

    return ret;
}

// The initialization function's name must match init_FILENAME())
void init_sub_agent(void)
{
	int ret;

    openlog(SUBAGENT_PROCESS_NAME, 0, LOG_USER);

	snmp_debug("initializing\n");

	ret = netsnmp_register_handler(netsnmp_create_handler_registration
							(SUBAGENT_PROCESS_NAME, sub_agent_handler,
							lr54_mib_oid, OID_LENGTH(lr54_mib_oid),
							HANDLER_CAN_RWRITE));

	if (ret == SNMPERR_SUCCESS)
	{
		snmp_debug("Done initializing\n");
	}
	else
	{
		snmp_error("Error: netsnmp_register_handler returned %d\n", ret);
	}
}

