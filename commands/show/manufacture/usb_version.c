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

#include "usb_version.h"
#include <get_field.h>
#include <tlr_print.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "strlcpy.h"

#define usb_version_filename "/usr/bin/cp2108_get_fw_version.sh"

int get_tlr_usb_serial_version(char const * * usb_version_ptr)
{
    int result;
    int byte_read;
    FILE * fp = NULL;
    char * line = NULL;
    size_t line_len = 0;
    char * temp_line = NULL;
    static char version[100];

    fp = popen(usb_version_filename, "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }
    byte_read = getline(&line, &line_len, fp);
    if (byte_read <= 0)
    {
        result = -1;
        goto done;
    }
    if (line[byte_read - 1] == '\n')
    {
        line[byte_read - 1] = '\0';
    }

    temp_line = strstr(line, "FW version");
    if (temp_line != NULL)
    {
        temp_line = get_field(line, ":", 2);
        if (temp_line != NULL)
        {
            strlcpy (version, temp_line, sizeof version);
        }
    }

    if (temp_line == NULL)
        strlcpy (version, "Not available", sizeof version);

    *usb_version_ptr = version;
    result = 0;

done:
    if (fp != NULL)
    {
        pclose(fp);
    }
    free(line);
    if (result != 0)
    {
        tlr_debug("error getting usb version\n");
    }
    return result;
}


