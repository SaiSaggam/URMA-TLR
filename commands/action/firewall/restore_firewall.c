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
#include <firewall_utils.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "config_def.h"
#include "config_mgr.h"
#include "digiutils.h"
#include "tlr_common.h"
#include "tlr_print.h"
#include "unittest_support.h"

#define TMP_FILE "/tmp/firewall_rules"

static char const * const process_name = "restore_firewall";

typedef enum
{
    rules_found,
    no_rules_found,
    error
} restore_firewall_rules_t;

STATIC_FUNCTION FILE *open_config_da0(void)
{
    char config_file[PATH_MAX];
    FILE *fp;

    tlr_get_config_file(config_file, sizeof config_file);

    fp = fopen(config_file, "r+");
    return fp;
}

STATIC_FUNCTION FILE *open_temporary_file(void)
{
    FILE *tmp_fp;

    tmp_fp = fopen(TMP_FILE, "w+");
    return tmp_fp;
}

STATIC_FUNCTION int write_rule(char *rule, FILE* fp)
{
    int ret=-1;

    if (fwrite(rule, strlen(rule), 1, fp) != 1)
    {
        tlr_event_log(tlr_log_debug, process_name, "fwrite failed\r\n");
        goto error;
    }

    ret = 0;
error:
    return ret;
}

STATIC_FUNCTION int read_rules_from_config_da0(FILE *config_da0_fp, FILE *tmp_fp)
{
    int num_rules=0;
    bool in_firewall_section=false;
    char *line=NULL;
    size_t len=0;

    while (getline(&line, &len, config_da0_fp) != -1)
    {
        if (firewall_line_is_start_tag(line))
        {
            if (in_firewall_section)
            {
                tlr_event_log(tlr_log_debug, process_name,
                        "Extra tag in in config.da0");
                goto error;
            }
            in_firewall_section = true;
            continue;
        }
        else if (firewall_line_is_end_tag(line))
        {
            break;
        }

        if (in_firewall_section)
        {
            num_rules++;
            if (write_rule(line, tmp_fp) < 0)
            {
                goto error;
            }
        }
    }

error:
    free(line);
    return num_rules;
}

STATIC_FUNCTION restore_firewall_rules_t restore_firewall_rules_from_config_da0(void)
{
    int ret=error;
    FILE *config_da0_fp;
    FILE *tmp_fp=NULL;
    int num_rules;

    if ((config_da0_fp = open_config_da0()) == NULL)
    {
        goto error;
    }

    if ((tmp_fp = open_temporary_file()) == NULL)
    {
        goto error;
    }

    if ((num_rules = read_rules_from_config_da0(config_da0_fp, tmp_fp)) < 0)
    {
        goto error;
    }
    ret = (num_rules > 0) ? rules_found : no_rules_found;

error:
    if (config_da0_fp != NULL)
    {
        fclose(config_da0_fp);
    }
    if (tmp_fp != NULL)
    {
        fclose(tmp_fp);
    }
    return ret;
}

STATIC_FUNCTION int restore_firewall_main(int const argc, char * const argv[])
{
    int ret = -1;
    restore_firewall_rules_t result = restore_firewall_rules_from_config_da0();

    switch (result)
    {
    case rules_found:
        if (system_command("iptables-restore --noflush < %s", TMP_FILE) < 0)
        {
            tlr_event_log(tlr_log_info, process_name, "iptables-restore failed\r\n");
            goto error;
        }
        break;
    case error:
        goto error;
    case no_rules_found:
        break;
    }

    ret = 0;
error:

    return ret;
}

#ifndef _CUNIT_
int main(int const argc, char * const argv[])
{
	int const ret = restore_firewall_main(argc, argv);

	return (ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
