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
*****************************************************************************/

#include "show_parse_helper.h"
#include "strlcpy.h"
#include "strlcat.h"
#include "tlr_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <json.h>
#include <config_mgr.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#ifndef __mips__
extern int ioctl (int __fd, unsigned long int __request, ...);
#endif

char const * const dev_table[4] = {"/dev/ttyUSB3", "/dev/ttyUSB2", "/dev/ttyUSB1", "/dev/ttyUSB0"};

/* Need to include space for a trailing '|' character as the code may put one in before removing
 * it again. Maybe not a problem now as strlcat() is being used instaed of strcat().
 */
#define LINE_STATUS_ALL  "RTS|CTS|DTR|DSR|CD|RI|"

typedef struct serial_stats
{
    char const * description;
    char line_status[sizeof LINE_STATUS_ALL];
    unsigned int admin_status;
    int oper_status;
    uint32_t uptime;
    uint32_t tx_bytes;
    uint32_t rx_bytes;
    uint32_t overflow;
    uint32_t overrun;
} serial_stats_t;

static int fetch_from_icount(int const instance, serial_stats_t * const pstat)
{
    int result = -1;
    uint32_t status = 0;
    struct serial_icounter_struct counters;
    int fd = -1;
    size_t const dev_table_index = INSTANCE_TO_INDEX(instance);

    if (dev_table_index >= ARRAY_SIZE(dev_table))
    {
        goto error;
    }

    fd = open(dev_table[dev_table_index], O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) goto error;

    result = ioctl(fd, TIOCGICOUNT, &counters);

    if (result < 0) goto error;

    pstat->tx_bytes = counters.tx;
    pstat->rx_bytes = counters.rx;
    pstat->overrun = counters.overrun;
    pstat->overflow = counters.buf_overrun;

    pstat->line_status[0] = '\0';
    result = ioctl(fd, TIOCMGET, &status);

    if (result < 0) goto error;

    if (status & TIOCM_RTS)
    {
    	strlcat(pstat->line_status, "RTS|", sizeof pstat->line_status);
    }
    if (status & TIOCM_CTS)
    {
    	strlcat(pstat->line_status, "CTS|", sizeof pstat->line_status);
    }
    if (status & TIOCM_DTR)
    {
    	strlcat(pstat->line_status, "DTR|", sizeof pstat->line_status);
    }
    if (status & TIOCM_DSR)
    {
    	strlcat(pstat->line_status, "DSR|", sizeof pstat->line_status);
    }
    if (status & TIOCM_CD)
    {
    	strlcat(pstat->line_status, "CD|", sizeof pstat->line_status);
    }
    if (status & TIOCM_RI)
    {
    	strlcat(pstat->line_status, "RI|", sizeof pstat->line_status);
    }

    /* Remove trailing '|' character if it's there. */
    status = strlen(pstat->line_status);
    if (status > 0)
    {
    	pstat->line_status[status - 1] = '0';
    }

error:
    if (fd >= 0)
    {
        close(fd);
    }

    return result;
}

static char * serial_get_element_alloc(size_t const instance, size_t const element_id)
{
    char * value;
    config_status_t status;

    status = tlr_get_element_alloc_value(config_serial, instance, config_serial_descr, (void * *)&value,
                                         config_mgr_element_type_string, config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        fprintf(stderr, "Failed to read element: %zu status: %d\n", element_id, status);
        value = NULL;
    }

    return value;
}

static char * get_serial_description(size_t const instance)
{
    char * const description = serial_get_element_alloc(instance, config_serial_descr);

    return description;
}

static bool get_serial_admin_status(size_t const instance, unsigned int * const admin_status)
{
    bool got_admin_status;
    config_status_t status;

    status = tlr_get_element_value(config_serial,
                          instance,
                          config_serial_state,
                          admin_status,
                          sizeof *admin_status,
                          config_mgr_element_type_on_off,
                          config_mgr_access_level_read_only);

    got_admin_status = status == config_status_success;

    return got_admin_status;
}

static void cleanup_serial_stats(serial_stats_t const * const pstat)
{
    free((void *)pstat->description);
}

