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

#include "config_mgr.h"

#include <stddef.h>
#include <stdbool.h>
#include <json.h>

struct tlr_show_object_t
{
    config_mgr_group_t const * group;
    json_object * json_parent;
};

tlr_show_object_t * tlr_show_object_create(size_t const group_id, size_t const instance)
{
    tlr_show_object_t * show_object;

    show_object = malloc(sizeof *show_object);
    if (show_object == NULL)
    {
        goto done;
    }

    show_object->group = tlr_get_group_ptr(group_id, instance, config_mgr_get_group);
    if (show_object->group == NULL)
    {
        goto error;
    }

    show_object->json_parent = json_object_new_object();
    if (show_object->json_parent == NULL)
    {
        goto error;
    }

    goto done;

error:
    free(show_object);
    show_object = NULL;

done:
    return show_object;
}

static json_object * new_json_object(config_mgr_element_type_t const type, void const * const value)
{
    json_object * new_object = NULL;

    if (value == NULL)
    {
        goto done;
    }

    switch (type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_enum_multi:
        case config_mgr_element_type_ipaddr:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_interface:
            new_object = json_object_new_string(value);
            break;

        case config_mgr_element_type_int32:
        case config_mgr_element_type_enum:
        case config_mgr_element_type_index:
            new_object = json_object_new_int(*(int32_t *)value);
            break;

        case config_mgr_element_type_uint32:
        case config_mgr_element_type_counter32:
        {
            char * value_as_string = NULL;
            if (asprintf(&value_as_string, "%" PRIu32, *(uint32_t *)value) > 0)
            {
                new_object = json_object_new_string(value_as_string);
            }
            free(value_as_string);
            break;
        }

        case config_mgr_element_type_counter64:
        {
            char * value_as_string = NULL;
            if (asprintf(&value_as_string, "%" PRIu64, *(uint64_t *)value) > 0)
            {
                new_object = json_object_new_string(value_as_string);
            }
            free(value_as_string);
            break;
        }

        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
            new_object = json_object_new_boolean(*(json_bool *)value);
            break;

        case config_mgr_element_type_none:
            new_object = NULL;
            break;
    }

done:
    return new_object;
}

void tlr_show_object_add_element(tlr_show_object_t * const show_object, size_t const element_id, void const * const value)
{
    config_mgr_element_t const * const element = &show_object->group->elements.element[element_id];
    json_object * const json_element = new_json_object(element->type, value);

    json_object_object_add(show_object->json_parent, element->name, json_element);
}

char const * tlr_show_object_to_json_string(tlr_show_object_t const * const show_object)
{
    return json_object_to_json_string_ext(show_object->json_parent, JSON_C_TO_STRING_PRETTY);
}

void tlr_show_object_destroy(tlr_show_object_t * const show_object)
{
    if (show_object != NULL)
    {
        json_object_put(show_object->json_parent);
        free(show_object);
    }
}

