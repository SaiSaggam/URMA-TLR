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
#ifndef CELLULAR_DEF_H
#define CELLULAR_DEF_H

#include "cellular_status.h"
#include <sys/types.h>
#include <netinet/in.h>

bool is_usb_ready(int const fd, size_t const index_3g);
int get_usb_index(int const fd, size_t const index_3g, size_t * const index_usb);
int get_connect_state(int const fd, size_t const index_usb, cellular_connect_status_t * const state);
int get_interface_name(int const fd, size_t const index_usb, char * const name, size_t const name_length);
int get_model(int const fd, size_t const index_3g, char * const name, size_t const name_length);
int get_imei(int const fd, size_t const index_3g, char * const value, size_t const value_size);
int get_firmware_version(int const fd, size_t const index_3g, char * const value, size_t const value_size);
int get_ip_address(int const fd, size_t const index_usb, struct in_addr * ipaddr);
int get_netmask(int const fd, size_t const index_usb, struct in_addr * ipaddr);
int get_gateway(int const fd, size_t const index_usb, struct in_addr * ipaddr);
int get_primary_dns(int const fd, size_t const index_usb, struct in_addr * dns);
int get_secondary_dns(int const fd, size_t const index_usb, struct in_addr * dns);
int get_manufacturer(int const fd, size_t const index_3g, char * const name, size_t const name_length);
int get_hardware_version(int const fd, size_t const index_3g, char * const name, size_t const name_length);

int get_active_sim(int const fd, size_t const index_3g, size_t * const active_sim);
int set_active_sim(int const fd, size_t const index_3g, size_t const active_sim);

int get_rssi(int const fd, size_t const index_3g, int32_t * const value);
int get_ecio(int const fd, size_t const index_3g, int32_t * const value);

int get_registration_status(int const fd, size_t const index_3g, cellular_registration_status_t * const value);
int get_network_name(int const fd, size_t const index_3g, char * const name, size_t const name_length);
int get_mcc(int const fd, size_t const index_3g, unsigned int * const mcc);
int get_mnc(int const fd, size_t const index_3g, unsigned int * const mnc);
int get_connection_type(int const fd, size_t const index_3g, cellular_connection_type_t * const value);
int get_radio_band(int const fd, size_t const index_3g, char * const name, size_t const name_length);
int get_channel(int const fd, size_t const index_3g, unsigned int * const channel);
int get_temperature(int const fd, size_t const index_3g, int32_t * temperature);
int get_pdp_context_pid(int const fd, size_t const index_3g, uint32_t * const value);
int get_pdp_context_apn(int const fd, size_t const index_3g, char * const name, size_t const name_length);

#endif /* CELLULAR_DEF_H */
