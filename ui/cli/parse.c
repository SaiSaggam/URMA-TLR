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
#include "firewall_utils.h"
#include "tlr_common.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include "config_mgr.h"

#include "cli.h"
#include "cli_cmd.h"

#define skip_spaces(buf) while (*buf == ' ') buf++

static int check_for_comment (char const * const line) {
	int ret = 0;
	switch (line[0]) {
		case '!':
		case '#':
			ret = 1;
	}
	return ret;
}

/*
    get_instance:
    return true if the string is numeric. If so, the numeric value of the string will be placed into value
    return false if there are any non-numeric chars in the string. value is unchanged in that case
    presumes that the string isn't empty.
*/
bool get_instance(char const * const string, unsigned int * const value)
{
	bool is_numeric = true;
	int instance = 0;
	size_t nb_chars = strlen(string);

	for (size_t i = 0; i < nb_chars; i++)
	{
		char digit = string[i];

		if (!isdigit((int)digit))
		{
			is_numeric = false;
			goto done;
		}
		instance *= 10;
		instance += digit - '0';
	}
	*value = instance;

done:
	return is_numeric;
}

static void print_parse_status(cli_t *context, int status, cli_cmd_t *cmd)
{
    switch (status)
    {
        case PARSE_INVALID_CMD:
            cli_printf(context, "\r\n## Invalid command. Type '?' for help.\r\n");
            break;

        case PARSE_INVALID_GROUP:
            cli_printf(context, "\r\n## Invalid group. Type '?' for help.\r\n");
            break;

        case PARSE_INVALID_PARAM:
            cli_printf(context, "\r\n## Invalid element. ");

			cli_printf(context, "Type '");
			if (cmd->action != NULL)
                cli_printf(context, "%s ", cmd->action->name);

            if (cmd->group != NULL)
            {
                cli_printf(context, "%s ", cmd->group->name);
                if (cmd->instance != INVALID_INSTANCE)
                    cli_printf(context, "%d ", cmd->instance);
            }

            cli_printf(context, "?' for help.\r\n");
            break;

		case PARSE_INVALID_INSTANCE:
            if (cmd->action != NULL)	/*e.g. show eth 123*/
                cli_printf(context, "\r\n## Invalid instance. Type '%s %s ?' for help.\r\n", cmd->action->name, cmd->group->name);
            else						/*e.g. eth 123*/
                cli_printf(context, "\r\n## Invalid instance. Type '%s ?' for help.\r\n", cmd->group->name);
            break;

        case PARSE_SYSTEM_CONFIG_ERROR:
            cli_printf(context, "\r\n## System-level error\r\n");
            break;

        default:
            /* For error code, error should already be displayed. */
            break;
    }
}

static cli_cmd_type_t find_command(cli_cmd_t * const cmd, char const * const command_name)
{
    cli_cmd_type_t result = cli_cmd_none;
    cli_tree_data_t * tree;

    {
        void * ptr = NULL;
        cli_tree_data_type_t ptr_type;

        cli_tree_find(cmd->tree_type, command_name, &ptr, &ptr_type);
        tree = ptr;
    }

//    tree_debug("tree_type %d, parse_buf %s, len %d, group %s, data_type %d, instances %u\n",
//            cmd->tree_type, parse_buf, strlen (parse_buf), cmd->group->name, cmd->group->data_type, cmd->group->instances);

    if (tree == NULL)
        goto done;

    switch (tree->tree_data_type)
    {
        case cli_tree_data_type_show:
            cmd->action = tree;
            result = cli_cmd_show;
            break;

		case cli_tree_data_type_action:
            cmd->action = tree;
            result = cli_cmd_action;
            break;

		case cli_tree_data_type_group:
            cmd->group = tree;
            result = cli_cmd_set;
            break;

        default:
            break;
    }

done:
    return result;
}

