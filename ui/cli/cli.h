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

#ifndef _CLI_H_
#define _CLI_H_

#include <stdio.h>
#include <stdint.h>
#include <syslog.h>

#include "config_mgr.h"
#include "cli_tree.h"
#include <stdbool.h>

#define CLI_DEBUG 0

#define CLI_FLAG_HIDDEN_INPUT     0x01
#define CLI_FLAG_BOOT             (0x01 << 1)
#define CLI_FLAG_SERIAL           (0x01 << 2)
#define CLI_FLAG_EOFEXIT          (0x01 << 3)
#define CLI_FLAG_ACTIONONLY       (0x01 << 4)
#define CLI_FLAG_FIREWALL_SECTION (0x01 << 5)
#define CLI_FLAG_NON_INTERACTIVE  (0x01 << 6)

#define CLI_IS_INTERACTIVE(cli) (((cli)->flag & CLI_FLAG_NON_INTERACTIVE) == 0)

#define cli_error(p, x...)     do { if (!(p->flag & (CLI_FLAG_BOOT|CLI_FLAG_ACTIONONLY))) syslog(LOG_ERR, x); } while (0)
#define cli_info(p, x...)      do { if (!(p->flag & (CLI_FLAG_BOOT|CLI_FLAG_ACTIONONLY))) syslog(LOG_INFO, x); } while (0)

#if (CLI_DEBUG == 1)
#define debug_printf(x...)  do { printf("%s: ", __func__); printf (x); } while (0)
#define cli_debug(x...)     do { printf("%s: ", __func__); printf (x); } while (0)

#elif (CLI_DEBUG == 2)
#define debug_printf(x...)  do { syslog(LOG_DEBUG, x); } while (0)
#define cli_debug(x...)     do { syslog(LOG_DEBUG, x); } while (0)

#elif (CLI_DEBUG == 3)
#define debug_printf(x...)
#define cli_debug(x...)     do { syslog(LOG_DEBUG, x); } while (0)

#else
#define debug_printf(x...)
#define cli_debug(x...)
#endif

#define MIN_NAME_LENGTH 10

#define SET_MAX(x, y) if ((x) < (y)) (x) = (y)
#define SET_MIN(x, y) if ((x) > (y)) (x) = (y)


typedef enum
{
    cli_success,
    cli_auth_error,
    cli_parse_error,
    cli_config_error,
    cli_apply_error,
    cli_resource_error
} cli_status_t;



#define MAX_LOGIN_TRIES      3  // Configuration???

#define INVALID_INSTANCE     (-1)

#define CLI_BUFFER_SIZE  511

/*
 * Line Editing definitions
 */
/* ...but no less that this wide */
#define MIN_TERMINAL_WIDTH  32
/* Maximum length of input (including the trailing null) */
#define MAX_COMMAND_LINE    1024
/* How far we scroll when the line gets too long for the display */
#define SCROLL_DELTA        10
/* Maximum number of commands we can save */
#define MAX_HISTORY         10
/* Standard number of commands to save */
#define DEFAULT_HISTORY     MAX_HISTORY
/* Number of deletions to save */
#define YANK_SIZE           10

#define MAX_LEN_CLI_PROMPT  31
#define MAX_SOURCE_LEN      (sizeof "255.255.255.255")

/*
 * Input return codes
 */
enum {
    INPUT_ENTER,
    INPUT_QUESTIONMARK,
    INPUT_TAB,
    INPUT_CTRL_Z
};

/*
 * Parse return codes
 */
typedef enum {
    PARSE_OK,
    PARSE_AMBIGUOUS,
    PARSE_INVALID,
    PARSE_INCOMPLETE,
    PARSE_HELP,
    PARSE_COMMENT,
    PARSE_FIREWALL,
    PARSE_EMPTY_CMD,
    PARSE_AUTOCOMPLETE,
    PARSE_INVALID_CMD,
    PARSE_INVALID_GROUP,
    PARSE_INVALID_INSTANCE,
    PARSE_INVALID_PARAM,
    PARSE_INVALID_PARAM_VALUE,
    PARSE_SYSTEM_CONFIG_ERROR,
    PARSE_SYSTEM_APPLY_ERROR,
    PARSE_SYSTEM_SHOW_ERROR,
    PARSE_SYSTEM_ACTION_ERROR,
    PARSE_SYSTEM_SET_ERROR
} cli_parse_status_t;

typedef enum
{
    cli_cmd_none,
    cli_cmd_set,
    cli_cmd_show,
    cli_cmd_action
} cli_cmd_type_t;

