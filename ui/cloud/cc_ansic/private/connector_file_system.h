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

#define FS_TRUNC_FLAG            0x01
#define FS_IS_DIR_FLAG           0x01
#define FS_IS_REG_FLAG           0x02
#define FS_IS_LARGE_FLAG         0x02
#define FS_LS_SINGLE_FILE        0x10
#define FS_ERROR_INTERNAL_FLAG   0x40
#define FS_SESSION_ERROR_CALLED  0x80

#define FS_OPCODE_BYTES           1

typedef enum
{
    fs_get_request_opcode = 1,
    fs_get_response_opcode,
    fs_put_request_opcode,
    fs_put_response_opcode,
    fs_ls_request_opcode,
    fs_ls_response_opcode,
    fs_rm_request_opcode,
    fs_rm_response_opcode,
    fs_error_opcode = 200
} fs_opcode_t;

typedef enum
{
    fs_state_none,
    fs_state_stat,
    fs_state_open,
    fs_state_lseek1,
    fs_state_lseek,
    fs_state_readdir,
    fs_state_stat_dir_entry,
    fs_state_closing,
    fs_state_closed
} fs_state_t;

typedef enum
{
    fs_error_path_too_long,
    fs_error_format,
    fs_error_invalid_offset,
    fs_error_invalid_hash,
    fs_error_generic,
    fs_error_large_file,
    fs_error_session_canceled
} fs_error_internal_t;

typedef struct
{
    void * user_context;
    connector_filesystem_errnum_t errnum;
} fs_user_data;

typedef struct
{
    void * user_context;
    union {
        connector_filesystem_errnum_t user;
        fs_error_internal_t internal;
    } errnum;

    struct {
        connector_filesystem_file_handle_t file;
        connector_filesystem_dir_handle_t dir;
    } handle;

    union
    {
        struct
        {
            uint32_t bytes_done;
            uint32_t data_length;
            connector_file_offset_t offset;
        } f;
        struct
        {
            char path[CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH];
            size_t path_len;
            uint32_t last_modified;
            connector_file_offset_t file_size;
            connector_file_system_hash_algorithm_t hash_alg;
        } d;
    }data;

    fs_opcode_t opcode;
    connector_status_t status;
    fs_state_t state;
    uint8_t flags;

} fs_context_t;

#define FsIsOpen(context) (((context)!=NULL) && ((context)->state >= fs_state_open) && ((context)->state < fs_state_closed))

#define FsIsBitSet(flag, bit)  (((flag) & (bit)) == (bit))
#define FsBitSet(flag, bit)    ((flag) |= (bit))
#define FsBitClear(flag, bit)  ((flag) &= ~(bit))

#define FsIsDir(context)    FsIsBitSet(context->flags, FS_IS_DIR_FLAG)
#define FsSetDir(context)   FsBitSet(context->flags, FS_IS_DIR_FLAG)
#define FsClearDir(context) FsBitClear(context->flags, FS_IS_DIR_FLAG)

#define FsIsReg(context)    FsIsBitSet(context->flags, FS_IS_REG_FLAG)
#define FsSetReg(context)   FsBitSet(context->flags, FS_IS_REG_FLAG)
#define FsClearReg(context) FsBitClear(context->flags, FS_IS_REG_FLAG)

#define FsNeedTrunc(context)  FsIsBitSet(context->flags, FS_TRUNC_FLAG)
#define FsSetTrunc(context)   FsBitSet(context->flags, FS_TRUNC_FLAG)
#define FsClearTrunc(context) FsBitClear(context->flags, FS_TRUNC_FLAG)

#define FsIsLsSingleFile(context)   FsIsBitSet(context->flags, FS_LS_SINGLE_FILE)
#define FsSetLsSingleFile(context)  FsBitSet(context->flags, FS_LS_SINGLE_FILE)

#define FsHasInternalError(context) FsIsBitSet(context->flags, FS_ERROR_INTERNAL_FLAG)
#define FsSetInternalError(context, error) {FsBitSet(context->flags,FS_ERROR_INTERNAL_FLAG); context->errnum.internal=error;}

#define FsSessionErrorCalled(context) FsIsBitSet(context->flags, FS_SESSION_ERROR_CALLED)
#define FsSetSessionErrorCalled(context) FsBitSet(context->flags, FS_SESSION_ERROR_CALLED)

#define FsSetState(context, s) (context->state = s)
#define FsGetState(context)    (context->state)


#define FsOperationSuccess(status, context) (status==connector_working && context->errnum.user==CONNECTOR_FILESYSTEM_ERRNUM_NONE)

STATIC void fs_set_service_error(msg_service_request_t * const service_request, connector_session_error_t const session_error)
{
    service_request->error_value = session_error;
    service_request->service_type = msg_service_type_error;
}

STATIC connector_status_t fs_set_abort(connector_data_t * const connector_ptr,
                                   fs_context_t * const context,
                                   connector_request_id_file_system_t const file_request,
                                   connector_status_t const error_status)
{

    connector_request_id_t request_id;
    request_id.file_system_request = file_request;

    notify_error_status(connector_ptr->callback, connector_class_id_file_system, request_id, error_status, connector_ptr->context);
    context->status = connector_abort;
    return context->status;
}

STATIC void fs_get_internal_error_data(connector_file_system_get_error_t * const data, fs_error_internal_t const error_internal)
{
    fs_error_internal_t error_code = error_internal;
    static struct
    {
        char const * hint;
        connector_file_system_error_t status;

    } error_data[] =
    {
        {"Path too long",                   connector_file_system_out_of_memory},
        {"Request format error",            connector_file_system_request_format_error},
        {"Invalid offset",                  connector_file_system_invalid_parameter},
        {"Invalid hash algorithm",          connector_file_system_invalid_parameter},
        {"Unspecified error",               connector_file_system_unspec_error},
        {"Offset is too large or negative", connector_file_system_request_format_error}
    };

    switch (error_code)
    {
        case fs_error_path_too_long:
        case fs_error_format:
        case fs_error_invalid_offset:
        case fs_error_invalid_hash:
        case fs_error_generic:
        case fs_error_large_file:
            break;

    default:
        ASSERT(connector_false);
        error_code = fs_error_generic;
        break;
    }
    ASSERT(error_code < ARRAY_SIZE(error_data));

    data->error_status = error_data[error_code].status;
    data->bytes_used = MIN_VALUE(data->bytes_available, strlen(error_data[error_code].hint));
    memcpy(data->buffer, error_data[error_code].hint, data->bytes_used);
}

