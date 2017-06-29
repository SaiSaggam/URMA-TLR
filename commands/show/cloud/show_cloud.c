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

#include "cc_status.h"
#include "plain_socket.h"
#include "show_parse_helper.h"
#include "config_mgr.h"
#include "tlr_print.h"
#include "getlinefd.h"

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include <unittest_support.h>

static int connect_to_cloud_connector(void)
{
    char const * const socket_name = CC_STATUS_SOCKET_NAME;
    bool const use_abstract_namespace = true;
    int const socket_fd = connect_to_plain_socket(socket_name, use_abstract_namespace);

    return socket_fd;
}

static void disconnect_from_cloud_connector(int const socket_fd)
{
    if (socket_fd >= 0)
    {
        close(socket_fd);
    }
}

static int write_line_to_cloud_connector(int const socket_fd, char const * const string)
{
    return dprintf(socket_fd, "%s\n", string);
}

static void read_lines_from_cloud_connector(int const socket_fd)
{
    char * line = NULL;
    size_t len;
    int result;

    do
    {
        struct timeval timeout = {.tv_sec = CC_STATUS_MSG_TIMEOUT};

        result = getlinefd_timeout(&line, &len, socket_fd, &timeout);
        if (result > 0)
        {
            printf("%s", line);
        }

    } while (result > 0);

    free(line);
}

static bool handle_show_cloud(char const * const response_format_string)
{
    bool success = false;
    int const socket_fd = connect_to_cloud_connector();

    if (socket_fd < 0)
    {
        tlr_error("could not connect to cloud connector\n");
        goto done;
    }

    if (write_line_to_cloud_connector(socket_fd, response_format_string) < 0)
    {
        tlr_error("could not write format string to cloud connector\n");
        goto done;
    }

    read_lines_from_cloud_connector(socket_fd);

    success = true;

done:
    disconnect_from_cloud_connector(socket_fd);

    return success;
}

STATIC_FUNCTION bool show_cloud(int argc, char *argv[])
{
    bool success = false;
    tlr_response_format_t format;
    char const * const progname = basename(argv[0]);

    openlog(progname, 0, LOG_USER);

    {
        int instance;
        int const max_instance = tlr_get_max_instance(config_state_cloud, config_mgr_table_type_show);
        int ret = parse_show_options(argc, argv, &format, &instance, FIRST_INSTANCE, max_instance);

        if (ret != 0)
        {
            usage_show(progname);
            goto done;
        }
    }

    switch (format)
    {
        case tlr_response_format_unknown:
            tlr_error("unknown response format\n");
            goto done;

        case tlr_response_format_ascii:
            success = handle_show_cloud(cc_msg_show_ascii);
            break;

        case tlr_response_format_json:
            success = handle_show_cloud(cc_msg_show_json);
            break;
    }

done:
    return success;
}

#if (!defined UNIT_TEST)
int main(int argc, char *argv[])
{
    bool const success = show_cloud(argc, argv);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
