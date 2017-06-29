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
#include "fs_lib.h"
#include "strlcpy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include <libgen.h>
#include <unistd.h>
#include <config_mgr.h>	/* for USER_DIR*/

static char const home_dir_root[] = USER_DIR;
static char const fs_db_dir[] = "/tmp/fs";
static char home_dir[PATH_MAX+1] = "";


const char const * get_home_dir(void)
{

    if (home_dir[0] == 0)
    {
        char dir_filename[PATH_MAX + 1];
        char buffer[PATH_MAX + 1];
        char * new_home_dir;
        FILE * dir_file;

        mkdir(fs_db_dir, ACCESSPERMS);
        snprintf(dir_filename, sizeof(dir_filename), "%s/%u", fs_db_dir, getppid());
        dir_file = fopen(dir_filename, "r");
        if (dir_file != NULL)
        {
            fgets(buffer, sizeof(buffer), dir_file);
            fclose(dir_file);
            new_home_dir = realpath(buffer, home_dir);
            if (new_home_dir == NULL)
            {
                home_dir[0] = 0;
            }
        }
        if (strncmp(home_dir, home_dir_root, strlen(home_dir_root)) != 0)
        {
            strlcpy(home_dir, home_dir_root, sizeof home_dir);
            dir_file = fopen(dir_filename, "w");
            if (dir_file != NULL)
            {
                fprintf(dir_file, "%s", home_dir);
                fclose(dir_file);
            }
        }
    }

    return home_dir;
}

const char const * get_cwd(void)
{
    const char const * current = get_home_dir();
    char const * result = "/";
    size_t const home_dir_root_length = strlen(home_dir_root);

    if (strlen(current) != home_dir_root_length)
    {
        result = (char *)&current[home_dir_root_length];
    }

    return result;
}


int set_home_dir(char const * new_home_dir)
{
    char buffer[PATH_MAX + 1];
    char * new_home;
    int result = -1;

    new_home = realpath(new_home_dir, buffer);
    if (strncmp(new_home, home_dir_root, strlen(home_dir_root)) == 0)
    {
        FILE * dir_file;
        char dir_filename[PATH_MAX + 1];

        mkdir(fs_db_dir, ACCESSPERMS);
        snprintf(dir_filename, sizeof(dir_filename), "%s/%u", fs_db_dir, getppid());
        dir_file = fopen(dir_filename, "w");
        if (dir_file != NULL)
        {
            fprintf(dir_file, "%s", new_home);
            fclose(dir_file);
            strlcpy(home_dir, new_home, sizeof home_dir);
            result = 0;
        }
    }

    return result;
}

bool is_illegal_filename(char const * pathname)
{
#define BAD_CHARS "[]\\|?*<>*&^%$#@!'`~ \t"
#define BAD_CHARS_LENGTH (sizeof(BAD_CHARS) - 1)
#define MAX_NAME_LENGTH (64)

    char buffer[PATH_MAX + 1];
    char * name;
    int i;
    bool illegal = false;

    strncpy(buffer, pathname, sizeof(buffer));
    buffer[PATH_MAX] = 0;

    name = basename(buffer);
    illegal = ((name[0] == '.') || (strlen(name) > MAX_NAME_LENGTH));
    if (!illegal)
    {
        for (i = 0; i < BAD_CHARS_LENGTH; i++)
        {
            if (strchr(name, BAD_CHARS[i]) != NULL)
            {
                illegal = true;
                break;
            }
        }
    }

    return illegal;
}


char * normalize(char const * name)
{
    char * result = NULL;

    if (name)
    {
        if (name[0] == '/')
        {
            result = realpath(name, NULL);
        }
        else
        {
            char buffer[PATH_MAX + 1];

            snprintf(buffer, sizeof(buffer), "%s/%s", get_home_dir(), name);
            result = realpath(buffer, NULL);
        }
    }
    else
    {
        /* this should never happen */
        result = strdup("");        /* create a string that can be "freed"*/
    }

    return result;
}


char * normalize_directory(char const * directory, unsigned int * error_info)
{
    char * normalized_dir = normalize(directory);
    char * result = NULL;
    struct stat file_info = { 0 };

    *error_info = 0;

    if (normalized_dir != NULL)
    {
        if (stat(normalized_dir, &file_info) == 0)
        {
            if (S_ISDIR(file_info.st_mode))
            {
                if (strncmp(home_dir_root, normalized_dir, strlen(home_dir_root)) == 0)
                {
                    result = normalized_dir;
                }
                else
                {
                    *error_info = FS_RESTRICTED_FILE;
                }
            }
            else
            {
                *error_info = FS_NOT_A_DIRECTORY;
            }
        }
        else
        {
            *error_info = FS_NO_FILE;
        }
    }
    else
    {
        *error_info = FS_NO_FILE;
    }
    if ((result == NULL) && (normalized_dir != NULL))
    {
        free(normalized_dir);
    }

    return result;
}

