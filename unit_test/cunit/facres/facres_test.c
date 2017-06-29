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
#include "facres.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdbool.h>



int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}



/****   Simulation functions */

static  char    * expected_system_command = NULL;

int system_command(char * const cmd)
{
    CU_ASSERT_PTR_NOT_NULL(expected_system_command);
    if (expected_system_command != NULL)
    {
        CU_ASSERT_STRING_EQUAL(cmd, expected_system_command);
        expected_system_command = NULL;
    }

    return 0;
}


static void clear_expect_reset(void)
{
    expected_system_command = NULL;
}

static void expect_reset(void)
{
    expected_system_command = "dofacres";
}



/************* Test case functions ****************/
bool process_gpio_read_input_line(char * const line, int * const input_value);  // Function we will test

void check_process_gpio_read_input_line(char  * const input_test_data, bool expected_return, int expected_value)
{
    bool    return_result;
    int     return_value;
    char    * str = strdup(input_test_data);

    if (str != NULL)
    {
        return_result = process_gpio_read_input_line(str, &return_value);
        CU_ASSERT_EQUAL(return_result, expected_return);
        if (return_result)
        {
            CU_ASSERT_EQUAL(return_value, expected_value);
        }
        free(str);
    }
}

t_reset_pb_state reset_pb_state_machine(t_reset_pb_state current_state, bool const pb_pressed);     // Function we will test.

void check_reset_pb_state_machine (t_reset_pb_state current_state, bool const pb_pressed, t_reset_pb_state expected_state)
{
    t_reset_pb_state    next_state;
    next_state = reset_pb_state_machine(current_state, pb_pressed);
    CU_ASSERT_EQUAL(next_state, expected_state);
}


/******* Test Cases ******/

void facres_test_good_return(void)
{

    check_process_gpio_read_input_line("bit18 = 0x1\n", true, 1);
    check_process_gpio_read_input_line("bit18 = 0x1", true, 1);
    check_process_gpio_read_input_line(" bit18 = 0x1", true, 1);
    check_process_gpio_read_input_line("bit18 = 0x0", true, 0);
    check_process_gpio_read_input_line("bit18 = 0x00", true, 0);
    check_process_gpio_read_input_line("bit18 = 0x43", true, 67);
    check_process_gpio_read_input_line("bit18 = 0x43\n\n", true, 67);
    check_process_gpio_read_input_line("bit18=0x43", true, 67);
    check_process_gpio_read_input_line("bit18       =       0x40", true, 64);
    check_process_gpio_read_input_line("          bit18       =       0x40", true, 64);

}

void facres_test_bad_return(void)
{
    check_process_gpio_read_input_line("bit18  0x1\n", false, 0);
    check_process_gpio_read_input_line("bit18 : 0x1", false, 0);
    check_process_gpio_read_input_line("bit18 = ", false, 0);
    check_process_gpio_read_input_line("", false, 0);
    check_process_gpio_read_input_line(" ", false, 0);
    check_process_gpio_read_input_line("18 = 0x00", false, 0);
    check_process_gpio_read_input_line("bit18 = hello", false, 0);
    check_process_gpio_read_input_line("bit18 = 0 x 43", false, 0);
}


void facres_test_pb_state_machine(void)
{
    int     cnt;

    for (cnt = 0; cnt < 30; cnt++)
    {
        // should stay in this state while the PB is pressed
        check_reset_pb_state_machine(RESET_PB_WAITING_FOR_RELEASE, true, RESET_PB_WAITING_FOR_RELEASE);
    }

    check_reset_pb_state_machine(RESET_PB_WAITING_FOR_RELEASE, false, RESET_PB_WAITING_FOR_PRESS);
    check_reset_pb_state_machine(RESET_PB_WAITING_FOR_PRESS, false, RESET_PB_WAITING_FOR_PRESS);
    check_reset_pb_state_machine(RESET_PB_WAITING_FOR_PRESS, false, RESET_PB_WAITING_FOR_PRESS);
    check_reset_pb_state_machine(RESET_PB_WAITING_FOR_PRESS, false, RESET_PB_WAITING_FOR_PRESS);

    clear_expect_reset();
    check_reset_pb_state_machine(RESET_PB_WAITING_FOR_PRESS, true, RESET_PB_COUNTING_PRESS);

    for (cnt = 1; cnt < COUNT_FOR_BUTTON-1; cnt++ )
        check_reset_pb_state_machine(RESET_PB_COUNTING_PRESS, true, RESET_PB_COUNTING_PRESS);
    expect_reset();
    // Next time we should get a reset and go back to waiting for RELEASE.
    check_reset_pb_state_machine(RESET_PB_COUNTING_PRESS, true, RESET_PB_WAITING_FOR_RELEASE);
    CU_ASSERT_PTR_NULL(expected_system_command);
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
    pSuite = CU_add_suite("facres_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (0
        || (CU_add_test(pSuite, "facres_test_good_return", facres_test_good_return) == NULL)
        || (CU_add_test(pSuite, "facres_test_bad_return", facres_test_bad_return) == NULL)
        || (CU_add_test(pSuite, "facres_test_pb_state_machine", facres_test_pb_state_machine) == NULL)

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
