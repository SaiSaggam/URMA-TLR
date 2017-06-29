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

#include "tlr_format_string.h"
#include "serial_number.h"
#include "tlr_common.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static char * get_serial_number(void)
{
    char * serial_number;

    if (get_tlr_serial_number(&serial_number) != 0)
    {
        /* If get_tlr_serial_number returns non-zero, the string will be "Not Available", which
         * isn't what we want. Replace this with an empty string.
         */
        serial_number = "";
    }

    return serial_number;
}

/*
 * tlr_replace_percent_s_with_serial_number().
 * Replaces the first instance of %s found in the input string with the unit's serial number.
 * Returns: 0 if the replacements were performed without any truncation.
 *          1 if some truncation was performed.
*/
int tlr_replace_percent_s_with_serial_number(char const * const input_string, char * const output, size_t const output_size)
{
    size_t maximum_formatted_length = output_size - 1; /* Allow for the NUL terminator */
    size_t formatted_length;
    char const * psrc;
    char * pdst;
    bool done_serial_number;
    bool truncation_occurred = false;

    done_serial_number = false;
    formatted_length = 0;
    pdst = output;
    psrc = input_string;

    while (!STRING_IS_EMPTY(psrc) && formatted_length < maximum_formatted_length)
    {
        // Substitute the first occurrence of "%s" with the unit's serial number.
        if (!done_serial_number && *psrc == '%' && *(psrc+1) == 's')
        {
            size_t serial_number_length;
            int required_length;

            /* snprintf returns the number of bytes that would have been printed if the buffer
             * was large enough. It does not return the number of bytes that were printed.
             * Include the space for the NUL terminator.
             */
            required_length = snprintf(pdst, maximum_formatted_length - formatted_length + 1, "%s", get_serial_number());
            serial_number_length = strlen(pdst);
            if (required_length > serial_number_length)
            {
                truncation_occurred = true;
            }
            pdst += serial_number_length;
            formatted_length += serial_number_length;
            psrc++; /* skip the '%'. The psrc++ below will skip the 's' */
            done_serial_number = true;
        }
        else
        {
            *pdst++ = *psrc;
            formatted_length++;
        }
        psrc++;
    }
    *pdst = '\0';

    if (!STRING_IS_EMPTY(psrc))
    {
        truncation_occurred = true;
    }

    return truncation_occurred;
}

void tlr_string_strip_trailing_newline(char * const string)
{
    while (!STRING_IS_EMPTY(string))
    {
        size_t const last_char_index = strlen(string) - 1;
        bool const char_should_be_stripped = string[last_char_index] == '\r' || string[last_char_index] == '\n';

        if (char_should_be_stripped)
        {
            string[last_char_index] = '\0';
        }
        else
        {
            break;
        }
    }
}

