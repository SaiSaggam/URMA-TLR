/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#include "cellular_config.h"
#include "cellular_message.h"
#include "cellular_monitor.h"
#include "cellular_passthrough.h"
#include "digiutils.h"
#include "daemonize.h"

#include "monitor_debug.h"
#include "cellular_odis.h"


bool init_3g_initialized = false;


static void * init_3g_thread(void * data)
{
    #define MAX_MODEM_GET_FAILURES 40

    int result = EXIT_SUCCESS;
    size_t failed_modem_gets = 0;
    int fd = -1;

    monitor_debug("Start init_3g.sh\n");

    /* initialize AMIT 3G components */
    system_command("/usr/etc/init.d/init_3g.sh");

    monitor_debug_printf("init_3g_thread: pid = %u ppid = %u\n", getpid(), getppid());

    //sleep(10);

    while (fd < 0)
    {
        sleep(1);

        fd = cellular_open_status();

        if (fd >= 0)
        {
            monitor_debug("init_3g_thread: csman_fd %d\n", fd);
        }

        system_command("/usr/bin/cellular_pdp_init.sh");

    }

    for (;;)
    {
        bool usb_ready = cellular_get_usb_ready(fd, CELLULAR_INDEX_3G);

        if (usb_ready)
        {
            cellular_modem_t const * const modem = cellular_get_modem(fd, CELLULAR_INDEX_3G);

            if (modem != NULL)
            {
                monitor_info("modem support = %s 4G support = %d\n", modem->model, modem->support_4g);
                tlr_set_uci_tag(config_cellular_preferred_mode_4g_inc_4g_support_tag,
                                                modem->support_4g);
                break;
            }
            else if (failed_modem_gets >= MAX_MODEM_GET_FAILURES)
            {
                break;
            }

            failed_modem_gets++;
        }
        sleep(1);
    }

    for (failed_modem_gets = 0; failed_modem_gets < MAX_MODEM_GET_FAILURES; failed_modem_gets++)
    {

        if (cellular_set_odis_info(fd) == 0)
        {
            break;
        }

        sleep(1);
    }

    init_3g_initialized = true;

    cellular_close_status(fd);

    monitor_debug("Done init_3g.sh\n");

    pthread_exit(&result);

}

#ifndef CELLULAR_MODEM_STATUS_TIMER
static void * cellular_status_thread(void * data)
{
    int result = EXIT_SUCCESS;
    bool usb_ready = cellular_get_usb_ready(CELLULAR_INDEX_3G);


    while (!init_3g_initialized || !usb_ready)
    {
        sleep(1);
    }

    monitor_debug_printf("cellular_status_thread: pid = %u ppid = %u\n", getpid(), getppid());

    for (;;)
    {
        size_t i;

        for (i = 0; i < interface_instance_max; i++)
        {
            cellular_context_t * context = &cellular_contexts[i];
            monitor_cellular_status(context);
        }
        sleep(1);
    }
    pthread_exit(&result);
}

#endif

static void * cellular_led_thread(void * data)
{
    int result = EXIT_SUCCESS;
    int fd = -1;

    while (!init_3g_initialized)
    {
        sleep(1);
    }

    while (fd < 0)
    {
        fd = cellular_open_status();
        sleep(1);
    }

    monitor_debug("cellular_led_thread: csman_fd %d\n", fd);
    monitor_debug_printf("cellular_led_thread: pid = %u ppid = %u\n", getpid(), getppid());

    for (;;)
    {

        size_t i;

        for (i = 0; i < interface_instance_max; i++)
        {
            cellular_context_t * context = &cellular_contexts[i];
            monitor_cellular_led(fd, context);
        }

        sleep(1);
    }

    cellular_close_status(fd);
    pthread_exit(&result);
}

static int cellular_monitor_main(void)
{
    static pthread_t init_3g_thread_id;

    #ifndef CELLULAR_MODEM_STATUS_TIMER
    static pthread_t cellular_status_thread_id;
    #endif

    static pthread_t cellular_led_thread_id;

    int result;
    message_thread_st * message_thread_context;
    timer_context_st * timer_context;

    /* Create thread that starts AMIT 3G init script
     * and it will install all necessary modem drives
    */
    if (pthread_create(&init_3g_thread_id, NULL, init_3g_thread, NULL) != 0)
    {
        monitor_error("could not create init_3g thread\n");
        result = EXIT_FAILURE;
        goto done;
    }

    if (cellular_is_pass_through_enabled())
    {

        while (!init_3g_initialized)
        {
            /* wait AMIT 3G init to finish */
            sleep(1);
        }

        int fd = cellular_open_status();

        if (fd >= 0)
        {

            while (!cellular_get_usb_ready(fd, CELLULAR_INDEX_3G))
            {
                /* wait for USB ready */
                sleep(1);
            }

            cellular_close_status(fd);

            if (cellular_start_pass_through())
            {
                /* ok then exit */
                result = EXIT_SUCCESS;
                goto done;
            }
        }
        else
        {
            monitor_error("could not open CSMAN\n");
        }

    }

    /* timer thread must be started before the state machine */
    timer_context = open_timer_context();
    if (timer_context == NULL)
    {
        monitor_error("unable to create timer thread\n");
        result = EXIT_FAILURE;
        goto done;
    }

    /*
        now start the state machine, which should run in a thread, allowing multiple state
        machines to run
    */
    if (init_cellular_monitor(timer_context) != 0)
    {
        monitor_error("Unable to initialize cellular monitor\n");
        result = EXIT_FAILURE;
        goto done;
    }


    if (start_cellular_monitor() != 0)
    {
        monitor_error("Unable to start cellular monitor\n");
        result = EXIT_FAILURE;
        goto done;
    }

    /*
        start the message handling thread. This must be done after starting the state machines so
        that events received over the messaging socket aren't delivered into a non-existent event
        queue
    */
    message_thread_context = open_messaging_context(cellular_monitor, monitor_message_callback);
    if (message_thread_context == NULL)
    {
        monitor_error("Unable to start monitor_message_callback\n");
        result = EXIT_FAILURE;
        goto done;
    }

#ifndef CELLULAR_MODEM_STATUS_TIMER
    /* create thread that starts AMIT 3G init script */
    if (pthread_create(&cellular_status_thread_id, NULL, cellular_status_thread, NULL) != 0)
    {
        monitor_error("could not create cellular status thread\n");
        result = EXIT_FAILURE;
        goto done;
    }

#endif

    {
        int status = pthread_create(&cellular_led_thread_id, NULL, cellular_led_thread, NULL);

        if (status != 0)
        {
            monitor_error("could not create cellular led thread %d\n", status);
            result = EXIT_FAILURE;
            goto done;
        }
    }

    daemon_confirm_is_running();

    result = EXIT_SUCCESS;

    /* this will block until all child threads end */
    pthread_exit(&result);


    /* shouldn't get here */

done:
    /* should only get here on error */
    close_cellular_monitor();
    return result;
}

int main(int argc, char ** argv)
{
    return start_daemon(argc, argv, cellular_monitor_main, true);
}

