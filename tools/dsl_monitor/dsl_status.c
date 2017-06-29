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

#include "dsl_status.h"
#include "dsl_context.h"
#include "config_mgr.h"

#include <string.h>

static config_state_dsl_admin_status_t get_admin_status(dsl_element_t const * const config)
{
    config_state_dsl_admin_status_t admin_status = config_state_dsl_admin_status_down;

    if (config == NULL || strcmp(config[config_dsl_state].value, "off") == 0)
    {
        goto done;
    }

    /* all other parameters should be valid except empty username and/or empty password */
    if (*config[config_dsl_ppp_username].value == '\0')
    {
        goto done;
    }

    admin_status = config_state_dsl_admin_status_up;

done:
    return admin_status;
}

static config_state_dsl_oper_status_t get_oper_status(dsl_state_t const current_state)
{
    config_state_dsl_oper_status_t oper_status;

    switch (current_state)
    {
        case dsl_state_connected:
            oper_status = config_state_dsl_oper_status_up;
            break;
        default:
            oper_status = config_state_dsl_oper_status_down;
            break;
    }

    return oper_status;
}

void dsl_status_get(dsl_context_t * const context)
{
    context->admin_status = get_admin_status(context->config);
    context->oper_status = get_oper_status(context->current_state);
}
