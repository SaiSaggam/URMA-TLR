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

#include "cellular_status.h"
#include "csman.h"
#include "csid/csid_3g.h"


uint32_t cellular_get_max_restart_count(int const fd, size_t const index_3g)
{
    uint32_t count = 0;

    if (read_csman(fd, CSID_C_3G_MODEM_RESTART_TIMES+index_3g, &count, sizeof count, CSM_R_ZERO) < 0)
    {
        cellular_error("Could not read CSID_C_3G_MODEM_RESTART_TIMES\n");
    }
    else
    {
        cellular_printf("usb max restart count = %u\n", count);
    }

    return count;
}

uint32_t cellular_get_fail_restart_count(int const fd, size_t const index_3g)
{
    uint32_t count = 0;

    if (read_csman(fd, CSID_S_3G_MODEM_RESTART_COUNT + index_3g, &count, sizeof count, CSM_R_ZERO) < 0)
    {
        cellular_error("Could not read CSID_S_3G_MODEM_RESTART_TIMES\n");
    }
    else
    {
        cellular_printf("usb connect fail count = %u\n", count);
    }

    return count;
}

