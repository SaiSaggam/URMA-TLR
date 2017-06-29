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
#include <errno.h>
#include <limits.h>

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"
#include "validate.h"

#include <stdlib.h>

config_status_t tlr_verify_access(config_mgr_access_t const access, config_mgr_operation_t const operation, config_mgr_access_level_t const user_access_level)
{
    config_status_t status = config_status_access_error;

    switch (operation)
    {
        case config_mgr_read_operation:
            switch (access)
            {
                case config_mgr_access_writeonly:   // temp bypass.. what's the point of a writeonly parameter...I think writeonly type will go.
                case config_mgr_access_readonly_super_readwrite:
                    if (user_access_level == config_mgr_access_level_super)
                        status = config_status_success;
                    break;
                case config_mgr_access_readwrite:
                case config_mgr_access_readonly:
                    status = config_status_success;
                    break;
                default:
                    break;
            }
            break;

        case config_mgr_write_operation:
            switch (access)
            {
                case config_mgr_access_readonly_super_readwrite:
                    switch (user_access_level) {
                        case config_mgr_access_level_super:
                            status = config_status_success;
                            break;
                        case config_mgr_access_level_read_only:
                        case config_mgr_access_level_read_write:
                        default:
                            status = config_status_access_error;
                            break;
                    }
                    break;
                case config_mgr_access_writeonly:
                case config_mgr_access_readwrite:
                    switch (user_access_level) {
                        case config_mgr_access_level_super:
                        case config_mgr_access_level_read_write:
                            status = config_status_success;
                            break;
                        case config_mgr_access_level_read_only:
                        default:
                            status = config_status_access_error;
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
    }

    return status;
}

config_status_t tlr_verify_range(config_mgr_valid_function_t validation, void * value_limit,
                        config_mgr_element_type_t const type, char const * const value)
{
    config_status_t status = config_status_success;

    if (validation == NULL) goto done;
    errno = 0;

    switch (type)
    {
        case config_mgr_element_type_password:
        case config_mgr_element_type_string:
        case config_mgr_element_type_encpassword:
        {
            config_mgr_valid_string_t validate = validation;
            config_mgr_element_string_limit_t * const limit_ptr = value_limit;
            size_t const min = (limit_ptr != NULL) ? limit_ptr->min_length_in_bytes : 0;
            size_t const max = (limit_ptr != NULL) ? limit_ptr->max_length_in_bytes : SIZE_MAX;

            // Allow empty strings in without any validation necessary
            if (*value!='\0' && !validate(value, min, max)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_uint32:
        {
            char *end_ptr = NULL;
            uint32_t const native_value = (uint32_t)strtoul(value, &end_ptr, 0);
            config_mgr_valid_uint32_t validate = validation;
            config_mgr_element_unsigned_integer_limit_t * const limit_ptr = value_limit;
            uint32_t const min = (limit_ptr != NULL) ? limit_ptr->min_value : 0;
            uint32_t const max = (limit_ptr != NULL) ? limit_ptr->max_value : UINT32_MAX;
            bool const invalid_input = ((end_ptr != NULL) && (*end_ptr != '\0'));

            if (invalid_input || ((native_value == ULONG_MAX || native_value == 0) && errno == ERANGE))
            {
                status = config_status_invalid_value;
            }
            else if (!validate(native_value, min, max))
            {
                status = config_status_invalid_value;
            }

            break;
        }

        case config_mgr_element_type_int32:
        {
            char *end_ptr = NULL;
            int32_t const native_value = (int32_t) strtol(value, &end_ptr, 0);
            config_mgr_valid_int32_t validate = validation;
            config_mgr_element_signed_integer_limit_t * const limit_ptr = value_limit;
            int32_t const min = (limit_ptr != NULL) ? limit_ptr->min_value : INT32_MIN;
            int32_t const max = (limit_ptr != NULL) ? limit_ptr->max_value : INT32_MAX;
            bool const invalid_input = ((end_ptr != NULL) && (*end_ptr != '\0'));

            if (invalid_input || ((native_value == LONG_MAX || native_value == LONG_MIN) && errno == ERANGE))
            {
                status = config_status_invalid_value;
            }
            else if (!validate(native_value, min, max))
            {
                status = config_status_invalid_value;
            }
            break;
        }

        case config_mgr_element_type_enum_multi:
        {
            config_mgr_valid_enum_multi_t validate = validation;
            config_mgr_element_enum_limit_t * limit_ptr = value_limit;

            if (!validate(value, limit_ptr)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_enum:
        {
            config_mgr_valid_enum_t validate = validation;
            config_mgr_element_enum_limit_t * limit_ptr = value_limit;
            if (!validate(value, limit_ptr)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_on_off:
        {
            config_mgr_valid_on_off_t validate = validation;

            if (!validate(value)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_boolean:
        {
            config_mgr_valid_boolean_t validate = validation;

            if (!validate(value)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_ipaddr:
        {
            config_mgr_valid_ipaddr_t validate = validation;

            if (!validate(value)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_fqdn:
        {
            config_mgr_valid_fqdn_t validate = validation;

            if (!validate(value)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_mac_addr:
        {
            config_mgr_valid_mac_addr_t validate = validation;

            if (!validate(value)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_counter32:
            /* Nothing to do */
            break;

        case config_mgr_element_type_counter64:
            /* Nothing to do */
            break;

        case config_mgr_element_type_index:
        {
            int32_t const native_value = (int32_t) strtol(value, NULL, 0);
            config_mgr_valid_int32_t validate = validation;
            config_mgr_element_signed_integer_limit_t * const limit_ptr = value_limit;
            int32_t const min = (limit_ptr != NULL) ? limit_ptr->min_value : INT32_MIN;
            int32_t const max = (limit_ptr != NULL) ? limit_ptr->max_value : INT32_MAX;

            if (!validate(native_value, min, max)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_interface:
        {
            config_mgr_valid_interface_t validate = validation;

            if (!validate(value)) status = config_status_invalid_value;
            break;
        }

        case config_mgr_element_type_none:
            tlr_config_debug("verify_range called for type config_mgr_element_type_none[%d]\n", type);
            status = config_status_invalid_value;
            goto done;
    }

    if (status != config_status_success)
    {
        tlr_config_debug("Value %s out of range for type [%d]\n", value, type);
    }

done:
    return status;
}