STATIC connector_status_t format_file_error_msg(connector_data_t * const connector_ptr,
                                                msg_service_request_t * const service_request,
                                                fs_context_t * const context)
{
    connector_status_t status = connector_working;

    if (service_request->service_type != msg_service_type_error)
    {
         /* 1st message so let's parse message-start packet:
          *
          * File System Get request format:
          *  ------------------------------------------------------
          * |   0    |     +1     |  +1               |    +N      |
          *  ------------------------------------------------------
          * | Opcode | Error code | Error Hint Length | Error hint |
          *  ------------------------------------------------------
          *
          */
         enum  {
             field_define(fs_error_response, opcode,         uint8_t),
             field_define(fs_error_response, error_code,     uint8_t),
             field_define(fs_error_response, error_hint_len, uint8_t),
             record_end(fs_error_response_header)
         };

         msg_service_data_t * const service_data = service_request->need_data;

         size_t const header_bytes = record_bytes(fs_error_response_header);
         size_t const buffer_size  = MIN_VALUE(service_data->length_in_bytes - header_bytes, UCHAR_MAX);

         uint8_t * fs_error_response = service_data->data_ptr;
         connector_file_system_get_error_t data;

         data.buffer = fs_error_response + header_bytes;
         data.errnum = context->errnum.user;
         data.bytes_available = buffer_size;
         data.bytes_used = 0;

         if (FsHasInternalError(context))
         {
             fs_get_internal_error_data(&data, context->errnum.internal);
         }
         else
         {
             connector_request_id_t request_id;
             data.user_context = context->user_context;
             request_id.file_system_request = connector_request_id_file_system_get_error;

             status = connector_callback(connector_ptr->callback,
                                         connector_class_id_file_system,
                                         request_id,
                                         &data,
                                         connector_ptr->context
                                        ) == connector_callback_continue ?
                            connector_working : connector_abort;

             context->user_context = data.user_context;

             if (status == connector_abort)
                 goto done;

             if (data.bytes_used > buffer_size)
             {
                 fs_set_abort(connector_ptr,
                              context,
                              connector_request_id_file_system_get_error,
                              connector_invalid_data_size);
                 status = connector_abort;
                 goto done;
             }
         }
         message_store_u8(fs_error_response, opcode, fs_error_opcode);
         /* coverity[uninit_use] */
         message_store_u8(fs_error_response, error_code, (uint8_t)data.error_status);
         message_store_u8(fs_error_response, error_hint_len, (uint8_t) data.bytes_used);

         service_data->length_in_bytes = header_bytes + data.bytes_used;

         MsgSetLastData(service_data->flags);
    }

done:
    return status;
}

STATIC connector_status_t fs_call_user(connector_data_t * const connector_ptr,
                                       msg_service_request_t * const service_request,
                                       fs_context_t * const context,
                                       connector_request_id_file_system_t const fs_request_id,
                                       void * const pdata)
{
    connector_status_t status = connector_working;
    connector_callback_status_t callback_status;
    connector_request_id_t request_id;
    fs_user_data * const data = pdata;

    request_id.file_system_request = fs_request_id;

    data->user_context = context->user_context;
    data->errnum = CONNECTOR_FILESYSTEM_ERRNUM_NONE;

    callback_status = connector_callback(connector_ptr->callback,
                                         connector_class_id_file_system,
                                         request_id,
                                         data,
                                         connector_ptr->context
                                        );

    context->user_context = data->user_context;

    switch (callback_status)
    {
        case connector_callback_continue:
            status = connector_working;
            break;

        case connector_callback_busy:
            status = connector_pending;
            break;

        case connector_callback_unrecognized:
            status = connector_working;
            FsSetInternalError(context, fs_error_session_canceled);
            fs_set_service_error(service_request, connector_session_error_cancel);
            break;

        case connector_callback_abort:
            status = context->status = connector_abort;
            break;

        case connector_callback_error:
            status = connector_working;
            /* don't overwrite previous errno */
            if (context->errnum.user == CONNECTOR_FILESYSTEM_ERRNUM_NONE)
            {
                if (data->errnum != CONNECTOR_FILESYSTEM_ERRNUM_NONE)
                {
                    /* user returned errno */
                    context->errnum.user = data->errnum;
                }
                else
                {
                    /* user returned connector_callback_error without setting errno */
                    FsSetInternalError(context, fs_error_generic);
                }
            }
            break;
    }

    return status;
}

STATIC size_t file_hash_size(connector_file_system_hash_algorithm_t const hash_alg)
{
    size_t result;

    switch(hash_alg)
    {
        case connector_file_system_hash_md5:
            result = 16;
            break;

        case connector_file_system_hash_crc32:
            result = 4;
            break;

        default:
            result = 0;
    }
    return result;
}

STATIC connector_status_t call_file_stat_user(connector_data_t * const connector_ptr,
                                              msg_service_request_t * const service_request,
                                              fs_context_t * const context,
                                              char const * const path,
                                              connector_file_system_hash_algorithm_t const hash_alg)
{
    connector_status_t status;

    connector_file_system_stat_t data;
    data.path = path;
    data.hash_algorithm.requested = hash_alg;

    data.statbuf.file_size = 0;
    data.statbuf.last_modified = 0;
    data.statbuf.flags = connector_file_system_file_type_none;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_stat,
                          &data);

    if (status == connector_pending)
        goto done;

    if (!FsOperationSuccess(status, context))
        goto done;

   context->data.d.file_size = data.statbuf.file_size;
   context->data.d.last_modified = data.statbuf.last_modified;

   switch(data.statbuf.flags)
   {
       case connector_file_system_file_type_is_dir:
           FsSetDir(context);
           break;

       case connector_file_system_file_type_is_reg:
           FsSetReg(context);
           break;

       default:
           context->flags = 0;
           break;
   }

   switch(hash_alg)
   {
       case connector_file_system_hash_none:
           context->data.d.hash_alg = hash_alg;
           break;

       case connector_file_system_hash_best:
           switch(data.hash_algorithm.actual)
           {
               case connector_file_system_hash_md5:
               case connector_file_system_hash_crc32:
                   context->data.d.hash_alg = data.hash_algorithm.actual;
                   break;

               default:
                   context->data.d.hash_alg = connector_file_system_hash_none;
                   break;
           }
           break;

       default:
           if (hash_alg != data.hash_algorithm.actual)
               context->data.d.hash_alg = connector_file_system_hash_none;
           break;
   }

   if (context->data.d.hash_alg != connector_file_system_hash_none)
   {
       if (!FsIsDir(context) && !FsIsReg(context))
           context->data.d.hash_alg = connector_file_system_hash_none;
   }
done:
    return status;
}

STATIC connector_status_t call_file_stat_dir_entry_user(connector_data_t * const connector_ptr,
                                                        msg_service_request_t * const service_request,
                                                        fs_context_t * const context,
                                                        char const * const path)
{
    connector_status_t status;

    connector_file_system_stat_dir_entry_t data;
    data.path = path;
    data.statbuf.file_size = 0;
    data.statbuf.last_modified = 0;
    data.statbuf.flags = connector_file_system_file_type_none;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_stat_dir_entry,
                          &data);

    if (!FsOperationSuccess(status, context))
        goto done;

   context->data.d.file_size = data.statbuf.file_size;
   context->data.d.last_modified = data.statbuf.last_modified;

   switch(data.statbuf.flags)
   {
       case connector_file_system_file_type_is_dir:
           FsSetDir(context);
           break;

       case connector_file_system_file_type_is_reg:
           FsSetReg(context);
           break;

       default:
           context->flags = 0;
           break;
   }