typedef struct
{
    cli_cmd_type_t cmd_type;
//    char * cmd_name;
    cli_tree_type_t tree_type;
    unsigned int instance;
    cli_tree_data_t const * action;
    cli_tree_data_t const * group;
    cli_tree_data_t const * element;
#if CONFIG_MGR_MAX_ACTION_PARAMETER_COUNT
    cli_tree_data_t const * parameter[CONFIG_MGR_MAX_ACTION_PARAMETER_COUNT];
#endif
    void * context;
    size_t parameter_count;
    unsigned int    walking_enum;
} cli_cmd_t;

/*
 * Information about the current CLI session.
 */
typedef struct
{
    int input_fd;
    FILE       *output_fd;

    char        user[MAX_USERNAME_LEN];
    char        password[MAX_USERNAME_LEN];
    config_mgr_access_level_t  access;

    uint32_t    flag;

    int         more;
    uint32_t    more_lines;
	bool 		more_cancelled;	/*set to true after the user presses 'q' during more output. once set, all subsequent output from the command is dropped*/

    /* Line Editing */
    char  screen[MAX_COMMAND_LINE];    /* What the physical terminal currently shows */
    char  required[MAX_COMMAND_LINE];  /* What we want the terminal to show */
    char  line[MAX_COMMAND_LINE];
    char  prompt[MAX_LEN_CLI_PROMPT + 1];
    int   cursor;                        /* Where the physical cursor currently is */
    int   editpos;                       /* The logical cursor position within "line" */
    int   scroll;                        /* Distance scrolled to get the edit position onto the display */
    int   width;                         /* Actual terminal width */
    int   history_size;                  /* How many commands are stored for recall */
    char *history[MAX_HISTORY];          /* The commands ready for recall (newest to oldest) */
    int   recall;                        /* Which command in the history is being recalled */
    char  saved_line[MAX_COMMAND_LINE];  /* Used during history processing */
    int   saved_editpos;                 /* to allow the current line in progress to be kept */
    int   saved_scroll;
    char  yank[YANK_SIZE][MAX_COMMAND_LINE];    /* Deletions... */
    int   yank_index;                    /* Where the most recently yanked thing is */
    size_t cmd_instance;

    uint32_t  cli_timeout_secs;            /* timeout in seconds (innactivity before logout) */
    long    next_logout_secs;           /* cpu seconds in which we should logout if do not receive a character within that time */
    char  source[MAX_SOURCE_LEN];
    size_t max_spacing;
} cli_t;

typedef struct {
    size_t name_length;
    size_t desc_length;
    size_t value_length;
    size_t element_length;
} screen_length_t;

extern screen_length_t screen_length;

int  cli_get_param_configuration (char const * const cmd, size_t instance, char const * const param, void **buffer, size_t * const len);
void clear_command_history (cli_t * cli);
void cli_set_edit_position(cli_t * const cli, size_t const edit_position);

void show_history (cli_t * cli);

cli_parse_status_t parse_input (cli_t * cli, int *position);
/*
    get_instance:
    return true if the string is numeric. The numeric value of the string will be placed into value
    return false if there are any non-numeric chars in the string. value is unchanged in that case
*/
bool get_instance(char const * const string, unsigned int * const value);

void help_all(cli_t * const cli, cli_cmd_t * const cmd, char const * const buf);
void help_group(cli_t * const cli, cli_cmd_t * const cmd, char const * const buf);
void help_group_element(cli_t * const cli, cli_cmd_t * const cmd);
void help_action(cli_t * const context, cli_cmd_t * const cmd, char const * const buf);
void help_action_element(cli_t * const cli, cli_cmd_t * const cmd);
void cli_cmd_print_all_elements_in_group(cli_t * const context, cli_cmd_t * const cmd);


int autocomplete_cmd (cli_t * const cli, cli_cmd_t * const cmd, char * const buf, int autohelp);
void autocomplete_element (cli_t * const context, cli_cmd_t * const cmd, cli_tree_data_t const * const command, char * const buffer);
void autocomplete_on_off (cli_t * const cli, cli_cmd_t * const cmd, char * const buf);
void autocomplete_boolean(cli_t * const context, cli_cmd_t * const cmd, char * const buffer);
void autocomplete_value (cli_t * const cli, cli_cmd_t * const cmd, char * const buf);
void autocomplete_multi_value (cli_t * const context, cli_cmd_t * const cmd, char * const buffer);
void autocomplete_instance(cli_t * const context, cli_cmd_t * const cmd);

void   cli_printf (cli_t * cli, char const * const format, ...);

void terminate_signal_handle(int const signo);

#endif /* _CLI_H_ */
