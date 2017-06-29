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
#ifndef __CELLULAR_H__
#define __CELLULAR_H__

#include <stddef.h>

/* the maximum time we'll wait for a sensible response to a request for some cellular information */
#define MAX_SECONDS_TO_WAIT_FOR_CELLULAR 80
/* the value to return if unable to get a sensible response to cellular information */
#define VALUE_UNAVAILABLE "Not available"


int get_cellular_info(size_t const index_3g, char * const cellular_hardware_version, size_t const cellular_hardware_version_size,
                      char * const cellular_firmware_version, size_t const cellular_firmware_version_size,
                      char * const cellular_3g_modem_imei, size_t const cellular_3g_modem_imei_size);

#endif /* __CELLULAR_H__ */
