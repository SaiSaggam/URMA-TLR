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
#include "mac_address.h"
#include "tlr_format_string.h"
#include <strip_whitespace.h>
#include <tlr_print.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char * get_mac_address(char const * const interface_name, size_t const instance)
{
    int result;
    int bytes_read;
    FILE * fp = NULL;
    char * line = NULL;
    size_t line_len = 0;
    char * address_filename = NULL;

    if (asprintf(&address_filename, "/sys/class/net/%s%zu/address", interface_name, instance) < 0)
    {
        result = -1;
        goto done;
    }

    fp = fopen(address_filename, "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }
    bytes_read = getline(&line, &line_len, fp);
    if (bytes_read <= 0)
    {
        result = -1;
        goto done;
    }
    if (line[bytes_read - 1] == '\n')
    {
        line[bytes_read - 1] = '\0';
    }
    result = 0;

done:
    free(address_filename);
    if (fp != NULL)
    {
        fclose(fp);
    }
    if (result != 0)
    {
        tlr_debug("error getting MAC address for interface %s %zd\n", interface_name, instance);
    }

    return line;
}


