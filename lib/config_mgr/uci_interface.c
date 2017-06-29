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

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "uci.h"
#include "uci_interface.h"
#include "config_mgr.h"
#include "tlr_common.h"

// #define UCI_DEBUG
#ifdef UCI_DEBUG
#define uci_debug(x...) tlr_config_debug(x)
#else
#define uci_debug(x...)
#endif

typedef enum
{
    uci_config_type_save,
    uci_config_type_revert,
    uci_config_type_apply,
    uci_config_type_get,
    uci_config_type_set
} uci_config_type_t;

static char const config_default_dir[] = UCI_DEFAULT_PATH;
static char const config_modified_dir[] = UCI_MODIFIED_PATH;
//static char const config_default_dir[] = "config_mgr/uci_default";
//static char const config_modified_dir[] = "config_mgr/uci_modified";

/* couldn't find any UCI API to extract the option value from the lookup pointer [vid, 11/01/13] */
static char * uci_config_extract_value(uci_config_data_t * const config_data)
{
    struct uci_ptr * ptr = &config_data->uci_data;
#ifndef USE_DEFAULT_DEFAULT_VALUES

    if (ptr->o != NULL)
    {
        switch(ptr->o->type)
        {
            case UCI_TYPE_STRING:
                break;

            case UCI_TYPE_LIST:
                {
                    struct uci_element *e;
                    struct uci_element *t;
                    char * data_ptr = config_data->option_value;
                    size_t avail_length = sizeof config_data->option_value;
                    bool sep = false;
                    int len;

                    uci_foreach_element_safe(&ptr->o->v.list, t, e) {
                        if (avail_length == 0)
                        {
                            tlr_config_info("uci_config_extract_value: %s.%s.%s not enough buffer space\n", ptr->package, ptr->section, ptr->option);
                            break;
                        }
                        if ((sep) && (avail_length > 0))
                        {
                            *data_ptr = ',';
                            data_ptr++;
                            avail_length--;
                        }

                        len = snprintf(data_ptr, avail_length, "%s", e->name);
                        data_ptr += len;
                        avail_length -= len;

                        //space = strpbrk(e->name, " \t\r\n");
                        sep = true;
                        uci_debug("uci_config_extract_value (LIST): %s", config_data->option_value);
                    }
                }
                goto done;

            default:
                tlr_config_info("uci_config_extract_value: %s.%s.%s not UCI type %d\n", ptr->package, ptr->section, ptr->option, ptr->o->type);
                break;
        }
    }

    {
        char * const value = (ptr->o != NULL) ? ptr->o->v.string : "";
        strlcpy(config_data->option_value, value, sizeof config_data->option_value);
        uci_debug("uci_config_extract_value: %s.%s.%s = %s\n", ptr->package, ptr->section, ptr->option, config_data->option_value);
    }

done:
    return config_data->option_value;
#else
    /* return the value if we find it, else return NULL */
    char * value = NULL;

    if ((value != NULL ) && (ptr->o != NULL))
    {
        switch(ptr->o->type)
        {
            case UCI_TYPE_STRING:
                strlcpy(config_data->option_value, ptr->o->v.string, sizeof config_data->option_value);
                value = config_data->option_value;
                break;

            case UCI_TYPE_LIST:
                {
                    struct uci_element *e;
                    struct uci_element *t;
                    char * data_ptr = config_data->option_value;
                    size_t avail_length = sizeof config_data->option_value;
                    bool sep = false;
                    int len;

                    uci_foreach_element_safe(&ptr->o->v.list, t, e)
                    {
                        if (avail_length == 0)
                        {
                            tlr_config_info("uci_config_extract_value: %s.%s.%s not enough buffer space\n", ptr->package, ptr->section, ptr->option);
                            break;
                        }
                        if (sep)
                        {
                            len = snprintf(data_ptr, avail_length, "%s", ", ");
                            data_ptr += len;
                            avail_length -= len;
                        }

                        len = snprintf(data_ptr, avail_length, "%s", e->name);
                        data_ptr += len;
                        avail_length -= len;

                        //space = strpbrk(e->name, " \t\r\n");
                        sep = true;
                        // tlr_config_info("uci_config_extract_value (LIST): %s", config_data->option_value);
                    }
                    value = config_data->option_value;
                }
                break;
            default:
                tlr_config_info("uci_config_extract_value: %s.%s.%s not UCI type %d\n", ptr->package, ptr->section, ptr->option, ptr->o->type);
                break;
        }

    }
    return value;

#endif
}

