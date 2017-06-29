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
 *  @brief Routines which implement the Cloud
 *         Connector file system
 *  
 *  @ref CONNECTOR_FILE_SYSTEM.
 */

#include "connector_api.h"
#include "platform.h"

/**
 * @brief   Open a File
 *
 * This routine opens a file for the specified path
 *
 * @param data  Pointer to a connector_file_system_open_t 
 *              data structure
 *
 * @retval connector_callback_continue	File opened successfully.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_open(connector_file_system_open_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Seek File Position
 *
 * This routine sets the offset for an open file.
 *
 * @param data  Pointer to a connector_file_system_lseek_t 
 *              data structure
 *
 * @retval connector_callback_continue	Offset set successfully.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_lseek(connector_file_system_lseek_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}


/**
 * @brief   Read File Data
 *
 * This routine reads data from a file to be sent to Device 
 * Cloud.
 *
 * @param data  Pointer to a connector_file_system_read_t data 
 *              structure
 *
 * @retval connector_callback_continue	Number of data bytes
 *                                  was read successfully or
 *                                  end-of-file.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_read(connector_file_system_read_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Write File Data
 *
 * This routine writes data received from Device Cloud by 
 * to a file.
 *
 * @param data  Pointer to a connector_file_system_write_t 
 *
 * @retval connector_callback_continue	Number of data bytes was  
 *                                  written successfully
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_write(connector_file_system_write_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Truncate an Open File.
 *
 * This routine truncates an open file to a specified length. 
 *
 * @param data  Pointer to a connector_file_system_truncate_t 
 *
 * @retval connector_callback_continue	File truncated successfully.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_ftruncate(connector_file_system_truncate_t * const data)

{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Close a File.
 *
 * This routine closes a file. 
 *
 * @param data  Pointer to a connector_file_system_truncate_t 
 *              data structure
 *
 * @retval connector_callback_continue	File truncated successfully.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_close(connector_file_system_close_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Remove a File.
 *
 * This routine removes a file for the specified path
 *
 * @param data  Pointer to a connector_file_system_remove_t data 
 *              structure
 *
 * @retval connector_callback_continue	File removed successfully.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_remove(connector_file_system_remove_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Get File Status
 *
 * This routine gets status for the specified path
 *
 * @param data  Pointer to a connector_file_system_stat_t data 
 *              structure
 *
 * @retval connector_callback_continue	Operation was successful.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_stat(connector_file_system_stat_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Get Directory Entry Status
 *
 * This routine gets status for the specified directory entry 
 * path 
 *
 * @param data  Pointer to a 
 *              connector_file_system_stat_dir_entry_t data structure 
 *
 * @retval connector_callback_continue	Operation was successful.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_stat_dir_entry(connector_file_system_stat_dir_entry_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Open a Directory
 *
 * This routine opens a directory for the specified path
 *
 * @param data  Pointer to a connector_file_system_opendir_t 
 *              data structure
 *
 * @retval connector_callback_continue	Directory opened 
 *                                  successfully.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_opendir(connector_file_system_opendir_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Read Next Directory Entry
 *
 * This routine reads next directory entry
 *
 * @param data  Pointer to a connector_file_system_readdir_t 
 *              data structure
 *
 * @retval connector_callback_continue	Next entry 
 *                                  retrieved or no more
 *                                  entries.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_readdir(connector_file_system_readdir_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Close a Directory
 *
 * This routine opens a directory for the specified path
 *
 * @param data  Pointer to a connector_file_system_close_t 
 *              data structure
 *
 * @retval connector_callback_continue	Directory closed
 *                                  successfully.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_closedir(connector_file_system_closedir_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Get File Hash Value
 *
 * This routine returns hash value for the specified path and 
 * hash algorithm
 *
 * @param data  Pointer to a connector_file_system_hash_t 
 *              data structure
 *
 * @retval connector_callback_continue	Hash value returned.
 * @retval connector_callback_busy 		Busy. The routine will be 
 *                                  called again.
 * @retval connector_callback_error     An error has occurred,
 *                                  Application-defined error
 *                                  code is returned in errnum.
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_hash(connector_file_system_hash_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Get Error Status and Description
 *
 * This routine returns error status and description for errnum 
 * returned in a previous callback 
 *
 * @param data  Pointer to a connector_file_system_get_error_t 
 *              data structure
 *
 * @retval connector_callback_continue	Success 
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_get_error(connector_file_system_get_error_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Inform of a Session Error
 *
 * This routine informs of a file system session error, 
 * which might be caused by network communication problems,
 * session timeout, insufficient memory, etc. 
 *
 * @param data  Pointer to a 
 *              connector_file_system_session_error_t
 *              data structure
 *
 * @retval connector_callback_continue	Continue 
 * @retval connector_callback_abort     The application aborts 
 *                                  Cloud Connector.
 */
connector_callback_status_t app_process_file_session_error(connector_file_system_session_error_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}


/**
 * @cond DEV
 */

connector_callback_status_t app_file_system_handler(connector_request_id_file_system_t const request,
                                                    void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request)
    {
        case connector_request_id_file_system_open:
            status = app_process_file_open(data);
            break;

        case connector_request_id_file_system_read:
            status = app_process_file_read(data);
            break;

        case connector_request_id_file_system_write:
            status = app_process_file_write(data);
            break;

        case connector_request_id_file_system_lseek:
            status = app_process_file_lseek(data);
            break;

        case connector_request_id_file_system_close:
            status = app_process_file_close(data);
            break;

        case connector_request_id_file_system_ftruncate:
            status = app_process_file_ftruncate(data);
            break;

        case connector_request_id_file_system_remove:
            status = app_process_file_remove(data);
            break;

        case connector_request_id_file_system_stat:
            status = app_process_file_stat(data);
            break;

        case connector_request_id_file_system_stat_dir_entry:
            status = app_process_file_stat_dir_entry(data);
            break;

        case connector_request_id_file_system_opendir:
            status = app_process_file_opendir(data);
            break;

        case connector_request_id_file_system_readdir:
            status = app_process_file_readdir(data);
            break;

        case connector_request_id_file_system_closedir:
            status = app_process_file_closedir(data);
            break;

        case connector_request_id_file_system_get_error:
            status = app_process_file_get_error(data);
            break;

        case connector_request_id_file_system_hash:
            status = app_process_file_hash(data);
            break;

        case connector_request_id_file_system_session_error:
            status = app_process_file_session_error(data);
            break;

        default:
            status = connector_callback_unrecognized;
            APP_DEBUG("Unsupported file system request %d\n", request);
    }

    return status;
}

/**
 * @endcond
 */

