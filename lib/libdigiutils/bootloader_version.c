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

#include "bootloader_version.h"
#include <get_field.h>
#include <tlr_print.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define cmdline_filename "/proc/cmdline"
#define boot_ver "boot_ver="

int get_tlr_bootloader_version(char * * bootloader_version_ptr)
{
    int result;
    FILE * fp;
    char * line = NULL;
    size_t line_len = 0;
    char * version;

    fp = fopen(cmdline_filename, "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }

    if (getline( &line, &line_len, fp) <= 0)
    {
        result = -1;
        goto done;
    }

    version = strstr(line, boot_ver);
    if (version == NULL)
    {
        result = -1;
        goto done;
    }

    version = get_field(version, "=", 2);
    if (version == NULL)
    {
        result = -1;
        goto done;
    }

    *bootloader_version_ptr = strdup(version);
    if (*bootloader_version_ptr == NULL)
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
        tlr_debug("error getting bootloader version\n");
    }

    return result;
}


