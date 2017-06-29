/*
 * Copyright (c) 2014 Digi International Inc.,
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
#ifdef ENV_LINUX
#include <linux/reboot.h>
#include <sys/reboot.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int connector_snprintf(char * const str, size_t const size, char const * const format, ...)
{
    va_list args;
    int result;

    va_start(args, format);

#if __STDC_VERSION__ >= 199901L
    result = vsnprintf(str, size, format, args);
#else
    /**************************************************************************************
    * NOTE: This is an example snprintf() implementation for environments                 *
    * that do not provide one.                                                            *
    * The following buffer should hold the longest possible string that can be generated. *
    * Consider the common case to be only one format specifier at once.                   *
    * WARNING: If an application is using double Data Point types, this value can be as   *
    * big as:                                                                             *
    *                 max_digits = 3 + DBL_MANT_DIG - DBL_MIN_EXP                         *
    * The above expression evaluates to 1077 in a 64-bit Linux machine for GCC 4.8.1.     *
    **************************************************************************************/
    #define SAFE_BUFFER_BYTES 64

    if (size >= SAFE_BUFFER_BYTES)
    {
        result = vsprintf(str, format, args);
    }
    else
    {
        char local_buffer[SAFE_BUFFER_BYTES];
        ssize_t const bytes_needed = vsprintf(local_buffer, format, args);

        if (bytes_needed < (ssize_t)size)
        {
            memcpy(str, local_buffer, bytes_needed + 1); /* Don't forget the \0 */
        }
        result = bytes_needed;
    }
    #undef SAFE_BUFFER_BYTES
#endif
    va_end(args);

    return result;
}

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

connector_callback_status_t app_os_realloc(connector_os_realloc_t * const realloc_data)
{
    connector_callback_status_t status = connector_callback_continue;
    void * const reallocated = realloc(realloc_data->ptr, realloc_data->new_size);

    if (reallocated == NULL)
    {
        APP_DEBUG("app_os_realloc: could not reallocate memory\n");
        status = connector_callback_abort;
        goto done;
    }
    realloc_data->ptr = reallocated;

done:
    return status;
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
        unsigned int const timeout_in_microseconds =  100000;
        usleep(timeout_in_microseconds);
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_os_reboot(void)
{
    APP_DEBUG("app_os_reboot!\n");

#ifdef ENV_LINUX
    /* Note: we must be running as the superuser to reboot the system */
    sync();
    reboot(LINUX_REBOOT_CMD_RESTART);
#endif
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

    case connector_request_id_os_realloc:
        {
            connector_os_realloc_t * p = data;
            status = app_os_realloc(p);
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


