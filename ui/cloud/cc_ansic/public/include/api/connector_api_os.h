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

#ifndef CONNECTOR_API_OS_H
#define CONNECTOR_API_OS_H

 /**
 * @defgroup connector_os_request_t OS Request IDs
 * @{
 */
 /**
 * Operating System Request ID passed to the application's callback for operating system interface.
 * The class id for this connector_os_request_t is connector_class_id_operating_system.
 */
typedef enum {
    connector_request_id_os_malloc,            /**< Callback used to dynamically allocate memory. */
    connector_request_id_os_free,              /**< Callback is called to free previous allocated memory. */
    connector_request_id_os_realloc,           /**< Callback is called to reallocate data in a different size memory position. */
    connector_request_id_os_system_up_time,    /**< Callback is called to return system up time in seconds. It is the time that a device has been up and running. */
    connector_request_id_os_yield,             /**< Callback is called with @ref connector_status_t to relinquish for other task to run when @ref connector_run is used. */
    connector_request_id_os_reboot            /**< Callback is called to reboot the system. */
} connector_request_id_os_t;
/**
* @}
*/

/**
* @defgroup connector_os_malloc_t Malloc 
* @{ 
*/
/**
* Structure passed to connector_request_id_os_malloc callback. 
*/
typedef struct {
    size_t CONST size;            /**< Number of bytes to be allocated */
    void * ptr;                   /**< Pointer to allocated memory */
} connector_os_malloc_t;
/**
* @}
*/

/**
* @defgroup connector_os_free_t Free
* @{ 
*/
/**
* Structure passed to connector_request_id_os_free callback. 
*/
typedef struct {
    void * CONST ptr;              /**< Pointer to memory to free */
} connector_os_free_t;
/**
* @}
*/

/**
* @defgroup connector_os_realloc_t Reallocate memory.
* @{
*/
/**
* Structure passed to connector_request_id_realloc callback.
*/
typedef struct {
    size_t CONST old_size;   /**< Number of bytes already allocated in "ptr". */
    size_t CONST new_size;   /**< Number of bytes to be allocated in "ptr". */
    void * ptr;              /**< Pointer to allocated memory. This field might be changed. */
} connector_os_realloc_t;

/**
* @}
*/

/**
* @defgroup connector_os_system_up_time_t System Uptime 
* @{
*/
/**
* Structure passed to connector_request_id_os_system_up_time 
* callback. 
*/
typedef struct {
    unsigned long sys_uptime;             /**< Returned system uptime */
} connector_os_system_up_time_t;
/**
* @}
*/

/**
* @defgroup connector_os_yield_t  Yield Request
* @{
*/
/**
* Structure passed to connector_request_id_os_yield callback. 
*/
typedef struct {
    connector_status_t CONST status;      /**< System status used to decide how to yield */
} connector_os_yield_t;
/**
* @}
*/

#if !defined _CONNECTOR_API_H
#error  "Illegal inclusion of connector_api_os.h. You should only include connector_api.h in user code."
#endif

#else
#error  "Illegal inclusion of connector_api_os.h. You should only include connector_api.h in user code."
#endif
