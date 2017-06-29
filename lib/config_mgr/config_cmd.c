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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/ioctl.h>

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"
#include "uci_interface.h"
#include "password.h"
#include "validate.h"
#include "strlcpy.h"
#include "odm_specific.h"
#include "password_gen.h"
#include "ip_helper.h"
#include "serial_number.h"
#include "tlr_common.h"

typedef struct
{
	void *uci_config_handle;
	config_mgr_group_t const * group_ptr;
	size_t instance;
	config_mgr_access_level_t access;
} tlr_config_data_t;

/* TODO: set LR54 platform */
static platform_type_t platform_module = platform_COUNT;
static char const default_platform_name[] = TLR_PLATFORM;
static char const * platform_name = default_platform_name;

static void tlr_init_platform(void);

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    config_mgr_group_t const * group_ptr = NULL;

    /* TODO: should call here or when boot option is given */
    tlr_init_platform();

    if (group_id < config_mgr_group_table[table_index].count)
    {
        config_mgr_group_t const * const temp_ptr = &config_mgr_group_table[table_index].groups[group_id];

        if (temp_ptr->access == config_mgr_access_no_access)
        {
            /* XXX - This function is getting called to determine if features are present or not
             * (e.g. Wi-Fi, DSL). Perhaps we should not be logging an error here.
             */
            /* XXX - CNisbet - I don't think we should be altering the 'access' field as a means of
             * disabling hardware features. We should have a separate field that gets checked.
             */
            tlr_config_error("Group %s not accessible\n", temp_ptr->name);
            goto error;
        }

        if (instance > temp_ptr->instances)
        {
            tlr_config_error("Group %s instance [%zd] out of range %zd\n", temp_ptr->name, instance, temp_ptr->instances);
            goto error;
        }
        group_ptr = temp_ptr;
    }
    else
    {
        tlr_config_error("Group index [%zd] is out of range for %d\n", group_id, table_index);
    }

error:
    return group_ptr;
}

size_t tlr_get_num_groups(config_mgr_group_type_t const table_index)
{
    return config_mgr_group_table[table_index].count;
}

static config_mgr_element_t const * get_element_ptr(config_mgr_group_t const * const group_ptr, size_t const element_index)
{
    config_mgr_element_t const * element_ptr = NULL;

    if (element_index < group_ptr->elements.count)
    {
        config_mgr_element_t const * const temp_ptr = &group_ptr->elements.element[element_index];

        if (temp_ptr->access == config_mgr_access_no_access)
        {
            tlr_config_error("Element %s in group %s not accessible\n", temp_ptr->name, group_ptr->name);
            goto error;
        }

        element_ptr = temp_ptr;
    }
    else
    {
        tlr_config_error("Element index [%zd] is out of range for %s\n", element_index, group_ptr->name);
    }

error:
    return element_ptr;
}

static config_mgr_element_enum_t const on_off_enum[] = {
    {"off", NULL, 0, 0, false, config_mgr_access_readwrite},
    {"on", NULL, 1, 1, false, config_mgr_access_readwrite}
};

static config_mgr_element_enum_limit_t const on_off_limit = {
        (config_mgr_element_enum_t *)on_off_enum,
        sizeof on_off_enum
};

static config_mgr_element_enum_t const boolean_enum[] = {
    {"false", NULL, 0, 0, false, config_mgr_access_readwrite},
    {"true", NULL, 1, 1, false, config_mgr_access_readwrite}
};

static config_mgr_element_enum_limit_t const boolean_limit = {
        (config_mgr_element_enum_t *)boolean_enum,
        sizeof boolean_enum
};

static config_status_t enum_string_to_value(char const * const input, config_mgr_element_enum_limit_t const * const match, unsigned int * const result, bool const need_index)
{
    config_status_t status = config_status_invalid_value;
    size_t i;

    for (i = 0; i < match->count; i++)
    {
        config_mgr_element_enum_t const * const enum_value = &match->value[i];

        if (strcmp(enum_value->name, input) == 0)
        {
            *result = need_index ? i : enum_value->value;
            status = config_status_success;
            break;
        }
    }

    return status;
}

