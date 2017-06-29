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
#include "line_ed.h"
#include "cli_tree.h"
#include "cli_cmd.h"
#include "cli_timeout.h"
#include "strlcpy.h"
#include "tlr_format_string.h"
#include "tlr_common.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <syslog.h>

#ifdef _CUNIT_
#define STATIC
#else
#define STATIC static
#endif

screen_length_t screen_length;

STATIC cli_t *cli = NULL;

STATIC cli_t *init_cli (void);
STATIC cli_status_t cli_user_auth (cli_t *context);
STATIC void   add_command_to_history(cli_t *context);

static char const * cli_get_base_prompt(cli_t * const context)
{
    static char const default_prompt[] = "digi.router>";
    char * base_prompt;

    if (cli_get_config(context, "system", 1, "prompt", &base_prompt) != cli_success)
    {
        cli_debug("Failed to get CLI Prompt\n");
        base_prompt = strdup(default_prompt);
    }

    return base_prompt;
}

static void cli_get_formatted_prompt(char const * const base_prompt, char * const formatted_prompt, size_t formatted_prompt_size)
{
    (void)tlr_replace_percent_s_with_serial_number(base_prompt, formatted_prompt, formatted_prompt_size);
}

/**
 * Get the latest prompt from the Config Mgr
 */
void cli_get_prompt(cli_t * const context)
{
    bool cli_needs_prompt = CLI_IS_INTERACTIVE(context);

    context->prompt[0] = '\0';
    if (cli_needs_prompt)
    {
        char const * const base_prompt = cli_get_base_prompt(context);

        if (base_prompt != NULL)
        {
            cli_get_formatted_prompt(base_prompt, context->prompt, sizeof context->prompt);
            free((void *)base_prompt);
        }
    }
}

STATIC void signal_handle(int signo)
{
	switch (signo)
	{
	case SIGUSR1:
        cli_debug("received SIGUSR1\n");
        break;

#if 0
    /* posix doesn't allow SIGKILL to be caught, nor SIGNSTOP */
	case SIGKILL:
        printf("received SIGKILL\n");
        break;

	case SIGSTOP:
        printf("received SIGSTOP\n");
        break;
#endif

	case SIGINT:
        cli_debug("received SIGINT\n");
        break;

	default:
        cli_debug("received %d\n", signo);
        break;
	}
}

void terminate_signal_handle(int const signo)
{

	signal_handle(signo);

	if (cli != NULL)
    {
        cli_printf(cli, "Exit");
        os_end(cli);

		cli_tree_destroy();
        clear_command_history(cli);
        closelog();
        free (cli);
        cli = NULL;
    }

    abort();

}

#ifndef _CUNIT_
static void store_pid(char const * const serial)
{
    int fd;

    {
        char device[32];

        snprintf(device, sizeof device, "/var/run/%s.pid", serial);
        fd = open(device, O_WRONLY | O_CREAT | O_TRUNC);
    }

    if (fd >= 0)
    {
        char buffer[32];
        snprintf(buffer, sizeof buffer, "%d", getpid());

        ssize_t const bytes_written = write(fd, buffer, strlen(buffer));
        if (bytes_written <= 0)
        {
            cli_error(cli, "Unable to update pid %s\n", buffer);
        }

        close(fd);
    }
}

static void reset_pagination(cli_t * const cli)
{
	cli->more_lines = 0;
	cli->more_cancelled = false;
}
#endif

void cli_set_edit_position(cli_t * const cli, size_t const edit_position)
{
    cli->line[edit_position] = '\0';
    cli->editpos = edit_position;
}

#ifndef _CUNIT_
// Have to take out main() for unit testing..
/**
 * Main function for CLI
 */
