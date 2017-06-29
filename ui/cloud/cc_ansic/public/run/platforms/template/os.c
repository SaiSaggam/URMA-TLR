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

 /**
  * @file
  *  @brief Functions used by Cloud Connector to interface to the OS.
  *
  */
#include "connector_api.h"
#include "platform.h"

/**
 * @brief Cloud Connector snprintf
 *
 * The Cloud Connector uses this function to produce output string according to the format
 * tags with the value of the argument list arg.
 *
 * @param [out] str String where output is written to.
 * @param [in] size Size of the str including the terminating null byte)
 * @param [in] format Tells how to format the various arguments
 * @param [out] ...   A variable argument list of expressions whose values should be printed according
 *                    to the placeholders in the "format" string. If there are more placeholders than
 *                    supplied arguments, the result is undefined. If there are more arguments than
 *                    placeholders, the excess arguments are simply ignored.
 *
 * @retval bytes    The number of characters formatted in the output string (excluding the null byte)
 */
int connector_snprintf(char * const str, size_t const size, char const * const format, ...)
{
    int result = 0;

    UNUSED_ARGUMENT(str);
    UNUSED_ARGUMENT(size);
    UNUSED_ARGUMENT(format);
    return result;
}

/**
 * @brief   Dynamically allocate memory
 *
 * Dynamically allocate memory, if you are not using malloc()
 * from the C library replace the malloc() call to an equivalent
 * call on your system.
 *
 * @param [in] size  Number of bytes to allocate
 *
 * @param [in] ptr  pointer to be filled in with the address of
 *                  the allocated memory
 *
 * @retval connector_callback_continue  Memory was allocated.
 *  
 * @retval connector_callback_busy      Memory was temporarily 
 *          unavailable. 
 * @retval connector_callback_abort     Memory was not allocated and abort Cloud Connector.
 *
 * @see os_free
 * @see @ref malloc API Operating System Callback
 */
connector_callback_status_t app_os_malloc(size_t const size, void ** ptr)
{
    UNUSED_ARGUMENT(size);
    UNUSED_ARGUMENT(ptr);
    return connector_callback_continue;
}

/**
 * @brief   Reallocate memory block
 *
 * Used to increase the size of a memory block previously allocated with @ref app_os_malloc(), if you are not using realloc()
 * from the C library replace the realloc() call to an equivalent call on your system.
 *
 * @param [in,out] realloc_data  Structure containing the old pointer, old size, and new size.
 *                               See @ref connector_os_realloc_t "connector_os_realloc_t" for more information.
 *
 * @retval connector_callback_continue  Memory was reallocated.
 *  
 * @retval connector_callback_abort     Memory was not allocated and abort Cloud Connector.
 *
 * @see os_malloc
 * @see os_free
 * @see @ref realloc API Operating System Callback
 */
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

/**
 * @brief   Free Dynamically allocate memory.
 *
 * Free dynamically allocate memory, if you are not using
 * free() from the C library replace the free() call to an
 * equivalent call on your system.
 *
 * @param [in] ptr  pointer to memory to be freed
 *
 * @retval connector_callback_continue  Memory was freed.
 *
 * @retval connector_callback_abort     Memory was not freed and abort Cloud Connector.
 *                                  @note This free callback may be called again to free other
 *                                  pointer even if it returns connector_callback_abort.
 * @see os_malloc
 * @see @ref free API Operating System Callback
 */
connector_callback_status_t app_os_free(void const * const ptr)
{
    UNUSED_ARGUMENT(ptr);
    return connector_callback_continue;
}

/**
 * @brief   Get the system time.
 *
 * Get the current system time in seconds, this is only used as a reference
 * by Cloud Connector.
 *
 *
 * @param [in] uptime   Current system time in seconds.
 *
 * @retval connector_callback_continue Able to get system time
 *
 * @retval connector_callback_abort    System time unavailable and abort Cloud Connector.
 *
 * @see @ref uptime API Operating System Callback
 */
connector_callback_status_t app_os_get_system_time(unsigned long * const uptime)
{
    UNUSED_ARGUMENT(uptime);
    return connector_callback_continue;
}

/**
 * @brief   Yield or relinquish for other task execution.
 *
 * Yield or relinquish to run other task. This is called
 * to let other task to be executed when connector_run is called.
 * Cloud Connector calls this callback if Cloud Connector is busy and is not calling
 * receive callback
 *
 * @param [in] status
 *                     - If status is connector_idle, Cloud Connector is idling. No message is processed.
 *                     - If status is connector_working, Cloud Connector is processing a message and should be called at the earliest possible time.
 *                     - If status is connector_pending, Cloud Connector is busy or waiting to process a message and relinquishes other task to execution.
 *                     - If status is connector_active, Cloud Connector is busy or waiting to process a message and should be called at the earliest possible time.
 *
 * @retval connector_callback_continue  It successfully yield for other task execution.
 * @retval connector_callback_abort     Abort Cloud Connector.
 *
 * @see @ref yield API Operating System Callback
 */
connector_callback_status_t app_os_yield(connector_status_t const * const status)
{
    UNUSED_ARGUMENT(status);

    if (*status == connector_idle)
    {
        /* should relinquish for other task execution */
    }

    return connector_callback_continue;
}

/**
 * @brief   Reboot the system
 *
 * Reboot the system. This is called to reboot the system and should not return.
 *
 *
 * @retval connector_callback_continue  Continue Cloud Connector
 * @retval connector_callback_abort     Abort Cloud Connector.
 *
 * @see @ref reboot API Operating System Callback
 */
static connector_callback_status_t app_os_reboot(void)
{

    /* should not return from rebooting the system */
    return connector_callback_continue;
}

/**
 * @cond DEV
 */
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

    case connector_request_id_os_realloc:
        {
            connector_os_realloc_t * const p = data;
            status = app_os_realloc(p);
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
/**
 * @endcond
 */


