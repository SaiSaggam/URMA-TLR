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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config_def.h"
#include "tlr_print.h"
#include "config_mgr.h"

// Return the instance parameter returned to a process.
int get_instance(int const argc, char * const argv[], size_t * const instance, const size_t group_id)
{
    int ret, opt;

    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);

    if (table->groups[group_id].instances == 0)
    {
        ret = -1;
        tlr_debug("Instances not supported for group %zd\n", group_id);
        goto error;
    }

    *instance = 0;

    opterr = 0;

    while ((opt = getopt (argc, argv, "i:u:a:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            *instance = strtol(optarg, NULL, 10);
            break;

        case 'u':
        case 'a':
        default:
            break;

        }
    }

    if ((*instance == 0) || (*instance > table->groups[group_id].instances))
    {
        ret = -1;
        tlr_debug("Invalid instance %zd\n", *instance);
        goto error;
    }

    ret = 0;

error:
    return ret;
}



