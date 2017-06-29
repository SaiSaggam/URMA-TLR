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

#include "dsl_show_socket.h"
#include "plain_socket.h"
#include "show_parse_helper.h"
#include "config_mgr.h"
#include "tlr_print.h"
#include "getlinefd.h"

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

static int connect_to_dsl_monitor(void)
{
    char const * const socket_name = DSL_SHOW_SOCKET_NAME;
    bool const use_abstract_namespace = true;
    int const socket_fd = connect_to_plain_socket(socket_name, use_abstract_namespace);

    return socket_fd;
}

static void disconnect_from_dsl_monitor(int const socket_fd)
{
	if (socket_fd >= 0)
	{
		close(socket_fd);
	}
}

static int write_line_to_dsl_monitor(int const socket_fd, char const * const string)
{
    return dprintf(socket_fd, "%s\n", string);
}

static void read_lines_from_dsl_monitor_and_print(int const socket_fd)
{
	char * line = NULL;
	size_t len = 0;
    int result;

    do
    {
        time_t const timeout_in_seconds = 5;
        struct timeval timeout;

        timeout.tv_sec = timeout_in_seconds;
        timeout.tv_usec = 0;

        result = getlinefd_timeout(&line, &len, socket_fd, &timeout);
        if (result > 0)
        {
            printf("%s", line);
        }
    }
    while (result > 0);

    free(line);
}

static bool print_response(char const * const response_format_string)
{
    bool response_printed = false;
	int const socket_fd = connect_to_dsl_monitor();

    if (socket_fd < 0)
    {
        tlr_error("could not connect to dsl monitor\n");
        goto done;
    }

    if (write_line_to_dsl_monitor(socket_fd, response_format_string) < 0)
    {
        tlr_error("could not write response format string to dsl monitor\n");
        goto done;
    }

    read_lines_from_dsl_monitor_and_print(socket_fd);

    response_printed = true;

done:
    disconnect_from_dsl_monitor(socket_fd);

    return response_printed;
}

static bool show_dsl_ascii(void)
{
    return print_response(TLR_FORMAT_ASCII);
}

static bool show_dsl_json(void)
{
    return print_response(TLR_FORMAT_JSON);
}

int main(int argc, char *argv[])
{
	int result = EXIT_FAILURE;
  	char const * const progname = basename(argv[0]);
    int const max_instance = tlr_get_max_instance(config_state_dsl, config_mgr_table_type_show);
	tlr_response_format_t format;
	int instance;

    openlog(progname, 0, LOG_USER);

    if (max_instance <= 0)
    {
        tlr_error("%s is not supported\n", progname);
        goto done;
    }

	if (parse_show_options(argc, argv, &format, &instance, FIRST_INSTANCE, max_instance) != 0)
	{
		usage_show(progname);
		goto done;
	}

	switch (format)
	{
        case tlr_response_format_unknown:
            tlr_error("unknown response format\n");
            goto done;

		case tlr_response_format_ascii:
			if (!show_dsl_ascii())
			{
                goto done;
			}
			break;

		case tlr_response_format_json:
			if (!show_dsl_json())
			{
                goto done;
			}
			break;
	}

	result = EXIT_SUCCESS;

done:
	return result;
}