done:
    return status;
}


STATIC connector_status_t call_file_opendir_user(connector_data_t * const connector_ptr,
                                                 msg_service_request_t * const service_request,
                                                 fs_context_t * const context,
                                                 char const * const path)
{
    connector_status_t   status;
    connector_file_system_opendir_t data;

    data.path = path;
    data.handle = CONNECTOR_FILESYSTEM_DIR_HANDLE_NOT_INITIALIZED;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_opendir,
                          &data);

    if (status == connector_pending)
        goto done;

    if (FsOperationSuccess(status, context))
    {
        if (data.handle != CONNECTOR_FILESYSTEM_DIR_HANDLE_NOT_INITIALIZED)
        {
            context->handle.dir = data.handle;
            FsSetState(context, fs_state_open);
        }
        else
        {
            status = fs_set_abort(connector_ptr,
                          context,
                          connector_request_id_file_system_opendir,
                          connector_invalid_data);
        }
    }

done:
    return status;
}

/* This function is a custom implementation of GNU/Linux's non-standard strnlen() */
STATIC int strnlen_(char const * const string, int maxlen)
{
    volatile const char *e;
    int n;

    for (e = string, n = 0; *e && n < maxlen; e++, n++)
        ;

    return n;
}

STATIC connector_status_t call_file_readdir_user(connector_data_t * const connector_ptr,
                                                 msg_service_request_t * const service_request,
                                                 fs_context_t * const context,
                                                 char * const path,
                                                 size_t const buffer_size)
{
    connector_status_t  status;

    connector_file_system_readdir_t data;
    data.handle = context->handle.dir;
    data.entry_name = path;
    data.bytes_available = buffer_size;
    *path = '\0';

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_readdir,
                          &data);
    if (!FsOperationSuccess(status, context))
        goto done;

    if (path[strnlen_(path, buffer_size)] != '\0')
    {
        /* no NUL-character within buffer */
        status =  fs_set_abort(connector_ptr,
                               context,
                               connector_request_id_file_system_readdir,
                               connector_invalid_data_size);
     }

done:
    return status;
}

STATIC connector_status_t call_file_close_user(connector_data_t * const connector_ptr,
                                               msg_service_request_t * const service_request,
                                               fs_context_t * const context,
                                               connector_request_id_file_system_t const fs_request_id)
{
    connector_status_t status = context->status;

    if (!FsIsOpen(context))
        goto done;

    FsSetState(context, fs_state_closing);

    switch (fs_request_id)
    {
        case connector_request_id_file_system_close:
        {
            connector_file_system_close_t data;

            data.handle = context->handle.file;

            status = fs_call_user(connector_ptr,
                                  service_request,
                                  context,
                                  fs_request_id,
                                  &data);
            break;
        }
        case connector_request_id_file_system_closedir:
        {
            connector_file_system_closedir_t data;

            data.handle = context->handle.dir;

            status = fs_call_user(connector_ptr,
                                  service_request,
                                  context,
                                  fs_request_id,
                                  &data);
            break;
        }
        case connector_request_id_file_system_open:
        case connector_request_id_file_system_read:
        case connector_request_id_file_system_write:
        case connector_request_id_file_system_lseek:
        case connector_request_id_file_system_ftruncate:
        case connector_request_id_file_system_remove:
        case connector_request_id_file_system_stat:
        case connector_request_id_file_system_stat_dir_entry:
        case connector_request_id_file_system_opendir:
        case connector_request_id_file_system_readdir:
        case connector_request_id_file_system_get_error:
        case connector_request_id_file_system_session_error:
        case connector_request_id_file_system_hash:
            break;
    }


    if (status == connector_pending)
        goto done;

    /* done with close, no matter if success or an error */
    FsSetState(context, fs_state_closed);

done:
    return status;
}

STATIC connector_status_t call_file_hash_user(connector_data_t * const connector_ptr,
                                              msg_service_request_t * const service_request,
                                              fs_context_t * const context,
                                              char const * const path,
                                              uint8_t * const hash_ptr)
{
    connector_status_t status;
    connector_file_system_hash_t data;

    data.bytes_requested = file_hash_size(context->data.d.hash_alg);
    data.path = path;
    data.hash_algorithm = context->data.d.hash_alg;
    data.hash_value = hash_ptr;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_hash,
                          &data);

    return status;
}

STATIC connector_status_t call_file_open_user(connector_data_t * const connector_ptr,
                                              msg_service_request_t * const service_request,
                                              fs_context_t * context,
                                              char const * const path,
                                              int const oflag)
{
    connector_status_t  status;
    connector_file_system_open_t data;

    data.path  = path;
    data.oflag = oflag;
    data.handle = CONNECTOR_FILESYSTEM_FILE_HANDLE_NOT_INITIALIZED;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_open,
                          &data);

    if (status == connector_pending)
        goto done;

    if (FsOperationSuccess(status, context))
    {
        if (data.handle != CONNECTOR_FILESYSTEM_FILE_HANDLE_NOT_INITIALIZED)
        {
            context->handle.file = data.handle;
            FsSetState(context, fs_state_open);
        }
        else
        {
            status = fs_set_abort(connector_ptr,
                          context,
                          connector_request_id_file_system_open,
                          connector_invalid_data);
        }
    }

    if (FsGetState(context) != fs_state_open)
        FsSetState(context, fs_state_closed);   /* never opened a file */

done:
    return status;
}

STATIC connector_status_t call_file_lseek_user(connector_data_t * const connector_ptr,
                                               msg_service_request_t * const service_request,
                                               fs_context_t * const context,
                                               connector_file_offset_t const  offset_in,
                                               connector_file_system_seek_origin_t  const origin,
                                               connector_file_offset_t * const offset_out)
{
    connector_file_system_lseek_t data;
    connector_status_t status;

    data.handle = context->handle.file;
    data.requested_offset = offset_in;
    data.resulting_offset = -1;
    data.origin = origin;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_lseek,
                          &data);

    *offset_out = data.resulting_offset;

    return status;
}


STATIC connector_status_t call_file_ftruncate_user(connector_data_t * const connector_ptr,
                                                   msg_service_request_t * const service_request,
                                                   fs_context_t * const context)
{
    connector_status_t status;
    connector_file_system_truncate_t data;

    data.handle = context->handle.file;
    data.length_in_bytes = context->data.f.offset;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_ftruncate,
                          &data);
    return status;
}

