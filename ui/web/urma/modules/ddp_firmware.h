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
#ifndef _DDP_FIRMWARE_H_
#define _DDP_FIRMWARE_H_

#include <time.h>
#include <stdbool.h>

typedef enum
{
    firmware_update_status_none,
    firmware_update_status_downloading,
    firmware_update_status_verifying,
    firmware_update_status_applying,
    firmware_update_status_complete,
    firmware_update_status_rebooting,
    firmware_update_status_error
} ddp_firmware_update_status_t;

typedef struct
{
    ddp_firmware_update_status_t status;
    unsigned int progress;
    char const * result;
    time_t start_time;
} ddp_firmware_update_t;

bool ddp_update_firmware(char const * const user, char const * const file_path, bool const remote);
char * ddp_get_firmware_files(void);
char * ddp_get_firmware_status(void);
bool ddp_reboot(unsigned int reboot_after);

#endif /* _DDP_FIRMWARE_H_ */
