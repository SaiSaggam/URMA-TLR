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
#include "config_def.h"
#include "config_mgr.h"
#include "digiutils.h"
#include "tlr_common.h"
#include "tlr_print.h"
#include "unittest_support.h"
#include "limits.h"
#include "strlcpy.h"
#include "config_file.h"

#define MAX_TABLE_NAME_LEN 128

static char const * const process_name = "save";

STATIC_FUNCTION FILE *execute_iptables_save(void)
{
    FILE * fp;

    if ((fp = popen("/usr/sbin/iptables-save", "r")) == NULL)
    {
        tlr_event_log(tlr_log_debug, process_name, "Could not execute iptables-save");
    }
    return fp;
}

static bool is_wan_interface_nat_rule(char const * const rule, char const * const interface_name)
{
    bool found_rule = false;
    char * rule_to_match = NULL;

    if (asprintf(&rule_to_match, "-A POSTROUTING -o %s -j MASQUERADE", interface_name) == -1)
    {
        goto done;
    }

    found_rule = (strstr(rule, rule_to_match) != NULL);

done:
    free(rule_to_match);

    return found_rule;
}

STATIC_FUNCTION bool is_wan_interfaces_nat_rule(char const * const rule)
{
    bool found_rule = false;
    config_mgr_element_enum_limit_t const * const enum_limit = tlr_get_element_enum_limit(config_mgr_table_type_config, config_wan, config_wan_interface);
    config_mgr_element_enum_t const * const enum_elements = enum_limit->value;
    int i;

    for (i = 0; i < enum_limit->count; i++)
    {
        config_mgr_element_enum_t const * const enum_element = &enum_elements[i];

        if (enum_element->id == config_wan_interface_none)
        {
            continue;
        }

        if (is_wan_interface_nat_rule(rule, enum_element->name))
        {
            found_rule = true;
            break;
        }
    }

    return found_rule;
}

STATIC_FUNCTION int write_table_name(char const * const table_name, FILE * const fp)
{
    int ret = 0;

    if (fwrite(table_name, strlen(table_name), 1, fp) <= 0)
    {
        tlr_event_log(tlr_log_debug, process_name, "fwrite failed");
        ret = -1;
    }
    return ret;
}

STATIC_FUNCTION int write_rule(char const * const line, FILE * const fp)
{
    int ret = 0;

    if (fwrite(line, strlen(line), 1, fp) != 1)
    {
        tlr_event_log(tlr_log_debug, process_name, "fwrite failed: cound not write rule");
        ret = -1;
    }
    return ret;
}

STATIC_FUNCTION int parse_ip_tables_save(FILE * const ipsave_fp, FILE * const config_file_fp)
{
    bool wrote_table_name = false;
    bool wrote_firewall_rule = false;
    char * line = NULL;
    size_t len = 0;
    int ret = -1;
    char table_name[MAX_TABLE_NAME_LEN];

    while (getline(&line, &len, ipsave_fp) != -1)
    {
        switch (line[0])
        {
            case ':': // Skip lines which start with : or #
            case '#':
                break;

            case '*':
                // "*" indicates the table name, an optional rule may follow.
                // Save the name of the table and write it if there is
                // an associated rule.
                strlcpy(table_name, line, sizeof table_name);
                wrote_table_name = false;
                wrote_firewall_rule = false;
                break;

            default:
                // Everything else is a rule or COMMIT.
                // Write the name of the table followed by the rules then COMMIT.
                // Rules have the form:
                //*nat
                //-A POSTROUTING -o dsl -j MASQUERADE
                //COMMIT
                // If there are no rules for a table, there is sometimes a section such as:
                //*mangle
                //COMMIT
                // Which we don't write to the config file
                //
                // Also, we don't write the NAT rules for WAN interfaces
                // into config.da0; they are configured by 'apply_wan'.
                if ((strstr(line, "COMMIT") != NULL) && !wrote_firewall_rule)
                {
                    // We have a COMMIT line but no rules, skip it.
                    break;
                }
                if (is_wan_interfaces_nat_rule(line))
                {
                    break;
                }
                if (!wrote_table_name)
                {
                    if (write_table_name(table_name, config_file_fp) < 0)
                    {
                        goto error;
                    }
                    wrote_table_name = true;
                    wrote_firewall_rule = false;
                }
                if (write_rule(line, config_file_fp) < 0)
                {
                    goto error;
                }
                wrote_firewall_rule = true;
                break;
        }
    }

    ret = 0;
error:
    free(line);
    return ret;
}

int config_file_write_firewall(FILE * const fp)
{
    int result;
    FILE * const ipsave_fp = execute_iptables_save();

    if (ipsave_fp == NULL)
    {
        result = -1;
        goto error;
    }

    fprintf(fp, "%s\n", FIREWALL_TAG);
    result = parse_ip_tables_save(ipsave_fp, fp);
    fprintf(fp, "%s\n", FIREWALL_END_TAG);

error:
    if (ipsave_fp != NULL)
    {
        pclose(ipsave_fp);
    }

    return result;
}

