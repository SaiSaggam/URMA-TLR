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
#include "ddp_device.h"
#include "config_mgr.h"
#include "digiutils.h"
#include "tlr_print.h"
#include "digiuci.h"

static int process_an_element(FILE * const fd, char const * const name, char const * const value)
{
    char element_name[CONFIG_MGR_NAME_LENGTH];
    bool const is_empty = ((value == NULL) || (*value == '\0'));
    char const * const format = is_empty ? "<%s/>" : "<%s>";
    int result = strlcpy_digi_to_uci(element_name, name, sizeof element_name);

    if (result < 0) goto done;

    result = fprintf(fd, format, element_name);
    if (result < 0) goto done;

    if (is_empty) goto done;

    result = ddp_encode_xml_entities(fd, value);
    if (result < 0) goto done;

    result = fprintf(fd, "</%s>", element_name);

done:
    return result;
}

static int process_all_config_elements(FILE * const fd, size_t const group_id, size_t const instance, config_mgr_group_t const * const group_ptr)
{
    int result = -1;
    size_t element_id;
    tlr_handle_t handle = tlr_config_open(group_id, instance, config_mgr_access_level_super);

    if (handle == NULL) goto done;

    for (element_id = 0; element_id < group_ptr->elements.count; element_id++)
    {
        config_mgr_element_t const * const element_ptr = &group_ptr->elements.element[element_id];

        if (element_ptr->access == config_mgr_access_no_access) continue;

        {
            char * value = NULL;
            config_status_t const status = tlr_config_read_alloc_value(handle, element_id, (void **)&value, config_mgr_element_type_string);

            if (status != config_status_success)
            {
                result = -1;
                goto done;
            }


            result = process_an_element(fd, element_ptr->name, value);
            free(value);
            if (result < 0) break;
        }
    }

done:
    if (handle != NULL)
    {
        tlr_config_close(handle);
    }

    return result;
}

static int process_all_state_elements(FILE * const fd, size_t const group_id, size_t const instance, config_mgr_group_t const * const group_ptr)
{
    int result = -1;
    size_t element_id;
    tlr_handle_t handle = tlr_status_open(group_id, instance);

    if (handle == NULL) goto done;

    for (element_id = 0; element_id < group_ptr->elements.count; element_id++)
    {
        char const * value;
        config_mgr_element_t const * const element_ptr = &group_ptr->elements.element[element_id];
        config_status_t const status = tlr_status_get_string_ptr(handle, element_id, &value);

        if (status != config_status_success) break;

        result = process_an_element(fd, element_ptr->name, value);
        if (result < 0) break;
    }

done:
    if (handle != NULL)
    {
        tlr_status_close(handle);
    }

    return result;
}

static char * read_group(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index, bool const at_init)
{
    int result = 0;
    FILE * fd = NULL;
    size_t buf_size;
    char * buffer = NULL;
    char group_name[CONFIG_MGR_NAME_LENGTH];
    config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(group_id, instance, table_index);
    char const * const table_name = (table_index == config_mgr_set_group) ? "settings" : "state";

    if (group_ptr == NULL) goto done;
    if (group_ptr->access == config_mgr_access_no_access) goto done;

    fd = open_memstream(&buffer, &buf_size);
    if (fd == NULL) goto done;

    result = strlcpy_digi_to_uci(group_name, group_ptr->name, sizeof group_name);
    if (result < 0) goto done;

    result = fprintf(fd, "<%s><%s index=\"%zd\">", table_name, group_name, instance);
    if (result < 0) goto done;

    if (!at_init)
    {
        int (* read_fn)(FILE * const fd, size_t const group_id, size_t const instance, config_mgr_group_t const * const group_ptr);

        read_fn = (table_index == config_mgr_set_group) ? process_all_config_elements : process_all_state_elements;
        result = read_fn(fd, group_id, instance, group_ptr);
        if (result < 0) goto done;
    }

    result = fprintf(fd, "</%s></%s>", group_name, table_name);

done:
    if (fd != NULL)
    {
        fclose(fd);
    }

    if (result < 0)
    {
        free(buffer);
        buffer = NULL;
    }

    return buffer;
}

size_t ddp_get_group_count(bool const is_config)
{
    config_mgr_group_type_t const type = is_config ? config_mgr_set_group : config_mgr_get_group;

    return tlr_get_num_groups(type);
}

size_t ddp_get_group_instance(size_t const group_id, bool const is_config)
{
    config_mgr_table_type_t const type = is_config ? config_mgr_table_type_config : config_mgr_table_type_show;

    return tlr_get_max_instance(group_id, type);
}

char * ddp_read_group(size_t const group_id, size_t const instance, bool const is_config, bool const at_init)
{
    char * result;
    config_mgr_group_type_t const type = is_config ? config_mgr_set_group : config_mgr_get_group;

    if (!is_config)
    {
        if (!tlr_is_json_supported(group_id))
        {
            result = NULL;
            goto done;
        }
    }

    result = read_group(group_id, instance, type, at_init);

done:
    return result;
}

