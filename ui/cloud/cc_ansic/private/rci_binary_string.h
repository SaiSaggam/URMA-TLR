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

static uint8_t const * rcistr_data(rcistr_t const * const string)
{
    return string->data;
}

STATIC void clear_rcistr(rcistr_t * const string)
{
    string->data = NULL;
    string->length = 0;
}

STATIC connector_bool_t rcistr_valid(rcistr_t const * const string)
{
    return connector_bool((string->data != NULL) && (string->length != 0));
}

STATIC void adjust_char_pointer(uint8_t * const new_base, uint8_t const * const old_base, uint8_t * * const pointer)
{
    size_t const offset = (size_t)(*pointer - old_base);

    *pointer = (new_base + offset);
}

STATIC void adjust_rcistr(uint8_t * const new_base, uint8_t const * const old_base, rcistr_t * const string)
{
    uint8_t * pointer = (uint8_t *) string->data;

    adjust_char_pointer(new_base, old_base, &pointer);
    string->data = pointer;
}



