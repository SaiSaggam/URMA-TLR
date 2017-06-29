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

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"
#include "strlcpy.h"
#include "tlr_print.h"
#include "uci_interface.h"
#include "tlr_common.h"

typedef enum {
    element_access_read,
    element_access_alloc_read,
    element_access_write
} element_access_t;

typedef union {
    struct {
        void * data;
        size_t size;
    } data_value;
    void ** alloc_value;
} element_value_t;

static ssize_t get_group_id(char const * const group_name, config_mgr_table_type_t const table)
{
    ssize_t group_id = -1;
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(table);
    size_t id;

    for (id = 0; id < table_ptr->count; id++)
    {
        if (strcmp(table_ptr->groups[id].name, group_name) == 0)
        {
            group_id = id;
            break;
        }
    }

    return group_id;
}

ssize_t tlr_get_group_id(char const * const group_name)
{
    return get_group_id(group_name, config_mgr_table_type_config);
}

ssize_t tlr_get_show_group_id(char const * const group_name)
{
    return get_group_id(group_name, config_mgr_table_type_show);
}

ssize_t tlr_get_element_id(size_t const group_id, char const * const element_name)
{
    ssize_t element_id = -1;
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);

    if (group_id < table_ptr->count)
    {
        config_mgr_group_t const * const group_ptr = &table_ptr->groups[group_id];
        config_mgr_element_t const * element_ptr = group_ptr->elements.element;
        size_t id;

        for (id = 0; id < group_ptr->elements.count; id++)
        {
            if (strcmp(element_ptr->name, element_name) == 0)
            {
                element_id = id;
                break;
            }

            element_ptr++;
        }
    }

    return element_id;
}

static bool is_element_valid_type(size_t const group_id, size_t const instance, size_t const element_id, config_mgr_element_type_t type)
{
    bool is_valid = false;
    config_mgr_group_t const * const group = tlr_get_group_ptr(group_id, instance, config_mgr_set_group);

    if (group == NULL)
    {
        goto done;
    }

    if ((element_id < group->elements.count) && (type == group->elements.element[element_id].type))
    {
        is_valid = true;
    }

done:
    return is_valid;
}

static config_status_t config_element_read_enum_multi(tlr_handle_t handle, size_t const element_id, config_mgr_enum_multi_value_t * const value)
{
    config_status_t status = config_status_invalid_parameter;

    if (value!= NULL)
    {
        config_mgr_enum_multi_value_t * enum_multi = NULL;
        size_t i;

        status = tlr_config_read_alloc_value(handle, element_id, (void **)&enum_multi, config_mgr_element_type_enum_multi);
        if (status != config_status_success)
        {
            goto done;
        }


        for (i=0; (i < enum_multi->count) && (i < value->count); i++)
        {
            value->value[i] = enum_multi->value[i];
        }
        value->count = i;

        free(enum_multi);
    }

done:
    return status;
}

static config_status_t config_element_read(tlr_handle_t handle,
                                            size_t const group_id, size_t const instance, size_t const element_id,
                                            void * const value, size_t const bytes, config_mgr_element_type_t const type)
{
    config_status_t status = config_status_invalid_parameter;

    if (value == NULL) goto done;

    switch (type)
    {
        case config_mgr_element_type_string:
            if (bytes == 0)
            {
                goto done;
            }

            status = tlr_config_read(handle, element_id, value, bytes);
            break;

        case config_mgr_element_type_enum_multi:
            if (!is_element_valid_type( group_id, instance, element_id, type))
            {
                goto done;
            }

            status = config_element_read_enum_multi(handle, element_id, value);
            break;
        case config_mgr_element_type_password:
        case config_mgr_element_type_uint32:
        case config_mgr_element_type_int32:
        case config_mgr_element_type_enum:
        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
        case config_mgr_element_type_ipaddr:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_counter32:
        case config_mgr_element_type_counter64:
        case config_mgr_element_type_index:
        case config_mgr_element_type_interface:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_none:
            if (!is_element_valid_type( group_id, instance, element_id, type))
            {
                goto done;
            }

            status = tlr_config_read_native(handle, element_id, value);
            break;
    }

done:
    return status;
}

