#ifndef __FAILOVER_DEBUG_H__
#define __FAILOVER_DEBUG_H__

#ifndef FAILOVER_DEBUG
#define FAILOVER_DEBUG 0
#endif

#define FAILOVER "failover"

#ifndef _CUNIT_
#if FAILOVER_DEBUG
#define failover_debug(x...)      do { printf(FAILOVER": " x); } while (0)
#define failover_error(x...)      do { printf(FAILOVER": " x); } while (0)
#define failover_info(x...)       do { printf(FAILOVER": " x); } while (0)
#define failover_warning(x...)    do { printf(FAILOVER": " x); } while (0)
#else
#include <tlr_print.h>
extern tlr_log_t failover_debug_priority;
#define failover_debug(x...)      do { tlr_event_debug(failover_debug_priority, FAILOVER ": " x); } while (0)
#define failover_error(x...)      do { tlr_event_log(tlr_log_error, FAILOVER, x); } while (0)
#define failover_info(x...)       do { tlr_event_log(tlr_log_info, FAILOVER, x); } while (0)
#define failover_warning(x...)    do { tlr_event_log(tlr_log_warning, FAILOVER, x); } while (0)
#endif

#else
#if FAILOVER_DEBUG
#define failover_debug(x...)      do { printf(FAILOVER": " x); } while (0)
#define failover_error(x...)      do { printf(FAILOVER": " x); } while (0)
#define failover_info(x...)       do { printf(FAILOVER": " x); } while (0)
#define failover_warning(x...)    do { printf(FAILOVER": " x); } while (0)
#else
#define failover_debug(x...)
#define failover_error(x...)
#define failover_info(x...)
#define failover_warning(x...)
#endif

#endif


#endif /* __FAILOVER_DEBUG_H__ */
