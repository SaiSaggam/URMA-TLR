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

#include "show_status_helper.h"

#include <stdio.h>
#include <stdarg.h>

static void fprint_char_repeatedly(FILE * const stream, char const char_to_repeat, size_t const repeat_times)
{
    int i;

    for (i = 0; i < repeat_times; i++)
    {
        fputc(char_to_repeat, stream);
    }
}

static void fprint_divider(FILE * const stream, size_t const line_length)
{
    fputc(' ', stream);
    fprint_char_repeatedly(stream, '-', line_length);
    fputc('\n', stream);
}

static void vfprintf_status_header(FILE * const stream, size_t const divider_width, char const * const format, va_list args)
{
    int header_length;
    int dashes_required;

    header_length = vfprintf(stream, format, args);

    if (divider_width == DIVIDER_WIDTH_AUTO)
    {
        dashes_required = header_length - 2;    /* subtract off the leading ' ' and trailing '\n' */
    }
    else
    {
        dashes_required = divider_width;
    }

    fprint_divider(stream, dashes_required);
}

void printf_status_header(size_t const divider_width, char const * const format, ...)
{
    va_list args;

    va_start(args, format);

    vfprintf_status_header(stdout, divider_width, format, args);

    va_end(args);
}

void fprintf_status_header(FILE * const stream, size_t const divider_width, char const * const format, ...)
{
    va_list args;

    va_start(args, format);

    vfprintf_status_header(stream, divider_width, format, args);

    va_end(args);
}

