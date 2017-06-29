/*****************************************************************************
 * Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#include "config_def.h"
#include "apply_defroute.h"
#include "tlr_print.h"
#include "interfaces.h"
#include "messages.h"
#include "message_codes.h"
#include "update_message.h"
#include "digiutils.h"

int main (int argc, char *argv[]) {
    int ret = EXIT_FAILURE;
    size_t instance;
    char msg[64];
    char const * const config_group = get_config_group_name_from_id(config_defroute);

	if (get_instance(argc, argv, &instance, config_defroute))
	{
		goto error;
	}
	snprintf(msg, sizeof msg, "%s %zd", config_group, instance);
    if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, update_message, msg, NULL) != 0)
    {
         char const to_ifm_failed_log[] = "sent message to interface manager failed";
         tlr_event_log(tlr_log_error, "apply defroute", to_ifm_failed_log);
         goto error;
    }

    ret = EXIT_SUCCESS;
error:
	return ret;
}
