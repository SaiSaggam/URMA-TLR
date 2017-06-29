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
#include "parsing.h"
#include "tlr_print.h"
#include "config_mgr.h"
#include "config_def.h"
};

extern "C"
{
    // mapped functions
};

TEST_GROUP(get_wan_message_info_test)
{
    void setup(void)
    {
    }
};

TEST(get_wan_message_info_test, test_get_wan_message_info_A)
{
    int sts;
    void * info_result = NULL;
    size_t info_size;
    int argc = 1;
    char * argv[4];

    argv[0] = strdup("-1");
    argv[1] = NULL;
    argv[2] = NULL;
    argv[3] = NULL;

    sts = get_wan_message_info(argc, argv, &info_result, &info_size);

    LONGS_EQUAL(-1, sts);

    free(argv[0]);
    if (info_result)
      {
	  free(info_result);
      }
};

TEST(get_wan_message_info_test, test_get_wan_message_info_B)
{
    int sts;
    void * info_result = NULL;
    size_t info_size;
    int argc = 0;
    char * argv[4];

    argv[0] = strdup("-1");
    argv[1] = NULL;
    argv[2] = NULL;
    argv[3] = NULL;

    sts = get_wan_message_info(argc, argv, &info_result, &info_size);

    LONGS_EQUAL(-1, sts);

    free(argv[0]);
    if (info_result)
      {
	  free(info_result);
      }
};

TEST(get_wan_message_info_test, test_get_wan_message_info_C)
{
    int sts;
    void * info_result = NULL;
    size_t info_size;
    int argc = 1;
    char * argv[4];
    wan_info_st * r;

    argv[0] = strdup("1");
    argv[1] = NULL;
    argv[2] = NULL;
    argv[3] = NULL;

    sts = get_wan_message_info(argc, argv, &info_result, &info_size);

    LONGS_EQUAL(0, sts);

    r = (wan_info_st *) info_result;
    LONGS_EQUAL(r->wan_instance, 1);

    free(argv[0]);
    if (info_result)
      {
	  free(info_result);
      }
};

// main entry point
int
main(int argc, char * argv[])
{
    MockSupportPlugin mock_plugin;

    TestRegistry::getCurrentRegistry()->installPlugin(&mock_plugin);

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
