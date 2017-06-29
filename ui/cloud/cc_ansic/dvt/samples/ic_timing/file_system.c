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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "connector_api.h"
#include "platform.h"
#include "connector_config.h"
#include "connector_debug.h"
#include "application.h"

#if !defined CONNECTOR_FILE_SYSTEM 
#error "Please define CONNECTOR_FILE_SYSTEM in connector_config.h to run this sample" 
#endif

#if defined APP_ENABLE_MD5
#include <openssl/md5.h>

#define APP_MD5_BUFFER_SIZE 1024

typedef struct
{
    MD5_CTX md5;
    char buf[APP_MD5_BUFFER_SIZE];
    unsigned int flags;
    int fd;

} app_md5_ctx;
#endif

#ifndef APP_MIN_VALUE
#define APP_MIN_VALUE(a,b) (((a)<(b))?(a):(b))
#endif

typedef struct
{
    DIR * dirp;
    struct dirent dir_entry;

} app_dir_data_t;


static connector_callback_status_t app_process_file_error(connector_filesystem_errnum_t * const error_token, long int const errnum)
{
    connector_callback_status_t status;

    switch(errnum)
    {
    #if EAGAIN != EWOULDBLOCK
        case EWOULDBLOCK:
    #endif
        case EAGAIN:
            status = connector_callback_busy;
            break;

        default:
            status = connector_callback_error;
            *error_token = errnum;
            break;
    }
    return status;
}

static int app_convert_file_open_mode(int const oflag)
{
#if (CONNECTOR_FILE_O_RDONLY == O_RDONLY) && (CONNECTOR_FILE_O_WRONLY == O_WRONLY) && (CONNECTOR_FILE_O_RDWR == O_RDWR) && \
    (CONNECTOR_FILE_O_CREAT == O_CREAT)   && (CONNECTOR_FILE_O_APPEND == O_APPEND) && (CONNECTOR_FILE_O_TRUNC == O_TRUNC)

    return oflag;
#else
    int result = 0;

    if (oflag & CONNECTOR_FILE_O_WRONLY) result |= O_WRONLY;
    if (oflag & CONNECTOR_FILE_O_RDWR)   result |= O_RDWR;
    if (oflag & CONNECTOR_FILE_O_APPEND) result |= O_APPEND;
    if (oflag & CONNECTOR_FILE_O_CREAT)  result |= O_CREAT;
    if (oflag & CONNECTOR_FILE_O_TRUNC)  result |= O_TRUNC;

    if ((oflag & (CONNECTOR_FILE_O_WRONLY | CONNECTOR_FILE_O_RDWR)) == 0)
        result |= O_RDONLY;

    return result;
#endif
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
        case EACCES:
        case EPERM:
        case EROFS:
            data->error_status = connector_file_system_permission_denied;
            break;

        case ENOMEM:
        case ENAMETOOLONG:
            data->error_status = connector_file_system_out_of_memory;
            break;

        case ENOENT:
        case ENODEV:
        case EBADF:
            data->error_status = connector_file_system_path_not_found;
            break;

        case EINVAL:
        case ENOSYS:
        case ENOTDIR:
        case EISDIR:
            data->error_status = connector_file_system_invalid_parameter;
            break;

        case ENOSPC:
            data->error_status = connector_file_system_insufficient_storage_space;
            break;

        default:
            data->error_status = connector_file_system_unspec_error;
            break;
    }

    return connector_callback_continue;
}


#if defined APP_ENABLE_MD5
static app_md5_ctx * app_allocate_md5_ctx(unsigned int const flags)
{
    app_md5_ctx * ctx = malloc(sizeof *ctx);

    if (ctx != NULL)
    {
        ctx->flags = flags;
        ctx->fd    = -1;
    }
    else
    {
        APP_DEBUG("app_allocate_md5_ctx: malloc fails\n");
    }
    return ctx;
}

