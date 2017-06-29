/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <malloc.h>
#include "connector_types.h"
#include "connector_api.h"
#include "connector_config.h"
#include "platform.h"
#include <string.h>

int32_t init_signed = 0;
uint32_t init_unsigned = 1;
float init_float = 0.1;
char * init_string = "default\0";
char * init_ipv4 = "0.0.0.0\0";
char * init_fqdnv4 = "login.etherios.com\0";
char * init_datetime = "2012-12-21T00:00:00Z\0";

void * ptr = 0;

#define arrplen(a) sizeof(a)/sizeof(setting_data_t)

typedef struct {
    void ** group_data;
    unsigned int group_id;
    unsigned int index;
    unsigned int capacity;
    unsigned int count;
} setting_data_t;

typedef struct {
    setting_data_t ** setting_data;
    unsigned int capacity;
    unsigned int count;
} rci_data_t;

rci_data_t rci_data[] = {
    { // setting data
        NULL,
        0,
        0
    },
    { // state data
        NULL,
        0,
        0
    }
};

typedef struct {
    void * group_context;
} remote_group_session_t;

void * get_setting_data(connector_remote_config_t * const remote_config,
    const void * def, int length);
void add_setting_data(connector_remote_config_t * const remote_config,
    const void * data, int length);

void * get_setting_data(connector_remote_config_t * const remote_config,
    const void * def, int length){

    unsigned int group_id = remote_config->group.id;
    unsigned int index = remote_config->group.index;
    unsigned int element_id = remote_config->element.id;

    rci_data_t * r_data = &rci_data[remote_config->group.type];

    // Check to see if the setting data already exists.
    if(r_data->setting_data != NULL && group_id < r_data->capacity){
        // Note: Unfortunately if there are indexed groups, the rci_data_t
        // structure does not provide direct access, so we first lookup the
        // data by group id, if there is a data mistmatch, we increment until
        // we find the data.
        // This can be enhanced by nesting a struct inside setting_data_t,
        // separating out indexed data, but that would complicate things a bit
        // more, so we have this instead.
        unsigned int i;
        for(i = group_id; i < r_data->capacity; i++){
            setting_data_t * s_data = r_data->setting_data[i];
            // If setting data is null, it hasn't been initialized yet.
            // If the encountered group id exceeds the one we're looking for
            // break out.
            if(s_data == NULL || s_data->group_id > group_id) break;
            if(s_data->group_id == group_id &&
               s_data->index == index &&
               element_id < s_data->count &&
               s_data->group_data[element_id] != NULL){
                return s_data->group_data[element_id];
            }
        }
    }

    // Data doesn't exist, add it, note: if this fails to store data,
    // we could enter infinite recursion, so be careful when modifying
    // add_setting_data.
    add_setting_data(remote_config, def, length);
    return get_setting_data(remote_config, def, length);
}

void add_setting_data(connector_remote_config_t * const remote_config,
    const void * data, int length){

    unsigned int group_id = remote_config->group.id;
    unsigned int index = remote_config->group.index;
    unsigned int element_id = remote_config->element.id;

    rci_data_t * r_data = &rci_data[remote_config->group.type];

    // Init setting data if not set.
    //APP_DEBUG("Setting Data for %d:%d.\n", group_id, element_id);
    if(r_data->setting_data == NULL){
        unsigned int const capacity = 10;
        size_t const malloc_size = capacity * sizeof(setting_data_t *);
        unsigned int i;

        r_data->capacity = capacity;
        r_data->setting_data = malloc(malloc_size);
        for (i = 0; i < capacity; i++)
        {
            r_data->setting_data[i] = NULL;
        }
    }

    // If group id extends size of setting_data grow it.
    if(r_data->count == r_data->capacity)
    {
        unsigned int const old_capacity = r_data->capacity;
        unsigned int const new_capacity = old_capacity + 10;
        size_t const new_size = new_capacity * sizeof(setting_data_t);
        unsigned int i;

        r_data->setting_data = realloc(r_data->setting_data, new_size);
        for (i = old_capacity; i < new_capacity; i++)
        {
            r_data->setting_data[i] = NULL;
        }
        r_data->capacity = new_capacity;
    }

    // Init group data if not set.
    setting_data_t * s_data = NULL;
    unsigned int i;
    for(i = group_id; i < r_data->count; i++){
        s_data = r_data->setting_data[i];
        if(s_data != NULL &&
            s_data->group_id == group_id &&
            s_data->index == index){
            // Found setting data;
            break;
        }
        else{
            s_data = NULL;
        }
    }

    // Allocate Setting Data if not found.
    if(s_data == NULL)
    {
        s_data = malloc(sizeof *s_data);

        r_data->setting_data[i] = s_data;
        r_data->count = r_data->count + 1;
        s_data->group_id = group_id;
        s_data->index = index;
        s_data->capacity = 10;
        s_data->count = 0;
        s_data->group_data = malloc(s_data->capacity * sizeof(ptr));
        {
            unsigned int index;
            for (index = 0; index < s_data->count; index++)
            {
                s_data->group_data[index] = NULL;
            }
        }
    }

    while(s_data->count == s_data->capacity ||
        element_id >= s_data->capacity)
    {
        unsigned int const old_capacity = s_data->capacity;
        unsigned int const new_capacity = old_capacity + 10;
        unsigned int const new_size = new_capacity * sizeof(ptr);
        unsigned int i;

        s_data->group_data = realloc(s_data->group_data, new_size);
        for (i = old_capacity; i < new_capacity; i++)
        {
            s_data->group_data[i] = NULL;
        }

        s_data->capacity = new_capacity;
    }

    void * data_ptr = malloc(length + 1);
    memcpy(data_ptr, data, length);
    s_data->group_data[element_id] = data_ptr;
    s_data->count += 1;
}