static cli_parse_status_t handle_command(cli_t * const context, cli_cmd_t * const cmd, char const * const buffer)
{
    //cli_status_t status = cli_success;
    cli_parse_status_t result = PARSE_OK;
    char const * value = buffer;
    config_mgr_group_t const * const  group = (cmd->group != NULL) ? cmd->group->data : NULL;
    config_mgr_element_t const * const  element = (cmd->element != NULL) ? cmd->element->data : NULL;
    config_mgr_action_t const * const  action = (cmd->action != NULL) ? cmd->action->data : NULL;

    skip_spaces(value);

    /* XXX why are semicolons to be treated as errors? We have some
     * commands (e.g. firewall commands) that may or may not support
     * semicolons. It seems odd to be placing this restriction here.
     * Presumably it's here as some kind of security workaround
     * (i.e. to prevent access to the bash shell), but that should
     * be prevented by not using popen() in the first place where
     * the user has control over some of the input to popen().
     */
    if (strchr(buffer, ';') != NULL)
    {
        result = PARSE_INVALID_CMD;
        goto done;
    }

    if ( (context->flag & CLI_FLAG_ACTIONONLY) && (cmd->cmd_type != cli_cmd_action) )
        goto done;

    switch (cmd->cmd_type)
    {
        case cli_cmd_action:

            if (action != NULL)
            {
                if ((context->flag & CLI_FLAG_BOOT) == 0)
                {
                    if (process_action_command(context, action->id, buffer) != 0)
                        result = PARSE_SYSTEM_ACTION_ERROR;
                }
            }
            else
            {
                result = PARSE_INVALID_CMD;
            }
            break;

        case cli_cmd_set:

            if (group == NULL)
            {
                /* should not come here since group is 1st argument. Added it for Coverity */
                result = PARSE_INVALID_GROUP;
            }
            else if (cmd->instance != INVALID_INSTANCE && element != NULL && !STRING_IS_EMPTY(value))
            {
                /*e.g. eth 1 description 123*/
                char const * error_string = NULL;
                char * pvalue = (char *)value;
                size_t const length = strlen(value);
                cli_status_t status;

                /*strip leading and trailing double quotes*/
                if (pvalue[0] == '\"')
                {
                    if (pvalue[length - 1] == '\"')
                        pvalue[length - 1] = '\0';
                    pvalue++;
                }

//            if (strlen(pvalue) == 0)
//                pvalue = SPACE_CHAR;

                /* for set configuration, instance, element and value must be specified */
                status = cli_cmd_set_element(context, group, cmd->instance, element->id, pvalue, &error_string);
                switch (status)
                {
                    case cli_success:
                        if ((context->flag & CLI_FLAG_BOOT) == 0)
                        {
                            /* TODO: Call apply process even if configuration doesn't change?? */
                            if (cli_apply_config(context, group->id, cmd->instance) != 0)
                            {
                                result = PARSE_SYSTEM_APPLY_ERROR;
                            }
                        }
                        break;
                    case cli_auth_error:
                    case cli_parse_error:
                    case cli_config_error:
                    case cli_resource_error:
                        if (error_string != NULL)
                            cli_printf(context, "## %s. Type '%s %d %s ?' for help.\r\n", error_string, group->name, cmd->instance, element->name);
                        else
                            cli_printf(context, "## Invalid element value. Type '%s %d %s ?' for help %d.\r\n", group->name, cmd->instance, element->name, status);
                        result = PARSE_SYSTEM_SET_ERROR;
                        break;

                    case cli_apply_error:
                        result = PARSE_SYSTEM_APPLY_ERROR;
                        break;

                }
            }
            else    /*user has entered a less than complete command. In that case display values*/
            {
                if (cmd->element != NULL)   /*e.g. eth 1 description\n*/
                {
                    /*display the current parameter value*/
                    char * element_value = NULL;
                    if (cli_cmd_get_element(context, group, cmd->instance, element->id, &element_value) != cli_success)
                        result = PARSE_INVALID_PARAM_VALUE;
                    else
                        cli_printf(context, "\r\n %s\r\n\r\n", element_value);

                    free(element_value);
                    goto done;
                }
                if (cmd->instance != INVALID_INSTANCE)
                {
                    /*e.g. eth 1. Display all parameter values for that instance*/
                    if (!STRING_IS_EMPTY(value))
                    {
                        result = PARSE_INVALID_PARAM;
                        cli_printf(context, "\n");
                    }
                    else
                    {
                        cli_cmd_print_all_elements_in_group(context, cmd);
                        cli_printf(context, "\n\n");
                    }
                }
                else
                {
                    /* get all instances of the group */
                    if (!STRING_IS_EMPTY(value))
                    {
                        result = PARSE_INVALID_PARAM_VALUE;
                    }
                    else
                    {
                        for (cmd->instance = 1; cmd->instance <= group->instances; cmd->instance++)
                        {

                            cli_printf(context, "\n %s %d:", group->name, cmd->instance);
                            cli_cmd_print_all_elements_in_group(context, cmd);
                            cli_printf(context, "\n");
                        }
                    }
                    cli_printf(context, "\n");
                }

            }
            break;
        case cli_cmd_show:
            if ((action == NULL) || (group == NULL))
            {
                cli_printf(context, "## Invalid group for show command\r\n");
                result = PARSE_INVALID_PARAM;
                goto done;
            }
            /*
                If the instance hasn't already been set, set to 1 for singleton, else leave as-is
            */
            if (cmd->instance == INVALID_INSTANCE)
            {
                if (group->instances == 1)
                {
                    cmd->instance = 1;
                }
                else
                {
                    // Pass an instance of zero if none is specified, this is used by
                    // show processes which print out a summary when no instance is specified.
                    cmd->instance = 0;
                }
            }
            if (process_show_command(context, group->id, cmd->instance, buffer) != 0)
                result = PARSE_SYSTEM_SHOW_ERROR;

            break;
        case cli_cmd_none:
            result = PARSE_INVALID_CMD;
            break;
    }

done:
    return result;
}


