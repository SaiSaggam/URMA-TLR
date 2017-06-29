/*****************************************************************************
 * Copyright (c) 2013 Digi International Inc., All Rights Reserved
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

#include "cli.h"
#include "cli_tree.h"
#include "cli_cmd.h"
#include "config_table.h"
#include "unittest_support.h"
#include "tlr_common.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#define SET_TREE_VALUE_LENGTH(data_tree, length)   do { \
                                                       cli_tree_search_t * const search_tree = (data_tree)->search_tree; \
                                                       search_tree->max_value_length = (length); \
                                                   } while (0)

/*
 * Variables used by the twalk functions
 */

static struct
{
     cli_t * context;
     cli_cmd_t const * cmd;

} cb_data = {NULL, NULL};

static char const empty_str[] = "";
static char const dash_string[]="-----------------------------------------------------------------------------------------";
static char const current_value_string[] = "Current Value";
static char const parameter_string[] = "Parameter";
static char const description_string[] = "Description";
static char const syntax_string[] = "Syntax";

static size_t const description_max_row_length = 40;

static char * get_string_delimiter(char const * const str)
{
    char const separators[] = " ';.,";
    size_t i;
    char * ptr = (char *)str;
    char * result_ptr = NULL;


    for (i=0; i < sizeof separators -1; i++)
    {
        char * p = strchr(ptr, separators[i]);
        if (p != NULL)
        {
            result_ptr = p;
            break;
        }

    }

    return result_ptr;
}
/*
 * This function returns the length that the string can be
 * printed within max_str_length spaces.
 */
STATIC_FUNCTION size_t get_string_line_length(char const * const str, size_t const max_str_length)
{
    size_t line_length = strlen(str);
    char * ptr = (char *)str;

    while (line_length > max_str_length)
    {
        char * p = get_string_delimiter(ptr);

        if (p != NULL)
        {
            if ((p - str) > max_str_length)
            {
                line_length = (ptr - str);
                break;
            }
            p++;
        }
        else
        {
            line_length = ((ptr - str) > 1) ? (ptr - str): max_str_length;
            break;
        }

        ptr = p;
    }
    return line_length;
}


static void append_line(cli_t * const context, size_t const start_space_length,
                                char const * const str1, size_t const max_str1_length,
                                char const * const str2, size_t const max_str2_length)
{
    #define MAX_SPACE_WIDTH 25

    size_t const max_line_width = (context->width > start_space_length) ?
                         (context->width - start_space_length)  :
                         (start_space_length + context->width);

    size_t const max_str_width = (max_line_width >> 1) > MAX_SPACE_WIDTH ? MAX_SPACE_WIDTH : (max_line_width >> 1);

    size_t const max_str1_spaces = (max_str1_length > max_str_width) ? max_str_width : max_str1_length;
    size_t const max_str2_spaces = ((max_line_width - max_str1_spaces) > MAX_SPACE_WIDTH) ? (max_line_width - max_str1_spaces) : MAX_SPACE_WIDTH;

    char const space_str[] = "";
    size_t space_length = 0;

    char * str1_ptr = (char *) str1;
    char * str2_ptr = (char *) str2;

    size_t str1_length = strlen(str1);
    size_t str2_length = strlen(str2);

    while ((str1_length > 0) || (str2_length > 0))
    {

        size_t const length1 = (str1_length > 0) ?
                                get_string_line_length(str1_ptr, max_str1_spaces) :
                                0;
        size_t const length2 = (str2_length > 0) ?
                                get_string_line_length(str2_ptr, max_str2_spaces) :
                                0;

        cli_printf(context, " %+*s %-.*s %+*s %-.*s",  space_length, space_str,
                            length1, str1_ptr,
                            (max_str1_spaces - length1), space_str,
                            length2, str2_ptr);

        str1_ptr += length1;
        str2_ptr += length2;
        str1_length -= length1;
        str2_length -= length2;

        space_length = start_space_length +1;
        if ((str1_length > 0) || (str2_length > 0))
        {
            cli_printf(context, "\r\n");
        }


    }
}