static config_status_t uci_config_lookup(uci_config_data_t * const config_data, uci_config_request_t const * const request)
{
#define uci_str_length 256
    config_status_t status = config_status_success;
    static char uci_string[uci_str_length]; /* Note: needs static/heap buffer, because UCI updates this string
                                               and its internal structure points to this for subsequent calls */


    if (request->option == NULL)
        snprintf(uci_string, uci_str_length, "%s.%s%zd", request->package, request->section, request->index);
    else
        snprintf(uci_string, uci_str_length, "%s.%s%zd.%s", request->package, request->section, request->index, request->option);

    {
        int const result = uci_lookup_ptr(config_data->context_to_use, &config_data->uci_data, uci_string, true);

        if (result != UCI_OK)
            status = config_status_no_entry_exist;
    }

    return status;
}

static config_status_t uci_config_read_option(uci_config_data_t * const config_data, uci_config_request_t * const request, bool const use_modified)
{
    config_status_t status;
    uci_config_request_t  req = *request;

    config_data->context_to_use = use_modified ? config_data->modified_context : config_data->default_context;

    if (!use_modified)
    {
        req.index = 1;
    }
    status = uci_config_lookup(config_data, &req);
    if (status == config_status_success)
        request->value = uci_config_extract_value(config_data);

    /* make sure we are back in modified directory, because all other operations are based on that */
    config_data->context_to_use = config_data->modified_context;

    return status;
}

static config_status_t uci_config_verify(uci_config_data_t const * const config_data, uci_config_request_t const * const request, uci_config_type_t const type)
{
    config_status_t status = config_status_invalid_parameter;

    if (config_data == NULL)
    {
        tlr_config_error("Null handle\n");
        goto error;
    }

    if (request == NULL)
    {
        tlr_config_error("Null request\n");
        goto error;
    }

    switch (type)
    {
        case uci_config_type_set:
            if (request->value == NULL)
            {
                tlr_config_error("Null value\n");
                break;
            }

            /* no break */
        case uci_config_type_get:
            if (request->option == NULL)
            {
                tlr_config_error("Null option\n");
                break;
            }

            /* no break */
        case uci_config_type_apply:
            if (request->section == NULL)
            {
                tlr_config_error("Null section\n");
                break;
            }

            /* no break*/
        case uci_config_type_revert:
        case uci_config_type_save:
            if (request->package == NULL)
                tlr_config_error("Null package\n");
            else
                status = config_status_success;
            break;
    }

error:
    return status;
}

/*
    create_uci_modified_config_file:
    Create a file with the name specified by config
    in the directory used to store 'modified' UCI values
    config: the name of the file to create
    Returns:
        0: success
        -1: error
*/
static int create_uci_modified_config_file( char const * const config )
{
    int result = -1;
    int fd;
    size_t const path_len = (sizeof config_modified_dir) + sizeof("/") + strlen( config ) + 1;
    char package_path[path_len];

    snprintf(package_path, path_len, "%s/%s", config_modified_dir, config);
    fd = open(package_path, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH);

    if (fd >= 0)
    {
        close(fd);
        result = 0;
    }

    return result;

}

/*
    uci_modified_config_file_exists:
    Check if the file indicated in config exists in the dircetory used to store
    'modified' UCI files
    Returns:
        1: file exists
        0: file doesn't exist
*/
static int uci_modified_config_file_exists( char const * const config )
{
    int result;
    size_t const path_len = (sizeof config_modified_dir) + sizeof("/") + strlen( config ) + 1;
    char package_path[path_len];
    struct stat buffer;

    snprintf(package_path, path_len, "%s/%s", config_modified_dir, config);
    result = stat( package_path, &buffer ) == 0;

    return result;

}

