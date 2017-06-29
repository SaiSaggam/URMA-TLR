#ifndef __WIFI_LOG_H__
#define __WIFI_LOG_H__

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

#include <syslog.h>

#ifdef RADIO_24
#define WIFI_MONITOR_PROCESS_NAME "wifi_monitor"
#define WIFI_MONITOR_INTERFACE wifi_interface
#endif

#ifdef RADIO_5G
#define WIFI_MONITOR_PROCESS_NAME "wifi5g_monitor"
#define WIFI_MONITOR_INTERFACE wifi5g_interface
#endif

#if defined(UNIT_TEST)
#define wifi_monitor_alert(x...)
#define wifi_monitor_critical(x...)
#define wifi_monitor_error(x...)
#define wifi_monitor_warning(x...)
#define wifi_monitor_notice(x...)
#define wifi_monitor_info(x...)
#define wifi_monitor_debug(x...)
#else

#if WIFI_MONITOR_LOG_TO_CONSOLE
#include <stdio.h>
#define logfunc(l, x...) do { printf(WIFI_MONITOR_PROCESS_NAME ": %s: ", __func__ ); printf(x); } while (0);
#else
#define logfunc syslog
#endif

#define wifi_monitor_alert(x...)		do { logfunc(LOG_ALERT, x); } while (0)
#define wifi_monitor_critical(x...)	do { logfunc(LOG_CRIT, x); } while (0)
#define wifi_monitor_error(x...)		do { logfunc(LOG_ERR, x); } while (0)
#define wifi_monitor_warning(x...)	    do { logfunc(LOG_WARNING, x); } while (0)
#define wifi_monitor_notice(x...)	    do { logfunc(LOG_NOTICE, x); } while (0)
#define wifi_monitor_info(x...)		do { logfunc(LOG_INFO, x); } while (0)
#define wifi_monitor_debug(x...)		do { logfunc(LOG_DEBUG, x); } while (0)

#endif

#endif /* __WIFI_LOG_H__ */