static config_status_t config_element_write(tlr_handle_t handle,
                                            size_t const group_id,size_t const instance, size_t const element_id,
                                            void * const value, config_mgr_element_type_t const type)
{
    config_status_t status = config_status_invalid_parameter;

    if (value == NULL)
    {
        goto done;
    }

    switch (type)
    {
        case config_mgr_element_type_string:
            status = tlr_config_write(handle, element_id, value);
            break;

        case config_mgr_element_type_enum_multi:
            /* TODO: current only take string */
        case config_mgr_element_type_password:
        case config_mgr_element_type_uint32:
        case config_mgr_element_type_int32:
        case config_mgr_element_type_enum:
        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
        case config_mgr_element_type_ipaddr:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_counter32:
        case config_mgr_element_type_counter64:
        case config_mgr_element_type_index:
        case config_mgr_element_type_interface:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_none:
            if (!is_element_valid_type( group_id, instance, element_id, type))
            {
                goto done;
            }
            status = tlr_config_write_native(handle, element_id, value);
            break;
    }

done:
    return status;
}


static config_status_t tlr_access_element_value(size_t const group_id,
                                                size_t const instance,
                                                size_t const element_id,
                                                element_value_t * const value,
                                                config_mgr_element_type_t const type,
                                                config_mgr_access_level_t const access,
                                                element_access_t element_access)
{
    config_status_t status = config_status_invalid_parameter;
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);

    if (group_id < table_ptr->count)
    {
        tlr_handle_t handle = tlr_config_open(group_id, instance, access);

        if (handle != NULL)
        {
            config_mgr_group_t const * const group_ptr = &table_ptr->groups[group_id];

            if (element_id < group_ptr->elements.count)
            {
                switch (element_access)
                {
                    case element_access_write:
                        status = config_element_write(handle, group_id, instance, element_id, value->data_value.data, type);
                        break;
                    case element_access_read:
                        status = config_element_read(handle, group_id, instance, element_id, value->data_value.data, value->data_value.size, type);
                        break;
                    case element_access_alloc_read:
                        status = tlr_config_read_alloc_value(handle, element_id, value->alloc_value, type);
                        break;

                }
            }

            tlr_config_close(handle);
       }
    }

    return status;
}

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    element_value_t element_value = { .data_value.data = value, .data_value.size = bytes};

    config_status_t const status = tlr_access_element_value(group_id, instance, element_id, &element_value, type, access, element_access_read);

    return status;
}

config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void ** value,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    element_value_t element_value = { .alloc_value = value };

    config_status_t const status = tlr_access_element_value(group_id, instance, element_id, &element_value, type, access, element_access_alloc_read);

    return status;

}

config_status_t tlr_set_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      char const * const value,
                                      config_mgr_access_level_t const access)
{
    element_value_t element_value = { .data_value.data = (void *)value};

    config_status_t const status = tlr_access_element_value(group_id, instance, element_id, &element_value, config_mgr_element_type_string, access, element_access_write);

    return status;
}

size_t enum_max_name_length(config_mgr_element_enum_limit_t const * const enum_limit)
{
	size_t max_name_length = 0;
	size_t i;

	for (i = 0; i < enum_limit->count; i++)
	{
		size_t name_length = strlen(enum_limit->value[i].name);
		if (name_length > max_name_length)
			max_name_length = name_length;
	}

	return max_name_length;
}

size_t enum_multi_max_name_length(config_mgr_element_enum_limit_t const * const enum_limit)
{
	size_t max_name_length = 0;
	size_t i;

	for (i = 0; i < enum_limit->count; i++)
	{
		size_t name_length = strlen(enum_limit->value[i].name);

        if (i > 0)
            max_name_length++;  /* account for a comma between items */
        max_name_length += name_length;
	}

	return max_name_length;
}