void reset_parse_buffer(char * const parse_buf, int * index)
{
    memset (parse_buf, 0, MAX_COMMAND_LINE);
    *index = 0;
}

static int handle_auto_complete(cli_t * const cli, cli_cmd_t * const cmd, char * string, int autohelp)
{
    int ret = 0;

    /* Auto-complete */
    /* We can try to auto-complete as much as we can */
    switch (cmd->cmd_type)
    {
    case cli_cmd_none:
        ret = autocomplete_cmd (cli, cmd, string, autohelp);
        break;
    case cli_cmd_action:
    case cli_cmd_show:
        if (cmd->group == NULL)
        {
            if (cmd->element == NULL)
                autocomplete_element (cli, cmd, cmd->action, string);
            break;
        }
        if (cmd->cmd_type == cli_cmd_action) break;
        /* no break; */
    default:
        if((cmd->element == NULL) || (cmd->cmd_type == cli_cmd_show))
        {
            config_mgr_group_t const * const this_group = cmd->group->data;

            if (this_group->instances == 1)
                cmd->instance = 1;

            if (cmd->instance == INVALID_INSTANCE)
            {
                autocomplete_instance(cli, cmd);
            }
            else
            {
                autocomplete_element(cli, cmd, cmd->group, string);
            }
        }
        else
        {
            config_mgr_element_t const * const element = cmd->element->data;
            switch (element->type)
            {
            case config_mgr_element_type_enum:
                autocomplete_value (cli, cmd, string);
                break;
            case config_mgr_element_type_enum_multi:
                autocomplete_multi_value (cli, cmd, string);
                break;
            case config_mgr_element_type_on_off:
                autocomplete_on_off (cli, cmd, string);
                break;
            case config_mgr_element_type_boolean:
                autocomplete_boolean (cli, cmd, string);
                break;
            default:
                autocomplete_boolean (cli, cmd, string);
                break;
            }
        }
        break;
    }

    return ret;


}

static int handle_help(cli_t * const cli, cli_cmd_t * const cmd, char * string)
{
    int result = PARSE_HELP;

    switch (cmd->cmd_type)
    {
    case cli_cmd_none:
        help_all (cli, cmd, string);
        break;
    case cli_cmd_action:
    case cli_cmd_show:

        if (cmd->group == NULL)
        {
            if (cmd->element != NULL)
            {
                help_action_element(cli, cmd);
            }
            else if (cmd->action != NULL)
            {
                help_action(cli, cmd, string);
            }
        }
        else
        {
            help_group (cli, cmd, "");
        }

        break;

    default:
        if (cmd->element != NULL)
        {
            help_group_element (cli, cmd);
        }
        else if (cmd->group != NULL)
        {
            help_group (cli, cmd, string);
        }
        break;
    }

    return result;
}

