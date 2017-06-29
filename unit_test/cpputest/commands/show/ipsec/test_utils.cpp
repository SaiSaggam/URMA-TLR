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
#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"

/*
 * This is where we include the header files of the C code under test
 */
extern "C"
{
    /* include C header files of code under test */
    #include "utils.h"
}

TEST_GROUP(parse_netmask)
{
    char * result_string;

    void setup()
    {
        result_string = NULL;
    }
    void teardown()
    {
        free(result_string);
        mock().clear();
    }

    void perform_test(char const * const input_string, char const * const expected_result)
    {
        result_string = parse_netmask(input_string);

        STRCMP_EQUAL(expected_result, result_string);
    }
};

TEST(parse_netmask, mask_zero_returns_0_0_0_0)
{
    perform_test("1.1.1.1/0", "0.0.0.0");
}

TEST(parse_netmask, no_slash_returns_0_0_0_0)
{
    perform_test("1.1.1.1", "0.0.0.0");
}

TEST(parse_netmask, empty_string_returns_0_0_0_0)
{
    perform_test("", "0.0.0.0");
}

TEST(parse_netmask, too_many_bits_returns_0_0_0_0)
{
    perform_test("1.1.1.1/33", "0.0.0.0");
}

TEST(parse_netmask, one_bit_returns_128_0_0_0)
{
    perform_test("1.1.1.1/1", "128.0.0.0");
}

TEST(parse_netmask, string_with_newline_still_works)
{
    perform_test("1.1.1.1/32\n", "255.255.255.255");
}

TEST(parse_netmask, string_with_nothing_after_slash_returns_0_0_0_0)
{
    perform_test("1.1.1.1/", "0.0.0.0");
}

TEST_GROUP(parse_network)
{
    char * result_string;

    void setup()
    {
        result_string = NULL;
    }
    void teardown()
    {
        free(result_string);
        mock().clear();
    }

    void perform_test(char const * const input_string, char const * const expected_result)
    {
        result_string = parse_network(input_string);

        STRCMP_EQUAL(expected_result, result_string);
    }
};

TEST(parse_network, no_slash_returns_0_0_0_0)
{
    perform_test("1.1.1.1", "0.0.0.0");
}

TEST(parse_network, empty_string_returns_0_0_0_0)
{
    perform_test("1.1.1.1", "0.0.0.0");
}

TEST(parse_network, string__with_slash_returns_everything_before_the_slash)
{
    perform_test("1.2.3.4/", "1.2.3.4");
}

TEST_GROUP(parse_ipsec_uptime_seconds)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }

    void perform_test(char const * const input_string, uint32_t expected_result)
    {
        uint32_t actual_result;

        actual_result = parse_ipsec_uptime_seconds(input_string);

        CHECK_EQUAL(expected_result, actual_result);
    }
};

TEST(parse_ipsec_uptime_seconds, empty_string_returns_0)
{
    perform_test("", 0);
}

TEST(parse_ipsec_uptime_seconds, no_units_returns_0)
{
    perform_test("1", 0);
}

TEST(parse_ipsec_uptime_seconds, seconds_units)
{
    perform_test("1 seconds", 1);
}

TEST(parse_ipsec_uptime_seconds, minutes_units)
{
    perform_test("2 minutes", 120);
}

TEST(parse_ipsec_uptime_seconds, hours_units)
{
    perform_test("2 hours", 2 * 60 * 60);
}

TEST(parse_ipsec_uptime_seconds, days_units)
{
    perform_test("3 days", 3 * 60 * 60 * 24);
}

TEST(parse_ipsec_uptime_seconds, unsupported_units_returns_zero)
{
    perform_test("4 things", 0);
}


