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
#include "create_file_list.h"
#include <stddef.h>
#include "strlcpy.h"
#include "digiutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>


#define FILE_DATE_LEN 256
#define FILE_TIME_LEN 256
#define FILE_SIZE_LEN 256
#define FILE_NAME_LEN 256
#define FILE_VERSION_LEN 256
#define FILE_AM_PM_LEN 4

typedef struct
{
    char date[FILE_DATE_LEN];
    char time[FILE_TIME_LEN];
    char am_pm[FILE_AM_PM_LEN];
    char size[FILE_SIZE_LEN];
    char name[FILE_NAME_LEN];
    char version[FILE_VERSION_LEN];
} file_info_t;

static FILE *open_index_file(void)
{
    FILE *fp;

    fp = popen("/usr/bin/get_fw_file_list.sh", "r");
    return fp;
}

static void close_index_file(FILE * const fp)
{
    if (fp != NULL)
    {
        fclose(fp);
    }
}

static int allocate_file_info_line(FILE * const fp, char **line)
{
    size_t len=0;
    int result = -1;

    while (getline(line, &len, fp) > 0)
    {
        if (strstr(*line, "HREF=") != NULL)
        {
            result = 0;
            break;
        }
    }

    return result;
}
static void free_file_info_line(char * const line)
{
    free(line);
}

static const char const end_of_file_info_token[] = "</A><br>";

static int convert_to_null_terminated_line(char const * const line)
{
    char *ptr_end_of_line;
    int result = -1;

    if ((ptr_end_of_line = strstr(line, end_of_file_info_token)) == NULL)
    {
        goto error;
    }
    *ptr_end_of_line = 0;

    result = 0;
error:
    return result;
}

static const char start_of_filename_token[] = "\">";

// The string containing the file name looks like:
// HREF=\"/support/firmware/branded/sieH7/latest/lr54-0.0.0.1.bin\">lr54-0.0.0.1.bin
static int get_file_name_from_html_string(char const * const string_with_filename, char * const file_name)
{
    char *ptr_to_file_name;
    int result = -1;

    if ((ptr_to_file_name = strstr(string_with_filename, start_of_filename_token)) == NULL)
    {
        goto error;
    }
    ptr_to_file_name += strlen(start_of_filename_token);

    strlcpy(file_name, ptr_to_file_name, FILE_NAME_LEN);

    result = 0;
error:
    return result;
}

// File name has the format lr54-0.0.0.1.bin
static int get_version_from_file_name(char const * const file_name, char * const version_string)
{
    char *ptr_to_start;
    char *ptr_to_end;
    int result = -1;

    if ((ptr_to_start = strstr(file_name, "-")) == NULL)
    {
        goto error;
    }
    ptr_to_start++;

    if ((ptr_to_end = strstr(file_name, ".bin")) == NULL)
    {
        goto error;
    }

    ptrdiff_t string_length = ptr_to_end - ptr_to_start;
    size_t max_string_length = FILE_VERSION_LEN - 1;

    string_length = (string_length > max_string_length) ? max_string_length : string_length;

    memcpy(version_string, ptr_to_start, string_length);
    version_string[string_length] = 0x0;

    result = 0;
error:
    return result;
}

#define MAX_FILE_LINE_LEN 256

// Lines have the format:
// <br> 3/14/2016  9:48 AM     22643547 <A HREF="/support/firmware/branded/sieH7/latest/lr54-0.0.0.1.bin">lr54-0.0.0.1.bin</A><br>
static int get_file_info_from_html(char const * const html_line, size_t * const length, file_info_t * const file_info)
{
    int result = -1;
    char file_string[MAX_FILE_LINE_LEN];

    if (convert_to_null_terminated_line(html_line) < 0)
    {
        goto error;
    }
    *length = strlen(html_line) + strlen(end_of_file_info_token);

    sscanf(html_line, "%s %s %s %s %*s %s", file_info->date,
           file_info->time, file_info->am_pm, file_info->size, file_string);

    if (get_file_name_from_html_string(file_string, file_info->name) < 0)
    {
        goto error;
    }

    if (get_version_from_file_name(file_info->name, file_info->version) < 0)
    {
        goto error;
    }

    result = 0;
error:
    return result;
}