static config_status_t uci_config_create_section(uci_config_data_t * const config_data, uci_config_request_t const * const request)
{

    uci_config_request_t const request_without_option =
        {.package = request->package,
         .index = request->index,
         .section = request->section,
         .option = NULL};
    config_status_t status;

    /*
        The UCI library functions only seem to work on config files that already exist.
        Because of this, we'll check that the config file exists, and create it if necessary.
        Once that is done, calls to uci_config_lookup then seem to succeed even though the
        section may not exist. However, I think we can check the value of config_data->uci_data.s,
        and if it is NULL, I believe this means that the section doesn't exist, and we can then
        create it if this is the case.
        I believe that we can execute the code to create a new section even if it already exists,
        without issue, but we may as well only do it if we really need to.
    */
    if ( uci_modified_config_file_exists( request->package ) == 0 )
    {
        uci_debug( "need to create package file '%s'\n", request->package );
        if ( create_uci_modified_config_file( request->package ) < 0 )
        {
            tlr_config_debug( "error creating package file '%s'\n", request->package );
            status = config_status_file_not_found;
            goto error;
        }
    }

    status = uci_config_lookup(config_data, &request_without_option);
    /* shouldn't fail, but section may still not exist */
    if ( status != config_status_success )
    {
        tlr_config_debug( "unexpected config lookup error\n" );
        goto error;
    }

    if (config_data->uci_data.s == NULL)    /* indicates that the desired section doesn't exist */
    {
        /* if the section doesn't exist, we attempt to create it using another uci_set request */
        struct uci_ptr ptr_tmp;

#define uci_str_length 256
        static char uci_string[uci_str_length]; /* Note: needs static/heap buffer, because UCI updates this string
                                                   and its internal structure points to this for subsequent calls */

        uci_debug( "need to create section '%s%zd'\n", request->section, request->index );
        snprintf(uci_string, uci_str_length, "%s.%s%zd=%s", request->package, request->section, request->index, request->section);
    	if (uci_lookup_ptr(config_data->modified_context, &ptr_tmp, uci_string, true) == UCI_OK )
        {
            /* uci_set() will create the section */
            if ( uci_set( config_data->modified_context, &ptr_tmp ) == UCI_OK )
            {
                /* we now need to perform a uci_save operation to complete the creation of the section */
                if ( uci_save( config_data->modified_context, ptr_tmp.p ) == UCI_OK )
                {
                    status = config_status_success;
                }
                else
                {
                    tlr_config_debug( "uci_save failed\n" );
                }
            }
            else
            {
                tlr_config_debug( "uci_set failed\n" );
            }
   	    }
        else
        {
            tlr_config_debug( "uci lookup failed\n" );
        }
    }

error:
    return status;
}

config_status_t uci_config_init(void)
{
    config_status_t status = config_status_success;
    mode_t const dir_mode = 0777;
    int const result = mkdir(config_modified_dir, dir_mode);

    if ((result != 0) && (errno != EEXIST))
    {
        tlr_config_error("Failed to create %s\n", config_modified_dir);
        status = config_status_file_not_found;
    }

    return status;
}

void * uci_config_alloc(void)
{
    uci_config_data_t * config_data = malloc(sizeof *config_data);

    if (config_data == NULL)
    {
        tlr_config_error("Failed to allocate config_data\n");
        goto error;
    }

    config_data->modified_context = uci_alloc_context();
    config_data->default_context = uci_alloc_context();

    if ((config_data->modified_context == NULL) || (config_data->default_context == NULL))
    {
        tlr_config_error("Failed to allocate uci context\n");
        if (config_data->modified_context)
        	uci_free_context(config_data->modified_context);
        if (config_data->default_context)
        	uci_free_context(config_data->default_context);
        free(config_data);
        config_data = NULL;
        goto error;
    }

    uci_set_confdir(config_data->modified_context, config_modified_dir);
    uci_set_confdir(config_data->default_context, config_default_dir);
    config_data->context_to_use = config_data->modified_context;

error:
    return config_data;
}

void uci_config_free(void * const handle)
{
    if (handle != NULL)
    {
        uci_config_data_t * config_data = handle;

        uci_free_context(config_data->modified_context);
        uci_free_context(config_data->default_context);
        free(config_data);
    }
}

static int delete_option(uci_config_data_t * const config_data, char const * const value)
{
    int result = UCI_OK;

    if (config_data->uci_data.o != NULL)
    {
        struct uci_ptr * ptr = &config_data->uci_data;

        config_data->uci_data.value = value;
        switch(ptr->o->type)
        {
            case UCI_TYPE_LIST:
                uci_debug("delete: list %s\n", config_data->uci_data.value);
                result = uci_del_list(config_data->modified_context, &config_data->uci_data);
                break;

            case UCI_TYPE_STRING:
            default:
                uci_debug("delete: %s\n", config_data->uci_data.value);
                result = uci_delete(config_data->modified_context, &config_data->uci_data);
                break;
        }
    }

    return result;
}

static int delete_list_options(uci_config_data_t * const config_data)
{
    int result = UCI_OK;

    struct uci_ptr uci_data = config_data->uci_data;
    struct uci_ptr * const ptr = &uci_data;

    if (uci_data.o != NULL)
    {
        struct uci_element *e;
        struct uci_element *t;

        uci_foreach_element_safe(&ptr->o->v.list, t, e)
        {
            uci_debug("delete_list_option: %s\n", e->name);

            result = delete_option(config_data, e->name);
            if (result != UCI_OK)
                break;
        }
    }

    return result;
}