int main(int argc, char * argv[])
{
    int result = 1;
    int opt;

    setuid(0);  //TODO: see TLR-1251

    openlog("CLI", 0, LOG_USER /* LOG_LOCAL5 */);

    cli = init_cli();
    if (cli == NULL)
    {
        cli_debug("init_cli failed\n");
        goto done;
    }

    while ((opt = getopt(argc, argv, "m:u:")) != -1)
    {
        static char serial_string[] = "serial";
        switch (opt)
        {
            case 'm':
                if (optarg == NULL)
                {
                    cli_printf(cli, "Invalid option (-m)\r\n");
                    goto error;
                }

                if (strcmp(optarg, "boot") == 0)
                {
                    cli->flag |= (CLI_FLAG_BOOT | CLI_FLAG_EOFEXIT);
                }
                else if (strstr(optarg, serial_string) != NULL)
                {
                    char * ptr = optarg + sizeof serial_string - 1;
                    char cmd[40];

                    snprintf(cmd, sizeof cmd, "/usr/bin/cli_set_serial -i %s", ptr);

                    system(cmd);
                    store_pid(optarg);
                    cli->flag |= CLI_FLAG_SERIAL;
                }
                else if (strcmp(optarg, "action") == 0)
                {
                    cli->flag |= (CLI_FLAG_ACTIONONLY | CLI_FLAG_EOFEXIT);
                }
                else if (strcmp(optarg, "non-interactive") == 0)
                {
                    cli->flag |= (CLI_FLAG_NON_INTERACTIVE | CLI_FLAG_EOFEXIT);
                    cli->more = 0;
                }
                else
                {
                    strlcpy(cli->source, optarg, sizeof cli->source);
                }
                break;
            case 'u':
                if (optarg != NULL)
                {
                    char * ptr = optarg;
                    char * user = strsep(&ptr, ":");

                    if (user == NULL)
                        break;

                    if (strlen(user) >= sizeof cli->user)
                    {
                        cli_printf(cli, "Username is too long\r\n");
                        goto error;
                    }

                    strcpy(cli->user, user);

                    if (strlen(ptr) >= sizeof cli->password)
                    {
                        cli_printf(cli, "Invalid password\r\n");
                        goto error;
                    }

                    strcpy(cli->password, ptr);
                }
                break;

            default:
                cli_printf(cli, "Invalid option %c\r\n", opt);
                goto error;
        }
    }

    os_init(cli);

    /* Start processing CLI requests from the user */
    signal(SIGINT, signal_handle);
    signal(SIGUSR1, terminate_signal_handle);

    if (cli_user_auth(cli) != cli_success)
    {
        cli_printf(cli, "User login failed.\n\n");
        goto end;
    }

    /* Populate a command tree */
    if (cli_tree_init(cli->access) != 0)
    {
        cli_printf(cli, "Cannot get Configurations. Check permission level.\n");
        goto end;
    }

    screen_length.name_length += 2;
    screen_length.desc_length += 2;
    screen_length.value_length += 2;
    screen_length.element_length += 2;

    cli_get_timeout(cli);

    for (;;)
    {
        int position;
        int line_edit_result;

        reset_pagination(cli);

        /* Make sure we have the latest prompt for this edit line */
        cli_get_prompt(cli);

        line_edit_result = line_edit(cli);
        switch (line_edit_result)
        {
            case INPUT_ENTER:
            case INPUT_TAB:
            case INPUT_QUESTIONMARK:
            {
                cli_parse_status_t parse_status;

                result = 0;
                /* Check to see if there is any input */
                if (STRING_IS_EMPTY(cli->line))
                {
                    break;
                }

                // XXX if we supported 'internal' commands (i.e. commands
                // handled directly by this process) then this could be one of
                // them (along with "cd", "dir" etc). It would set some flag that is checked for in
                // the 'for' expression above.
                /* FIXME - There is some code in os_getc() that copies a magic string just like this
                 * one into cli->line. This should be fixed.
                 */
                if (strcasecmp(cli->line, "exit") == 0)
                {
                    goto end;
                }

                position = strlen(cli->prompt);

                /* Parse the input */
                parse_status = parse_input(cli, &position);
                switch (parse_status)
                {
                    case PARSE_OK:
                        add_command_to_history(cli);
                        cli_set_edit_position(cli, 0);
                        break;

                    case PARSE_INVALID_CMD:
                    case PARSE_INVALID_GROUP:
                    case PARSE_INVALID_INSTANCE:
                    case PARSE_INVALID_PARAM:
                    case PARSE_INVALID_PARAM_VALUE:
                    case PARSE_SYSTEM_CONFIG_ERROR:
                    case PARSE_SYSTEM_APPLY_ERROR:
                    case PARSE_SYSTEM_SHOW_ERROR:
                    case PARSE_SYSTEM_ACTION_ERROR:
                    case PARSE_SYSTEM_SET_ERROR:
                        /* An error message has already been displayed */
                        result = 3;
                        add_command_to_history(cli);
                        cli_set_edit_position(cli, 0);
                        break;

                    case PARSE_AMBIGUOUS:
                    case PARSE_INVALID:
                    case PARSE_INCOMPLETE:
                        result = 4;
                        add_command_to_history(cli);
                        cli_printf(cli, "## Incomplete command. Type '?' for help.\r\n");
                        cli_set_edit_position(cli, 0);
                        break;

                    case PARSE_AUTOCOMPLETE:
                    {
                        size_t const line_length = strlen(cli->line);

                        if (line_length > 0)
                        {
                            size_t const last_char_index = line_length - 1;

                            if (cli->line[last_char_index] == '\t')
                            {
                                cli->line[last_char_index] = '\0';
                            }
                        }

                        /* Wind the cursor back to the start. The line_edit function
                         * will overwrite the current line
                         */
                        cursor_to(cli, 0);
                        break;
                    }

                    case PARSE_HELP:
                        /*
                         * Input parsed successfully and the user entered a '?' that was dealt with.
                         * Remove the '?' from the end of the input string and feed it back into the
                         * 'get_input' function.
                         */
//                cli->line[strlen(cli->line)-1] = '\0';
                        add_command_to_history(cli);
                        cli_set_edit_position(cli, position);
                        break;

                    case PARSE_COMMENT:
                    case PARSE_FIREWALL:
                    case PARSE_EMPTY_CMD:
                        cli_set_edit_position(cli, 0);
                        break;
                }
                break;
            }

            case INPUT_CTRL_Z:
                goto end;

            default:
                break;
        }

    };

    result = 2; /* SIGINT */
    cli_debug("SIGINT\n");

end:
    if (CLI_IS_INTERACTIVE(cli))
    {
        usleep(500000); /* XXX what's this sleep for? */
    }
    os_end(cli);

error:
    if (cli_tree_destroy() != 0)
    {
        cli_error(cli, "cli_tree_destroy failed\n");
    }

    cli_info(cli, "%s user logout\n", cli->user);

    closelog();

done:
    if (cli)
    {
        free(cli);
    }
    return result;
}
#endif

