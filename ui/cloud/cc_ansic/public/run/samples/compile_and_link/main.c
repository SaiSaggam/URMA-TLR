/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "connector_api.h"
#include "connector_config.h"

int main (void)
{
    /*
     * Call connector_init() with a NULL callback, we are only verifying that we
     * can compile and link Cloud Connector.
     */
    (void)connector_init((connector_callback_t)0, NULL);
    return 0;
}

int connector_snprintf(char * const str, size_t const size, char const * const format, ...)
{
    va_list args;
    int result;

    va_start(args, format);

#if __STDC_VERSION__ >= 199901L
    result = vsnprintf(str, size, format, args);
#else
    /*************************************************************************
     * NOTE: Decided to have 64 bytes here considering following assumption  *
     * 1. In the worst case, only one format specifier will be used.         *
     * 2. Maximum of 48 bytes are used to represent a single precision value *
     *************************************************************************/
    #define SAFE_BUFFER_BYTES 64

    if (size >= SAFE_BUFFER_BYTES)
    {
        result = vsprintf(str, format, args);
    }
    else
    {
        char local_buffer[SAFE_BUFFER_BYTES];
        size_t const bytes_needed = vsprintf(local_buffer, format, args);

        result = (bytes_needed < size) ? bytes_needed : size - 1;
        memcpy(str, local_buffer, result);
        str[result] = '\0';
        result = bytes_needed;
    }
    #undef SAFE_BUFFER_BYTES
#endif
    va_end(args);

    return result;
}


#if defined(CONNECTOR_DEBUG)

#include "connector_debug.h"

void connector_debug_vprintf(debug_t const debug, char const * const format, va_list args)
{
    if ((debug == debug_all) || (debug == debug_beg))
    {
        /* lock mutex here. */
        printf("CC: ");
    }

    vprintf(format, args);

    if ((debug == debug_all) || (debug == debug_end))
    {
        /* unlock mutex here */
        printf("\n");
        fflush(stdout);
    }
}

#endif
