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

#include "firmware_version.h"
#include <strip_whitespace.h>
#include <tlr_print.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define firmware_version_filename "/usr/etc/tlr-version"

int get_tlr_firmware_version(char * * firmware_version_ptr)
{
    int result;
    int temp_result;
    FILE * fp;
    char * line = NULL;
    size_t line_len = 0;
    char search_char = ':';
    char * version;

    fp = fopen(firmware_version_filename, "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }
    temp_result = getline( &line, &line_len, fp);
    if (temp_result <= 0)
    {
        result = -1;
        goto done;
    }
    /* we're just interested in the bit after the first ':' character */
    version = strchr(line, search_char);
    if (version == NULL)
    {
        result = -1;
        goto done;
    }
    version++; /* skip passed the ':' */
    version = strip_whitespace(version);
    if (version[0] == '\0')    /* ensure that there is something left */
    {
        result = -1;
        goto done;
    }
    *firmware_version_ptr = strdup(version);
    if (*firmware_version_ptr == NULL)
    {
        result = -1;
        goto done;
    }
    result = 0;
done:
    if (fp != NULL)
    {
        fclose(fp);
    }
    free(line);
    if (result != 0)
    {
        tlr_debug("error getting firmware version\n");
    }

    return result;
}


