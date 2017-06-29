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

#include "modem_to_config_state_dsl.h"
#include "dsl_context.h"
#include "config_mgr.h"
#include "tlr_common.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static bool line_mode_enum_matches_modem_line_mode(config_state_dsl_line_mode_t const line_mode, char const * const modem_line_mode)
{
    bool match_found = false;
    char const * required_line_mode;

    switch (line_mode)
    {
        case config_state_dsl_line_mode_adsl2_plus:
            required_line_mode = "ADSL2 PLUS";
            break;
        case config_state_dsl_line_mode_adsl2:
            required_line_mode = "ADSL2";
            break;
        case config_state_dsl_line_mode_gdmt:
            required_line_mode = "G.DMT";
            break;
        case config_state_dsl_line_mode_glite:
            required_line_mode = "G.LITE";
            break;
        case config_state_dsl_line_mode_unknown:
            goto done;
    }

    match_found = strcmp(modem_line_mode, required_line_mode) == 0;

done:
    return match_found;
}

config_state_dsl_line_mode_t modem_to_config_state_dsl_line_mode(char const * const modem_line_mode)
{
    config_mgr_element_enum_limit_t const * const line_mode_enum_limit = tlr_get_element_enum_limit(config_mgr_table_type_show, config_state_dsl, config_state_dsl_line_mode);
    config_mgr_element_enum_t const * const line_mode_enum = line_mode_enum_limit->value;
    config_state_dsl_line_mode_t line_mode = config_state_dsl_line_mode_unknown;
    int i;

    for (i = 0; i < line_mode_enum_limit->count; i++)
    {
        if (line_mode_enum_matches_modem_line_mode(line_mode_enum[i].id, modem_line_mode))
        {
            line_mode = line_mode_enum[i].id;
            break;
        }
    }

    return line_mode;
}

static bool line_status_enum_matches_modem_line_status(config_state_dsl_line_status_t const line_status, char const * const modem_line_status)
{
    bool match_found = false;
    char const * required_line_status;

    switch (line_status)
    {
        case config_state_dsl_line_status_up:
            required_line_status = "up";
            break;
        case config_state_dsl_line_status_down:
            required_line_status = "down";
            break;
        case config_state_dsl_line_status_waiting_for_initialisation:
            required_line_status = "wait for initialization";
            break;
        case config_state_dsl_line_status_initialising:
            required_line_status = "initializing";
            break;
        case config_state_dsl_line_status_unknown:
            goto done;
    }

    match_found = strcmp(modem_line_status, required_line_status) == 0;

done:
    return match_found;
}

config_state_dsl_line_status_t modem_to_config_state_dsl_line_status(char const * const modem_line_status)
{
    config_mgr_element_enum_limit_t const * const line_status_enum_limit = tlr_get_element_enum_limit(config_mgr_table_type_show, config_state_dsl, config_state_dsl_line_status);
    config_mgr_element_enum_t const * const line_status_enum = line_status_enum_limit->value;
    config_state_dsl_line_status_t line_status = config_state_dsl_line_status_unknown;
    int i;

    for (i = 0; i < line_status_enum_limit->count; i++)
    {
        if (line_status_enum_matches_modem_line_status(line_status_enum[i].id, modem_line_status))
        {
            line_status = line_status_enum[i].id;
            break;
        }
    }

    return line_status;
}

static bool channel_type_enum_matches_modem_channel_type(config_state_dsl_downstream_channel_type_t const channel_type, char const * const modem_channel_type)
{
    bool match_found = false;
    char const * required_channel_type;

    switch (channel_type)
    {
        case config_state_dsl_downstream_channel_type_fast:
            required_channel_type = "fast";
            break;
        case config_state_dsl_downstream_channel_type_interleaved:
            required_channel_type = "interleaved";
            break;
        case config_state_dsl_downstream_channel_type_unknown:
            goto done;
    }

    match_found = strcmp(modem_channel_type, required_channel_type) == 0;

done:
    return match_found;
}

config_state_dsl_downstream_channel_type_t modem_to_config_state_dsl_channel_type(char const * const modem_channel_type)
{
    config_mgr_element_enum_limit_t const * const channel_type_enum_limit = tlr_get_element_enum_limit(config_mgr_table_type_show, config_state_dsl, config_state_dsl_downstream_channel_type);
    config_mgr_element_enum_t const * const channel_type_enum = channel_type_enum_limit->value;
    config_state_dsl_downstream_channel_type_t channel_type = config_state_dsl_downstream_channel_type_unknown;
    int i;

    for (i = 0; i < channel_type_enum_limit->count; i++)
    {
        if (channel_type_enum_matches_modem_channel_type(channel_type_enum[i].id, modem_channel_type))
        {
            channel_type = channel_type_enum[i].id;
            break;
        }
    }

    return channel_type;
}
