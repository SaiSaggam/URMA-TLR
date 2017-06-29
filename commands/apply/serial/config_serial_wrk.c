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
#include "config_mgr.h"
#include "config_def.h"
#include "tlr_common.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


#define LOCK_FILE "/var/lock/ttyUSB"

static char * tty_devices[] = { "/dev/ttyUSB3", "/dev/ttyUSB2",  "/dev/ttyUSB1", "/dev/ttyUSB0" };

bool is_enabled(tlr_handle_t tlr_handle)
{
    bool rc = false;
    char buffer[8];
    config_status_t status;

    status =  tlr_config_read(tlr_handle, config_serial_state, buffer, sizeof buffer);
    if (status != config_status_success)
    {
        fprintf(stderr, "Unable to read serial state configuration . Error %d\n", status);
        goto ret;
    }

    {
        unsigned int value;

        status = tlr_config_string_to_value(config_serial, config_serial_state, buffer, &value);
        if (status != config_status_success)
        {
            fprintf(stderr, "Invalid state configuration. Error %d\n", status);
            goto ret;
        }

        rc = (value == 1);
    }

ret:
    return rc;
}

int set_baud(tlr_handle_t tlr_handle, struct termios * tio)
{
    int rc = 0;
    char buffer[16];
    config_serial_baud_t value;
    config_status_t status;

    static unsigned int const baud_rates[] = {B110, B300, B600, B1200, B2400, B4800, B9600,
                                //B14400,
                                B19200, B38400, B57600, B115200, B230400, B460800, B921600};

    status =  tlr_config_read(tlr_handle, config_serial_baud, buffer, sizeof buffer);
    if (status != config_status_success)
    {
        fprintf(stdout, "Unable to read serial baud configuration . Error %d\n", status);
        rc = -1;
        goto ret;
    }

    status = tlr_config_string_to_value(config_serial, config_serial_baud, buffer, &value);
    if ((status != config_status_success) || (value >= sizeof baud_rates/sizeof baud_rates[0]))
    {
        fprintf(stdout, "Invalid baud rate configuration. Error %d\n", status);
        rc = -1;
        goto ret;
    }

    switch (value)
    {
    case config_serial_baud_110:
    case config_serial_baud_300:
    case config_serial_baud_600:
    case config_serial_baud_1200:
    case config_serial_baud_2400:
    case config_serial_baud_4800:
    case config_serial_baud_9600:
    case config_serial_baud_19200:
    case config_serial_baud_38400:
    case config_serial_baud_57600:
    case config_serial_baud_115200:
    case config_serial_baud_230400:
    case config_serial_baud_460800:
    case config_serial_baud_921600:
        break;
    }

    cfsetospeed(tio, baud_rates[value]);
    cfsetispeed(tio, baud_rates[value]);

ret:
    return rc;
}

typedef struct databits_config_to_native_value_st databits_config_to_native_value_st;
struct databits_config_to_native_value_st
{
    config_serial_databits_t config;
    unsigned int native;
};

static const databits_config_to_native_value_st databits_config_to_native_values[] =
{
    {
        .config = config_serial_databits_8,
        .native = CS8
    },
    {
        .config = config_serial_databits_7,
        .native = CS7
    }
};
#define NUM_DATABITS_CONFIG_VALUES ARRAY_SIZE(databits_config_to_native_values)

static bool get_native_databits_value_from_config(config_serial_databits_t const config, unsigned int * const native)
{
    size_t i;
    bool found_native_value;

    for (i = 0; i < NUM_DATABITS_CONFIG_VALUES; i++)
    {
        if (config == databits_config_to_native_values[i].config)
        {
            *native = databits_config_to_native_values[i].native;
            found_native_value = true;
            goto done;
        }
    }
    found_native_value = false;

done:

    return found_native_value;
}

int set_databits(tlr_handle_t tlr_handle, struct termios * tio)
{
    int rc = 0;
    char buffer[4];
    config_status_t status;

    status =  tlr_config_read(tlr_handle, config_serial_databits, buffer, sizeof buffer);
    if (status != config_status_success)
    {
        fprintf(stdout, "Unable to read serial data bits configuration . Error %d\n", status);
        rc = -1;
        goto ret;
    }

    {
        config_serial_databits_t config_value;
        unsigned int native_value;

        status = tlr_config_string_to_value(config_serial, config_serial_databits, buffer, &config_value);
        if (status != config_status_success)
        {
            fprintf(stdout, "Error getting databits value from configuration (%d)\n", status);
            rc = -1;
            goto ret;
        }
        if (!get_native_databits_value_from_config(config_value, &native_value))
        {
            fprintf(stdout, "Unsupported data bits configuration value %d\n", config_value);
            rc = -1;
            goto ret;
        }
        tio->c_cflag &= ~CSIZE;
        tio->c_cflag |= native_value;
    }

ret:
    return rc;
}