static connector_callback_status_t app_process_file_session_error(connector_file_system_session_error_t * const data)
{
     APP_DEBUG("Session Error %d\n", data->session_error);

    /* All application resources, used in the session, must be released in this callback */
    if (data->user_context != NULL)
    {
        app_md5_ctx * ctx = data->user_context;

        if (ctx->fd >= 0)
            close(ctx->fd);

        free(data->user_context);
        data->user_context = NULL;
    }
    return connector_callback_continue;
}

static connector_callback_status_t app_process_file_hash(connector_file_system_hash_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    app_md5_ctx * ctx = data->user_context;
    int ret;

    if (ctx == NULL)
    {   
        ASSERT(0);
        goto error;
    }

    if (ctx->fd < 0)
    {
        ctx->fd = open(data->path, O_RDONLY);
        APP_DEBUG("Open %s, returned %d\n", data->path, ctx->fd);

        if (ctx->fd < 0)
        {
            ASSERT(0);
            goto error;
        }

        MD5_Init(&ctx->md5);
    }

    while ((ret = read (ctx->fd, ctx->buf, sizeof ctx->buf)) > 0)
    {
        MD5_Update(&ctx->md5, ctx->buf, ret);
    }
    if (ret == -1 && errno == EAGAIN)
    {
        status = connector_callback_busy;
        goto done;
    }

    APP_DEBUG("Close %d\n", ctx->fd);
    close (ctx->fd);
    ctx->fd = -1;

    if (ret == 0)
    {
        MD5_Final (data->hash_value, &ctx->md5);
        goto done;
    }

error:
    memset(data->hash_value, 0, data->bytes_requested);

done:
    if (ctx != NULL && status == connector_callback_continue)
    {
        /* free md5 context here,  if ls was issued a single file */
        if ((ctx->flags == connector_file_system_file_type_is_dir) == 0)
        {
            free(data->user_context);
            data->user_context = NULL;
        }
    }
    return status;
}
#else

static connector_callback_status_t app_process_file_session_error(connector_file_system_session_error_t * const data)
{
    UNUSED_ARGUMENT(data);
    APP_DEBUG("Session Error %d\n", data->session_error);

    /* All application resources, used in the session, must be released in this callback */
    return connector_callback_continue;
}

static connector_callback_status_t app_process_file_hash(connector_file_system_hash_t * const data)
{

    /* app_process_file_hash() should not be called if APP_ENABLE_MD5 is not defined */
    ASSERT(0);

    memset(data->hash_value, 0, data->bytes_requested);
    return connector_callback_continue;
}
#endif

static connector_callback_status_t app_process_file_stat(connector_file_system_stat_t * const data)
{
    struct stat statbuf;
    connector_file_system_statbuf_t * pstat = &data->statbuf;
    connector_callback_status_t status = connector_callback_continue;

    int const result = stat(data->path, &statbuf);

    if (result < 0)
    {
        status = app_process_file_error(&data->errnum, errno);
        APP_DEBUG("stat for %s returned %d, errno %d\n", data->path, result, errno);
        goto done;
    }

    APP_DEBUG("stat for %s returned %d, filesize %ld\n", data->path, result, statbuf.st_size);

    pstat->file_size = statbuf.st_size;
    pstat->last_modified = statbuf.st_mtime;

    pstat->flags = connector_file_system_file_type_none;

    if (S_ISDIR(statbuf.st_mode))
       pstat->flags = connector_file_system_file_type_is_dir;
    else
    if (S_ISREG(statbuf.st_mode))
       pstat->flags = connector_file_system_file_type_is_reg;


    data->hash_algorithm.actual = connector_file_system_hash_none;

#if defined APP_ENABLE_MD5
    switch (data->hash_algorithm.requested)
    {
        case connector_file_system_hash_best:
        case connector_file_system_hash_md5:
            if (pstat->flags != connector_file_system_file_type_none)
            {
                data->hash_algorithm.actual = connector_file_system_hash_md5;
                if (data->user_context == NULL)
                {
                    data->user_context = app_allocate_md5_ctx(pstat->flags);
                    if (data->user_context == NULL) 
                    {
                        status = app_process_file_error(&data->errnum, ENOMEM);
                    }
                }
            }
            break;


        default:
            break;
    }
#endif
done:
    return status;
}


