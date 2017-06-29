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
#ifndef __APP_DEBUG_H_
#define __APP_DEBUG_H_

#ifndef bool
typedef int bool;
#define false 0
#define true  1
#endif

extern int  app_verbose_mode(void);
extern void app_alt_printf(char const * const format, ...);

#define APP_ALT_PRINTF   app_alt_printf
#define APP_PRINTF       printf
#define app_show_progress()   APP_PRINTF(".")

#endif
