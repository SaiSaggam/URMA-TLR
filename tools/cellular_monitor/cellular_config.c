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
#include <stdlib.h>
#include "cellular_config.h"
#include "config_mgr.h"
#include "monitor_debug.h"


static config_status_t get_cellular_element_value(size_t const instance, size_t const element_id, void * value)
{
    char * buffer = NULL;
    config_status_t status = tlr_get_element_alloc_value(config_cellular, instance,
                                                         element_id,
                                                         (void **)&buffer, config_mgr_element_type_string,
                                                         config_mgr_access_level_super);

    if (status == config_status_success)
    {
        status = tlr_config_string_to_value(config_cellular, element_id, buffer, value);
        if (status != config_status_success)
        {
            monitor_debug_printf("Invalid cellular element %zd value %s. Error %d\n", element_id, buffer, status);
        }

    }
    free(buffer);
    return status;
}


static config_status_t get_cellular_preferred_mode(size_t const instance, config_cellular_preferred_mode_t * const preferred_mode)
{
    config_status_t const status = get_cellular_element_value(instance, config_cellular_preferred_mode, preferred_mode);

    if (status != config_status_success)
    {
        monitor_debug_printf("Unable to get cellular %zd preferred_mode status = %d\n", instance, status);
    }

    return status;
}


static config_status_t get_cellular_connection_attempts(size_t const instance, uint32_t * const connection_attempts)
{

    config_status_t const status = get_cellular_element_value(instance, config_cellular_connection_attempts, connection_attempts);

    if (status != config_status_success)
    {
        monitor_debug_printf("Unable to get cellular %zd apn status = %d\n", instance, status);
    }

    return status;
}


cellular_config_status_t get_cellular_data(size_t const instance, cellular_config_t * const config)
{
    #define CELLULAR_CONFIG_DATA_STATUS(x)    (0x01 << (x))

    cellular_config_status_t rc = cellular_config_no_changed;
    unsigned int error = 0;

    {
        config_cellular_preferred_mode_t preferred_mode;

        config_status_t const status = get_cellular_preferred_mode(instance, &preferred_mode);
        if (status == config_status_success)
        {
            if (config->preferred_mode != preferred_mode)
            {
                config->preferred_mode = preferred_mode;
                error |= CELLULAR_CONFIG_DATA_STATUS(cellular_config_changed);
            }
        }
    }

    get_cellular_connection_attempts(instance, &config->connection_attempts);

    if (error & CELLULAR_CONFIG_DATA_STATUS(cellular_config_changed))
        rc = cellular_config_changed;

    return rc;
}



