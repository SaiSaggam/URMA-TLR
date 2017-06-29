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
#ifdef ENV_LINUX
#include <linux/reboot.h>
#include <sys/reboot.h>
#endif
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <semaphore.h>

typedef struct malloc_info{
    void const * ptr;
    size_t length;
    struct malloc_info * prev;
    struct malloc_info * next;
} malloc_info_t;

static malloc_info_t  * malloc_info_list = NULL;
size_t          total_malloc_size = 0;
static sem_t * malloc_sem = NULL;

static void add_malloc_ptr(void const * const ptr, size_t const length)
{
    malloc_info_t   * pMalloc;

    if (malloc_sem == NULL)
    {
        malloc_sem = malloc(sizeof *malloc_sem);
        ASSERT(malloc_sem != NULL);

        if (sem_init(malloc_sem, 0, 1) < 0)
        {
            ASSERT(0);
        }
    }

    sem_wait(malloc_sem);

    pMalloc = malloc(sizeof *pMalloc);
    ASSERT(pMalloc != NULL);

    if (pMalloc != NULL)
    {
        pMalloc->ptr = ptr;
        pMalloc->length = length;

        if (malloc_info_list != NULL)
        {
            malloc_info_list->prev = pMalloc;
        }
        pMalloc->next = malloc_info_list;
        pMalloc->prev = NULL;
        malloc_info_list = pMalloc;

        total_malloc_size += length;
    }
    sem_post(malloc_sem);
}

/* free and remove allocated memory */
static void free_malloc_ptr(void const * const ptr)
{
    malloc_info_t   * pMalloc;

    sem_wait(malloc_sem);

    for (pMalloc = malloc_info_list; pMalloc != NULL; pMalloc = pMalloc->next)
    {
        if (pMalloc->ptr == ptr)
        {
            if (pMalloc->next != NULL) \
            {\
                pMalloc->next->prev = pMalloc->prev;\
            }\
            if (pMalloc->prev != NULL) \
            {\
                pMalloc->prev->next = pMalloc->next;\
            }\
            if (pMalloc == malloc_info_list)\
            {\
                malloc_info_list = pMalloc->next;\
            }\

            total_malloc_size -= pMalloc->length;
            free(pMalloc);
            break;
        }
    }

    ASSERT(pMalloc != NULL);

    sem_post(malloc_sem);
}

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
    connector_callback_status_t status= connector_callback_abort;

    *ptr = malloc(size);
    ASSERT(*ptr != NULL);
    if (*ptr != NULL)
    {
        memset(*ptr, 0xCD, size);
        status = connector_callback_continue;
        add_malloc_ptr(*ptr, size);
    }

    return status;
}

connector_callback_status_t app_os_free(void * const ptr)
{
    ASSERT(ptr != NULL);

    if (ptr != NULL)
    {
        free_malloc_ptr(ptr);
        free(ptr);
    }
    return connector_callback_continue;
}

connector_callback_status_t app_os_realloc(connector_os_realloc_t * const realloc_data)
{
    connector_callback_status_t status = connector_callback_continue;

    realloc_data->ptr = realloc(realloc_data->ptr, realloc_data->new_size);
    if (realloc_data->ptr == NULL)
    {
        APP_DEBUG("app_os_realloc: could not reallocate memory\n");
        status = connector_callback_abort;
    }

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
    int error;

    if (*status == connector_idle)
    {
        unsigned int const timeout_in_microseconds =  100000;
        usleep(timeout_in_microseconds);
    }

    error = sched_yield();
    if (error)
    {
        /* In the Linux implementation this function always succeeds */
        APP_DEBUG("app_os_yield: sched_yield failed with %d\n", error);
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_os_reboot(void)
{
    APP_DEBUG("app_os_reboot!\n");

    /* Note: we must be running as the superuser to reboot the system */
#ifdef ENV_LINUX
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
            status = app_os_realloc(p->ptr);
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