static size_t element_max_name_length(config_mgr_element_type_t const type, void * const value_limit)
{
    size_t size = 1024; // some high value if the size cannot be determined

    switch (type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_mac_addr:
        {
            config_mgr_element_string_limit_t const * const string_limit = value_limit;
            if (string_limit != NULL)
                size = string_limit->max_length_in_bytes + 1;
            break;
        }
        case config_mgr_element_type_ipaddr:
            size = sizeof("255.255.255.255");
            break;
        case config_mgr_element_type_fqdn:
            size = 256; // see validate_fqdn()
            break;
        case config_mgr_element_type_enum:
        {
            config_mgr_element_enum_limit_t const * const enum_limit = value_limit;
            if (enum_limit != NULL)
                size = enum_max_name_length(enum_limit) + 1;
            break;
        }
        case config_mgr_element_type_enum_multi:
        {
            config_mgr_element_enum_limit_t const * const enum_limit = value_limit;

            if (enum_limit != NULL)
                size = enum_multi_max_name_length(enum_limit) + 1;
            break;
        }
        case config_mgr_element_type_counter64:
            size = UINT64_STRING_LENGTH;
            break;
        case config_mgr_element_type_counter32:
        case config_mgr_element_type_uint32:
        case config_mgr_element_type_index:
        case config_mgr_element_type_int32:
            size = INTEGER_STRING_LENGTH;
            break;
        case config_mgr_element_type_on_off:
            size = sizeof("off");
            break;
        case config_mgr_element_type_boolean:
            size = sizeof("false");
            break;
        case config_mgr_element_type_interface:
        case config_mgr_element_type_none:
        default:
            size = 0;
            break;
    }

    return size;
}

config_mgr_element_enum_limit_t const * tlr_get_element_enum_limit(config_mgr_table_type_t const type, size_t const group_id, size_t const element_id)
{
    config_mgr_group_table_t const * const table_ptr = tlr_get_table(type);
    config_mgr_group_t const * const group_ptr = &table_ptr->groups[group_id];
    config_mgr_element_t const * const element_ptr = &group_ptr->elements.element[element_id];
    config_mgr_element_enum_limit_t const * const enum_limit_ptr = (config_mgr_element_enum_limit_t *)element_ptr->value_limit;

    return enum_limit_ptr;
}

size_t tlr_get_element_size(config_mgr_element_t const * const element)
{
    return element_max_name_length(element->type, element->value_limit);
}

// Given a element find the instance
config_status_t tlr_get_group_instance(size_t const group_id,
                                       size_t const element_id,
                                       char const * const value,
                                       size_t const value_size,
                                       size_t * const instance)
{
    config_status_t status = config_status_no_entry_exist;
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);

    if (group_id < table_ptr->count)
    {
        config_mgr_group_t * const group_ptr = &table_ptr->groups[group_id];
        size_t i;


        for (i=1; i <= group_ptr->instances; i++)
        {
            char uci_value[value_size + 8];
            config_status_t read_status = tlr_get_element_value(group_id, i, element_id,
                                     (char * const)uci_value, sizeof uci_value,
                                     config_mgr_element_type_string, config_mgr_access_level_read_only);

            //tlr_debug("checking [%s] against [%s] %d", value, uci_value, read_status);

            if ((read_status == config_status_success) &&
                (strcmp(uci_value, value) == 0))
             {
                 /* found it */
                //tlr_debug("found it\n");
                 status = config_status_success;
                 *instance = i;
                 break;
             }
        }
    }

    return status;
}

size_t tlr_get_config_value_max_length(config_mgr_table_type_t const type, size_t const config_id, size_t const element_id)
{
    size_t max_length = 0;

    void const * const table_ptr =  tlr_get_table(type);

    switch (type)
    {
        case config_mgr_table_type_show:
        case config_mgr_table_type_config:
            {
                config_mgr_group_table_t const * const table = table_ptr;
                if (config_id < table->count)
                {
                    config_mgr_group_t const * const group_ptr = &table->groups[config_id];

                    if (element_id < group_ptr->elements.count)
                    {
                        config_mgr_element_t const * const element = &group_ptr->elements.element[element_id];
                        max_length = element_max_name_length(element->type, element->value_limit);
                    }
                }
            }
            break;

        case config_mgr_table_type_action:
            {
                config_mgr_action_table_t const * const table = table_ptr;

                if (config_id < table->count)
                {
                    config_mgr_action_t const * const action_ptr = &table->actions[config_id];

                    if (element_id < action_ptr->elements.count)
                    {
                        config_mgr_action_element_t const * const element_ptr = &action_ptr->elements.element[element_id];
                        switch (element_ptr->type)
                        {
                            case config_mgr_action_group:
                                max_length = tlr_get_config_value_max_length(config_mgr_table_type_config, config_id, element_id);
                                break;
                            case config_mgr_action_element:
                                {
                                    config_mgr_element_t const * const action_element = element_ptr->element;
                                    max_length = element_max_name_length(action_element->type, action_element->value_limit);
                                }
                                break;
                            case config_mgr_action_parameter:
                                {
                                    config_mgr_action_parameter_t const * const action_parameter = element_ptr->element;
                                    max_length = element_max_name_length(action_parameter->type, action_parameter->value_limit);
                                }
                                break;
                        }
                    }
                }
            }
            break;
    }

    return max_length;
}

