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
#include "tlr_print.h"
#include "tlr_common.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>


#define MAX_MATCHES 50
static char  const * matches[MAX_MATCHES];
static int    match_idx = 0;

#ifdef SIMULATE_SHOW_COMMAND
static config_ipm_tree_type_t match_tree_type = config_ipm_tree_type_command;
#endif

#define skip_spaces(buf) while (*buf == ' ') buf++

static void write_value_matches(cli_t * const context, cli_cmd_t * const cmd, size_t value_length);

static void update_line_matches(cli_t * const context, char const * const buffer, size_t const skip_length)
{
    char const *match_space_character = (match_idx == 1) ? " " : "";
    size_t const pos = context->editpos - 1 - skip_length;
    size_t avail_size = sizeof context->line - pos;
    char * ptr = &context->line[pos];
    size_t match_length;
    char * endstring = NULL;
    int const append_length = asprintf(&endstring, "%s", &context->line[context->editpos]);

    context->editpos = ptr - context->line;

    //printf("\n update_line: %d (%s) + auto string (%s) %d + length %d (%s) space (%s)\n",
           // context->editpos, ptr, buffer, strlen(buffer), append_length, endstring, match_space_character);

    match_length = snprintf(ptr, avail_size, "%s%s", buffer, match_space_character);
    if (match_length > avail_size)
    {
        tlr_debug("%s: truncation occurred\n", __func__);
        match_length = avail_size;
    }
    context->editpos += match_length;
    ptr += match_length;
    avail_size -= match_length;

    if (append_length > 0 && avail_size > 0)
    {
        snprintf(ptr, avail_size, "%s", endstring);
    }
    free(endstring);
}

/**
 * Returns the index of where s1 and s2 start to differ
 */
static int strdiffidx(const char * const s1, const char * const s2)
{
    char *p1 = (char *)s1, *p2 = (char *)s2;
    int   idx = 0;

    if (p1 == NULL || p2 == NULL)
        return 0;

    while (*p1 && *p2)
    {
        if (*p1 != *p2)
            return idx;

        idx++;
        p1++;
        p2++;
    }

    return idx;
}

static void walk_cmd_cb(cli_tree_data_type_t const data_type, char const * walk_name,
                        void * const data_tree, void * const context)
{
    cli_tree_data_t const * const tree = data_tree;

    if (tree != NULL && match_idx < MAX_MATCHES)
    {
        matches[match_idx++] = tree->name;
    }
}



int autocomplete_cmd (cli_t * const context, cli_cmd_t * const cmd, char * const buffer, int autohelp)
{
    int  orig_len;
    char *buf = buffer;
    int ret = 0;

    match_idx = 0;

    /* Skip past any spaces */
    skip_spaces(buf);
    /* Check we have something to match */
    if (STRING_IS_EMPTY(buf))
    {
        write_value_matches(context, cmd, 0);
        ret = -1;
        goto done;
    }

    orig_len = strlen(buf);

    /* First find the commands that match buf */
    cli_tree_walk(cmd->tree_type, buf, walk_cmd_cb, context);

    switch (match_idx)
    {
    case 0:
         /* No matches - nothing to do */
        update_line_matches(context, "", 0);
        ret = -1;
        break;

    case 1:
        /* One command matched - copy it into the cli context */
        update_line_matches(context, matches[0], orig_len);
        if (autohelp)
        {
            help_all(context, cmd, buffer);
            //strcpy (context->line, buffer);
        }
        ret = 0;
        break;

    default:
        /* Multiple matches - complete as much as we can */
        {
            int i, len, idx;

            /* Start with the first command ... */
            // FIXME - what's the size of the destination buffer? Use strlcpy()
            strcpy(buf, matches[0]);
            len = strlen(buf);

            /* ... and now run through the rest and find where each command differs */
            for (i = 1; i < match_idx; i++)
            {
                idx = strdiffidx (buf, matches[i]);
                if (idx < len)
                {
                    len = idx;
                    buf[len] = '\0';
                }
            }

            /* Did we manage to add anything? If not, display the available commands
             */
//            if (len == orig_len)
            {
                cli_printf(context, "\r\n");

                if (cmd->tree_type == cli_tree_type_command)
                {
                    help_all(context, cmd, buf);
                }
            }
            update_line_matches(context, buf, orig_len);

        }
        ret = 0;
        break;
    }
done:
    return ret;
}