static config_status_t enum_value_to_string(unsigned int value, config_mgr_element_enum_limit_t const * const match, char * const enum_str)
{
    config_status_t const status = (value < match->count) ? config_status_success : config_status_invalid_format;

    if (status == config_status_success)
    {
        strcpy(enum_str, match->value[value].name);
    }

    return status;
}

static config_status_t convert_string_to_enum_multi(config_mgr_element_enum_limit_t const * const enum_limit,
                                                    char const * const string,
                                                    config_mgr_enum_multi_value_t * const multi_list, bool const need_index)
{
    config_status_t status;

    size_t const max_length = enum_max_name_length(enum_limit);
    char each_value[max_length + 1];
    size_t i = 0;

    char * ptr = (char *)string;

    if (multi_list->count == 0)
    {
        tlr_config_info("No value return since ENUM_MULTI data not initialized\n");
        status = config_status_success;
        goto done;
    }

    while ((ptr != NULL) && (strlen(ptr) > 0) && (i < enum_limit->count) && (i < multi_list->count))
    {
        size_t n;
        char * p = strchr(ptr, ',');
        size_t len = (p != NULL) ? (p - ptr) : strlen(ptr);

        //snprintf(each_value, sizeof each_value, "%.*s", len, ptr);
        len++;
        strlcpy(each_value, ptr, len);

        status = enum_string_to_value(each_value, enum_limit, &multi_list->value[i], need_index);
        if (status != config_status_success)
        {
            tlr_config_debug("Invalid enum type value %s\n", each_value);
            goto done;
        }

        ptr = (p != NULL) ? p+1 : p;

        for (n=0; n < i; n++)
        {
            if (multi_list->value[n] == multi_list->value[i])
                break;   // duplicate
        }

        if (n == i) i++;
    }
    multi_list->count = i;
    status = config_status_success;

done:
    return status;
}

static config_status_t string_to_value(config_mgr_element_t const * const element, config_mgr_element_type_t const type,
                                       char const * const buffer, void * const value, bool const need_index)
{
    config_status_t result = config_status_success;

    if ((buffer == NULL) || (value == NULL)) {
        result = config_status_invalid_parameter;
        goto done;
    }

    if ((type != config_mgr_element_type_string) && (type != element->type))
    {
        result = config_status_invalid_format;
        goto done;
    }

    switch (type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_counter64:
        case config_mgr_element_type_ipaddr:
            /* NO conversion. just copy */
            strcpy(value, buffer);  /* Ouch, no length checking */
            break;

        case config_mgr_element_type_counter32:
        case config_mgr_element_type_uint32:
        {
            uint32_t * integer_value = value;
            int const count = sscanf(buffer, "%" SCNu32, integer_value);

            if (count != 1)
            {
                tlr_config_debug("Invalid uint32 value\n");
                result = config_status_invalid_format;
            }
            break;
        }

        case config_mgr_element_type_index:
        case config_mgr_element_type_int32:
        {
            int32_t * integer_value = value;
            int const count = sscanf(buffer, "%" SCNd32, integer_value);

            if (count != 1)
            {
                tlr_config_debug("Invalid int32 value\n");
                result = config_status_invalid_format;
            }
            break;
        }

        case config_mgr_element_type_enum_multi:
        {
            config_mgr_enum_multi_value_t * multi_list = value;

            result = convert_string_to_enum_multi(element->value_limit, buffer, multi_list, need_index);
            break;
        }

        case config_mgr_element_type_enum:
        {
            unsigned int * enum_value = value;

            result = enum_string_to_value(buffer, element->value_limit, enum_value, need_index);
            if (result != config_status_success)
            {
                tlr_config_debug("Invalid enum type value %s\n", buffer);
            }
            break;
        }

        case config_mgr_element_type_on_off:
        {
            unsigned int * enum_value = value;

            result = enum_string_to_value(buffer, &on_off_limit, enum_value, need_index);
            if (result != config_status_success)
            {
                tlr_config_debug("Invalid on-off type value\n");
            }
            break;
        }

        case config_mgr_element_type_boolean:
        {
            unsigned int * enum_value = value;

            result = enum_string_to_value(buffer, &boolean_limit, enum_value, need_index);
            if (result != config_status_success)
            {
                tlr_config_debug("Invalid boolean type value\n");
            }
            break;
        }

        case config_mgr_element_type_interface:
            /* TODO: handle interface */
            result = config_status_invalid_format;
            break;

        case  config_mgr_element_type_none:
            tlr_config_debug("Invalid element data type (%d)\n", element->type);
            result = config_status_invalid_format;
            break;
    }

done:
    return result;
}

