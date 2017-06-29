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

#include "connector_debug.h"
#include "connector_api.h"
#include "platform.h"

#ifdef CONNECTOR_FILE_SYSTEM

#include  <fs.h>
#include  <fs_file.h>
#include  <fs_dir.h>
#include  <fs_entry.h>
#include  <fs_vol.h>

/* Configure Volume Name
   Currently only one volume at a time is supported. Available ones:
   APP_CFG_FS_SD_CARD_EN
   APP_CFG_FS_RAM_EN (Be aware of memory constrains)
*/
#if (APP_CFG_FS_SD_CARD_EN == DEF_ENABLED)
#define VOLUME_NAME "sdcard:0:\\"
#elif (APP_CFG_FS_RAM_EN == DEF_ENABLED)
#define VOLUME_NAME "ram:0:\\"
#else
#error You have to enable a uC/FS: DRIVER in app_cfg.h 
#endif


#ifndef APP_MIN_VALUE
#define APP_MIN_VALUE(a,b) (((a)<(b))?(a):(b))
#endif

#if CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH > FS_CFG_MAX_PATH_NAME_LEN
#error CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH > FS_CFG_MAX_PATH_NAME_LEN. Please review these definitions
#endif

typedef struct
{
    FS_DIR * p_dir;
    FS_DIR_ENTRY dir_entry;
} app_dir_data_t;

static connector_callback_status_t arrange_path(char **pstr_dest, const char *pstr_cat)
{
    size_t n, total_size;
    
    total_size = strlen(pstr_cat) + strlen(VOLUME_NAME) + 1;
    
    if ( total_size >= FS_CFG_MAX_FULL_NAME_LEN )
    {
        APP_DEBUG("Too long path\n");
        return connector_callback_error;
    }
    
    *pstr_dest = malloc (total_size);
    if (*pstr_dest == NULL)
    {
        APP_DEBUG("Error in malloc for path\n");
        return connector_callback_error;
    }
    
    strcpy(*pstr_dest, VOLUME_NAME);
    if (pstr_cat[0] == '/')
        strcat(*pstr_dest, &pstr_cat[1]);  //remove leading '/'
    else
        strcat(*pstr_dest, pstr_cat);
 
    for(n=0;n<strlen(*pstr_dest);n++)
    {
        if((*pstr_dest)[n] == '/')
            (*pstr_dest)[n] = '\\';            
    }
    
    return connector_callback_continue;
}

static connector_callback_status_t app_process_file_error(void ** const error_token, long int const errnum)
{
    connector_callback_status_t status;

    switch(errnum)
    {
/*  Not available
    #if EAGAIN != EWOULDBLOCK
        case EWOULDBLOCK:
    #endif
        case FS_EAGAIN:
            status = connector_callback_busy;
            break;
*/
        default:
            status = connector_callback_error;
            *error_token = (void *) errnum;
            break;
    }
    return status;
}

