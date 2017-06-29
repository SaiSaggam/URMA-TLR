/*****************************************************************************
* Copyright(c)2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.By accepting transfer of this copy, Recipient agrees
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
*****************************************************************************/

#include "facres.h"

#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <libgen.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/signalfd.h>

#include "tlr_assert.h"
#include "digiutils.h"
#include "unittest_support.h"

#define     USE_IRQ_NOTIFICATION        0   // Set to use the Ralink GPIO driver to signal so we don't have to keep polling.

#if USE_IRQ_NOTIFICATION
#define CONFIG_RALINK_MT7621
#include "ralink_gpio.h"
#endif

//#define FACRES_DAEMON_NAME "facres"

#define     GPIO_RESET_BUTTON       18      // The GPIO for the reset button
#define     ONE_SECOND_MICROS       1000000
#define     USLEEP_TIME             (ONE_SECOND_MICROS/POLLS_PER_SECOND)

#if USE_IRQ_NOTIFICATION

STATIC_FUNCTION int register_with_driver (int const pin)
{
    int fd;
    ralink_gpio_reg_info info;

    fd = open("/dev/gpio", O_RDONLY);
    if (fd < 0)
    {
        goto gpio_error;
    }

    /* enable gpio interrupt */
    if (ioctl(fd, RALINK_GPIO_ENABLE_INTP) < 0)
    {
        goto gpio_error;
    }
    /* register my process information to the ralink driver so we are notified. */
    info.pid = getpid();
    info.irq = pin;
    if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0)
    {
        goto gpio_error;
    }
    close(fd);
    return 0;

gpio_error:
    if (fd >= 0)
    {
        close(fd);
    }
    return -1;
}

// Returns TRUE if SIGUSR1 or SIGUSR2
STATIC_FUNCTION bool read_signals(int const sfd)
{
    ssize_t     s;
    struct signalfd_siginfo fdsi;
    bool        ret_value = false;

    s = read(sfd, &fdsi, sizeof fdsi);
    if (s == sizeof fdsi)
    {
        if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2)
        {
            ret_value = true;
        }
    }

    return ret_value;
}

// Return a File descriptor so can read signals to this process
STATIC_FUNCTION int get_signalfd(void)
{
    sigset_t mask;
    int     sfd = -1;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    {
        goto get_sig_error;
    }
    sfd = signalfd(-1, &mask, 0);

get_sig_error:
    return sfd;
}

#endif

STATIC_FUNCTION bool process_gpio_read_input_line(char * const line, int * const input_value)
{
    return (sscanf(line, " bit%*u = 0x%x", input_value) == 1);
}


STATIC_FUNCTION int read_gpio(int gpio_number)
{
    FILE     * fp = NULL;
    char     * command = NULL;
    char     * line = NULL;
    size_t  line_len = 0;
    int      bytes_count;
    int      value_read = -1;

    bytes_count = asprintf(&command, "/usr/bin/gpio r %d", gpio_number);
    if (bytes_count == -1)
    {
        goto ret;
    }
    fp = popen(command, "r");
    if (fp == NULL)
    {
        goto ret;
    }

    while ((bytes_count = getline(&line, &line_len, fp)) != -1)
    {
        if (process_gpio_read_input_line(line, &value_read))
        {
            break;
        }
    }

ret:
    if (fp != NULL)
    {
        pclose(fp);
    }
    free(line);
    free(command);
    return value_read;
}


STATIC_FUNCTION void do_fact_reset(void)
{
    // Do the factory reset.
    system_command("dofacres");
}




STATIC_FUNCTION t_reset_pb_state reset_pb_state_machine(t_reset_pb_state current_state, bool const pb_pressed)
{
    static int     pb_count = 0;

    tlr_assert(pb_count>=0 && pb_count <= COUNT_FOR_BUTTON);

    switch (current_state)
    {
        case RESET_PB_WAITING_FOR_RELEASE:
            if (!pb_pressed)
            {
                current_state = RESET_PB_WAITING_FOR_PRESS;
            }
            break;
        case RESET_PB_WAITING_FOR_PRESS:
            if (pb_pressed)
            {
                current_state = RESET_PB_COUNTING_PRESS;
                pb_count = 1;
            }
            break;
        case RESET_PB_COUNTING_PRESS:
            if (pb_pressed)
            {
                if (++pb_count == COUNT_FOR_BUTTON)
                {
                    // RESET
                    do_fact_reset();
                    current_state = RESET_PB_WAITING_FOR_RELEASE;
                }
            }
            else
            {
                current_state = RESET_PB_WAITING_FOR_PRESS;
            }
            break;
    }
    return current_state;
}


int facres_worker(void)
{
    int     gpio_value;
    // Start in this state and that way if theres a hardware problem with the button
    // we won't be continually factory resetting the unit.
    t_reset_pb_state        reset_pb_state = RESET_PB_WAITING_FOR_RELEASE;


#if USE_IRQ_NOTIFICATION
    int     signalfd;
    int     hooked_gpio = -1;

    if ((signalfd = get_signalfd()) != -1)
    {
        hooked_gpio = register_with_driver(GPIO_RESET_BUTTON);
    }
#endif

    syslog(LOG_INFO, "facres worker");
    while (1)
    {
        usleep(USLEEP_TIME);
#if USE_IRQ_NOTIFICATION
        if (hooked_gpio == 0 && reset_pb_state == RESET_PB_WAITING_FOR_PRESS)
        {
            // If have successfully latched into the GPIO notification signalling from
            // the Ralink GPIO driver we can just wait here until notified something
            // has happenend in this state.
            read_signals(signalfd);  // Blocks (and uses no CPU) until there is a signal (i.e. push button change)
        }
#endif

        if ((gpio_value = read_gpio(GPIO_RESET_BUTTON)) != -1)
        {
            bool    const pb_pressed = (gpio_value == 0);

            reset_pb_state = reset_pb_state_machine(reset_pb_state, pb_pressed);
        }
    }

    return EXIT_SUCCESS;
}



