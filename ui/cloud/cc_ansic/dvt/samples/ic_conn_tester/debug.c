/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include "connector_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "app_debug.h"


#if defined(CONNECTOR_DEBUG)

void app_alt_printf(char const * const format, ...)
{
    if (!app_verbose_mode())
    {
        va_list args;

        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        fflush(stdout);
    }
    else
    {
        (void) format;
    }
}

void connector_debug_printf(char const * const format, ...)
{
    if (app_verbose_mode())
    {
        va_list args;

        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        fflush(stdout);
    }
    else
    {
        (void) format;
    }
}
#else

void connector_debug_printf(char const * const format, ...)
{
    (void) format;
}
void app_alt_printf(char const * const format, ...)
{
 (void) format;
}
 
#endif