config_status_t uci_config_get_option(void * const handle, uci_config_request_t * const request)
{
    config_status_t status = uci_config_verify(handle, request, uci_config_type_get);
    bool const use_modified_dir = true;

    request->value = NULL;

    if (status != config_status_success) goto error;

    status = uci_config_read_option(handle, request, use_modified_dir);
#ifndef USE_DEFAULT_DEFAULT_VALUES
    if ((status == config_status_no_entry_exist) || ((status == config_status_success) && *request->value == 0))
        status = uci_config_read_option(handle, request, !use_modified_dir);
#else
    if ( request->value == NULL )   /* not found in modified directory */
        status = uci_config_read_option(handle, request, !use_modified_dir);
#endif

error:
    return status;
}

config_status_t uci_config_set_option(void * const handle, uci_config_request_t const * const request, bool const is_list)
{
    config_status_t status = uci_config_verify(handle, request, uci_config_type_set);
    uci_config_data_t * const config_data = handle;
    bool set_not_delete = true;
    int result;

    if (status != config_status_success) goto error;

    status = uci_config_create_section(config_data, request);
    if (status != config_status_success) goto error;

    config_data->context_to_use = config_data->default_context;

    {
        uci_config_request_t * const req = (uci_config_request_t * const)request; /* unconsting to use index 1 */
        size_t const index = request->index;

        req->index = 1;
        status = uci_config_lookup(config_data, req);
        req->index = index;
    }

    config_data->context_to_use = config_data->modified_context;
    if (status != config_status_success) goto error;

    if ((config_data->uci_data.o == NULL) && (*request->value == 0))
    {
        set_not_delete = false;
    }
    else if (config_data->uci_data.o != NULL)
    {
        struct uci_ptr * ptr = &config_data->uci_data;

        switch(ptr->o->type)
        {
            case UCI_TYPE_LIST:
                {
                    struct uci_element *e;
                    struct uci_element *t;
                    char const * value_ptr = request->value;
                    size_t default_element_count = 0;
                    size_t element_count = 0;

                    uci_foreach_element_safe(&ptr->o->v.list, t, e)
                        default_element_count++;

                    while (value_ptr != NULL)
                    {
                        char * p = strchr(value_ptr, ',');
                        size_t length = (p != NULL) ? (p - value_ptr) : strlen(value_ptr);
                        bool is_default_value = false;


                        uci_foreach_element_safe(&ptr->o->v.list, t, e)
                        {
                            if ((length == strlen(e->name)) && (memcmp(value_ptr, e->name, length) == 0))
                            {
                                is_default_value = true;
                                break;
                            }
                        }

                        if (!is_default_value)
                        {
                            set_not_delete = true;
                            break;
                        }
                        else
                        {
                            set_not_delete = false;
                        }

                        element_count++;
                        value_ptr = (p != NULL) ? p + 1 : p;
                    }

                    if (default_element_count > element_count)
                        set_not_delete = true;


                }
                break;

            case UCI_TYPE_STRING:
            default:
                if (strcmp(config_data->uci_data.o->v.string, request->value) == 0)
                    set_not_delete = false;
                break;
        }
    }

    status = uci_config_lookup(config_data, request);
    if (status != config_status_success) goto error;

    if (config_data->uci_data.o != NULL)
    {
        struct uci_ptr * ptr = &config_data->uci_data;

        switch(ptr->o->type)
        {
            case UCI_TYPE_LIST:
                {
                    struct uci_element *e;
                    struct uci_element *t;
                    uci_foreach_element_safe(&ptr->o->v.list, t, e)
                    {
                        uci_debug("Current value: list %s\n", e->name);
                    }
                }
                break;
            case UCI_TYPE_STRING:
            default:
                uci_debug("Current value: %s\n", config_data->uci_data.o ? config_data->uci_data.o->v.string : "NULL");
                break;
        }
    }

	if (set_not_delete)
	{
	    uci_debug("Setting package: %s index: %zd %s.%s = %s\n", request->package, request->index, request->section, request->option, request->value);

	    if (is_list)
	    {

	        char const * value_ptr = request->value;

            result = delete_list_options(config_data);

            while ((value_ptr != NULL) && (result == UCI_OK))
            {
                char new_value[CONFIG_MGR_NAME_LENGTH];
                char * p = strchr(value_ptr, ',');
                size_t length = (p != NULL) ? (p - value_ptr) : strlen(value_ptr);
                bool is_duplicate = false;

                config_data->uci_data.value = new_value;
                snprintf(new_value, sizeof new_value, "%.*s", length, value_ptr);
                //length++;  // add 1 for nul
                //strlcpy(new_value, value_ptr, length);

                if (config_data->uci_data.o != NULL)
                {
                    struct uci_ptr * ptr = &config_data->uci_data;
                    struct uci_element *e;
                    struct uci_element *t;
                    uci_foreach_element_safe(&ptr->o->v.list, t, e)
                    {
                        if (strcmp(new_value, e->name) == 0)
                        {
                            is_duplicate = true;
                            break;
                        }
                    }
                }

                if (!is_duplicate)
                {
                    uci_debug("setting package: list %s\n", config_data->uci_data.value);
                    result = uci_add_list(config_data->modified_context, &config_data->uci_data);
                }
                value_ptr = (p != NULL) ? (p + 1) : p;
            }

	    }
	    else
	    {
	        config_data->uci_data.value = *request->value ? request->value : " ";
	        result = uci_set(config_data->modified_context, &config_data->uci_data);
	    }

        if (result != UCI_OK)
        {
            tlr_config_error("Failed[%d] to set %s.%s = %s\n", result, request->section, request->option, request->value);
            status = config_status_set_error;
            goto error;
        }
	}
	else
	{
	    if (config_data->uci_data.o != NULL)
	    {
	        struct uci_ptr * ptr = &config_data->uci_data;

	        switch(ptr->o->type)
	        {
	            case UCI_TYPE_LIST:
	                uci_debug("delete LIST\n");
	                result = delete_list_options(config_data);
                    break;

	            case UCI_TYPE_STRING:
	            default:
	                uci_debug("delete: %s\n", config_data->uci_data.value);
	                //result = uci_delete(config_data->modified_context, &config_data->uci_data);
                    // tlr_config_debug("delete String\n");
	                result = delete_option(config_data, config_data->uci_data.value);
	                break;
	        }

	        if (result != UCI_OK && result != UCI_ERR_NOTFOUND)	// it's okay to try to delete a non-existant entry
	        {
	            tlr_config_error("Failed[%d] to delete %s.%s = %s\n", result, request->section, request->option, request->value);
	            status = config_status_set_error;
	            goto error;
	        }
	    }
	}

    if (request->commit)
    {
        /* TODO: status = uci_config_apply(config_data, request); */
        result = uci_commit(config_data->modified_context, &config_data->uci_data.p, false);
        if (result != UCI_OK)
        {
            tlr_config_error("Failed[%d] to commit %s.%s = %s\n", result, request->section, request->option, request->value);
            status = config_status_commit_error;
            goto error;
        }
    }

error:
    return status;
}

