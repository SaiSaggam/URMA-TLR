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

#ifndef __DSL_LOG_H__
#define __DSL_LOG_H__

#include <syslog.h>

#define DSL_PROCESS_NAME "dsl_monitor"

// DSL_LOG_MODE = 0: discard all messages
// DSL_LOG_MODE = 1: send all messages to syslog (normal operation)
// DSL_LOG_MODE = 2: send all messages to console (during development)
#ifndef DSL_LOG_MODE
#define DSL_LOG_MODE 1
#endif

#if DSL_LOG_MODE == 0
#define logfunc(l, x...)
#elif DSL_LOG_MODE == 1
#define logfunc syslog
#elif DSL_LOG_MODE == 2
#define logfunc(l, x...) printf(DSL_PROCESS_NAME ": " x)
#else
#error DSL_LOG_MODE is set to an unsupported value
#endif

#define dsl_alert(x...)		do { logfunc(LOG_ALERT, x); } while (0)
#define dsl_critical(x...)	do { logfunc(LOG_CRIT, x); } while (0)
#define dsl_error(x...)		do { logfunc(LOG_ERR, x); } while (0)
#define dsl_warning(x...)	do { logfunc(LOG_WARNING, x); } while (0)
#define dsl_notice(x...)	do { logfunc(LOG_NOTICE, x); } while (0)
#define dsl_info(x...)		do { logfunc(LOG_INFO, x); } while (0)
#define dsl_debug(x...)		do { logfunc(LOG_DEBUG, x); } while (0)

#endif // __DSL_LOG_H__
