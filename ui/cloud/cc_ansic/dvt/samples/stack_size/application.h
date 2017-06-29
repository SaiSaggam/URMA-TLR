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

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdarg.h>
#include "stdbool.h"
#include "connector_api.h"

#define TERMINATE_TEST_FILE "terminate.test"
#define STACK_SIZE_FILE "./dvt/stacksize.html"
//#define STACK_SIZE_FILE "../../stacksize.html"

extern connector_handle_t connector_handle;
extern connector_status_t connector_run_thread_status;
extern size_t total_malloc_size;

#define STACKSIZE_HTML_OPEN_BODY "\
<html>\n\
  <head>\n\
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"/>\n\
    <title>Etherios Cloud Connector API Timing</title>\n\
  </head>\n\
  <body>\n\
 \n\
    <h2>Etherios Cloud Connector Stack Size</h2>\n\
    <p>It calculates the maximum stack size used in connector_run() excluding callback's stack size usage.</p>\n\
    "
#define STACKSIZE_HTML_TABLE   "\
<table border=\"2\">\n"

#define STACKSIZE_HTML_TABLE_ROW(format)   "\
    <tr><th align=\"left\">%s</th><td align=\"right\">" format "</td></tr>\n"

#define STACK_SIZE_HTML_OPTION_LIST_OPEN  "    <tr><th align=\"left\">Options</th><td align=\"right\"><dl>"
#define STACK_SIZE_HTML_OPTION_LIST       "    <dd>%s</dd>"
#define STACK_SIZE_HTML_OPTION_LIST_CLOSE "    </dl></td></tr></table>"
#define STACKSIZE_HTML_CLOSE_BODY     "       </body></html>\n"


extern connector_callback_status_t app_data_service_handler(connector_request_id_data_service_t const request_id,
                                                  void * const data);
extern connector_callback_status_t app_firmware_handler(connector_request_id_firmware_t const request_id,
                                                  void * const data);

extern connector_callback_status_t app_remote_config_handler(connector_request_id_remote_config_t const request_id,
                                                      void * const data);

extern connector_status_t send_put_request(connector_handle_t handle);

extern void clear_stack_size(void);
extern void write_stack_info_action(char const * const action);

#endif /* APPLICATION_H_ */
