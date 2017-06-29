/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/queue.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>

#include "tlr_popen.h"
#include "strip_whitespace.h"
#include "unittest_support.h"

struct process_id_st
{
	FILE 	* read_process_stream_fh;
	FILE 	* write_process_stream_fh;
	pid_t 	child_pid;
    int     read_file_handle;
    int     write_file_handle;
	TAILQ_ENTRY(process_id_st) entry;
};

// Maintain a List of active processes(pid's)..(so we know which one to terminate or wait for termination).
TAILQ_HEAD(, process_id_st) active_pid_list = TAILQ_HEAD_INITIALIZER(active_pid_list);
static sem_t list_access_semiphore;
static int initialised_semiphore = 0;


STATIC_FUNCTION void add_into_pid_list(pid_t child_pid, FILE * read_process_fh, FILE * write_process_fh,
                                       int const read_file_handle, int const write_file_handle)
{
	// We maintain a list of process file handles versus pid (as we need to get the pid to close properly)
	struct process_id_st 	* pid_entry;
	if (read_process_fh == NULL && write_process_fh == NULL)
	{
		goto error;
	}
	pid_entry = malloc(sizeof *pid_entry);
	if (pid_entry == NULL)
	{
		goto error;
	}
	pid_entry->child_pid = child_pid;
	pid_entry->read_process_stream_fh = read_process_fh;
	pid_entry->write_process_stream_fh = write_process_fh;
    pid_entry->read_file_handle = read_file_handle;
    pid_entry->write_file_handle = write_file_handle;
	TAILQ_INSERT_TAIL(&active_pid_list, pid_entry, entry);
error:;
}


STATIC_FUNCTION struct process_id_st *find_in_pid_list(FILE * process_fh)
{
	struct process_id_st	*pid_entry;
	for (pid_entry = TAILQ_FIRST(&active_pid_list); pid_entry != NULL; )
	{
		if (pid_entry->read_process_stream_fh == process_fh || pid_entry->write_process_stream_fh == process_fh)
		{
			break;
		}
		pid_entry = TAILQ_NEXT(pid_entry, entry);
	}
	return pid_entry;
}


STATIC_FUNCTION void free_from_pid_list(struct process_id_st	* pid_entry)
{
	TAILQ_REMOVE(&active_pid_list, pid_entry, entry);
	free(pid_entry);
}

