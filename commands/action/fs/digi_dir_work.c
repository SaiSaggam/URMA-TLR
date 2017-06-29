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
#include "config_mgr.h"
#include "strlcpy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

static int get_next_file(DIR *dir, char const *directory, char **name, unsigned int *size, char **last_access, unsigned int *is_dir)
{
    struct dirent *file = readdir(dir);
    char pathname[2*PATH_MAX+1];
    struct stat file_info = {0};

    while (file != NULL) {
        if (file->d_type == DT_REG) {
            snprintf(pathname, sizeof(pathname), "%s/%s", directory, file->d_name);
            if (stat(pathname, &file_info) == 0) {
                *name = file->d_name;
                *size = file_info.st_size;
                *last_access = asctime(localtime(&file_info.st_mtime));
                *is_dir = 0;
                return 0;
            }
        } else if ((file->d_type == DT_DIR) && (file->d_name[0] != '.')) {
            *name = file->d_name;
            *is_dir = 1;
            return 0;
        }

        file = readdir(dir);
    }

    return -1;
}

static unsigned int get_length_of_longest_filename(char const *directory)
{
    unsigned int longest = 0;
    DIR *dir;
    char *name = NULL, *last_access = NULL;
    unsigned int size, is_dir = 0;

    dir = opendir(directory);
    if (dir != NULL) {
        while (get_next_file(dir, directory, &name, &size, &last_access, &is_dir) == 0) {
            if (strlen(name) > longest)
                longest = strlen(name);
        }
        closedir(dir);
    }
    return longest;
}

char const *skip_number(char const *buffer)
{
    char const *p = buffer;

    while ((*p) && (!isdigit(*p)))
        p++;
    while ((*p) && (isdigit(*p)))
        p++;
    while ((*p) && (isblank(*p)))
        p++;
    return p;
}


unsigned long long int get_available_space(FILE *df)
{
    char buffer[128];
    unsigned long long int available_space   = 0;

    fgets(buffer, sizeof(buffer), df);
    if (fgets(buffer, sizeof(buffer), df) != NULL) {
        char const *p = buffer;

        while ((*p != 0) && (!isblank(*p))) {
            p++;
        }
        p = skip_number(p);
        p = skip_number(p);
        if (*p != 0) {
            available_space = atoll(p);
        }
    }

    return available_space;
}


int json_output(char const *directory)
{
    (void) directory;
    printf("Not implemented\n");
    return 0;
}

void printfcomma (unsigned long long int n) {
    if (n < 1000) {
        printf ("%llu", n);
        return;
    }
    printfcomma(n/1000);
    printf (",%03llu", n%1000);
}

int text_output(char const *directory)
{
#define MAX_FILENAME_FORMAT_WIDTH   (40)
#define MIN_FILENAME_FORMAT_WIDTH   (20)
    const char *title_format = " %-*s  %10.10s   %-20.20s\n";
    const char *format = " %-*s  %10u   %-20.20s\n";
    const char *format_dir = " %-*s               %-20.20s\n";
    FILE *df = NULL;
    int result = 1;
    DIR *dir;
    char *name = NULL, *last_access = NULL;
    unsigned int i, size, is_dir = 0;
    unsigned long long int avail;
    unsigned int longest_filename = get_length_of_longest_filename(directory);
    unsigned int filename_format_width = 0;

    setlocale(LC_NUMERIC, "");
    filename_format_width = longest_filename;
    if (filename_format_width > MAX_FILENAME_FORMAT_WIDTH)
        filename_format_width = MAX_FILENAME_FORMAT_WIDTH;
    if (filename_format_width < MIN_FILENAME_FORMAT_WIDTH)
        filename_format_width = MIN_FILENAME_FORMAT_WIDTH;
    dir = opendir(directory);
    df = popen("df -B 1 " USER_DIR, "r");
    if ((dir == NULL) || (df == NULL)) {
        goto text_output_exit;
    }

    printf(title_format, filename_format_width, "File", "Size", "Last Modified");
    printf(" ------------------------------------------------------");
    for (i = 0; i < (filename_format_width - MIN_FILENAME_FORMAT_WIDTH); i++)
        fputc('-', stdout);
    fputc('\n', stdout);
    while (get_next_file(dir, directory, &name, &size, &last_access, &is_dir) == 0) {
        if (is_dir) {
            printf(format_dir, filename_format_width, name, "Directory");
        } else {
            printf(format, filename_format_width, name, size, last_access);
        }
    }
    avail = get_available_space(df);
    printf("\n Remaining User Space: ");
    printfcomma(avail);
    printf(" bytes\n\n");

    result = 0;

text_output_exit:
    if (df != NULL)
        pclose(df);
    if (dir != NULL)
        closedir(dir);
    return result;
}


int digi_dir_main(int argc, char **argv)
{
    char * directory = NULL;
    char current_dir[PATH_MAX+1] = "";
    int first_arg_index = 0;
    int want_json = 0;
    int want_help = 0;
    int result = 1;
    unsigned int error_info = 0;

    parse_output_format(argc, argv, &first_arg_index, &want_json, &want_help);

    if (want_help)
    {
        fprintf(stderr, "Usage:  dir [<directory>]\n");
        goto fn_exit;
    }
    else if (argc > first_arg_index)
    {
        directory = argv[first_arg_index];
    }
    else
    {
        strlcpy(current_dir, get_home_dir(), sizeof current_dir);
        directory = current_dir;
    }

    directory = normalize_directory(directory, &error_info);
    if (directory == NULL)
    {
        if (error_info == FS_RESTRICTED_FILE)
        {
            printf("%s is restricted\n", argv[first_arg_index]);
        }
        else if (error_info == FS_NO_FILE)
        {
            printf("%s does not exist\n", argv[first_arg_index]);
        }
        else
        {
            fprintf(stderr, "Invalid directory\n");
        }
        goto fn_exit;
    }

    if (want_json)
    {
        result = json_output(directory);
    }
    else
    {
        result = text_output(directory);
    }

fn_exit:
    free(directory);

	return result;
}
