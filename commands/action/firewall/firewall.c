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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "config_def.h"
#include "config_mgr.h"
#include "digiutils.h"
#include "tlr_common.h"
#include "tlr_print.h"
#include "unittest_support.h"

static char const * const process_name = "action_firewall";

static const char * invalid_characters[] = { "|", "&", ";", "<", ">", "$", "`" };

// Derived from https://en.wikipedia.org/wiki/Code_injection#Shell_injection
// TODO: we should replace this with a common function in the TLR
STATIC_FUNCTION int check_rule_for_invalid_characters(char const * const rule)
{
    int ret = -1;

    if (rule == NULL)
    {
        goto error;
    }

    for (int i = 0; i < asizeof(invalid_characters); i++)
    {
        if (strstr(rule, invalid_characters[i]) != NULL)
        {
            tlr_event_log(tlr_log_info, process_name, "Firewall rule contains invalid character %s", invalid_characters[i]);
            goto error;
        }
    }

    ret = 0;
error:
    return ret;
}


STATIC_FUNCTION void replace_iptables_with_firewall(char * str)
{
    const char * old_str = "iptables";
    const char * new_str = "firewall";
    char * ptr;

    while ((ptr = strstr(str, old_str)) != NULL)
    {
        memcpy(ptr, new_str, strlen(new_str));
        ptr += strlen(new_str);
    }

    return;
}

STATIC_FUNCTION void print_usage(void)
{
    printf("Usage: firewall rule\n");
}

/*
 * Given a number of args, append them all together and separate
 * each of them with a space.
*/
STATIC_FUNCTION char * convert_args_to_string(int const argc, char * const argv[])
{
    char * output_string = NULL;
    int i;
    bool error;

    for (i = 0; i < argc; i++)
    {
        char * new_string;
        int bytes_written;

        if (i == 0)
        {
            bytes_written = asprintf(&new_string, "%s", argv[i]);
        }
        else
        {
            bytes_written = asprintf(&new_string, "%s %s", output_string, argv[i]);
        }
        if (bytes_written == -1)
        {
            error = true;
            goto done;
        }
        free(output_string);
        output_string = new_string;
    }

    error = false;

done:
    if (error)
    {
        free(output_string);
        output_string = NULL;
    }

    return output_string;
}

#define FIRST_FIREWALL_RULE_ARG 5

STATIC_FUNCTION char * extract_rule_from_args(const int argc, char * const argv[])
{
    char * rule = NULL;

    if (argc < FIRST_FIREWALL_RULE_ARG + 1)
    {
        print_usage();
        goto error;
    }
    /* all the args after the first firewall rule arg make up the
     * rule
     */
    rule = convert_args_to_string(argc - FIRST_FIREWALL_RULE_ARG, &argv[FIRST_FIREWALL_RULE_ARG]);

error:
    return rule;
}

STATIC_FUNCTION void display_ip_tables_output(FILE * fp)
{
    size_t len = 0;
    char * line = NULL;

    while (getline(&line, &len, fp) != -1)
    {
        replace_iptables_with_firewall(line);
        printf("%s", line);
    }
    free(line);
    return;
}

STATIC_FUNCTION int execute_ip_tables_command(char const * const rule)
{
    char * cmd = NULL;
    FILE * fp = NULL;
    int ret = -1;

    if (asprintf(&cmd, "iptables %s  2>&1", rule) < 0)
    {
        goto error;
    }
    if ((fp = popen(cmd, "r")) != NULL)
    {
        display_ip_tables_output(fp);
    }
    else
    {
        goto error;
    }

    ret = 0;
error:
    if (fp != NULL)
    {
        pclose(fp);
    }
    free(cmd);

    return ret;
}

STATIC_FUNCTION int update_firewall_main(int const argc, char * const argv[])
{
    int ret = -1;
    char * rule;

    rule = extract_rule_from_args(argc, argv);
    if (rule == NULL)
    {
        goto error;
    }

    if (check_rule_for_invalid_characters(rule) < 0)
    {
        goto error;
    }

    if (execute_ip_tables_command(rule) < 0)
    {
        goto error;
    }

    ret = 0;
error:
    free(rule);
    return ret;
}

#ifndef _CUNIT_
int main(int const argc, char * const argv[])
{
    int const ret = update_firewall_main(argc, argv);

    return (ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
