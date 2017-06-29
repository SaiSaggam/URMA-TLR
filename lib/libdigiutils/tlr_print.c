#include "tlr_print.h"
#include "config_mgr.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "strlcpy.h"

bool tlr_parse_args(int const argc, char * const argv[], tlr_response_format_t * const format, char const ** const user)
{
    optind = 1;
    opterr = 0;

#define MAX_USERNAME_LEN 33
    static char username[MAX_USERNAME_LEN]; // Not thread safe, but this API will (must) be called only once per process.
    bool result = true;
    int opt;

    if (format != NULL) *format = tlr_response_format_unknown;
    if (user != NULL) *user = NULL;

    while ((opt = getopt(argc, argv, "f:u:i:a:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            if (format == NULL) break;

            if (strcmp(optarg, TLR_FORMAT_ASCII) == 0)
                *format = tlr_response_format_ascii;
            else if (strcmp(optarg, TLR_FORMAT_JSON) == 0)
                *format = tlr_response_format_json;
            else
            {
                tlr_debug("%s: invalid format %s\n", argv[0], optarg);
                result = false;
            }
            break;

        case 'u':
            if (user != NULL)
            {
                strlcpy(username, optarg, sizeof username);
                *user = username;
            }
            break;

        case 'i': // Option not used at this time
            break;

        case 'a': // Option not used at this time
            break;

        default:
            tlr_debug("%s: invalid option %c\n", argv[0], (char)opt);
            result = false;
            break;
        }
    }

    return result;
}


bool tlr_printf(tlr_response_format_t const type, char const * const format, ...)
{
    bool result = true;
    va_list args;

    va_start(args, format);
    switch (type)
    {
    case tlr_response_format_ascii:
        result = (vprintf(format, args) >= 0);
        break;

    case tlr_response_format_json:
        /* @TODO: Create a json format */
        vsyslog(LOG_INFO, format, args);
        break;

    default:
        vsyslog(LOG_NOTICE, format, args);
        break;
    }
    va_end(args);

    fflush(stdout);

    return result;
}

void tlr_event_log(tlr_log_t const priority, char const * const process_name, char const * const format, ...)
{
    va_list args;

    va_start(args, format);
    openlog(process_name, 0, LOG_USER);
    vsyslog(priority, format, args);
    closelog();
    va_end(args);
}

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{
    if (priority != tlr_log_nolog)
    {
        va_list ap;

        va_start(ap, fmt);

        vsyslog(priority,  fmt, ap);

        va_end(ap);
    }
}

tlr_response_format_t tlr_response_format_from_string(char const * const string)
{
    tlr_response_format_t response_format = tlr_response_format_unknown;

    if (string == NULL)
    {
        goto done;
    }

    if (strcmp(string, TLR_FORMAT_ASCII) == 0)
    {
        response_format = tlr_response_format_ascii;
    }
    else if (strcmp(string, TLR_FORMAT_JSON) == 0)
    {
        response_format = tlr_response_format_json;
    }

done:
    return response_format;
}

