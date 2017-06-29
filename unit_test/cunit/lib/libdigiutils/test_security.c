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

#include "test_security.h"
#include "digiutils.h"
#include "tlr_common.h"
#include "security.h"
#include "CUnit/CUnit.h"
#include <string.h>
#include <stdbool.h>

static char enc_global_password[128] = {0};
static char enc_device_password[128] = {0};
static char const global_password[] = "global:password";
static char const device_password[] = "device:password";
static char const global_prefix[] = "$00$";
static char const device_prefix[] = "$01$";

static void test_encrypt_password(void)
{
    bool success;
    size_t const expected_enc_pwd_len = 64;
    size_t const prefix_length = sizeof device_prefix - 1;
    size_t const expected_str_len = prefix_length + expected_enc_pwd_len;

    /* NULL clear-text */
    success = encrypt_password(password_scope_global, NULL, enc_global_password, sizeof enc_global_password);
    CU_ASSERT_EQUAL(success, false);

    /* NULL encrypt_pwd */
    success = encrypt_password(password_scope_global, global_password, NULL, sizeof enc_global_password);
    CU_ASSERT_EQUAL(success, false);

    /* insufficient size */
    success = encrypt_password(password_scope_global, global_password, enc_global_password, expected_str_len/2);
    CU_ASSERT_EQUAL(success, false);

    success = encrypt_password(password_scope_global, global_password, enc_global_password, sizeof enc_global_password);
    CU_ASSERT_EQUAL(success, true);
    CU_ASSERT_EQUAL(strlen(enc_global_password), expected_str_len);
    CU_ASSERT_EQUAL(strncmp(enc_global_password, global_prefix, prefix_length), 0);

    success = encrypt_password(password_scope_device, device_password, enc_device_password, sizeof enc_device_password);
    CU_ASSERT_EQUAL(success, true);
    CU_ASSERT_EQUAL(strlen(enc_device_password), expected_str_len);
    CU_ASSERT_EQUAL(strncmp(enc_device_password, device_prefix, prefix_length), 0);
}

static void test_decrypt_password(void)
{
    bool success;
    static char clear_pwd[64];

    success = decrypt_password(enc_global_password, clear_pwd, sizeof clear_pwd);
    CU_ASSERT_EQUAL(success, true);
    CU_ASSERT_STRING_EQUAL(clear_pwd, global_password);

    success = decrypt_password(enc_device_password, clear_pwd, sizeof clear_pwd);
    CU_ASSERT_EQUAL(success, true);
    CU_ASSERT_STRING_EQUAL(clear_pwd, device_password);

    /* insufficient size */
    success = decrypt_password(enc_device_password, clear_pwd, sizeof "device");
    CU_ASSERT_EQUAL(success, false);

    /* NULL encrypt_pwd */
    success = decrypt_password(NULL, clear_pwd, sizeof clear_pwd);
    CU_ASSERT_EQUAL(success, false);

    /* NULL clear_pwd */
    success = decrypt_password(enc_device_password, NULL, sizeof clear_pwd);
    CU_ASSERT_EQUAL(success, false);
}

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_encrypt_password),
    CU_TEST_INFO(test_decrypt_password),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_security_suites[] =
{
    { "test_security", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};
