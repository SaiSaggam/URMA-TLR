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
#if !defined(_CUNIT_)
#include <Python.h>
#endif
#include "ddp_firmware.h"
#include "ddp_device.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include "digiutils.h"
#include "tlr_print.h"
#include "create_file_list.h"
#include "unittest_support.h"

STATIC ddp_firmware_update_t ddp_firmware_update_info;

static char * prepare_file_list_group(char * const file_list)
{
    int bytes = -1;
    size_t buf_size = 0;
    char * buffer = NULL;
    FILE * const fd = open_memstream(&buffer, &buf_size);

    if (fd == NULL) goto done;

    bytes = fprintf(fd, "<state><firmware_files><available_images>");
    if (bytes < 0) goto done;

    bytes = ddp_encode_xml_entities(fd, file_list);
    if (bytes < 0) goto done;

    bytes = fprintf(fd, "</available_images></firmware_files></state>");

done:
    if (fd != NULL)
    {
        fclose(fd);
    }

    if (bytes < 0)
    {
        free(buffer);
        buffer = NULL;
    }

    return buffer;
}

static void set_firmware_update_error(ddp_firmware_update_t * const info_ptr)
{
    switch (info_ptr->status)
    {
        case firmware_update_status_none:
            info_ptr->result = NULL;
            break;

        case firmware_update_status_downloading:
            info_ptr->result = "download image failed";
            break;

        case firmware_update_status_verifying:
            info_ptr->result = "verify image failed";
            break;

        case firmware_update_status_applying:
            info_ptr->result = "apply firmware failed";
            break;

        case firmware_update_status_rebooting:
            info_ptr->result = "reboot failed";
            break;

        default:
            info_ptr->result = "update failed";
            break;
    }

    info_ptr->status = firmware_update_status_error;
    tlr_debug("firmware update over ddp, status [%d]\n", info_ptr->status);
}

static bool apply_firmware_update(char const * const user, char const * const path, ddp_firmware_update_t * const info_ptr)
{
    bool success;
    int result;

    info_ptr->progress = 0;
    info_ptr->start_time = time(NULL);
    info_ptr->status = firmware_update_status_applying;

#if !defined(_CUNIT_)
    Py_BEGIN_ALLOW_THREADS
    result = system_command("action_update_firmware -u %s %s", user, path);
    Py_END_ALLOW_THREADS
#else
    result = system_command("action_update_firmware -u %s %s", user, path);
#endif

    success = (result == EXIT_SUCCESS);

    if (success)
    {
        info_ptr->progress = 100;
    }

    return success;
}

static bool update_with_remote_file(char const * const user, char const * const file_path, ddp_firmware_update_t * const info_ptr)
{
    bool success = false;
    int result;
    char * abs_path = NULL;

    info_ptr->status = firmware_update_status_downloading;

#if !defined(_CUNIT_)
    Py_BEGIN_ALLOW_THREADS
    result = system_command("fw_update_with_download.sh -n %s -a 0", file_path);
    Py_END_ALLOW_THREADS
#else
    result = system_command("fw_update_with_download.sh -n %s -a 0", file_path);
#endif

    if (result != EXIT_SUCCESS) goto error;

    result = asprintf(&abs_path, "/tmp/fw/%s", file_path);
    if (result < 0) goto error;

    success = apply_firmware_update(user, abs_path, info_ptr);

error:
    free(abs_path);
    return success;
}

static void init_update_status(ddp_firmware_update_t * const info_ptr)
{
    info_ptr->status = firmware_update_status_none;
    info_ptr->progress = 0;
    info_ptr->result = NULL;
    info_ptr->start_time = 0;
}

static void update_download_progress(ddp_firmware_update_t * const info_ptr)
{
    struct stat stat_buf;
    char const status_file[] = "/tmp/fw/status.log";
    int result = stat(status_file, &stat_buf);
    char * response = NULL;

    if (result != -1)
    {
        char const command[] = "/usr/web/fw_progress.sh %s";
        size_t resp_len = 0;

        result = system_command_with_response(&response, &resp_len, command, status_file);
        if (result <= 0) goto done;
        if (response == NULL) goto done;
        if (!isdigit(*response)) goto done;

        info_ptr->progress = strtoul(response, NULL, 10);
    }

done:
    free(response);
}

static void update_apply_progress(ddp_firmware_update_t * const info_ptr)
{
    static unsigned int const max_apply_time_seconds = 13;
    time_t const current_time = time(NULL);
    unsigned int const delta_sec = (unsigned int)(current_time - info_ptr->start_time);

    info_ptr->progress = (delta_sec < max_apply_time_seconds) ? (delta_sec * 100)/max_apply_time_seconds : 99;
}

static int update_firmware_status(FILE * const fd, ddp_firmware_update_t * const info_ptr)
{
    int result;

    result = fprintf(fd, "<status>%d</status><progress>%u</progress>", info_ptr->status, info_ptr->progress);
    if (result < 0) goto error;

    if (info_ptr->result != NULL)
    {
        result = fprintf(fd, "<result>%s</result>", info_ptr->result);
    }
    else
    {
        result = fprintf(fd, "<result/>");
    }

error:
    return result;
}

char * ddp_get_firmware_files(void)
{
    char * buffer;
    char * const file_list = tlr_get_remote_firmware_filenames_as_xml();

    if (file_list == NULL)
    {
        buffer = strdup("<state><firmware_files/></state>");
    }
    else
    {
        buffer = prepare_file_list_group(file_list);
        free(file_list);
    }

    return buffer;
}

char * ddp_get_firmware_status(void)
{
    int bytes = -1;
    size_t buf_size = 0;
    char * buffer = NULL;
    FILE * const fd = open_memstream(&buffer, &buf_size);
    ddp_firmware_update_t * const info_ptr = &ddp_firmware_update_info;

    if (fd == NULL) goto done;

    bytes = fprintf(fd, "<state><firmware_status>");
    if (bytes < 0) goto error;

    switch (info_ptr->status)
    {
        case firmware_update_status_none:
            init_update_status(info_ptr);
            break;

        case firmware_update_status_downloading:
            update_download_progress(info_ptr);
            break;

        case firmware_update_status_applying:
            update_apply_progress(info_ptr);
            break;

        default:
            break;
    }

    bytes = update_firmware_status(fd, info_ptr);
    if (bytes < 0) goto error;

    bytes = fprintf(fd, "</firmware_status></state>");

error:
    if (fd != NULL)
    {
        fclose(fd);
    }

    if (bytes < 0)
    {
        free(buffer);
        buffer = NULL;
    }

done:
    return buffer;
}

bool ddp_update_firmware(char const * const user, char const * const file_path, bool const remote)
{
    bool success;
    ddp_firmware_update_t * const info_ptr = &ddp_firmware_update_info;
    bool (* update_fn)(char const * const user, char const * const path, ddp_firmware_update_t * const ptr);

    init_update_status(info_ptr);

    update_fn = remote ? update_with_remote_file : apply_firmware_update;
    success = update_fn(user, file_path, info_ptr);

    if (success)
    {
        info_ptr->status = firmware_update_status_complete;
        info_ptr->result = "success";
    }
    else
    {
        set_firmware_update_error(info_ptr);
    }

    return success;
}

bool ddp_reboot(unsigned int reboot_after)
{
    int result;

    ddp_firmware_update_info.status = firmware_update_status_rebooting;
    result = system_command("action_reboot in %u", reboot_after);
    return (result != -1);
}
