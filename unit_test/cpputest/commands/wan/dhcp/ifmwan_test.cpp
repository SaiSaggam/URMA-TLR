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

extern "C"
{
#include "ifmanager.h"
#include "tlr_print.h"
#include "config_mgr.h"
#include "config_def.h"
#include "failover.h"
#include "ifmwan.h"
};

//#define MANY_TESTS

int send_message_return = 0;

// the status of the call return here
int tlr_get_element_value_return[2] = {
    config_status_success,	/* config_mgr_element_type_enum   */
    config_status_success	/* config_mgr_element_type_on_off */
};

// value gets returned as this
int tlr_get_element_value_value[2] = {
    1 /* eth1 */ , 0		/* off */
};

config_mgr_group_t tlr_groups[config_wan + 1];
config_mgr_group_table_t tlr_get_table_return = { tlr_groups, config_wan };

ifm_update_info_t update_info = {
    32,				// instance
    config_eth,			// config_group_t
    "eth1"			// name
};

void
set_initial_state(void)
{
}

struct ifm_wan_config_struct
{
    config_wan_interface_t interface;
    bool dhcp_on;
};

extern "C"
{

    // pretend to be send_message
    int send_message(message_destination_t const destination,
		     size_t instance, message_codes_t const message_code,
		     char const *const str, char **const response_buffer)
    {

	mock().actualCall("send_message").withParameter("destination",
							destination).withParameter
	    ("instance", instance).withParameter("message_code",
						 message_code).withParameter
	    ("str", str);

	return send_message_return;
    }

    // pretend to be tlr_get_element_value
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

	config_status_t res;

	switch (type)
	  {
	  case config_mgr_element_type_enum:
	      res = (config_status_t) tlr_get_element_value_return[0];
	      memcpy(value, &tlr_get_element_value_value[0], 4);
	      break;
	  case config_mgr_element_type_on_off:
	      res = (config_status_t) tlr_get_element_value_return[1];
	      memcpy(value, &tlr_get_element_value_value[1], 4);
	      break;
	  default:
	      printf("FAIL: Bad Test Case\n");
	      res = (config_status_t) 0;
	      break;
	  }

	return res;
    }

    // pretend to be tlr_get_table
    void const *tlr_get_table(config_mgr_table_type_t const type)
    {
	mock().actualCall("tlr_get_table").withParameter("type", type);

	return (void *) &tlr_get_table_return;
    }

    // must declare this
    extern void wan_close(void);

    extern struct ifm_wan_config_struct *wan_config;

    extern void wan_set_contexts(struct ifm_wan_config_struct *const
				 contexts);

    char const *get_config_group_name_from_id(config_group_t const group_id)
    {
	static char data[32];
	strcpy(data, "eth1");

	return data;
    }

};				// extern C

TEST_GROUP(ifmwan_test)
{
    void setup(void)
    {
    }
};

#if defined(MANY_TESTS)
TEST(ifmwan_test, test_wan_init_A)
{
    int sts;

    set_initial_state();

    tlr_groups[config_wan].instances = 0;

    mock().expectOneCall("tlr_get_table").withParameter("type",
							config_mgr_table_type_config);

    // initialize wan
    sts = wan_init();

    LONGS_EQUAL(-1, sts);

    // do this to free memory
    wan_close();
};
#endif

#if defined(MANY_TESTS)
TEST(ifmwan_test, test_wan_init_B)
{
    int sts;

    set_initial_state();

    // setup for one instance
    tlr_groups[config_wan].instances = 1;

    mock().expectOneCall("tlr_get_table").withParameter("type",
							config_mgr_table_type_config);

    mock().expectOneCall("send_message").withParameter("destination",
						       ethernet_monitor).withParameter
	("instance", 1).withParameter("message_code",
				      assigned_to_wan_message).withParameter
	("str", "1");

    // initialize wan
    sts = wan_init();

    LONGS_EQUAL(0, sts);

    // do this to free memory
    wan_close();
};
#endif

#if defined(MANY_TESTS)
TEST(ifmwan_test, wan_close_A)
{
    set_initial_state();

    // check for memory leak
    wan_set_contexts((struct ifm_wan_config_struct *) malloc(32));

    // do this to free memory
    wan_close();
};
#endif

#if defined(MANY_TESTS)
TEST(ifmwan_test, wan_close_B)
{
    set_initial_state();

    // check case where no memory allocated
    wan_config = NULL;

    // do this to free memory
    wan_close();
};
#endif

#if defined(MANY_TESTS)
TEST(ifmwan_test, handle_change_A)
{
    int sts;

    set_initial_state();

    update_info.instance = 32;	// out of range

    sts = wan_handle_config_change(&update_info);

    LONGS_EQUAL(-1, sts);

};
#endif // 0

#if defined(MANY_TESTS)
// we were off, we stay off, we were on eth1 and stay on eth1
TEST(ifmwan_test, handle_change_B)
{
    int sts;

    set_initial_state();

    // setup for one instance
    tlr_groups[config_wan].instances = 1;

    mock().expectOneCall("tlr_get_table").withParameter("type",
							config_mgr_table_type_config);

    mock().expectOneCall("send_message").withParameter("destination",
						       ethernet_monitor).withParameter
	("instance", 1).withParameter("message_code",
				      assigned_to_wan_message).withParameter
	("str", "1");

    // initialize wan
    sts = wan_init();

    LONGS_EQUAL(0, sts);

    update_info.instance = 1;	// in range

    sts = wan_handle_config_change(&update_info);

    LONGS_EQUAL(-1, sts);

    // do this to free memory
    wan_close();

};
#endif // 0

// we were off, but turn it on
TEST(ifmwan_test, handle_change_C)
{
    int sts;

    set_initial_state();

    // setup for one instance
    tlr_groups[config_wan].instances = 1;

    mock().expectOneCall("tlr_get_table").withParameter("type",
							config_mgr_table_type_config);

    mock().expectOneCall("send_message").withParameter("destination",
						       ethernet_monitor).withParameter
	("instance", 1).withParameter("message_code",
				      assigned_to_wan_message).withParameter
	("str", "1");

    // initialize wan
    sts = wan_init();

    LONGS_EQUAL(0, sts);

    mock().clear();

    mock().expectOneCall("send_message").withParameter("destination",
						       0).withParameter
	("instance", 1).withParameter("message_code",
				      1005).withParameter("str", "eth1 1");

    update_info.instance = 1;	// in range
    tlr_get_element_value_value[1] = 1;	// turn on

    sts = wan_handle_config_change(&update_info);

    LONGS_EQUAL(0, sts);

    // do this to free memory
    wan_close();

};

// main entry point
int
main(int argc, char *argv[])
{
    MockSupportPlugin mock_plugin;

    TestRegistry::getCurrentRegistry()->installPlugin(&mock_plugin);

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
