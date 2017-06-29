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
#include "connector_api.h"
#include "platform.h"
#include "plain_socket.h"
#include "tlr_print.h"
#include "getlinefd.h"
#include "config_mgr.h"
#include "ifuptime_lib.h"
#include "show_parse_helper.h"

#include <stdlib.h>
#include <string.h>

#include <unittest_support.h>

static cc_msg_type_t message_to_type(char const * const message)
{
    cc_msg_type_t msg_type;

    if (strstr(message, cc_msg_show_ascii) != NULL)
    {
        msg_type = cc_msg_type_show_ascii;
    }
    else if (strstr(message, cc_msg_show_json) != NULL)
    {
        msg_type = cc_msg_type_show_json;
    }
    else if (strstr(message, cc_msg_restart) != NULL)
    {
        msg_type = cc_msg_type_restart;
    }
    else
    {
        msg_type = cc_msg_type_unknown;
    }

    return msg_type;
}

static cc_msg_type_t process_client_request(int const socket_fd)
{
    char * line = NULL;
    cc_msg_type_t msg_type = cc_msg_type_unknown;

    {
        size_t len = 0;
        struct timeval timeout = {.tv_sec = CC_STATUS_MSG_TIMEOUT};

        if (getlinefd_timeout(&line, &len, socket_fd, &timeout) <= 0) goto done;
    }

    {
        char const * const message = strtok(line, "\n");

        if (message == NULL) goto done;

        msg_type = message_to_type(message);
    }

done:
    free(line);
    return msg_type;
}

static void print_uptime(int const fd, time_t const start)
{
    char uptime_str[sizeof "ddddd Days, hh Hours, mm Minutes, ss Seconds"];
    uint64_t const uptime = time(NULL) - start;

    format_ifuptime(uptime, uptime_str, sizeof uptime_str);
    dprintf(fd, "\n Uptime    : %s\n", uptime_str);
}

static void cc_show_ascii(int const fd, cloud_status_t const * const status_ptr)
{
    dprintf(fd, " Device Cloud Status\n");
    dprintf(fd, " -------------------\n\n");

    dprintf(fd, " Status    : %s\n", status_ptr->connected ? "Connected" : "Disconnected");
    dprintf(fd, " Server    : %s\n", status_ptr->server_name);
    dprintf(fd, " Device ID : %s\n", status_ptr->device_id_str);

    if (!status_ptr->connected) goto done;

    print_uptime(fd, status_ptr->start_time);

    dprintf(fd, "\n");
    dprintf(fd, "                  %-24s%-24s\n", "Received", "Sent");
    dprintf(fd, "                  %-24s%-24s\n", "--------", "----");
    dprintf(fd, " Packets          %-24" PRIu64 "%-20" PRIu64 "\n", status_ptr->packets.received, status_ptr->packets.sent);
    dprintf(fd, " Bytes            %-24" PRIu64 "%-20" PRIu64 "\n", status_ptr->bytes.received, status_ptr->bytes.sent);

done:
    dprintf(fd, "\n");
}

static void add_element_to_show_object(cloud_status_t const * const status_ptr, tlr_show_object_t * const show_object, config_state_cloud_t const element_id)
{
    unsigned int uptime;
    void const * ptr = NULL;
    config_state_cloud_status_t state;

    switch (element_id)
    {
        case config_state_cloud_status:
            state = status_ptr->connected ? config_state_cloud_status_connected : config_state_cloud_status_disconnected;
            ptr = &state;
            break;

        case config_state_cloud_server:
            ptr = status_ptr->server_name;
            break;

        case config_state_cloud_deviceid:
            ptr = status_ptr->device_id_str;
            break;

        case config_state_cloud_uptime:
            uptime = status_ptr->connected ? (unsigned int)(time(NULL) - status_ptr->start_time) : 0;
            ptr = &uptime;
            break;

        case config_state_cloud_rx_bytes:
            ptr = &status_ptr->bytes.received;
            break;

        case config_state_cloud_rx_packets:
            ptr = &status_ptr->packets.received;
            break;

        case config_state_cloud_tx_bytes:
            ptr = &status_ptr->bytes.sent;
            break;

        case config_state_cloud_tx_packets:
            ptr = &status_ptr->packets.sent;
            break;
    }

    tlr_show_object_add_element(show_object, element_id, ptr);
}

static void add_elements_to_show_object(cloud_status_t const * const status_ptr, tlr_show_object_t * const show_object)
{
    unsigned int index;
    size_t const instance = FIRST_INSTANCE;
    config_mgr_group_t const * const group = tlr_get_group_ptr(config_state_cloud, instance, config_mgr_get_group);

    if (group == NULL) goto done;

    for (index = 0; index < group->elements.count; index++)
    {
        add_element_to_show_object(status_ptr, show_object, group->elements.element[index].id);
    }

done:
    return;
}

static void cc_show_json(int const fd, cloud_status_t const * const status_ptr)
{
    size_t const instance = FIRST_INSTANCE;
    tlr_show_object_t * const show_object = tlr_show_object_create(config_state_cloud, instance);

    if (show_object == NULL)
    {
        APP_DEBUG("failed to create show object\n");
        goto done;
    }

    add_elements_to_show_object(status_ptr, show_object);
    dprintf(fd, "%s\n", tlr_show_object_to_json_string(show_object));
    tlr_show_object_destroy(show_object);

done:
    return;
}

STATIC_FUNCTION void cc_show_callback(int const fd, void * const user_context)
{
    cloud_status_t * const status_ptr = user_context;
    cc_msg_type_t const msg_type = process_client_request(fd);

    switch (msg_type)
    {
        case cc_msg_type_show_ascii:
            cc_show_ascii(fd, status_ptr);
            break;

        case cc_msg_type_show_json:
            cc_show_json(fd, status_ptr);
            break;

        case cc_msg_type_restart:
            status_ptr->config_changed = true;
            cc_set_restart_cloud();
            break;

        case cc_msg_type_unknown:
            APP_DEBUG("received unknown message\n");
            break;
    }

    close(fd);
}

bool cc_socket_init(void)
{
    cloud_status_t * const status_ptr = cc_get_cloud_status();
	plain_socket_context_st * const context = open_plain_socket_context(CC_STATUS_SOCKET_NAME, true, cc_show_callback, status_ptr);
	bool success = (context != NULL);

	if (!success)
    {
	    APP_DEBUG("failed to open plain socket context\n");
    }

	cc_clear_cloud_status();

	return success;
}
