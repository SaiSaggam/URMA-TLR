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
#include "serial_number.h"
#include <strip_whitespace.h>
#include <tlr_print.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "odm.h"

int get_tlr_serial_number(char * * serial_number_ptr)
{
    int result;
    static char serial_number[100];
    
    *serial_number_ptr = serial_number;
    memset(serial_number, 0, sizeof serial_number);
    if (get_odm_SN (*serial_number_ptr, sizeof serial_number) <= 0)
    {
        result = -1;
        strlcpy (serial_number, "Not available", sizeof serial_number);
        goto done;
    }

    result = 0;

done:
    return result;
}