static void print_line(cli_t * const context,
                                char const * const str1, size_t const max_str1_length,
                                char const * const str2, size_t const max_str2_length)
{
    #define MIN_SPACE_WIDTH 25

    size_t const max_line_width = context->width;

    size_t const max_str1_spaces = (max_str1_length > max_line_width) ? max_line_width : max_str1_length;
    size_t const max_str2_spaces = ((max_line_width - max_str1_spaces) > MIN_SPACE_WIDTH) ? (max_line_width - max_str1_spaces) : MIN_SPACE_WIDTH;

    char const space_str[] = "";

    char * str1_ptr = (char *) str1;
    char * str2_ptr = (char *) str2;

    size_t str1_length = strlen(str1);
    size_t str2_length = strlen(str2);


    while ((str1_length > 0) || (str2_length > 0))
    {

        size_t const length1 = (str1_length > 0) ?
                                get_string_line_length(str1_ptr, max_str1_spaces) :
                                0;
        size_t const length2 = (str2_length > 0) ?
                                get_string_line_length(str2_ptr, max_str2_spaces) :
                                0;

        cli_printf(context, "\r\n%-.*s %+*s %-.*s",
                            length1, str1_ptr,
                            (max_str1_spaces - length1), space_str,
                            length2, str2_ptr);

        str1_ptr += length1;
        str2_ptr += length2;
        str1_length -= length1;
        str2_length -= length2;
    }
}


static char * group_element_value (cli_t * const context,
                                        cli_tree_data_t const * action_tree,
                                        cli_tree_data_t const * group_tree, size_t instance,
                                        cli_tree_data_t const * element_tree, bool current_value)
{
    cli_status_t status;
    config_mgr_group_t const * const group = (group_tree != NULL) ? group_tree->data : NULL;
    config_mgr_element_t const * const element = element_tree->data;
    char * value = NULL;

    if (group == NULL) goto done;

    status = cli_cmd_get_element(context, group, instance, element->id, &value);

    if (status == cli_success)
    {
        if (current_value)
        {
            if (!STRING_IS_EMPTY(value))
            {
                if (element->access == config_mgr_access_writeonly)
                {
                  char const * const pwd_str = tlr_user_show_password(element->type, value);
                  char * pwd_ptr = strdup(pwd_str);

                  free(value);
                  value = pwd_ptr;
              }
            }
        }
        else
        {
            free(value);
            value = (element->default_value != NULL) ? strdup(element->default_value) : NULL;
        }
    }

done:
    return value;
}

static void print_group_element_value (cli_t * const context,
                                        cli_tree_data_t const * action_tree,
                                        cli_tree_data_t const * group_tree, size_t instance,
                                        cli_tree_data_t const * element_tree, bool current_value)
{
    cli_tree_search_t const * const search_tree = group_tree->search_tree;
    char * value = group_element_value (context,
                                        action_tree,
                                        group_tree, instance,
                                        element_tree, current_value);

    {
        size_t length = (value != NULL) ? strlen(value) : 1;

        SET_MAX(length, search_tree->max_value_length);

        cli_printf(context, " %-*s", length, (value != NULL) ? value : "");
    }

    free(value);
}


static void walk_help_cmd_cb(cli_tree_data_type_t data_type, char const * walk_name,
                                void * const data_tree, void * const context)
{
    if (data_tree != NULL)
    {
        switch (data_type)
        {
        case cli_tree_data_type_group:
            {
                cli_cmd_t * const cmd = context;
                cli_tree_data_t * const tree = data_tree;
                config_mgr_group_t const * const group = tree->data;
                cli_printf(cmd->context, "\r\n%s %-*s %-s", "", screen_length.name_length, group->name, group->description);
            }
            break;
        case cli_tree_data_type_show:
        case cli_tree_data_type_action:
            {
                cli_cmd_t * const cmd = context;
                cli_tree_data_t * const tree = data_tree;
                config_mgr_action_t const * const group = tree->data;

                cli_printf(cmd->context, "\r\n%s %-*s %-s", "", screen_length.name_length, group->name, group->description);
            }
            break;
        case cli_tree_data_type_element:
        case cli_tree_data_type_action_element:
        case cli_tree_data_type_enum:
            break;
        case cli_tree_data_type_COUNT:
            break;
        }
    }
}


