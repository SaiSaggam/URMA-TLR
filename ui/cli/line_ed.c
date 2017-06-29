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
#include "cli_timeout.h"
#include "strlcpy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <stdio.h>

static const char more_prompt[] = "--More--";

#ifndef _CUNIT_
static size_t cli_get_page_size(void)
{
	static const size_t def_page_size = 40;
    uint32_t config_page_size;
    config_status_t const status = tlr_get_element_value(config_system, 1, config_system_page,
                                                        &config_page_size, sizeof config_page_size,
                                                        config_mgr_element_type_uint32, config_mgr_access_level_read_only);

	size_t const page_size = (status == config_status_success) ? config_page_size : def_page_size;

	return page_size;
}

static size_t os_window_max_lines(cli_t *context)
{
	size_t max_lines = cli_get_page_size();

//	context->more = (max_lines > 0) ? 1 : 0;

    if (context->more)
    {
        struct winsize ws;
        int const status = ioctl(0, TIOCGWINSZ, &ws);

        if (status >= 0)
        {
        	size_t const lines_available = ws.ws_row - 3;

        	if (lines_available < max_lines)
        		max_lines = lines_available;
        }
    }

    return max_lines;
}

static void print_more_prompt(cli_t * const context)
{
    char const * c = more_prompt;

    while (*c)
    {
        os_putc(*c, context);
        c++;
    }
    os_flush(context);
}

static void remove_more_prompt(cli_t * const context)
{
    char const * c = more_prompt;

    os_putc ('\r', context);
    while (*c)
    {
        /* Replace each character in the prompt with a space. */
        os_putc(' ', context);
        c++;
    }
    os_putc ('\r', context);
    os_flush(context);
}

static void cli_do_more(cli_t * const context)
{
    bool done = false;

    print_more_prompt(context);

    while (!done)
    {
        unsigned int const i = os_getc(context);

        switch (i)
        {
            case '\n':
                /* Allow one more line to be output. */
                context->more_lines--;
                done = true;
                break;

            case ' ':
                /* Allow one more page to be output. */
                context->more_lines = 0;
                done = true;
                break;

            case 'q':
                /* Cancel 'more' for this command. */
                context->more_cancelled = true;
                done = true;
                break;
        }
    }

    remove_more_prompt(context);
}

// Out for unit testing, so we can use it as a output function to check for correct operation.
void cli_puts(char * string, cli_t * context)
{
	while (*string && context->more_cancelled == false)
	{
		if (*string == '\n')
		{
			size_t const max_lines = os_window_max_lines(context);

			os_putc('\r', context);
			os_putc(*string, context);

            if ((context->more) && (++context->more_lines >= max_lines) && (max_lines > 0))
            {
				cli_do_more(context);
            }
		}
		else
		{
			os_putc(*string, context);
		}
		string++;
	}

	os_flush (context);
}
#else
void cli_puts(char * string, cli_t * context);
#endif


void cli_printf (cli_t *context, char const * const format, ...)
{
	char * line_buffer;
    va_list ap;
	int bytes_printed;

	va_start (ap, format);
	bytes_printed = vasprintf(&line_buffer, format, ap);
    va_end (ap);

	if (bytes_printed >= 0)
	{
		cli_puts (line_buffer, context);
		free(line_buffer);
	}
}


/*
 * Move the cursor to the given position on the screen.
 *
 * Moves backwards use backspaces. Moves forward re-display the characters
 * with the cursor movement being the desired side-effect.
 */

void cursor_to (cli_t * const context, int pos)
{
    char c;
    while (context->cursor > pos)
    {
        os_putc (BSP, context);
        context->cursor--;
    }
    while (context->cursor < pos)
    {
        c = context->required[context->cursor];
        os_putc (c, context);
        context->screen[context->cursor] = c;
        context->cursor++;
    }
}

/*
 * Examine the current editing context and update the screen display
 * accordingly.
 */