static connector_callback_status_t app_rci_group_process(connector_remote_config_t * const remote_config);

static connector_callback_status_t app_rci_group_get(connector_remote_config_t * const remote_config);

static connector_callback_status_t app_rci_group_set(connector_remote_config_t * const remote_config);



static connector_callback_status_t app_rci_group_process(connector_remote_config_t * const remote_config)
{

    if(remote_config->action == connector_remote_action_query){
        return app_rci_group_get(remote_config);
    }
    else{
        return app_rci_group_set(remote_config);
    }
}

static connector_callback_status_t app_rci_group_get(connector_remote_config_t * const remote_config)
{
    void * data;

    if (remote_config->group.type == connector_remote_group_setting && remote_config->group.id == connector_setting_attibute_feedback)
    {
        switch (remote_config->element.id)
        {
            case connector_setting_attibute_feedback_source:
                if (remote_config->attribute.source != rci_query_setting_attribute_source_current)
                {
                    remote_config->response.element_value->unsigned_integer_value = remote_config->attribute.source;
                    goto done;
                }
                break;
            case connector_setting_attibute_feedback_compare_to:
                if (remote_config->attribute.compare_to != rci_query_setting_attribute_compare_to_none)
                {
                    remote_config->response.element_value->unsigned_integer_value = remote_config->attribute.compare_to;
                    goto done;
                }
                break;
            default:
                ASSERT(0);
                break;
        }
    }

    switch(remote_config->element.type){
        case connector_element_type_string:
        case connector_element_type_multiline_string:
        case connector_element_type_password:
            data = get_setting_data(remote_config, init_string,
                sizeof(char)*(strlen(init_string) + 1));
            remote_config->response.element_value->string_value = data;
            break;
        case connector_element_type_int32:
            data = get_setting_data(remote_config, &init_signed, sizeof(int32_t));
            remote_config->response.element_value->signed_integer_value = *(int32_t *)data;
            break;
        case connector_element_type_uint32:
        case connector_element_type_hex32:
        case connector_element_type_0x_hex32:
            data = get_setting_data(remote_config, &init_unsigned,
                sizeof(uint32_t));
            remote_config->response.element_value->unsigned_integer_value = *(uint32_t *)data;
            break;
        case connector_element_type_float:
            data = get_setting_data(remote_config, &init_float, sizeof(float));
            remote_config->response.element_value->float_value = init_float;
            break;
        case connector_element_type_enum:
        case connector_element_type_on_off:
        case connector_element_type_boolean:
            data = get_setting_data(remote_config, &init_unsigned, sizeof(uint32_t));
            remote_config->response.element_value->unsigned_integer_value = *(uint32_t *)data;
            break;
        case connector_element_type_ipv4:
            data = get_setting_data(remote_config, init_ipv4,
                sizeof(char)*(strlen(init_ipv4)+1));
            remote_config->response.element_value->string_value = data;
            break;
        case connector_element_type_fqdnv4:
        case connector_element_type_fqdnv6:
            data =  get_setting_data(remote_config, init_fqdnv4,
                sizeof(char)*(strlen(init_fqdnv4)+1));
            remote_config->response.element_value->string_value = data;
            break;
        case connector_element_type_datetime:
            data = get_setting_data(remote_config, init_datetime,
                sizeof(char)*(strlen(init_datetime)+1));
            remote_config->response.element_value->string_value = data;
            break;
        default:
            APP_DEBUG("Unknown Type.\n");
    }

done:
    return connector_callback_continue;
}

static connector_callback_status_t app_rci_group_set(connector_remote_config_t * const remote_config)
{

    switch(remote_config->element.type){
        case connector_element_type_string:
        case connector_element_type_multiline_string:
        case connector_element_type_password:
        case connector_element_type_ipv4:
        case connector_element_type_fqdnv4:
        case connector_element_type_fqdnv6:
        case connector_element_type_datetime:
            add_setting_data(remote_config, remote_config->element.value->string_value,
                sizeof(char)*(strlen(remote_config->element.value->string_value)+1));
            break;
        case connector_element_type_int32:
            add_setting_data(remote_config,
                &remote_config->element.value->signed_integer_value,
                sizeof(remote_config->element.value->signed_integer_value));
            break;
        case connector_element_type_uint32:
        case connector_element_type_hex32:
        case connector_element_type_0x_hex32:
            add_setting_data(remote_config,
                &remote_config->element.value->unsigned_integer_value,
                sizeof(remote_config->element.value->unsigned_integer_value));
            break;
        case connector_element_type_float:
            add_setting_data(remote_config,
                &remote_config->element.value->float_value,
                sizeof(remote_config->element.value->float_value));
            break;
        case connector_element_type_enum:
        case connector_element_type_on_off:
        case connector_element_type_boolean:
            add_setting_data(remote_config,
                &remote_config->element.value->boolean_value,
                sizeof(remote_config->element.value->boolean_value));
            break;
    }


    return connector_callback_continue;
}

connector_callback_status_t app_remote_config_handler(connector_request_id_remote_config_t const request_id,
                                                      void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch(request_id){
        case connector_request_id_remote_config_session_start:
        case connector_request_id_remote_config_action_start:
        case connector_request_id_remote_config_group_start:
        case connector_request_id_remote_config_group_end:
        case connector_request_id_remote_config_action_end:
        case connector_request_id_remote_config_session_end:
        case connector_request_id_remote_config_session_cancel:
            // do nothing here.
            break;
        case connector_request_id_remote_config_group_process:
            status = app_rci_group_process(data);
            break;
        default:
            APP_DEBUG("Request Unknown: %d.\n", request_id);
            ASSERT(0);
            break;
    }

    return status;
}