static void walk_help_param_value_cb(cli_tree_data_type_t const tree_data_type, char const * tree_name,
                                   void * const data_tree, void * const context)
{
    (void) tree_name;

    if (data_tree != NULL)
    {
        switch (tree_data_type)
        {
        case cli_tree_data_type_group:
        case cli_tree_data_type_action:
        case cli_tree_data_type_show:
            break;
        case cli_tree_data_type_element:
        case cli_tree_data_type_action_element:
            {
                cli_cmd_t * const cmd = context;
                cli_t * const cli = cmd->context;
                cli_tree_data_t const * tree = data_tree;
                config_mgr_element_t const * const element = tree->data;

                cli_tree_search_t const * const search_tree = (tree_data_type == cli_tree_data_type_element) ?
                                                                cmd->group->search_tree :
                                                                cmd->action->search_tree;


                char * value = group_element_value (cli,
                                                    cmd->action,
                                                    cmd->group, cli->cmd_instance,
                                                    data_tree, 1);

                if (value != NULL)
                {
                    cli_printf(cli, "\r\n %-*s", search_tree->max_name_length, element->name);

                    append_line(cli, search_tree->max_name_length, value, search_tree->max_value_length,
                               element->description, description_max_row_length);

                    free(value);
                }
            }
            break;
        case cli_tree_data_type_enum:
            break;
        case cli_tree_data_type_COUNT:
            break;
        }

    }
}

static void print_element_value_cb(cli_tree_data_type_t const tree_data_type, char const * tree_name,
                                   void * const data_tree, void * const context)
{
    (void) tree_name;

    if (data_tree != NULL)
    {
        switch (tree_data_type)
        {
        case cli_tree_data_type_group:
        case cli_tree_data_type_action:
        case cli_tree_data_type_show:
            break;
        case cli_tree_data_type_element:
        case cli_tree_data_type_action_element:
            {
                cli_cmd_t * const cmd = context;
                cli_t * const cli = cmd->context;
                cli_tree_data_t const *  tree = data_tree;
                config_mgr_element_t const * const element = tree->data;

                char * value = group_element_value (cli,
                                                    cmd->action,
                                                    cmd->group, cli->cmd_instance,
                                                    data_tree, 1);

                if (value != NULL)
                {
                    cli_printf(cli, "\r\n   %-*s %s", screen_length.element_length, element->name, value);
                    free(value);
                }
            }
            break;
        case cli_tree_data_type_enum:
            break;
        case cli_tree_data_type_COUNT:
            break;
        }

    }
}

void cli_cmd_print_all_elements_in_group(cli_t * const context, cli_cmd_t * const cmd)
{
    cli_tree_data_t const * group_tree = cmd->group;

    context->cmd_instance = cmd->instance;
    cli_tree_walk_subtree(group_tree->search_tree, NULL, print_element_value_cb, cmd);
}

static void walk_help_param_value_length_cb(cli_tree_data_type_t const tree_data_type, char const * tree_name,
                                   void * const data_tree, void * const context)
{
    (void)tree_name;

    if (data_tree != NULL)
    {
        switch (tree_data_type)
        {
        case cli_tree_data_type_group:
        case cli_tree_data_type_action:
        case cli_tree_data_type_show:
            break;
        case cli_tree_data_type_element:
        case cli_tree_data_type_action_element:
            {
                cli_cmd_t * const cmd = context;
                cli_t * const cli = cmd->context;

                cli_tree_search_t * const search_tree = (tree_data_type == cli_tree_data_type_element) ?
                                                        cmd->group->search_tree:
                                                        cmd->action->search_tree;

                char * const value = group_element_value (cli,
                                                    cmd->action,
                                                    cmd->group, cli->cmd_instance,
                                                    data_tree, 1);

                size_t const max_length = (value != NULL) ? strlen(value) : 0;

                SET_MAX(search_tree->max_value_length, max_length);

                free(value);

            }
            break;
        case cli_tree_data_type_enum:
            break;
        case cli_tree_data_type_COUNT:
            break;
        }

    }
}


