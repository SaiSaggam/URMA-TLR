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

#include "bank.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BANK0_MTDBLOCK_NUM 3
#define BANK1_MTDBLOCK_NUM 5

#define cmdline_filename "/proc/cmdline"
#define mtdblock_param "root=/dev/mtdblock"

int get_tlr_bank(int * bank_ptr)
{
    int result;
    char * line = NULL;
    size_t len;
    char * ptr;
    int mtdblock_num;

    FILE * const fp = fopen(cmdline_filename, "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }

    if (getline(&line, &len, fp) <= 0)
    {
        result = -1;
        goto done;
    }

    ptr = strstr(line, mtdblock_param);
    if (ptr == NULL)
    {
        result = -1;
        goto done;
    }

    ptr += strlen(mtdblock_param);
    mtdblock_num = strtol(ptr, NULL, 10);

    switch (mtdblock_num)
    {
        case BANK0_MTDBLOCK_NUM:
            *bank_ptr = 0;
            result = 0;
            break;
        case BANK1_MTDBLOCK_NUM:
            *bank_ptr = 1;
            result = 0;
            break;
        default:
            result = -1;
            break;
    }

done:
    free(line);
    if (fp != NULL)
    {
        fclose(fp);
    }
    return result;
}

