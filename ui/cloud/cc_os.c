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
****************************************************************************/
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

int connector_snprintf(char * const str, size_t const size, char const * const format, ...)
{
    va_list args;
    int result;

    va_start(args, format);

    result = vsnprintf(str, size, format, args);

    va_end(args);

    return result;
}

connector_callback_status_t app_os_malloc(size_t const size, void ** ptr)
{
    connector_callback_status_t status;

    *ptr = malloc(size);
    if (*ptr != NULL)
    {
        status = connector_callback_continue;
    }
    else
    {
        status = connector_callback_abort;
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
    time_t present_time;

    time(&present_time);
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
    /* @TODO: yet to implement so return error for now. */
    return connector_callback_error;
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