STATIC connector_status_t call_file_rm_user(connector_data_t * const connector_ptr,
                                            msg_service_request_t * const service_request,
                                            fs_context_t * const context,
                                            char const * const path)
{
    connector_status_t status;
    connector_file_system_remove_t data;
    data.path = path;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_remove,
                          &data);
    return status;
}

STATIC connector_status_t call_file_read_user(connector_data_t * const connector_ptr,
                                              msg_service_request_t * const service_request,
                                              fs_context_t * const context,
                                              void   * const buffer,
                                              size_t * const buffer_size)
{
    connector_status_t status;
    connector_file_system_read_t data;

    data.handle = context->handle.file;
    data.buffer = buffer;
    data.bytes_available = *buffer_size;
    data.bytes_used = 0;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_read,
                          &data);

    if (!FsOperationSuccess(status, context))
        goto done;

    if (data.bytes_used > *buffer_size)
    {
        status =  fs_set_abort(connector_ptr,
                               context,
                               connector_request_id_file_system_read,
                               connector_invalid_data_size);
        goto done;
    }

    *buffer_size = data.bytes_used;

done:
    return status;
}

STATIC connector_status_t call_file_write_user(connector_data_t * const connector_ptr,
                                               msg_service_request_t * const service_request,
                                               fs_context_t * const context,
                                               void const * const buffer,
                                               size_t * const bytes_done)
{
    connector_status_t status;
    connector_file_system_write_t data;

    data.handle = context->handle.file;
    data.buffer = buffer;
    data.bytes_available = *bytes_done;
    data.bytes_used = 0;

    status = fs_call_user(connector_ptr,
                          service_request,
                          context,
                          connector_request_id_file_system_write,
                          &data);

    if (!FsOperationSuccess(status, context))
        goto done;


    if (data.bytes_used > *bytes_done)
    {
        status =  fs_set_abort(connector_ptr,
                               context,
                               connector_request_id_file_system_write,
                               connector_invalid_data_size);
        goto done;
    }

    *bytes_done = data.bytes_used;

    if (*bytes_done == 0)
    {
        status = connector_pending;
    }

done:
    return status;
}

STATIC size_t parse_file_path(fs_context_t * const context,
                              void const * const path_ptr,
                              size_t const buffer_size)
{
    char const * const path = path_ptr;
    size_t path_len = strnlen_(path, buffer_size);

    if (path_len == 0)
    {
        FsSetInternalError(context, fs_error_format);
    }
    else
    if (path_len >= buffer_size)
    {
        FsSetInternalError(context, fs_error_path_too_long);
        path_len = 0;
    }
    else
        path_len++;

    return path_len;
}

STATIC size_t parse_file_get_header(fs_context_t * const context,
                                    uint8_t const * const header_ptr,
                                    size_t const buffer_size)
{

    /* 1st message so let's parse message-start packet:
     *
     * File System Get request format:
     *  -------------------------------------
     * |   0    |   N    |  +4    |    +4    |
     *  --------------------------------------
     * | Opcode | Path   | Offset | Length   |
     *  -------------------------------------
     *
     */

    enum fs_get_request {
        field_define(fs_get_request, offset, uint32_t),
        field_define(fs_get_request, length, uint32_t),
        record_end(fs_get_request_header)
    };
    uint8_t const * fs_get_request = header_ptr + FS_OPCODE_BYTES;
    size_t  const header_len = record_bytes(fs_get_request_header) + FS_OPCODE_BYTES;

    size_t len = parse_file_path(context, fs_get_request, buffer_size - header_len);
    if (len == 0)
        goto done;

    fs_get_request += len;
    context->data.f.offset = message_load_be32(fs_get_request, offset);
    if (context->data.f.offset < 0)
    {
        FsSetInternalError(context, fs_error_large_file);
        len = 0;
        goto done;
    }
    context->data.f.data_length = message_load_be32(fs_get_request, length);
    len += header_len;

done:
    return len;
}

STATIC connector_status_t process_file_get_request(connector_data_t * const connector_ptr,
                                                   msg_service_request_t * const service_request,
                                                   fs_context_t * const context)
{
    msg_service_data_t * const service_data = service_request->have_data;
    connector_status_t status = connector_working;

    if (parse_file_get_header(context, service_data->data_ptr, service_data->length_in_bytes) > 0)
    {
        char const * path = service_data->data_ptr;
        path += FS_OPCODE_BYTES;

        status = call_file_open_user(connector_ptr, service_request, context, path, CONNECTOR_FILE_O_RDONLY);
    }

    return status;
}

STATIC connector_status_t set_file_position(connector_data_t * const connector_ptr,
                                            msg_service_request_t * const service_request,
                                            fs_context_t * const context)

{
    connector_status_t status = connector_working;
    connector_file_offset_t ret;

    /* Check that offset is inside the file
       Some systems crash, when trying to set offset outside the file
     */

    if (FsGetState(context) < fs_state_lseek1)
    {
        status = call_file_lseek_user(connector_ptr, service_request, context, 0, connector_file_system_seek_end, &ret);
        if (!FsOperationSuccess(status, context))
            goto done;

        if (ret < context->data.f.offset)
        {
            ret = -1;
            FsSetInternalError(context, fs_error_invalid_offset);
            goto done;
        }

        FsSetState(context, fs_state_lseek1);
        status = call_file_lseek_user(connector_ptr, service_request, context, context->data.f.offset, connector_file_system_seek_set, &ret);
        if (FsOperationSuccess(status, context) && ret == -1)
        {
            FsSetInternalError(context, fs_error_invalid_offset);
        }
    }
done:
    return status;
}

STATIC connector_status_t process_get_close(connector_data_t * const connector_ptr,
                                            msg_service_request_t * const service_request,
                                            fs_context_t * const context,
                                            connector_request_id_file_system_t close_request)
{

    msg_service_data_t * const service_data = service_request->need_data;
    connector_status_t status = connector_working;

    if (FsGetState(context) < fs_state_closed)
    {
        status = call_file_close_user(connector_ptr,
                                      service_request,
                                      context,
                                      close_request);

        if (status == connector_pending)
        {
            if (context->status == connector_working &&
                context->errnum.user == CONNECTOR_FILESYSTEM_ERRNUM_NONE &&
                service_data->length_in_bytes > 0)
            {
                /* Return final data portion, will set last bit later
                   when closing the file completes in the next callback */
                status = connector_working;
            }
            goto done;
        }
    }

    /* finished closing file */
    if (context->status == connector_abort)
        status = connector_abort;

    /* abort condition or messaging error and session will be canceled */
    if (status == connector_abort || service_request->service_type == msg_service_type_error)
        goto done;

    /* no errors, set last bit and send out last portion of data */
    if (context->errnum.user == CONNECTOR_FILESYSTEM_ERRNUM_NONE)
    {
        MsgSetLastData(service_data->flags);
        goto done;
    }
    /* errors */
    if (MsgIsStart(service_data->flags))
    {
        /* send file system-level an error response if no data was sent yet, */
        if (format_file_error_msg(connector_ptr, service_request, context) == connector_abort)
           status = connector_abort;
    }
    else
    {
        /* or cancel the session, if it's late to send a file system level error response */
        fs_set_service_error(service_request, connector_session_error_cancel);
    }

done:
    return status;
}