static void update_screen (cli_t *context)
{
    int i;
    int getter;
    int putter;
    int prompt_len;
    int first;
    int last;
    int width;
    int ok;

    /* I think the extra 1 is for the space we put after the prompt. See below where the prompt is
     * written to context->required.
     */
    prompt_len = strlen(context->prompt) + 1;

    /*
     * To prevent the terminal from auto-line feeding, never use the last column
     */

    width = context->width - 1;

    /*
     * Now work out what the screen *should* be displaying.
     *
     * Because we don't know without looking just how many characters
     * the screen will display (control characters take two positions)
     * we might have to make a few attempts to make sure that the cursor
     * actually is on the screen.
     */

#pragma message ("Have another look at the EOL $ handling")
    for (ok = 0; !ok; )
    {
        ok = 1;                 /* It *usually* works... */

        /*
         * See if the display cursor has passed beyond the edges of the displayed
         * part of the screen. Adjust the scroll distance if so.
         */

        while (context->scroll && (context->editpos <= context->scroll))
        {
            context->scroll -= SCROLL_DELTA;
        }

        while ((prompt_len + context->editpos - context->scroll) > width)
        {
            context->scroll += SCROLL_DELTA;
        }

        strcpy (context->required, context->prompt);        /* This always comes first */
        strcat (context->required, " ");
        putter = prompt_len;
        getter = context->scroll;
        if (getter)                                 /* If scrolled, supply the indicator */
        {
            context->required[putter++] = '$';
            getter++;
        }
        while (putter < width)
        {
            if (context->line[getter] != '\0')
            {
                if ((context->flag & CLI_FLAG_HIDDEN_INPUT) != 0)
                {
                    context->required[putter++] = '*';
                    getter++;
                }
                else
                {
                    context->required[putter++] = context->line[getter++];
                }
            }
            else
            {
                context->required[putter++] = ' ';
            }
        }

        /* Have we left anything undisplayed? Supply the right hand marker if so. */

        if (context->line[getter] != '\0')
        {
            context->required[width - 1] = '$';

            /*
             * If the cursor is about to be placed at or beyond the right hand
             * scroll marker - then oops, better try that one again...
             */

            if ((prompt_len + context->editpos - context->scroll) >= (width - 1))
            {
                context->scroll += SCROLL_DELTA;
                ok = 0;
            }
        }
    }

    /*
     * Now compare what should be on the screen with what is on the screen,
     * noting the first and last positions which are different.
     */

    first = -1;
    last = -1;
    for (i = 0; i < width; i++)
    {
        if (context->screen[i] != context->required[i])
        {
            if (first == -1)
            {
                first = i;
            }
            last = i;
        }
    }

    /*
     * Rewrite those parts of the screen which are wrong. Do this by moving
     * the cursor back and forward, relying on the side effect when moving
     * forward of drawing what should be there.
     */

    if (last != -1)
    {
        cursor_to (context, first);
        cursor_to (context, last + 1);
    }

    /*
     * Put the physical cursor in the place which equates to the edit position.
     */
    //printf("\n update_screen: %d + %d - %d = %d (cursor %d)\n", prompt_len , context->editpos , context->scroll,
      //      prompt_len + context->editpos - context->scroll,
        //    context->cursor);
    cursor_to (context, prompt_len + context->editpos - context->scroll);
    os_flush (context);
}

/*
 * Kill part of the line, saving the result in the yank buffer.
 * Silently ignore attempts to kill nothing.
 */

static void kill (cli_t *context, int start, int end)
{
    int i, j;

    if (end > start)
    {
        context->yank_index = (context->yank_index + 1) % YANK_SIZE;
        for (i = start, j = 0; i < end; i++, j++)
        {
            context->yank[context->yank_index][j] = context->line[i];
        }
        context->yank[context->yank_index][j] = '\0';

        while (context->line[end] != '\0')
        {
            context->line[start++] = context->line[end++];
        }
        context->line[start] = '\0';
    }
}
#if 0
/*
 * Tests if the cursor is at the end of the last (non-null) word in the
 * input line. This is a pre-requisite for TAB completion and for '?'
 * prompting.
 */

static int valid_tab_position (cli_t *context, int pos)
{
    int i;

    if (!pos || isspace (context->line[pos - 1]))    /* Never true at the start of the line */
    {
        return 0;
    }

    for (i = pos; context->line[i]; i++)    /* Rest of line must be blank */
    {
        if (!isspace (context->line[i]))
        {
            return 0;
        }
    }
    return 1;
}
#endif
/*
 * Force the input line to be redrawn by claiming that the
 * screen is all spaces.
 */

static void force_redraw (cli_t *context)
{
    memset (context->screen, ' ', sizeof (context->screen));
}

/*
 * Insert a character at the current position, if possible.
 * Updates pos and len.
 */

static int insert (cli_t * const context, char const c)
{
    int len;
    int i;
    char * const line = context->line;

    /* XXX - Wouldn't it be better if we kept a running total of the line length rather than having
     * to do a strlen() with each new character?
     */
    len = strlen(line);

    /* Need space for the new character and the NUL terminator. */
    if (len < (sizeof context->line - 1))
    {
        len++;
        /* Move right hand part up */
        /* Note, this moves the NUL terminator as well. */
        for (i = len; i > context->editpos; i--)
        {
            line[i] = line[i - 1];
        }
        /* Can now insert the new character. */
        line[context->editpos] = c;
        context->editpos++;
    }
    return len;
}


