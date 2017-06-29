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

#ifndef __CONFIG_UTILS_H__
#define __CONFIG_UTILS_H__

#include "config_mgr.h"
#include "config_def.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct config_st;
typedef struct config_st config_st;
typedef bool (*config_compare_fn)(size_t const instance, config_st * const previous_config, config_st * const current_config);

config_st * config_alloc(config_group_t group_id);
int config_init(config_st * * const config, size_t const instance, config_group_t config_group);
int config_update_and_compare_with_previous(config_st * * const config,
                                            size_t const instance,
                                            config_compare_fn compare_fn,
                                            bool * const config_has_changed);
size_t config_get_element_count(config_st * const config);
char const * config_get_element_value(config_st * const config, size_t const index);
void config_set_element_value(config_st * const config, size_t const index, char const * const value);

typedef struct
{
    char * value;
    size_t size;
} element_t;

struct config_st
{
    config_group_t group_id;
    size_t count;
    element_t * elements;
};

#endif /* __CONFIG_UTILS_H__ */
