/**
 * @file unilog.h
 *
 * UniLog -- Unified Logging Mechanism.
 *
 * Environment Vairables:
 * 
 *   $DEBUG_LEVEL: to control which level of debug log to be kept
 *   $LOG_COLLECTOR: to select log collector, 0=unilog, 1=stdout, 2=stderr
 * 
 * LOG Write API:
 * 
 *   LOG_SETTAG(char *tag): set TAG of log
 *   LPRINTF(int PRI, ...): print log of PRI typed
 */

#ifndef _UNILOG_H

#define _UNILOG_H

#define _PRI(x,y)       ((x)*8+(y))

#define PRI_URGENCY     _PRI(1,0)   //Emergency: system is unusable
#define PRI_ALERT       _PRI(1,1)   //Alert: action must be taken immediately
#define PRI_CRITICAL    _PRI(1,2)   //Critical: critical conditions
#define PRI_ERROR       _PRI(1,3)   //Error: error conditions
#define PRI_WARNING     _PRI(1,4)   //Warning: warning conditions
#define PRI_NOTICE      _PRI(1,5)   //Notice: normal but significant condition
#define PRI_INFO        _PRI(1,6)   //Informational: informational messages
#define PRI_DEBUG       _PRI(1,7)   //Debug: debug-level messages
#define PRI_LOGIN       _PRI(2,0)   //Login: login messages
#define PRI_MOBILE_DATA_USAGE       _PRI(2,1)   //Mobile data usage messages

#define PRI_RLOG_CMD    _PRI(111,0) //for RingLog Syslogd to pass commands

#define PRI_DEBUG1      _PRI(115,7) //Debug-level 1 messages
#define PRI_DEBUG2      _PRI(116,7) //Debug-level 2 messages
#define PRI_DEBUG3      _PRI(117,7) //Debug-level 3 messages
#define PRI_DEBUG4      _PRI(118,7) //Debug-level 4 messages
#define PRI_DEBUG5      _PRI(119,7) //Debug-level 5 messages
#define PRI_DEBUG6      _PRI(120,7) //Debug-level 6 messages
#define PRI_DEBUG7      _PRI(121,7) //Debug-level 7 messages
#define PRI_DEBUG8      _PRI(122,7) //Debug-level 8 messages
#define PRI_DEBUG9      _PRI(123,7) //Debug-level 9 messages

#define PRI_U           PRI_URGENCY
#define PRI_A           PRI_ALERT
#define PRI_C           PRI_CRITICAL
#define PRI_E           PRI_ERROR
#define PRI_W           PRI_WARNING
#define PRI_N           PRI_NOTICE
#define PRI_I           PRI_INFO
#define PRI_D           PRI_DEBUG

#define PRI_D1          PRI_DEBUG1
#define PRI_D2          PRI_DEBUG2
#define PRI_D3          PRI_DEBUG3
#define PRI_D4          PRI_DEBUG4
#define PRI_D5          PRI_DEBUG5
#define PRI_D6          PRI_DEBUG6
#define PRI_D7          PRI_DEBUG7
#define PRI_D8          PRI_DEBUG8
#define PRI_D9          PRI_DEBUG9

#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL	0
#endif

#define _LC_UNILOG      0
#define _LC_STDOUT      1
#define _LC_STDERR      2
#define _LC_SYSLOG      3

#ifndef LOG_COLLECTOR
    #define LOG_COLLECTOR   _LC_STDERR
#endif

#if LOG_COLLECTOR == _LC_UNILOG

int unilog_settag(char *tag);
int unilog_printf(int pri, ...);

#define LOG_SETTAG(tag)     unilog_settag(tag)
#define LPRINTF(pri, ...)                       \
    if (pri < (((DEBUG_LEVEL) + 115) * 8)) {    \
        unilog_printf(pri, __VA_ARGS__);        \
    }

#elif LOG_COLLECTOR == _LC_SYSLOG

#include <syslog.h>

#define LOG_SETTAG(tag)     openlog(tag, 0, LOG_USER)
#define LPRINTF(pri, ...)                       \
    if (pri < (((DEBUG_LEVEL) + 115) * 8)) {    \
        syslog(pri, __VA_ARGS__);               \
    }

#elif LOG_COLLECTOR == _LC_STDOUT

#include <stdio.h>

#define LOG_SETTAG(tag)
#define LPRINTF(pri, ...)                       \
    if (pri < (((DEBUG_LEVEL) + 115) * 8)) {    \
        fprintf(stdout, __VA_ARGS__);           \
    }

#else

#include <stdio.h>

#define LOG_SETTAG(tag)
#define LPRINTF(pri, ...)                       \
    if (pri < (((DEBUG_LEVEL) + 115) * 8)) {    \
        fprintf(stderr, __VA_ARGS__);           \
    }

#endif

#endif //ifdef _UNILOG_H