/*
 * group_id: config_state_group_t or config_group_t in config_def.h
 * type: one of the config_mgr_table_type_t in config_mgr.h
 */
size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type)
{
    size_t ret = 0;
    config_mgr_group_table_t const * table;
    config_mgr_group_t const * group;
    size_t count;

    if (type != config_mgr_table_type_config && type != config_mgr_table_type_show)
        goto error;

    table = tlr_get_table(type);
    count = table->count;
    if (group_id >= count)
        goto error;

    group = &table->groups[group_id];
    ret = group->instances;

error:
    return ret;
}

void tlr_log_set_event(size_t const group_id,
                       size_t const instance,
                       size_t const element_id,
                       char const * const value,
                       char const * const user,
                       bool const success)
{
    char const hidden[] = "****";
    config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(group_id, instance, config_mgr_set_group);

    if (group_ptr == NULL) goto error;
    if (element_id >= group_ptr->elements.count) goto error;

    {
        config_mgr_element_t const * const element_ptr = &group_ptr->elements.element[element_id];
        char const * const value_to_print = (element_ptr->access != config_mgr_access_writeonly) ? value : hidden;

        tlr_event_log(tlr_log_info, user, "set %s%zd %s to %s %s", group_ptr->name, instance, element_ptr->name, value_to_print, success ? "success" : "failed");
    }

 error:
    return;
}

bool tlr_apply_group(size_t const group_id, size_t const instance, char const * const user, config_mgr_access_level_t const access)
{
    bool success = false;
    config_status_t status;
    tlr_handle_t const handle = tlr_apply_open(group_id, instance, user, access);
    config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(group_id, instance, config_mgr_set_group);

    if (handle == NULL)
    {
        tlr_debug("apply %s[%zd] failed\n", group_ptr->name, instance);
        goto done;
    }

    do
    {
#define APPLY_RESPONSE_LEN 128
        char response[APPLY_RESPONSE_LEN] = {0};

        status = tlr_apply_read(handle, response, sizeof response, WAIT_FOREVER);

        switch (status)
        {
            case config_status_success:
                tlr_debug("%s\n", response);
                break;

            case config_status_read_complete:
                success = true;
                tlr_debug("%s\n", response);
                break;

            default:
                tlr_debug("apply %s[%zd] failed\n", group_ptr->name, instance);
                break;
        }

    } while (status == config_status_success);

done:
    if (handle != NULL)
    {
        tlr_apply_close(handle);
    }

    return success;
}

unsigned int tlr_get_group_loglevel(size_t const group_id)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    config_mgr_group_t const * const group = &table->groups[group_id];
    unsigned int loglevel = (unsigned int)tlr_log_nolog;

    /*
     * config debug 'debug1'
     *   option loglevel 'debug'
     */
    char * debug_level_string = uci_get_option(group->filename, "debug", 1, "loglevel");

    if (debug_level_string != NULL)
    {
        unsigned int debug_level;

        if (tlr_config_string_to_value(config_system, config_system_loglevel,
                debug_level_string, &debug_level) == config_status_success)
        {
            loglevel = debug_level;
        }

        free(debug_level_string);

    }

    return loglevel;
}

bool tlr_is_json_supported(size_t const group_id)
{
    bool supported = false;

    switch (group_id)
    {
        case config_state_ipsec:
        case config_state_eth:
        case config_state_system:
        case config_state_cellular:
        case config_state_lan:
        case config_state_failover:
        case config_state_cloud:
            supported = true;
            break;

        case config_state_ipstats:
        case config_state_serial:
        case config_state_route:
        case config_state_log:
        case config_state_dsl:
        case config_state_config:
        case config_state_firewall:
        case config_state_manufacture:
            break;
    }

    return supported;
}
