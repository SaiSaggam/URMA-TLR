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
*****************************************************************************/

#ifndef __SHOW_IPSEC_WORK_H__
#define __SHOW_IPSEC_WORK_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define BASE_INDEX 	1

#define TIME_STRING_LEN 256
#define IP_ADDR_STRING_LEN 256
#define KEY_INFO_LEN 256
#define CIPHER_LEN 256
#define ESP_SA_LEN 256
#define NAME_LEN 16
#define MODP_LEN 64
#define BYTES_LEN 128

typedef struct tunnel_status
{
    char name[NAME_LEN];
    char * description;
    int  instance;
    bool is_tunnel_up;
    bool is_tunnel_enabled;
    char up_time[TIME_STRING_LEN];
    char peer_ip[IP_ADDR_STRING_LEN];
    char local_cidr_network[IP_ADDR_STRING_LEN];
    char remote_cidr_network[IP_ADDR_STRING_LEN];
    char key_negotiation[KEY_INFO_LEN];
    char rekeying_in[TIME_STRING_LEN];
    char ah_ciphers[CIPHER_LEN];
    char esp_ciphers[CIPHER_LEN];
    char renegotiating_in[TIME_STRING_LEN];
    char outbound_esp_sa[ESP_SA_LEN];
    char inbound_esp_sa[ESP_SA_LEN];
    char modp[MODP_LEN];
    char bytes_in[BYTES_LEN];
    char bytes_out[BYTES_LEN];
    int  ike_version;
} tunnel_status_t;

void tunnel_status_context_init(tunnel_status_t * ptr_tunnel, size_t instance);
void tunnel_status_context_cleanup(tunnel_status_t * ptr_tunnel);
bool read_all_tunnel_status_data(tunnel_status_t * ptr_tunnel);
int get_tunnel_status(tunnel_status_t * ptr_tunnel);
int get_admin_status(tunnel_status_t * ptr_tunnel);

int read_tunnel_info_from_ipsec_status(FILE * const fp, tunnel_status_t * ptr_tunnel);
int show_ipsec_main(int argc, char * argv[]);

#endif /*__SHOW_IPSEC_WORK_H__*/
