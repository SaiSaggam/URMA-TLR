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

#include <timer.h>
#include <timespec_helpers.h>
#include <tlr_common.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

static volatile bool timer1_callback_was_called;
static void * timer1_callback_user_data_arg;
static volatile bool timer2_callback_was_called;
static void * timer2_callback_user_data_arg;
static int user_data;

extern int do_expired_timer_check(timer_context_st * const context);

int init_suite(void)
{
    return 0;
}
int clean_suite(void)
{
    return 0;
}


/****************  Stubs   ***************************/
int sem_init(sem_t * __sem, int __pshared, unsigned int __value)
{
    return 0;
}

int sem_wait(sem_t * __sem)
{
    return 0;
}

int sem_post(sem_t * __sem)
{
    return 0;
}

int pthread_sigmask(int __how,
                    const __sigset_t * __restrict __newmask,
                    __sigset_t * __restrict __oldmask)
{
    return 0;
}

int pthread_create(pthread_t * __restrict __newthread,
                   const pthread_attr_t * __restrict __attr,
                   void * (*__start_routine)(void *),
                   void * __restrict __arg)
{
    return 0;
}

int pthread_kill(pthread_t __threadid, int __signo)
{
    return 0;
}

static struct timespec current_time;

int clock_gettime(clockid_t __clock_id, struct timespec * __tp)
{
    __tp->tv_sec = current_time.tv_sec;
    __tp->tv_nsec = current_time.tv_nsec;

    return 0;
}

int pselect(int __nfds, fd_set * __restrict __readfds,
            fd_set * __restrict __writefds,
            fd_set * __restrict __exceptfds,
            const struct timespec * __restrict __timeout,
            const __sigset_t * __restrict __sigmask)
{
    return 0;
}

/************* Test case functions ****************/
static void timer1_callback(void * user_data)
{
    timer1_callback_was_called = true;
    timer1_callback_user_data_arg = user_data;
}

static void timer2_callback(void * user_data)
{
    timer2_callback_was_called = true;
    timer2_callback_user_data_arg = user_data;
}

/******* Test Cases ******/
static void test_single_timer_doesnt_expire_with_no_time_elapsed(void)
{
    timer_context_st * timer_context;
    timer_id_st * timer_under_test;
    unsigned int test_time_millisecs;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);
    /* create the timer */
    timer_under_test = create_timer(timer_context, timer1_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_under_test);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    /* start the timer */
    test_time_millisecs = 1000;
    start_timer(timer_under_test, test_time_millisecs);
    /* pretend that no time has elapsed */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    timer1_callback_was_called = false;
    /* perform test */
    do_expired_timer_check(timer_context);

    /* check result*/
    CU_ASSERT_FALSE(timer1_callback_was_called);

}

static void test_single_timer_doesnt_expire_with_some_time_elapsed(void)
{
    timer_context_st * timer_context;
    timer_id_st * timer_under_test;
    unsigned int test_time_millisecs;
    long remainder;
    struct timespec time_to_subtract;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);
    /* create the timer */
    timer_under_test = create_timer(timer_context, timer1_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_under_test);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    /* start the timer */
    test_time_millisecs = 1000;
    start_timer(timer_under_test, test_time_millisecs);

    /* pretend that less than just less than the expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    time_to_subtract.tv_sec = 0;
    time_to_subtract.tv_nsec = milliseconds_to_nanoseconds(1);
    timerspec_sub(&current_time, &time_to_subtract, &current_time);
    timer1_callback_was_called = false;
    /* perform test */
    do_expired_timer_check(timer_context);

    /* check result */
    CU_ASSERT_FALSE(timer1_callback_was_called);

}

static void test_single_timer_expires_with_expiry_time_elapsed(void)
{
    timer_context_st * timer_context;
    timer_id_st * timer_under_test;
    unsigned int test_time_millisecs;
    long remainder;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);
    /* create the timer */
    timer_under_test = create_timer(timer_context, timer1_callback, &user_data);
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_under_test);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    /* start the timer */
    test_time_millisecs = 1000;
    start_timer(timer_under_test, test_time_millisecs);

    /* pretend that the exact expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    timer1_callback_was_called = false;
    timer1_callback_user_data_arg = NULL;
    /* perform test */
    do_expired_timer_check(timer_context);

    /* check result */
    CU_ASSERT_TRUE(timer1_callback_was_called);
    CU_ASSERT_PTR_EQUAL(timer1_callback_user_data_arg, &user_data);

}

