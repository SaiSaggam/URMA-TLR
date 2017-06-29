#include "resource_monitor.h"
#include <daemonize.h>

static int resource_monitor_main(void)
{
    return res_main(-1);
}

int main(int argc, char *argv[])
{
    return start_daemon(argc, argv, resource_monitor_main, false);
}

char *get_proc_stat_file_name(void)
{
	static char *proc_stat_file_name="/proc/stat";

	return proc_stat_file_name;
}