/**
 * Initialises the CLI context structure
 *
 */
STATIC cli_t * init_cli (void)
{
    cli_t * const pcli = calloc(1, sizeof *pcli);

    if (pcli != NULL)
    {
        pcli->input_fd  = STDIN_FILENO;
        pcli->output_fd = stdout;

        pcli->history_size = DEFAULT_HISTORY;
        pcli->width = 80;

        pcli->more  = 1;
        pcli->user[0] = '\0';
        pcli->flag = 0;
        pcli->access = config_mgr_access_level_read_only;
        pcli->source[0] = '\0';

    }

    return pcli;
}

/**
 * Log an event into the eventlog.
 */
STATIC void lognoticeevent( char const * const str )
{
    syslog(LOG_NOTICE, "%s", str );
}



/**
 * Gets the username and password and authenticates with the Config Mgr.
 *
 */
STATIC cli_status_t cli_user_auth (cli_t *context)
{
    cli_status_t result = cli_auth_error;
    bool check_validation = false;
    char    logger_buf[80];
    bool user_is_logged_in = false;

    enum {
        prompt_username,
        prompt_password,
        prompt_skip
    } prompt = prompt_username;

    int tries = MAX_LOGIN_TRIES;

    if (context->flag & (CLI_FLAG_BOOT|CLI_FLAG_ACTIONONLY|CLI_FLAG_NON_INTERACTIVE))
    {
        char * user = CLI_IS_INTERACTIVE(context) ? "boot" : "non-interactive";
        context->access = config_mgr_access_level_super;
        strcpy(context->user, user);
        result = cli_success;
        goto ret;
    }

    if (*context->user == '\0')
    {
        char username[L_cuserid+1];

        username[0] = '\0';
        cuserid(username);
        if ((username[0] != '\0') && (strcmp(username, "root") != 0))
        {
            strlcpy(context->user, username, sizeof(context->user));
            /*
             * Now that *context->user != '\0' the next if clause will
             * set
             *
             * tries = 1;
             * prompt = prompt_skip;
             * check_validation = true;
             */
            user_is_logged_in = true;
        }
    }

    if (*context->user != '\0')
    {
        tries = 1;
        prompt = prompt_skip;
        check_validation = true;
    }

    /* Populate a command tree */
    if (cli_tree_init(cli->access) != 0)
    {
        cli_printf(cli, "Cannot get Configurations. Check permission level.\n");
        goto ret;
    }

    if ((cli->flag & CLI_FLAG_SERIAL) == 0)
    {
        cli_get_timeout(cli);
    }

    while (tries > 0)
    {
        if (prompt != prompt_skip)
        {
            cli->more_lines = 0;
            cli_set_edit_position(cli, 0);

            if (prompt == prompt_username)
            {
                strcpy(cli->prompt, "\nUsername:");
                cli->flag &= ~CLI_FLAG_HIDDEN_INPUT;
            }
            else if (prompt == prompt_password)
            {
                strcpy(cli->prompt, "Password:");
                cli->flag |= CLI_FLAG_HIDDEN_INPUT;
            }
            else
                break;

            switch (line_input (cli))
            {
            case INPUT_ENTER:
                /* Check to see if there is any input */
                if (prompt == prompt_username)
                {
                    snprintf(cli->user, sizeof cli->user, "%s", cli->line);
                    prompt = prompt_password;
                }
                else
                {
                    snprintf(cli->password, sizeof cli->password, "%s", cli->line);
                    prompt = prompt_username;
                    check_validation = true;
                }
                break;

            default:
                prompt = prompt_skip;
                break;
            }
        }

        //cli_debug("%d username: %s password: %s %d\n", prompt, cli->user, cli->password, tries);

        /* Calling openlog() for the second time to overwrite the ident */
        {
        	static char ident[sizeof "CLI[@]" + sizeof cli->user + sizeof cli->source];

        	if (*cli->source != '\0')
        		snprintf(ident, sizeof ident, "CLI[%s@%s]", cli->user, cli->source);
        	else
        		snprintf(ident, sizeof ident, "CLI[%s]", cli->user);

            openlog(ident, 0, LOG_USER);
        }

        if (check_validation)
        {

            if (tlr_user_is_authorized(cli->user, cli->password, &context->access, user_is_logged_in))
            {
                switch (context->access) {
                    case config_mgr_access_level_read_only:
                    case config_mgr_access_level_read_write:
                    case config_mgr_access_level_super:
                        {
                        config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
                        config_mgr_group_t    const * const     group = &table->groups[config_user];
                        config_mgr_element_t const * const access = &group->elements.element[config_user_access];
                        config_mgr_element_enum_limit_t  const * const val = (config_mgr_element_enum_limit_t *)access->value_limit;
                        config_mgr_element_enum_t const *const access_value = &val->value[context->access];

                        snprintf (logger_buf, sizeof logger_buf, "Login by %s.", cli->user );
                        lognoticeevent (logger_buf);
                        result = cli_success;
                        cli_printf ( cli, "\nWelcome %s\n", cli->user );
                        cli_printf ( cli, "Access Level: %s\n", access_value->name );
                        cli_printf ( cli, "Timeout     : %u seconds\n", cli->cli_timeout_secs );
                        goto ret;
                        }
                    default:
                        break;
                }
            }
            snprintf (logger_buf, sizeof logger_buf, "Login failure by %s.", cli->user );
            lognoticeevent (logger_buf);
            cli_printf(cli, "\nInvalid username or password.\n\n");
            tries--;
            check_validation = false;

        }

    }

ret:
    cli->more_lines = 0;
    cli_set_edit_position(cli, 0);
    cli->flag &= ~CLI_FLAG_HIDDEN_INPUT;
    return result;
}


