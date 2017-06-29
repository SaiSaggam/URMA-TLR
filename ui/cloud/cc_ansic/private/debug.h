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

#if (defined CONNECTOR_DEBUG)

#include <stdarg.h>
#include "connector_debug.h"

#if !(defined CALL_DEBUG_VPRINTF)
#define CALL_DEBUG_VPRINTF(type, format) \
    do \
    { \
        va_list args; \
 \
        va_start(args, (format)); \
        connector_debug_vprintf((type), (format), args); \
        va_end(args); \
    } \
    while (0)
#endif

#else

#if (defined CALL_DEBUG_VPRINTF)
#undef CALL_DEBUG_VPRINTF
#endif

#define CALL_DEBUG_VPRINTF(type, format) UNUSED_PARAMETER(format)

#endif

STATIC void connector_debug_line(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_all, format);
}

#if (defined CONNECTOR_DEBUG)

STATIC void connector_debug_line_beg(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_beg, format);
}

STATIC void connector_debug_line_mid(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_mid, format);
}

STATIC void connector_debug_line_end(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_end, format);
}

#define enum_to_case(name)  case name:  result = #name;             break

#if !(defined CONNECTOR_DEBUG_NEW_LINE_STR)
#define CONNECTOR_DEBUG_NEW_LINE_STR	"\n"
#endif

void connector_debug_print_buffer(char const * const label, void const * const buffer, size_t const length)
{
    size_t i;
    uint8_t const * const content = buffer;

    connector_debug_line_beg("%s:", label);
    for (i = 0; i < length; i++)
    {
        if ((i % 16) == 0)
        {
            connector_debug_line_mid(CONNECTOR_DEBUG_NEW_LINE_STR);
        }

        connector_debug_line_mid(" %02X", content[i]);
    }
    connector_debug_line_end("");
}

#else
#define connector_debug_print_buffer(label, buffer, length)

#define CALL_DEBUG_VPRINTF(type, format) UNUSED_PARAMETER(format)

#endif

#if (defined CONNECTOR_DEBUG)
static char const * transport_to_string(connector_transport_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        #if (defined CONNECTOR_TRANSPORT_TCP)
        enum_to_case(connector_transport_tcp);
        #endif
        #if (defined CONNECTOR_TRANSPORT_UDP)
        enum_to_case(connector_transport_udp);
        #endif
        #if (defined CONNECTOR_TRANSPORT_SMS)
        enum_to_case(connector_transport_sms);
        #endif
        enum_to_case(connector_transport_all);
    }
    return result;
}
#else
#define transport_to_string(value)       NULL
#endif

