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

#ifndef fs_lib_h
#define fs_lib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdbool.h>

enum {
    FS_RESTRICTED_FILE = 1,
    FS_NOT_A_FILE,
    FS_NO_FILE,
    FS_FILE_EXISTS,
    FS_NOT_A_DIRECTORY,
    FS_TOO_LONG
};

enum {
    FS_FILE_MUST_EXIST = 1,
    FS_FILE_MUST_NOT_EXIST
};

int set_home_dir(char const *new_home_dir);
const char const *get_home_dir(void);
bool is_illegal_filename(char const *pathname);
char * normalize_directory(char const *directory, unsigned int *error_info);
char * combine_dir_and_name(char const *to_dir, char const *from);
char * normalize_pathname(char const *pathname, unsigned int *error_info);
char const *normalize_nonexistant_pathname(char const *pathname, unsigned int *error_info);
const char const *get_cwd(void);
char const * skip_home_dir_path(char const * const full_path);

FILE *pipe_open(char const *format, char const *argument);
void parse_output_format(int argc, char **argv, int *first_arg_index, int *want_json, int *want_help);

int digi_dir_main(int argc, char **argv);
int digi_del_main(int argc, char **argv);
int digi_rename_main(int argc, char **argv);
int digi_rmdir_main(int argc, char **argv);
int digi_cd_main(int argc, char **argv);
int digi_mkdir_main(int argc, char **argv);
int digi_copy_main(int argc, char **argv);
int digi_more_main(int argc, char **argv);
int digi_pwd_main(int argc, char **argv);
#endif