STATIC connector_status_t process_file_get_response(connector_data_t * const connector_ptr,
                                                    msg_service_request_t * const service_request,
                                                    fs_context_t * const context)
{
    msg_service_data_t * const service_data = service_request->need_data;
    connector_status_t status = connector_working;

    if ((context->errnum.user != CONNECTOR_FILESYSTEM_ERRNUM_NONE) || (FsGetState(context) >= fs_state_closing))
    {
        service_data->length_in_bytes = 0;
        goto close_file;
    }

    {
        uint8_t * data_ptr = service_data->data_ptr;
        size_t buffer_size = service_data->length_in_bytes;
        connector_bool_t last_msg = connector_false;
        size_t bytes_read = 0;
        size_t bytes_to_read;

        if (MsgIsStart(service_data->flags))
        {
           if (FsGetState(context) < fs_state_lseek)
           {
               if (context->data.f.offset != 0)
               {
                    status = set_file_position(connector_ptr, service_request, context);
                    if (status == connector_pending)
                        goto done;

                    if (!FsOperationSuccess(status, context))
                        goto close_file;
               }
               FsSetState(context, fs_state_lseek);
           }
           *data_ptr++ = fs_get_response_opcode;
           buffer_size--;
        }

        /* bytes to read in this callback */
        bytes_to_read = MIN_VALUE(buffer_size, context->data.f.data_length - context->data.f.bytes_done);

        while (bytes_to_read > 0)
        {
            size_t cnt = bytes_to_read;
            status = call_file_read_user(connector_ptr, service_request, context, data_ptr, &cnt);

            if (status == connector_pending)
            {
                if (bytes_read > 0)
                    status = connector_working;  /* Return what's read already */
                break;
            }
            if (!FsOperationSuccess(status, context))
                goto close_file;

            if (cnt > 0)
            {
                data_ptr += cnt;
                bytes_to_read -= cnt;
                bytes_read += cnt;
            }
            else
            {
                bytes_to_read = 0;
                last_msg = connector_true;
            }
        }
        context->data.f.bytes_done += bytes_read;
        service_data->length_in_bytes = bytes_read;

        if (MsgIsStart(service_data->flags))
            service_data->length_in_bytes++; /* opcode */

        if (context->data.f.data_length == context->data.f.bytes_done)
            last_msg = connector_true;

        if (!last_msg)
            goto done;
    }

close_file:
    status = process_get_close(connector_ptr,
                               service_request,
                               context,
                               connector_request_id_file_system_close);

done:
    return status;
}

STATIC connector_status_t process_file_rm_request(connector_data_t * const connector_ptr,
                                                  msg_service_request_t * const service_request,
                                                  fs_context_t * const context)
{
    msg_service_data_t * const service_data = service_request->have_data;
    char const * path = service_data->data_ptr;
    connector_status_t status = connector_working;

    path += FS_OPCODE_BYTES;

    if (parse_file_path(context, path, service_data->length_in_bytes - FS_OPCODE_BYTES) == 0)
        goto done;

    status = call_file_rm_user(connector_ptr, service_request, context, path);

done:
    return status;
}

STATIC connector_status_t process_file_response_nodata(connector_data_t * const connector_ptr,
                                                       msg_service_request_t * const service_request,
                                                       fs_context_t * const context,
                                                       fs_opcode_t const opcode)
{
    msg_service_data_t * const service_data = service_request->need_data;
    connector_status_t status = connector_working;

    if (context->errnum.user == CONNECTOR_FILESYSTEM_ERRNUM_NONE)
    {
        uint8_t * const data_ptr = service_data->data_ptr;

        *data_ptr = opcode;

        service_data->length_in_bytes = sizeof *data_ptr;
        MsgSetLastData(service_data->flags);
    }
    else
    {
        if (format_file_error_msg(connector_ptr, service_request, context) == connector_abort)
        {
            status = connector_abort;
        }
    }
    return status;
}

STATIC size_t parse_file_put_header(fs_context_t * const context,
                                    uint8_t const * const header_ptr,
                                    size_t const buffer_size)
{
    /*
     *
     * File System Put request format:
     *  --------------------------------------------
     * |   0    |   N    |  +1   |  +4    |   N     |
     *  --------------------------------------------
     * | Opcode | Path   | Flags | Offset | Payload |
     *  --------------------------------------------
     *
     */

    enum {
        field_define(fs_put_request, flags,  uint8_t),
        field_define(fs_put_request, offset, uint32_t),
        record_end(fs_put_request_header)
    };
    uint8_t const *fs_put_request = header_ptr + FS_OPCODE_BYTES;
    size_t  const header_len = record_bytes(fs_put_request_header) + FS_OPCODE_BYTES;

    size_t len = parse_file_path(context, fs_put_request, buffer_size - header_len);
    if (len == 0)
        goto done;

    fs_put_request += len;
    context->flags  |= message_load_u8(fs_put_request, flags);
    context->data.f.offset = message_load_be32(fs_put_request, offset);
    if (context->data.f.offset < 0)
    {
        FsSetInternalError(context, fs_error_large_file);
        len = 0;
        goto done;
    }
    len   += header_len;

done:
    return len;
}