void replace_char(char const * const string_src, char * const string_dest, size_t const length, char src, char dest) {
    size_t i;
    for (i = 0; i < length; i++) {
        if (string_src[i] == src)
            string_dest[i] = dest;
        else
            string_dest[i] = string_src[i];
    }
    string_dest[length] = '\0';
}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(group_id, instance, config_mgr_set_group);
    config_status_t status;
    tlr_config_data_t * config_data = NULL;
    void * uci_config_handle = NULL;

    if (group_ptr == NULL)
    {
        tlr_config_error("Unable to get group id %zd instance %zd\n", group_id, instance);
        goto error;
    }

    status = tlr_verify_access(group_ptr->access, config_mgr_read_operation, access);
    if (status != config_status_success) goto error;

	config_data = malloc(sizeof *config_data);
    if (config_data == NULL)
    {
        tlr_config_error("Failed to allocate config_data\n");
        goto error;
    }

	uci_config_handle = uci_config_alloc();
	if (uci_config_handle == NULL)
	{
		free(config_data);
		config_data = NULL;
		goto error;
	}

	config_data->uci_config_handle = uci_config_handle;
	config_data->group_ptr = group_ptr;
	config_data->instance = instance;
	config_data->access = access;

error:
	return config_data;
}

static config_status_t get_uci_data(tlr_handle_t const handle, size_t const element_id, uci_config_request_t * const request,
                                    char const * const element_name_suffix)
{
    tlr_config_data_t const * const config_data = handle;
    config_mgr_element_t const * const element_ptr = get_element_ptr(config_data->group_ptr, element_id);
    config_status_t status;

    if (element_ptr == NULL)
    {
        status = config_status_element_not_found;
        goto error;
    }

    status = tlr_verify_access(element_ptr->access, config_mgr_read_operation, config_data->access);

    if (status != config_status_success) goto error;

    {
        size_t   const   element_name_suffix_len = element_name_suffix != NULL ? strlen(element_name_suffix) : 0;
        char group_section[CONFIG_MGR_NAME_LENGTH];
        char group_option[CONFIG_MGR_NAME_LENGTH+element_name_suffix_len];
        replace_char(config_data->group_ptr->name, group_section, strlen(config_data->group_ptr->name), '-', '_');
        replace_char(element_ptr->name, group_option, strlen(element_ptr->name), '-', '_');
        if (element_name_suffix != NULL)
        {
            if ((strlen(group_option) + element_name_suffix_len) < sizeof group_option)
            {
                strcat(group_option, element_name_suffix);
            }
        }
        request->package = config_data->group_ptr->filename;
        request->section = group_section;
        request->index = config_data->instance;
        request->option = group_option;

        status = uci_config_get_option(config_data->uci_config_handle, request);
    }

error:
    return status;
}

config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    uci_config_request_t request = {0};
    config_status_t const status = get_uci_data(handle, element_id, &request, NULL);

    if (status == config_status_success)
        strlcpy(data, request.value, bytes);
    else
        *data = '\0';

    return status;
}

config_status_t tlr_config_read_native(tlr_handle_t const handle, size_t const element_id, void * const data)
{
    uci_config_request_t request = {0};
    tlr_config_data_t const * const config_data = handle;
    config_status_t status;

    status = get_uci_data(handle, element_id, &request, NULL);

    if (status != config_status_success) goto done;

    {
        config_mgr_element_t const * const element_ptr = get_element_ptr(config_data->group_ptr, element_id);
        bool const need_index = true;

        status = string_to_value(element_ptr, element_ptr->type, request.value, data, need_index);
    }

done:
    return status;
}

