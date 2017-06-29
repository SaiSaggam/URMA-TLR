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
*****************************************************************************/

#ifndef __CELLULAR_CONFIG_H__
#define __CELLULAR_CONFIG_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "config_def.h"


#define CELLULAR_1_INSTANCE 1
#define CELLULAR_APN_LENGTH             64

typedef enum
{
    cellular_config_no_changed,
    cellular_config_changed,
    cellular_config_error
} cellular_config_status_t;


typedef struct
{
    uint32_t connection_attempts;
    char apn[CELLULAR_APN_LENGTH];
    char apn_username[CELLULAR_APN_LENGTH];
    char apn_password[CELLULAR_APN_LENGTH];
    config_cellular_preferred_mode_t preferred_mode;
} cellular_config_t;

cellular_config_status_t get_cellular_data(size_t const instance, cellular_config_t * const config);


#endif /* __CELLULAR_CONFIG_H__*/
