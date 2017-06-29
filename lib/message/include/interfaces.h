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

#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include <config_def.h>

#define INTERFACE_MANAGER_INSTANCE 1

typedef enum interfaces_t
{
    first_valid_interface = 0,  /* tag to aid with unit testing */
    cellular_interface = first_valid_interface,
    eth_interface,
    dsl_interface,
    bridge_interface,
    wifi_interface,
    wifi5g_interface,
    vpn_interface,
    invalid_interface,
    last_valid_interface = invalid_interface - 1  /* tag to aid with unit testing */
} interfaces_t;

interfaces_t interface_get_type_from_name(char const * const name);
char const * interface_get_name_from_type(interfaces_t const type);
interfaces_t interface_get_type_from_config_group(config_group_t const config_group);
char const * interface_get_name_from_config_group(config_group_t const config_group);

#endif /* __INTERFACES_H__ */
