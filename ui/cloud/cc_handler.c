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
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"
#include "cc_status.h"
#include <stdbool.h>
#include <unistd.h>

static void * cc_handle = NULL;

connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    connector_bool_t type;

    switch (status)
    {
           /* if either Device Cloud or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
        case connector_close_status_device_stopped:
        case connector_close_status_abort:
            type = connector_false;
             break;

       /* otherwise it's an error and we want to retry, after timeout */
       default:
       {
           connector_status_t result = connector_initiate_action(cc_handle, connector_initiate_terminate, NULL);

           if (result != connector_success)
           {
               APP_DEBUG("failed to terminate the cloud connection\n");
           }

           type = connector_true;
           break;
       }
    }

    return type;
}

connector_callback_status_t app_connector_callback(connector_class_id_t const class_id, connector_request_id_t const request_id,
                                                   void * const data, void * const context)
{
    connector_callback_status_t status = connector_callback_unrecognized;

    UNUSED_ARGUMENT(context);

    switch (class_id)
    {
    case connector_class_id_config:
        status = app_config_handler(request_id.config_request, data);
        break;

    case connector_class_id_operating_system:
        status = app_os_handler(request_id.os_request, data);
        break;

    case connector_class_id_network_tcp:
        status = app_network_tcp_handler(request_id.network_request, data);
        break;

    case connector_class_id_status:
        status = app_status_handler(request_id.status_request, data);
        break;

    case connector_class_id_remote_config:
        status = app_remote_config_handler(request_id.remote_config_request, data);
        break;

    case connector_class_id_firmware:
        status = app_firmware_handler(request_id.firmware_request, data);
        break;

    default:
        /* not supported */
        break;
    }
    return status;
}

int application_run(connector_handle_t handle)
{
    volatile cloud_status_t * const status_ptr = cc_get_cloud_status();
    cc_handle = handle;

    for (;;)
    {
        if (status_ptr->restart_cloud) break;

        sleep(2);
    }

    if (status_ptr->config_changed)
    {
        connector_initiate_action(cc_handle, connector_initiate_terminate, NULL);
    }

    return 0;
}

