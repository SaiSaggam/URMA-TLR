/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
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
****************************************************************************/

#ifndef __IP_HELPER_H__
#define __IP_HELPER_H__

#include <stdbool.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdint.h>

#define NOT_ASSIGNED_STRING "Not Assigned"
#define IP_ADDRESS_SIZE     sizeof "000.000.000.000"

typedef enum {
 ip_rx_packets,
 ip_tx_packets,
 ip_rx_bytes,
 ip_tx_bytes
} ip_statistics_item_t;

bool get_ifr(char const * const ifname, int request, struct ifreq * const ifr);
void get_ipaddr (char const * const ifname, char * const buf, size_t const bufsize );
void get_hwaddr (char const * const ifname, char * const buf, size_t const bufsize );
void get_newmaskaddr (char const * const ifname, char * const buf, size_t const bufsize );
void get_destaddr (char const * const ifname, char * const buf, size_t const bufsize );
void get_mtu (char const * const ifname, int * const buf);
int get_rx_packets(char const * const interface, uint64_t * const value);
int get_tx_packets(char const * const interface, uint64_t * const value);
int get_rx_bytes(char const * const interface, uint64_t * const value);
int get_tx_bytes(char const * const interface, uint64_t * const value);

char const * tlr_get_mac_address(char const * const interface_name);
char * tlr_get_sys_class_net(char const * const interface_name, char const * const object);

#endif // __IP_HELPER_H__
