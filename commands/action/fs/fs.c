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
#include <libgen.h>
#include <sys/stat.h>
#include "fs_lib.h"


int main (int argc, char **argv)
{
    int result = 1;
    char *progname = basename(argv[0]);

    if (strcmp(progname, "action_dir") == 0) {
        result = digi_dir_main(argc, argv);
    } else if (strcmp(progname, "action_del") == 0) {
        result = digi_del_main(argc, argv);
    } else if (strcmp(progname, "action_rename") == 0) {
        result = digi_rename_main(argc, argv);
    } else if (strcmp(progname, "action_rmdir") == 0) {
        result = digi_rmdir_main(argc, argv);
    } else if (strcmp(progname, "action_cd") == 0) {
        result = digi_cd_main(argc, argv);
    } else if (strcmp(progname, "action_mkdir") == 0) {
        result = digi_mkdir_main(argc, argv);
    } else if (strcmp(progname, "action_copy") == 0) {
        result = digi_copy_main(argc, argv);
    } else if (strcmp(progname, "action_more") == 0) {
        result = digi_more_main(argc, argv);
    } else if (strcmp(progname, "action_pwd") == 0) {
        result = digi_pwd_main(argc, argv);
    } else {
        fprintf(stderr, "%s has not been implemented.\n", progname);
        result = 1;
    }

    return result;
}
