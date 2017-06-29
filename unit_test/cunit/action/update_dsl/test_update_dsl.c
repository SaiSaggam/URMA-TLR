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

#include "test_update_dsl.h"
#include "tlr_print.h"
#include "tlr_common.h"
#include "CUnit/CUnit.h"
#include <unistd.h>
#include <stdbool.h>

static char const * const network1;
static bool network1_in_use;
static char const * const network2;
static bool network2_in_use;

static bool network_in_use(char const * const network)
{
    return (strcmp(network, network1) == 0 && network1_in_use) ||
           (strcmp(network, network2) == 0 && network2_in_use);
}

static bool set_modem_ip_address_return_value;

static bool set_modem_ip_address(char const * const network)
{
    return set_modem_ip_address_return_value;
}

static bool set_local_ip_address_return_value;

static bool set_local_ip_address(char const * const network)
{
    return set_local_ip_address_return_value;
}

static bool ping_modem_before_update_return_value;

static bool ping_modem_before_update(char const * const network)
{
    return ping_modem_before_update_return_value;
}

static bool upload_firmware_return_value;

static bool upload_firmware(char const * const pathname, char const * const network)
{
    return upload_firmware_return_value;
}

static bool ping_modem_after_update_return_value;

static bool ping_modem_after_update(char const * const network)
{
    return ping_modem_after_update_return_value;
}

#include "update_dsl.c"

bool tlr_printf(tlr_response_format_t const type, char const * const format, ...)
{
    return true;
}

void openlog(const char *ident, int option, int facility)
{
}

void syslog(int priority, const char *format, ...)
{
}

void closelog(void)
{
}

void vsyslog(int priority, const char *format, va_list ap)
{
}

int system_command_with_log(char const * const process_name, char const * const format, ...)
{
    return 0;
}

unsigned int sleep(unsigned int seconds)
{
    return 0;
}

static int access_return_value;

int access(const char *pathname, int mode)
{
    return access_return_value;
}

static int asprintf_return_value;

int asprintf(char **strp, const char *fmt, ...)
{
    return asprintf_return_value;
}

static void test_parse_command(void)
{
    char const * filename;
    tlr_response_format_t format = tlr_response_format_ascii;

#undef TEST
#define TEST(expect, args...) \
    do { \
        char * const argv[] = {"action_update_dsl", args}; \
        CU_ASSERT_EQUAL(expect, parse_command(ARRAY_SIZE(argv)-1, argv, &filename, &format)); \
    } while (0);


    TEST(false, NULL);
    TEST(false, "-q", NULL);
    TEST(false, "-u", NULL);
    TEST(false, "-uusername", NULL);
    TEST(false, "-u", "username", NULL);
    TEST(false, "-f", NULL);
    TEST(false, "-fascii", NULL);
    TEST(false, "-f", "ascii", NULL);
    TEST(false, "-u", "-f", NULL);
    TEST(false, "-uusername", "-fascii", NULL);
    TEST(true, "firmware.dat", NULL);
    CU_ASSERT_STRING_EQUAL(filename, "firmware.dat");
    CU_ASSERT_EQUAL(format, tlr_response_format_unknown);
    TEST(false, "-u", "firmware.dat", NULL);
    TEST(false, "-f", "firmware.dat", NULL);
    TEST(true, "-u", "username", "firmware.dat", NULL);
    CU_ASSERT_STRING_EQUAL(filename, "firmware.dat");
    CU_ASSERT_EQUAL(format, tlr_response_format_unknown);
    TEST(true, "-f", "ascii", "firmware.dat", NULL);
    CU_ASSERT_STRING_EQUAL(filename, "firmware.dat");
    CU_ASSERT_EQUAL(format, tlr_response_format_ascii);
    TEST(true, "-u", "username", "-f", "ascii", "firmware.dat", NULL);
    CU_ASSERT_STRING_EQUAL(filename, "firmware.dat");
    CU_ASSERT_EQUAL(format, tlr_response_format_ascii);
}

static void test_check_file(void)
{
    char const * const filename = "firmware.dat";
    char const * const pathname = USER_DIR "/firmware.dat";

    access_return_value = 0;
    CU_ASSERT_TRUE(check_file(tlr_response_format_ascii, filename, pathname));
    access_return_value = -1;
    CU_ASSERT_FALSE(check_file(tlr_response_format_ascii, filename, pathname));
}

static void test_start_update(void)
{
    char const * const filename = "firmware.dat";
    char const * const pathname = USER_DIR "/firmware.dat";

    network1_in_use = true;
    network2_in_use = true;
    CU_ASSERT_FALSE(start_update(tlr_response_format_ascii, filename, pathname));

    network1_in_use = true;
    network2_in_use = false;
    set_modem_ip_address_return_value = false;
    CU_ASSERT_FALSE(start_update(tlr_response_format_ascii, filename, pathname));

    network1_in_use = false;
    set_modem_ip_address_return_value = false;
    CU_ASSERT_FALSE(start_update(tlr_response_format_ascii, filename, pathname));

    set_modem_ip_address_return_value = true;
    set_local_ip_address_return_value = false;
    CU_ASSERT_FALSE(start_update(tlr_response_format_ascii, filename, pathname));

    set_local_ip_address_return_value = true;
    ping_modem_before_update_return_value = false;
    CU_ASSERT_FALSE(start_update(tlr_response_format_ascii, filename, pathname));

    ping_modem_before_update_return_value = true;
    upload_firmware_return_value = false;
    CU_ASSERT_FALSE(start_update(tlr_response_format_ascii, filename, pathname));

    upload_firmware_return_value = true;
    ping_modem_after_update_return_value = false;
    CU_ASSERT_FALSE(start_update(tlr_response_format_ascii, filename, pathname));

    ping_modem_after_update_return_value = true;
    CU_ASSERT_TRUE(start_update(tlr_response_format_ascii, filename, pathname));
}

static void test_update_dsl_main(void)
{
#undef TEST
#define TEST(expect, args...) \
    do { \
        char * const argv[] = {"action_update_dsl", args}; \
        CU_ASSERT_EQUAL(expect, update_dsl_main(ARRAY_SIZE(argv)-1, argv)); \
    } while (0);
    char * const filename = "firmware.dat";

    network1_in_use = true;
    set_local_ip_address_return_value = true;
    set_modem_ip_address_return_value = true;
    ping_modem_before_update_return_value = true;
    upload_firmware_return_value = true;
    ping_modem_after_update_return_value = true;
    access_return_value = 0;

    asprintf_return_value = -1;
    TEST(false, filename, NULL);

    asprintf_return_value = 10;
    TEST(true, filename, NULL);
}

static int init_suite(void)
{
    opterr = 0;

    return 0;
}

static int clean_suite(void)
{
    return 0;
}

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_parse_command),
    CU_TEST_INFO(test_check_file),
    CU_TEST_INFO(test_start_update),
    CU_TEST_INFO(test_update_dsl_main),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_update_dsl_suites[] =
{
    { "update_dsl", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};
