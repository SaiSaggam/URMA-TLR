/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"

#ifdef _CUNIT_
#define TEMP_CONFIG_DIR   "config/"
#else
#define TEMP_CONFIG_DIR   "/tmp/config/"
#endif

static bool check_and_create_dir(char const * const path)
{
    struct stat file_info;
    bool success = (stat(path, &file_info) == 0);

    if (!success)
    {
        success = (mkdir(path, 0666) == 0);
    }

    return success;
}

static config_status_t get_group_name(size_t const group_id, size_t const instance, char const ** const group_name)
{
    config_status_t status;
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);

    if (group_id < table_ptr->count)
    {
        config_mgr_group_t const * const group_ptr = &table_ptr->groups[group_id];

        if (instance <= group_ptr->instances)
        {
            *group_name = group_ptr->name;
            status = config_status_success;
        }
        else
        {
            status = config_status_instance_not_found;
        }
    }
    else
    {
        status = config_status_group_not_found;
    }

    return status;
}

static FILE * get_ram_config_file(char const * const group_name, size_t const instance, char const * const mode)
{
    FILE * fd = NULL;

    if (!check_and_create_dir(TEMP_CONFIG_DIR)) goto error;  // @TODO : Move this to init script

    {
        char * dir_path = NULL;

        if (asprintf(&dir_path, "%s/%s", TEMP_CONFIG_DIR, group_name) > sizeof TEMP_CONFIG_DIR)
        {
            if (check_and_create_dir(dir_path))
            {
                char * file_path = NULL;

                if (asprintf(&file_path, "%s/%03zu", dir_path, instance) > strlen(dir_path))
                {
                    fd = fopen(file_path, mode);
                }

                free(file_path);
            }
        }
        free(dir_path);
    }

error:
    return fd;
}

config_status_t tlr_get_last_config(size_t const group_id, size_t const instance, void * const ptr, size_t const size)
{
    char const * group_name = NULL;
    config_status_t status = get_group_name(group_id, instance, &group_name);

    if (status == config_status_success)
    {
        FILE * fd = get_ram_config_file(group_name, instance, "r");

        if (fd == NULL)
        {
            status = config_status_file_not_found;
            goto error;
        }
        if (fread(ptr, size, 1, fd) != 1)
        {
            status = config_status_file_read_error;
        }
        fclose(fd);
    }

error:
    return status;
}

config_status_t tlr_set_last_config(size_t const group_id, size_t const instance, void const * const ptr, size_t const size)
{
    char const * group_name;
    config_status_t status = get_group_name(group_id, instance, &group_name);

    if (status == config_status_success)
    {
        FILE * fd = get_ram_config_file(group_name, instance, "w");

        if (fd == NULL)
        {
            status = config_status_file_not_found;
            goto error;
        }

        if (fwrite(ptr, size, 1, fd) != 1)
        {
            status = config_status_file_write_error;
        }

        fclose(fd);
    }

error:
    return status;
}