static void walk_element_cb(cli_tree_data_type_t const tree_type, char const * walk_name,
                            void * const data_tree, void * const context)
{
    if (data_tree != NULL)
    {
        switch (tree_type)
        {
        case cli_tree_data_type_element:
            {
                cli_tree_data_t * const context_data = context;
                cli_tree_data_t * const tree = data_tree;
                config_mgr_element_t * const element = tree->data;

                if ((strncmp(element->name, context_data->name, strlen(context_data->name)) == 0) &&
                    (match_idx < MAX_MATCHES))
                    matches[match_idx++] = tree->name;
                break;
            }
        case cli_tree_data_type_group:
            {
                cli_tree_data_t * const context_data = context;
                cli_tree_data_t * const tree = data_tree;
                config_mgr_group_t * const group = tree->data;

                if ((strncmp(group->name, context_data->name, strlen(context_data->name)) == 0) &&
                    (match_idx < MAX_MATCHES))
                {
                    matches[match_idx++] = tree->name;
                }
            }
            break;
        case cli_tree_data_type_action_element:
            {
                cli_tree_data_t * const context_data = context;
                cli_tree_data_t * const tree = data_tree;
                config_mgr_action_element_t const * action_element = tree->data;

                if ((strncmp(action_element->name, context_data->name, strlen(context_data->name)) == 0) &&
                    (match_idx < MAX_MATCHES))
                {
                    matches[match_idx++] = tree->name;
                }

            }
            break;
        default:
            cli_debug("unexpected type %d\n", tree_type);
            break;
        }
    }
}

void autocomplete_instance(cli_t * const context, cli_cmd_t * const cmd)
{
    match_idx = 0;
    help_group(context, cmd, "");
    update_line_matches(context, "", 0);
}

void autocomplete_element (cli_t * const context, cli_cmd_t * const cmd, cli_tree_data_t const * const command, char * const buffer)
{
    int orig_len;
    //size_t line_pos = context->editpos;

    char *buf = buffer;
    skip_spaces(buf);

    match_idx = 0;
    if (STRING_IS_EMPTY(buf))
    {
        /* Nothing to complete - show the available parameters */
        cli_printf(context, "\r\n");
        if (command->tree_data_type == cli_tree_data_type_group)
            help_group(context, cmd, buf);
        else
            help_action(context, cmd, buf);
        write_value_matches(context, cmd, 0);
        goto done;
    }

    orig_len = strlen(buf);

    /* First find the commands that match buf */
    {
        cli_tree_data_t tree_data;
        tree_data.name = buf;
        tree_data.data = NULL;

        cli_tree_walk_subtree(command->search_tree, NULL, walk_element_cb, &tree_data);
    }

    switch (match_idx)
    {
    case 0:
        /* No matches - nothing to do */
        update_line_matches(context, "", 0);
        break;

    case 1:
        /* One parameter matched - copy it into the cli context */
        update_line_matches(context, matches[0], orig_len);
        break;

    default:
        /* Multiple matches - complete as much as we can */
        {
            int i, len, idx;

            /* Start with the first parameter ... */
            strcpy (buf, matches[0]);
            len = strlen (buf);

            /* ... and now run through the rest and find where each parameter differs */
            for (i = 1; i < match_idx; i++)
            {
                idx = strdiffidx (buf, matches[i]);
                if (idx < len)
                {
                    len = idx;
                    buf[len] = '\0';
                }
            }

            /* Did we manage to add anything? If not, display the available
             * commands
             */
//            if (len == orig_len)
            {
                cli_printf(context, "\r\n");
                if (command->tree_data_type == cli_tree_data_type_group)
                    help_group(context, cmd, buf);
                else
                    help_action(context, cmd, buf);
            }
            update_line_matches(context, buf, orig_len);
        }
        break;
    }
done:
    return;
}

