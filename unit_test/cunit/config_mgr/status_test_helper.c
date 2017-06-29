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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config_def.h"
#include "config_mgr.h"
#include "strlcpy.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

static tlr_handle_t show_handle = NULL;
static int count;

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    config_mgr_group_t const * const ptr = (instance > 0) ? &config_mgr_group_table[table_index].groups[group_id] : NULL;

    return ptr;
}

void status_test_set_show_handle(tlr_handle_t handle)
{
    show_handle = handle;
}

tlr_handle_t tlr_show_open(char const * const format, int const id, size_t const instance, char const * const parameter, config_mgr_access_level_t const access)
{
    CU_ASSERT_STRING_EQUAL(format, "json");
    CU_ASSERT_PTR_NULL(parameter);
    CU_ASSERT_EQUAL(access, config_mgr_access_level_read_only);
    count = 0;

    return show_handle;
}

config_status_t tlr_show_close(tlr_handle_t const handle)
{
    config_status_t status = config_status_success;

    CU_ASSERT_PTR_EQUAL(show_handle, handle);

    return status;
}


char * tlr_show_read_line_alloc(tlr_handle_t const handle, size_t const timeout)
{
    char * string;
    CU_ASSERT_PTR_EQUAL(show_handle, handle);

    static char const * json_strings[] =
    {
       "{\"description\":\"Factory default configuration for Ethernet 1\",",
       "\"admin-status\":0,\"oper-status\":1,\"uptime\":92151,",
       "\"mac-address\":\"00:40:FF:0F:43:AE\",",
       "\"dhcpcli\":\"off\",",
       "\"ip-address\":\"10.52.19.193\",",
       "\"mask\":\"255.255.255.0\",",
       "\"dns-primary\":\"\",",
       "\"dns-secondary\":\"\",",
       "\"link-status\":\"Up\",",
       "\"link-speed\":100,",
       "\"link-duplex\":\"Half\",",
       "\"rx-unicast-packets\":2333297,",
       "\"tx-unicast-packets\":2824,",
       "\"rx-broadcast-packets\":214998,",
       "\"tx-broadcast-packets\":2,",
       "\"rx-multicast-packets\":286349,",
       "\"tx-multicast-packets\":6,",
       "\"rx-crc-errors\":0,",
       "\"tx-crc-errors\":0,",
       "\"rx-drop-packets\":0,",
       "\"tx-drop-packets\":0,",
       "\"rx-pause-packets\":0,",
       "\"tx-pause-packets\":0,",
       "\"rx-filtering-packets\":2322826,",
       "\"tx-collisionss\":4,",
       "\"rx-alignment-error\":0,",
       "\"rx-undersize-error\":0,",
       "\"rx-fragment-error\":13457,",
       "\"rx-oversize-error\":0,",
       "\"rx-jabber-error\":0}",
       NULL
    };

    if (json_strings[count] != NULL)
    {
        string = strdup(json_strings[count]);
        count++;
    }
    else
    {
        string = NULL;
    }

    return string;
}
