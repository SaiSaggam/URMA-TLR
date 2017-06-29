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

#include "wifi_radio_5g.h"
#include "config_mgr.h"
#include "interfaces.h"
#include "tlr_common.h"
#include "wifi_monitor.h"
#include "wifi_monitor_ifconfig_commands.h"
#include "tlr_popen.h"

#include <stdlib.h>

unsigned int wifi_radio_interface_count_from_config_5g(void)
{
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    return table->groups[config_wifi5g].instances;
}



bool wifi_radio_implement_config_change_5g(size_t const instance, wifi_common_elements_t * const wifi_configuration, bool * const connection_state)
{
    // TODO.. 5g config
    return false;
}

bool wifi_radio_connect_5g(size_t const instance)
{
    // TODO:
    return false;
}

bool wifi_radio_disconnect_5g(size_t const instance)
{
    // TODO:
    return false;
}


wifi_common_elements_t * wifi_get_configuration_5g(size_t const instance)
{
    // TODO:
    return NULL;
}


void wifi_free_configuration_5g(wifi_common_elements_t * const wifi_config)
{
    // TODO:
}