static connector_callback_status_t app_process_file_stat_dir_entry(connector_file_system_stat_dir_entry_t * const data)
{
    struct stat statbuf;
    connector_file_system_statbuf_t * pstat = &data->statbuf;
    connector_callback_status_t status = connector_callback_continue;

    int const result = stat(data->path, &statbuf);

    if (result < 0)
    {
        status = app_process_file_error(&data->errnum, errno);
        APP_DEBUG("stat for %s returned %d, errno %d\n", data->path, result, errno);
        goto done;
    }

    APP_DEBUG("stat for %s returned %d, filesize %ld\n", data->path, result, statbuf.st_size);

    pstat->file_size = statbuf.st_size;
    pstat->last_modified = statbuf.st_mtime;

    pstat->flags = connector_file_system_file_type_none;

    if (S_ISDIR(statbuf.st_mode))
       pstat->flags = connector_file_system_file_type_is_dir;
    else
    if (S_ISREG(statbuf.st_mode))
       pstat->flags = connector_file_system_file_type_is_reg;

done:
    return status;
}

static connector_callback_status_t app_process_file_opendir(connector_file_system_opendir_t * const data)
{

    connector_callback_status_t status = connector_callback_continue;
    DIR * dirp;

    open_timing_table("Start file opendir", NULL);
    writing_timing_description("Start file opendir: ", data->path);

    errno = 0;
    dirp = opendir(data->path);

    if (dirp != NULL)
    {
        app_dir_data_t * dir_data = malloc (sizeof *dir_data);

        if (dir_data != NULL)
        {
            data->handle = dir_data;

            dir_data->dirp = dirp;
            APP_DEBUG("opendir for %s returned %p\n", data->path, (void *) dirp);
        }
        else
        {
            APP_DEBUG("app_process_file_opendir: malloc fails %s\n", data->path);
            status = app_process_file_error(&data->errnum, ENOMEM);
            closedir(dirp);
        }
    }
    else
        status = app_process_file_error(&data->errnum, errno);

    return status;
}

static connector_callback_status_t app_process_file_closedir(connector_file_system_closedir_t * const data)
{
    app_dir_data_t * dir_data = (app_dir_data_t *)data->handle;

    ASSERT(dir_data != NULL);
    APP_DEBUG("closedir %p\n", (void *) dir_data->dirp);

    close_timing_table("Close file closedir", NULL);

    closedir(dir_data->dirp);
    free(dir_data);

    /* All application resources, used in the session, must be released in this callback */

#if defined APP_ENABLE_MD5
    if (data->user_context != NULL)
    {
        /* free md5 context here, if ls was issued a directory */
        free(data->user_context);
        data->user_context = NULL;
    }
#endif
    return connector_callback_continue;
}

static connector_callback_status_t app_process_file_readdir(connector_file_system_readdir_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    app_dir_data_t * dir_data = (app_dir_data_t *)data->handle;
    struct dirent  * result = NULL;

    /* Read next directory entry */
    int rc = readdir_r(dir_data->dirp, &dir_data->dir_entry, &result);

    /* error */
    if (rc != 0)
    {
        status = app_process_file_error(&data->errnum, rc);
        APP_DEBUG("readdir_r returned %d\n", rc);
        *data->entry_name = '\0';
        goto done;
    }

    /* finished with the directory */
    if (result == NULL)
    {
        APP_DEBUG("No more directory entries %d\n", rc);
        goto done;
    }

    /* read valid entry */
    {
        size_t const name_len = strlen(result->d_name);

        APP_DEBUG("readdir_r returned directory entry name %s\n", result->d_name);

        if(name_len < data->bytes_available)
        {
            memcpy(data->entry_name, result->d_name, name_len + 1);
        }
        else
        {
            APP_DEBUG("directory entry name too long\n");
            status = app_process_file_error(&data->errnum, ENAMETOOLONG);
        }
    }

done:
    return status;
}