static cli_parse_status_t parse_action_argument(cli_t * const cli, cli_cmd_t * const cmd, char const * string)
{
    cli_parse_status_t result = PARSE_OK;
    void * ptr;
    cli_tree_data_type_t ptr_type;

    if (cli_tree_find_subtree(cmd->action->search_tree, string, &ptr, &ptr_type) != 0)
    {
        /*
         * XXX This seems kludgy
        */
        if (strcmp(cmd->action->name, "show") == 0)
        {
            result = PARSE_INVALID_PARAM;
            goto done;
        }
    }

    if (ptr != NULL)
    {
        switch (ptr_type)
        {
        case cli_tree_data_type_group:
            cmd->group = ptr;
            break;

        case cli_tree_data_type_element:
            cmd->element = ptr;
            break;

        case cli_tree_data_type_action_element:
            debug_printf("ADD CODE TO HANDLE ACTION PARAMETER\n");
            break;

        default:
            cli_error(cli, "Invalid type %d for %s under action %s\n", ptr_type, string, cmd->group->name);
            result = PARSE_INVALID_PARAM;
            goto done;
        }
    }

done:
    return result;
}

static cli_parse_status_t parse_element_argument(cli_t * const cli, cli_cmd_t * const cmd, char * string)
{
    cli_parse_status_t result = PARSE_OK;
    void * ptr;
    cli_tree_data_type_t ptr_type;

    if ((cli_tree_find_subtree(cmd->group->search_tree, string, &ptr, &ptr_type) != 0) ||
        (ptr == NULL))
    {
        /* No match */
        result = PARSE_INVALID_PARAM;
        goto done;
    }

    switch (ptr_type)
    {
        case cli_tree_data_type_action:
        case cli_tree_data_type_group:
            cli_error(cli, "Invalid type %d on %s under %s\n", ptr_type, string, cmd->group->name);
            result = PARSE_INVALID_PARAM;
            goto done;

        case cli_tree_data_type_element:
            cmd->element = ptr;
            break;

        case cli_tree_data_type_action_element:
            /* TODO */
            debug_printf("HANDLE PARAMETER ARGUMENT\n");
            break;

        case cli_tree_data_type_enum:
            cli_error(cli, "Invalid ENUM %s under %s\n", string, cmd->group->name);
            result = PARSE_INVALID_PARAM;
            goto done;
        case cli_tree_data_type_show:
        case cli_tree_data_type_COUNT:
            break;
    }

done:
    return result;
}

static cli_parse_status_t find_cmd_type(cli_t * const cli, cli_cmd_t * const cmd, char * const buffer)
{
    cli_parse_status_t result;

    cmd->cmd_type = find_command(cmd, buffer);
    if (cmd->cmd_type == cli_cmd_none)
    {
        if (!STRING_IS_EMPTY(buffer))
        {
            if(autocomplete_cmd(cli, cmd, buffer, 1) == 0)
            {
                result =  PARSE_AUTOCOMPLETE;
                goto done;
            }
            else
            {
                /* No match */
                result = PARSE_INVALID_CMD;
                goto done;
            }
        }
        else
        {
            /* No match */
            result = PARSE_INVALID_CMD;
            goto done;
        }
    }
    else if ((cmd->action != NULL) && (strcmp(cmd->action->name, "show") == 0))
    {
        // XXX these comparisons with the magic string
        // "show" seem bad. why isn't cmd->cmd_type == cli_cmd_show
        // already?
        cmd->cmd_type = cli_cmd_show;
    }

    result = PARSE_OK;

done:
    return result;

}

static char * strip_leading_spaces(char * const input_line)
{
    char * line = input_line;

    /* Skip past any leading spaces */
    while (*line == ' ')
    {
        line++;
    }

    return line;
}

static void strip_trailing_spaces(char * const input_line)
{
    char * last_char = input_line + strlen(input_line) - 1;

    while (last_char >= input_line && *last_char == ' ')
    {
        *last_char = '\0';
        last_char--;
    }
}

static char * strip_leading_and_trailing_spaces(char * const input_line)
{
    char * line;

    line = strip_leading_spaces(input_line);
    strip_trailing_spaces(line);

    return line;
}

static bool check_for_firewall_input(char const * const line, uint32_t * flag)
{
    bool result;

    if (firewall_line_is_start_tag(line))
    {
        *flag |= CLI_FLAG_FIREWALL_SECTION;
        result = true;
        goto done;
    }
    if (firewall_line_is_end_tag(line))
    {
        *flag &= ~CLI_FLAG_FIREWALL_SECTION;
        result = true;
        goto done;
    }
    /* Ignore the firewall section */
    if (*flag & CLI_FLAG_FIREWALL_SECTION)
    {
        result = true;
        goto done;
    }
    result = false;

done:
    return result;
}