static void walk_help_param_cb(cli_tree_data_type_t const tree_data_type, char const * tree_name,
                                  void * const data_tree, void * const context)
{

    if (data_tree != NULL)
    {
        switch (tree_data_type)
        {
        case cli_tree_data_type_group:
            {
                cli_cmd_t * const cmd = context;
                cli_tree_data_t const * const tree = data_tree;
                cli_tree_search_t const * const search_tree = (cmd->action != NULL) ?
                                                            cmd->action->search_tree:
                                                            cmd->group->search_tree;
                config_mgr_group_t const * const group = tree->data;
                print_line(cmd->context, group->name, search_tree->max_name_length, group->description, screen_length.desc_length);
            }
            break;
        case cli_tree_data_type_show:
        case cli_tree_data_type_action:
            break;
        case cli_tree_data_type_action_element:
        case cli_tree_data_type_element:
            {
                cli_cmd_t * const cmd = context;
                cli_tree_search_t const * const search_tree = (tree_data_type == cli_tree_data_type_action_element) ?
                                                            cmd->action->search_tree :
                                                            cmd->group->search_tree;

                cli_tree_data_t const * const tree = data_tree;

                switch (tree->tree_data_type)
                {
                    case cli_tree_data_type_group:
                    {
                        config_mgr_group_t const * const group = tree->data;
                        print_line(cmd->context, group->name, search_tree->max_name_length, group->description, screen_length.desc_length);
                        cli_tree_walk_subtree(tree->search_tree, NULL, walk_help_param_cb, cmd);
                        break;
                    }
                    case cli_tree_data_type_action_element:
                    {
                        config_mgr_action_element_t const * const element = tree->data;

                        print_line(cmd->context, element->name, search_tree->max_name_length, element->description, screen_length.desc_length);
                        break;
                    }
                    case cli_tree_data_type_element:
                    {
                        config_mgr_element_t const * const element = tree->data;
                        print_line(cmd->context, element->name, search_tree->max_name_length, element->description, screen_length.desc_length);
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        case cli_tree_data_type_enum:
            break;
        case cli_tree_data_type_COUNT:
            break;
        }

    }

}

static void walk_help_parm_enum_cb (cli_tree_data_type_t const tree_data_type, char const * tree_name,
                             void * const data_tree, void * const context)
{
    cli_tree_data_t * const tree = data_tree;

    (void) tree_name;

    if (data_tree != NULL)
    {
        cli_cmd_t * const cmd = context;

        if (tree_data_type == cli_tree_data_type_enum)
        {
            config_mgr_element_enum_t const * const enum_value = tree->data;
            cli_printf(cmd->context, "%s%s", cmd->walking_enum == 1 ? "" : ", ", enum_value->name);
            cmd->walking_enum = 2;
        }
        else
        {
            cli_debug("Invalid enum type %d\n", tree_data_type);
        }
    }
}

static void print_group_syntax(cli_t * const context, cli_cmd_t * const cmd, config_mgr_group_t const * const group)
{
    static const char help_value_str[] = "<parameter> <value>";
    static const char help_parameter_str[] = "<parameter>";
    char const * help_string = help_value_str;

    cli_printf(context, "\r\n%s:\r\n", syntax_string);

    if (cmd->action != NULL)
    {
        if ((group->instances == 1))
            cli_printf(context, "%s %s [1]\r\n", cmd->action->name, group->name);
        else if (cmd->instance != INVALID_INSTANCE)
            cli_printf(context, "%s %s %d\r\n", cmd->action->name, group->name, cmd->instance);
        else
            cli_printf(context, "%s %s <1 - %d>\r\n",cmd->action->name, group->name, group->instances);
    }
    else
    {

        if (context->access == config_mgr_access_level_read_only)
            help_string = help_parameter_str;

        if ((group->instances == 1))
            cli_printf(context, "%s [1] %s\r\n", group->name, help_string);
        else if (cmd->instance != INVALID_INSTANCE)
            cli_printf(context, "%s %d %s\r\n", group->name, cmd->instance, help_string);
        else
            cli_printf(context, "%s <1 - %d> %s\r\n", group->name, group->instances, help_string);
    }

}

void help_group (cli_t * const context, cli_cmd_t * const cmd, char const * const buf)
{
    bool show_values = true;

    cli_tree_data_t const * group_tree = cmd->group;
    config_mgr_group_t const * const group = group_tree->data;
    cli_tree_search_t * search_tree = group_tree->search_tree;

    /* This prints in alphabetical order. */
    if (cmd->instance == INVALID_INSTANCE && !STRING_IS_EMPTY(buf))
    {
		unsigned int instance;

		if (get_instance(buf, &instance) == true)
		{
			if (instance > 0 && instance <= group->instances)
                cmd->instance = instance;
        }
		else if (group->instances == 1)
			cmd->instance = 1;
    }

    if (cmd->instance == INVALID_INSTANCE)
        show_values = false;


    cli_printf(context, "\r\n%s\r\n", group->description);
    print_group_syntax(context, cmd, group);


    if (cmd->cmd_type == cli_cmd_show)
    {
        cli_printf(context, "\r\n\r\nStatus/Statistics Elements:\r\n\r\n");
        cli_printf(context, " %-*s", search_tree->max_name_length, "Elements");
        show_values = false;
    }
    else
    {
        cli_printf(context, "\r\nAvailable Parameters:\r\n");
        cli_printf(context, " %-*s", search_tree->max_name_length, parameter_string);
    }

    if (show_values)
    {
        SET_TREE_VALUE_LENGTH(group_tree, sizeof current_value_string + 2);

        context->cmd_instance = cmd->instance;
        cli_tree_walk_subtree(group_tree->search_tree, buf, walk_help_param_value_length_cb, cmd);
        SET_MAX(search_tree->max_value_length, MIN_NAME_LENGTH);

        SET_TREE_VALUE_LENGTH(group_tree, search_tree->max_value_length + 2);

        append_line(context, search_tree->max_name_length,
                    current_value_string, search_tree->max_value_length,
                    description_string, description_max_row_length);

    }
    else
    {
        cli_printf(context, " %-.*s", screen_length.desc_length, description_string);
    }

    {
        size_t spaces = (search_tree->max_name_length + screen_length.desc_length);
        if (show_values)
            spaces += search_tree->max_value_length;

        cli_printf(context,"\r\n%.*s", (spaces < context->width) ? spaces : context->width , dash_string);
    }

    if (show_values)
    {
        context->cmd_instance = cmd->instance;
        cli_tree_walk_subtree(group_tree->search_tree, buf, walk_help_param_value_cb, cmd);
    }
    else
        cli_tree_walk_subtree(group_tree->search_tree, buf, walk_help_param_cb, cmd);

    cli_printf(context, "\r\n\r\n");
}

static void print_valid_values(cli_t * const context, cli_cmd_t * const cmd, cli_tree_data_t const * element_tree)
{
    config_mgr_element_t const * const element = element_tree->data;

    if (element->type != config_mgr_element_type_none)
	{
		char const valid_value_string[] = "Valid Values     : ";
        cli_printf(context, "\r\n%s", valid_value_string);

		switch (element->type)
		{
			case config_mgr_element_type_int32:
			case config_mgr_element_type_index:
				if (element->value_limit)
				{
					config_mgr_element_signed_integer_limit_t * limit = (config_mgr_element_signed_integer_limit_t *) element->value_limit;
					cli_printf(context, "Integer in the range of %d to %d", limit->min_value, limit->max_value);
				}
				else
					cli_printf(context, "Integer");
				break;

			case config_mgr_element_type_uint32:
			case config_mgr_element_type_counter32:
				if (element->value_limit)
				{
					config_mgr_element_unsigned_integer_limit_t * limit = (config_mgr_element_unsigned_integer_limit_t *) element->value_limit;
					cli_printf(context, "Integer in the range of %u to %u", limit->min_value, limit->max_value);
				}
				else
					cli_printf(context, "Integer");
				break;

			case config_mgr_element_type_string:
			case config_mgr_element_type_password:
            case config_mgr_element_type_encpassword:
				if (element->value_limit)
				{
					config_mgr_element_string_limit_t * limit = (config_mgr_element_string_limit_t *) element->value_limit;
					cli_printf(context, "String from %zu to %zu characters", limit->min_length_in_bytes, limit->max_length_in_bytes);
				}
				else
					cli_printf(context, "String");
				break;

			case config_mgr_element_type_mac_addr:
				cli_printf(context, "MAC address in format aa:bb:bb:dd:ee:ff");
				break;

			case config_mgr_element_type_counter64:
				if (element->value_limit)
				{
					config_mgr_element_string_limit_t * limit = (config_mgr_element_string_limit_t *) element->value_limit;
					cli_printf(context, "Counter64 string form from %zu to %zu characters", limit->min_length_in_bytes, limit->max_length_in_bytes);
				}
				else
					cli_printf(context, "Counter64 string");
				break;

			case config_mgr_element_type_fqdn:
				cli_printf(context, "Fully qualified domain name or IP address in dotted decimal notation (e.g. 192.168.1.1)");
				break;

			case config_mgr_element_type_ipaddr:
				cli_printf(context, "IP address in dotted decimal notation (e.g. 192.168.1.1)");
				break;

			case config_mgr_element_type_on_off:
				cli_printf(context, "on, off");
				break;

			case config_mgr_element_type_boolean:
				cli_printf(context, "true, false");
				break;

            case config_mgr_element_type_enum_multi:
			case config_mgr_element_type_enum:
				cmd->walking_enum = 1;
				cli_tree_walk_subtree(element_tree->search_tree, NULL, walk_help_parm_enum_cb, cmd);
				if (element->type == config_mgr_element_type_enum_multi)
				    cli_printf(context, "%.*s Multiple values must be separated with a comma.\n", strlen(valid_value_string), " ");
				break;

			case config_mgr_element_type_interface:
            case config_mgr_element_type_none:
                break;

		}
	}
}

void help_action(cli_t * const context, cli_cmd_t * const cmd, char const * const buf)
{
    cli_tree_data_t const * const action_tree = cmd->action;
    cli_tree_search_t const * const search_tree = action_tree->search_tree;
    cli_tree_data_t * element_tree = NULL;
    config_mgr_action_t const * const action = action_tree->data;

    //debug_printf("%s\n", "action_tree->name");
    if (!STRING_IS_EMPTY(buf))
    {
        void * ptr;
        cli_tree_data_type_t ptr_type;


        if ((cli_tree_find_subtree(action_tree->search_tree, buf, &ptr, &ptr_type) == 0) &&
            (ptr != NULL))
        {
            /* found it */
            element_tree = ptr;
            if (element_tree->tree_data_type == cli_tree_data_type_element)
            {
                config_mgr_action_element_t const * const element = element_tree->data;
                print_line(context, element->name, search_tree->max_name_length, element->description, screen_length.desc_length);
            }
        }
    }

    cli_printf(context, "\r\n%s\r\n", action->description);

    cli_printf(context, "\r\n%s:", syntax_string);

    if (action->syntax != NULL)
    {
        cli_printf(context, " %s %s", action->name, action->syntax);

    }
    else if (element_tree == NULL || element_tree->tree_data_type != cli_tree_data_type_element)
    {
        size_t i;

        cli_printf(cmd->context, "\r\n %s", action->name);

        for (i=0; i < action->elements.count; i++)
        {
            config_mgr_action_element_t const * const action_element = &action->elements.element[i];
            switch (action_element->type)
            {
            case config_mgr_action_group:
                break;
            case config_mgr_action_element:
                if (i == (action->elements.count -1))
                    cli_printf(cmd->context, " %s",  action_element->name);
                else
                    cli_printf(cmd->context, " %s |",  action_element->name);
                    break;
            case config_mgr_action_parameter:
                cli_printf(cmd->context, " [%s]",  action_element->name);
                break;
            }
        }
        cli_printf(cmd->context, "\r\n");

#if CONFIG_MGR_MAX_ACTION_PARAMETER_COUNT
        if (cmd->parameter[0] != NULL)
            cli_printf(cmd->context, " %s", cmd->parameter[0]->name);
#endif
    }
    else
        cli_printf(context, " %s %s", action_tree->name, element_tree->name);

    cli_printf(context, "\r\n\r\nAvailable Parameters:\r\n\r\n");
    print_line(context, parameter_string, search_tree->max_name_length, description_string, screen_length.desc_length);

    {
        size_t spaces = (search_tree->max_name_length + screen_length.desc_length);

        cli_printf(context,"\r\n%.*s", spaces, dash_string);
    }

    if (element_tree == NULL || element_tree->tree_data_type != cli_tree_data_type_element)
         cli_tree_walk_subtree(action_tree->search_tree, NULL, walk_help_param_cb, cmd);

    cli_printf(context, "\r\n\r\n");
}


void help_action_element (cli_t * const context, cli_cmd_t * const cmd)
{
    cli_tree_data_t const * const action_tree = cmd->action;
    cli_tree_data_t const * const element_tree = cmd->element;

    //debug_printf("%s\n", "action_tree->name");

    if (element_tree->tree_data_type == cli_tree_data_type_group)
    {
        cli_printf(context, "\r\n%s:", syntax_string);
        cli_printf(context, "\r\n %s <config>", action_tree->name);
        cli_printf(context, "\r\n\r\nAvailable Configuration and Status:\r\n");
        cli_printf(context, "\r\n %-*s %-*s", screen_length.name_length, "Configuration",
                                                screen_length.desc_length, description_string);

        {
            size_t spaces = (screen_length.name_length + screen_length.desc_length);

            cli_printf(context,"\r\n%.*s", spaces, dash_string);
        }

        cli_tree_walk_subtree(element_tree->search_tree, NULL, walk_help_param_cb, cmd);

    }
    else
    {
        help_action(context, cmd, element_tree->name);
    }

    cli_printf(context, "\r\n\r\n");

}

void help_group_element (cli_t * const context, cli_cmd_t * const cmd)
{
    static char default_value_string[] = "Default value";
    static char value_str[] = "<value>";

    size_t spaces = sizeof syntax_string;

    char * value_help_str = value_str;

    cli_tree_data_t const * const group_tree = cmd->group;
    cli_tree_data_t const * const element_tree = cmd->element;
    config_mgr_element_t const * const element = element_tree->data;

    SET_TREE_VALUE_LENGTH(group_tree, 0);

    SET_MAX(spaces, sizeof default_value_string);
    SET_MAX(spaces, sizeof description_string);
    SET_MAX(spaces, sizeof current_value_string);
    spaces += 2;  // extra 2 spaces

    if (cmd->cmd_type != cli_cmd_show)
    {
        if (context->access == config_mgr_access_level_read_only)
        {
            value_help_str = (char *)empty_str;
        }
        cli_printf(context, "\r\n%-*s :", spaces, syntax_string);

        if (cmd->instance != INVALID_INSTANCE)
        {
            cli_printf(context, " %s %d %s %s", group_tree->name, cmd->instance, element_tree->name, value_help_str);
        }
        else
        {
            config_mgr_group_t const * const group = group_tree->data;
            if (group->instances == 1)
                cli_printf(context, " %s [1] %s %s", group_tree->name, element_tree->name, value_help_str);
            else
                cli_printf(context, " %s <1 - %d> %s %s", group->name, group->instances, element->name, value_help_str);
        }
    }

    cli_printf(context, "\r\n%-*s : %s", spaces, description_string, element->description);

    if (cmd->instance != INVALID_INSTANCE)
    {
        cli_printf(context, "\r\n%-*s :", spaces, current_value_string);
        print_group_element_value(context, cmd->action, group_tree, cmd->instance, element_tree, 1);
    }

    if (cmd->cmd_type != cli_cmd_show)
        print_valid_values(context, cmd, element_tree);

    if (cmd->instance != INVALID_INSTANCE)
    {
        cli_printf(context, "\r\n%-*s :", spaces, default_value_string);
        print_group_element_value(context, cmd->action, group_tree, cmd->instance, element_tree, 0);
    }

    cli_printf(context, "\r\n\r\n");

}

void help_all (cli_t * const context, cli_cmd_t * const cmd, char const * const buf)
{
    cb_data.context = context;
    cb_data.cmd = cmd;

    cli_printf(context, "Available Commands:\r\n");

    if (STRING_IS_EMPTY(buf))
    {
        cli_tree_walk(cmd->tree_type, NULL, walk_help_cmd_cb, cmd);
    }
    else
    {
        cli_tree_walk(cmd->tree_type, buf, walk_help_cmd_cb, cmd);
    }

    cli_printf(context, "\r\n\r\n");
}
