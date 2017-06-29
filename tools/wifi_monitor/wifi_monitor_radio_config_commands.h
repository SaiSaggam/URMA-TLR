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

#ifndef __WIFI_MONITOR_RADIO_CONFIG_COMMANDS_H__
#define __WIFI_MONITOR_RADIO_CONFIG_COMMANDS_H__

#include "config_mgr.h"

#define     IWPRIVEXE   "/usr/sbin/iwpriv "

typedef struct
{
    char            * wifi_param_ssid;
    config_wifi_security_t  wifi_param_security;
    char            * wifi_param_password;

} wifi_common_elements_t;


void wifi_radio_no_security(char const * const interface_name, size_t const instance, wifi_common_elements_t * const wifi_configuration);
void wifi_radio_wpa2_personal_security(char const * const interface_name, size_t const instance, wifi_common_elements_t * const wifi_configuration);
void wifi_radio_wpa_wpa2_personal_security(char const * const interface_name, size_t const instance, wifi_common_elements_t * const wifi_configuration);


#endif /* __WIFI_MONITOR_RADIO_CONFIG_COMMANDS_H__ */
