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

#ifndef __IFUPTIME_LIB_H__
#define __IFUPTIME_LIB_H__

#include <stdint.h>
#include <sys/types.h>

typedef enum get_ifuptime_result_t
{
	ifuptime_ok,
	ifuptime_interface_down = -1,
	ifuptime_error = -2
} get_ifuptime_result_t;

get_ifuptime_result_t get_system_uptime(uint64_t * const uptime_in_seconds);
get_ifuptime_result_t get_ifuptime(char const * const interface_name, uint64_t * const uptime_in_seconds);
void format_ifuptime(uint64_t const input_seconds, char * const buf, size_t const bufsize);

#endif /*  __IFUPTIME_LIB_H__ */
