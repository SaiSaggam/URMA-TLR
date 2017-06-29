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

#ifndef __WIFI_RADIO_H__
#define __WIFI_RADIO_H__

/* for the purposes of unit testing the main monitor code, use the radio_24 functions. */
#if defined(RADIO_24) || defined(UNIT_TEST)
#include "wifi24/wifi_radio_24.h"
#define     RADIO   _24
#endif
#ifdef RADIO_5G
#include "wifi5g/wifi_radio_5g.h"
#define     RADIO   _5g
#endif

#define     WIFI_RADIO_DEFINE1(a,b) WIFI_RADIO_##a##b
#define     WIFI_RADIO_DEFINE(a,b)  WIFI_RADIO_DEFINE1(a,b)

#define     wifi_radio_call1(a,b)    wifi##a##b
#define     wifi_radio_call(a,b)   wifi_radio_call1(a,b)

// Calls to radio depending on the radio type we are compiling for.
#define     wifi_radio_interface_count_from_config  wifi_radio_call(_radio_interface_count_from_config,RADIO)
#define     wifi_radio_implement_config_change  wifi_radio_call(_radio_implement_config_change,RADIO)
#define     WIFI_RADIO_CONFIG_GROUP      WIFI_RADIO_DEFINE(CONFIG_GROUP,RADIO)
#define     wifi_radio_connect          wifi_radio_call(_radio_connect,RADIO)
#define     wifi_radio_disconnect          wifi_radio_call(_radio_disconnect,RADIO)
#define     wifi_get_configuration      wifi_radio_call(_get_configuration,RADIO)
#define     wifi_free_configuration     wifi_radio_call(_free_configuration,RADIO)

#endif /* __WIFI_RADIO_H__ */