static void test_single_timer_expires_with_more_than_expiry_time_elapsed(void)
{
    timer_context_st * timer_context;
    timer_id_st * timer_under_test;
    unsigned int test_time_millisecs;
    long remainder;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);
    /* create the timer */
    timer_under_test = create_timer(timer_context, timer1_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_under_test);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    /* start the timer */
    test_time_millisecs = 1000;
    start_timer(timer_under_test, test_time_millisecs);

    /* pretend that the more than the exact expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder) + 1;
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    timer1_callback_was_called = false;
    /* perform test */
    do_expired_timer_check(timer_context);

    /* check result */
    CU_ASSERT_TRUE(timer1_callback_was_called);

}

static void test_single_timer_doesnt_call_callback_if_stopped_before_expiry_time_elapsed(void)
{
    timer_context_st * timer_context;
    timer_id_st * timer_under_test;
    unsigned int test_time_millisecs;
    long remainder;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);
    /* create the timer */
    timer_under_test = create_timer(timer_context, timer1_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_under_test);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    /* start the timer */
    test_time_millisecs = 1000;
    start_timer(timer_under_test, test_time_millisecs);
    /* stop the timer without elapsing any time */
    stop_timer(timer_under_test);

    /* pretend that the expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    timer1_callback_was_called = false;
    /* perform test */
    do_expired_timer_check(timer_context);

    /* check result */
    CU_ASSERT_FALSE(timer1_callback_was_called);

}

