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

#ifndef __WIFI_RADIO_24_H__
#define __WIFI_RADIO_24_H__

#include "wifi_monitor_radio_config_commands.h"

#include <stddef.h>
#include <stdbool.h>

#define     WIFI_RADIO_CONFIG_GROUP_24     config_wifi

unsigned int wifi_radio_interface_count_from_config_24(void);
bool wifi_radio_implement_config_change_24(size_t const instance, wifi_common_elements_t * const wifi_configuration, bool * const connection_state);
bool wifi_radio_connect_24(size_t const instance);
bool wifi_radio_disconnect_24(size_t const instance);
wifi_common_elements_t * wifi_get_configuration_24(size_t const instance);
void wifi_free_configuration_24(wifi_common_elements_t * const wifi_config);

#endif /* __WIFI_RADIO_24_H__ */
