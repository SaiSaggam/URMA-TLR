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
*****************************************************************************/
#include "tlr_backup.h"
#include "tlr_print.h"
#include "digiutils.h"
#include "config_mgr.h"
#include "config_status.h"
#include "tlr_common.h"

#include "unittest_support.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <stdarg.h>
#include <termios.h>
#include <sys/stat.h>

static size_t malloc_backup_data(size_t const group_id, size_t const instance, char ** data)
{
    config_mgr_group_t const * const group = tlr_get_group_ptr(group_id, instance,
                                                             config_mgr_set_group);
    size_t total_length = 0;
    size_t i;

    if (group == NULL || data == NULL)
    {
        goto done;
    }

    for (i=0; i < group->elements.count; i++)
    {
        total_length += tlr_get_config_value_max_length(config_mgr_table_type_config,
                                                        group_id, i);
    }

    *data = malloc(total_length);

    if (*data == NULL)
    {
        total_length = 0;
        goto done;
    }

done:

    return total_length;

}

static void free_backup_data(char * data)
{
    free(data);
}

int config_restore_default(size_t const group_id, size_t const instance)
{
    int result = -1;

    config_mgr_group_t const * const group = tlr_get_group_ptr(group_id, instance,
                                                             config_mgr_set_group);
    size_t i;

    if (group == NULL)
        goto done;
    
    for (i=0; i < group->elements.count; i++)
    {
        config_mgr_element_t const * const element = &group->elements.element[i];
        if (element->access != config_mgr_access_no_access)
        {
            char const * const default_value = (element->default_value != NULL) ? element->default_value : "";

            config_status_t const status = tlr_set_element_value(group_id, instance, i, default_value, config_mgr_access_level_read_write);

            if (status != config_status_success)
            {
                tlr_debug("failed to set element %d with default value\n", (int)i);
                goto done;
            }
        }
    }

    result = 0;

done:
    return result;
}

int config_restore_backup(size_t const group_id, size_t const instance)
{
    int result = -1;

    config_mgr_group_t const * const group = tlr_get_group_ptr(group_id, instance,
                                                             config_mgr_set_group);
     char * data = NULL;
     size_t const total_length = malloc_backup_data(group_id, instance, &data);

     size_t i;
     char * ptr;
     config_status_t status;

     if (data == NULL || group == NULL)
     {
         goto done;
     }

     status = tlr_get_last_config(group_id, instance, data, total_length);
     if (status != config_status_success)
     {
         tlr_debug("failed to retrieve previous configuration %d\n", (int)instance);
         goto done;
     }

     ptr = data;

     for (i=0; i < group->elements.count; i++)
     {
         size_t length = tlr_get_config_value_max_length(config_mgr_table_type_config,
                                                         group_id, i);

         status = tlr_set_element_value(group_id, instance, i, ptr, config_mgr_access_level_read_write);

         if (status != config_status_success)
         {
             tlr_debug("failed to restore configuration %d\n", (int)i);
             goto done;
         }

         ptr += length;
     }


    result = 0;

done:
    free_backup_data(data);
    return result;
}

int config_backup_current(size_t const group_id, size_t const instance)
{
    config_mgr_group_t const * const group = tlr_get_group_ptr(group_id, instance,
                                                            config_mgr_set_group);

    int result = -1;
    char * data = NULL;
    size_t const total_length = malloc_backup_data(group_id, instance, &data);

    size_t i;
    char * ptr;
    config_status_t status;

    if (data == NULL || group == NULL)
    {
        goto done;
    }

    ptr = data;

    for (i=0; i < group->elements.count; i++)
    {
        size_t length = tlr_get_config_value_max_length(config_mgr_table_type_config,
                                                        group_id, i);

        status = tlr_get_element_value(group_id, instance, i, ptr, length,
                                                        config_mgr_element_type_string,
                                                        config_mgr_access_level_read_write);

        if (status != config_status_success)
        {
            tlr_debug("failed to get element %d\n", (int)i);
            goto done;
        }

        ptr += length;
    }

    status = tlr_set_last_config(group_id, instance, data, total_length);
    if (status != config_status_success)
    {
        tlr_debug("failed to set last config\n");
        goto done;
    }

    result = 0;

done:
    free_backup_data(data);
    return result;
}