/******************************************************************************
 * line_edit
 *
 */
int line_edit (cli_t * const context)
{
    unsigned int c, i, len, pos;
    char * const line = context->line;  /* Shorthand. */
    int escaped = 0;

    cli_get_timeout(context);   // And the latest tiemout value in case it's changed.
    calculate_next_logout_secs(context);

//    len = context->editpos = context->cursor = strlen(context->line);
    len = context->cursor = strlen(line);

    context->scroll = 0;
    context->recall = 0;
    memset (context->screen, ' ', sizeof context->screen);

    pos = context->editpos;         /* Shorthand */

    for (;;)
    {
        /*
         * Make sure the user sees the result of the latest editing.
         */
        context->editpos = pos;
        update_screen (context);
        pos = context->editpos;

        c = os_getc (context);

        if (escaped)
        {
            switch (c)
            {
            case 'b':           /* Move back a word */
            case 'B':
                while (pos && !isalnum (line[pos-1]))
                {
                    pos--;
                }
                while (pos && isalnum (line[pos-1]))
                {
                    pos--;
                }
                break;

            case 'c':           /* Capitalize the next word */
            case 'C':
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                if (pos < len)
                {
                    line[pos] = (char) toupper (line[pos]);
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    line[pos] = (char) tolower (line[pos]);
                    pos++;
                }
                break;

            case 'd':           /* Delete the next word */
            case 'D':
                i = pos;
                while (i < len && !isalnum (line[i]))
                {
                    i++;
                }
                while (i < len && isalnum (line[i]))
                {
                    i++;
                }
                kill (context, pos, i);
                len = strlen (line);
                break;

            case 'f':
            case 'F':           /* Move forward a word */
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    pos++;
                }
                break;

            case 'l':           /* Lower case next word */
            case 'L':
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    line[pos] = (char) tolower (line[pos]);
                    pos++;
                }
                break;

            case 'q':           /* Escape the next character  */
            case 'Q':
#pragma message ("ESC Q handling not yet implemented")
                break;

            case 'u':           /* Upper case next word */
            case 'U':
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    line[pos] = (char) toupper (line[pos]);
                    pos++;
                }
                break;

            case 'y':           /* Cycle through deleted entries */
            case 'Y':
#pragma message ("ESC Y handling not yet implemented")
                break;
            }
            escaped = 0;
        }
        else if (c == '?')
        {
    	    cli_puts ("?", context);
    	    cli_puts("\n", context);
    	    len = insert (context, (char)c);
            pos = context->editpos;
	        return INPUT_QUESTIONMARK;
        }
        else if (isprint (c))               /* Insert a character */
        {
            len = insert (context, (char)c);
            pos = context->editpos;
        }
        else
        {
            switch (c)
            {
            case CTRL_A:                    /* Start of line */
                pos = 0;
                break;

            case CTRL_B:                    /* Move cursor left */
                if (pos)
                {
                    pos--;
                }
                break;

            case CTRL_C:
#pragma message ("Debug stuff here...")
                printf ("Yank index is %d\n", context->yank_index);
                for (i = 0; i < YANK_SIZE; i++)
                {
                    printf ("Yank buffer %d contains <%s>\n", i, context->yank[i]);
                }
                break;

            case CTRL_D:
                if (pos < len)
                {
                    for (i = pos; i < len; i++)
                    {
                        line[i] = line[i+1];
                    }
                    len--;
                }

                break;

            case CTRL_E:                    /* End of line */
                pos = len;
                break;

            case CTRL_F:                   /* Move cursor right */
                if (pos < len)
                {
                    pos++;
                }
                break;

            case CTRL_H:                  /* Backspace - delete to left of cursor */
                if (pos)
                {
                    for (i = pos; i <= len; i++)
                    {
                        line[i-1] = line[i];
                    }
                    pos--;
                    len--;
                }
                break;

            case CTRL_I:                /* Tab */
                len = insert (context, (char)'\t');
                pos = context->editpos;
                return INPUT_TAB;

            case CTRL_J:                /* Enter (CR or LF) */
            case CTRL_M:
         		cli_puts ("\n", context);
	        	return INPUT_ENTER;

            case CTRL_K:                /* Delete to end of line */
                kill (context, pos, len);
                line[pos] = 0;
                len = pos;
                break;

            case CTRL_L:                /* Refresh display */
            case CTRL_R:
                cli_puts ("\n", context);
                force_redraw (context);
		break;

            case CTRL_N:
                if (context->recall)
                {
                    context->recall--;
                    if (context->recall)
                    {
                        strlcpy(line, context->history[context->recall - 1], sizeof context->line);
                        context->scroll = 0;
                        pos = strlen(line);
                    }
                    else
                    {
                        strlcpy(line, context->saved_line, sizeof context->line);
                        context->scroll = context->saved_scroll;
                        pos = context->saved_editpos;
                    }
                    len = strlen(line);
                }
                break;

            case CTRL_P:
                if (context->recall < MAX_HISTORY && context->history[context->recall])
                {
                    /*
                     * Is this an initial excursion into the history?
                     * If so, save the current line, just in case the
                     * user returns back down to it.
                     */

                    if (!context->recall)
                    {
                        strlcpy(context->saved_line, line, sizeof context->saved_line);
                        context->saved_scroll = context->scroll;
                        context->saved_editpos = pos;
                    }

                    /*
                     * Now copy in the recalled line, ready to edit.
                     */
                    strlcpy(line, context->history[context->recall], sizeof context->line);
                    len = pos = strlen(line);
                    context->scroll = 0;

                    /*
                     * Note where we are in the history list.
                     */
                    context->recall++;
                }
                break;

            case CTRL_T:                /* Transpose characters */
                if (pos && pos < len)
                {
                    char const temp = line[pos];

                    line[pos] = line[pos - 1];
                    line[pos - 1] = temp;
                    pos++;
                }
                break;

            case CTRL_U:
            case CTRL_X:                /* Delete from start of line */
                kill (context, 0, pos);
                len = strlen(line);
                pos = 0;
                break;

            case CTRL_V:                /* Escape the next character */
#pragma message ("Control-V handling not yet implemented")
                break;

            case CTRL_W:                /* Delete word left */
                i = pos;
                while (pos && !isalnum (line[pos-1]))
                {
                    pos--;
                }
                while (pos && isalnum (line[pos-1]))
                {
                    pos--;
                }
                kill (context, pos, i);
                break;

            case CTRL_Y:                /* Yank last deletion */
#pragma message ("Control-Y handling not yet implemented")
                break;

            case CTRL_Z:                /* Exit */
                return INPUT_CTRL_Z;

            case ESC:                   /* Start of extended key sequence */
                escaped = !escaped;
                break;
            }
        }
    }
    return INPUT_ENTER;
}

