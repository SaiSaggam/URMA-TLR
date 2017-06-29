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

STATIC void rci_set_buffer(rci_buffer_t * const dst, rci_service_buffer_t const * const src)
{
    uint8_t * const start = src->data;

    dst->start = start;
    dst->end = start + src->bytes;
    dst->current = start;
}

STATIC size_t rci_buffer_remaining(rci_buffer_t const * const buffer)
{
    size_t const remaining_length = (size_t)(buffer->end - buffer->current);
    return remaining_length;
}

STATIC size_t rci_buffer_used(rci_buffer_t const * const buffer)
{
    size_t const used_length = (size_t)(buffer->current - buffer->start);
    return used_length;
}

STATIC uint8_t * rci_buffer_position(rci_buffer_t const * const buffer)
{
    return buffer->current;
}

STATIC void rci_buffer_advance(rci_buffer_t * const buffer, size_t const amount)
{
    ASSERT((buffer->current + amount) <= buffer->end);
    buffer->current += amount;
}

STATIC uint8_t rci_buffer_read(rci_buffer_t const * const buffer)
{
    ASSERT(rci_buffer_remaining(buffer) != 0);

    return *(buffer->current);
}

STATIC connector_bool_t ptr_in_range(void const * const pointer, void const * const start, void const * const end)
{
    return connector_bool((pointer >= start) && (pointer <= end));
}

STATIC connector_bool_t ptr_in_buffer(uint8_t const * const pointer, rci_buffer_t const * const buffer)
{
    return ptr_in_range(pointer, buffer->start, buffer->end);
}

