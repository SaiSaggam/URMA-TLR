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
#include "unittest_support.h"
#include "save_config_process.h"
#include "config_file.h"
#include "save_config_debug.h"
#include "tlr_print.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <config_mgr.h>
#include <linux/limits.h>

int config_save_process(char const * const user)
{
    int result = -1;
    FILE * fp;
    char modified_config_filename[PATH_MAX];

    tlr_get_config_file(modified_config_filename, sizeof modified_config_filename);

    debug("config file = %s\n", modified_config_filename);
    debug("uci modified = %s\n", uci_modified_dir);

    tlr_info("save config by %s\n", user);

    fp = FOPEN(modified_config_filename, "w+");
    if (fp == NULL)
    {
        tlr_debug("Cannot create %s file\n", modified_config_filename);
        goto ret;
    }

    config_file_write_timestamp(fp, user);

    if (config_file_write_commands(fp) < 0)
    {
        goto error;
    }

    if (config_file_write_firewall(fp) < 0)
    {
         goto error;
    }

	result = 0;

error:
    FCLOSE(fp);

ret:
    return result;
}