config_status_t uci_config_save(void * const handle, uci_config_request_t const * const request)
{
    config_status_t status = uci_config_verify(handle, request, uci_config_type_save);
    uci_config_data_t * config_data = handle;
    int result;

    if (status != config_status_success) goto error;

    result = uci_load(config_data->modified_context, request->package, NULL);
    if (result != UCI_OK)
    {
        tlr_config_error("Failed to load %s [%d]\n", request->package, result);
        status = config_status_no_entry_exist;
        goto error;
    }

    uci_debug("Committing %s\n", config_data->uci_data.p->path);
    result = uci_commit(config_data->modified_context, &config_data->uci_data.p, false);
    if (result != UCI_OK)
    {
        tlr_config_error("Failed to commit %s [%d]\n", request->package, result);
        status = config_status_commit_error;
        goto error;
    }

error:
    return status;
}

config_status_t uci_config_revert(void * const handle, uci_config_request_t const * const request)
{
    config_status_t status = uci_config_verify(handle, request, uci_config_type_revert);
    uci_config_data_t * config_data = handle;

    if (status != config_status_success) goto error;
    status = uci_config_lookup(config_data, request);
    if (status != config_status_success) goto error;

    uci_debug("Reverting [%s]\n", config_data->uci_data.p->path);

    {
        int const result = uci_revert(config_data->modified_context, &config_data->uci_data);

        if (result != UCI_OK)
        {
            tlr_config_error("Failed to revert %s [%d]\n", request->package, result);
            status = config_status_revert_error;
        }
    }

error:
    return status;
}

char * uci_get_option(char const * const package, char const * const section, size_t const instance, char const * const option)
{
    char * data = NULL;

    uci_config_request_t request =
    {
        .package = package,
        .section = section,
        .index = instance,
        .option = option
    };

    void * handle = uci_config_alloc();

    if (handle != NULL)
    {
        config_status_t status;

        status = uci_config_get_option(handle, &request);

        if (status == config_status_success)
        {
            asprintf(&data, "%s", request.value);
        }
        uci_config_free(handle);
    }
    return data;
}