STATIC connector_status_t process_file_put_request(connector_data_t * const connector_ptr,
                                                   msg_service_request_t * const service_request,
                                                   fs_context_t * const context)
{
    connector_status_t status = connector_working;

    if ((context->errnum.user != CONNECTOR_FILESYSTEM_ERRNUM_NONE) || (FsGetState(context) >= fs_state_closing))
        goto close_file;

    {
        msg_service_data_t * const service_data = service_request->have_data;
        uint8_t const * data_ptr = service_data->data_ptr;
        size_t bytes_to_write = service_data->length_in_bytes;
        size_t bytes_written = 0;

        if (MsgIsStart(service_data->flags))
        {
            size_t header_len = parse_file_put_header(context, data_ptr, service_data->length_in_bytes);
            if (header_len == 0)
                goto done;

            if (FsGetState(context) < fs_state_open)
            {
                int open_flags = CONNECTOR_FILE_O_WRONLY | CONNECTOR_FILE_O_CREAT;
                char const * path = service_data->data_ptr;
                path += FS_OPCODE_BYTES;

                if (FsNeedTrunc(context))
                {
                    if (context->data.f.offset == 0)
                    {
                        open_flags |= CONNECTOR_FILE_O_TRUNC;
                        FsClearTrunc(context);
                    }
                }

                status = call_file_open_user(connector_ptr, service_request, context, path, open_flags);
                if (FsGetState(context) != fs_state_open)
                    goto done;
            }

            if (FsGetState(context) < fs_state_lseek)
            {
                if (context->data.f.offset != 0)
                {
                    status = set_file_position(connector_ptr, service_request, context);
                    if (status == connector_pending)
                        goto done;

                    if (!FsOperationSuccess(status, context))
                        goto close_file;
                }
                FsSetState(context, fs_state_lseek);
            }
            data_ptr  += header_len;
            bytes_to_write -= header_len;
        }

        data_ptr += context->data.f.bytes_done;
        bytes_to_write -= context->data.f.bytes_done;

        while(bytes_to_write > 0)
        {
            size_t cnt = bytes_to_write;
            status = call_file_write_user(connector_ptr, service_request, context, data_ptr, &cnt);

            if (status == connector_pending)
            {
                context->data.f.bytes_done += bytes_written;
                context->data.f.offset  += bytes_written;
                goto done;
            }
            if (!FsOperationSuccess(status, context))
               goto close_file;

            data_ptr       += cnt;
            bytes_to_write -= cnt;
            bytes_written  += cnt;
        }
        context->data.f.bytes_done = 0;
        context->data.f.offset  += bytes_written;

        if (!MsgIsLastData(service_data->flags))
            goto done;

        if (FsNeedTrunc(context))
        {
            status = call_file_ftruncate_user(connector_ptr, service_request, context);
            if (status == connector_pending)
                goto done;
        }
    }
close_file:
    status = call_file_close_user(connector_ptr,
                                  service_request,
                                  context,
                                  connector_request_id_file_system_close);

    if (status == connector_pending)
        goto done;

    /* finished closing file */
    if (context->status == connector_abort)
        status = connector_abort;

done:
    return status;
}

STATIC size_t parse_file_ls_header(fs_context_t * const context,
                                   uint8_t const * const header_ptr,
                                   size_t const buffer_size)
{
    /*
     * File System Ls request format:
     *  ----------------------------
     * |   0    |   N    |   1      |
     *  ----------------------------
     * | Opcode | Path   | hash alg |
     *  ----------------------------
     */
    uint8_t const * data_ptr = header_ptr + FS_OPCODE_BYTES;
    const size_t header_len = 1 + FS_OPCODE_BYTES;
    connector_file_system_hash_algorithm_t hash_alg;

    size_t len = parse_file_path(context, data_ptr, buffer_size - header_len);
    if (len != 0)
    {
        data_ptr += len;
        len += header_len;

        hash_alg = (connector_file_system_hash_algorithm_t) *data_ptr;

        switch (hash_alg)
        {
            case connector_file_system_hash_crc32:
            case connector_file_system_hash_md5:
            case connector_file_system_hash_best:
            case connector_file_system_hash_none:
                context->data.d.hash_alg = hash_alg;
                break;

            default:
                FsSetInternalError(context, fs_error_invalid_hash);
                len = 0;
        }
    }
    return len;
}

STATIC size_t format_file_ls_response_header(connector_file_system_hash_algorithm_t const hash_alg,
                                             size_t const hash_size,
                                             uint8_t * const data_ptr)
{
   /*
     * File System Ls response header format:
     *  --------------------------------
     * |   1    |    1     |    1       |
     *  --------------------------------
     * | opcode | hash_alg | hash_bytes |
     *  --------------------------------
     */

    enum {
        field_define(fs_ls_response, opcode,     uint8_t),
        field_define(fs_ls_response, hash_alg,   uint8_t),
        field_define(fs_ls_response, hash_bytes, uint8_t),
        record_end(fs_ls_response_header)
    };

    uint8_t * fs_ls_response = data_ptr;

    message_store_u8(fs_ls_response, opcode,     fs_ls_response_opcode);
    message_store_u8(fs_ls_response, hash_alg,   hash_alg);
    message_store_u8(fs_ls_response, hash_bytes, hash_size);

    return record_bytes(fs_ls_response_header);
}

STATIC size_t file_ls_resp_header_size(void)
{
    /*
     * File System Ls request format:
     *  ----------------------------
     * |   1    |     4    |   0/4  |
     *  ----------------------------
     * | flags  | last     | size   |
     * |        | modified |        |
     *  ----------------------------
     */

    enum  {
        field_define(fs_ls_response, flags,  uint8_t),
        field_define(fs_ls_response, last_modified, uint32_t),
        field_define(fs_ls_response, size, connector_file_offset_t),
        record_end(fs_ls_response_file)
    };
    return record_bytes(fs_ls_response_file);
}

STATIC size_t format_file_ls_response(fs_context_t const * context,
                                      char const * path,
                                      size_t const path_len,
                                      uint8_t * const data_ptr)
{
    /*
     * File System Ls request format:
     *  ----------------------------
     * |   1    |     4    |   0/4  |
     *  ----------------------------
     * | flags  | last     | size   |
     * |        | modified |        |
     *  ----------------------------
     */

    enum  {
        field_define(fs_ls_response, flags,  uint8_t),
        field_define(fs_ls_response, last_modified, uint32_t),
        record_end(fs_ls_response_dir)
    };
    enum  {
        field_define(fs_ls_response, size, connector_file_offset_t),
        record_end(fs_ls_response_file)
    };

    uint8_t * fs_ls_response = data_ptr + path_len;
    uint8_t flags = FsIsDir(context) ? FS_IS_DIR_FLAG : 0;
    size_t result;

#if (defined CONNECTOR_FILE_SYSTEM_HAS_LARGE_FILES)
    flags |= FS_IS_LARGE_FLAG;
#endif
    memcpy(data_ptr, path, path_len);

    message_store_u8(fs_ls_response,   flags, flags);
    message_store_be32(fs_ls_response, last_modified, context->data.d.last_modified);
    result = path_len + record_bytes(fs_ls_response_dir);

    if (!FsIsDir(context))
    {
        fs_ls_response += record_bytes(fs_ls_response_dir);
#if (defined CONNECTOR_FILE_SYSTEM_HAS_LARGE_FILES)
        message_store_be64(fs_ls_response, size, context->data.d.file_size);
#else
        message_store_be32(fs_ls_response, size, context->data.d.file_size);
#endif
        result += record_bytes(fs_ls_response_file);
    }

    return result;
}

STATIC connector_status_t file_store_path(fs_context_t * const context,
                                          char const * const path)
{
    connector_status_t status = connector_working;
    size_t path_len = strlen(path);

    if (FsIsDir(context) && path[path_len - 1] != '/')
    {
        path_len++;
    }

    if (path_len >= CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH)
    {
        FsSetInternalError(context, fs_error_path_too_long);
        goto done;
    }

    memcpy(context->data.d.path, path, path_len + 1);

    if (FsIsDir(context) && (path[path_len - 1] != '/'))
    {
        context->data.d.path[path_len - 1] = '/';
        context->data.d.path[path_len] = '\0';
    }
    context->data.d.path_len = path_len;

done:
    return status;
}

