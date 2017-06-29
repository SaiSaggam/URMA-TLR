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

#include "cellular.h"
#include <tlr_common.h>
#include <tlr_print.h>
#include <cellular_status.h>
#include <strip_whitespace.h>
#include "strlcpy.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <unittest_support.h>

#define SECONDS_BETWEEN_TRIES 1

STATIC_FUNCTION int get_cellular_item(size_t const index_3g, config_state_cellular_t const item, char * const value, size_t const value_size)
{
    int ret;
    size_t count;
    size_t const sleep_time_seconds = SECONDS_BETWEEN_TRIES;

    /* it can take some time for the cellular module to initialise so allow some time for this to
     * succeed, but don't wait forever.
     * It seems that even though cellular_read_status() may return success, nothing is put into the
     * buffer, so also wait until the buffer isn't empty.
     * Sigh, sometimes the buffer contains a single space, which is also unexpected, so ignore that
     * as well.
     * Because I've had problems trying to get cellular_read_status() to work reliably, I'm also
     * opening a new handle with each new attempt.
     */
    for (value[0] = '\0', count = 0, ret = -1;
          (count == 0 || ((count-1) * sleep_time_seconds) < MAX_SECONDS_TO_WAIT_FOR_CELLULAR);
           count++)
    {
        int fd;

        if (count)
        {
            sleep(sleep_time_seconds);
        }
        fd = cellular_open_status();
        if (fd < 0)
        {
            continue;
        }
        value[0] = '\0';
        ret = cellular_read_status(fd, index_3g, item, value, value_size);
        cellular_close_status(fd);
        if (ret != -1 && value[0] != '\0' && value[0] != ' ')
            break;
    }
    /* rather than fail if the buffer is still empty, let's insert an error message */
    if (ret == -1 || value[0] == '\0' || value[0] == ' ')
    {
        strlcpy(value, VALUE_UNAVAILABLE, value_size);
    }

    return 0;
}

int get_cellular_info(size_t const index_3g,
                      char * const cellular_hardware_version, size_t const cellular_hardware_version_size,
                      char * const cellular_firmware_version, size_t const cellular_firmware_version_size,
                      char * const cellular_3g_modem_imei, size_t const cellular_3g_modem_imei_size)
{
    int result;

    if (get_cellular_item(index_3g, config_state_cellular_module, cellular_hardware_version, cellular_hardware_version_size) < 0)
    {
        tlr_debug("failed to get cellular module hardware revision\n");
        result = -1;
        goto done;
    }
    if( get_cellular_item(index_3g, config_state_cellular_firmware_version, cellular_firmware_version, cellular_firmware_version_size) < 0)
    {
        tlr_debug("failed to get cellular module firmware revision\n");
        result = -1;
        goto done;
    }
    if (get_cellular_item(index_3g, config_state_cellular_imei, cellular_3g_modem_imei, cellular_3g_modem_imei_size) < 0)
    {
        tlr_debug("failed to get cellular module imie\n");
        result = -1;
        goto done;
    }

    result = 0;

done:

    return result;
}


