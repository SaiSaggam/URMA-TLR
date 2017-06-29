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

#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include "connector_api.h"
#include "platform.h"
#include "application.h"


connector_callback_status_t app_os_malloc(size_t const size, void ** ptr)
{
    connector_callback_status_t status = connector_callback_abort;

    *ptr = malloc(size);
    if (*ptr != NULL)
    {
        memset(*ptr, 0xCD, size);
        status = connector_callback_continue;
    }
    else
    {
        APP_DEBUG("app_os_malloc: Failed to malloc\n");
    }

    return status;
}

connector_callback_status_t app_os_free(void const * const ptr)
{
    void * const free_ptr = (void *)ptr;

    if (free_ptr != NULL)
    {
        free(free_ptr);
    }
    else
    {
        APP_DEBUG("app_os_free: called with NULL\n");
    }

    return connector_callback_continue;
}

connector_callback_status_t app_os_get_system_time(unsigned long * const uptime)
{
	static time_t start_system_up_time;
	time_t present_time;

    time(&present_time);

    if (start_system_up_time == 0)
       start_system_up_time = present_time;

    present_time -= start_system_up_time;
	*uptime = (unsigned long) present_time;

    return connector_callback_continue;
}

connector_callback_status_t app_os_yield(connector_status_t const * const status)
{
    if (*status == connector_idle)
    {
        unsigned int const timeout_in_microseconds =  1000000;
        usleep(timeout_in_microseconds);
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_os_reboot(void)
{

    APP_DEBUG("Reboot from server\n");
    /* should not return from rebooting the system */
    if (delay_receive_state != no_delay_receive && reboot_state == no_reboot_received)
        reboot_state = reboot_received;
    return connector_callback_continue;
}

connector_callback_status_t app_os_handler(connector_request_id_os_t const request,
                                           void * const data)
{
    connector_callback_status_t status;

    switch (request)
    {
    case connector_request_id_os_malloc:
        {
            connector_os_malloc_t * p = data;
            status = app_os_malloc(p->size, &p->ptr);
        }
        break;

    case connector_request_id_os_free:
        {
            connector_os_free_t * p = data;
            status = app_os_free(p->ptr);
        }
        break;

    case connector_request_id_os_system_up_time:
        {
            connector_os_system_up_time_t * p = data;
            status = app_os_get_system_time(&p->sys_uptime);
        }
        break;

    case connector_request_id_os_yield:
        {
            connector_os_yield_t * p = data;
            status = app_os_yield(&p->status);
        }
        break;

    case connector_request_id_os_reboot:
        status = app_os_reboot();
        break;

    default:
        APP_DEBUG("app_os_handler: unrecognized request [%d]\n", request);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}