char * normalize_pathname(char const * pathname, unsigned int * error_info)
{
    char * normalized_filename = normalize(pathname);
    char * result = NULL;
    struct stat file_info = { 0 };

    *error_info = 0;
    if (normalized_filename != NULL)
    {
        if (strncmp(home_dir_root, normalized_filename, strlen(home_dir_root)) == 0)
        {
            if (stat(normalized_filename, &file_info) == 0)
            {
                if (S_ISREG(file_info.st_mode))
                {
                    result = normalized_filename;
                }
                else
                {
                    *error_info = FS_NOT_A_FILE;
                }
            }
            else
            {
                *error_info = FS_NO_FILE;
            }
        }
        else
        {
            *error_info = FS_RESTRICTED_FILE;
        }
    }
    else
    {
        *error_info = FS_NO_FILE;
    }
    if ((result == NULL) && (normalized_filename != NULL))
    {
        free(normalized_filename);
    }

    return result;
}


char const * normalize_nonexistant_pathname(char const * pathname, unsigned int * error_info)
{
#define TLR_PATH_MAX        (128)
    char buffer[PATH_MAX + 1];
    char buffer2[PATH_MAX + 1] = "";
    char * normalized_filename = NULL;
    char * result = NULL;
    struct stat file_info = { 0 };
    char * dir;
    char * name;

    if (strlen(pathname) > TLR_PATH_MAX)
    {
        *error_info = FS_TOO_LONG;
    }
    *error_info = 0;
    strlcpy(buffer, pathname, sizeof buffer);
    name = basename(buffer);
    dir = dirname(buffer);
    normalized_filename = normalize_directory(dir, error_info);

    if (normalized_filename != NULL)
    {
        if ((strlen(normalized_filename) + strlen(name) + 1) > TLR_PATH_MAX)
        {
            *error_info = FS_TOO_LONG;          /* don't allow ridiculously long pathnames */
            goto fn_exit;
        }
        snprintf(buffer2, sizeof(buffer2), "%s/%s", normalized_filename, name);
        free(normalized_filename);
        normalized_filename = NULL;
        if (buffer2[0] != 0)
        {
            if (stat(buffer2, &file_info) == 0)
            {
                *error_info = FS_FILE_EXISTS;
            }
            else
            {
                result = strdup(buffer2);
            }
        }
        else
        {
            *error_info = FS_RESTRICTED_FILE;
        }
    }

fn_exit:
    if ((result == NULL) && (normalized_filename != NULL))
    {
        free(normalized_filename);
    }

    return result;
}

char * combine_dir_and_name(char const * to_dir, char const * from)
{
    char buffer1[PATH_MAX + 1];
    char buffer2[PATH_MAX + 1]; /* basename() can mung source buffer so need extra buffers. */

    strlcpy(buffer1, from, sizeof buffer1);
    snprintf(buffer2, sizeof buffer2, "%s/%s", to_dir, basename(buffer1));

    return strdup(buffer2);                         /* don't return a pointer to stack variable*/
}

FILE * pipe_open(char const * format, char const * argument)
{
    char command[2 * PATH_MAX + 1];

    snprintf(command, sizeof command, format, argument);

    return popen(command, "r");
}

void parse_output_format(int argc, char ** argv, int * first_arg_index, int * want_json, int * want_help)
{
    char c;

    *want_json = 0;
    *want_help = 0;

    while ((c = getopt(argc, argv, "hf:u:")) != -1)
    {
        switch (c)
        {
            case 'f':
                *want_json = (strcmp(optarg, "json") == 0);
                break;

            case 'u':
                /* user */
                break;

            case 'h':
            case '?':
                *want_help = 1;
                break;
        }
    }

    *first_arg_index = optind;
}

char const * skip_home_dir_path(char const * const full_path)
{
    char const * sub_path;
    size_t const home_dir_root_length = strlen(home_dir_root) + 1;  /* Include the trailing '/' in the length to skip. */

    /* Assumes that the supplied full path is below home_dir_root. */
    if (strlen(full_path) >= home_dir_root_length)
    {
        sub_path = &full_path[home_dir_root_length];
    }
    else
    {
        sub_path = NULL;
    }

    return sub_path;
}
