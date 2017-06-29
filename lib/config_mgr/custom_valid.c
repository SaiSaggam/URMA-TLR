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
****************************************************************************/


#include "config_table.h"
#include "custom_valid.h"
#include "config_mgr.h"
#include "tlr_print.h"

#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <malloc.h>

#if defined VALIDATE_ACTION_DATE_TIME
bool validate_action_date_time (char const * const string, size_t const min_length, size_t const max_length)
{
    bool is_valid = validate_string (string, min_length, max_length);

    return is_valid;
}

#endif


#if defined VALIDATE_ACTION_DATE_DATE
bool validate_action_date_date (char const * const string, size_t const min_length, size_t const max_length)
{

    bool is_valid = validate_string (string, min_length, max_length);

    return is_valid;
}
#endif

