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
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"
#include "validate.h"

static FILE    notexisting;

static char const * access_to_string_user(config_mgr_access_level_t const access)
{
    char const * access_string;

    switch (access)
    {
        case config_mgr_access_level_read_write:
            access_string = "readwrite";
            break;
        case config_mgr_access_level_read_only:
            access_string = "readonly";
            break;
        case config_mgr_access_level_super:
            access_string = "super";
            break;
        default:
            access_string = "no access";  // LZ: I made this up, or should it be readonly?
            break;
    }

    return access_string;
}

static char const * access_to_string_group(config_mgr_access_t const access)
{
    char const * access_string;

    switch (access)
    {
        case config_mgr_access_readwrite:
            access_string = "readwrite";
            break;
        case config_mgr_access_readonly:
            access_string = "readonly";
            break;
        case config_mgr_access_writeonly:
            access_string = "writeonly";
            break;
        case config_mgr_access_readonly_super_readwrite:
            access_string = "super";
            break;
        case config_mgr_access_no_access:
        default:
            access_string = "no access";
            break;
    }

    return access_string;
}

tlr_handle_t tlr_apply_open(size_t const id, size_t const instance, char const * const user, config_mgr_access_level_t const access)
{
	FILE * fp = NULL;
	config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);
	config_mgr_group_t const * const group_ptr = &table_ptr->groups[id];

	char group_name[CONFIG_MGR_NAME_LENGTH];
	replace_char(group_ptr->name, group_name, strlen(group_ptr->name), '-', '_');

    if (tlr_verify_access(group_ptr->access, config_mgr_write_operation, access) != config_status_success)
    {
        tlr_config_error("Permission denied %s required access %s, have access %s\n", group_name, access_to_string_group(group_ptr->access), access_to_string_user(access));
        goto ret;
    }

	if (instance <= group_ptr->instances)
	{
		char process[CONFIG_MGR_NAME_LENGTH + sizeof "apply_ -i nnnn\0"];
        char pathname[CONFIG_MGR_NAME_LENGTH + + sizeof "/usr/bin/apply_\0"];
        struct stat file_info;

		snprintf(process, sizeof process, "apply_%s -i %zu -u %s -a %d", group_name, instance, user, access);
		snprintf(pathname, sizeof pathname, "/usr/bin/apply_%s", group_name );

		tlr_config_debug("executing \"%s\"\n", process);

        if (stat(pathname, &file_info) == 0)
            fp = popen(process, "r");
        else
            fp = &notexisting;
	}
	else
	{
	    tlr_config_error("Invalid instance %zu for %s to apply\n", instance, group_name);
	}

ret:
	return fp;
}

config_status_t tlr_apply_read(tlr_handle_t const handle, char * const data, size_t const data_length, size_t const timeout)
{
	config_status_t status = config_status_read_complete;
	FILE * const fp = (FILE * const)handle;
	struct timeval tv = {.tv_sec = timeout/1000, .tv_usec = (timeout % 1000) * 1000};
	int const fd = fileno(fp);
	fd_set fds;
	int ret_val;

	*data = '\0';

	if ( fp == &notexisting )
        return status;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	ret_val = select(fd+1, &fds, NULL, NULL, (timeout != WAIT_FOREVER) ? &tv : NULL);
	switch (ret_val)
	{
	case 0:
		status = config_status_timeout;
		break;

	case -1:
		status = config_status_commit_error;
		break;

	default:
		ret_val = fread(data, sizeof(char), data_length-1, fp);
		if (ret_val < 0)
		    status = config_status_system_error;
		else	/* caller expects NUL terminated data */
		{
			data[ret_val] = '\0';
			if (ret_val != 0) status = config_status_success;
		}
		break;
	}

	return status;
}

int tlr_apply_close(tlr_handle_t const handle)
{
    FILE * const fp = (FILE * const )handle;
    if ( fp == &notexisting )
         return 0;
    else
	    return pclose(fp);
}

config_status_t tlr_run_all_apply_processes(void)
{
	config_status_t status = config_status_success;
	config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);

	size_t group_id;

	for (group_id = 0; group_id < table_ptr->count; group_id++)
	{
		size_t instance;
		config_mgr_group_t const * const group_ptr = &table_ptr->groups[group_id];
		size_t max_instances = group_ptr->instances;

		if (group_ptr->access == config_mgr_access_no_access)
		{
		    /* not supported */
		    continue;
		}

		for (instance = 1; instance <= max_instances; instance++)
		{
		    /* this process is called at startup to apply all the config changes made by user "boot" */
		    tlr_handle_t const handle = tlr_apply_open(group_id, instance, "boot", config_mgr_access_level_super);

            tlr_config_debug("/usr/bin/apply_%s %zd\n", group_ptr->name, instance);
			if (handle != NULL)
			{
				char status_buf[128];

                /* XXX Do we really need to read (and drop) the response from each apply process?
                 */
                while (status == config_status_success)
				{
				    status = tlr_apply_read(handle, status_buf, sizeof status_buf, WAIT_FOREVER);
                    switch (status)
                    {
                    case config_status_success:
                        break;
                    case config_status_read_complete:
                        tlr_config_debug("%s\n", status_buf);
                        break;
                    default:
                        tlr_config_error("%s: %d\n", group_ptr->name, status);
                        break;
                    }
                }

				tlr_apply_close(handle);
			}
		}

		status = config_status_success;
	}

	return status;
}
