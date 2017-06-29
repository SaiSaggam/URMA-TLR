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



#include "connector_config.h"

#ifdef APPLICATION_FILE_SYSTEM_SDCARD
#include "sdcard_task.h"
#endif
#ifdef APPLICATION_FILE_SYSTEM_USB
#include "MFS_USB.h"
#include "USB_File.h"
#endif

#include "platform.h"
#include <errno.h>

#ifdef CONNECTOR_FILE_SYSTEM

#if (!defined(APPLICATION_FILE_SYSTEM_USB) && !defined(APPLICATION_FILE_SYSTEM_SDCARD)) || \
	(defined(APPLICATION_FILE_SYSTEM_USB) && defined(APPLICATION_FILE_SYSTEM_SDCARD))
#error You must define which file system device to use in connector_config.h (APPLICATION_FILE_SYSTEM_SDCARD or APPLICATION_FILE_SYSTEM_USB)
#endif

#if !defined CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH
#error Please define CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH in connector_config.h
#endif

#if CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH > 460
#error The maximum supported CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH is 460
#endif

#ifndef APP_MIN_VALUE
#define APP_MIN_VALUE(a,b) (((a)<(b))?(a):(b))
#endif


static connector_callback_status_t app_process_file_error(void ** const error_token, long int const errnum)
{
    connector_callback_status_t status;

    switch(errnum)
    {
        case MQX_EAGAIN:
            status = connector_callback_busy;
            break;

        default:
            status = connector_callback_error;
            *error_token = (void *) errnum;
            break;
    }
    return status;
}

static uint_32 mfs_date_to_epoch(MFS_DATE_TIME_PARAM_PTR date)
{
	TIME_STRUCT epoch;
	DATE_STRUCT mqx_date;
	
	mqx_date.YEAR = ((*date->DATE_PTR & MFS_MASK_YEAR) >> MFS_SHIFT_YEAR) + 1980; /* MS-DOS has different reference! */
	mqx_date.MONTH = (*date->DATE_PTR & MFS_MASK_MONTH) >> MFS_SHIFT_MONTH;
	mqx_date.DAY = (*date->DATE_PTR & MFS_MASK_DAY) >> MFS_SHIFT_DAY;
	mqx_date.HOUR = (*date->TIME_PTR & MFS_MASK_HOURS) >> MFS_SHIFT_HOURS;
	mqx_date.MINUTE = (*date->TIME_PTR & MFS_MASK_MINUTES) >> MFS_SHIFT_MINUTES;
	mqx_date.SECOND = ((*date->TIME_PTR & MFS_MASK_SECONDS) >> MFS_SHIFT_SECONDS) * 2; /* Resolution of seconds is 2 */
	mqx_date.MILLISEC = 0;
	
	if (_time_from_date(&mqx_date, &epoch)) {
		return epoch.SECONDS;	
	} else {
		return ~0; /* Error occurred, probably invalid date*/
	}
}

static connector_callback_status_t app_process_file_hash(connector_file_system_hash_t * const data)
{

    // app_process_file_hash() should not be called if APP_ENABLE_MD5 is not defined

    memset(data->hash_value, 0, data->bytes_requested);
    return connector_callback_continue;
}

/*
 * This function fills all "statbuf" members, path must be Nul-terminated
 */
static _mqx_int get_statbuf(char const * const path, connector_file_system_statbuf_t * statbuf)
{
	MFS_FILE_ATTR_PARAM attributes_param;
	uchar attributes = 0;
    char full_path[50] = {0};
	_mqx_int retval = 0;

    strcpy(full_path, filesystem_info->FS_NAME);
    strcat(full_path, path);
    
    attributes_param.PATHNAME = full_path;
    attributes_param.ATTRIBUTE_PTR = &attributes;
    
    retval = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_GET_FILE_ATTR, &attributes_param);
    if (retval < 0) {
		goto done;
    }

    if (attributes & (MFS_ATTR_DIR_NAME) || attributes == 0) {
    	statbuf->flags = connector_file_system_file_type_is_dir;
    	statbuf->file_size = 0;
    	statbuf->last_modified = 0; /* Directories don't have this field */
    } else if (attributes & MFS_ATTR_ARCHIVE) {
        MQX_FILE_PTR file;

    	file = _io_fopen(full_path, "r");
		if (file == NULL) {
			APP_DEBUG("get_statbuf: _io_fopen(%s, \"r\") failed!", full_path);
			retval = errno;
			goto done;
		}
		statbuf->file_size = file->SIZE;
		statbuf->flags = connector_file_system_file_type_is_reg;
		{
			uint_16 date, time;
			MFS_DATE_TIME_PARAM file_date;
			
			file_date.DATE_PTR = &date;
			file_date.TIME_PTR = &time;
			
			retval = _io_ioctl(file, IO_IOCTL_GET_DATE_TIME, (uint_32 *) &file_date);
			if (retval < 0) {
				goto done;
			}
			
			statbuf->last_modified = mfs_date_to_epoch(&file_date);
		}
		_io_fclose(file);
    } else {
    	statbuf->flags = connector_file_system_file_type_none;
		statbuf->file_size = 0;
		statbuf->last_modified = 0;
    }

