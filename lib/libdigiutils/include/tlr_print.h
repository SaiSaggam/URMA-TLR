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

#ifndef __TLR_PRINT_H__
#define __TLR_PRINT_H__

#include <stdbool.h>
#include <syslog.h>
#include <sys/types.h>

typedef enum
{
    tlr_response_format_unknown,
    tlr_response_format_ascii,
    tlr_response_format_json,
} tlr_response_format_t;

#define TLR_FORMAT_ASCII  "ascii"
#define TLR_FORMAT_JSON   "json"

typedef enum
{
	tlr_log_emergency = LOG_EMERG,
	tlr_log_alert = LOG_ALERT,
	tlr_log_critical = LOG_CRIT,
	tlr_log_error = LOG_ERR,
	tlr_log_warning = LOG_WARNING,
	tlr_log_notice = LOG_NOTICE,
	tlr_log_info = LOG_INFO,
	tlr_log_debug = LOG_DEBUG,
	tlr_log_nolog
} tlr_log_t;

bool tlr_parse_args(int const argc, char * const args[], tlr_response_format_t * const format, char const ** const user);
bool tlr_printf(tlr_response_format_t const type, char const * const format, ...);
void tlr_event_log(tlr_log_t const priority, char const * const process_name, char const * const format, ...);
void tlr_event_debug(tlr_log_t const priority, char * fmt, ...);
tlr_response_format_t tlr_response_format_from_string(char const * const string);

#define tlr_debug(x...) do { syslog(LOG_DEBUG, x); } while (0)
#define tlr_error(x...) do { syslog(LOG_ERR, x); } while (0)
#define tlr_info(x...) do { syslog(LOG_INFO, x); } while (0)
#define tlr_notice(x...) do { syslog(LOG_NOTICE, x); } while (0)
#define tlr_critical(x...) do { syslog(LOG_CRIT, x); } while (0)

#endif /* __TLR_PRINT_H__ */
