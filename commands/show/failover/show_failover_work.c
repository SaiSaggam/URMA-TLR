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

#include "show_failover_work.h"
#include "show_failover_json.h"
#include "plain_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <config_mgr.h>
#include <ctype.h>
#include <strlcpy.h>
#include <json.h>
#include "get_field.h"
#include "show_parse_helper.h"
#include "unittest_support.h"

static char const * get_element_label(config_state_failover_t const element_id)
{
    char const * name = NULL;

    switch (element_id)
    {
        case config_state_failover_description:
            {
                static char const description_string[] = "Description";
                name = description_string;
            }
            break;
        case config_state_failover_status:
            {
                static char const status_string[] = "Status";
                name = status_string;
            }
            break;
        case config_state_failover_primary_interface:
            {
                static char const primary_string[] = "Primary interface";
                name = primary_string;
            }
            break;
        case config_state_failover_primary_interface_status:
            {
                static char const primary_status_string[] = "Primary interface status";
                name = primary_status_string;
            }
            break;
       case config_state_failover_secondary_interface:
            {
                static char const secondary_string[] = "Secondary interface";
                name = secondary_string;
            }
            break;
        case config_state_failover_secondary_interface_status:
            {
                static char const secondary_status_string[] = "Secondary interface status";
                name = secondary_status_string;
            }
            break;
       case config_state_failover_using_interface:
            {
                static char const using_interface_string[] = "Using interface";
                name = using_interface_string;
            }
            break;
        case config_state_failover_detection_method:
            {
                static char const detection_method_string[] = "Detection method";
                name = detection_method_string;
            }
            break;
       case config_state_failover_last_ping:
            {
                static char const last_ping_string[] = "Last probe received";
                name = last_ping_string;
            }
            break;
        case config_state_failover_failing_over:
            {
                static char const failing_over_string[] = "Failing over in";
                name = failing_over_string;
            }
            break;
        case config_state_failover_switching_back:
            {
                static char const switching_back_string[] = "Switching back in";
                name = switching_back_string;
            }
            break;
    }

    return name;
}

static void show_failover_ascii(failover_status_t const * const pstatus, int const instance, bool const found, bool const summary)
{
    if (found)
    {
        fprintf(stdout, " Failover %d Status and State\n", instance);
        fprintf(stdout, " ----------------------------\n\n");
        fprintf(stdout, " %-26s %3s %-s\n", get_element_label(config_state_failover_primary_interface), ":", pstatus->primary_if);
        fprintf(stdout, " %-26s %3s %-s\n", get_element_label(config_state_failover_primary_interface_status), ":", pstatus->primary_if_status);
        fprintf(stdout, "\n");
        fprintf(stdout, " %-26s %3s %-s\n", get_element_label(config_state_failover_secondary_interface), ":", pstatus->secondary_if);
        fprintf(stdout, " %-26s %3s %-s\n", get_element_label(config_state_failover_secondary_interface_status), ":", pstatus->secondary_if_status);
        fprintf(stdout, "\n");
        fprintf(stdout, " %-26s %3s %-s\n", get_element_label(config_state_failover_using_interface), ":", pstatus->using_if);
        fprintf(stdout, "\n");
        fprintf(stdout, " %s %s %s\n", "Using" , pstatus->detection_method, "detection");
        fprintf(stdout, "\n");
        if (pstatus->using_ping == true)
        {
            fprintf(stdout, " %-26s %3s %s %-s\n", get_element_label(config_state_failover_last_ping), ":", pstatus->probe_received_time, "seconds ago");
        }
        if (pstatus->show_failing_over)
        {
            fprintf(stdout, " %-26s %3s %s %-s\n", get_element_label(config_state_failover_failing_over), ":", pstatus->failing_over_time, "seconds");
        }
        if (pstatus->show_switch_back)
        {
            fprintf(stdout, " %-26s %3s %s %-s\n", get_element_label(config_state_failover_switching_back), ":", pstatus->switching_back_time, "seconds");
        }
        fprintf(stdout,"\n");
    }
    else if (!summary)
    {
        fprintf(stdout, " Failover %d is not active\n", instance);
        fprintf(stdout,"\n");
    }
}

static char * parse_failover_status(char const * const line, char const * const item, int const instance)
{
    char * ret = NULL;
    char buffer[128] = "";
    char * buf = (char *)line;

    if (line == NULL || item == NULL)
        goto done;

    snprintf(buffer, sizeof buffer, "failover%d %s:", instance, item);
    if (strstr(buf, buffer) == line)
        ret = get_field(buf, ":", 2);

done:
    return ret;
}