// Point to the next non deliminating character
static char * next_non_delim(char  const * const strin, int * const quote_count)
{
    char  const * str = strin;
    while (*str == ' ' || *str == '\"')
    {
        if (*str == '\"' )
        {
            if (*quote_count == 0)
            {
                *quote_count = 1;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (*quote_count == 1)
            {
                break;
            }
        }
        str++;
    }
    return (char *)str;
}


// Look for the next space or double quote character
// and return its position
static char * next_delim_char(char  const * const strin)
{
    char  const * str = strin;
    while (*str != '\0' && *str != ' ' && *str != '\"')
        str++;

    return (*str == '\0') ? NULL : (char *)str;
}


STATIC_FUNCTION bool make_args_list(char const * const  cmdstring, char *** argslist)
{
	char * buf;
	int  arg_count = 1;
	char * delim;
	char ** argv = NULL;
	bool result;
    int  	max_possible_args;
    int     current_quote_count = 0;

	if (cmdstring == NULL)
	{
		result = false;
		goto error;
	}
    max_possible_args = 1 + strlen(cmdstring) / 2;
	argv = malloc((max_possible_args+1) * sizeof(char *)); // One extra slot for the NULL.
	if (argv == NULL)
	{
		result = false;
		goto error;
	}
	buf = strdup(next_non_delim(cmdstring, &current_quote_count));
	if (buf == NULL)
	{
		free(argv);
		argv = NULL;
		result = false;
		goto error;
	}
	delim = argv[0] = buf;

    while ((delim = next_delim_char(delim)) != NULL)
	{
        char    * next_arg;

        if ((current_quote_count == 0 && *delim == ' ')
            || (current_quote_count == 1 && *delim == '\"'))
        {
            // Just scanned in next parameter
            int const current_quote_count_memo = current_quote_count;
            current_quote_count = 0;
            next_arg = next_non_delim(delim + 1, &current_quote_count);
            *delim = '\0';
            if (*next_arg == '\0')
            {
                break;
            }
            delim = argv[arg_count] = next_arg;
            if (current_quote_count_memo == 0)
            {
                // Strip trailing white of param we've just gathered if it wasn't in quotes.
                strip_whitespace(argv[arg_count - 1]);
            }
            arg_count++;
        }
        else if (*delim == '\"')
        {
            current_quote_count = 1;
            delim++;
        }
        else
        {
            delim++;
        }
    }
	argv[arg_count] = NULL;
	result = true;
error:
	*argslist = argv;
	return result;
}


STATIC_FUNCTION void free_args_list(char ** argslist)
{
    if (argslist != NULL)
    {
        free(argslist[0]);
        free(argslist);
    }
}


static bool tlr_popen_args_read_write(char * const * const arg_list, char const * const mode, FILE ** const read_handle, FILE ** const write_handle)
{
    // a popen() like replacement except doesn't use shell and can have both a read handle FILE * and write handle returned.
    // mode is either 'r', 'w', or 'rw'
    // returns true is succesfull.
    FILE 	* read_process_fh = NULL;
    FILE 	* write_process_fh = NULL;
    bool    result = false;
    bool    is_mode_r = false;
    bool    is_mode_w = false;
    int     pfds_stdin[2];
    int     pfds_stdout[2];
    pid_t   fork_result;

    if (initialised_semiphore == 0)
    {
        initialised_semiphore = 1;
        sem_init(&list_access_semiphore, 0, 1);
    }

    // Check modes (read and/or write simultaneously supported)
    {
        int     idx;
        for (idx = 0; idx < 2; idx++)
        {
            char    c;
            if ((c = mode[idx]) == '\0')
                break;
            switch (c)
            {
                case 'r':
                    is_mode_r = true;   break;
                case 'w':
                    is_mode_w = true;   break;
            }
        }
    }
    if (!is_mode_r && !is_mode_w)
    {
        // Got to have at least one working mode
        goto error;
    }

    if (is_mode_r)
    {
        if (pipe(pfds_stdout) == -1)
        {
            goto error;
        }
    }
    if (is_mode_w)
    {
        if (pipe(pfds_stdin) == -1)
        {
            goto error;
        }
    }

    fork_result = fork();
    if (fork_result == -1)
    {
        if (is_mode_r)
        {
            close(pfds_stdout[0]);
        }
        if (is_mode_w)
        {
            close(pfds_stdin[1]);
        }
        goto error;
    }
    if (fork_result == 0)
    {
        // Child
        if (is_mode_r)
        {
            close(pfds_stdout[0]);  /* the child doesn't use this copy of the handle, so close it */
            dup2(pfds_stdout[1], STDOUT_FILENO);   /* child stdout will now go to pfds_stdout */
        }
        if (is_mode_w)
        {
            close(pfds_stdin[1]);
            dup2(pfds_stdin[0], STDIN_FILENO);   /* child stdin will come from pfds_stdin */
        }
        execvp(arg_list[0],  arg_list);
        // If all is ok should never get here.
        exit(errno);
    }
    else
    {
        int     read_file_handle = -1;
        int     write_file_handle = -1;
        // Parent
        // Get a stream for the return to the caller.
        if (is_mode_r)
        {
            close(pfds_stdout[1]);
            read_file_handle = pfds_stdout[0];
            read_process_fh = fdopen(pfds_stdout[0], mode);
        }
        if (is_mode_w)
        {
            close(pfds_stdin[0]);
            write_file_handle = pfds_stdin[1];
            write_process_fh = fdopen(pfds_stdin[1], mode);
        }
        sem_wait(&list_access_semiphore);
        add_into_pid_list(fork_result, read_process_fh, write_process_fh, read_file_handle, write_file_handle);
        sem_post(&list_access_semiphore);

        if (read_handle != NULL)
        {
            *read_handle = read_process_fh;
        }
        if (write_handle != NULL)
        {
            *write_handle = write_process_fh;
        }
        result = true;
    }

error:
    return result;
}


static FILE * tlr_popen_args(char * const * const arg_list, char const * const mode)
{
    // The popen() replacement... mode is either 'r' or 'w' (not both).
    // and the single FILE * is the return value.
    bool    result;
    FILE    * process_fh;

    if (*mode == 'r')
    {
        result = tlr_popen_args_read_write(arg_list, "r", &process_fh, NULL);
    }
    else if (*mode == 'w')
    {
        result = tlr_popen_args_read_write (arg_list, "w", NULL, &process_fh);
    }
    else
    {
        result = false;
    }
    return result ? process_fh : NULL;
}



int tlr_pclose(FILE * const process)
{
    int 	rv;
    struct process_id_st * pid_entry;
    pid_t child_pid;

    sem_wait(&list_access_semiphore);
    pid_entry = find_in_pid_list(process);
    sem_post(&list_access_semiphore);

    if (pid_entry == NULL)
    {
        rv = -1;
        goto error;
    }
    if (pid_entry->read_process_stream_fh != NULL)
    {
        fflush(pid_entry->read_process_stream_fh);
    }
    if (pid_entry->write_process_stream_fh != NULL)
    {
        fflush(pid_entry->write_process_stream_fh);
    }
    if (pid_entry->read_file_handle != -1)
    {
        close(pid_entry->read_file_handle);
    }
    if (pid_entry->write_file_handle != -1)
    {
        close(pid_entry->write_file_handle);
    }
    child_pid = pid_entry->child_pid;

    if (pid_entry->read_process_stream_fh != NULL)
    {
        fclose(pid_entry->read_process_stream_fh);
    }
    if (pid_entry->write_process_stream_fh != NULL)
    {
        fclose(pid_entry->write_process_stream_fh);
    }
    sem_wait(&list_access_semiphore);
    free_from_pid_list(pid_entry);
    sem_post(&list_access_semiphore);

    if (waitpid(child_pid, &rv, 0) == -1)
    {
        rv = -1;
    }
error:
    return rv;
}

FILE * tlr_popen(char  const * const command, char const * const mode)
{
	// Popen replacement BUT doesn't use the shell and like popen returns a FILE stream pointer.
    char 	** cmd_args;
    FILE    * return_val = NULL;

    if (make_args_list(command, &cmd_args))
    {
        return_val = tlr_popen_args(cmd_args, mode);
        free_args_list(cmd_args);
    }
    return return_val;
}

bool tlr_popen_read_write(char  const * const command, char const * const mode, FILE ** const read_handle, FILE ** const write_handle)
{
    // a popen() like replacement except doesn't use shell and can have both a read handle FILE * and write handle returned.
    // mode is either 'r', 'w', or 'rw'
    // returns true is succesfull.
    // only one call to tlr_pclose() is needed when done and want to close.  Can use either FILE * handle.
    char 	** cmd_args;
    bool    return_val = false;

    if (make_args_list(command, &cmd_args))
    {
        return_val = tlr_popen_args_read_write(cmd_args, mode, read_handle, write_handle);
        free_args_list(cmd_args);
    }
    return return_val;
}

static int call_process(char * command)
{
    FILE 	* process;
    int 	rv = -1;

    process = tlr_popen(command, "r");
    if (process != NULL)
    {
        rv = tlr_pclose(process);
    }
    return rv;
}


int tlr_system_command(char const * const format, ...)
{
    char * command = NULL;
    va_list args;
    int result = -1;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        result = call_process(command);
        free(command);
    }

    va_end(args);
    return result;
}


int tlr_system_command_with_response(char ** const response, size_t * const resp_len, char const * const format, ...)
{
    char * command = NULL;
    va_list args;
    int result = -1;

    if (response == NULL) goto error;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        FILE *fp = tlr_popen(command, "r");

        if (fp != NULL)
        {
            result = getline(response, resp_len, fp);

            if (result > 0)
            {
                char * resp_str = *response;
                size_t const last_char = result - 1;

                if (resp_str[last_char] == '\n')
                {
                    resp_str[last_char] = '\0';
                    *resp_len = last_char;
                }
                else
                {
                    *resp_len = result;
                }
            }

            tlr_pclose(fp);
        }

        free(command);
    }
    va_end(args);

error:
    return result;
}