STATIC connector_status_t process_file_ls_request(connector_data_t * const connector_ptr,
                                                  msg_service_request_t * const service_request,
                                                  fs_context_t * const context)
{
    msg_service_data_t    * const service_data = service_request->have_data;
    char const * path = service_data->data_ptr;
    connector_status_t status = connector_working;

    path += FS_OPCODE_BYTES;

    if (context->errnum.user != CONNECTOR_FILESYSTEM_ERRNUM_NONE)
        goto done;

    if (parse_file_ls_header(context, service_data->data_ptr, service_data->length_in_bytes) == 0)
        goto done;

    if (FsGetState(context) < fs_state_stat)
    {
        status = call_file_stat_user(connector_ptr, service_request, context, path, context->data.d.hash_alg);
        if (!FsOperationSuccess(status, context))
        {
            goto done;
        }

        FsSetState(context, fs_state_stat);
    }
    if (FsIsDir(context))
    {
        status = call_file_opendir_user(connector_ptr, service_request, context, path);
        if (FsGetState(context) != fs_state_open)
            goto done;
    }
    else
    {
        FsSetLsSingleFile(context);
    }

    /* call_file_stat_user must be done before store path, to strcat '/' to dir path */
    status = file_store_path(context, path);

done:
    return status;
}

STATIC connector_status_t process_file_ls_response(connector_data_t * const connector_ptr,
                                                   msg_service_request_t * const service_request,
                                                   fs_context_t * const context)
{
    msg_service_data_t    * const service_data = service_request->need_data;
    connector_status_t status = connector_working;

    if ((context->errnum.user != CONNECTOR_FILESYSTEM_ERRNUM_NONE) || (FsGetState(context) >= fs_state_closing))
    {
       service_data->length_in_bytes = 0;
       goto close_dir;
    }
    {
        uint8_t * data_ptr = service_data->data_ptr;
        size_t buffer_size = service_data->length_in_bytes;
        size_t resp_len    = 0;

        size_t const header_len = file_ls_resp_header_size();
        size_t hash_len   = file_hash_size(context->data.d.hash_alg);
        char * file_path;
        size_t file_path_len;

        if (MsgIsStart(service_data->flags))
        {
            resp_len = format_file_ls_response_header(context->data.d.hash_alg, hash_len, data_ptr);
            data_ptr += resp_len;
            buffer_size -= resp_len;
        }

        if (FsIsLsSingleFile(context))
        {
            /* ls command was issued for a single file */
            file_path_len = context->data.d.path_len + 1;
            file_path     = context->data.d.path;

            if ((file_path_len + header_len + hash_len) > buffer_size)
            {
                FsSetInternalError(context, fs_error_path_too_long);
                goto close_dir;
            }

            if (hash_len != 0)
            {
                uint8_t * const hash_ptr = data_ptr + file_path_len + header_len;
                status = call_file_hash_user(connector_ptr, service_request, context, file_path, hash_ptr);
                if (status == connector_pending || status == connector_abort)
                    goto done;

                if (status == connector_working && context->errnum.user != CONNECTOR_FILESYSTEM_ERRNUM_NONE)
                {
                    if (format_file_error_msg(connector_ptr, service_request, context) == connector_abort)
                    {
                        status = connector_abort;
                    }
                    goto done;
                }
                resp_len += hash_len;
            }

            MsgSetLastData(service_data->flags);
        }
        else
        {
            /* ls command was issued for a directory */
            file_path = context->data.d.path + context->data.d.path_len;

            while (FsGetState(context) < fs_state_readdir)
            {
                /* read next dir entry */
                size_t len = header_len + hash_len + context->data.d.path_len;
                size_t path_max = 0;

                context->flags = 0;
                /* minimum of bytes left for the entry name in output buffer and context->data.d.path buffer */
                if (len < buffer_size)
                    path_max = MIN_VALUE((buffer_size - len), (CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH - context->data.d.path_len));

                status = call_file_readdir_user(connector_ptr, service_request, context, file_path, path_max);
                if (status == connector_pending)
                    goto done;

                if (!FsOperationSuccess(status, context))
                    goto close_dir;

                if (*file_path == '\0')
                {
                    /* all entries processed */
                    service_data->length_in_bytes = resp_len;
                    goto close_dir;
                }
                /* Don't send "." in any directory and ".." in "/" directory up to Device Cloud */
                if ((memcmp(file_path, ".", 2) != 0) &&
                    (memcmp(context->data.d.path, "/..", 3) != 0))
                {
                    FsSetState(context, fs_state_readdir);
                }
            }

            file_path_len = strlen(file_path) + 1;

            if (FsGetState(context) < fs_state_stat_dir_entry)
            {
                status = call_file_stat_dir_entry_user(connector_ptr, service_request, context, context->data.d.path);
                if (status == connector_pending)
                    goto done;

                if (!FsOperationSuccess(status, context))
                    goto close_dir;

                FsSetState(context, fs_state_stat_dir_entry);
            }
            file_path = context->data.d.path;
            file_path_len = context->data.d.path_len + file_path_len;

            if (FsIsDir(context))
                hash_len = 0;

            if (hash_len != 0)
            {
                uint8_t * const hash_ptr = data_ptr + file_path_len + header_len;

                if (FsIsReg(context))
                {
                    status = call_file_hash_user(connector_ptr, service_request, context, file_path, hash_ptr);
                    if (status == connector_pending)
                        goto done;

                    if (!FsOperationSuccess(status, context))
                        goto close_dir;
                }
                else
                {
                    memset(hash_ptr, 0, hash_len);
                }
                resp_len += hash_len;
            }

            /* to read next dir entry */
            FsSetState(context, fs_state_open);
        }
        resp_len += format_file_ls_response(context, file_path, file_path_len, data_ptr);
        service_data->length_in_bytes = resp_len;
        goto done;
    }

close_dir:
    status = process_get_close(connector_ptr,
                               service_request,
                               context,
                               connector_request_id_file_system_closedir);
done:
    return status;
}

STATIC connector_status_t allocate_file_context(connector_data_t * const connector_ptr,
                                              fs_opcode_t const opcode,
                                              fs_context_t * * const result)
{
    fs_context_t * context = NULL;
    connector_status_t status;

    void * ptr;

    status = malloc_data_buffer(connector_ptr, sizeof *context, named_buffer_id(msg_service), &ptr);
    if (status != connector_working)
        goto done;

    context = ptr;

    context->handle.file = CONNECTOR_FILESYSTEM_FILE_HANDLE_NOT_INITIALIZED;
    context->handle.dir = CONNECTOR_FILESYSTEM_DIR_HANDLE_NOT_INITIALIZED;
    context->user_context = NULL;
    context->flags = 0;
    context->errnum.user = CONNECTOR_FILESYSTEM_ERRNUM_NONE;
    context->state = fs_state_none;
    context->status = connector_working;

    if (opcode != fs_ls_request_opcode)
    {
        context->data.f.bytes_done = 0;
    }

done:
    *result = context;
    return status;
}

