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

#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <json.h>

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"
#include <tlr_common.h>
#include <strlcpy.h>

#define SHOW_READ_TIMEOUT_MILLISECS 2000

char * read_json_object(tlr_handle_t handle)
{
#define MALLOC_BLOCK_SIZE 1024
    size_t space_available = MALLOC_BLOCK_SIZE;
    char * buffer = malloc(space_available);

    if (buffer == NULL)
    {
        tlr_config_debug("malloc to hold json string failed\n");
        goto done;
    }

    {
        static size_t const timeout_msec = SHOW_READ_TIMEOUT_MILLISECS;
        size_t bytes_read = 0;
        buffer[0] = '\0';

        for (;;)
        {
            /*
             * XXX Do we really need this timeout? Would using the JSON API call to read directly
             * from a file handle be acceptable in this case?
            */
            char * const line = tlr_show_read_line_alloc(handle, timeout_msec);
            size_t line_len;
            size_t required_space;

            if (line == NULL)
            {
                break;
            }
            line_len = strlen(line);
            required_space = bytes_read + line_len + 1; /* Allow 1 for NUL terminator */

            if (required_space > space_available)
            {
                space_available += required_space - space_available;
                buffer = realloc(buffer, space_available);

                if (buffer == NULL)
                {
                    tlr_config_debug("malloc to hold json string failed\n");
                    free(line);
                    free(buffer);
                    buffer = NULL;
                    goto done;
                }
            }
            strcpy(buffer + bytes_read, line);
            free(line);
            bytes_read += line_len;
        }
    }

done:
    return buffer;
}

static json_object * const get_json_object(tlr_handle_t handle)
{
    json_object * show_obj = NULL;
    char * const buffer = read_json_object(handle);

    if (buffer == NULL)
    {
        goto done;
    }
    show_obj = json_tokener_parse(buffer);

done:
    free(buffer);
    return show_obj;
}

static char const * get_status_string_ptr(json_object * const parent_obj, config_mgr_element_t const * const element_ptr)
{
    char const * string_ptr = "";

    json_object_object_foreach(parent_obj, key, val)
    {
        if (strcmp(element_ptr->name, key) == 0)
        {
            char const * const value = json_object_get_string(val);

            if (value != NULL)
            {
                string_ptr = value;
            }

            break;
        }
    }

    return string_ptr;
}

static void get_value_from_json_object(json_object * const obj, config_mgr_element_type_t element_type, void * const value_buffer, size_t const value_size)
{
    switch (element_type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_enum_multi:
        case config_mgr_element_type_ipaddr:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_interface:
        {
            char * const ptr = value_buffer;
            char const * const value = json_object_get_string(obj);
            char const * const normalized = ((value == NULL) ? "" : value);

            strlcpy(ptr, normalized, value_size);
            break;
        }

        case config_mgr_element_type_int32:
        case config_mgr_element_type_enum:
        case config_mgr_element_type_index:
        {
            int32_t * const ptr = value_buffer;

            *ptr = json_object_get_int(obj);
            break;
        }

        case config_mgr_element_type_uint32:
        case config_mgr_element_type_counter32:
        {
            uint32_t * const ptr = value_buffer;
            char const * const value = json_object_get_string(obj);

            *ptr = strtoul(value, NULL, 10);
            break;
        }

        case config_mgr_element_type_counter64:
        {
            uint64_t * const ptr = value_buffer;
            char const * const value = json_object_get_string(obj);

            *ptr = strtoull(value, NULL, 10);
            break;
        }

        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
        {
            bool * const ptr = value_buffer;

            *ptr = json_object_get_boolean(obj);
            break;
        }

        case config_mgr_element_type_none:
            tlr_config_debug("got unexpected request for value of element type 'none\n");
            break;
    }
}

static config_status_t get_status_value(json_object * const parent_obj, config_mgr_element_t const * const element_ptr, void * const value_buffer, size_t const value_size)
{
    config_status_t status = config_status_element_not_found;

    json_object_object_foreach(parent_obj, key, val)
    {
        if (strcmp(element_ptr->name, key) == 0)
        {
            get_value_from_json_object(val, element_ptr->type, value_buffer, value_size);
            status = config_status_success;
            break;
        }
    }

    return status;
}

typedef struct
{
    config_mgr_group_t const * group_ptr;
    json_object * json_obj;
} tlr_status_info_t;

tlr_handle_t tlr_status_open(size_t const group_id, size_t const instance)
{
    tlr_status_info_t * status_ptr = malloc(sizeof *status_ptr);

    if (status_ptr == NULL) goto done;

    status_ptr->group_ptr = tlr_get_group_ptr(group_id, instance, config_mgr_get_group);

    if (status_ptr->group_ptr == NULL) goto error;

    {
        tlr_handle_t show_handle = tlr_show_open("json", group_id, instance, NULL, config_mgr_access_level_read_only);

        if (show_handle == NULL) goto error;

        status_ptr->json_obj = get_json_object(show_handle);
        tlr_show_close(show_handle);
    }

    if (status_ptr->json_obj != NULL) goto done;

error:
    free(status_ptr);
    status_ptr = NULL;

done:
    return status_ptr;
}

config_status_t tlr_status_read(tlr_handle_t const handle, size_t const element_id, void * const data, size_t const bytes)
{
    config_status_t status;
    tlr_status_info_t const * const status_ptr = handle;
    config_mgr_group_t const * const group_ptr = status_ptr->group_ptr;
    json_object * const jobj = status_ptr->json_obj;

    if (element_id < group_ptr->elements.count)
    {
        config_mgr_element_t const * const element_ptr = &group_ptr->elements.element[element_id];

        status = get_status_value(jobj, element_ptr, data, bytes);
    }
    else
    {
        tlr_config_debug("invalid element id %zd\n", element_id);
        status = config_status_element_not_found;
    }

    return status;
}

config_status_t tlr_status_get_string_ptr(tlr_handle_t const handle, size_t const element_id, char const ** const string_ptr)
{
    config_status_t status;
    tlr_status_info_t const * const status_ptr = handle;
    config_mgr_group_t const * const group_ptr = status_ptr->group_ptr;
    json_object * const jobj = status_ptr->json_obj;

    if (element_id < group_ptr->elements.count)
    {
        config_mgr_element_t const * const element_ptr = &group_ptr->elements.element[element_id];

        *string_ptr = get_status_string_ptr(jobj, element_ptr);
        status = config_status_success;
    }
    else
    {
        tlr_config_debug("invalid element id %zd\n", element_id);
        *string_ptr = "";
        status = config_status_element_not_found;
    }

    return status;
}


void tlr_status_close(tlr_handle_t handle)
{
    if (handle != NULL)
    {
        tlr_status_info_t * const status_ptr = (tlr_status_info_t * const)handle;
        json_object * const jobj = status_ptr->json_obj;

        json_object_put(jobj);
        free(status_ptr);
    }
}