int line_input (cli_t * const context)
{
    unsigned int c, i, len, pos;
    char * const line = context->line;
    int escaped = 0;

    calculate_next_logout_secs(context);

    /* Initialize some line editing parts of the cli context */
    context->cursor  = 0;
    cli_set_edit_position(context, 0);

    context->scroll = 0;
    context->recall = 0;
    memset (context->screen, ' ', sizeof (context->screen));
    len = strlen(line);

    pos = context->editpos;         /* Shorthand */

    for (;;)
    {
        /*
         * Make sure the user sees the result of the latest editing.
         */
        context->editpos = pos;
        update_screen (context);
        pos = context->editpos;

        c = os_getc (context);

        /* As soon as the user has entered a character ensure that the timeout has been assigned.
         * For serial ports, there is no timout assigned after the "Username" prompt has been
         * printed out. If the user starts entering something though, the timeout should apply.
         */
        cli_get_timeout(context);
        calculate_next_logout_secs(context);

        if (escaped)
        {
            switch (c)
            {
            case 'b':           /* Move back a word */
            case 'B':
                while (pos && !isalnum (line[pos-1]))
                {
                    pos--;
                }
                while (pos && isalnum (line[pos-1]))
                {
                    pos--;
                }
                break;

            case 'c':           /* Capitalize the next word */
            case 'C':
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                if (pos < len)
                {
                    line[pos] = (char) toupper (line[pos]);
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    line[pos] = (char) tolower (line[pos]);
                    pos++;
                }
                break;

            case 'd':           /* Delete the next word */
            case 'D':
                i = pos;
                while (i < len && !isalnum (line[i]))
                {
                    i++;
                }
                while (i < len && isalnum (line[i]))
                {
                    i++;
                }
                kill (context, pos, i);
                len = strlen (line);
                break;

            case 'f':
            case 'F':           /* Move forward a word */
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    pos++;
                }
                break;

            case 'l':           /* Lower case next word */
            case 'L':
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    line[pos] = (char) tolower (line[pos]);
                    pos++;
                }
                break;

            case 'q':           /* Escape the next character  */
            case 'Q':
#pragma message ("ESC Q handling not yet implemented")
                break;

            case 'u':           /* Upper case next word */
            case 'U':
                while (pos < len && !isalnum (line[pos]))
                {
                    pos++;
                }
                while (pos < len && isalnum (line[pos]))
                {
                    line[pos] = (char) toupper (line[pos]);
                    pos++;
                }
                break;

            case 'y':           /* Cycle through deleted entries */
            case 'Y':
#pragma message ("ESC Y handling not yet implemented")
                break;
            }
            escaped = 0;
        }
        else if (isprint (c))               /* Insert a character */
        {
            len = insert (context, (char)c);
            pos = context->editpos;
        }
        else
        {
            switch (c)
            {
            case CTRL_A:                    /* Start of line */
                pos = 0;
                break;

            case CTRL_B:                    /* Move cursor left */
                if (pos)
                {
                    pos--;
                }
                break;

            case CTRL_C:
#pragma message ("Debug stuff here...")
                printf ("Yank index is %d\n", context->yank_index);
                for (i = 0; i < YANK_SIZE; i++)
                {
                    printf ("Yank buffer %d contains <%s>\n", i, context->yank[i]);
                }
                break;

            case CTRL_D:
                if (pos < len)
                {
                    for (i = pos; i < len; i++)
                    {
                        line[i] = line[i+1];
                    }
                    len--;
                }
                break;

            case CTRL_E:                    /* End of line */
                pos = len;
                break;

            case CTRL_F:                   /* Move cursor right */
                if (pos < len)
                {
                    pos++;
                }
                break;

            case CTRL_H:                  /* Backspace - delete to left of cursor */
                if (pos)
                {
                    for (i = pos; i <= len; i++)
                    {
                        line[i-1] = line[i];
                    }
                    pos--;
                    len--;
                }
                break;

            case CTRL_J:                /* Enter (CR or LF) */
            case CTRL_M:
                cli_puts ("\n", context);
                return INPUT_ENTER;

            case CTRL_K:                /* Delete to end of line */
                kill (context, pos, len);
                line[pos] = 0;
                len = pos;
                break;

            case CTRL_L:                /* Refresh display */
            case CTRL_R:
                cli_puts ("\n", context);
                force_redraw (context);
                break;

            case CTRL_N:
                if (context->recall > 0)
                {
                    context->recall--;
                    if (context->recall)
                    {
                        strlcpy(line, context->history[context->recall - 1], sizeof context->line);
                        context->scroll = 0;
                        pos = strlen(line);
                    }
                    else
                    {
                        strlcpy(line, context->saved_line, sizeof context->line);
                        context->scroll  = context->saved_scroll;
                        pos = context->saved_editpos;
                    }
                    len = strlen(line);
                }
                break;

            case CTRL_P:
                if (context->recall < MAX_HISTORY && context->history[context->recall] != NULL)
                {
                    /*
                     * Is this an initial excursion into the history?
                     * If so, save the current line, just in case the
                     * user returns back down to it.
                     */

                    if (context->recall == 0)
                    {
                        strlcpy(context->saved_line, line, sizeof context->saved_line);
                        context->saved_scroll = context->scroll;
                        context->saved_editpos = pos;
                    }

                    /*
                     * Now copy in the recalled line, ready to edit.
                     */
                    strlcpy(line, context->history[context->recall], sizeof context->line);
                    len = pos = strlen(line);
                    context->scroll = 0;

                    /*
                     * Note where we are in the history list.
                     */
                    context->recall++;
                }
                break;

            case CTRL_T:                /* Transpose characters */
                if (pos > 0 && pos < len)
                {
                    // XXX - Why are line_edit and line_input so similar?
                    char const temp = line[pos];

                    line[pos] = line[pos - 1];
                    line[pos - 1] = temp;
                    pos++;
                }
                break;

            case CTRL_U:
            case CTRL_X:                /* Delete from start of line */
                kill (context, 0, pos);
                len = strlen(line);
                pos = 0;
                break;

            case CTRL_V:                /* Escape the next character */
#pragma message ("Control-V handling not yet implemented")
                break;

            case CTRL_W:                /* Delete word left */
                i = pos;
                while (pos && !isalnum (line[pos-1]))
                {
                    pos--;
                }
                while (pos && isalnum (line[pos-1]))
                {
                    pos--;
                }
                kill (context, pos, i);
                break;

            case CTRL_Y:                /* Yank last deletion */
#pragma message ("Control-Y handling not yet implemented")
                break;

            case CTRL_Z:                /* Exit */
                return INPUT_CTRL_Z;

            case ESC:                   /* Start of extended key sequence */
                escaped = !escaped;
                break;
            }
        }
    }
    return INPUT_ENTER;
}


