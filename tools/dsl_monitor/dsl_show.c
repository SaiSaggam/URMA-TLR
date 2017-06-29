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

#include "dsl_show.h"
#include "dsl_show_ascii.h"
#include "dsl_show_json.h"
#include "dsl_context.h"
#include "dsl_modem.h"
#include "dsl_log.h"
#include "dsl_show_socket.h"
#include "dsl_status.h"
#include "plain_socket.h"
#include "tlr_print.h"
#include "getlinefd.h"

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static tlr_response_format_t get_response_format_from_client(int const socket_fd)
{
    tlr_response_format_t response_format = tlr_response_format_unknown;
	char * line = NULL;
	size_t len = 0;
    time_t const timeout_in_seconds = 5;
    struct timeval timeout;
    char const * response_format_string;

    timeout.tv_sec = timeout_in_seconds;
    timeout.tv_usec = 0;

    if (getlinefd_timeout(&line, &len, socket_fd, &timeout) <= 0)
    {
        goto done;
    }

    response_format_string = strtok(line, "\n");
    if (response_format_string == NULL)
    {
        goto done;
    }

    response_format = tlr_response_format_from_string(response_format_string);

done:
    free(line);

    return response_format;
}

static void communicate_with_client(int const socket_fd, dsl_context_t const * const context)
{
    tlr_response_format_t const response_format = get_response_format_from_client(socket_fd);

    switch (response_format)
    {
        case tlr_response_format_unknown:
            dsl_error("unknown response format\n");
            break;

        case tlr_response_format_ascii:
            dsl_show_ascii(socket_fd, context);
            break;

        case tlr_response_format_json:
            dsl_show_json(socket_fd, context);
            break;
    }
}

static void dsl_show_socket_callback(int const socket_fd, void * const user_context)
{
	dsl_context_t * const context = user_context;

	if (dsl_modem_status == NULL)
	{
        goto done;
    }

	pthread_mutex_lock(&dsl_modem_status->lock);
    dsl_status_get(context);
	communicate_with_client(socket_fd, context);
	pthread_mutex_unlock(&dsl_modem_status->lock);

done:
    close(socket_fd);
}

int show_init(dsl_context_t * const context)
{
	plain_socket_context_st * plain_socket_context;

    plain_socket_context = open_plain_socket_context(DSL_SHOW_SOCKET_NAME, true, dsl_show_socket_callback, context);
    if (plain_socket_context == NULL)
    {
        dsl_error("failed to open plain socket context\n");
        goto error;
    }

	return EXIT_SUCCESS;

error:
    return EXIT_FAILURE;
}
