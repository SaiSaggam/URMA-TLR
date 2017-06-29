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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <inttypes.h>

#include "ifuptime_lib.h"
#include "libifuptime_defs.h"

#if defined(_CUNIT_)
#define STATIC_FUNC
#else
#define STATIC_FUNC static
#endif


/* data */
static char const system_uptime_filename[] = SYSTEM_UPTIME_FILENAME;
static char const interface_uptime_base_directory[] = INTERFACE_UPTIME_BASE_DIRECTORY;

/* code */

/*
	Read the time in seconds from a file
	the file to read should be in the same format as /proc/uptime
	The 'time' value is read from the first field in the first line of the file
	If the file isn't present, this isn't necessarily an error as we use this as an indication that an interface is down.
	returns:
    read_time_ok: a numeric value was able to be read from the specified file
    read_time_no_file: the file was unable to be opened (we presume that it doesn't exist)
    read_time_error: some error occurred, or a NULL time parameter was passed
*/
STATIC_FUNC read_time_result_t read_time_from_file(char const * const filename, uint64_t *time)
{
	FILE * fp;
	char *line = NULL; // getline() mallocs this buffer
	size_t len;
	ssize_t read;
	read_time_result_t ret = read_time_error;

	if((fp = fopen( filename, "r" )) == NULL)
	{
		ret = read_time_no_file;
		goto done;
	}

	if ((read = getline(&line, &len, fp)) != -1)
	{
        if (sscanf(line, "%" PRIu64, time) != 1)
        {
			goto done;
        }
		ret = read_time_ok;
	}

done:
	free(line);

    if (fp != NULL)
    {
		fclose(fp);
    }

	return ret;
}

get_ifuptime_result_t get_system_uptime(uint64_t * const uptime_in_seconds)
{
    get_ifuptime_result_t result;
    uint64_t system_uptime;

	if (read_time_from_file(system_uptime_filename, &system_uptime) != read_time_ok)
	{
        result = ifuptime_error;
        goto done;
	}

    *uptime_in_seconds = system_uptime;
    result = ifuptime_ok;

done:
    return result;
}

get_ifuptime_result_t get_ifuptime(char const * const interface_name, uint64_t * const uptime_in_seconds)
{
	get_ifuptime_result_t result = ifuptime_error;
	uint64_t system_uptime = 0, interface_connect_time = 0;
	char interface_connect_time_filename_buffer[sizeof(interface_uptime_base_directory)+50];
	read_time_result_t iface_uptime_result;

	/* determine the name of the interface connect time file */
	snprintf(interface_connect_time_filename_buffer, sizeof interface_connect_time_filename_buffer, "%s/%s", interface_uptime_base_directory, interface_name);
	/* read the connect time from this file */
	iface_uptime_result = read_time_from_file(interface_connect_time_filename_buffer, &interface_connect_time);

	if (iface_uptime_result == read_time_ok)
	{
		/* read current system uptime */
		if (read_time_from_file(system_uptime_filename, &system_uptime) == read_time_ok)
		{
			/* and the difference is the uptime */
			*uptime_in_seconds = system_uptime - interface_connect_time;

			result = ifuptime_ok;
		}
	}
	else if (iface_uptime_result == read_time_no_file)
	{
		result = ifuptime_interface_down;
	}

	return result;
}

/* format the interface uptime in seconds into human readable form
   e.g. 1 minute, 53 seconds
*/
void format_ifuptime(uint64_t const input_seconds, char * const buf, size_t const bufsize)
{
	uint64_t work = input_seconds;
	unsigned int days, hours, minutes, seconds;

	days = input_seconds / (60 * 60 * 24);
	work -= days * (60 * 60 * 24);
	hours = work / (60 * 60);
	work -= hours * (60 * 60);
	minutes = work / 60;
	work -= minutes * 60;
	seconds = work;

	if (days > 0)
		snprintf(buf, bufsize, "%u Day%s, %u Hour%s, %u Minute%s, %u Second%s",
			   days,
			   days == 1 ? "" : "s",
			   hours,
			   hours == 1 ? "" : "s",
			   minutes,
			   minutes == 1 ? "" : "s",
			   seconds,
			   seconds == 1 ? "" : "s"
			   );
	else if (hours > 0)
		snprintf(buf, bufsize, "%u Hour%s, %u Minute%s, %u Second%s",
			   hours,
			   hours == 1 ? "" : "s",
			   minutes,
			   minutes == 1 ? "" : "s",
			   seconds,
			   seconds == 1 ? "" : "s"
			   );
	else if (minutes > 0)
		snprintf(buf, bufsize, "%u Minute%s, %u Second%s",
			   minutes,
			   minutes == 1 ? "" : "s",
			   seconds,
			   seconds == 1 ? "" : "s"
			   );
	else
		snprintf(buf, bufsize, "%u Second%s",
			   seconds,
			   seconds == 1 ? "" : "s"
			   );
}


