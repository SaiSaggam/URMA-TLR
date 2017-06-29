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

#include "config_file.h"
#include "config_file_helper.h"
#include "config_mgr.h"
#include <stdio.h>
#include <string.h>
#include <uci.h>
#include <time.h>

void config_file_write_timestamp(FILE * const fp, char const * const user)
{
	time_t cur_time = time(NULL);
	fprintf(fp, "# Last updated by %s on %s\r\n", user, ctime(&cur_time));
}

int config_file_write_commands(FILE * const fp)
{
	int result = -1;
	config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
	size_t i;
	struct uci_context * uci_context = uci_alloc_context();

	if (uci_context == NULL)
	{
		fprintf(stderr, "Unable to access UCI\n");
		goto error;
	}

	uci_set_confdir(uci_context, UCI_MODIFIED_PATH);

	for (i = 0; i < table->count; i++)
	{
		config_mgr_group_t const * const group = &table->groups[i];
		if (write_group_commands_to_file(uci_context, fp, group) == -1)
			goto error;
	}

	result = 0;

error:
	if (uci_context != NULL)
		uci_free_context(uci_context);

	return result;
}