static FS_FLAGS app_convert_file_open_mode(int const oflag)
{
    FS_FLAGS result = 0;

    if (oflag & CONNECTOR_FILE_O_WRONLY) result |= FS_FILE_ACCESS_MODE_WR;
    if (oflag & CONNECTOR_FILE_O_RDWR)   result |= FS_FILE_ACCESS_MODE_RDWR;
    if (oflag & CONNECTOR_FILE_O_APPEND) result |= FS_FILE_ACCESS_MODE_APPEND;
    if (oflag & CONNECTOR_FILE_O_CREAT)  result |= FS_FILE_ACCESS_MODE_CREATE;
    if (oflag & CONNECTOR_FILE_O_TRUNC)  result |= FS_FILE_ACCESS_MODE_TRUNCATE;

    if ((oflag & (CONNECTOR_FILE_O_WRONLY | CONNECTOR_FILE_O_RDWR)) == 0)
        result |= FS_FILE_ACCESS_MODE_RD;

    return result;
}

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
static connector_callback_status_t app_process_file_open(connector_file_system_open_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_FLAGS oflag = app_convert_file_open_mode(data->oflag);
    FS_FILE *p_file;
    FS_ERR fs_err;
    char *full_path=NULL;
    
    if ((status = arrange_path(&full_path, data->path)) != connector_callback_continue)
        return status;
    
    p_file = FSFile_Open(full_path, oflag, &fs_err);
          
    APP_DEBUG("Open file %s, oflag 0x%x, returned p_file 0x%x", data->path, oflag, p_file);
    
    if (fs_err != FS_ERR_NONE)
    {
        status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
    }
    
    APP_DEBUG("\n");

    data->handle = (void *) p_file;

    if (full_path != NULL)
        free(full_path);
   
    return status;
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
static connector_callback_status_t app_process_file_lseek(connector_file_system_lseek_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_FILE *p_file = data->handle;
    FS_ERR fs_err;
    FS_STATE  origin;
    FS_FILE_SIZE result = 0;

    APP_DEBUG("lseek p_file 0x%x, offset %d, origin %d", p_file, 
              data->requested_offset, data->origin);
        
    switch(data->origin)
    {
        case connector_file_system_seek_set:
            origin = FS_FILE_ORIGIN_START;
            break;

        case connector_file_system_seek_end:
            origin = FS_FILE_ORIGIN_END;
            break;

        case connector_file_system_seek_cur:
        default:
            origin = FS_FILE_ORIGIN_CUR;
            break;
    }
    
    FSFile_PosSet(p_file, data->requested_offset, origin, &fs_err);
    if (fs_err == FS_ERR_NONE)
    {
        result = FSFile_PosGet(p_file, &fs_err);
    }
    data->resulting_offset = (connector_file_offset_t) result;

    APP_DEBUG(", returned %d", data->resulting_offset);

    if (fs_err != FS_ERR_NONE)
    {
        status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d",fs_err);
    }
  
    APP_DEBUG("\n");
    
    return status;
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
static connector_callback_status_t app_process_file_read(connector_file_system_read_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_FILE* p_file = data->handle;
    FS_ERR fs_err;

    APP_DEBUG("read p_file 0x%x, %u", p_file, data->bytes_available);
    
    CPU_SIZE_T result = FSFile_Rd(p_file, data->buffer, data->bytes_available, &fs_err);
    data->bytes_used = result;
    
    APP_DEBUG(", returned %d", result);
    
    if (fs_err != FS_ERR_NONE)
    {
		status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
    }

    APP_DEBUG("\n");

    return status;
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
static connector_callback_status_t app_process_file_write(connector_file_system_write_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_FILE* p_file = data->handle;
    FS_ERR fs_err;
    CPU_SIZE_T result;

#if 1
    /*
        We have found problem on the Filesystem or the SDCard driver that let 
        us do for now following operations:
            - Align to 32bit
            - Split into 512 byte blocks
    */
    char * p_buf = (char*)data->buffer;
    CPU_SIZE_T remaining = data->bytes_available;
    char char_to_write;
    
    APP_DEBUG("write p_file 0x%x, %u", p_file, data->bytes_available);
    
    data->bytes_used = 0;

    /* Align to 32bit */
    while (  ((CPU_INT32U)p_buf) % 4)
    {
        char_to_write = *p_buf;
        result = FSFile_Wr(p_file, (void*)&char_to_write,1, &fs_err);
        if ((result != 1) || (fs_err != FS_ERR_NONE))
        {
            APP_DEBUG("write ERROR p_file 0x%x, offset %u, to write 1", p_file, data->bytes_used);
            goto done;
        }
        remaining -= result;
        data->bytes_used += result;
        p_buf += result;
    }
    APP_DEBUG(", aligned %u", data->bytes_used);

    /* Split into 512 byte blocks */
    while (remaining)
    {
        result = FSFile_Wr(p_file, (void*)p_buf, (remaining>512)?512:remaining, &fs_err);
        if ((result != ((remaining>512)?512:remaining)) || (fs_err != FS_ERR_NONE))
        {
            APP_DEBUG("write ERROR p_file 0x%x, offset %u, to write 512", p_file, data->bytes_used);
            goto done;
        }
        remaining -= result;
        data->bytes_used += result;
        p_buf += result;
    }      
#else
    APP_DEBUG("write p_file 0x%x, %u", p_file, data->bytes_available);
    
    result = FSFile_Wr(p_file, (void*)data->buffer, data->bytes_available, &fs_err);
    data->bytes_used = result;
#endif
    
done:
    APP_DEBUG(", returned %d", data->bytes_used);
    
    if (fs_err != FS_ERR_NONE)
    {
		status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG("fs_err %d", fs_err);
    }

    APP_DEBUG("\n");

    return status;
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
static connector_callback_status_t app_process_file_ftruncate(connector_file_system_truncate_t * const data)

{
    connector_callback_status_t status = connector_callback_continue;
    FS_FILE* p_file = data->handle;
    FS_ERR fs_err;

    APP_DEBUG("ftruncate p_file 0x%x, %u", p_file, data->length_in_bytes);
    
    FSFile_Truncate(p_file, data->length_in_bytes, &fs_err);
    
    if (fs_err != FS_ERR_NONE)
    {
		status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
    }

    APP_DEBUG("\n");

    return status;
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
static connector_callback_status_t app_process_file_close(connector_file_system_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_FILE* p_file = data->handle;
    FS_ERR fs_err;

    FSFile_Close(p_file, &fs_err);
    
    APP_DEBUG("close p_file 0x%x", p_file);
    
    if (fs_err != FS_ERR_NONE)
    {
        status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
    }

    APP_DEBUG("\n");

    /* All application resources, used in the session, must be released in this callback */

    return status;
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
static connector_callback_status_t app_process_file_remove(connector_file_system_remove_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_ENTRY_INFO  info;
    FS_ERR fs_err;
    char *full_path=NULL;

    if ((status = arrange_path(&full_path, data->path)) != connector_callback_continue)
        return status;
    
    APP_DEBUG("remove file %s", data->path);
    
    FSEntry_Query(full_path, &info, &fs_err);
    if (fs_err == FS_ERR_NONE)
    {
        if (info.Attrib & FS_ENTRY_ATTRIB_DIR)
        {
            APP_DEBUG(",err: FS_ENTRY_ATTRIB_DIR");
            status = app_process_file_error(&data->errnum, FS_ERR_ENTRY_NOT_FILE);
        }
        else if (!(info.Attrib & FS_ENTRY_ATTRIB_WR))
        {
            APP_DEBUG("err: !FS_ENTRY_ATTRIB_WR");
            status = app_process_file_error(&data->errnum, FS_ERR_ENTRY_RD_ONLY);
        }
        else if (info.Attrib & FS_ENTRY_ATTRIB_HIDDEN)
        {
            APP_DEBUG("err: FS_ENTRY_ATTRIB_HIDDEN");
            status = app_process_file_error(&data->errnum, FS_ERR_ENTRY_RD_ONLY);
        }
        else
        {
            FSEntry_Del(full_path, FS_ENTRY_TYPE_FILE, &fs_err);
        }
    }
    
    if (fs_err != FS_ERR_NONE)
    {
        status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
    }
    
    APP_DEBUG("\n");
    
    if (full_path != NULL)
        free(full_path);
    
    return status;
}

/*
 * This function fills all "statbuf" members, path must be Nul-terminated
 */
static connector_callback_status_t get_statbuf(char const * const path, connector_file_system_statbuf_t * statbuf, void * errnum)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_FILE *p_file;
    FS_ENTRY_INFO info;
    FS_ERR fs_err;
    char *full_path=NULL;

    if ((status = arrange_path(&full_path, path)) != connector_callback_continue)
        return status;
    
    APP_DEBUG("get_statbuf: path %s", path);
    
    p_file = FSFile_Open(full_path, FS_FILE_ACCESS_MODE_RD, &fs_err);
    if (fs_err == FS_ERR_ENTRY_NOT_FILE)
    {
        APP_DEBUG(", Directory");
        statbuf->flags = connector_file_system_file_type_is_dir;
    	statbuf->file_size = 0;
    	statbuf->last_modified = 0; /* Directories don't have this field */
        goto done;
    }
    else if (fs_err != FS_ERR_NONE)
    {
        status = app_process_file_error(errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
        goto done;
    }
            
    FSFile_Query( p_file, &info, &fs_err); 
    if (fs_err != FS_ERR_NONE)
    {
        status = app_process_file_error(errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
        goto done;
    }

    APP_DEBUG(", File");
    
    statbuf->file_size = info.Size;
    statbuf->flags = connector_file_system_file_type_is_reg;
    /*  DateAccess is not keeping time 
        So we can choose DateTimeWr or DateTimeCreate */
    statbuf->last_modified = info.DateTimeWr;   // Date/time of last write

    FSFile_Close(p_file, &fs_err);
done:
  
    APP_DEBUG("\n");

    if (full_path != NULL)
        free(full_path);
        
	return status;
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
static connector_callback_status_t app_process_file_stat(connector_file_system_stat_t * const data)
{
    connector_callback_status_t status;

    status = get_statbuf(data->path, &data->statbuf, &data->errnum);
    if (status != connector_callback_continue)
    {
    	APP_DEBUG("app_process_file_stat: get_statbuf returned %d\n", status);
    	goto done;
    }
	data->hash_algorithm.actual = connector_file_system_hash_none;
done:
    return status;
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
static connector_callback_status_t app_process_file_stat_dir_entry(connector_file_system_stat_dir_entry_t * const data)
{
    connector_callback_status_t status;

    status = get_statbuf(data->path, &data->statbuf, &data->errnum);
    if (status != connector_callback_continue)
    {
    	APP_DEBUG("app_process_file_stat: get_statbuf returned %d\n", status);
    	goto done;
    }
done:
    return status;
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
static connector_callback_status_t app_process_file_opendir(connector_file_system_opendir_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_ERR fs_err;
    FS_DIR * p_dir;
    char *full_path=NULL;

    if ((status = arrange_path(&full_path, data->path)) != connector_callback_continue)
        return status;
           
    p_dir = FSDir_Open (full_path, &fs_err);
    
    APP_DEBUG("Open dir %s, returned p_dir 0x%x", data->path, p_dir);
    
    if (fs_err == FS_ERR_NONE)
    {
        app_dir_data_t * dir_data = malloc (sizeof *dir_data);

        if (dir_data != NULL)
        {
            data->handle = dir_data;
            dir_data->p_dir = p_dir;
        }
        else
        {
            APP_DEBUG("malloc failed");
            status = app_process_file_error(&data->errnum, FS_ERR_MEM_ALLOC);
            FSDir_Close(p_dir, &fs_err);
        }
    }
    else
    {
        status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
    }
    
    APP_DEBUG("\n");

    if (full_path != NULL)
        free(full_path);
    
    return status;
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
static connector_callback_status_t app_process_file_readdir(connector_file_system_readdir_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_ERR fs_err;
    app_dir_data_t * dir_data = data->handle;
    
    APP_DEBUG("readdir p_dir 0x%x", dir_data->p_dir);
    
    /* Read next directory entry */
    FSDir_Rd(dir_data->p_dir, &dir_data->dir_entry, &fs_err);

    /* finished with the directory */
    if (fs_err == FS_ERR_EOF)
    {
        APP_DEBUG(", No more entries");
        goto done;
    }
    
    /* error */
    if (fs_err != FS_ERR_NONE)
    {
        status = app_process_file_error(&data->errnum, fs_err);
        APP_DEBUG(", fs_err %d", fs_err);
        *data->entry_name = '\0';
        goto done;
    }

    /* read valid entry */
    {
        size_t const name_len = strlen(dir_data->dir_entry.Name);

        APP_DEBUG(", Name %s", dir_data->dir_entry.Name);

        if(name_len < data->bytes_available)
        {
            memcpy(data->entry_name, dir_data->dir_entry.Name, name_len + 1);
        }
        else
        {
            APP_DEBUG(", entry name too long");
            status = app_process_file_error(&data->errnum, FS_ERR_NAME_TOO_LONG);
        }
    }

done:
    APP_DEBUG("\n");
    
    return status;
}

/**
 * @brief   Close a Directory
 *
 * This routine opens a directory for the specified path
 *
 * @param data  Pointer to a connector_file_system_closedir_t 
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
static connector_callback_status_t app_process_file_closedir(connector_file_system_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    FS_ERR fs_err;
    app_dir_data_t * dir_data = data->handle;

    ASSERT(dir_data != NULL);
    APP_DEBUG("closedir p_dir 0x%x\n", dir_data->p_dir);

    FSDir_Close(dir_data->p_dir, &fs_err);
    if (fs_err != FS_ERR_NONE)
    {
        APP_DEBUG(", fs_err %d", fs_err);
        status = app_process_file_error(&data->errnum, fs_err);
    }
      
    APP_DEBUG("\n");
    
    free(dir_data);

    /* All application resources, used in the session, must be released in this callback */

    return status;
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
static connector_callback_status_t app_process_file_hash(connector_file_system_hash_t * const data)
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
static connector_callback_status_t app_process_file_get_error(connector_file_system_get_error_t * const data)
{
    long int errnum = (long int)data->errnum;
    size_t  max_bytes_required;

    data->bytes_used = 0;

    /* Send original uCOS fs_err error code */
    #define UCOS_ERR_STR "uCOS fs_err:"
    max_bytes_required = strlen(UCOS_ERR_STR) + 6;
    if ((errnum != 0) && (data->bytes_available > max_bytes_required))
    {
        sprintf((char*)data->buffer, "%s %d", UCOS_ERR_STR, errnum);     
        data->bytes_used = strlen((char*)data->buffer);
    }

    switch(errnum)
    {
        case FS_ERR_DEV_WR_PROT:
        case FS_ERR_ENTRY_RD_ONLY:
            data->error_status = connector_file_system_permission_denied;
            break;

        case FS_ERR_MEM_ALLOC:
        case FS_ERR_NAME_PATH_TOO_LONG:
        case FS_ERR_NAME_TOO_LONG:
            data->error_status = connector_file_system_out_of_memory;
            break;

        case FS_ERR_ENTRY_NOT_FOUND:
            data->error_status = connector_file_system_path_not_found;
            break;

        case FS_ERR_ENTRY_EXISTS:
        case FS_ERR_ENTRY_INVALID:
        case FS_ERR_ENTRY_NOT_DIR:
        case FS_ERR_ENTRY_NOT_EMPTY:
        case FS_ERR_ENTRY_NOT_FILE:
        case FS_ERR_ENTRY_OPEN:
        case FS_ERR_ENTRY_TYPE_INVALID:
        case FS_ERR_NAME_EMPTY:
        case FS_ERR_NAME_EXT_TOO_LONG:
        case FS_ERR_NAME_INVALID:
        case FS_ERR_NAME_NULL:
            data->error_status = connector_file_system_invalid_parameter;
            break;

        case FS_ERR_DEV_FULL:
        case FS_ERR_DIR_FULL:
            data->error_status = connector_file_system_insufficient_storage_space;
            break;

        default:
            data->error_status = connector_file_system_unspec_error;
            break;
    }

    return connector_callback_continue;
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
static connector_callback_status_t app_process_file_session_error(connector_file_system_session_error_t * const data)
{
     APP_DEBUG("Session Error %d\n", data->session_error);

    /* All application resources, used in the session, must be released in this callback */
    data->user_context = NULL;
    return connector_callback_continue;
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

#endif