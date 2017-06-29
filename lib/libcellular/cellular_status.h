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
#ifndef CELLULAR_STATUS_H
#define CELLULAR_STATUS_H

#include <stdint.h>
#include <stdbool.h>
#include <syslog.h>

#include "config_mgr.h"

#define CELLULAR_INDEX_3G   0
#define CELLULAR_NOT_SUPPORTED  0
#define CELLULAR_SUPPORTED      1

#define CELLULAR_MANUFACTURER_LENGTH   32
#define CELLULAR_MODEL_LENGTH   32
#define CELLULAR_INTERFACE_NAME_LENGTH 16

/* CELLULAR_DEBUG = 0 no cellular_debug output
 * CELLULAR_DEBUG = 1 route cellular_debug to syslog
 * CELLULAR_DEBUG = 2 route cellular_debug to printf
 * CELLULAR_DEBUG = 3 route all cellular debug, info, error, notice, & critical to printf
 *
 */
#define CELLULAR_DEBUG    1
#define CELLULAR_PRINTF   0

#if (CELLULAR_PRINTF == 1)
#define cellular_printf(x...)     do { printf("%s: ", __func__); printf (x); } while (0)
#else
#define cellular_printf(x...)
#endif

#if (CELLULAR_DEBUG == 1)
#define cellular_debug(x...)      do { syslog(LOG_DEBUG, x); } while (0)

#elif (CELLULAR_DEBUG == 2)
#define cellular_debug(x...)     do { printf("%s: ", __func__); printf (x); } while (0)

#elif (CELLULAR_DEBUG != 3)
#define cellular_debug(x...)
#endif

#if (CELLULAR_DEBUG == 3)
//#define cellular_debug(x...)     do { printf("%s: ", __func__); printf (x); } while (0)
#define cellular_debug printf
#define cellular_error cellular_debug
#define cellular_info cellular_debug
#define cellular_notice cellular_debug
#define cellular_critical cellular_debug
#define cellular_printf cellular_debug

#else
#define cellular_error(x...)     do { syslog(LOG_ERR, x); } while (0)
#define cellular_info(x...)      do { syslog(LOG_INFO, x); } while (0)
#define cellular_notice(x...)      do { syslog(LOG_NOTICE, x); } while (0)
#define cellular_critical(x...)      do { syslog(LOG_CRIT, x); } while (0)
#endif


typedef enum {
    cellular_disconnected,
    cellular_connecting,
    cellular_connected,
    cellular_disconnecting
} cellular_connect_status_t;

typedef enum {
    cellular_module_unknown,
    cellular_module_HE910,
    cellular_module_MC7455
} cellular_module_type_t;

typedef enum {
    cellular_registration_status_idle,
    cellular_registration_status_registered,
    cellular_registration_status_search,
    cellular_registration_status_roaming
} cellular_registration_status_t;

typedef enum {
    cellular_connection_type_unknown,
    cellular_conenction_type_2g,
    cellular_conenction_type_3g,
    cellular_conenction_type_4g
} cellular_connection_type_t;

typedef struct {
    size_t id;
    char const * model;
    bool support_4g;
} cellular_modem_t;

int cellular_get_usb_index(int const fd, size_t const index_3g, size_t * const index_usb);
bool cellular_get_usb_ready(int const fd, size_t const index_3g);

char const * const cellular_get_model(int const fd, size_t const index_3g);
cellular_connect_status_t cellular_get_connection_status(int const fd, size_t const index_3g);
char * cellular_get_interface_name(int const fd, size_t const index_3g);
int cellular_get_active_sim(int const fd, size_t const index_3g, size_t * const active_sim);
int cellular_set_active_sim(int const fd, size_t const index_3g, size_t const active_sim);

int cellular_open_status(void);
void cellular_close_status(int fd);
int cellular_read_status(int const fd, size_t const index_3g, config_state_cellular_t const sid, void * value, size_t const value_size);

cellular_modem_t const * cellular_get_modem(int const fd, size_t const index_3g);
bool cellular_sim_ready(int const fd, size_t const index_3g);

uint32_t cellular_get_max_restart_count(int const fd, size_t const index_3g);
uint32_t cellular_get_fail_restart_count(int const fd, size_t const index_3g);

config_cellular_state_t get_cellular_state(size_t const instance);
int cellular_get_connection_type(int const fd, size_t const index_3g, cellular_connection_type_t * const value);
int cellular_get_rssi(int const fd, size_t const index_3g, int32_t * const value);
size_t cellular_get_switch_sim_card(int const fd, size_t const index_3g);

unsigned int cellular_get_pass_through_port_number(void);
bool cellular_is_pass_through_enabled(void);


#endif /* CELLULAR_STATUS_H */
