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
#ifndef __DDP_DEVICE_H__
#define __DDP_DEVICE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum
{
   ddp_no_access = -1,
   ddp_read_only,
   ddp_read_write,
   ddp_super_user
} ddp_access_t;

typedef struct
{
    char const * group_name;
    char const * element_name;
    size_t instance;
    char * value;
    char * user;
    ddp_access_t access_level;
} ddp_group_info_t;

bool ddp_push_config_change(char const * const group_name, size_t const instance);

size_t ddp_get_group_count(bool const is_config);
size_t ddp_get_group_instance(size_t const group_id, bool const is_config);
char * ddp_read_group(size_t const group_id, size_t const instance, bool const is_config, bool const at_init);
ssize_t ddp_get_group_id(char const * const group_name, bool const is_config);
char * ddp_read_config_descriptor(void);
char * ddp_read_state_descriptor(void);

bool ddp_set_element(ddp_group_info_t const * const info);
bool ddp_apply_group(ddp_group_info_t const * const info);
bool ddp_save(char const * const user);

ddp_access_t ddp_authenticate(char const * const username, char const * const password);

int ddp_encode_xml_entities(FILE * const fd, char const * const value);

#endif // __DDP_DEVICE_H__