done:
	return retval;
}

static connector_callback_status_t app_process_file_stat(connector_file_system_stat_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    _mqx_int result;

    result = get_statbuf(data->path, &data->statbuf);
    if (result < 0)
    {
    	APP_DEBUG("app_process_file_stat: get_statbuf returned %d\n", result);
    	goto done;
    }
	data->hash_algorithm.actual = connector_file_system_hash_none;
done:
    return status;
}

static connector_callback_status_t app_process_file_stat_dir_entry(connector_file_system_stat_dir_entry_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    _mqx_int result;

    result = get_statbuf(data->path, &data->statbuf);
    if (result < 0)
    {
    	APP_DEBUG("app_process_file_stat: get_statbuf returned %d\n", result);
    	goto done;
    }
done:
    return status;
}

#define	CONNECTOR_FS_FIRST_SEARCH	((void *) 0xFF)
#define	CONNECTOR_FS_NEXT_SEARCH	((void *) 0x00)
static connector_callback_status_t app_process_file_opendir(connector_file_system_opendir_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    _mqx_int result = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_CHANGE_CURRENT_DIR, (uint_32 *)data->path);

    if (result < 0) {
    	status = app_process_file_error(&data->errnum, errno);
    } else {
    	MFS_SEARCH_PARAM *search_param = _mem_alloc(sizeof(MFS_SEARCH_PARAM));
    	MFS_SEARCH_DATA *search_data = _mem_alloc(sizeof(MFS_SEARCH_DATA));
    	
    	if (search_param == NULL) {
    		APP_DEBUG("app_process_file_opendir: failed to allocate memory for MFS_SEARCH_PARAM\n", __FILE__, __FUNCTION__, __LINE__);
    		goto done;
    	} else if (search_data == NULL) {
    		APP_DEBUG("app_process_file_opendir: failed to allocate memory for MFS_SEARCH_DATA\n", __FILE__, __FUNCTION__, __LINE__);
    		goto done;
    	}
    	search_param->ATTRIBUTE = MFS_SEARCH_ANY;
    	search_param->WILDCARD = "*";
    	search_param->SEARCH_DATA_PTR = search_data;
    	data->user_context = CONNECTOR_FS_FIRST_SEARCH;
    	data->handle = search_param;
    }

done:
    return status;
}

static connector_callback_status_t app_process_file_readdir(connector_file_system_readdir_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    _mqx_int result;
    MFS_SEARCH_PARAM *search_param = data->handle;
	MFS_SEARCH_DATA *search_data = search_param->SEARCH_DATA_PTR;
	
	if (data->user_context == CONNECTOR_FS_FIRST_SEARCH) {
		result = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_FIND_FIRST_FILE, (uint_32_ptr)search_param);
		data->user_context = CONNECTOR_FS_NEXT_SEARCH;
	} else {
		result = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_FIND_NEXT_FILE, search_data);
	}
	
	if (result == MFS_NO_ERROR) {
		MFS_GET_LFN_STRUCT long_filename_struct;
		
		long_filename_struct.PATHNAME = search_data->NAME;
		long_filename_struct.LONG_FILENAME = data->entry_name;
		long_filename_struct.SEARCH_DATA_PTR = NULL;
		result = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_GET_LFN, (uint_32 *)&long_filename_struct);
		if (result != MFS_NO_ERROR) {
			/* It's not a long filename */	
			memcpy(data->entry_name, search_data->NAME, strlen(search_data->NAME) + 1);
		}
	} else {
		/* No more directory entries */
	}
	
    return status;
}

