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
#include <stdlib.h>

#include "connector_api.h"
#include "platform.h"
#include "fservapi.h"


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#endif

typedef struct {
    uint32_t    version;
    uint32_t    code_size;
    char        * name_spec;
    char        * description;
} firmware_list_t;

/* list of all supported firmware target info */
static firmware_list_t firmware_list[] = {
    /* version     code_size     name_spec          description */
    {0x01000000, -1, ".*\\.[tT][xX][tT]", "Text File"}, /* any *.a files */
    {0x00000100, -1, ".*\\.[bB][iI][nN]", "Binary Image" }  /* any *.bin files */
};

static int firmware_download_started = 0;
static size_t total_image_size = 0;

static char filename[256];
static int handle = 0;
static int is_fw_image = 0;

static connector_callback_status_t app_firmware_download_request(connector_fw_download_request_t const * const download_info, connector_fw_status_t * download_status)
{
    connector_callback_status_t   status = connector_callback_continue;

    if ((download_info == NULL) || (download_status == NULL))
    {
        APP_DEBUG("firmware_download_request ERROR: iDigi passes incorrect parameters\n");
        status = connector_callback_abort;
        goto done;
    }
    if (firmware_download_started)
    {
        *download_status = connector_fw_device_error;
        goto done;
    }

    APP_DEBUG("target = %d\n",         download_info->target);
    APP_DEBUG("filename = %s\n",       download_info->filename);
    APP_DEBUG("code size = %d\n",      download_info->code_size);

    strcpy(filename, download_info->filename);
    is_fw_image = naFWIsFirmwareFile(filename);
    ++handle;
    
    total_image_size = 0;
    firmware_download_started = 1;

    *download_status = connector_fw_success;

done:
    return status;
}

static connector_callback_status_t app_firmware_image_data(connector_fw_image_data_t const * const image_data, connector_fw_status_t * data_status)
{
    connector_callback_status_t   status = connector_callback_continue;

    if (image_data == NULL || data_status == NULL)
    {
        APP_DEBUG("app_firmware_image_data ERROR: iDigi passes incorrect parameters\n");
        status = connector_callback_abort;
        goto done;
    }

    if (!firmware_download_started)
    {
        APP_DEBUG("app_firmware_image_data:no firmware download request started\n");
        *data_status = connector_fw_download_denied;
        goto done;
    }

    APP_DEBUG("target = %d\n", image_data->target);
    APP_DEBUG("offset = 0x%04X\n", image_data->offset);
    APP_DEBUG("data = %p\n", image_data->data);
    total_image_size += image_data->length;
    APP_DEBUG("length = %" PRIsize " (total = %" PRIsize ")\n", image_data->length, total_image_size);

    if (is_fw_image)
    {
        int result = naFWStoreImage((char *) image_data->data, image_data->length, filename, handle);
        if (result != 0)
        {
            *data_status = connector_fw_encountered_error;
            APP_DEBUG("firmware_image_data: naFWStoreImage returned %d\n", result);
            goto done;
        }
    }
    *data_status = connector_fw_success;
done:
    return status;
}

static connector_callback_status_t app_firmware_download_complete(connector_fw_download_complete_request_t const * const complete_request, connector_fw_download_complete_response_t * complete_response)
{
    connector_callback_status_t   status = connector_callback_continue;

    if ((complete_request == NULL) || (complete_response == NULL))
    {
        APP_DEBUG("firmware_download_complete Error: iDigi passes incorrect parameters\n");
        status = connector_callback_abort;
        goto done;
    }

    /* use the same version since we are not really updating the code */
    complete_response->version = firmware_list[complete_request->target].version;

    if (!firmware_download_started)
    {
        APP_DEBUG("app_firmware_download_complete:no firmware download request started\n");
        complete_response->status = connector_fw_download_not_complete;
        goto done;
    }

    APP_DEBUG("target    = %d\n",    complete_request->target);
    APP_DEBUG("code size = %u\n",    complete_request->code_size);
    APP_DEBUG("checksum  = 0x%x\n", (unsigned)complete_request->checksum);

    complete_response->status = connector_fw_download_success;

    if (complete_request->code_size != total_image_size)
    {
        APP_DEBUG("app_firmware_download_complete: actual image size (%u) != the code size received (%" PRIsize ")\n",
                      complete_request->code_size, total_image_size);
        complete_response->status = connector_fw_download_not_complete;
    }

    firmware_download_started = 0;

done:
    return status;
}


static connector_callback_status_t app_firmware_download_abort(connector_fw_download_abort_t const * const abort_data)
{
    connector_callback_status_t   status = connector_callback_continue;

    /* Server is aborting firmware update */
    APP_DEBUG("app_firmware_download_abort\n");
    firmware_download_started = 0;

    if (abort_data == NULL)
    {
        APP_DEBUG("app_firmware_download_abort Error: iDigi passes incorrect parameters\n");
        status = connector_callback_abort;
        goto done;
    }

done:
    return status;
}

static connector_callback_status_t app_firmware_reset(connector_fw_config_t const * const reset_data)
{
    connector_callback_status_t   status = connector_callback_continue;

    UNUSED_ARGUMENT(reset_data);
    /* Server requests firmware reboot */
    APP_DEBUG("firmware_reset\n");

    if (is_fw_image)
    {
        naFWControlClose(handle);
    }

    return status;
}

connector_callback_status_t app_firmware_handler(connector_firmware_request_t const request,
                                                  void const * const request_data, size_t const request_length,
                                                  void * response_data, size_t * const response_length)
{
    connector_callback_status_t status = connector_callback_continue;
    connector_fw_config_t * config = (connector_fw_config_t *)request_data;

    UNUSED_ARGUMENT(request_length);

    switch (request)
    {
    case connector_firmware_target_count:
        if (response_data != NULL)
        {
            static uint16_t firmware_list_count = ARRAY_SIZE(firmware_list);

            uint16_t * count = response_data;

            *count = firmware_list_count;
            break;
        }
    case connector_firmware_version:
        if (response_data != NULL)
        {
            uint32_t * version = response_data;

             *version = firmware_list[config->target].version;
            break;
        }
    case connector_firmware_code_size:
        if (response_data != NULL)
        {
            uint32_t * code_size = response_data;

            *code_size = firmware_list[config->target].code_size;
            break;
        }
    case connector_firmware_description:
        if (response_data != NULL)
        {
            char ** description = response_data;

            *description = firmware_list[config->target].description;
            *response_length = strlen(firmware_list[config->target].description);
           break;
        }
    case connector_firmware_name_spec:
        if (response_data != NULL)
        {
            char ** name_spec = response_data;

            *name_spec = firmware_list[config->target].name_spec;
            *response_length = strlen(firmware_list[config->target].name_spec);
            break;
        }
    case connector_firmware_download_request:
        status = app_firmware_download_request(request_data, response_data);
        break;

    case connector_firmware_binary_block:
        status = app_firmware_image_data(request_data, response_data);
        break;

    case connector_firmware_download_complete:
        status = app_firmware_download_complete(request_data, response_data);
        break;

    case connector_firmware_download_abort:
        status =  app_firmware_download_abort(request_data);
        break;

    case connector_firmware_target_reset:
        status =  app_firmware_reset(request_data);
        break;

    }

    return status;
}
