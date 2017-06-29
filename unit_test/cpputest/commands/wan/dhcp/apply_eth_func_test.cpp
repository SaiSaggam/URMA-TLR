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

#include <CppUTest/CommandLineTestRunner.h>
#include "CppUTestExt/MockSupportPlugin.h"
#include <CppUTest/TestHarness.h>
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/MockSupport.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dhcp_client.h>
#include <ethregs.h>

extern "C"
{
#include "eth_monitor.h"
#include "parsing.h"
#include "tlr_print.h"
#include "config_mgr.h"
#include "config_def.h"
#include "config_mgr.h"
};

extern "C"
{
    // mapped functions

    // Note: This function is externed here as it was
    // changed to static for the test only and is not
    // defined in any header.
    bool read_wan_dhcp_config(eth_context_t * const context);

    tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance,
				 config_mgr_access_level_t const access)
    {
	return (tlr_handle_t) 0;
    }

    config_status_t tlr_get_element_value(size_t const group_id,
					  size_t const instance,
					  size_t const element_id,
					  void *const value,
					  size_t const bytes,
					  config_mgr_element_type_t const
					  type,
					  config_mgr_access_level_t const
					  access)
    {
	int res = 1;
	memcpy(value, &res, sizeof(res));

	return (config_status_t) 0;
    }

    void tlr_config_close(tlr_handle_t const handle)
    {
    }

    void dhcp_client_get_pid(void)
    {
    }
    void dhcp_client_start(void)
    {
    }
    void dhcp_client_stop(void)
    {
    }
    void reg_read(void)
    {
    }
    void reg_write(void)
    {
    }

};

config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    return config_status_success;
}


TEST_GROUP(apply_eth_func_test)
{
    void setup(void)
    {
    }
};

TEST(apply_eth_func_test, eth_do_assigned_to_wan_test)
{
    int sts;
    eth_context_t context;
    wan_info_st wan_info;

    memset(&context, 0, sizeof(context));
    memset(&wan_info, 0, sizeof(wan_info));

    sts = eth_do_assigned_to_wan(&context, &wan_info);

    LONGS_EQUAL(-1, sts);
};

TEST(apply_eth_func_test, eth_do_removed_from_wan_test)
{
    int sts;
    eth_context_t context;
    wan_info_st wan_info;

    memset(&context, 0, sizeof(context));
    memset(&wan_info, 0, sizeof(wan_info));

    sts = eth_do_removed_from_wan(&context, &wan_info);

    LONGS_EQUAL(-1, sts);
};

TEST(apply_eth_func_test, read_wan_dhcp_config_test)
{
    bool sts;
    eth_context_t context;

    memset(&context, 0, sizeof(context));

    sts = read_wan_dhcp_config(&context);

    LONGS_EQUAL(0, sts);
};

// main entry point
int
main(int argc, char *argv[])
{
    MockSupportPlugin mock_plugin;

    TestRegistry::getCurrentRegistry()->installPlugin(&mock_plugin);

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