char const end_tag[] = "</A><br>";

static bool more_files_left_in_line(char const * const line)
{
    char const * ptr;
    bool more_files_left = false;

    if ((ptr = strstr(line, end_tag)) != NULL)
    {
        if ((ptr = strstr(line, end_tag)) != NULL)
        {
            more_files_left = true;
        }
    }

    return more_files_left;
}

static const char starting_token[] = "<br><br>";

static char *goto_start_of_file_info(char const * const line)
{
    char *ptr=NULL;

    if ((ptr = strstr(line, starting_token)) == NULL)
    {
        goto error;
    }

    ptr += strlen(starting_token);

error:
    return ptr;
}

static int save_file_info_as_xml(FILE * const fp, file_info_t const * const file_info, int const index)
{
    int result = -1;
    int bytes_written;

    bytes_written = fprintf(fp, "<file index=\"%d\">", index);
    if (bytes_written < 0)
    {
        goto error;
    }

    bytes_written = fprintf(fp, "<name>%s</name>", file_info->name);
    if (bytes_written < 0)
    {
        goto error;
    }

    bytes_written = fprintf(fp, "<size>%s</size>", file_info->size);
    if (bytes_written < 0)
    {
        goto error;
    }

    bytes_written = fprintf(fp, "<date>%s</date>", file_info->date);
    if (bytes_written < 0)
    {
        goto error;
    }

    bytes_written = fprintf(fp, "<version>%s</version>", file_info->version);
    if (bytes_written < 0)
    {
        goto error;
    }

    bytes_written = fprintf(fp, "</file>");
    if (bytes_written < 0)
    {
        goto error;
    }

    result = 0;
error:
    return result;
}

static char *convert_html_to_xml(char const * const html_line)
{
    int index = 1;
    char const *ptr_to_html_file_info = html_line;
    size_t length;
    file_info_t file_info;
    FILE *fp = NULL;
    char *ptr_xml_buffer=NULL;
    size_t xml_buffer_len=0;
    bool found_error = false;

    fp = open_memstream(&ptr_xml_buffer, &xml_buffer_len);
    if (fp == NULL)
    {
        found_error = true;
        goto error;
    }

    if ((ptr_to_html_file_info = goto_start_of_file_info(html_line)) == NULL)
    {
        found_error = true;
        goto error;
    }

    do
    {
        if (get_file_info_from_html(ptr_to_html_file_info, &length, &file_info) < 0)
        {
            found_error = true;
            goto error;
        }

        if ((save_file_info_as_xml(fp, &file_info, index)) < 0)
        {
            found_error = true;
            goto error;
        }

        ptr_to_html_file_info += length;
        index++;

    } while (more_files_left_in_line(ptr_to_html_file_info));

error:
    if (fp != NULL)
    {
        fclose(fp);
    }

    if (found_error)
    {
        free(ptr_xml_buffer);
        ptr_xml_buffer = NULL;
    }

    return ptr_xml_buffer;
}

static char *create_xml_buffer(FILE * const fp)
{
    char *line = NULL;
    char *xml_output=NULL;

    if (allocate_file_info_line(fp, &line) < 0)
    {
        goto error;
    }

    if ((xml_output = convert_html_to_xml(line)) == NULL)
    {
        goto error;
    }

error:
    free_file_info_line(line);

    return xml_output;
}

// This API allocates memory which must be freed later
char *tlr_get_remote_firmware_filenames_as_xml(void)
{
    FILE *fp = NULL;
    char *xml_output = NULL;

    if ((fp = open_index_file()) == NULL)
    {
        goto error;
    }

    if ((xml_output = create_xml_buffer(fp)) == NULL)
    {
        goto error;
    }

    close_index_file(fp);

error:

    return xml_output;
}

