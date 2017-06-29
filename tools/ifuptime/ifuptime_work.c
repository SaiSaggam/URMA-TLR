#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ifuptime_lib.h>

static void usage(char const * const appname)
{
	printf("Usage: %s <iface>\n", appname);
}

/*
	print out the uptime of an interface. The interface name should be given in argv[1]
	Works by looking for the existence of a file with the same name as the interface
	in the directory /var/network/uptimes.
	If the file doesn't exist, the interface is presumed down
	Otherwise, the file is expected to contain the system uptime in the same format as the
	/proc/uptime file. If there is some error reading this file, a -ve return value is used,
	else the interface uptime is calculated from the current uptime in
	/proc/uptime - /var/network/uptimes
*/
int if_uptime(int argc, char * argv[])
{
	int result = -1;

	if (argc > 1)
	{
		unsigned long long interface_uptime = 0;
		get_ifuptime_result_t iface_uptime_result;

		iface_uptime_result = get_ifuptime(argv[1], &interface_uptime);

		if (iface_uptime_result == ifuptime_ok)
		{
			char buf[100];

			format_ifuptime(interface_uptime, buf, sizeof buf);
			printf("%s\n", buf);

			result = 0;
		}
		else if (iface_uptime_result == ifuptime_interface_down)
		{
			printf("Down\n");

			result = 0;
		}
		else
			printf("Error\n");
	}
	else
	{
		usage(argv[0]);
	}

	return result;
}