int set_parity(tlr_handle_t tlr_handle, struct termios * tio)
{
    int rc = 0;
    char buffer[8];
    config_status_t status;

    status =  tlr_config_read(tlr_handle, config_serial_parity, buffer, sizeof buffer);
    if (status != config_status_success)
    {
        fprintf(stdout, "Unable to read serial parity configuration . Error %d\n", status);
        rc = -1;
        goto ret;
    }

    {

        config_serial_parity_t value;

        status = tlr_config_string_to_value(config_serial, config_serial_parity, buffer, &value);
        if (status != config_status_success)
        {
            fprintf(stdout, "Invalid parity configuration. Error %d\n", status);
            rc = -1;
            goto ret;
        }

        switch (value)
        {
            case config_serial_parity_none:
                tio->c_iflag &= ~INPCK;
                tio->c_iflag |= IGNPAR;
                tio->c_cflag &= ~PARENB;
                break;
            case config_serial_parity_odd:
                //tio->c_iflag &= ~(IGNPAR | PARMRK);
                tio->c_iflag &= ~IGNPAR;
                tio->c_iflag |= INPCK;

                tio->c_cflag |= PARODD | PARENB;
                break;
            case config_serial_parity_even:
                //tio->c_iflag &= ~(IGNPAR | PARMRK);
                tio->c_iflag &= ~IGNPAR;
                tio->c_iflag |= INPCK;

                tio->c_cflag &= ~PARODD;
                tio->c_cflag |= PARENB;
                break;
#if 0
            case config_serial_parity_mark:
                tio->c_cflag |= PARENB | CMSPAR | PARODD;
                tio->c_cflag |= PARENB | CMSPAR | PARODD;
                tio->c_iflag |= INPCK;
                break;
            case config_serial_parity_space:
                tio->c_cflag |= PARENB | CMSPAR;
                tio->c_cflag &= ~PARODD;
                tio->c_iflag |= INPCK;
                break;
#endif
            default:
                fprintf(stdout, "Invalid parity configuration %d\n", value);
                rc = -1;
                goto ret;

        }
    }
ret:
    return rc;
}

int set_stopbits(tlr_handle_t tlr_handle, struct termios * tio)
{
    int rc = 0;
    char buffer[8];
    config_status_t status;

    status =  tlr_config_read(tlr_handle, config_serial_stopbits, buffer, sizeof buffer);
    if (status != config_status_success)
    {
        fprintf(stdout, "Unable to read serial stop bits configuration . Error %d\n", status);
        rc = -1;
        goto ret;
    }

    {
        config_serial_stopbits_t value;

        status = tlr_config_string_to_value(config_serial, config_serial_stopbits, buffer, &value);
        if (status != config_status_success)
        {
            fprintf(stdout, "Invalid stop bits configuration. Error %d\n", status);
            rc = -1;
            goto ret;
        }

        switch (value)
        {
            case config_serial_stopbits_1:
                tio->c_cflag &= ~CSTOPB;
                break;
            //case config_serial_stopbits_1_5:
                //break;
            case config_serial_stopbits_2:
                tio->c_cflag |= CSTOPB;
                break;
            default:
                fprintf(stdout, "Invalid stop bit %d\n", value);
                rc = -1;
                break;
        }
    }
ret:
    return rc;
}

int set_flowcontrol(tlr_handle_t tlr_handle, struct termios * tio)
{
    int rc = 0;
    char buffer[10];
    config_status_t status;

    status =  tlr_config_read(tlr_handle, config_serial_flowcontrol, buffer, sizeof buffer);
    if (status != config_status_success)
    {
        fprintf(stdout, "Unable to read serial flowcontrol configuration . Error %d\n", status);
        rc = -1;
        goto ret;
    }

    {
        config_serial_flowcontrol_t value;

        status = tlr_config_string_to_value(config_serial, config_serial_flowcontrol, buffer, &value);
        if (status != config_status_success)
        {
            fprintf(stdout, "Invalid flow control configuration %s. Error %d\n", buffer, status);
            rc = -1;
            goto ret;
        }

        switch (value)
        {
            case config_serial_flowcontrol_none:
                tio->c_cflag &= ~CRTSCTS;
                tio->c_iflag &= ~(IXON | IXOFF | IXANY);
                break;
            case config_serial_flowcontrol_hardware:
                tio->c_cflag |= CRTSCTS;
                tio->c_iflag &= ~(IXON | IXOFF | IXANY);
                break;
            case config_serial_flowcontrol_software:
                tio->c_cflag &= ~CRTSCTS;
                tio->c_iflag |= IXON | IXOFF | IXANY;
                break;
            default:
                fprintf(stdout, "Invalid flowcontrol %d\n", value);
                rc = -1;
                break;
        }
    }
ret:
    return rc;
}

#if 0
static int get_cli_process(char * const device)
{
    FILE * pid_fd;
    char cli_cmd[64];
    char line[64];
    int pid = -1;

    //pid_t killpid;
    //pid_t mypid   = getpid();
    //pid_t myppid  = getppid();
    snprintf(cli_cmd, sizeof cli_cmd, "pidof cli -m serial >%s 2>&1 <%s\n", device, device);
    pid_fd = popen(cli_cmd,"r");
    if (pid_fd != NULL)
    {
        while (fgets(line,sizeof line, pid_fd))
        {
            pid = atoi(line);
        }
        fclose(pid_fd);
    }

    return pid;
}
#endif


