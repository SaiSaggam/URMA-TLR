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
#include "connector_api.h"
#include "connector_config.h"

#if (defined CONNECTOR_DEBUG)
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void connector_debug_printf(char const * const format, ...)
{
    va_list args;

    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}
#else
 /* to avoid ISO C forbids an empty translation unit compiler error */
typedef int dummy;

#endif

int main (void)
{
    /*
     * Call connector_init() with a NULL callback, we are only verifying that we
     * can compile and link the iDigi Connector.
     */
	(void)connector_init((connector_callback_t)0, NULL);
    return 0;
}

