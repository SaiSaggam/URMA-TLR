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
#include "tlr_common.h"
#include "tlr_print.h"
#include "show_parse_helper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#define SHOW_FIREWALL "show_firewall"

typedef struct {
    char * name;
    char * title;
} firewall_table_t;

static firewall_table_t const firewall_tables[] = {
        {"filter", "Filter"},
        {"mangle", "Raw"},
        {"nat", "NAT"}
};

static void show_usage(char const * const progname)
{
    size_t i;
    size_t const max_count = ARRAY_SIZE(firewall_tables);
    fprintf(stderr, "Usage: %s [-i <instance>] [-f <json|ascii>] [", progname);
    for (i=0; i < ARRAY_SIZE(firewall_tables); i++)
    {
        fprintf(stderr, "%s%c", firewall_tables[i].name, (i < (max_count -1)) ? '|' : '\0' );
    }
    fprintf(stderr, "]\n");
}

static int show_ascii(firewall_table_t const * const table)
{
    static char const table_string[] = "Table";

    int result;

    FILE * fp = NULL;
    char * cmd = NULL;

    if (asprintf(&cmd,
                 "iptables -L -t %s -v", table->name) < 0)
    {
        tlr_event_log(tlr_log_debug, SHOW_FIREWALL, "asprintf error\n");
        fprintf(stderr, "Unable to get firewall %s table\n", table->title);
        result = EXIT_FAILURE;
        goto done;
    }

    tlr_event_log(tlr_log_debug, SHOW_FIREWALL, "%s\n", cmd);

    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        tlr_event_log(tlr_log_debug, SHOW_FIREWALL, "popen error\n");
        fprintf(stderr, "Unable to get firewall %s table\n", table->title);
        result = EXIT_FAILURE;
        goto done;
    }

    fprintf(stdout, "\n\n %s %s\n", table->title, table_string);
    fprintf(stdout, " %.*s\n", (int)(strlen(table->title) + sizeof table_string), "--------------");

    {
        char * linebuf = NULL;
        size_t n = 0;

        while (getline(&linebuf, &n, fp) > 0)
        {
            if (linebuf != NULL)
            {
                fprintf(stdout, " %s", linebuf);
            }
        }

        free(linebuf);
    }
    fprintf(stdout, "\n");
    pclose(fp);
    result = EXIT_SUCCESS;

done:
    free(cmd);
    return result;
}

int show_firewall_main(int const argc, char ** argv)
{
    int result = EXIT_FAILURE;
    tlr_response_format_t format;
    char * progname = basename(argv[0]);
    size_t i;
    size_t table_count;

    {
        int instance;
        int const max_instance = 1;

        if ((parse_show_options(argc, argv, &format, &instance, 0, max_instance) != 0) ||
            (format == tlr_response_format_unknown))
        {
            show_usage(progname);
            goto done;
        }
    }

    if (optind == argc)
    {
        i = 0;
        table_count = ARRAY_SIZE(firewall_tables);
    }
    else
    {
        i = optind;
        table_count = argc;
    }

    while (i < table_count)
    {
        firewall_table_t const * table;

        if (argc == optind)
        {
            table = &firewall_tables[i];
        }
        else
        {
            size_t t;

            table = NULL;
            for (t=0; t < ARRAY_SIZE(firewall_tables); t++)
            {
                if ((strcasecmp(argv[i], firewall_tables[t].name) == 0) ||
                    (strcasecmp(argv[i], firewall_tables[t].title) == 0))
                {
                    table = &firewall_tables[t];
                    break;
                }
            }
            if (table == NULL)
            {
                fprintf(stderr, "Invalid argument (%s)\n", argv[i]);
                show_usage(progname);
                goto done;
            }
        }

        switch (format)
        {
            case tlr_response_format_ascii:
                result = show_ascii(table);
                break;
            case tlr_response_format_json:
                /* TODO: Need implementation */
                result = EXIT_SUCCESS;
                break;
            case tlr_response_format_unknown:
                fprintf(stderr, "Format not supported\n");
                result = EXIT_FAILURE;
                break;
        }

        if (result != EXIT_SUCCESS) goto done;
        i++;
    }

    result = EXIT_SUCCESS;

done:
	return result;
}

