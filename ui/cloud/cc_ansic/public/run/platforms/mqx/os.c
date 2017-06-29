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

#include <mqx.h>
#include "connector_api.h"
#include "platform.h"
#include "connector_debug.h"
#include "os_support.h"
#include <stdio.h>
#include <stdarg.h>

int connector_malloc_failures = 0;
unsigned long start_system_up_time = 0;

int connector_snprintf(char * const str, size_t const size, char const * const format, ...)
{
    va_list args;
    int result;

    va_start(args, format);

#if __STDC_VERSION__ >= 199901L
    result = vsnprintf(str, size, format, args);
#else
    /*************************************************************************
     * NOTE: Decided to have 64 bytes here considering following assumption  *
     * 1. In the worst case, only one format specifier will be used.         *
     * 2. Maximum of 48 bytes are used to represent a single precision value *
     *************************************************************************/
    #define SAFE_BUFFER_BYTES 64

    if (size >= SAFE_BUFFER_BYTES)
    {
        result = vsprintf(str, format, args);
    }
    else
    {
        char local_buffer[SAFE_BUFFER_BYTES];
        size_t const bytes_needed = vsprintf(local_buffer, format, args);

        result = (bytes_needed < size) ? bytes_needed : size - 1;
        memcpy(str, local_buffer, result);
        str[result] = '\0';
        result = bytes_needed;
    }
    #undef SAFE_BUFFER_BYTES
#endif
    va_end(args);

    return result;
}

static connector_callback_status_t app_os_malloc(size_t const size, void ** ptr)
{
    connector_callback_status_t status = connector_callback_abort;

    *ptr = _mem_alloc_zero(size);
    if (*ptr != NULL)
    {
        status = connector_callback_continue;
    }
    else
    {
        APP_DEBUG ("os_malloc: failed\n");
        connector_malloc_failures ++;
    }

    return status;
}

static connector_callback_status_t app_os_free(void * const ptr)
{
    connector_callback_status_t status = connector_callback_abort;

    ASSERT(ptr != NULL);
    if (ptr != NULL)
    {
        unsigned int const result = _mem_free(ptr);

        if (result)
        {
            APP_DEBUG("os_free: _mem_free failed [%d]\n");
        }
        else 
        {
        	status = connector_callback_continue;
        }
    }
    return status;
}

connector_callback_status_t app_os_get_system_time(unsigned long * const uptime)
{
    TIME_STRUCT curtime;

    _time_get(&curtime);
    if (start_system_up_time == 0)
        start_system_up_time = curtime.SECONDS;

    /* Up time in seconds */
    *uptime = curtime.SECONDS - start_system_up_time;

    return connector_callback_continue;
}

static connector_callback_status_t app_os_yield(void)
{
    _sched_yield();

    return connector_callback_continue;
}

connector_callback_status_t app_os_reboot(void)
{
	APP_DEBUG("Reboot request from Device Cloud!\n");
	ecc_software_reset();
	return connector_callback_continue;
}


connector_callback_status_t app_os_handler(connector_request_id_os_t const request, void * const data)
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
            status = app_os_yield();
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