/**
 * Add a command to the history buffer
 *
 */
STATIC void add_command_to_history(cli_t *context)
{
    int i;

    if (context->history_size)
    {
        /*
         * Save this command in the history. The history buffer
         * array runs newest to oldest. Save by copying pointers
         * around. Also re-use the oldest buffer as the next
         * command line input buffer.
         *
         * If this command is exactly the same as the previous one,
         * don't save it.
         */
        if (context->history[0] == NULL || strcmp(context->line, context->history[0]) != 0)
        {
            char * temp;
            size_t const history_line_size = MAX_COMMAND_LINE;

            temp = context->history[context->history_size - 1];
            if (temp == NULL)
            {
                temp = (char *)malloc(history_line_size);
            }

            /*
             * If the malloc works, shuffle the history around. Otherwise
             * silently ignore the problem - it's not worth crashing the
             * system for sake of being unable to save a command...
             */
            if (temp != NULL)
            {
                for (i = context->history_size - 1; i; i--)
                {
                    context->history[i] = context->history[i-1];
                }
                context->history[0] = temp;
                strlcpy(temp, context->line, history_line_size);
            }
        }
    }
}


void clear_command_history(cli_t *context)
{
    int i;

    for (i = 0; i < context->history_size; i++)
    {
        if (context->history[i])
        {
            free(context->history[i]);
            context->history[i] = 0;
        }
    }
}


void show_history (cli_t *context)
{
    int i;

    /* Count the number of commands in the history */
    for (i = 0; context->history[i]; i++)
        ;

    if (i)
    {
        while (--i >= 0)
        {
            cli_printf (context, "  %s\r\n", context->history[i]);
        }
    }
}