static void test_multiple_timers_expire_in_the_correct_order(void)
{
    timer_context_st * timer_context;
    timer_id_st * first_timer_under_test;
    timer_id_st * second_timer_under_test;
    unsigned int timer1_test_time_millisecs;
    unsigned int timer2_test_time_millisecs;
    long remainder;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);
    /* create the timers */
    first_timer_under_test = create_timer(timer_context, timer1_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(first_timer_under_test);
    second_timer_under_test = create_timer(timer_context, timer2_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(second_timer_under_test);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    /* start the timers */
    timer1_test_time_millisecs = 1000;
    start_timer(first_timer_under_test, timer1_test_time_millisecs);
    timer2_test_time_millisecs = 2000;
    start_timer(second_timer_under_test, timer2_test_time_millisecs);

    /* pretend that the expiry time has elapsed for timer 1 */
    current_time.tv_sec = milliseconds_to_seconds(timer1_test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    timer1_callback_was_called = false;
    timer2_callback_was_called = false;
    /* perform test */
    do_expired_timer_check(timer_context);

    /* check that the first timer callback was called, but not the second */
    CU_ASSERT_TRUE(timer1_callback_was_called);
    CU_ASSERT_FALSE(timer2_callback_was_called);

    /* and this time make the first timer expire after the second timer */
    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    /* start the timers. Set timer1 to expire after timer2 */
    timer1_test_time_millisecs = 2000;
    start_timer(first_timer_under_test, timer1_test_time_millisecs);
    timer2_test_time_millisecs = 1000;
    start_timer(second_timer_under_test, timer2_test_time_millisecs);

    /* pretend that the expiry time has elapsed for timer 2 */
    current_time.tv_sec = milliseconds_to_seconds(timer2_test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    timer1_callback_was_called = false;
    timer2_callback_was_called = false;
    /* perform test */
    do_expired_timer_check(timer_context);

    /* check that the second timer callback was called, but not the first */
    CU_ASSERT_FALSE(timer1_callback_was_called);
    CU_ASSERT_TRUE(timer2_callback_was_called);

    /* pretend that the expiry time has elapsed for the other timer */
    current_time.tv_sec = milliseconds_to_seconds(timer1_test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    timer1_callback_was_called = false;
    /* perform test */
    do_expired_timer_check(timer_context);
    CU_ASSERT_TRUE(timer1_callback_was_called);

}

static void test_timer_is_running(void)
{
    timer_context_st * timer_context;
    timer_id_st * timer_under_test;
    unsigned int test_time_millisecs;
    long remainder;
    bool is_running;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);

    /* create the timer */
    timer_under_test = create_timer(timer_context, timer1_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_under_test);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;

    /* start the timer */
    test_time_millisecs = 1000;
    start_timer(timer_under_test, test_time_millisecs);

    /* pretend that no time has elapsed */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    do_expired_timer_check(timer_context);
    is_running = timer_is_running(timer_under_test);
    CU_ASSERT_TRUE(is_running);

    /* pretend that some time less than the expiry time has elapsed */
    current_time.tv_sec = 0;
    current_time.tv_nsec = milliseconds_to_nanoseconds(test_time_millisecs/2);
    do_expired_timer_check(timer_context);
    is_running = timer_is_running(timer_under_test);
    CU_ASSERT_TRUE(is_running);

    /* pretend that the exact expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    do_expired_timer_check(timer_context);
    is_running = timer_is_running(timer_under_test);
    CU_ASSERT_FALSE(is_running);

    /* pretend that more than the exact expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder) + 1;
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    do_expired_timer_check(timer_context);
    is_running = timer_is_running(timer_under_test);
    CU_ASSERT_FALSE(is_running);
}

static void test_seconds_remaining(void)
{
    timer_context_st * timer_context;
    timer_id_st * timer_under_test;
    unsigned int test_time_millisecs;
    long remainder;
    int seconds;
    struct timespec time_to_subtract;

    /* setup */
    timer_context = open_timer_context();
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_context);

    /* create the timer */
    timer_under_test = create_timer(timer_context, timer1_callback, NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(timer_under_test);

    // Check that seconds_remaining() returns zero if the timer is not started.
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, 0);

    /* set the current time */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;

    /* start the timer */
    test_time_millisecs = 10000;
    start_timer(timer_under_test, test_time_millisecs);

    /* pretend that no time has elapsed */
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, milliseconds_to_seconds(test_time_millisecs, &remainder));

    /* pretend that some time less than half a second has elapsed */
    current_time.tv_sec = 0;
    current_time.tv_nsec = milliseconds_to_nanoseconds(100);
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, milliseconds_to_seconds(test_time_millisecs, &remainder) -1);

    /* pretend that exactly half a second has elapsed */
    current_time.tv_sec = 0;
    current_time.tv_nsec = milliseconds_to_nanoseconds(500);
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, milliseconds_to_seconds(test_time_millisecs, &remainder) -1);

    /* pretend that some time between half a second and a second has elapsed */
    current_time.tv_sec = 0;
    current_time.tv_nsec = milliseconds_to_nanoseconds(900);
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, milliseconds_to_seconds(test_time_millisecs, &remainder) -1);

    /* pretend that exactly one second has elapsed */
    current_time.tv_sec = 1;
    current_time.tv_nsec = 0;
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, milliseconds_to_seconds(test_time_millisecs, &remainder) -1);

    /* pretend that just over one second has elapsed */
    current_time.tv_sec = 1;
    current_time.tv_nsec = milliseconds_to_nanoseconds(100);
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, milliseconds_to_seconds(test_time_millisecs, &remainder) -2);

    /* pretend that the timer is about to expire */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    time_to_subtract.tv_sec = 0;
    time_to_subtract.tv_nsec = milliseconds_to_nanoseconds(1);
    timerspec_sub(&current_time, &time_to_subtract, &current_time);
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, 0);

    /* pretend that the exact expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder);
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, 0);

    /* pretend that some time after the exact expiry time has elapsed */
    current_time.tv_sec = milliseconds_to_seconds(test_time_millisecs, &remainder) + 1;
    current_time.tv_nsec = milliseconds_to_nanoseconds(remainder);
    do_expired_timer_check(timer_context);
    seconds = seconds_remaining(timer_under_test);
    CU_ASSERT_EQUAL(seconds, 0);
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
    pSuite = CU_add_suite("libmessage_timer_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_single_timer_doesnt_expire_with_no_time_elapsed", test_single_timer_doesnt_expire_with_no_time_elapsed) == NULL)
         || (CU_add_test(pSuite, "test_single_timer_doesnt_expire_with_some_time_elapsed", test_single_timer_doesnt_expire_with_some_time_elapsed) == NULL)
         || (CU_add_test(pSuite, "test_single_timer_expires_with_expiry_time_elapsed", test_single_timer_expires_with_expiry_time_elapsed) == NULL)
         || (CU_add_test(pSuite, "test_single_timer_expires_with_more_than_expiry_time_elapsed", test_single_timer_expires_with_more_than_expiry_time_elapsed) == NULL)
         || (CU_add_test(pSuite, "test_single_timer_doesnt_call_callback_if_stopped_before_expiry_time_elapsed", test_single_timer_doesnt_call_callback_if_stopped_before_expiry_time_elapsed) == NULL)
         || (CU_add_test(pSuite, "test_multiple_timers_expire_in_the_correct_order", test_multiple_timers_expire_in_the_correct_order) == NULL)
         || (CU_add_test(pSuite, "test_timer_is_running", test_timer_is_running) == NULL)
         || (CU_add_test(pSuite, "test_seconds_remaining", test_seconds_remaining) == NULL)
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



