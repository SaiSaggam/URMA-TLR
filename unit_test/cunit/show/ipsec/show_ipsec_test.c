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
#define     USE_BASIC_STYLE     1
#define     USE_AUTOMATED_STYLE     0
#define     USE_CONSOLE_STYLE     0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "show_ipsec.h"



int init_suite(void)
{
    return 0;
}
int clean_suite(void)
{
    return 0;
}

static tunnel_status_t *allocate_tunnel(int instance)
{
    tunnel_status_t *ptr_tunnel;

    ptr_tunnel = calloc(1, sizeof *ptr_tunnel);
    CU_ASSERT_PTR_NOT_NULL_FATAL(ptr_tunnel);

    ptr_tunnel->instance = instance;
    sprintf(ptr_tunnel->name, "ipsec%d", ptr_tunnel->instance);
    ptr_tunnel->is_tunnel_enabled = true;

    return ptr_tunnel;
}

static void free_tunnel(tunnel_status_t *ptr_tunnel)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(ptr_tunnel);
    free(ptr_tunnel);
}
/*
 * Test Cases, these are more of a module level test which tests
 * the parsing code verses function level tests.
 */

// Read a file which contains a good 'ipsec statusall' output
static void test_good_ipsec(void)
{
    FILE * fp = NULL;
    tunnel_status_t *ptr_tunnel;
    int ret;

    fp = fopen("./good_ipsec.txt", "r");
    CU_ASSERT_PTR_NOT_NULL_FATAL(fp);

    ptr_tunnel = allocate_tunnel(1);

    ret = read_tunnel_info_from_ipsec_status(fp, ptr_tunnel);
    CU_ASSERT_EQUAL(ret, 0);

    CU_ASSERT_TRUE(ptr_tunnel->is_tunnel_up);
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->up_time, "25 minutes ");

    CU_ASSERT_STRING_EQUAL(ptr_tunnel->peer_ip, "10.52.18.109");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->local_cidr_network, "100.100.102.0/24 ");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->remote_cidr_network, "100.100.101.0/24 ");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->key_negotiation, "IKEv1, aes128, sha1, modp1536");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->rekeying_in, "19 minutes");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->esp_ciphers, "aes128, sha1");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->renegotiating_in, "17 minutes");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->outbound_esp_sa, "1C4AD12F");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->inbound_esp_sa, "6D0D1719");

    free_tunnel(ptr_tunnel);
    if (fp != NULL)
    {
        pclose(fp);
    }
}

// Positive test cases

// Read a file which contains a good 'ipsec statusall' output
// Test AES_CBC_256/HMAC_SHA1_256/PRF_HMAC_SHA1/MODP_2048 encryption
// types.
void test_good_ipsec1(void)
{
    FILE * fp = NULL;
    tunnel_status_t *ptr_tunnel;
    int ret;

    fp = fopen("./good_ipsec1.txt", "r");
    CU_ASSERT_PTR_NOT_NULL_FATAL(fp);

    ptr_tunnel = allocate_tunnel(1);

    ret = read_tunnel_info_from_ipsec_status(fp, ptr_tunnel);
    CU_ASSERT_EQUAL(ret, 0);

    CU_ASSERT_TRUE(ptr_tunnel->is_tunnel_up);
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->up_time, "25 minutes ");

    CU_ASSERT_STRING_EQUAL(ptr_tunnel->peer_ip, "10.52.18.109");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->local_cidr_network, "100.100.102.0/24 ");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->remote_cidr_network, "100.100.101.0/24 ");

    CU_ASSERT_STRING_EQUAL(ptr_tunnel->key_negotiation, "IKEv1, aes256, sha1, modp2048");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->rekeying_in, "19 minutes");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->esp_ciphers, "aes128, sha1");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->renegotiating_in, "17 minutes");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->outbound_esp_sa, "1C4AD12F");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->inbound_esp_sa, "6D0D1719");

    free_tunnel(ptr_tunnel);
    if (fp != NULL)
    {
        pclose(fp);
    }
}

// This tests when there is no encryption type specified.
void test_good_ipsec2(void)
{
    FILE * fp = NULL;
    tunnel_status_t *ptr_tunnel;
    int ret;

    fp = fopen("./good_ipsec1.txt", "r");
    CU_ASSERT_PTR_NOT_NULL_FATAL(fp);

    ptr_tunnel = allocate_tunnel(1);

    ret = read_tunnel_info_from_ipsec_status(fp, ptr_tunnel);
    CU_ASSERT_EQUAL(ret, 0);

    CU_ASSERT_TRUE(ptr_tunnel->is_tunnel_up);
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->up_time, "25 minutes ");

    CU_ASSERT_STRING_EQUAL(ptr_tunnel->peer_ip, "10.52.18.109");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->local_cidr_network, "100.100.102.0/24 ");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->remote_cidr_network, "100.100.101.0/24 ");

    CU_ASSERT_STRING_EQUAL(ptr_tunnel->key_negotiation, "IKEv1, aes256, sha256, modp2048");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->rekeying_in, "17 minutes");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->esp_ciphers, "aes128, sha96");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->renegotiating_in, "19 minutes");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->outbound_esp_sa, "1C4AD12F");
    CU_ASSERT_STRING_EQUAL(ptr_tunnel->inbound_esp_sa, "6D0D1719");

    free_tunnel(ptr_tunnel);
    if (fp != NULL)
    {
        pclose(fp);
    }
}


// Negative test cases

// Read a file which contains a good 'ipsec statusall' output
void test_bad_ipsec(void)
{
    FILE * fp = NULL;
    tunnel_status_t *ptr_tunnel;
    int ret;

    fp = fopen("./bad_ipsec.txt", "r");
    CU_ASSERT_PTR_NOT_NULL_FATAL(fp);

    ptr_tunnel = allocate_tunnel(1);

    ret = read_tunnel_info_from_ipsec_status(fp, ptr_tunnel);
    CU_ASSERT_EQUAL(ret, 0);

    CU_ASSERT_FALSE(ptr_tunnel->is_tunnel_up);

    free_tunnel(ptr_tunnel);
    if (fp != NULL)
    {
        pclose(fp);
    }
}


/************* Test Runner Code goes here **************/

int main(void)
{
    int number_of_failures;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("resource_monitor_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (0
        || (CU_add_test(pSuite, "test_good_ipsec", test_good_ipsec) == NULL)
        || (CU_add_test(pSuite, "test_bad_ipsec", test_bad_ipsec) == NULL)
        || (CU_add_test(pSuite, "test_good_ipsec1", test_good_ipsec1) == NULL)
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

#if USE_BASIC_STYLE
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

#elif USE_AUTOMATED_STYLE
    // Run all tests using the automated interface
    CU_automated_run_tests();
    CU_list_tests_to_file();

    // Run all tests using the console interface
    CU_console_run_tests();
#endif

    //	Clean up registry and return
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    //	Ensure we return an error if any tests failed
    if (number_of_failures)
        return -1;
    return CU_get_error();
}



/****************  Stubs   ***************************/

