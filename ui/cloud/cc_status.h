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

#ifndef __CC_STATUS_H__
#define __CC_STATUS_H__

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>

#define CC_STATUS_SOCKET_NAME "CC_STATUS_SOCKET"

#define CC_STATUS_MSG_TIMEOUT  2
#define CC_MAX_URL_LEN         256
#define CC_MAX_DEV_ID_LEN      16
#define CC_MAX_DEV_ID_STR_LEN  36
#define CC_MAC_ADDR_LEN        6
#define CC_DEVICE_TYPE_LEN     32

typedef struct
{
    char server_name[CC_MAX_URL_LEN];
    uint8_t device_id[CC_MAX_DEV_ID_LEN];
    char device_id_str[CC_MAX_DEV_ID_STR_LEN];
    uint8_t mac_addr[CC_MAC_ADDR_LEN];
    char device_type[CC_DEVICE_TYPE_LEN];

    time_t start_time;

    struct
    {
        uint64_t received;
        uint64_t sent;
    } bytes;

    struct
    {
        uint64_t received;
        uint64_t sent;
    } packets;

    bool connected;
    bool restart_cloud;
    bool config_changed;
} cloud_status_t;

typedef enum
{
    cc_msg_type_unknown,
    cc_msg_type_show_ascii,
    cc_msg_type_show_json,
    cc_msg_type_restart
} cc_msg_type_t;

#define cc_msg_show_ascii  "show_ascii"
#define cc_msg_show_json   "show_json"
#define cc_msg_restart     "restart"

bool cc_socket_init(void);

cloud_status_t * cc_get_cloud_status(void);
void cc_clear_cloud_status(void);
void cc_set_restart_cloud(void);
void cc_set_connected_status(bool const state);

#define cc_set_connected() cc_set_connected_status(true)
#define cc_set_disconnected() cc_set_connected_status(false)

uint8_t * const cc_get_mac_addr(void);
uint8_t * const cc_get_device_id(void);
char * const cc_get_cloud_url(void);
char * const cc_get_device_type(void);

#endif // __CC_STATUS_H__