static connector_callback_status_t app_process_file_open(connector_file_system_open_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    open_timing_table("Start file open", NULL);
    writing_timing_description("Start file open: ", data->path);

    {
        int const oflag = app_convert_file_open_mode(data->oflag);
    
        /* 0664 = read,write owner + read,write group + read others */
        long int const fd = open(data->path, oflag, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
    
        if (fd < 0)
        {
            status = app_process_file_error(&data->errnum, errno);
        }
    
        APP_DEBUG("Open %s, %d, returned %ld\n", data->path, oflag, fd);
    
        data->handle = fd;
    }

    return status;
}


static connector_callback_status_t app_process_file_lseek(connector_file_system_lseek_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    long int const fd = (long int) data->handle;
    int  origin; 

    switch(data->origin)
    {
        case connector_file_system_seek_set:
            origin = SEEK_SET;
            break;

        case connector_file_system_seek_end:
            origin = SEEK_END;
            break;

        case connector_file_system_seek_cur:
        default:
            origin = SEEK_CUR;
            break;
    }

    data->resulting_offset = lseek(fd, data->requested_offset, origin);

    if (data->resulting_offset < 0)
    {
        status = app_process_file_error(&data->errnum, errno);
    }

    APP_DEBUG("lseek fd %ld, offset %d, origin %d returned %d\n", 
                fd, data->requested_offset, data->origin, data->resulting_offset);

    return status;
}

static connector_callback_status_t app_process_file_ftruncate(connector_file_system_truncate_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    long int const fd = (long int) data->handle;

    int const result = ftruncate(fd, data->length_in_bytes);

    if (result < 0)
    {
        status = app_process_file_error(&data->errnum, errno);
    }

    APP_DEBUG("ftruncate %ld, %d returned %d\n", fd, data->length_in_bytes, result);

    return status;
}

static connector_callback_status_t app_process_file_remove(connector_file_system_remove_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    int const result = unlink(data->path);

    if (result < 0)
    {
        status = app_process_file_error(&data->errnum, errno);
    }

    APP_DEBUG("unlink %s returned %d\n", data->path, result);

    return status;
}

static connector_callback_status_t app_process_file_read(connector_file_system_read_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    long int const fd = (long int) data->handle;

    int const result = read(fd, data->buffer, data->bytes_available);

    if (result < 0)
    {
        status = app_process_file_error(&data->errnum, errno);
        APP_DEBUG("read %ld, %" PRIsize ", returned %d, errno %d\n", fd, data->bytes_available, result, errno);
        goto done;
    }

    APP_DEBUG("read %ld, %" PRIsize ", returned %d\n", fd, data->bytes_available, result);
    data->bytes_used = result;

done:
    return status;
}

static connector_callback_status_t app_process_file_write(connector_file_system_write_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    long int const fd = (long int) data->handle;

    int const result = write(fd, data->buffer, data->bytes_available);

    if (result < 0)
    {
        status = app_process_file_error(&data->errnum, errno);
        APP_DEBUG("write %ld, %" PRIsize ", returned %d, errno %d\n", fd, data->bytes_available, result, errno);
        goto done;
    }

    APP_DEBUG("write %ld, %" PRIsize ", returned %d\n", fd, data->bytes_available, result);

    data->bytes_used = result;

done:
    return status;
}

static connector_callback_status_t app_process_file_close(connector_file_system_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    long int const fd = (long int) data->handle;
    int const result = close(fd);

    APP_DEBUG("close %ld returned %d\n", fd, result);
    close_timing_table("Close file close", NULL);

    if (result < 0 && errno == EIO)
    {
        status = app_process_file_error(&data->errnum, EIO);
    }


    /* All application resources, used in the session, must be released in this callback */

    return status;
}

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