static int get_serial_stats(int const instance, serial_stats_t * const pstat)
{
    int result = -1;

    memset(pstat, 0, sizeof *pstat);

    pstat->description = get_serial_description(instance);
    if (pstat->description == NULL)
    {
        goto error;
    }

    if (!get_serial_admin_status(instance, &pstat->admin_status))
    {
        goto error;
    }

    if (pstat->admin_status)
    {
        result = fetch_from_icount(instance, pstat);

        if (result >= 0)
        {
            struct sysinfo info;

            if (sysinfo(&info) >= 0)
            {
                pstat->uptime = (uint32_t)info.uptime;
            }

            pstat->oper_status = 1;
        }
    }
    else
    {
        pstat->oper_status = 0;
        pstat->uptime = 0;
        pstat->tx_bytes = 0;
        pstat->rx_bytes = 0;
        pstat->overflow = 0;
        pstat->overrun = 0;
        strlcpy(pstat->line_status, "-", sizeof pstat->line_status);
        result = 0;
    }

error:
    if (result != 0)
    {
        cleanup_serial_stats(pstat);
    }

    return result;
}

static int format_json(serial_stats_t const * const pstat)
{
    int ret=0;
    int const hrs = pstat->uptime/3600;
    int const mins = (pstat->uptime % 3600)/60;
    int const secs = pstat->uptime % 60;
    char uptime[20];

    sprintf(uptime, "%d:%02d:%02d", hrs, mins, secs);

    // Create the serial json object
    json_object *serial_obj = json_object_new_object();

    json_object_object_add(serial_obj,"Description", json_object_new_string(pstat->description));
    json_object_object_add(serial_obj,"Admin_Status", json_object_new_string(pstat->admin_status ? "up" : "down"));
    json_object_object_add(serial_obj,"Oper_Status", json_object_new_string(pstat->oper_status ? "up" : "down"));
    json_object_object_add(serial_obj,"Uptime", json_object_new_string(uptime));
    json_object_object_add(serial_obj,"Tx_Bytes", json_object_new_int(pstat->tx_bytes));
    json_object_object_add(serial_obj,"Rx_Bytes", json_object_new_int(pstat->rx_bytes));
    json_object_object_add(serial_obj,"Overflows", json_object_new_int(pstat->overflow));
    json_object_object_add(serial_obj,"Overruns", json_object_new_int(pstat->overrun));
    json_object_object_add(serial_obj,"Line", json_object_new_string(pstat->line_status));

    printf ("%s", json_object_to_json_string(serial_obj));

    json_object_put(serial_obj);

    return ret;
}

static int format_ascii(int const instance, serial_stats_t * const pstat)
{
    int ret=0;
    int const hrs = pstat->uptime/3600;
    int const mins = (pstat->uptime % 3600)/60;
    int const secs = pstat->uptime % 60;

    printf("Serial %d Status\r\n", instance);
    printf("---------------\r\n");
    printf(" Description  : %s\r\n", pstat->description);
    printf(" Admin Status : %s\r\n", pstat->admin_status ? "up" : "down");
    printf(" Oper Status  : %s\r\n", pstat->oper_status ? "up" : "down");
    printf(" Uptime       : %d:%02d:%02d\r\n", hrs, mins, secs);
    printf(" Tx Bytes     : %u\r\n", pstat->tx_bytes);
    printf(" Rx Bytes     : %u\r\n", pstat->rx_bytes);
    printf(" Overflows    : %u\r\n", pstat->overflow);
    printf(" Overruns     : %u\r\n", pstat->overrun);
    printf(" Line status  : %s\r\n", pstat->line_status);

    return ret;
}

int main (int argc, char *argv[])
{
    int ret = EXIT_FAILURE;
    int instance = -1;
    serial_stats_t stats;
    tlr_response_format_t format;
    int max_instance = tlr_get_max_instance(config_state_serial, config_mgr_table_type_show);

    if (max_instance <= 0)
        goto error;

    if (parse_show_options(argc, argv, &format, &instance, 0, max_instance) != 0)
    {
        goto error;
    }

    if (instance < 0)
    {
        goto error;
    }

    if (instance == 0)
    {
        // If no instance is given, then default to instance one.
        instance = 1;
    }

    ret = get_serial_stats(instance, &stats);
    if (ret < 0)
    {
        goto error;
    }

    switch (format)
    {
        case tlr_response_format_json:
            ret = format_json(&stats);
            break;

        case tlr_response_format_ascii:
            ret = format_ascii(instance, &stats);
            break;

        default:
            ret = -1;
            break;
    }

    cleanup_serial_stats(&stats);

error:
    return ret;
}
