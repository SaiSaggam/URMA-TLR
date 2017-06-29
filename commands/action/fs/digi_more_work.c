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
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <unistd.h>
#include "fs_lib.h"


int digi_more_main(int argc, char **argv)
{
    char * pathname = NULL;
    int first_arg_index = 0;
    int want_json = 0;
    int want_help = 0;
    int result = 1;
    unsigned int error_info = 0;
    FILE *in;

    parse_output_format(argc, argv, &first_arg_index, &want_json, &want_help);
    if ((want_help) || (!(argc > first_arg_index))) {
        fprintf(stderr, "Usage:  more <filename>\n");
        goto fn_exit;
    } else {
        pathname = argv[first_arg_index];
    }
    pathname = normalize_pathname(pathname, &error_info);
    if (pathname == NULL) {
        switch (error_info)
        {
            case FS_RESTRICTED_FILE:
                printf("%s is a restricted file\n", argv[first_arg_index]);
                break;
            case FS_NOT_A_FILE:
                printf("%s is not a file\n", argv[first_arg_index]);
                break;
            default:
                printf("%s does not exist\n", argv[first_arg_index]);
                break;
        }
        goto fn_exit;
    }

    in = fopen(pathname, "r");
    if (in != NULL) {
        while (!feof(in)) {
            int ch = fgetc(in);

            if (!feof(in)) {
                putchar(ch);
            }
        }
        fclose(in);
        result = 0;
        printf("\n");
    } else {
        printf("Error reading %s\n", argv[first_arg_index]);
    }
fn_exit:
	free(pathname);
    return result;
}