static bool update_failover_status_from_file(FILE * fp, failover_status_t * pstatus, int instance)
{
    char * line = NULL;
    char * field = NULL;
    size_t len = 0;
    bool found =false;

    if (fp == NULL || pstatus == NULL)
        goto error;

    while (getline(&line, &len, fp) != -1)
    {
        //fprintf(stdout, "%s\n", line);
        if(parse_failover_status(line, "valid", instance))
        {
            found = true;
            pstatus->status = true;
        }
        if (found)
        {
            if((field = parse_failover_status(line, "primary", instance)) != NULL)
                strlcpy(pstatus->primary_if, field, sizeof pstatus->primary_if);
            else if((field = parse_failover_status(line, "primary_status", instance)) != NULL)
                strlcpy(pstatus->primary_if_status, field, sizeof pstatus->primary_if_status);
            else if((field = parse_failover_status(line, "secondary", instance)) != NULL)
                strlcpy(pstatus->secondary_if, field, sizeof pstatus->secondary_if);
            else if((field = parse_failover_status(line, "secondary_status", instance)) != NULL)
                strlcpy(pstatus->secondary_if_status, field, sizeof pstatus->secondary_if_status);
            else if((field = parse_failover_status(line, "current", instance)) != NULL)
                strlcpy(pstatus->using_if, field, sizeof pstatus->using_if);
            else if((field = parse_failover_status(line, "detection method", instance)) != NULL)
            {
                strlcpy(pstatus->detection_method, field, sizeof pstatus->detection_method);
                if (strcmp(pstatus->detection_method, "probe") == 0)
                    pstatus->using_ping = true;
                else
                    pstatus->using_ping = false;
            }
            else if ((field = parse_failover_status(line, "last ping received", instance)) != NULL)
            {
                strlcpy(pstatus->probe_received_time, field, sizeof pstatus->probe_received_time);
            }
            else if ((field = parse_failover_status(line, "failing over in", instance)) != NULL)
            {
                strlcpy(pstatus->failing_over_time, field, sizeof pstatus->failing_over_time);
                pstatus->show_failing_over = true;
            }
            else if ((field = parse_failover_status(line, "switch back in", instance)) != NULL)
            {
                strlcpy(pstatus->switching_back_time, field, sizeof pstatus->switching_back_time);
                pstatus->show_switch_back = true;
            }
        }
    }

error:
    if (line != NULL)
        free(line);

    return found;
}

STATIC_FUNCTION bool update_failover_status(failover_status_t * pstatus, int instance)
{
    int fd;
    FILE * fp = NULL;
    bool found =false;

    // TODO: "FAILOVER_SHOW_SOCKET" needs to be centrally defined somewhere (see failover_show.c)
    fd = connect_to_plain_socket("FAILOVER_SHOW_SOCKET", true);
    if (fd < 0)
        goto error;

    fp = fdopen(fd, "r");
    if (fp == NULL)
        goto error;

    found = update_failover_status_from_file(fp, pstatus, instance);

error:
    if (fp != NULL)
        fclose(fp);
    if (fd >= 0)
        close(fd);

    return found;
}

int show_failover_main(int argc, char **argv)
{
	int result = EXIT_FAILURE;	/* default to ERROR. If all goes well, we'll change to SUCCESS */
	char * progname;
	tlr_response_format_t format;
	int instance;
    int i;
    static failover_status_t failover_status;
    bool found;
    bool summary = false;
    int max_instance = tlr_get_max_instance(config_state_failover, config_mgr_table_type_show);

    if (max_instance <= 0)
        goto error;

	progname = basename(argv[0]);

	if (parse_show_options(argc, argv, &format, &instance, 0, max_instance) != 0)
	{
		usage_show(progname);
		goto error;
	}

    if (instance < 0)
    {
        goto error;
    }

    if (instance != 0)
        max_instance = instance;
    else
    {
        instance = 1;  // If no instance is given, then default to instance one.
        summary = true;
    }

	/*
	    Now show the status and statistics.
	*/
	switch (format)
	{
		case tlr_response_format_ascii:
            for (i = instance; i <= max_instance; i++)
            {
                 found = update_failover_status(&failover_status, i);
                 show_failover_ascii(&failover_status, i, found, summary);
             }
			break;
		case tlr_response_format_json:
            update_failover_status(&failover_status, instance);
			show_failover_json(&failover_status, instance);
			break;
		case tlr_response_format_unknown:
			goto error;
	}

	result = EXIT_SUCCESS;
error:
	return result;
}
