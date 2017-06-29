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
****************************************************************************/

#ifndef _SHOW_LAN_WORK_H_
#define _SHOW_LAN_WORK_H_


#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

static char fake_ip_address[] = "000.000.000.000";
#define LARGEST_DHCP_STRING     "off"

typedef struct {
    size_t instance;
    char const * description;
    char const * interfaces;
    char const * mac_address;
    int mtu;
    char ip_address[sizeof fake_ip_address];
    char mask[sizeof fake_ip_address];
    char dhcp[sizeof LARGEST_DHCP_STRING];
    char dns1[sizeof fake_ip_address];
    char dns2[sizeof fake_ip_address];
    uint64_t rx_packets;
    uint64_t tx_packets;

    uint64_t rx_bytes;
    uint64_t tx_bytes;

    bool admin_status;
    bool oper_status;

} lan_status_t;

int show_lan_work (int argc, char *argv[]);

#if defined(UNIT_TEST)
void get_lan_status(int const instance, lan_status_t * const lan);
#endif

#endif /* _SHOW_LAN_WORK_H_ */

