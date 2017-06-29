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
#ifndef __BRIDGE_DNS_H__
#define __BRIDGE_DNS_H__

#include <stddef.h>
#include <stdbool.h>

void bridge_refresh_dns_configuration(size_t const instance);
bool bridge_set_dns_servers(size_t const instance, char const * const dns1, char const * const dns2);
bool bridge_check_if_dhcp_client_needed(size_t const instance);
bool bridge_interface_set_dhcp_client_to_desired_state(size_t const instance, char const * desired_state);
bool bridge_interface_set_dhcp_client_off(size_t const instance);
void delete_dhcp_nameservers_file(size_t const instance);


#endif /* __BRIDGE_DNS_H__ */
