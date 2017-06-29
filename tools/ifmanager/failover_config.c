#include "failover_config.h"
#include "failover_debug.h"
#include "failover.h"
#include <strlcpy.h>
#include <string.h>

static int get_failover_element(size_t const instance, size_t const element_id, void * const element_value)
{
    int result;
    config_status_t status;

    size_t const max_length = tlr_get_config_value_max_length(config_mgr_table_type_config, config_failover, element_id);
    char value[max_length+1];

    status = tlr_get_element_value(config_failover, instance, element_id,
                                       value, sizeof value,
                                       config_mgr_element_type_string, config_mgr_access_level_read_only);
    if (status != config_status_success) goto error;

    status = tlr_config_string_to_value(config_failover, element_id, value, element_value);
    if (status != config_status_success) goto error;

error:
    result = (status == config_status_success) ? 0 : -1;
    return result;
}


static failover_config_status_t get_probe_host(size_t const instance, char * const host, size_t const max_host_length)
{
    failover_config_status_t status;
    size_t const max_length = tlr_get_config_value_max_length(config_mgr_table_type_config,
                                                                config_failover,
                                                                config_failover_probe_host);
    char element_value[max_length];

    if (get_failover_element(instance, config_failover_probe_host, element_value) != 0)
    {
        status = failover_config_status_error;
        goto done;
    }

    if (strcmp(host, element_value) != 0)
    {
        strlcpy(host, element_value, max_host_length);
        status = failover_config_status_changed;
        goto done;
    }
    status = failover_config_status_unchanged;

done:
    return status;
}

static failover_config_status_t get_interface(size_t const instance, size_t const element_id, ifm_if_info_t * const interface_info)
{
    failover_config_status_t status;
    unsigned int element_value;
    ifm_if_info_t element_interface_info;

    if (get_failover_element(instance, element_id, &element_value) != 0)
    {
        status = failover_config_status_error;
        goto done;
    }

    if (!get_failover_interface(element_value, &element_interface_info))
    {
        status = failover_config_status_error;
        goto done;
    }

    if (!is_interface_info(interface_info, &element_interface_info))
    {
        //*interface_info = element_interface_info;
        memcpy(interface_info, &element_interface_info, sizeof *interface_info);
        status = failover_config_status_changed;
        goto done;
    }
    status = failover_config_status_unchanged;

done:
    return status;
}

static failover_config_status_t get_state(size_t const instance, bool * const state)
{
    failover_config_status_t status;
    bool element_value;

    if (get_failover_element(instance, config_failover_state, &element_value) != 0)
    {
        status = failover_config_status_error;
        goto done;
    }

    if (*state != element_value)
    {
        *state = element_value;
        status = failover_config_status_changed;
        goto done;
    }
    status = failover_config_status_unchanged;

done:
    return status;
}

static failover_config_status_t get_use_type(size_t const instance, unsigned int * const use_type)
{
    failover_config_status_t status;
    unsigned int element_value;

    if (get_failover_element(instance, config_failover_use, &element_value) != 0)
    {
        status = failover_config_status_error;
        goto done;
    }

    if (*use_type != element_value)
    {
        *use_type = element_value;
        status = failover_config_status_changed;
        goto done;
    }
    status = failover_config_status_unchanged;

done:
    return status;
}

static failover_config_status_t get_interval(size_t const instance, config_failover_t const config_failover_id, size_t * const interval)
{
    failover_config_status_t status;
    uint32_t element_value;

    if (get_failover_element(instance, config_failover_id, &element_value) != 0)
    {
        status = failover_config_status_error;
        goto done;
    }

    if (*interval != (uint32_t)element_value)
    {
        *interval = element_value;
        status = failover_config_status_changed;
        goto done;
    }
    status = failover_config_status_unchanged;

done:
    return status;
}


failover_config_status_t failover_get_config(size_t const instance, failover_config_t * const info)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    config_mgr_group_t const * const group = &table->groups[config_failover];
    unsigned int status_mask = 0;
    failover_config_status_t status;

    if (instance == 0 || instance > group->instances)
    {
        failover_debug("invalid instance %zu\n", instance);
        status_mask |= CONFIG_STATUS_MASK(failover_config_status_error);
        goto done;
    }

    status = get_state(instance, &info->enabled);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_interface(instance, config_failover_from, &info->from);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_interface(instance, config_failover_to, &info->to);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_use_type(instance, &info->use_type);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_interval(instance, config_failover_probe_size, &info->probe_size);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_interval(instance, config_failover_timeout, &info->timeout_after);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_interval(instance, config_failover_probe_interval, &info->probe_interval);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_interval(instance, config_failover_probe_timeout, &info->probe_timeout);
    status_mask |= CONFIG_STATUS_MASK(status);

    status = get_interval(instance, config_failover_switch_primary_up, &info->switch_primary_up);
    status_mask |= CONFIG_STATUS_MASK(status);

    //info->timeout_after = seconds_to_milliseconds(element_value);

    status = get_probe_host(instance, info->probe_host, sizeof info->probe_host);
    status_mask |= CONFIG_STATUS_MASK(status);


done:
    if (IS_CONFIG_STATUS_ERROR(status_mask))
        status = failover_config_status_error;
    else if (IS_CONFIG_STATUS_CHANGED(status_mask))
        status = failover_config_status_changed;
    else
        status = failover_config_status_unchanged;

    //failover_debug("failover %zu configuration is %d\n", instance, status);
    return status;
}


