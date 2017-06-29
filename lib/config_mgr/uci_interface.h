
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

#ifndef UCI_INTERFACE_H_
#define UCI_INTERFACE_H_

#include <stdbool.h>
#include "uci.h"
#include "config_status.h"

typedef struct {
    char const * package;
    char const * section;
    char const * option;
    char const * value;
    size_t index;
    bool commit;
} uci_config_request_t;

typedef struct
{
#define MAX_VALUE_BYTES    256
    struct uci_context * default_context;
    struct uci_context * modified_context;
    struct uci_context * context_to_use;
    struct uci_ptr uci_data;
    char option_value[MAX_VALUE_BYTES];
} uci_config_data_t;

/*
 * Returns the handle which can be used for subsequent UCI calls in a session.
 */
void * uci_config_alloc(void);

/*
 * This UCI C API routine will be called to get the string associated
 * with a UCI variable. The string will be copied to the value field of
 * the request parameter. If the variable is not found in the modified UCI
 * file, the default UCI file is checked. If The variable is not found
 * in either file, a zero length string is placed it the value field of
 * the request parameter.
 */
config_status_t uci_config_get_option(void * const handle, uci_config_request_t * const request);

/*
 * This UCI C API routine will be called to set the string associated with a UCI variable.
 * If the "commit" flag in the request parameter is true, a UCI commit command is also sent
 * and an interface manager API is called for the change to take effect immediately.
 */
config_status_t uci_config_set_option(void * const handle, uci_config_request_t const * const request, bool const is_list);

/*
 * This UCI C API routine will be called to commit a package. This API writes the
 * current UCI settings to the configuration file to make the change permanent.
 */
config_status_t uci_config_save(void * const handle, uci_config_request_t const * const request);

/*
 * This UCI C API routine will be called to revert a package.
 */
config_status_t uci_config_revert(void * const handle, uci_config_request_t const * const request);

/*
 * This UCI C API routine will be called to free the handle and other allocated
 * internal resources. This function must be called after making a call to the
 * uci_config_alloc() function.
 */
void uci_config_free(void * const handle);

/*
 * Initialization routine called only once during initialization.
 */
config_status_t uci_config_init(void);

char * uci_get_option(char const * const package, char const * const section, size_t const instance, char const * const option);

#endif /* UCI_INTERFACE_H_ */