static void write_value_matches(cli_t * const context, cli_cmd_t * const cmd, size_t value_length)
{
    char buffer[CONFIG_MGR_NAME_LENGTH];

    //printf("write_value_match:match_idx %d editpos %d cursor %d (%s) %d match len %d\n", match_idx,
            //context->editpos, context->cursor, context->line, strlen(context->line), value_length);
    switch (match_idx)
    {
    case 0:
        /* No matches - nothing to do */
        update_line_matches(context, "", 0);
        break;

    case 1:
        /* One parameter matched - copy it into the cli context */
        update_line_matches(context, matches[0], value_length);
        break;

    default:
        /* Multiple matches - complete as much as we can */
        {
            int i;
            int idx;
            int len;

            /* Start with the first value ... */
            strcpy (buffer, matches[0]);
            len = strlen (buffer);

            /* ... and now run through the rest and find where each value differs */
            for (i = 1; i < match_idx; i++)
            {
                idx = strdiffidx (buffer, matches[i]);
                if (idx < len)
                {
                    len = idx;
                    buffer[len] = '\0';
                }
            }

            //if (match_idx > 0)
            {
                cli_printf(context, "\r\n");
                help_group_element(context, cmd);
            }
            update_line_matches(context, buffer, value_length);

        }
        break;
    }

}

/* FIXME - Shouldn't be using these magic strings. Get their values from command tool somewhere. */
static char * const on_off_string[] = { "off", "on"};
static char * const true_false_string[] = { "false", "true"};

static void autocomplete_string(cli_t * const context, cli_cmd_t * const cmd, char * const buffer, char * const * valid_values, size_t instances)
{
    size_t orig_len;
    char *buf = buffer;

    skip_spaces(buf);

    match_idx = 0;

    if (STRING_IS_EMPTY(buf))
    {
        /* Nothing to complete - show the available values */
        cli_printf(context, "\r\n");
        help_group_element(context, cmd);
        write_value_matches(context, cmd, 0);
        goto done;
    }

    orig_len = strlen(buf);

    {
        size_t i;

        for (i=0; i < instances; i++)
        {
            if ((strncmp(valid_values[i], buf, strlen(buf)) == 0) &&
                (match_idx < MAX_MATCHES))
            {
                matches[match_idx++] = valid_values[i];
            }
        }
    }

    write_value_matches(context, cmd, orig_len);

done:
    return;
}

void autocomplete_on_off(cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
    autocomplete_string(context, cmd, buffer, on_off_string, config_mgr_asizeof(on_off_string));
}

void autocomplete_boolean(cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
    autocomplete_string(context, cmd, buffer, true_false_string, config_mgr_asizeof(true_false_string));
}

static void walk_enum_cb(cli_tree_data_type_t const tree_type, char const * walk_name,
                         void * const data_tree, void * const context)
{
    cli_tree_data_t const * tree = data_tree;
    if (data_tree != NULL)
    {
        config_mgr_element_enum_t const * enum_value = tree->data;

        if (enum_value != NULL && match_idx < MAX_MATCHES)
        {
           matches[match_idx++] = enum_value->name;
        }
    }
}


void autocomplete_value (cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
    size_t orig_len;
    char *buf = buffer;

    skip_spaces(buf);

    match_idx = 0;

    if (STRING_IS_EMPTY(buf))
    {
        /* Nothing to complete - show the available values */
        cli_printf(context, "\r\n");
        help_group_element(context, cmd);
        write_value_matches(context, cmd, 0);
        goto done;
    }

    orig_len = strlen(buf);

    /* First find the values that match buf */
    cli_tree_walk_subtree(cmd->element->search_tree, buf, walk_enum_cb, context);

    write_value_matches(context, cmd, orig_len);

done:
    return;
}

void autocomplete_multi_value (cli_t * const context, cli_cmd_t * const cmd, char * const buffer)
{
    size_t orig_len;
    char *buf = buffer;

    skip_spaces(buf);

    match_idx = 0;

    if (STRING_IS_EMPTY(buf))
    {
        /* Nothing to complete - show the available values */
        cli_printf(context, "\r\n");
        help_group_element(context, cmd);
        write_value_matches(context, cmd, 0);
        goto done;
    }

    /* First find the values that match buf */
    {
        char * p = strrchr(buf, ',');
        if (p != NULL)
        {
            buf = p+1;
        }
    }

    orig_len = strlen(buf);

    cli_tree_walk_subtree(cmd->element->search_tree, buf, walk_enum_cb, context);

    write_value_matches(context, cmd, orig_len);
done:
    return;
}