static int read_descriptor(FILE * const fd, char const * const path)
{
    int result = -1;
    FILE * const fptr = fopen(path, "rb");
    size_t fsize;

    if (fptr == NULL) goto done;

    fseek(fptr, 0, SEEK_END);

    {
        long const last_pos = ftell(fptr);

        if (last_pos < 0) goto done;

        fsize = (size_t)last_pos;
    }

    rewind(fptr);

    {
        char * buffer = malloc(fsize + 1);
        size_t bytes;

        if (buffer == NULL) goto done;

        bytes = fread(buffer, 1, fsize, fptr);
        if (bytes == fsize)
        {
            buffer[fsize] = '\0';
            result = ddp_encode_xml_entities(fd, buffer);
        }

        free(buffer);
    }


done:
    if (fptr != NULL)
    {
        fclose(fptr);
    }

    return result;
}

static char * frame_descriptor(char const * const tag, char const * const fpath)
{
    int result;
    size_t buf_size;
    char * buffer = NULL;
    FILE * fd = open_memstream(&buffer, &buf_size);

    if (fd == NULL) goto error;

    result = fprintf(fd, "<%s>", tag);
    if (result < 0) goto done;

    result = read_descriptor(fd, fpath);
    if (result < 0) goto done;

    result = fprintf(fd, "</%s>", tag);

done:
    fclose(fd);

    if (result < 0)
    {
        free(buffer);
        buffer = NULL;
    }

 error:
    return buffer;
}

char * ddp_read_config_descriptor(void)
{
#if defined(_CUNIT_) || defined(UNIT_TEST)
    static char const config_path[] = "descriptor_query_setting.xml";
#else
    static char const config_path[] = "/usr/web/descriptors/descriptor_query_setting.xml";
#endif
    static char const config_tag[] = "settings_descriptor";

    return frame_descriptor(config_tag, config_path);
}

char * ddp_read_state_descriptor(void)
{
#if defined(_CUNIT_) || defined(UNIT_TEST)
    static char const state_path[] = "descriptor_query_state.xml";
#else
    static char const state_path[] = "/usr/web/descriptors/descriptor_query_state.xml";
#endif
    static char const state_tag[] = "state_descriptor";

    return frame_descriptor(state_tag, state_path);
}

ssize_t ddp_get_group_id(char const * const group_name, bool const is_config)
{
    return is_config ? tlr_get_group_id(group_name) : tlr_get_show_group_id(group_name);
}

bool ddp_set_element(ddp_group_info_t const * const info)
{
    bool success = false;
    ssize_t const group_id = tlr_get_group_id(info->group_name);
    config_mgr_access_level_t const access = (config_mgr_access_level_t)info->access_level;

    if (group_id < 0) goto error;

    {
        ssize_t const element_id = tlr_get_element_id(group_id, info->element_name);

        if (element_id < 0) goto error;

        success = (tlr_set_element_value(group_id, info->instance, element_id, info->value, access) == config_status_success);

        tlr_event_log(tlr_log_info, info->user, "set %s/%zd/%s %s\n", info->group_name, info->instance,
                      info->element_name, success ? "success" : "failed");
    }

error:
    return success;
}

bool ddp_apply_group(ddp_group_info_t const * const info)
{
    ssize_t const group_id = tlr_get_group_id(info->group_name);
    config_mgr_access_level_t const access = (config_mgr_access_level_t)info->access_level;

    return tlr_apply_group(group_id, info->instance, info->user, access);
}

bool ddp_save(char const * const user)
{
    int const result = system_command("action_save_config -f ascii -i 1 -u %s", user);

    return (result != -1);
}

ddp_access_t ddp_authenticate(char const * const username, char const * const password)
{
    bool const is_logged_in = false;
    ddp_access_t access = ddp_no_access;
    config_mgr_access_level_t access_level = config_mgr_access_level_read_only;
    bool const result = tlr_user_is_authorized(username, password, &access_level, is_logged_in);

    if (result)
    {
        access = access_level;
    }

    return access;
}

int ddp_encode_xml_entities(FILE * const fd, char const * const value)
{
    int result = -1;
    char const * current_ptr = value;

    for (;;)
    {
        switch (*current_ptr)
        {
            case '>':
                result = fprintf(fd, "&gt;");
                break;

            case '<':
                result = fprintf(fd, "&lt;");
                break;

            case '"':
                result = fprintf(fd, "&quot;");
                break;

            case '\'':
                result = fprintf(fd, "&apos;");
                break;

            case '&':
                result = fprintf(fd, "&amp;");
                break;

            case '\0':
                result = 0;
                break;

            default:
                result = fprintf(fd, "%c", *current_ptr);
                break;
        }

        if (result <= 0) break;

        current_ptr++;
    }

    return result;
}