static size_t get_bytes_needed(config_mgr_element_t const * const element_ptr, char const * const value)
{
    size_t bytes = 0;

    switch (element_ptr->type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_counter64:
        case config_mgr_element_type_ipaddr:
            bytes = strlen(value) + 1;
            break;

        case config_mgr_element_type_counter32:
        case config_mgr_element_type_uint32:
        case config_mgr_element_type_index:
        case config_mgr_element_type_int32:
            bytes = sizeof(uint32_t);
            break;

        case config_mgr_element_type_enum_multi:
        {
            config_mgr_element_enum_limit_t const * const value_limit = element_ptr->value_limit;
            size_t const list_size = sizeof(unsigned int) * value_limit->count;

            bytes = sizeof(config_mgr_enum_multi_value_t) + list_size;
            break;
        }

        case config_mgr_element_type_enum:
        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
            bytes = sizeof(unsigned int);
            break;

        case config_mgr_element_type_interface:
        case  config_mgr_element_type_none:
            tlr_config_debug("Invalid element data type (%d)\n", element_ptr->type);
            break;
    }

    return bytes;
}

config_status_t tlr_config_read_alloc_value(tlr_handle_t const handle, size_t const element_id, void ** data, config_mgr_element_type_t const type)
{
    tlr_config_data_t const * const config_data = handle;
    config_mgr_element_t const * const element_ptr = get_element_ptr(config_data->group_ptr, element_id);
    uci_config_request_t request;
    config_status_t status;
    char    * name_suffix = NULL;
    config_mgr_element_type_t   type_to_return = type;

    if (element_ptr == NULL)
    {
        status = config_status_group_not_found;
        goto done;
    }
    if (element_ptr->type == config_mgr_element_type_encpassword &&
        type == config_mgr_element_type_password)
    {
        // Type is 'encpassword' and the requested type is 'password'. (i.e. the clear version)
        name_suffix = "_clear";
        type_to_return = config_mgr_element_type_string;    // And no further conversion
    }
    status = get_uci_data(handle, element_id, &request, name_suffix);
    if (status != config_status_success)
    {
        goto done;
    }

    {
        bool const need_index = false;
        size_t const bytes = (type_to_return == config_mgr_element_type_string) ?
                                strlen(request.value) + 1:
                                get_bytes_needed(element_ptr, request.value);

        *data = malloc(bytes);
        if (*data == NULL)
        {
            tlr_config_debug("malloc failed\n");
            status = config_status_no_memory;
            goto done;
        }

        status = string_to_value(element_ptr, type_to_return, request.value, *data, need_index);
        if (status != config_status_success)
        {
            free(*data);
            *data = NULL;
            goto done;
        }
    }

done:
    return status;
}


