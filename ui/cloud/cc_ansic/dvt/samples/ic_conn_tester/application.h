/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "app_debug.h"


#define APP_NAME "IC_Conn_Tester"

#define CONNECTOR_MAC_ADDR_LENGTH   6
#define CONNECTOR_VENDOR_ID_LENGTH  4
#define APP_STABLE_TIME         60
#define APP_CONNECT_FAIL_CNT    3
#define DEVICE_WAIT_COUNT       3

typedef enum 
{
    missed_ka_id,
    missed_ka_time_id,
    open_conn_id,
    open_conn_fail_id,
    closed_conn_id,
    dns_error_id,
    ping_failed_id,
    close_reason_id,
    error_status_id

}app_stats_id_t;

extern unsigned long app_get_stats(app_stats_id_t const id);
extern void app_set_stats(app_stats_id_t const id, unsigned long val);
extern void app_inc_stats(app_stats_id_t const id);

extern bool application_get_ip_address(uint8_t ** const ip_address, size_t * const size);
extern bool application_get_mac_addr(uint8_t ** const addr, size_t * const size);
extern bool application_get_vendor_id(uint8_t const ** id, size_t * const size);
extern bool application_get_cloud_url(char const ** url, size_t * const size);

extern bool application_init(int argc, char **argv);
extern void application_cleanup(void);
extern bool application_check_connection(void);

extern int  app_dns_resolve_name(char const * const domain_name, unsigned long * ip_addr);
extern char const * app_status_error_to_string(connector_status_t const value);
extern void app_os_status(connector_os_status_type_t const * const status);
extern void print_test_result(int result);

#endif
