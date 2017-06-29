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
#ifndef __MONITOR_DEBUG_H__
#define __MONITOR_DEBUG_H__

#include <syslog.h>
#include <stdio.h>

/* MONITOR_DEBUG_PRINTF = 0 no monitor_debug_printf output
 * MONITOR_DEBUG_PRINTF = 1 route monitor_debug_printf to syslog with cellular_debug_loglevel variable
 * MONITOR_DEBUG_PRINTF = 2 route monitor_debug_printf to printf
 * MONITOR_DEBUG_PRINTF = 3 route all monitor_debug_printf, info, error, notice, & critical to fprintf
 *
 */
#define MONITOR_DEBUG_PRINTF    1


#if (MONITOR_DEBUG_PRINTF == 0)
#define monitor_debug_printf(x...)  {}

#elif (MONITOR_DEBUG_PRINTF == 1)
#include <tlr_print.h>
extern tlr_log_t cellular_debug_priority;
#define monitor_debug_printf(x...)      do { tlr_event_debug(cellular_debug_priority, x); } while (0)

#elif (MONITOR_DEBUG_PRINTF == 2)
#define monitor_debug_printf(x...)     do { fprintf(stdout, "%s: ", __func__); fprintf (stdout, x); } while (0)

#endif

#if (MONITOR_DEBUG_PRINTF == 3)
#define monitor_debug(x...)     do { fprintf(stderr, "%s: ", __func__); fprintf (stderr, x); } while (0)
#define monitor_error monitor_debug
#define monitor_info monitor_debug
#define monitor_notice monitor_debug
#define monitor_critical monitor_debug
#define monitor_debug_printf monitor_debug

#else
#define monitor_debug(x...)     do { syslog(LOG_DEBUG, x); } while (0)
#define monitor_error(x...)     do { syslog(LOG_ERR, x); } while (0)
#define monitor_info(x...)      do { syslog(LOG_INFO, x); } while (0)
#define monitor_notice(x...)      do { syslog(LOG_NOTICE, x); } while (0)
#define monitor_critical(x...)      do { syslog(LOG_CRIT, x); } while (0)
#endif

extern char const * cellular_state_strings[];
extern char const * cellular_message_strings[];
extern size_t const cellular_state_strings_count;
extern size_t const cellular_message_strings_count ;

void show_cellular_debug(size_t const instance);

#endif /* __MONITOR_DEBUG_H__ */
