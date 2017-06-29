/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <stdlib.h>
#include <stdbool.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"

static char ds_buffer[DS_DATA_SIZE];

unsigned int put_file_active_count[connector_transport_all] = {0};
static bool first_time = true;
size_t put_request_size = 0;

void initialize_send_data(void)
{
    if (first_time)
    {
        int i;

        for (i = 0; i < DS_DATA_SIZE; i++)
            ds_buffer[i] = 0x41 + (rand() % 0x3B);
        first_time = false;
    }
}

connector_status_t send_file(connector_handle_t handle, int index, char * const filename, char * const content, size_t content_length, connector_transport_t const transport)
{

    connector_status_t status = connector_success;
    static char file_type[] = "text/plain";
    ds_record_t * user;

    {
        void * ptr;

        connector_callback_status_t const is_ok = app_os_malloc(sizeof *user, &ptr);
        if (is_ok != connector_callback_continue|| ptr == NULL)
        {
            /* no memeory stop Cloud Connector */
            APP_DEBUG("send_put_request: malloc fails\n");
            status = connector_no_resource;
            goto done;
        }
        user = ptr;
    }

    sprintf(user->file_path, "%s", filename);
    user->header.option = connector_data_service_send_option_overwrite;
    user->header.path  = user->file_path;
    user->header.content_type = file_type;
    user->header.user_context = user;
    user->header.transport = transport;
    user->bytes_sent = 0;
    user->file_data = content;
    user->file_length_in_bytes = content_length;
    user->index = index;
    app_os_get_system_time(&user->initiate_time);

    status = connector_initiate_action(handle, connector_initiate_send_data, &user->header);
    if (status == connector_success)
    {
        APP_DEBUG("send_file: ***** %s ***** %p %s length %" PRIsize " \n", transport_to_string(transport), (void *)user, user->file_path, user->file_length_in_bytes);
        put_file_active_count[transport]++;
    }
    else
    {
        app_os_free(user);
    }

done:
    return status;
}

connector_status_t send_put_request(connector_handle_t handle, int index, connector_transport_t const transport)
{

    connector_status_t status = connector_success;
    char filename[DS_FILE_NAME_LEN];
    int fileindex = put_file_active_count[transport];

    if (put_file_active_count[transport] >= DS_MAX_USER)
    {
        status = connector_invalid_data_range;
        goto done;
    }

#if (defined CONNECTOR_TRANSPORT_TCP)
    if (transport == connector_transport_tcp) fileindex += 0x00;
#endif

#if (defined CONNECTOR_TRANSPORT_UDP)
    if (transport == connector_transport_udp) fileindex += 0x10;
#endif

#if (defined CONNECTOR_TRANSPORT_SMS)
    if (transport == connector_transport_sms) fileindex += 0x20;
#endif


    sprintf(filename, "test/dvt%02X.txt", fileindex);
    status = send_file(handle, index, filename, ds_buffer, (rand() % (DS_DATA_SIZE +1)), transport);


done:
    return status;
}


