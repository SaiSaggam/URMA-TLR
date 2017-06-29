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

#include "dsl_version.h"
#include <get_field.h>
#include <tlr_print.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "strlcpy.h"


#define dsl_version_filename "/usr/bin/show_dsl -i 1 -f ascii"

int get_tlr_dsl_version(char const * * dsl_version_ptr)
{
    int result;
    int byte_read;
    FILE * fp = NULL;
    char * line = NULL;
    size_t line_len = 0;
    char * temp_line = NULL;
    static char version[100];

    fp = popen(dsl_version_filename, "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }

    while ((byte_read = getline(&line, &line_len, fp)) != -1)
    {
        if (line[byte_read - 1] == '\n')
        {
            line[byte_read - 1] = '\0';
        }

        temp_line = strstr(line, "FW Version");
        if (temp_line == NULL)
            continue;

        temp_line = get_field(line, ":", 2);
        if (temp_line != NULL)
        {
            strlcpy (version, temp_line, sizeof version);
            break;
        }
    }

    if (temp_line == NULL)
       strlcpy (version, "Not available", sizeof version);

    *dsl_version_ptr = version;
    result = 0;

done:
    if (fp != NULL)
    {
        pclose(fp);
    }
    free(line);
    if (result != 0)
    {
        tlr_debug("error getting dsl version\n");
    }
    return result;
}


