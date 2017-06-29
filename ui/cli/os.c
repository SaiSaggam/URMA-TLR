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

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>

#include "cli.h"
#include "line_ed.h"
#include "cli_timeout.h"
#include "strlcpy.h"

static struct termios oldtty;

static void prepare_terminal(int const input_fd)
{
    struct termios newtty;

    tcgetattr(input_fd, &oldtty);

    /* Start with the original settings. */
    newtty = oldtty;

    /* Change to non-canonical mode */
    newtty.c_lflag &= (~ICANON);
    newtty.c_lflag &= (~ECHO);

    /* Block until one character is read */
    //newtty.c_cc[VMIN] = 1;
    //newtty.c_cc[VTIME] = 0;

    tcsetattr(input_fd, TCSANOW, &newtty);
}

static void restore_terminal(int const input_fd)
{
    /* XXX - Why are we setting these flags. Shouldn't we be restoring to the way they were when
     * the terminal was prepared?
     */
    oldtty.c_lflag |= (ICANON);
    oldtty.c_lflag |= (ECHO);

    tcsetattr (input_fd, TCSANOW, &oldtty);
}

/**
 * os_init
 *
 */
int os_init(cli_t * const cli)
{
    prepare_terminal(cli->input_fd);

    calculate_next_logout_secs(cli);

    return 0;
}

/**
 * os_end
 *
 */
void os_end(cli_t * const cli)
{
    restore_terminal(cli->input_fd);
}

/**
 * calculate_next_logout_secs
 *
 * works out the next time we will logout (in cpu seconds),
 * unless we get another key input.
 */
void calculate_next_logout_secs (cli_t * const cli)
{
    if (cli->cli_timeout_secs == 0)   // i.e logout is disabled.
    {
        cli->next_logout_secs = 0;
    }
    else
    {
        cli->next_logout_secs = getcpuseconds() + cli->cli_timeout_secs;
        if (cli->next_logout_secs == 0)
        {
            cli->next_logout_secs = 1;
        }
    }
}


/**
 * logout
 *
 * logs the user out and terminates the CLI session.
 */
void logout (cli_t * const cli)
{
    cli_info(cli, "%s user logout\n", cli->user);
    calculate_next_logout_secs(cli);
    //raise(SIGUSR1);
    terminate_signal_handle(SIGUSR1);
}


static int cli_getchar(int const input_fd)
{
    int read_result;
    unsigned char char_read;
    int ch = EOF;/* default to EOF unless a char is read. */

    do
    {
        read_result = read(input_fd, &char_read, 1);
    }
    while (read_result == -1 && errno == EINTR);

    if (read_result == 1)
    {
        ch = (int)char_read;
    }

    return ch;
}

/**
 * getchar_timeout
 *
 */
int getchar_timeout (cli_t * const cli)
{
	fd_set rfds;
	struct timeval tv;
	int retval = EOF;
    long secs_2_go;

    if (cli->next_logout_secs == 0 || (cli->flag & CLI_FLAG_EOFEXIT))   // not enabled...
    {
        fflush(cli->output_fd);     // Otherwise for some reason 'select' stops current stdout going out.
        retval = cli_getchar(cli->input_fd);
        goto ret;
    }

    // While no need to logout.
    while ((secs_2_go=(cli->next_logout_secs-getcpuseconds())) > 0)
    {
    	FD_ZERO(&rfds);
    	FD_SET(cli->input_fd, &rfds);

        if (secs_2_go > 30 || secs_2_go <= 0)
        {
            secs_2_go = 30;
        }

        tv.tv_sec = secs_2_go;
    	tv.tv_usec = 0;

        fflush(cli->output_fd);     // Otherwise for some reason 'select' stops current stdout going out.

    	retval = select(cli->input_fd + 1, &rfds, NULL, NULL, &tv);

    	if (FD_ISSET(cli->input_fd, &rfds))
        {
   		    retval = cli_getchar(cli->input_fd);

    		// And calculate the next logout time (if no user entry within that time).
            calculate_next_logout_secs(cli);
            goto ret;
        }
        // else have a character timeout.
    }

    cli->more = 0;
    cli_info(cli, "%s user timed out\n", cli->user);
    cli_printf (cli, "Timed out\n" );
    logout(cli);

ret:
	return retval;
}



/**
 * os_getc
 *
 */
int os_getc (cli_t *const context)
{
    int c;

    for (;;)
    {
        c = getchar_timeout(context);

        if (c == EOF)
        {
            /* FIXME - 1) Don't use magic strings. 2) Find a better way to indicate that the cli
             * session should end.
             */
            strlcpy (context->line, "exit", sizeof context->line);
            c = CTRL_M;
            goto done;
        }

        switch (c)
        {
            case 0x09: return CTRL_I;
            case 0x0E: return CTRL_N;
            case 0x10: return CTRL_P;
            case 0x7F: return CTRL_H;  /* Backpsace */
            case 0x1B:
                c = cli_getchar(context->input_fd);
                if (c == 0x5B)
                {
                    c = cli_getchar(context->input_fd);

                    switch (c)
                    {
                        case 0x31:
                            c = cli_getchar(context->input_fd);
                            return CTRL_A;      /* Home key */
                        case 0x34:
                            c = cli_getchar(context->input_fd);
                            return CTRL_E;      /* End key */
                        case 0x33:
                            c = cli_getchar(context->input_fd);
                            return CTRL_D;      /* Delete */
                        case 0x44:
                            return CTRL_B;      /* Left arrow */
                        case 0x43:
                            return CTRL_F;      /* Right arrow */
                        case 0x41:
                            return CTRL_P;      /* Up arrow */
                        case 0x42:
                            return CTRL_N;      /* Down arrrow */
                    }
                }
                break;
            default:
                goto done;
        }
    }

done:
    return c;
}

/*
 * Used cached output. Not really nesessary here, but handy for
 * telnet to assist the stack in producing long packets instead
 * of lots of short ones.
 */

#define PUT_CACHE_SIZE 40

static char put_cache[PUT_CACHE_SIZE];
static int put_cache_size = 0;

/**
 * os_putc
 *
 */
void os_putc (char c, cli_t * const context)
{
    put_cache[put_cache_size++] = c;
    if (put_cache_size >= PUT_CACHE_SIZE)
    {
        os_flush (context);
    }
}

/**
 * os_flush
 *
 */
void os_flush (cli_t * const context)
{
    int i;

    for (i = 0; i < put_cache_size; i++)
    {
        putc (put_cache[i], context->output_fd);
    }
    put_cache_size = 0;
}

