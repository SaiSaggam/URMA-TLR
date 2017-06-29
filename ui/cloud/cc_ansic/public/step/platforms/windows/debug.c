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
#include "connector_config.h"

#if defined(CONNECTOR_DEBUG)
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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

#else
/* to avoid ISO C forbids an empty translation unit compiler error */
typedef int dummy;

#endif
