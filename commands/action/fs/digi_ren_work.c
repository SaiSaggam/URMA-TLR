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


int digi_rename_main(int argc, char **argv)
{
    int first_arg_index = 0;
    int want_json = 0;
    int want_help = 0;
    int result = 1;
    unsigned int error_info = 0;
    char const *org_from, *org_to;
    char * from = NULL;
    char const * to;
    char * pathname = NULL;
    char * to_dir = NULL;
    int to_exists = 0;

    parse_output_format(argc, argv, &first_arg_index, &want_json, &want_help);
    if ((want_help) || (!(argc > (first_arg_index + 1)))) {
        fprintf(stderr, "Usage:  rename <old-filename> <new-filename>\n");
        goto fn_exit;
    } else {
        org_from = argv[first_arg_index];
        org_to = argv[first_arg_index+1];
    }
    from = normalize_pathname(org_from, &error_info);
    if (from == NULL) {
        switch (error_info)
        {
            case FS_RESTRICTED_FILE:
                printf("%s is a restricted file\n", org_from);
                break;
            case FS_NOT_A_FILE:
                printf("%s is not a file\n", org_from);
                break;
            default:
                printf("%s does not exist\n", org_from);
                break;
        }
        goto fn_exit;
    }

    to_dir = normalize_directory(org_to, &error_info);
    to = NULL;
    if (to_dir != NULL) {
        pathname = combine_dir_and_name(to_dir, from);

        if (pathname) {
            to = normalize_nonexistant_pathname(pathname, &error_info);
        }
    } else {
        pathname = strdup(org_to);
		if (pathname == NULL)
			goto fn_exit;
        to = normalize_nonexistant_pathname(org_to, &error_info);
    }
    if ((to == NULL) && (error_info == FS_FILE_EXISTS)) {
        to = normalize_pathname(pathname, &error_info);
        to_exists = (to != NULL);
    }
    if (to == NULL) {
        switch (error_info)
        {
            case FS_RESTRICTED_FILE:
                printf("%s is a restricted file\n", org_to);
                break;
            case FS_NOT_A_FILE:
                printf("%s is not a file\n", org_to);
                break;
            case FS_TOO_LONG:
                printf("%s results in a pathname which is too long\n", argv[first_arg_index]);
                break;
            default:
                printf("%s is invalid\n", org_to);
                break;
        }
        goto fn_exit;
    }


    if (is_illegal_filename(to)) {
        printf("%s is an illegal filename\n", org_to);
        goto fn_exit;
    }

    if (strcmp(to, from) == 0) {
        printf("%s and %s are the same file\n", org_from, org_to);
        goto fn_exit;
    }
    if (access(from, W_OK) != 0) {
        printf("%s is read-only\n", org_from);
        goto fn_exit;
    }
    if (to_exists) {
        int reply;

        fprintf(stderr, "%s already exists, overwrite it? <y|n>", org_to);
        reply = getchar();
        printf("\n");
        if ((reply != 'y') && (reply != 'Y')) {
            printf("Rename cancelled\n");
            goto fn_exit;
        } else {
            remove(to);
        }
    }

    if (rename(from, to) != 0) {
        printf("Error renaming %s to %s\n", org_from, org_to);
    }

    result = 0;
fn_exit:
	free(to_dir);
	free(from);
	free(pathname);

	return result;
}
