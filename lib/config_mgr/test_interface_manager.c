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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery.h>
#include "uci_interface.h"
#include "interface_manager.h"

extern void * im_get_element(void * handle, char const * const element_name);

#if UNIT_TESTING
void test_im_get_element_no_handle(void **state) {
	expect_assert_failure(im_get_element(NULL, RX_BYTES));
}

void test_im_get_element_no_element_name(void **state)
{
    im_request_t *im_data;

    im_data = (im_request_t * )im_alloc("eth", 0);
    expect_assert_failure(im_get_element(im_data, NULL));
	im_free(im_data);
}
#endif

