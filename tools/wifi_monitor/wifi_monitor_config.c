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

#include "wifi_monitor_config.h"
#include "wifi_monitor_ifconfig_commands.h"
#include "wifi_monitor_log.h"
#include "wifi_monitor.h"
#include "wifi_monitor_event_handlers.h"
#include "str_replace_char.h"
#include "tlr_assert.h"
#include "wifi_radio.h"

#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>


void wifi_config_has_changed(wifi_context_st * const context, wifi_common_elements_t * const new_config)
{
    bool connection_state;

    if (new_config != NULL && wifi_radio_implement_config_change(context->instance, new_config, &connection_state))
    {
        if (connection_state)
        {
            wifi_handle_connected_event(context);
        }
        else
        {
            wifi_handle_disconnected_event(context);
        }
    }
}

bool wifi_config_check_for_change(size_t const instance, wifi_common_elements_t * const config, wifi_common_elements_t * const new_config)
{
    bool config_changed = false;

    if (config == NULL || new_config == NULL)
    {
        config_changed = true;
        goto done;
    }
    if (strcmp(config->wifi_param_ssid, new_config->wifi_param_ssid))
    {
        config_changed = true;
        goto done;
    }
    if (config->wifi_param_security != new_config->wifi_param_security)
    {
        config_changed = true;
        goto done;
    }
    if (strcmp(config->wifi_param_password, new_config->wifi_param_password))
    {
        config_changed = true;
        goto done;
    }

done:
    return config_changed;
}