static unsigned int get_cli_pid(unsigned int const instance)
{
    unsigned int pid = 0;

    char device[32];

    snprintf(device, sizeof device, "/var/run/serial%d.pid", instance);
    int fd = open(device, O_RDONLY);

    fprintf(stdout, "%s fd %d\n", device, fd);

    if (fd >= 0)
    {
        ssize_t bytes_read;
        char buffer[32];

        bytes_read = read(fd, buffer, sizeof buffer);
        if (bytes_read > 0)
        {
            pid = atoi(buffer);
            fprintf(stdout, "%s pid = %d\n", device, pid);
        }

        close(fd);
    }

    return pid;

}
static void remove_cli(unsigned int const instance)
{
    char cmd[40];

    snprintf(cmd, sizeof cmd, "rm /var/run/serial%d.pid", instance);
    system(cmd);

}

int config_serial_main (int argc, char *argv[])
{
    int fd;
    struct termios oldtio;
    struct termios newtio;
    tlr_handle_t tlr_handle = NULL;
    int rc = -1;
    char * device;
    int instance_1_based = 1;

    if (argc < 3)
    {
        fprintf(stdout, "Missing arguments\n");
        fprintf(stdout, "Syntax:\n");
        fprintf(stdout, "    %s -i [instance]\n", argv[0]);
        goto done;
    }

    {
        int opt;

        while ((opt = getopt (argc, argv, "i:u:a:")) != -1)
        {
            switch (opt)
            {
            case 'i':
                instance_1_based = atoi(optarg);
                break;
            case 'u':
            case 'a':
                break;
            default:
                fprintf (stdout, "Unknown option '-%c'.\n", optopt);
                goto done;
            }
        }

        if ((instance_1_based == 0) || (instance_1_based > (sizeof tty_devices / sizeof tty_devices[0])))
        {
            fprintf(stdout, "Instance not supported %d\n", instance_1_based);
            goto done;
        }

        tlr_handle = tlr_config_open(config_serial, instance_1_based, config_mgr_access_level_read_only);
        if (tlr_handle == NULL)
        {
            fprintf(stdout, "Unable to read serial port %d configuration\n", instance_1_based);
            goto done;
        }

        {
            size_t index_0_based = instance_1_based -1;
            device = tty_devices[index_0_based];
        }
    }

#if 0
    lock_fd = file_lock();

    if (lock_fd < 0)
    {
        fprintf(stdout, "Unable to process the command\n");
        rc = -1;
        goto done;
    }
#endif

    fd = open(device, O_RDWR | O_NOCTTY );
    if (fd < 0)
    {
        perror(device);
        goto done;
    }

    tcgetattr(fd,&oldtio); /* save current serial port settings */
    memcpy(&newtio, &oldtio, sizeof newtio);

    //bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

//    config_serial_descr,

    if (set_baud(tlr_handle, &newtio) != 0) goto error;

    if (set_databits(tlr_handle, &newtio) != 0) goto error;

    if (set_parity(tlr_handle, &newtio) != 0) goto error;

    if (set_stopbits(tlr_handle, &newtio) != 0) goto error;

    if (set_flowcontrol(tlr_handle, &newtio) != 0) goto error;

    /*
    CLOCAL  : local connection, no modem contol
    CREAD   : enable receiving characters
    */
    newtio.c_cflag |= CLOCAL | CREAD;

    /*
    ICRNL   : map CR to NL (otherwise a CR input on the other computer
            will not terminate input)
    otherwise make device raw (no other input processing)
    */
    newtio.c_iflag |=  ICRNL;

    /*
    Raw output.
    */
    //newtio.c_oflag = 0;

    /*
    ICANON  : enable canonical input
    disable all echo functionality, and don't send signals to calling program
    */
    //newtio.c_lflag = ICANON;

    /*
    now clean the modem line and activate the settings for the port
    */
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd,TCSANOW,&newtio) != 0)
        perror("Unable to configure the serial port");

    close(fd);
    fd = -1;

    if (instance_1_based != 1)
    {
        char cmd[64];
        unsigned int cli_pid = get_cli_pid(instance_1_based);

        if (is_enabled(tlr_handle))
        {
            if (cli_pid <= 0)
            {
                fprintf(stdout, "Restart CLI on serial %d\n", instance_1_based);
                snprintf(cmd, sizeof cmd, "cli -m serial%d >%s 2>&1 <%s &", instance_1_based, device, device);
                system(cmd);
            }
        }
        else if (cli_pid > 0)
        {
            kill(cli_pid, SIGTERM);
            remove_cli(instance_1_based);
        }
    }

    rc = 0;

error:
    if (rc != 0)
        tcsetattr(fd,TCSANOW,&oldtio);

    if (fd >= 0)
        close(fd);

done:
    if (tlr_handle != NULL)
        tlr_config_close(tlr_handle);

#if 0
    if (lock_fd >= 0)
        file_unlock(lock_fd);
#endif

    return rc;

}
