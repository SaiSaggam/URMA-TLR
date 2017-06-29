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

extern "C"
{
    #include "tlr_print.h"
}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

TEST_GROUP(tlr_response_format_from_string)
{
};

TEST(tlr_response_format_from_string, supported_strings_return_expected_response)
{
    tlr_response_format_t response_format;

    response_format = tlr_response_format_from_string(TLR_FORMAT_ASCII);
    UNSIGNED_LONGS_EQUAL(tlr_response_format_ascii, response_format);

    response_format = tlr_response_format_from_string(TLR_FORMAT_JSON);
    UNSIGNED_LONGS_EQUAL(tlr_response_format_json, response_format);
}

TEST(tlr_response_format_from_string, unsupported_strings_return_format_unknown)
{
    tlr_response_format_t response_format;

    response_format = tlr_response_format_from_string("");
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);

    response_format = tlr_response_format_from_string("12345");
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);

    response_format = tlr_response_format_from_string(TLR_FORMAT_ASCII "\n");
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);

    response_format = tlr_response_format_from_string(TLR_FORMAT_JSON " ");
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);

    response_format = tlr_response_format_from_string(" " TLR_FORMAT_ASCII);
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);

    response_format = tlr_response_format_from_string(TLR_FORMAT_ASCII TLR_FORMAT_ASCII);
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);

    response_format = tlr_response_format_from_string(TLR_FORMAT_ASCII " " TLR_FORMAT_ASCII);
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);
}

TEST(tlr_response_format_from_string, null_string_returns_format_unknown)
{
    tlr_response_format_t response_format;

    response_format = tlr_response_format_from_string(NULL);
    UNSIGNED_LONGS_EQUAL(tlr_response_format_unknown, response_format);
}
