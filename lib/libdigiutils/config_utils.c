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

#include "config_utils.h"
#include "config_mgr.h"
#include "tlr_print.h"
#include <security.h>
#include <tlr_common.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void config_free(config_st * config)
{
    if (config != NULL)
    {
        if (config->elements != NULL)
        {
            size_t i;

            for (i = 0; i < config->count; i++)
            {
                free(config->elements[i].value);
            }
            free(config->elements);
        }
        free(config);
    }
}

config_st * config_alloc(config_group_t group_id)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    config_mgr_group_t const * const group = &table->groups[group_id];
    size_t const count = group->elements.count;
    config_st * config = NULL;
    size_t i;

    config = calloc(1, sizeof *config);
    if (config == NULL)
    {
        tlr_debug("calloc(%d, %zd) failed\n", 1, sizeof( *config));
        goto error;
    }
    config->group_id = group_id;
    config->elements = calloc(count, sizeof *config->elements);
    if (config->elements == NULL)
    {
        tlr_debug("calloc(%zd, %zd) failed\n", count, sizeof( *config->elements));
        goto error;
    }
    config->count = count;

    for (i = 0; i < count; i++)
    {
        size_t element_size = tlr_get_element_size(&group->elements.element[i]);

        if (element_size == 0)
        {
            tlr_debug("element size zero on #%zd name: %s\n", i, group->elements.element[i].name);
            goto error;
        }
        config->elements[i].value = calloc(element_size, sizeof(char));
        if (config->elements[i].value == NULL)
        {
            tlr_debug("calloc(%zd, %zd) failed\n", element_size, sizeof(char));
            goto error;
        }
        config->elements[i].size = element_size;
    }

    goto done;

error:
    if (config != NULL)
    {
        config_free(config);
        config = NULL;
    }

done:
    return config;
}

static int config_read(config_st * const config, size_t const instance)
{
    tlr_handle_t tlr_handle = NULL;
    size_t i;
    int result;

    tlr_handle = tlr_config_open(config->group_id, instance, config_mgr_access_level_super);
    if (tlr_handle == NULL)
    {
        tlr_debug("tlr_config_open failed\n");
        result = -1;
        goto done;
    }

    for (i = 0; i < config->count; i++)
    {
        config_status_t config_status;

        config_status = tlr_config_read(tlr_handle, i, config->elements[i].value, config->elements[i].size);
        if (config_status != config_status_success)
        {
            tlr_debug("tlr_config_read(%p, %zd) failed\n", tlr_handle, i);
            result = -1;
            goto done;
        }
    }

    result = 0;

done:
    if (tlr_handle != NULL)
        tlr_config_close(tlr_handle);

    return result;
}

static config_st * config_get(config_group_t config_group, size_t const instance)
{
    config_st * config = config_alloc(config_group);

    if (config != NULL)
    {
        if (config_read(config, instance) != 0)
        {
            config_free(config);
            config = NULL;
        }
    }

    return config;
}

int config_init(config_st * * const config, size_t const instance, config_group_t config_group)
{
    int result;

    *config = config_get(config_group, instance);
    if (*config == NULL)
    {
        tlr_debug("could not get config for group %d\n", config_group);
        result = -1;
        goto done;
    }

    result = 0;

done:
    return result;
}

size_t config_get_element_count(config_st * const config)
{
    return config->count;
}

char const * config_get_element_value(config_st * const config, size_t const index)
{
    return config->elements[index].value;
}

void config_set_element_value(config_st * const config, size_t const index, char const * const value)
{
    strlcpy(config->elements[index].value, value, config->elements[index].size);
}

int config_update_and_compare_with_previous(config_st * * const config,
                                            size_t const instance,
                                            config_compare_fn compare_fn,
                                            bool * const config_has_changed)
{
    int result;
    config_st * new_config;

    new_config = config_get((*config)->group_id, instance);
    if (new_config == NULL)
    {
        tlr_debug("could not get new config for group %d\n", (*config)->group_id);
        result = -1;
        goto done;
    }

    *config_has_changed = compare_fn(instance, *config, new_config);

    /* replace the previous configuration with the latest */
    config_free(*config);
    *config = new_config;
    new_config = NULL;

    result = 0;

done:

    config_free(new_config);

    return result;
}


