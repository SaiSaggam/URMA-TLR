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
#include "libgen.h"
#include "tlr_backup.h"
#include "tlr_print.h"
#include "digiutils.h"
#include "config_mgr.h"
#include "wan_nat.h"
#include "update_message.h"
#include "messages.h"
#include "interfaces.h"


int main(int argc, char * argv[])
{
    int result = EXIT_FAILURE;
    size_t instance = 0;
	char const * const config_group = get_config_group_name_from_id(config_wan);
    char * process_name;
    char * msg = NULL;

    process_name = basename(argv[0]);
    openlog(process_name, 0, LOG_USER);

    if (get_instance(argc, argv, &instance, config_wan) < 0)
    {
        goto error;
    }

	if (asprintf(&msg, "%s %d", config_group, instance) == -1)
	{
        tlr_debug("Could not allocate memory in asprintf\n");
        goto error;
	}

	if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, update_message, msg, NULL))
	{
		tlr_event_log(tlr_log_error, "apply_wan", "apply_wan: send_message() to interface_manager failed");
	}
	else
	{
		tlr_event_log(tlr_log_info, "apply_wan", "apply_wan: sent update_message to interface_manager");
    }

    if (apply_wan_nat_settings(instance) < 0)
    {
        goto error;
    }

    result = EXIT_SUCCESS;
error:
    free(msg);
    closelog();
    return result;
}


