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

#include "save_config_work.h"
#include "save_config_process.h"
#include "tlr_print.h"

#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <syslog.h>

bool save_config_main(int const argc, char * const argv[])
{
    bool success = false;
    char const * const process_name = basename(argv[0]);
    char const * user = NULL;

    openlog(process_name, 0, LOG_USER);

    if (!tlr_parse_args(argc, argv, NULL, &user))
    {
        tlr_debug("tlr_parse_args() failed\n");
        goto done;
    }

    if (user == NULL)
    {
        tlr_debug("no username supplied\n");
        goto done;
    }

    success = (config_save_process(user) == 0);

done:
    return success;
}

