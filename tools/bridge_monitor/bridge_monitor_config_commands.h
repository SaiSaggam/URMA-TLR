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

#ifndef __BRIDGE_MONITOR_CONFIG_COMMANDS_H__
#define __BRIDGE_MONITOR_CONFIG_COMMANDS_H__

#include <stddef.h>
#include <stdbool.h>
#include <signal.h>

bool bridge_connect_interface(size_t const instance);
bool bridge_disconnect_interface(size_t const instance);
bool bridge_interface_set_mtu(size_t const instance, char const * const mtu_string);
bool bridge_interface_set_ip_address_and_mask(size_t const instance, char const * const ip_address_string, char const * const mask_string);
bool bridge_set_dns_servers(size_t const instance, char const * const dns1, char const * const dns2);
bool start_dhcp_client(size_t const instance, char const * const pid_filename);
void stop_dhcp_client(size_t const instance, pid_t pid, char const * const pid_filename);

#endif /* __BRIDGE_MONITOR_CONFIG_COMMANDS_H__ */
