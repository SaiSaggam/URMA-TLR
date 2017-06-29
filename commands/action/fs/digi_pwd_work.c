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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs_lib.h"


int digi_pwd_main(int argc, char **argv)
{
    char const *pathname = NULL;
    int first_arg_index = 0;
    int want_json = 0;
    int want_help = 0;
    int result = 1;

    parse_output_format(argc, argv, &first_arg_index, &want_json, &want_help);
    if (want_help) {
        fprintf(stderr, "Usage:  pwd\n");
        goto fn_exit;
    }
    pathname = get_cwd();
    if (pathname) {
        printf("%s\n", pathname);
    } else {
        printf("Invalid directory\n");
    }
    result = 0;
fn_exit:

    return result;
}