static connector_callback_status_t app_process_file_closedir(connector_file_system_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    MFS_SEARCH_PARAM *search_param = data->handle;
	MFS_SEARCH_DATA *search_data = search_param->SEARCH_DATA_PTR;
	_mqx_int result;

	result = _mem_free(search_data);
	if (result < 0) {
		status = app_process_file_error(&data->errnum, errno);
	}
	result = _mem_free(search_param);
	if (result < 0) {
		status = app_process_file_error(&data->errnum, errno);
	}
    
	return status;
}


static connector_callback_status_t app_process_file_get_error(connector_file_system_get_error_t * const data)
{
    long int errnum = (long int)data->errnum;

    data->bytes_used = 0;

    if (errnum != 0)
    {
        char * err_str = strerror(errnum);

        data->bytes_used = APP_MIN_VALUE(strlen(err_str), data->bytes_available);
        memcpy(data->buffer, err_str, data->bytes_used);
    }

    switch(errnum)
    {
        case MQX_EACCES:
        case MQX_EPERM:
        case MQX_EROFS:
            data->error_status = connector_file_system_permission_denied;
            break;

        case MQX_ENOMEM:
        case MQX_ENAMETOOLONG:
            data->error_status = connector_file_system_out_of_memory;
            break;

        case MQX_ENOENT:
        case MQX_ENODEV:
        case MQX_EBADF:
            data->error_status = connector_file_system_path_not_found;
            break;

        case MQX_EINVAL:
        case MQX_ENOSYS:
        case MQX_ENOTDIR:
        case MQX_EISDIR:
            data->error_status = connector_file_system_invalid_parameter;
            break;

        case MQX_ENOSPC:
            data->error_status = connector_file_system_insufficient_storage_space;
            break;

        default:
            data->error_status = connector_file_system_unspec_error;
            break;
    }

    return connector_callback_continue;
}

static char const * app_convert_file_open_mode(int const oflag)
{
    if ((oflag & (CONNECTOR_FILE_O_WRONLY | CONNECTOR_FILE_O_RDWR)) == 0) {
        return "r";
    } else if (oflag & CONNECTOR_FILE_O_WRONLY) {
    	return "w"; /* Open a new file in "write-only" mode; overwrite an existing file. */
    } else if (oflag & CONNECTOR_FILE_O_RDWR) {
    	return "r+"; /* Open an existing file in "read-write" mode. */
    } else if (oflag & CONNECTOR_FILE_O_APPEND) {
    	return "a+"; /* Open a file at EOF in "read-write" mode; create the file if it does not exist. */
    } else if (oflag & CONNECTOR_FILE_O_CREAT) {
    	return "n+"; /* Open a new file in "read-write" mode; do nothing if the file already exists. */
    } else if (oflag & CONNECTOR_FILE_O_TRUNC) {
    	return "w+"; /* Open a new file in "read-write" mode; overwrite an existing file. */
    } else if (oflag & CONNECTOR_FILE_O_RDONLY) {
    	return "r"; /* Open an existing file in "read-only" mode. */
    } else {
    	return "r"; /* Open an existing file in "read-only" mode. */
    }
}

static connector_callback_status_t app_process_file_open(connector_file_system_open_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    char const * open_mode = app_convert_file_open_mode(data->oflag);

    MQX_FILE_PTR fd;
    char full_path[50] = {0};
    
    strcpy(full_path, filesystem_info->FS_NAME);
    strcat(full_path, data->path);
    
    fd = _io_fopen(full_path, open_mode);
    		
    if (fd == NULL)
    {
		status = app_process_file_error(&data->errnum, MQX_ENOENT);
        APP_DEBUG("app_process_file_open: _io_fopen returned: %ld\n", __FILE__, __FUNCTION__, __LINE__, fd);
    }

    APP_DEBUG("Open %s, %s, returned %ld\n", data->path, open_mode, fd);

    data->handle = (void *) fd;
    data->user_context = NULL;
    
    return status;
}

static connector_callback_status_t app_process_file_lseek(connector_file_system_lseek_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    MQX_FILE_PTR fd = data->handle;
    int origin;
    _mqx_int offset;
    
    switch(data->origin)
	{
		case connector_file_system_seek_set:
			origin = 0;//SEEK_SET;
			break;

		case connector_file_system_seek_end:
			origin = 1;//SEEK_END;
			break;

		case connector_file_system_seek_cur:
		default:
			origin = 2;//SEEK_CUR;
			break;
	}

    offset = fseek(fd, data->requested_offset, origin);
    if (offset < 0)
    {
		status = app_process_file_error(&data->errnum, errno);
        APP_DEBUG("app_process_file_lseek error: fseek returned: %ld\n", __FILE__, __FUNCTION__, __LINE__, fd);
    }
	data->resulting_offset = (connector_file_offset_t) offset;

	APP_DEBUG("lseek fd %ld, offset %d, origin %d returned %d", fd, data->requested_offset, data->origin, data->resulting_offset);
	
	if (offset < 0)
		APP_DEBUG(", errno %d\n", errno);
	else 
		APP_DEBUG("\n");

    return status;
}

