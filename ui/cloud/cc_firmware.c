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
#include "connector_api.h"
#include "platform.h"
#include "firmware_version.h"
#include "digiutils.h"

typedef enum
{
    firmware_target_image,
    /* new firmware type goes here... */
    firmware_target_COUNT
} firmware_target_t;

typedef struct
{
    char const * filespec;
    char const * description;
    char * path;
    FILE * fd;
} firmware_info_t;

/* list of all supported firmware target info */
static firmware_info_t firmware_list[firmware_target_COUNT] =
{
    {"[-+.a-zA-Z0-9]+\\.[bB][iI][nN]", "Binary Image", NULL, NULL}
    /* new entry goes here... */
};

static firmware_info_t * get_firmware_info(unsigned int const target_no)
{
    firmware_info_t * info = NULL;

    if (target_no < firmware_target_COUNT)
    {
        info = &firmware_list[target_no];
    }

    return info;
}

static void app_firmware_cleanup(firmware_info_t * const info)
{
    if (info == NULL) goto done;

    if (info->fd != NULL)
    {
        fclose(info->fd);
        info->fd = NULL;
    }

    free(info->path);
    info->path = NULL;

done:
    return;
}

static connector_callback_status_t app_firmware_target_count(connector_firmware_count_t * const target_info)
{
    target_info->count = firmware_target_COUNT;
    return connector_callback_continue;
}

static connector_callback_status_t app_firmware_target_info(connector_firmware_info_t * const request_info)
{
    connector_callback_status_t status = connector_callback_error;
    firmware_info_t * const info = get_firmware_info(request_info->target_number);

    if (info == NULL) goto error;

    request_info->description = info->description;
    request_info->filespec = info->filespec;

    {
        static int const version_octet_count = 4;
        char * version_ptr = NULL;
        int result = get_tlr_firmware_version(&version_ptr);

        if (result != 0) goto error;

        result = sscanf(version_ptr, "%hhu.%hhu.%hhu.%hhu%*s",
                        &request_info->version.major,
                        &request_info->version.minor,
                        &request_info->version.revision,
                        &request_info->version.build);
        free(version_ptr);

        if (result != version_octet_count) goto error;
    }

    status = connector_callback_continue;

error:
    return status;
}

static connector_callback_status_t app_firmware_download_request(connector_firmware_download_start_t * const download_info)
{
    connector_callback_status_t status = connector_callback_continue;
    firmware_info_t * const info = get_firmware_info(download_info->target_number);

    if (info == NULL) goto error;
    if (download_info->filename == NULL || download_info->filename[0] == '\0') goto error;

    info->path = NULL;

    {
        int const result = asprintf(&info->path, "/tmp/%s", download_info->filename);

        if (result <= 0) goto error;
    }

    info->fd = fopen(info->path, "wb");
    if (info->fd == NULL) goto error;

    download_info->status = connector_firmware_status_success;
    goto done;

error:
    download_info->status = connector_firmware_status_download_denied;
    app_firmware_cleanup(info);

done:
    return status;
}

static connector_callback_status_t app_firmware_image_data(connector_firmware_download_data_t * const image_data)
{
    connector_callback_status_t status = connector_callback_continue;
    firmware_info_t * const info = get_firmware_info(image_data->target_number);

    if (info == NULL) goto error;
    if (info->fd == NULL) goto error;

    {
        size_t const bytes_written = fwrite(image_data->image.data, 1, image_data->image.bytes_used, info->fd);

        if (bytes_written == image_data->image.bytes_used)
        {
            image_data->status = connector_firmware_status_success;
            goto done;
        }
    }

error:
    image_data->status = connector_firmware_status_encountered_error;
    app_firmware_cleanup(info);

done:
    return status;
}

static connector_callback_status_t app_firmware_download_complete(connector_firmware_download_complete_t * const download_complete)
{
    connector_callback_status_t status = connector_callback_continue;
    firmware_info_t * const info = get_firmware_info(download_complete->target_number);
    int result;

    if (info == NULL) goto error;
    if (info->fd == NULL) goto error;

    fclose(info->fd);
    info->fd = NULL;
    APP_DEBUG("download complete, applying...\n");

    switch (download_complete->target_number)
    {
        case firmware_target_image:
            result = system_command("action_update_firmware -u %s %s", "cloud", info->path);
            if (result != EXIT_SUCCESS)
            {
                goto error;
            }
            break;

        default:
            goto error;
    }

    download_complete->status = connector_firmware_download_success;
    goto done;

error:
    download_complete->status = connector_firmware_download_not_complete;

done:
    app_firmware_cleanup(info);
    return status;
}

static connector_callback_status_t app_firmware_download_abort(connector_firmware_download_abort_t const * const abort_data)
{
    firmware_info_t * const info = get_firmware_info(abort_data->target_number);

    /* Device Cloud is aborting firmware update */
    APP_DEBUG("firmware download aborted, status = %d\n", abort_data->status);
    if (info != NULL)
    {
        app_firmware_cleanup(info);
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_firmware_reset(connector_firmware_reset_t const * const reset_data)
{
    /* Device Cloud requests firmware reboot */
    switch (reset_data->target_number)
    {
        case firmware_target_image:
            APP_DEBUG("reset after firmware update\n");
            system_command("action_reboot");
            break;

        default:
            break;
    }

    return connector_callback_continue;
}

connector_callback_status_t app_firmware_handler(connector_request_id_firmware_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_unrecognized;

    switch (request_id)
    {
    case connector_request_id_firmware_target_count:
        status = app_firmware_target_count(data);
        break;

    case connector_request_id_firmware_info:
        status = app_firmware_target_info(data);
        break;

    case connector_request_id_firmware_download_start:
        status = app_firmware_download_request(data);
        break;

    case connector_request_id_firmware_download_data:
        status = app_firmware_image_data(data);
        break;

    case connector_request_id_firmware_download_complete:
        status = app_firmware_download_complete(data);
        break;

    case connector_request_id_firmware_download_abort:
        status =  app_firmware_download_abort(data);
        break;

    case connector_request_id_firmware_target_reset:
        status =  app_firmware_reset(data);
        break;

    }

    return status;
}
