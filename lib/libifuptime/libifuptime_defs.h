#ifndef __LIBIFUPTIME_DEFS_H__
#define __LIBIFUPTIME_DEFS_H__

#if defined(_CUNIT_)
#define SYSTEM_UPTIME_FILENAME "/tmp/proc/uptime"
#define INTERFACE_UPTIME_BASE_DIRECTORY "/tmp"
#else
#define SYSTEM_UPTIME_FILENAME "/proc/uptime"
#define INTERFACE_UPTIME_BASE_DIRECTORY "/var/network/uptimes"
#endif

/* types */
typedef enum read_time_result_t
{
	read_time_ok,
	read_time_no_file = -1,
	read_time_error = -2
} read_time_result_t;

#endif /* __LIBIFUPTIME_DEFS_H__ */
