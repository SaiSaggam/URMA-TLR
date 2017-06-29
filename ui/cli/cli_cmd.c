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

#include "cli_cmd.h"
#include "config_mgr.h"
#include "tlr_common.h"

#include <string.h>
#include <signal.h>
#include <strlcpy.h>

static char output_buffer[CLI_BUFFER_SIZE];
static char const ascii_format[] = "ascii";

cli_status_t cli_get_config(cli_t * context, char const * const group_name, size_t const instance, char const * const element_name, char  ** const value)
{
    cli_status_t  retval = cli_config_error;
    cli_tree_data_t * tree_data = NULL;
    cli_tree_data_type_t tree_data_type;
    void * tree = NULL;

    *value = NULL;

    if ((cli_tree_find(cli_tree_type_command, group_name, &tree, &tree_data_type) != 0) || (tree == NULL))
    {
        cli_error(context, "Cannot find \"%s\" configuration %p\n", group_name, tree_data);
        goto ret;
    }

    tree_data = tree;
    if (tree_data_type == cli_tree_data_type_group)
    {
        config_mgr_group_t const * const group = tree_data->data;
        cli_tree_data_t const * element_tree;
        cli_tree_data_type_t element_tree_type;

        if (cli_tree_find_subtree(tree_data->search_tree, element_name, &tree, &element_tree_type) != 0)
        {
            cli_error(context, "Cannot find \"%s\" element in \"%s\" configuration\n", element_name, group_name);
            goto ret;
        }

        element_tree = tree;
        if (element_tree_type  == cli_tree_data_type_element)
        {
            config_mgr_element_t const * const element = element_tree->data;
            char * data_ptr = NULL;

            {
                config_status_t status = cli_cmd_get_element(context, group, instance, element->id, &data_ptr);
                if (status != config_status_success)
                {
                    free(data_ptr);
                    goto ret;
                }
            }

            *value = data_ptr;
            retval = cli_success;
        }

    }

ret:
    return retval;
}


cli_status_t cli_cmd_set_element(cli_t * const context, config_mgr_group_t const * const group, size_t const instance, size_t const element_id, char const * const element_value, char const ** const error_string)
{
	cli_status_t status;
    config_status_t const config_status = tlr_set_element_value(group->id, instance, element_id, element_value, context->access);

	if (config_status != config_status_success)
	{
	    if (error_string != NULL)
	    {
            *error_string = tlr_config_get_error(config_status);

	        if ((config_status == config_status_invalid_value) && (element_id < group->elements.count))
	        {
                config_mgr_element_t const * element = &group->elements.element[element_id];
                if (element->error != NULL)
                {
                    *error_string = element->error;
                }
	        }
	    }

	    status = cli_config_error;
	}
	else
	{
	    status = cli_success;
	}

	return status;
}

cli_status_t cli_cmd_get_element(cli_t * const context, config_mgr_group_t const * const group, size_t const instance,
                                    size_t const element_id, char ** element_value)
{
	cli_status_t status;
	config_status_t const config_status = tlr_get_element_alloc_value(group->id, instance, element_id, (void **)element_value, config_mgr_element_type_string, context->access);

	if (config_status != config_status_success)
	{
			status = cli_config_error;
	}
	else
	{
        if (group->elements.element[element_id].access == config_mgr_access_writeonly)
        {
            char const * const result_str = tlr_user_show_password(group->elements.element[element_id].type, *element_value);
            free(*element_value);
            *element_value = strdup(result_str);
        }

        status = cli_success;
	}

	return status;
}

int cli_apply_config(cli_t * const context, size_t const group_id, size_t const instance)
{
    bool is_ok = false;
    tlr_handle_t handle = tlr_apply_open(group_id, instance, context->user, context->access);

    if (handle == NULL)
    {
        cli_printf(context, "\r\nUnable to apply the configuration\r\n");
    }
    else
    {
        config_status_t config_status;

        config_status = tlr_apply_read( handle, output_buffer, sizeof output_buffer, seconds_to_milliseconds(5));
        switch (config_status)
        {
        case  config_status_timeout:
            cli_printf(context, "\r\nApply timeout\n");
            break;
        case config_status_commit_error:
            cli_printf(context, "\r\nApply error\n");
            break;
        default:
            if (strlen(output_buffer) > 0)
                cli_printf(context, "\r\n%s\n", output_buffer);
            break;
        }

        if (tlr_apply_close(handle) == 0) is_ok = true;
    }

    if (!is_ok)
        cli_printf(context, "\r\n%s\r\n\r\n", "ERROR");

    return is_ok ? 0 : -1;

}

int process_show_command(cli_t * const context, size_t const group_id, size_t const instance, char const * const parameter)
{
    /* TODO: need semaphore lock for each show command? */
    bool is_ok = false;

    tlr_handle_t handle = tlr_show_open(ascii_format, group_id, instance, parameter, context->access);

    if (handle == NULL)
    {
        cli_printf(context, "\r\nUnable to process the command\r\n");
    }
    else
    {
        ssize_t bytes_read;

        cli_printf(context, "\r\n");
        while ((bytes_read = tlr_show_read(handle, output_buffer, sizeof output_buffer-1, seconds_to_milliseconds(10))) > 0)
        {
            output_buffer[bytes_read] = '\0';
            cli_printf(context, "%s", output_buffer);
        }

        if (tlr_show_close(handle) == 0) is_ok = true;


    }
    if (!is_ok)
        cli_printf(context, "\r\n%s\r\n\r\n", "ERROR");

    return is_ok ? 0 : -1;
}

static volatile bool sigint_occurred = false;
static void sigint_handler (int signum)
{
    sigint_occurred = true;
}

int process_action_command(cli_t * const context, size_t const action_id, char const * const parameter)
{
    /* TODO: need semaphore lock for each show command? */
    bool is_ok = false;
    tlr_handle_t handle = NULL;
    __sighandler_t old_sigint;

    old_sigint = signal(SIGINT, sigint_handler);

    sigint_occurred = false;
    handle = tlr_action_open(ascii_format, action_id, context->user, parameter, context->access);

    if (handle == NULL)
    {
        cli_printf(context, "\r\nUnable to process the command\r\n");
    }
    else
    {
        ssize_t bytes_read;

        cli_printf(context, "\r\n");
        while ((bytes_read = tlr_action_read(handle, output_buffer, sizeof output_buffer -1, WAIT_FOREVER)) > 0)
        {
            output_buffer[bytes_read] = '\0';
            cli_printf(context, "%s", output_buffer);
        }

        if ((tlr_action_close(handle) == 0) || (sigint_occurred)) is_ok = true;


    }
    signal(SIGINT, old_sigint);
    if (!is_ok)
        cli_printf(context, "\r\n%s\r\n\r\n", "ERROR");

    return is_ok ? 0 : -1;

}
