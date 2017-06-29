#include "update_message.h"
#include "messages.h"
#include "parsing.h"
#include <config_mgr.h>
#include <tlr_print.h>
#include <stdlib.h>

int get_update_info(int const argc, char ** const argv, void ** const interface_info_result, size_t * const interface_info_size)
{
    int result;
    update_info_st * update_info = NULL;
    ssize_t config_group_id;

    if (argc < 2)
    {
        tlr_debug("%s: too few args to get update info\n", __func__);
        goto error;
    }
    update_info = calloc(1, sizeof *update_info);
    if (update_info == NULL)
        goto error;

    config_group_id = tlr_get_group_id(argv[0]);
    if (config_group_id == -1)
    {
        tlr_debug("%s: failed to get configuration group from name %s\n", __func__, argv[0]);
        goto error;
    }
    update_info->config_group = config_group_id;

    if (!get_instance_from_string(argv[1], &update_info->instance))
    {
        tlr_debug("%s: failed to get instance from %s\n", __func__, argv[1]);
        goto error;
    }

    /* everything is OK if we get here */
    *interface_info_result = update_info;
    *interface_info_size = sizeof *update_info;
    result = 0;
    goto done;

error:
    free(update_info);
    result = -1;
done:
    return result;
}

int get_config_group_type_from_name(char const * const name)
{
    return tlr_get_group_id(name);
}

char const * get_config_group_name_from_id(config_group_t const group_id)
{
    char const * group_name = NULL;
    config_mgr_group_table_t const * const table_ptr = tlr_get_table(config_mgr_table_type_config);

    if (group_id < table_ptr->count)
    {
        config_mgr_group_t const * const group_ptr = &table_ptr->groups[group_id];
        group_name = group_ptr->name;
    }

    return group_name;
}