static bool get_this_device_default_password(char * const password, size_t const max_password_len)
{
    char    * serial_number = NULL;
    char    mac_addr[PASSWORD_GEN_MAC_ADDR_LEN + 1];
    struct ifreq ifr;
    bool    result;

    if (get_ifr("eth1", SIOCGIFHWADDR, &ifr))
    {
        unsigned char const * const mac = (unsigned char *)&ifr.ifr_hwaddr.sa_data;
        snprintf(mac_addr, sizeof mac_addr, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    else
    {
        tlr_config_debug("unable to generate get mac address\n");
        result = false;
        goto error;
    }


    if (get_tlr_serial_number(&serial_number) != 0)
    {
        tlr_config_debug("failed to get serial number\n");
        result = false;
        goto error;
    }

    if (generate_default_password(mac_addr, serial_number, password, max_password_len) < 0)
    {
        tlr_config_debug("unable to generate default password\n");
        result = false;
        goto error;
    }

    result = true;
error:
    return result;
}


static bool config_write_default_password(void * const config_handle, config_mgr_element_type_t const type, uci_config_request_t * const prequest)
{
    char    password[PASSWORD_GEN_PASSWORD_LEN];
    bool    result;

    if (!get_this_device_default_password(password, sizeof password))
    {
        tlr_config_debug("unable to generate default password\n");
        result = false;
        goto error;
    }

    prequest->value = password;
    if (set_password(config_handle, type, prequest) != config_status_success)
    {
        result = false;
    }
    else
    {
        result = true;
    }
error:
    return result;
}

static struct
{
    size_t group_id;
    size_t element_id;
    char const * password_string;
} default_password_elements[] =
{
    {config_user, config_user_password, "default"},
    {config_wifi, config_wifi_password, "wifidefault"},
    {config_wifi5g, config_wifi5g_password, "wifidefault"},
};

static bool element_is_default_password(size_t const group_id, size_t const element_id, char const * const data)
{
    bool result = false;
    int index;

    for (index = 0; index < ARRAY_SIZE(default_password_elements); index++)
    {
        if (group_id == default_password_elements[index].group_id &&
            element_id == default_password_elements[index].element_id &&
            strcmp(data, default_password_elements[index].password_string) == 0)
        {
            result = true;
            goto done;
        }
    }

done:
    return result;
}

config_status_t tlr_config_write(tlr_handle_t const handle, size_t const element_id, char const * const data)
{
    tlr_config_data_t const * const config_data = handle;
    config_mgr_element_t const * const element_ptr = get_element_ptr(config_data->group_ptr, element_id);
    config_status_t status = config_status_success;

    status = tlr_verify_access(config_data->group_ptr->access, config_mgr_write_operation, config_data->access);
    if (status != config_status_success)
        goto error;

    if (element_ptr == NULL)
    {
        status = config_status_element_not_found;
        goto error;
    }

    status = tlr_verify_access(element_ptr->access, config_mgr_write_operation, config_data->access);
    if (status != config_status_success)
        goto error;

    status = tlr_verify_range(element_ptr->validation, element_ptr->value_limit, element_ptr->type, data);
    if (status != config_status_success)
        goto error;

    {
        char group_section[CONFIG_MGR_NAME_LENGTH];
        char group_option[CONFIG_MGR_NAME_LENGTH];

        replace_char(config_data->group_ptr->name, group_section, strlen(config_data->group_ptr->name), '-', '_');
        replace_char(element_ptr->name, group_option, strlen(element_ptr->name), '-', '_');

        uci_config_request_t request =
        {
            .package = config_data->group_ptr->filename,
            .section = group_section,
            .index = config_data->instance,
            .option = group_option,
            .value = data,
            .commit = true
        };

        if (element_ptr->access == config_mgr_access_writeonly)
        {
            config_mgr_group_t const * const group_ptr = config_data->group_ptr;

            if (element_is_default_password(group_ptr->id, element_id, data))
            {
                if (!config_write_default_password(config_data->uci_config_handle, element_ptr->type, &request))
                {
                    tlr_config_debug("unable to write default password\n");
                    status = config_status_system_error;    // couldn't get mac or serial..
                    goto error;
                }
            }
            else
            {
                status = set_password(config_data->uci_config_handle, element_ptr->type, &request);
            }
        }
        else
        {
            status = uci_config_set_option(config_data->uci_config_handle, &request, (element_ptr->type == config_mgr_element_type_enum_multi));
        }
    }

error:
    return status;
}

config_status_t tlr_config_write_native(tlr_handle_t const handle, size_t const element_id, void const * const value)
{
#define MAX_NON_STR_SIZE 64
    tlr_config_data_t const * const config_data = handle;
    config_mgr_element_t const * const element_ptr = get_element_ptr(config_data->group_ptr, element_id);
    config_status_t status = config_status_success;
    char str_value[MAX_NON_STR_SIZE];
    char const * data_ptr = str_value;

    switch (element_ptr->type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_counter64:
        case config_mgr_element_type_ipaddr:
            data_ptr = value;
            break;

        case config_mgr_element_type_counter32:
        case config_mgr_element_type_uint32:
        {
            uint32_t const * const ptr = value;

            snprintf(str_value, sizeof str_value, "%" SCNu32, *ptr);
            break;
        }

        case config_mgr_element_type_index:
        case config_mgr_element_type_int32:
        {
            int32_t const * const ptr = value;

            snprintf(str_value, sizeof str_value, "%" SCNd32, *ptr);
            break;
        }

        case config_mgr_element_type_enum_multi:
            data_ptr = value;
            /* @TODO: currently cloud connector treats enum_multi as string, fix this when it is fixed in CC */
            break;

        case config_mgr_element_type_enum:
        {
            unsigned int const * const enum_value = value;

            status = enum_value_to_string(*enum_value, element_ptr->value_limit, str_value);

            if (status != config_status_success)
            {
                tlr_config_debug("Invalid enum value %u\n", *enum_value);
            }
            break;
        }

        case config_mgr_element_type_on_off:
        {
            unsigned int const * const enum_value = value;

            status = enum_value_to_string(*enum_value, &on_off_limit, str_value);

            if (status != config_status_success)
            {
                tlr_config_debug("Invalid on-off value %u\n", *enum_value);
            }
            break;
        }

        case config_mgr_element_type_boolean:
        {
            unsigned int const * const enum_value = value;

            status = enum_value_to_string(*enum_value, &boolean_limit, str_value);

            if (status != config_status_success)
            {
                tlr_config_debug("Invalid boolean value %u\n", *enum_value);
            }
            break;
        }

        case config_mgr_element_type_interface:
            /* TODO: handle interface */
            status = config_status_invalid_format;
            break;

        case  config_mgr_element_type_none:
            tlr_config_debug("Invalid element data type (%d)\n", element_ptr->type);
            status = config_status_invalid_format;
            break;
    }

    if (status == config_status_success)
    {
        status = tlr_config_write(handle, element_id, data_ptr);
    }

    return status;
}

void tlr_config_close(tlr_handle_t const handle)
{
	tlr_config_data_t * const config_data = (tlr_config_data_t * const)handle;

    if (config_data->uci_config_handle != NULL)
    {
		uci_config_free(config_data->uci_config_handle);
    }
	free(config_data);
}


char const * tlr_config_get_error(config_status_t const status)
{
    /* NOTE: Make sure this table matches the config_status_t enum values */
    static char const * const error_table[config_status_error_COUNT] =
    {
        "No error",
        "Invalid parameter",
        "No memory",
        "Invalid_format",

        "Group not found",
        "Element not found",
        "Instance not found",
        "Invalid value",
        "Action not found",
        "Access error",
        "Send error",

        "File not found",
        "File write error",
        "File read error",
        "No entry exist",
        "Set error",
        "Commit error",
        "Save error",
        "Revert error",

        "Command not found",
        "System error",
    };

    return (status < config_status_error_COUNT) ? error_table[status] : "";
}

static void update_table_max_tag(void)
{
    config_mgr_tag_table_t * table = &config_mgr_tag_table[config_mgr_max_tag];
    size_t i;

    for (i=0; i < table->count; i++)
    {
        config_mgr_tag_t * tag = &table->objects[i];

        switch (tag->type)
        {
        case config_mgr_tag_group:
            {
                config_mgr_group_t * group = tag->data;
                group->instances = tag->tag_value[platform_module];
            }
            break;

        case config_mgr_tag_element:
            {
                config_mgr_element_t * element = tag->data;

                switch (element->type)
                {
                case config_mgr_element_type_string:
                case config_mgr_element_type_encpassword:
                case config_mgr_element_type_password:
                case config_mgr_element_type_ipaddr:
                case config_mgr_element_type_fqdn:
                case config_mgr_element_type_mac_addr:
                    {
                        config_mgr_element_string_limit_t * limit = element->value_limit;

                        limit->max_length_in_bytes = tag->tag_value[platform_module];
                    }
                    break;
                case config_mgr_element_type_uint32:
                    {
                        config_mgr_element_unsigned_integer_limit_t * limit = element->value_limit;

                        limit->max_value = tag->tag_value[platform_module];
                    }
                    break;

                case config_mgr_element_type_int32:
                    {
                        config_mgr_element_signed_integer_limit_t * limit = element->value_limit;

                        limit->max_value = tag->tag_value[platform_module];
                    }
                    break;
                case config_mgr_element_type_enum_multi:
                case config_mgr_element_type_enum:
                case config_mgr_element_type_on_off:
                case config_mgr_element_type_boolean:
                case config_mgr_element_type_counter32:
                case config_mgr_element_type_counter64:
                case config_mgr_element_type_index:
                case config_mgr_element_type_interface:
                case config_mgr_element_type_none:
                    break;
                }
            }
            break;
        case config_mgr_tag_action:
        case config_mgr_tag_value:
        case config_mgr_tag_action_element:
            break;
        }
    }
}

static config_status_t uci_tag_action(config_tag_t const tag_id,
                                bool * const enable_tag, bool const set_action)
{
    config_status_t status;
    config_mgr_tag_table_t * const tag_table = &config_mgr_tag_table[config_mgr_feature_tag];
    void * uci_config_handle = NULL;

    if (tag_id > tag_table->count)
    {
        status = config_status_group_not_found;
        goto done;
    }

    uci_config_handle = uci_config_alloc();
    if (uci_config_handle == NULL)
    {
        status = config_status_no_memory;
        goto done;
    }

    {
        config_mgr_tag_t * const tag = &tag_table->objects[tag_id];
        config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(tag->group_id, 1, config_mgr_set_group);
        char const tag_section[] = "tag";

        if (group_ptr == NULL)
        {
            status = config_status_group_not_found;
            uci_config_free(uci_config_handle);
            goto done;

        }
        uci_config_request_t request =
        {
            .package = group_ptr->filename,
            .section = tag_section,
            .index = 1,
            .option = tag->name,
        };


        if (set_action)
        {
            request.value = (*enable_tag) ? "1" : "0";
            request.commit = true;
            status = uci_config_set_option(uci_config_handle, &request, false);
        }
        else
        {
            status = uci_config_get_option(uci_config_handle, &request);

            if (status == config_status_success)
            {
                *enable_tag = (atoi(request.value) != 0);
            }
        }
    }

    uci_config_free(uci_config_handle);

done:
    return status;
}

config_status_t tlr_set_uci_tag(config_tag_t const tag_id, bool const enable_tag)
{
    bool tag_value = enable_tag;

    config_status_t const status = uci_tag_action(tag_id, &tag_value, true);

    return status;
}

config_status_t tlr_get_uci_tag(config_tag_t const tag_id, bool * const enable_tag)
{

    config_status_t const status = uci_tag_action(tag_id, enable_tag, false);

    return status;
}


static void update_table_tag(void)
{
    config_mgr_tag_table_t * table = &config_mgr_tag_table[config_mgr_feature_tag];
    size_t i;
    unsigned int value;

    for (i=0; i < table->count; i++)
    {
        config_mgr_tag_t * tag = &table->objects[i];
        bool enable_tag;
        config_status_t const status = tlr_get_uci_tag(i, &enable_tag);

        if ((status == config_status_success) && (enable_tag))
        {
            /* enable access level when tag_value is non-zero */
            value = (unsigned int)enable_tag;
        }
        else
        {
            /* enable access level when tag_value is non-zero */
            value = tag->tag_value[platform_module];
        }

        /* enable access level when tag_value is non-zero */
        tlr_update_table_tag(i, value);

    }

}

config_status_t tlr_config_string_to_value(size_t const group_id, size_t const element_id, char const * const buffer, void * const value)
{
    config_status_t rc = config_status_success;
    config_mgr_group_table_t const * const table =  tlr_get_table(config_mgr_table_type_config);
    config_mgr_element_t * element;
    bool const need_index = false;

    if (group_id > table->count)    // XXX shouldn't this be >= ?
    {
        rc = config_status_group_not_found;
        goto ret;
    }

    {
        config_mgr_group_t const * const group = &table->groups[group_id];
        if (element_id > group->elements.count) // XXX shouldn't this be >= ?
        {
            rc = config_status_element_not_found;
            goto ret;
        }

        element = &group->elements.element[element_id];
    }

    rc = string_to_value(element, element->type, buffer, value, need_index);

ret:
    return rc;
}

void const * tlr_get_table(config_mgr_table_type_t const type)
{
    void const * table = NULL;

    /* TODO: should call here or when boot option is given */
    tlr_init_platform();

    switch (type)
    {
        case config_mgr_table_type_show:
            table = &config_mgr_group_table[config_mgr_get_group];
            break;

        case config_mgr_table_type_config:
            table = &config_mgr_group_table[config_mgr_set_group];
            break;

        case config_mgr_table_type_action:
            table = &config_mgr_action_table[0];
            break;

    }

    return table;
}

void tlr_update_table_tag(config_tag_t const tag_id, unsigned int value)
{
    config_mgr_tag_table_t * table = &config_mgr_tag_table[config_mgr_feature_tag];

    if (tag_id < table->count)
    {
        config_mgr_tag_t * tag = &table->objects[tag_id];
        config_mgr_access_t * access = NULL;
        config_mgr_group_t * group = NULL;

        switch (tag->type)
        {
        case config_mgr_tag_group:
            {
                group = tag->data;
                access = &group->access;
            }
            break;

        case config_mgr_tag_element:
            {
                config_mgr_element_t * element = tag->data;
                access = &element->access;
            }
            break;
        case config_mgr_tag_action:
            {
                config_mgr_action_t * action = tag->data;
                access = &action->access;
            }
            break;
        case config_mgr_tag_value:
            {
                config_mgr_element_enum_t * element_enum = tag->data;
                access = &element_enum->access;
            }
            break;

        case config_mgr_tag_action_element:
            {
                config_mgr_action_element_t * element = tag->data;
                access = &element->access;
            }
            break;
        }
        if ((access != NULL) && (value == 0))
        {
            if (group != NULL)
            {
                group->in_mib = false;
            }
            *access = config_mgr_access_no_access;
        }
    }

}

static int update_platform_tag(char const * const name)
{
    int result = -1;

    size_t i;

    for (i=0; i < platform_COUNT; i++)
    {
        config_platform_table_t const * const config_platform = &config_platform_table[i];

        if (strcmp(config_platform->name, name) == 0)
        {
            if (platform_module != config_platform->type)
            {
                tlr_config_debug("Current platform = %s\n", platform_name);
                platform_module = config_platform->type;
                update_table_max_tag();
                update_table_tag();
            }
            result = 0;
            break;
        }
    }

    return result;
}
static void tlr_init_platform(void)
{
    if (get_tlr_platform_name(&platform_name) < 0)
    {
        platform_name = default_platform_name;
    }

    if (update_platform_tag(platform_name) < 0)
    {
        platform_name = default_platform_name;

        update_platform_tag(platform_name);
    }

}

platform_type_t tlr_get_platform(void)
{
    if (platform_module == platform_COUNT)
        tlr_init_platform();

    return platform_module;
}

char const * tlr_get_platform_name(void)
{
    if (platform_module == platform_COUNT)
        tlr_init_platform();

    return platform_name;
}

void tlr_get_config_file(char *cfg_file, int maxlen)
{
    int     fd;

    fd = open(CONFIG_FILE_PTR, O_RDONLY, S_IRUSR | S_IWUSR);
    if (fd == -1)
        {
        snprintf ( cfg_file, maxlen, "%s", DEFAULT_CONFIG_DA0_FILE );
        }
    else
        {
        int     len2go,sl;
        ssize_t bytes;

        snprintf ( cfg_file, maxlen, "%s", CONFIG_PATH );

        len2go = maxlen - (sl=strlen(cfg_file));

        if ( (len2go > 2) && ((bytes=read(fd, &cfg_file[sl], len2go-1)) > 0) )
            {
            char    *tail = &cfg_file[sl+bytes-1];
            int     len, idx;

            while ( bytes-- && (*tail==0 || isspace(*tail)) )
                {
                tail--;
                }
            *(tail+1) = '\0';

            // Now remove any whitespace from the front....
            len = strlen(&cfg_file[sl]);
            for (idx=0; idx<len; idx++ )
                {
                if ( isspace(cfg_file[sl]) )
                    {
                    memmove(&cfg_file[sl], &cfg_file[sl+1], len-idx );
                    }
                else
                    break;
                }

            }
        else
            snprintf ( cfg_file, maxlen, "%s", DEFAULT_CONFIG_DA0_FILE );
        }

    if ( fd != -1 )
        close(fd);
}