STATIC connector_status_t file_system_request_callback(connector_data_t * const connector_ptr,
                                                       msg_service_request_t * const service_request)
{
    msg_session_t * const session = service_request->session;
    msg_service_data_t * const service_data = service_request->have_data;
    fs_context_t * context = session->service_context;
    connector_status_t status = connector_working;

    if (MsgIsStart(service_data->flags))
    {
        if (context == NULL)
        {
            uint8_t const * const ptr = service_data->data_ptr;
            const fs_opcode_t opcode = (fs_opcode_t) *ptr;

            status = allocate_file_context(connector_ptr, opcode, &context);
            if (status != connector_working)
            {
                 goto done;
            }
            session->service_context = context;
            context->opcode = opcode;
        }
    }
    else
    {
        if (context == NULL)
        {
            fs_set_service_error(service_request, connector_session_error_format);
            ASSERT_GOTO(connector_false, done);
        }
    }

    if (context->opcode != fs_put_request_opcode)
    {
        /* don't support request in >1 message */
        if ( !(MsgIsStart(service_data->flags) && MsgIsLastData(service_data->flags)) )
        {
            FsSetInternalError(context, fs_error_path_too_long);
            goto done;
        }
    }

    switch (context->opcode)
    {
        case fs_get_request_opcode:
            status = process_file_get_request(connector_ptr, service_request, context);
            break;

        case fs_put_request_opcode:
            status = process_file_put_request(connector_ptr, service_request, context);
            break;

        case fs_rm_request_opcode:
            status = process_file_rm_request(connector_ptr, service_request, context);
            break;

        case fs_ls_request_opcode:
            status = process_file_ls_request(connector_ptr, service_request, context);
            break;

        default:
            FsSetInternalError(context, fs_error_format);
            ASSERT(connector_false);
    }

done:
    return status;
}

STATIC connector_status_t file_system_response_callback(connector_data_t * const connector_ptr,
                                                        msg_service_request_t * const service_request)
{
    msg_session_t * const session = service_request->session;
    fs_context_t * const context = session->service_context;
    connector_status_t status = connector_working;

    if (context == NULL)
    {
        fs_set_service_error(service_request, connector_session_error_unknown_session);
        ASSERT_GOTO(connector_false, done);
    }


    switch (context->opcode)
    {
        case fs_get_request_opcode:
            status = process_file_get_response(connector_ptr, service_request, context);
            break;

        case fs_put_request_opcode:
            status = process_file_response_nodata(connector_ptr, service_request, context, fs_put_response_opcode);
            break;

        case fs_rm_request_opcode:
            status = process_file_response_nodata(connector_ptr, service_request, context, fs_rm_response_opcode);
            break;

        case fs_ls_request_opcode:
            status = process_file_ls_response(connector_ptr, service_request, context);
            break;

        default:
            fs_set_service_error(service_request, connector_session_error_unknown_session);
            ASSERT_GOTO(connector_false, done);
            break;
    }

done:
    return status;
}

STATIC connector_status_t file_system_free_callback(connector_data_t * const connector_ptr,
                                                    msg_service_request_t * const service_request)
{
    msg_session_t * const session = service_request->session;
    fs_context_t * const context = session->service_context;
    connector_status_t status = connector_working;

    if (context != NULL)
    {
        status = free_data_buffer(connector_ptr, named_buffer_id(msg_service), context);
    }

    return status;
}

STATIC connector_status_t call_session_error_user(connector_data_t * const connector_ptr,
                                                  msg_service_request_t * const service_request,
                                                  fs_context_t * const context)
{
    connector_status_t status = connector_working;

    if (context != NULL && FsSessionErrorCalled(context))
        goto done;

    {
        connector_file_system_session_error_t data;
        connector_request_id_t request_id;
        connector_callback_status_t callback_status;

        request_id.file_system_request = connector_request_id_file_system_session_error;
        data.session_error = service_request->error_value;
        data.user_context = context == NULL ? NULL : context->user_context;

        callback_status = connector_callback(connector_ptr->callback,
                               connector_class_id_file_system,
                               request_id,
                               &data,
                               connector_ptr->context
                                );

        if (context != NULL)
            context->user_context = data.user_context;

        switch(callback_status)
        {
            case connector_callback_busy:
                status = connector_pending;
                goto done;

            case connector_callback_continue:
                break;

            case connector_callback_abort:
            default:
                status = connector_abort;
                if (context != NULL)
                    context->status = connector_abort;
                break;
        }

        if (context != NULL)
            FsSetSessionErrorCalled(context);
    }

done:
    return status;
}


STATIC connector_status_t file_system_session_error_callback(connector_data_t * const connector_ptr,
                                                             msg_service_request_t * const service_request)
{
    msg_session_t * const session = service_request->session;
    fs_context_t * const context = session->service_context;

    connector_status_t status = call_session_error_user(connector_ptr, service_request, context);

    if (context != NULL)
    {
        connector_request_id_file_system_t fs_request_id = context->opcode == fs_ls_request_opcode ?
                                        connector_request_id_file_system_closedir :
                                        connector_request_id_file_system_close;

        status = call_file_close_user(connector_ptr, service_request, context, fs_request_id);
        if (status == connector_pending)
            goto done;

        if (context->status == connector_abort)
            status = connector_abort;
    }

done:
    return status;
}


STATIC connector_status_t file_system_callback(connector_data_t * const connector_ptr,
                                               msg_service_request_t * const service_request)
{
    connector_status_t status = connector_abort;

    ASSERT_GOTO(connector_ptr != NULL, done);
    ASSERT_GOTO(service_request != NULL, done);
    ASSERT_GOTO(service_request->session != NULL, done);

    switch (service_request->service_type)
    {
    case msg_service_type_have_data:
        status = file_system_request_callback(connector_ptr, service_request);
        break;

    case msg_service_type_need_data:
        status = file_system_response_callback(connector_ptr, service_request);
        break;

    case msg_service_type_error:
         status = file_system_session_error_callback(connector_ptr, service_request);
        break;

    case msg_service_type_free:
        status = file_system_free_callback(connector_ptr, service_request);
        break;

    default:
        ASSERT(connector_false);
        break;
    }

done:
    return status;
}

STATIC connector_status_t connector_facility_file_system_cleanup(connector_data_t * const connector_ptr)
{
    return msg_cleanup_all_sessions(connector_ptr,  msg_service_id_file);
}

STATIC connector_status_t connector_facility_file_system_delete(connector_data_t * const data_ptr)
{
    return msg_delete_facility(data_ptr, msg_service_id_file);
}

STATIC connector_status_t connector_facility_file_system_init(connector_data_t * const data_ptr, unsigned int const facility_index)
{
    return msg_init_facility(data_ptr, facility_index, msg_service_id_file, file_system_callback);
}