/**
 * parse_input
 *
 */
cli_parse_status_t parse_input (cli_t * cli, int * position)
{
    /* XXX - parse_buf needs to be at least as large as cli->line. */
    char parse_buf[MAX_COMMAND_LINE];
    char * p;
    char * line;

    /* XXX - This monster function should be split up/simplified so
     * that it's easier to follow.
     * Also, all the special handling to automatically determine the
     * instance number (in the case of singleton commands) should be
     * simplified.
     * It should be made more obvious what this 'position' variable
     * is all about as well.
     */
    cli_cmd_t cmd = {
//            .cmd_name = empty_cmd_name,
            .cmd_type = cli_cmd_none,
            .tree_type = cli_tree_type_command,
            .instance = INVALID_INSTANCE,
            .action = NULL,
            .group = NULL,
            .element = NULL,
            .parameter_count = 0,
            .context = cli
    };

    int i;
    int nArgs = 0;
    cli_parse_status_t result = PARSE_COMMENT;

    /* Note that we don't strip all whitespace because this
     * function seems to get called if the leading char is TAB,
     * and the cli handles these TAB chars differently.
     */
    line = strip_leading_and_trailing_spaces(cli->line);
    *position = line - cli->line;

    /* Is there any input to parse? */
    if (STRING_IS_EMPTY(line))
    {
        result = PARSE_EMPTY_CMD;
        goto done;
    }


    /* Check to see if input is a comment */
    if (check_for_comment(line))
    {
        result = PARSE_COMMENT;
        goto done;
    }

    /*
     * The firewall section contains a set of rules, these are
     * not CLI commands so we skip over them here, the rules
     * are restored in the restore_firewall process which is called
     * from S06execconfda0.sh
     */
    if (cli->flag & (CLI_FLAG_BOOT|CLI_FLAG_ACTIONONLY))
    {
        /* Should we check for the firewall tags whenever they appear,
         * and not just when the boot flag or action only flag are
         * set?
         */
        if (check_for_firewall_input(line, &cli->flag))
        {
            result = PARSE_FIREWALL;
            goto done;
        }
    }

	/* attempt to locate command, instance, parameter, value etc */
	reset_parse_buffer(parse_buf, &i);
    p = line;



    while (!STRING_IS_EMPTY(p))
    {
        switch (*p)
        {
        case '\t':
            handle_auto_complete(cli, &cmd, parse_buf, 0);
            result = PARSE_AUTOCOMPLETE;
            goto done;

        case '!':
            if(cmd.cmd_type == cli_cmd_set && cmd.instance != INVALID_INSTANCE && cmd.element != NULL && cmd.element->data != NULL)
            {
				/*
				    Beware! Don't use a pointer to a const string here as handle_command modifies the buffer
				    which results in a crash because handle_command modifies the buffer (even though it takes a pointer to const).
				*/
                /* XXX - How does using a double quote as a default value work? */
                char temp[4] = "\"";
				char const * default_value;
                config_mgr_element_t * real_element = cmd.element->data;

				if (real_element->default_value == NULL)
                {
					default_value = temp;
                }
                else
                {
					default_value = real_element->default_value;
                }
                handle_command(cli, &cmd, default_value);

				reset_parse_buffer(parse_buf, &i);
                result = PARSE_OK;
            }
            else	/*What, not able to use '!' in non-set commands? FIXME*/
            {
                result = PARSE_INVALID_CMD;
            }
			goto done;

		case '?':
		{
			int nb_args_for_help;

			if ((cmd.cmd_type == cli_cmd_action)
	            || (cmd.cmd_type == cli_cmd_show))
				nb_args_for_help = 1;
			else
			{
				nb_args_for_help = 2;	/*need at least <command> <instance>*/
				if (cmd.group != NULL)
				{
					config_mgr_group_t const * const group = cmd.group->data;

					if (group->instances == 1)	/*only need <command> in this case*/
						nb_args_for_help--;
				}
			}
			if (STRING_IS_EMPTY(parse_buf))
			{
				handle_help(cli, &cmd, parse_buf);
				/* don't want to show the name of the group in the action */
				if (cmd.element != NULL && cmd.element->tree_data_type == cli_tree_data_type_group)
				{
					(*position) -= strlen(cmd.element->name);
					if (((*position) > 0) && ((*(p-1) == ' ')))
						(*position)--;
				}
				result = PARSE_HELP;
				goto done;
			}
			else if (nArgs >= nb_args_for_help)
			{
				handle_help(cli, &cmd, parse_buf);
                if (((*position) > 0) && ((*(p-1) == ' ')))
                    (*position)--;
				result = PARSE_HELP;
				goto done;
			}
			else
			{
				p--;
				(*position)--;
			}
		}
            /* no break; */

        case ' ':
            if (STRING_IS_EMPTY(parse_buf))
            {
                result = PARSE_OK;
                break;
            }
            nArgs++;

#if (CLI_DEBUG == 1)
            {
                char const * const cli_cmd_strings[] = {
                "cli_cmd_none",
                "cli_cmd_set",
                "cli_cmd_show",
                "cli_cmd_action"
                };

                debug_printf("parse_input: argument %d \"%s\" (%d) action %p group %p instance %d element %p \n",
                            nArgs, cli_cmd_strings[cmd.cmd_type], cmd.cmd_type,
                            cmd.action, cmd.group, cmd.instance,
                            cmd.element);
            }
#endif
            switch (cmd.cmd_type)
            {
                case cli_cmd_none:
                    result = find_cmd_type(cli, &cmd, parse_buf);
                    if (result != PARSE_OK)
                    {
                        goto done;
                    }
                    reset_parse_buffer(parse_buf, &i);
                    break;
                case cli_cmd_set:
                case cli_cmd_show:

                    if (cmd.group == NULL)
                    {
                        /* expects another element, parameter or configuration (if it's group) */
                        result =  parse_action_argument(cli, &cmd, parse_buf);
                        if (result != PARSE_OK)
                            goto done;

                        reset_parse_buffer(parse_buf, &i);
                    }
                    else if (cmd.instance == INVALID_INSTANCE)
                    {
                        unsigned int instance;
                        config_mgr_group_t const * const group = cmd.group->data;

                        if (get_instance(parse_buf, &instance) == true)
                        {
                            if (!INSTANCE_IS_VALID(instance, group->instances))
                            {
                                result = PARSE_INVALID_INSTANCE;
                                goto done;
                            }
                            cmd.instance = instance;
                            /* Clear the parse buffer */
                            reset_parse_buffer(parse_buf, &i);
                        }
                        else if (group->instances == 1) /*user must have gone straight to next element*/
                        {
                            cmd.instance = 1;
                            continue;
                        }
                        else    /*non-numeric value specified when instance number is expected.*/
                        {
                            result = PARSE_INVALID_INSTANCE;
                            goto done;
                        }
                    }
                    else if (cmd.element == NULL)
                    {
                        /* Looks like we have found the parameter
                         * Try to match it against the parameter table
                         */
                        result = parse_element_argument(cli, &cmd, parse_buf);
                        /*
                            If a parameter name is not parsed for a set command, we break out now.
                            If we don't, we wind up accepting commands like "eth 1 2 3 description 1234" (TLR-783)
                        */
                        if (result != PARSE_OK)
                        {
                            if (cmd.cmd_type == cli_cmd_set)
                            {
                                result = PARSE_INVALID_PARAM;
                                goto done;
                            }
                        }

                        if (cmd.element != NULL)
                        {
                            reset_parse_buffer(parse_buf, &i);
                        }
                        else
                        {
                            parse_buf[i++] = *p;
                        }

                    }
                    else    /*accumulate the parameter value into parse_buf. The value may include spaces*/
                    {
                        config_mgr_element_t const * const element = cmd.element->data;

                        if (element->type == config_mgr_element_type_string)
                        {
                            parse_buf[i++] = *p;
                        }

                    }
                    break;

                case cli_cmd_action:
                    parse_buf[i++] = *p;
                    break;
            }
            break;

        default:
            parse_buf[i++] = *p;
            break;
        }

        p++;
        (*position)++;
    }

    //debug_printf("cmd_type %d, group_type %d, instance %u, group_name %s, element_name %s\n",
      //                   cmd.cmd_type, cmd.tree_type, cmd.instance,
      //                  cmd.group == NULL ? "NULL" : cmd.group->name,
      //                  cmd.element == NULL ? "NULL" : cmd.element->name);

    if (cmd.element != NULL)
    {
        config_mgr_element_t const * const element = cmd.element->data;

        if (element->access == config_mgr_access_writeonly)
        {
            size_t const len = strlen(line) - strlen(parse_buf);
            cli_info(cli, "%.*s***\n", (int)len, line);
        }
        else
        {
            cli_info(cli, "%s\n", line);
        }

    }

    if (!STRING_IS_EMPTY(parse_buf))	/*this will be everything after the parameter name for set commands*/
								/* occurs if the user enters something like "eth 1<ENTER>". parsebuf contains the last arg*/
    {
        switch (cmd.cmd_type)
        {
            case cli_cmd_none:
                cmd.cmd_type = find_command(&cmd, parse_buf);
                /* Clear the parse buffer */
                memset(parse_buf, 0, MAX_COMMAND_LINE);

                if (cmd.cmd_type == cli_cmd_show)
                {
                    result = PARSE_INCOMPLETE;
                    goto done;
                }
                break;

            case cli_cmd_show:
            case cli_cmd_action:
                /* if the instance hasn't been specified by the user, use instance 1 if this is a singleton command*/
                if ((cmd.group != NULL) && (cmd.instance == INVALID_INSTANCE))
                {
                    /* means that the user hasn't specified an instance
                     * (e.g. "show failover")
                     */
                    unsigned int instance;
                    config_mgr_group_t const * const group = cmd.group->data;

                    if (get_instance(parse_buf, &instance) == true)
                    {
                        if (!INSTANCE_IS_VALID(instance, group->instances))
                        {
                            result = PARSE_INVALID_INSTANCE;
                            goto done;
                        }
                        cmd.instance = instance;
                        reset_parse_buffer(parse_buf, &i);
                    }
                    else if (group->instances == 1)
                    {
                        cmd.instance = 1;
                    }
                    /*don't fail if an instance isn't specified (e.g. "show eth" == show all eth instances)*/
                }
                else if (cmd.group == NULL)
                {
                    /* expects another element, parameter or configuration (if it's group) */
                    result =  parse_action_argument(cli, &cmd, parse_buf);
                    if (result != PARSE_OK)
                        goto done;
                    /* It seems that for 'action' commands group will still be
                     * NULL at this point, so parse_buf won't get reset. This is
                     * exactly what we want. For 'show' style commands, group will
                     * be non-NULL, so parse_buf will get reset. This is also what
                     * we want.
                     */
                    if (cmd.group != NULL)
                    {
                        reset_parse_buffer(parse_buf, &i);
                    }
                }
                break;

            case cli_cmd_set:
                /* Parse instance if not specified.
                 * no space is typed after instance.
                 */
                if (cmd.instance == INVALID_INSTANCE)
                {
                    unsigned int instance;
                    config_mgr_group_t const * const group = cmd.group->data;

                    if (get_instance(parse_buf, &instance) == true)
                    {
                        if (!INSTANCE_IS_VALID(instance, group->instances))
                        {
                            result = PARSE_INVALID_INSTANCE;
                            goto done;
                        }
                        cmd.instance = instance;
                        /*parse_buf contained the instance, so clear it*/
                        reset_parse_buffer(parse_buf, &i);
                    }
                    else if (group->instances == 1)
                    {
                        cmd.instance = 1;
                    }
                    else    /*non-numeric value specified when instance number is expected.*/
                    {
                        result = PARSE_INVALID_INSTANCE;
                        goto done;
                    }
                }

                /* if parse_buf hasn't been cleared out getting the instance, check for a parameter name */
                if (!STRING_IS_EMPTY(parse_buf))
                {
                    /* Parse element if not specified
                     * no space is typed after element.
                     */
                    if (cmd.instance != INVALID_INSTANCE && cmd.element == NULL)
                    {
                        void * ptr;
                        cli_tree_data_type_t ptr_type;

                        result = parse_element_argument(cli, &cmd, parse_buf);
                        if (result != PARSE_OK)
                        {
                            goto done;
                        }

                        if (cli_tree_find_subtree(cmd.group->search_tree, parse_buf, &ptr, &ptr_type) != 0)
                        {
                            result = PARSE_SYSTEM_CONFIG_ERROR;
                            goto done;
                        }
                        reset_parse_buffer(parse_buf, &i);
                    }
                }
                break;
        }
    }

    result = handle_command(cli, &cmd, parse_buf);

done:
    print_parse_status(cli, result, &cmd);
    return result;
}
