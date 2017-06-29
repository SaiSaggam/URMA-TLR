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
*****************************************************************************/

#include "digiutils.h"
#include "tlr_print.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

int system_command(char const * const format, ...)
{
    char * command = NULL;
    va_list args;
    int result = -1;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        result = system(command);
        if (result == -1)
        {
        	tlr_error("failed to execute command '%s'\n", command);
        }
        else if (WIFEXITED(result))
        {
        	result = WEXITSTATUS(result);	// normal termination
        }
        else if (WIFSIGNALED(result))
        {
        	tlr_error("command '%s' terminated with signal %d (pid[%d])\n", command, WTERMSIG(result), getpid());
        	result = -1;
        }
        else if (WEXITSTATUS(result))
        {
            tlr_error("command %s terminated with WEXITSTATUS %d\n", command, result);
        }
        else if (WTERMSIG(result))
        {
            tlr_error("command %s terminated with WTERMSIG %d\n", command, result);
        }
        else if (WIFSTOPPED(result))
        {
            tlr_error("command %s terminated with WIFSTOPPED %d\n", command, result);
        }
        else
        {
            tlr_error("command %s terminated with unknown %d\n", command, result);
        }
        free(command);
    }
    va_end(args);

    return result;
}

int system_command_with_log(char const * const process_name, char const * const format, ...)
{
    int result;
    char * command = NULL;
    va_list args;

    va_start(args, format);
    if (vasprintf(&command, format, args) < 0)
    {
        result = -1;
        goto error;
    }

    tlr_debug("%s\n", command);

    // This would be easier using 'process substitution', but the shell on the
    // target doesn't seem to support it. The following convoluted command
    // does the following:
    // - 'set -o pipefail' ensures we know whether the command fails
    // - discards stdout from the command
    // - redirects stderr from the command to syslog debug messages
    result = system_command("set -o pipefail; %s 2>&1 >/dev/null | logger -p user.debug -t %s", command, process_name);

error:
    free(command);
    va_end(args);

    return result;
}

int system_command_with_response(char ** response, size_t * const resp_len, char const * const format, ...)
{
    char * command = NULL;
    va_list args;
    int result = -1;

    if (response == NULL) goto error;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        FILE *fp = popen(command, "r");

        if (fp != NULL)
        {
            result = getline(response, resp_len, fp);

            if (result > 0)
            {
                char * resp_str = *response;
                size_t const last_char = strlen(resp_str) - 1;

                if (resp_str[last_char] == '\n')
                {
                    resp_str[last_char] = '\0';
                    *resp_len = last_char;
                }
                else
                {
                    *resp_len = last_char + 1;
                }
            }

            pclose(fp);
        }

        free(command);
    }
    va_end(args);

error:
    return result;
}

