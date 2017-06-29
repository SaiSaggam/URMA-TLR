/*****************************************************************************
 * Copyright (c) 2013 Digi International Inc., All Rights Reserved
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

#include "password.h"
#include "security.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "tlr_print.h"
#include "unittest_support.h"

STATIC_FUNCTION config_status_t set_hashed_password(void * const handle, uci_config_request_t * const request)
{
    config_status_t status;
    char hashed_password[MAX_HASHED_PW_LEN];
    int hash_type;

    if (request->value[0] == '\0') goto done;

    if (sscanf(request->value, "$%1d$", &hash_type) == 0)  /* already hashed? */
    {
        bool const success = salt_hash_password(request->value, NULL, hashed_password, sizeof hashed_password);

        if (!success)
        {
            status = config_status_system_error;
            goto error;
        }

        request->value = hashed_password;
    }

done:
    status = uci_config_set_option(handle, request, false);

error:
    return status;
}

STATIC_FUNCTION config_status_t set_clear_text_password(void * const handle, uci_config_request_t * const request)
{
    config_status_t status;
    char * new_option = NULL;

    if (asprintf(&new_option, "%s_clear", request->option) > 0)
    {
        request->option = new_option;
        status = uci_config_set_option(handle, request, false);
    }
    else
    {
        status = config_status_no_memory;
    }

    free(new_option);

    return status;
}

STATIC_FUNCTION password_scope_t get_system_scope(void)
{
    password_scope_t scope = password_scope_global;
    char value_str[sizeof "off"];
    config_status_t const status = tlr_get_element_value(config_system, 1, config_system_device_specific_passwords,
                                                         value_str, sizeof value_str,
                                                         config_mgr_element_type_string, config_mgr_access_level_read_only);

    if (status == config_status_success)
    {
        if (strcmp(value_str, "on") == 0)
            scope = password_scope_device;
    }

    return scope;
}

STATIC_FUNCTION config_status_t set_encrypted_password(void * const handle, uci_config_request_t * const request)
{
    config_status_t status;
    char password[MAX_HASHED_PW_LEN];
    password_scope_t const system_scope = get_system_scope();
    int scope_set;
    char const * clear_text;

    if (request->value[0] == '\0')
    {
        clear_text = "";
        goto done;
    }

    if (sscanf(request->value, PASSWORD_PREFIX_FORMAT, &scope_set) == 0)
    {
        bool const success = encrypt_password(system_scope, request->value, password, sizeof password);

        if (!success)
        {
            status = config_status_system_error;
            goto error;
        }

        clear_text = request->value;
        request->value = password;
    }
    else /* already encrypted? */
    {
        bool const success = decrypt_password(request->value, password, sizeof password);

        if (!success)
        {
            status = config_status_system_error;
            goto error;
        }

        clear_text = password;
    }

done:
    status = uci_config_set_option(handle, request, false);
    if (status != config_status_success)  goto error;

    request->value = clear_text;
    status = set_clear_text_password(handle, request);

error:
    return status;
}

config_status_t set_password(void * const handle, config_mgr_element_type_t const type, uci_config_request_t * const request)
{
    config_status_t status;

    if (request->value == NULL)
    {
        request->value = "";
    }

    switch (type)
    {
        case config_mgr_element_type_password:
            status = set_hashed_password(handle, request);
            break;

        case config_mgr_element_type_encpassword:
            status = set_encrypted_password(handle, request);
            break;

        default:
            status = config_status_invalid_parameter;
            break;
    }

    return status;
}
