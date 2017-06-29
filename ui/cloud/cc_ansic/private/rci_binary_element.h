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

#define ELEMENT_ID_VARIABLE(rci)        ((rci)->shared.element.id)
#define set_element_id(rci, value)      (ELEMENT_ID_VARIABLE(rci) = (value))
#define get_element_id(rci)             (ELEMENT_ID_VARIABLE(rci))
#define invalidate_element_id(rci)      set_element_id(rci, INVALID_ID)
#define have_element_id(rci)            (get_element_id(rci) != INVALID_ID)

static connector_group_element_t const * get_current_element(rci_t const * const rci)
{
    ASSERT(have_group_id(rci));
    ASSERT(have_element_id(rci));

    {
        connector_group_t const * const group = get_current_group(rci);
        unsigned int const id = get_element_id(rci);

        ASSERT(id < group->elements.count);

        return (group->elements.data + id);
    }
}
