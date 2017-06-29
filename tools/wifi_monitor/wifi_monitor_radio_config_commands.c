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

#include "wifi_monitor_radio_config_commands.h"
#include "tlr_popen.h"
#include "tlr_print.h"


void wifi_radio_no_security(char const * const interface_name, size_t const instance, wifi_common_elements_t * const wifi_configuration)
{
    tlr_system_command(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_debug(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_system_command(IWPRIVEXE "%s%zu set AuthMode=OPEN", interface_name, instance);
    tlr_debug(IWPRIVEXE "%s%zu set AuthMode=OPEN", interface_name, instance);
    tlr_system_command(IWPRIVEXE "%s%zu set EncrypType=NONE", interface_name, instance);
    tlr_debug(IWPRIVEXE "%s%zu set EncrypType=NONE", interface_name, instance);
    tlr_system_command(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_debug(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
}

void wifi_radio_wpa2_personal_security(char const * const interface_name, size_t const instance, wifi_common_elements_t * const wifi_configuration)
{
    tlr_system_command(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_debug(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_system_command(IWPRIVEXE "%s%zu set AuthMode=WPA2PSK", interface_name, instance);
    tlr_debug(IWPRIVEXE "%s%zu set AuthMode=WPA2PSK", interface_name, instance);
    tlr_system_command(IWPRIVEXE "%s%zu set EncrypType=AES", interface_name, instance);
    tlr_debug(IWPRIVEXE "%s%zu set EncrypType=AES", interface_name, instance);
    tlr_system_command(IWPRIVEXE "%s%zu set WPAPSK=%s", interface_name, instance, wifi_configuration->wifi_param_password);
    tlr_debug(IWPRIVEXE "%s%zu set WPAPSK=%s", interface_name, instance, wifi_configuration->wifi_param_password);
    tlr_system_command(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_debug(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
}

void wifi_radio_wpa_wpa2_personal_security(char const * const interface_name, size_t const instance, wifi_common_elements_t * const wifi_configuration)
{
    tlr_system_command(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_debug(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_system_command(IWPRIVEXE "%s%zu set AuthMode=WPAPSKWPA2PSK", interface_name, instance);
    tlr_debug(IWPRIVEXE "%s%zu set AuthMode=WPAPSKWPA2PSK", interface_name, instance);
    tlr_system_command(IWPRIVEXE "%s%zu set EncrypType=TKIPAES", interface_name, instance);
    tlr_debug(IWPRIVEXE "%s%zu set EncrypType=TKIPAES", interface_name, instance);
    tlr_system_command(IWPRIVEXE "%s%zu set WPAPSK=%s", interface_name, instance, wifi_configuration->wifi_param_password);
    tlr_debug(IWPRIVEXE "%s%zu set WPAPSK=%s", interface_name, instance, wifi_configuration->wifi_param_password);
    tlr_system_command(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
    tlr_debug(IWPRIVEXE "%s%zu set SSID=%s", interface_name, instance, wifi_configuration->wifi_param_ssid);
}

