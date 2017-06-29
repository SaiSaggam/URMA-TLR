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
#include <semaphore.h>
#include "connector_api.h"
#include "platform.h"

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

connector_callback_status_t app_os_malloc(size_t const size, void ** ptr)
{
    connector_callback_status_t status= connector_callback_abort;

    *ptr = malloc(size);
    ASSERT(*ptr != NULL);
    if (*ptr != NULL)
    {
#if 0
        size_t i;
        uint8_t * thisPtr = *ptr;
        for (i=0; i<size; i++)
        {
            *(thisPtr + i) = 0xCD;
        }
#else
        memset(*ptr, 0xCD, size);
#endif
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



