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
#include <pthread.h>
#include "connector_api.h"
#include "platform.h"
#include "cc_status.h"
#include <config_mgr.h>
#include <daemonize.h>
#include "show_parse_helper.h"
#include <unittest_support.h>

static bool get_config_value(config_cloud_t const element, unsigned int * const value, config_mgr_element_type_t const type)
{
    connector_callback_status_t const status = tlr_get_element_value(config_cloud, FIRST_INSTANCE, element,
                                                                     value, sizeof *value, type, config_mgr_access_level_read_only);
    bool const success = (status == config_status_success);

    return success;
}

static bool cc_is_cloud_enabled(void)
{
    unsigned int state;
    bool enabled = get_config_value(config_cloud_state, &state, config_mgr_element_type_on_off);

    if (!enabled) goto done;

    enabled = (state == 1);

done:
    return enabled;
}

static bool wait_to_reconnect(void)
{
    unsigned int reconnect_sec;
    bool success = get_config_value(config_cloud_reconnect, &reconnect_sec, config_mgr_element_type_uint32);

    if (!success) goto done;

    for (;;)
    {
        cloud_status_t const * const status_ptr = cc_get_cloud_status();

        if (status_ptr->config_changed) break;

        sleep(1);

        /* it is intentional to stay here if the reconnect_sec is set to 0 */
        if (reconnect_sec > 0)
        {
            reconnect_sec--;
            if (reconnect_sec == 0) break;
        }
    }

done:
    return success;
}

static void * connector_run_thread(void * handle)
{
    APP_DEBUG("connector_run thread starts\n");

    for (;;)
    {
        connector_status_t const status = connector_run(handle);

        APP_DEBUG("connector_run returns %d\n", status);

        if (status != connector_open_error) break;
    }

    APP_DEBUG("connector_run thread exits\n");
    cc_set_disconnected();
    cc_set_restart_cloud();
    pthread_exit(handle);

    return NULL;
}

static void * application_run_thread(void * arg)
{
    int status;

    APP_DEBUG("application_run thread starts\n");

    status = application_run(arg);

    APP_DEBUG("application_run thread exits %d\n", status);

    pthread_exit(arg);

    return NULL;
}

static int cloudc_start(void)
{
    pthread_t connector_thread;
    pthread_t application_thread;
    connector_handle_t connector_handle;
    bool const listening = cc_socket_init();

    if (!listening)
    {
        APP_DEBUG("failed to start show cloud thread\n");
    }

    daemon_confirm_is_running();

    for(;;)
    {
        if (!cc_is_cloud_enabled())
        {
            sleep(2);
            continue;
        }

        cc_clear_cloud_status();
        connector_handle = connector_init(app_connector_callback, NULL);

        if (connector_handle != NULL)
        {
            int ccode = pthread_create(&connector_thread, NULL, connector_run_thread, connector_handle);
            if (ccode != 0)
            {
                APP_DEBUG("thread_create() error on connector_run_thread %d\n", ccode);
                goto done;
            }

            ccode = pthread_create(&application_thread, NULL, application_run_thread, connector_handle);
            if (ccode != 0)
            {
                APP_DEBUG("thread_create() error on application_run_thread %d\n", ccode);
                goto done;
            }

            APP_INFO("cloud started\n");
            pthread_join(connector_thread, NULL);
            pthread_join(application_thread, NULL);
            APP_INFO("cloud terminated\n");
        }
        else
        {
            APP_INFO("Unable to initialize the connector\n");
            break;
        }

        if (!wait_to_reconnect()) break;

    }

done:
    APP_INFO("cloud exited\n");
    return 0;
}

#if (!defined UNIT_TEST)
int main (int argc, char * argv[])
{
    return start_daemon(argc, argv, cloudc_start, true);
}
#endif
