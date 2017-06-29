#include "probing.h"
#include "failover_debug.h"
#include "tlr_popen.h"
#include <tlr_common.h>
#include <tlr_print.h>
#include <create_worker_thread.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unittest_support.h>

#define FAILED_WORKER_THREAD_CREATE_TRIES 3
#define FAILED_WORKER_THREAD_RETRY_DELAY_MILLISECONDS 1
#define PING_EXECUTABLE "/bin/ping"
#define LINEBUF_LEN 256 /* maximum expected line length read from ping command */
#define SUCCESSFUL_PING_STRING "1 packets received"

struct probe_context_st
{
    pthread_t thread_id;
};

/* a structure of this type is passed to the worker thread */
typedef struct probe_info_st
{
    char const * host;
    char const * interface;
    size_t size;
    uint32_t probe_id;
    void (*success_cb)(void * arg, uint32_t probe_id);
    void * arg;
    size_t timeout_seconds;
} probe_info_st;

static void free_probe_context(probe_context_st * probe_context)
{
    free(probe_context);
}

static void free_probe_info(probe_info_st * probe_info)
{
    free((void *)probe_info->host);
    free((void *)probe_info->interface);
    free(probe_info);
}

/*
 * read_file_until_string_found
 * read a file until a line is read that contains search_string
 * returns:
 *  true: if a line witht he search string was found
 *  false: otherwise
 */
STATIC_FUNCTION bool read_until_line_contains_string(FILE * fp, char const * const search_string)
{
    bool string_found;
    char * linebuf = NULL;
    size_t n = 0;

    while (getline(&linebuf, &n, fp) > 0)
    {
        if (linebuf != NULL && strstr(linebuf, search_string) != NULL)
        {
            string_found = true;
            goto done;
        }
    }
    string_found = false;

done:
    free(linebuf);
    return string_found;
}

static void * send_ping_thread(void * const pv)
{
    bool probe_succeeded;
    probe_info_st * probe_info = pv;
    char * cmd = NULL;
    FILE * fp = NULL;

    /* call 'ping' and wait a while for a response. If we get a response, call the callback, else
     * don't call the callback
     */

    failover_debug("PING: %s size: %zd interface: %s\n", probe_info->host, probe_info->size, probe_info->interface);
    if (asprintf(&cmd,
                 PING_EXECUTABLE " -I %s -c 1 -W %zd -s %zd %s",
                 probe_info->interface,
                 probe_info->timeout_seconds,
                 probe_info->size,
                 probe_info->host) < 0)
    {
        probe_succeeded = false;
        goto done;
    }
    fp = tlr_popen(cmd, "r");
    if (fp == NULL)
    {
        probe_succeeded = false;
        goto done;
    }

    if (read_until_line_contains_string(fp, SUCCESSFUL_PING_STRING))
    {
        probe_succeeded = true;
        goto done;
    }

    probe_succeeded = false;

done:
    failover_debug("PING result: %s\n", probe_succeeded ? "success" : "failure");
    if (probe_succeeded && probe_info->success_cb != NULL)
    {
        /* call the user suppplied success callback */
        probe_info->success_cb(probe_info->arg, probe_info->probe_id);
    }
    if (fp != NULL)
    {
        tlr_pclose(fp);
    }

    free(cmd);
    free_probe_info(probe_info);

    pthread_exit(NULL);

}

probe_context_st * send_probe(char const * const host, char const * const interface, uint32_t probe_id, size_t const size, void (*success_cb)(void * arg, uint32_t probe_id), void * const arg, size_t timeout_seconds)
{
    probe_context_st * probe_context;
    probe_info_st * probe_info;

    probe_context = calloc(1, sizeof *probe_context);
    if (probe_context == NULL)
    {
        failover_debug("memory shortage\n");
        goto done;
    }

    probe_info = calloc(1, sizeof *probe_info);
    if (probe_info == NULL)
    {
        free_probe_context(probe_context);
        probe_context = NULL;
        failover_debug("memory shortage\n");
        goto done;
    }

    probe_info->host = strdup(host);
    probe_info->interface = strdup(interface);
    probe_info->probe_id = probe_id;
    probe_info->size = size;
    probe_info->success_cb = success_cb;
    probe_info->arg = arg;
    probe_info->timeout_seconds = timeout_seconds;

    if (probe_info->host == NULL || probe_info->interface == NULL)
    {
        free_probe_info(probe_info);
        free_probe_context(probe_context);
        probe_context = NULL;
        failover_debug("memory shortage\n");
        goto done;
    }

    if (create_worker_thread(&probe_context->thread_id,
                             true,
                             FAILED_WORKER_THREAD_CREATE_TRIES,
                             FAILED_WORKER_THREAD_RETRY_DELAY_MILLISECONDS,
                             send_ping_thread,
                             probe_info) != 0)
    {
        free_probe_info(probe_info);
        free_probe_context(probe_context);
        probe_context = NULL;
        failover_debug("failed to create interface probing thread\n");
        goto done;
    }


done:
    return probe_context;
}

void free_probe(probe_context_st * probe_context)
{
    if (probe_context != NULL)
    {
        free_probe_context(probe_context);
    }
}
