/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
****************************************************************************/
#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <stdio.h>
#include "connector_debug.h"
#include <syslog.h>

#define APP_DEBUG(x...)   do { syslog(LOG_DEBUG, x); } while (0)
#define APP_INFO(x...)    do { syslog(LOG_INFO, x); } while (0)

#define UNUSED_ARGUMENT(x)     ((void)x)

/* GCC emits a warning message for alignment issues when casting. Normally we fix these by reorganizing our structures.
   In some cases these structures are in system header files and cannot be changed. This macro should be used in those situations. */
#define cast_for_alignment(cast, ptr)   ((cast) ((void *) (ptr)))


typedef enum {
    connector_connector_idle,
    connector_connector_running
} connector_connector_state_t;

extern connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
                                                          connector_request_id_t const request_id,
                                                          void * const data, void * const context);

/* Callbacks for this platform */
extern connector_callback_status_t app_os_handler(connector_request_id_os_t const request,
                                                  void * const data);

extern connector_callback_status_t app_network_tcp_handler(connector_request_id_network_t const request,
                                                           void * const data);

extern connector_callback_status_t app_network_udp_handler(connector_request_id_network_t const request,
                                                           void * const data);

extern connector_callback_status_t app_network_sms_handler(connector_request_id_network_t const request,
                                                           void * const data);

extern connector_callback_status_t app_config_handler(connector_request_id_config_t const request,
                                                      void * const data);

#if (defined CONNECTOR_FILE_SYSTEM)
extern connector_callback_status_t app_file_system_handler(connector_request_id_file_system_t const request, void * const data);
#endif

#if (defined CONNECTOR_FIRMWARE_SERVICE)
connector_callback_status_t app_firmware_handler(connector_request_id_firmware_t const request_id, void * const data);
#endif

extern int application_run(connector_handle_t handle);

extern connector_callback_status_t app_os_get_system_time(unsigned long * const uptime);

extern connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status);
extern connector_callback_status_t app_status_handler(connector_request_id_status_t const request,
                                                      void * const data);

#if !(defined APP_SSL_CA_CERT_PATH)
#define APP_SSL_CA_CERT_PATH   "../../../../public/certificates/Digi_Int-ca-cert-public.crt"
#endif
#endif
