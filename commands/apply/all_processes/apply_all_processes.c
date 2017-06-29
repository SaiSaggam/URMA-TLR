#include "digiutils.h"
#include "config_mgr.h"
#include "tlr_common.h"
#include "lr54leds.h"
#include "tlr_print.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define APPLY_ALL_PROCESSES_LOG_ID "apply_all_processes"

typedef struct daemons_st
{
    char const * const name;
} daemons_st;

#define PKILL_EXECUTABLE "/usr/bin/pkill"

static char const daemon_executable_prefix[] = "/usr/bin/";

static daemons_st const daemons[] =
{
    {
        .name = "cellular_monitor"
    },
    {
        .name = "eth_monitor"
    },
    {
        .name = "dsl_monitor"
    },
    {
        .name = "vpn_monitor"
    },
    {
        .name = "bridge_monitor"
    },
    {
        .name = "wifi_monitor"
    },
    {
        .name = "wifi5g_monitor"
    },

    /* all monitors should start before interface manager */
    /* XXX It shouldn't really matter which order the monitors/interface manager are started in.
     * They should be able to cope no matter the order.
     * There may be times (esp. during debugging) when monitors are started at any time, and they
     * should function correctly. Also, imagine the case where one of these daemons crashes and is
     * restarted by some future watchdog application.
     */
    {
        .name = "ifmanager"
    }
};
#define NUM_DAEMONS ARRAY_SIZE(daemons)

static void start_daemon(char const * const daemon_name)
{
    int const start_command_result = system_command("%s%s -d", daemon_executable_prefix, daemon_name);

    if (start_command_result != 0)
    {
        tlr_error("Error (%d) starting daemon '%s'\n", start_command_result, daemon_name);
    }
}

static void stop_daemon(char const * const daemon_name)
{
    int const stop_command_result = system_command(PKILL_EXECUTABLE " %s%s", daemon_executable_prefix, daemon_name);

    if (stop_command_result != 0)
    {
        tlr_error("Error (%d) stopping daemon '%s'\n", stop_command_result, daemon_name);
    }
}

static void start_stop_daemons(bool const start_daemons)
{
    size_t index;

    for (index = 0; index < NUM_DAEMONS; index++)
    {
        daemons_st const * const daemon = &daemons[index];
        char const * const daemon_name = daemon->name;

        if (start_daemons)
        {
            start_daemon(daemon_name);
        }
        else
        {
            stop_daemon(daemon_name);
        }
    }
}


static int start_all_processes(void)
{
    config_status_t status;

    start_stop_daemons(true);

    status = tlr_run_all_apply_processes();

    if (status != config_status_success)
    {
        tlr_error("Unable to run all apply processes (%d)\n", status);
    }

    return 0;
}

static int stop_all_processes(void)
{
    start_stop_daemons(false);

    /* Need to turn off power LED? */

    return 0;
}

int main(int argc, char * argv[])
{
    int result = EXIT_FAILURE;

    openlog(APPLY_ALL_PROCESSES_LOG_ID, 0, LOG_USER);

    switch (argc)
    {
        case 2:
            if (strcmp(argv[1], "stop") == 0)
            {
                if (stop_all_processes() == 0)
                {
                    result = EXIT_SUCCESS;
                }
                break;
            }
            else if (strcmp(argv[1], "start") == 0)
            {
                if (start_all_processes() == 0)
                {
                    result = EXIT_SUCCESS;
                }
                break;
            }
            /* no break; */
        default:
        {
            char const * const progname = basename(argv[0]);

            fprintf(stderr, "Invalid argument\n");
            fprintf(stderr, "Usage: %s [start | stop]\n", progname);
            break;
        }
    }

    closelog();

    return result;
}
