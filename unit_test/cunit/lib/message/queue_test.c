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
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

static bool would_have_blocked;
static queue_st * queue;
static int item_to_place_onto_queue = 0;
static int second_item_to_place_onto_queue = 0;
static bool testing_put_queue_item;

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */
int pthread_cond_wait(pthread_cond_t * __restrict __cond,
                  pthread_mutex_t * __restrict __mutex)
{
    would_have_blocked = true;
    /* and put something onto the queue to stop it get_queue_item() from blocking */
    if (testing_put_queue_item == false)
        /* put an item to the queue to stop get_queue_item() blocking */
        put_queue_item(queue, &item_to_place_onto_queue);
    else    /* pull an item from the queue to stop put_queue_item() blocking */
        get_queue_item(queue);
    return 0;
}

/************* Test case functions ****************/

/******* Test Cases ******/
static void test_queue_get(void)
{
    void * get_queue_item_result;

    /* setup */
    queue = alloc_queue(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(queue);
    testing_put_queue_item = false;

    /* perform test */
    put_queue_item(queue, &item_to_place_onto_queue);
    get_queue_item_result = get_queue_item(queue);

    /* check result */
    CU_ASSERT_PTR_EQUAL(get_queue_item_result, &item_to_place_onto_queue);
}

static void test_queue_get_order(void)
{
    void * get_queue_item_result;
    void * second_get_queue_item_result;

    /* setup */
    queue = alloc_queue(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(queue);
    testing_put_queue_item = false;

    /* perform test */
    put_queue_item(queue, &item_to_place_onto_queue);
    put_queue_item(queue, &second_item_to_place_onto_queue);
    get_queue_item_result = get_queue_item(queue);
    second_get_queue_item_result = get_queue_item(queue);

    /* check result */
    CU_ASSERT_PTR_EQUAL(get_queue_item_result, &item_to_place_onto_queue);
    CU_ASSERT_PTR_EQUAL(second_get_queue_item_result, &second_item_to_place_onto_queue);
}

static void test_queue_len(void)
{
    size_t queue_len;

    /* setup */
    queue = alloc_queue(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(queue);
    testing_put_queue_item = false;

    /* perform test */
    queue_len = get_queue_len(queue);

    /* check result */
    CU_ASSERT_EQUAL(queue_len, 0);

    /* add an item to the queue */
    put_queue_item(queue, &item_to_place_onto_queue);

    /* perform test */
    queue_len = get_queue_len(queue);

    /* check result */
    CU_ASSERT_EQUAL(queue_len, 1);

    /* add an item to the queue */
    put_queue_item(queue, &item_to_place_onto_queue);

    /* perform test */
    queue_len = get_queue_len(queue);

    /* check result */
    CU_ASSERT_EQUAL(queue_len, 2);

    /* now remove items from the queue */
    get_queue_item(queue);

    /* perform test */
    queue_len = get_queue_len(queue);

    /* check result */
    CU_ASSERT_EQUAL(queue_len, 1);

    /* now remove items from the queue */
    get_queue_item(queue);

    /* perform test */
    queue_len = get_queue_len(queue);

    /* check result */
    CU_ASSERT_EQUAL(queue_len, 0);
}

static void test_get_queue_item_would_block(void)
{
    /* setup */
    queue = alloc_queue(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(queue);
    would_have_blocked = false;
    testing_put_queue_item = false;

    /* perform test */
    get_queue_item(queue);

    /* check result */
    CU_ASSERT_TRUE(would_have_blocked);

    /* setup */
    queue = alloc_queue(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(queue);
    would_have_blocked = false;

    /* perform test */
    put_queue_item(queue, &item_to_place_onto_queue);
    get_queue_item(queue);

    /* check result */
    CU_ASSERT_FALSE(would_have_blocked);
}

static void test_put_queue_item_would_block(void)
{
    /* initial setup */
    queue = alloc_queue(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(queue);
    testing_put_queue_item = true;

    /* test setup */

    /* perform test */
    would_have_blocked = false;
    put_queue_item(queue, &item_to_place_onto_queue);

    /* check result */
    CU_ASSERT_FALSE(would_have_blocked);

    put_queue_item(queue, &item_to_place_onto_queue);

    /* check result */
    CU_ASSERT_FALSE(would_have_blocked);

    /* perform test */
    put_queue_item(queue, &item_to_place_onto_queue);

    /* check result */
    CU_ASSERT_TRUE(would_have_blocked);
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
    pSuite = CU_add_suite("libmessage_queue_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_queue_get", test_queue_get) == NULL)
         || (CU_add_test(pSuite, "test_queue_get_order", test_queue_get_order) == NULL)
         || (CU_add_test(pSuite, "test_queue_len", test_queue_len) == NULL)
         || (CU_add_test(pSuite, "test_get_queue_item_would_block", test_get_queue_item_would_block) == NULL)
         || (CU_add_test(pSuite, "test_put_queue_item_would_block", test_put_queue_item_would_block) == NULL)
         )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    //	Clean up registry and return
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    //	Ensure we return an error if any tests failed
    if (number_of_failures)
        return -1;

    return CU_get_error();
}



