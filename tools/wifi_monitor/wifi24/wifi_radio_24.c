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

#include "wifi_radio_24.h"
#include "config_mgr.h"
#include "interfaces.h"
#include "tlr_common.h"
#include "wifi_monitor.h"
#include "wifi_monitor_ifconfig_commands.h"
#include "tlr_popen.h"

#include <stdlib.h>


static char     const * const interface_name_24 = "wifi";


void wifi_free_configuration_24(wifi_common_elements_t * const wifi_config)
{
    if (wifi_config != NULL)
    {
        free(wifi_config->wifi_param_ssid);
        free(wifi_config->wifi_param_password);
        free(wifi_config);
    }
}


wifi_common_elements_t * wifi_get_configuration_24(size_t const instance)
{
    wifi_common_elements_t * wifi_config = calloc(1, sizeof *wifi_config);
    config_status_t status;
    bool    have_got_configuration;

    if (wifi_config == NULL)
    {
        have_got_configuration = false;
        goto error;
    }

    status = tlr_get_element_alloc_value(config_wifi,
                                         instance,
                                         config_wifi_ssid,
                                         (void **)&wifi_config->wifi_param_ssid,
                                         config_mgr_element_type_string,
                                         config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        have_got_configuration = false;
        goto error;
    }


    status = tlr_get_element_value(config_wifi,
                                         instance,
                                         config_wifi_security,
                                         (void *)&wifi_config->wifi_param_security,
                                         sizeof wifi_config->wifi_param_security,
                                         config_mgr_element_type_enum,
                                         config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        have_got_configuration = false;
        goto error;
    }

    status = tlr_get_element_alloc_value(config_wifi,
                                         instance,
                                         config_wifi_password,
                                         (void **)&wifi_config->wifi_param_password,
                                         config_mgr_element_type_password,  // type config_mgr_element_type_string or config_mgr_element_type_encpassword
                                                                            // would return the encrypted version.
                                         config_mgr_access_level_super);
    if (status != config_status_success)
    {
        have_got_configuration = false;
        goto error;
    }

    have_got_configuration = true;
error:
    if (!have_got_configuration)
    {
        wifi_free_configuration_24(wifi_config);
        wifi_config = NULL;
    }
    return wifi_config;
}


static bool wifi_radio_configure_24(size_t const instance, wifi_common_elements_t * const wifi_configuration)
{
    // Returns true if AP can be considered to be connected or UP (for an AP this is when it is possible for a client to be
    // able to connect.)
    bool    configured = false;
    wifi_context_st * const context = wifi_get_context(instance);

    if (wifi_configuration == NULL || context == NULL)
    {
        configured = false;
        goto done;
    }

    if (context->desired_state == wifi_state_connected && wifi_configuration->wifi_param_ssid[0] != '\0')
    {
        wifi_connect_interface(instance);
        switch (wifi_configuration->wifi_param_security)
        {
            case config_wifi_security_none:
                wifi_radio_no_security(interface_name_24, instance, wifi_configuration);
                configured = true;
                break;
            case config_wifi_security_wpa2_personal:
                if (wifi_configuration->wifi_param_password[0] != '\0')
                {
                    wifi_radio_wpa2_personal_security(interface_name_24, instance, wifi_configuration);
                    configured = true;
                }
                else
                {
                    wifi_disconnect_interface(instance);
                    configured = false;
                }
                break;
            case config_wifi_security_wpa_wpa2_personal:
                if (wifi_configuration->wifi_param_password[0] != '\0')
                {
                    wifi_radio_wpa_wpa2_personal_security(interface_name_24, instance, wifi_configuration);
                    configured = true;
                }
                else
                {
                    wifi_disconnect_interface(instance);
                    configured = false;
                }
                break;
        }
    }
    else
    {
        wifi_disconnect_interface(instance);
        configured = false;
    }

done:
    return configured;
}



unsigned int wifi_radio_interface_count_from_config_24(void)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    return table->groups[config_wifi].instances;
}


bool wifi_radio_implement_config_change_24(size_t const instance, wifi_common_elements_t * const wifi_configuration, bool * const connection_state)
{
    // return true if possible reconfiguration has taken place
    bool reconfigured_wifi = false;

    if (wifi_configuration == NULL)
    {
        goto done;
    }

    *connection_state = wifi_radio_configure_24(instance, wifi_configuration);
    reconfigured_wifi = true;
done:
    return reconfigured_wifi;

}

static bool reconfigure_24(size_t const instance)
{
    bool    return_value;
    wifi_common_elements_t     * const wifi_configuration = wifi_get_configuration_24(instance);
    return_value = wifi_radio_configure_24(instance, wifi_configuration);
    wifi_free_configuration_24(wifi_configuration);
    return return_value;
}


bool wifi_radio_connect_24(size_t const instance)
{
    return reconfigure_24(instance);
}


bool wifi_radio_disconnect_24(size_t const instance)
{
    return reconfigure_24(instance);
}