static connector_callback_status_t app_process_file_ftruncate(connector_file_system_truncate_t * const data)
{
    APP_DEBUG("%s:%s:%d:ERROR:app_process_file_ftruncate not implemented yet!!\n", __FILE__, __FUNCTION__, __LINE__);
    return connector_callback_continue;
}

static connector_callback_status_t app_process_file_remove(connector_file_system_remove_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
	uchar attributes;
	MFS_FILE_ATTR_PARAM attributes_param;
    _mqx_int result;
    char full_path[50] = {0};
    
    strcpy(full_path, filesystem_info->FS_NAME);
    strcat(full_path, data->path);
    
    attributes_param.PATHNAME = full_path;
    attributes_param.ATTRIBUTE_PTR = &attributes;

    /* Read attributes to determine if it is a regular file or a subdirectory */
    result = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_GET_FILE_ATTR, &attributes_param);
    if (result != MFS_NO_ERROR) {
		APP_DEBUG("app_process_file_remove failed: %ld\n", __FILE__, __FUNCTION__, __LINE__, result);
		status = app_process_file_error(&data->errnum, errno);
		goto done;
    }

    if (attributes & MFS_ATTR_ARCHIVE) {
    	result = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_DELETE_FILE, (void *)data->path);
    } else if (attributes & MFS_ATTR_DIR_NAME) {
    	result = _io_ioctl(filesystem_info->FS_FD_PTR, IO_IOCTL_REMOVE_SUBDIR, (void *)data->path);
    } else {
        APP_DEBUG("%s is not a file or directory\n", data->path, result, errno);
		status = app_process_file_error(&data->errnum, MQX_EINVAL);
    	goto done;
    }
    
    if (result != MFS_NO_ERROR)
    {
		status = app_process_file_error(&data->errnum, errno);
        goto done;
    }    


done:
    return status;
}

static connector_callback_status_t app_process_file_read(connector_file_system_read_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    MQX_FILE_PTR fd = data->handle;

    _mqx_int result = _io_read(fd, data->buffer, data->bytes_available);

    if (result < 0)
    {
		status = app_process_file_error(&data->errnum, errno);
        APP_DEBUG("read %ld, %u, returned %d, errno %d\n", fd, data->bytes_available, result, errno);
        goto done;
    }

    APP_DEBUG("read %ld, %u, returned %d\n", fd, data->bytes_available, result);
    data->bytes_used = result;

done:
    return status;
}

static connector_callback_status_t app_process_file_write(connector_file_system_write_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    MQX_FILE_PTR fd = data->handle;

    _mqx_int result = _io_write(fd, (void *)data->buffer, data->bytes_available);
    
    if (result < 0)
    {
		status = app_process_file_error(&data->errnum, errno);
        APP_DEBUG("write %ld, %u, returned %d, errno %d\n", fd, data->bytes_available, result, errno);
        goto done;
    }

    APP_DEBUG("write %ld, %u, returned %d\n", fd, data->bytes_available, result);

    data->bytes_used = result;
    
done:
    return status;
}

static connector_callback_status_t app_process_file_close(connector_file_system_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    MQX_FILE_PTR fd = data->handle;
    int result = _io_fclose(fd);

    if (result < 0 && errno == MQX_EIO)
    {
		status = app_process_file_error(&data->errnum, errno);
    }

    APP_DEBUG("close %ld returned %d\n", fd, result);

    /* All application resources, used in the session, must be released in this callback */

    return status;
}

static connector_callback_status_t app_process_file_session_error(connector_file_system_session_error_t * const data)
{
     APP_DEBUG("Session Error %d\n", data->session_error);

    /* All application resources, used in the session, must be released in this callback */
    data->user_context = NULL;
    return connector_callback_continue;
}

connector_callback_status_t app_file_system_handler(connector_request_id_file_system_t const request, void * const data)
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
#endif
