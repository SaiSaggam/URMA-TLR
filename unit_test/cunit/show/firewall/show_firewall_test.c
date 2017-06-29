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

#include "tlr_common.h"
#include "tlr_print.h"
#include "show_firewall_work.h"

int init_suite(void)
{
    return 0;
}
int clean_suite(void)
{
    return 0;
}
static int popen_fail = 0;
static int asprintf_value = 0;
int asprintf(char **strp, const char *fmt, ...)
{
    return asprintf_value;
}

static char const program_name[] ="show_firewall";

void test_show_firewall(void)
{
    int status;

    {
        char const * const argv[] = { program_name, "-f "};
        int const argc = asizeof(argv);
        status = show_firewall_main(argc, (char **)argv);
        CU_ASSERT_EQUAL(status, EXIT_FAILURE);
    }

    {
        char const * const argv[] = { program_name, "-fascii", "badtable" };
        int const argc = asizeof(argv);
        status = show_firewall_main(argc, (char **)argv);
        CU_ASSERT_EQUAL(status, EXIT_FAILURE);
    }

    {
        char const * const argv[] = { program_name, "-fascii", "filter" };
        int const argc = asizeof(argv);
        status = show_firewall_main(argc, (char **)argv);
        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
    }

    {
        char const * const argv[] = { program_name, "-fascii", "raw" };
        int argc = asizeof(argv);
        status = show_firewall_main(argc, (char **)argv);
        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
    }

    {
        char const * const argv[] = { program_name, "-fascii", "nat" };
        int const argc = asizeof(argv);
        status = show_firewall_main(argc, (char **)argv);
        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
    }

    {
        char const * const argv[] = { program_name};
        int const argc = asizeof(argv);
        status = show_firewall_main(argc, (char **)argv);
        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
    }

    {
        char const * const argv[] = { program_name, "-fascii"};
        int const argc = asizeof(argv);
        asprintf_value = -1;

        status = show_firewall_main(argc, (char **)argv);
        CU_ASSERT_EQUAL(status, EXIT_FAILURE);
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
        || (CU_add_test(pSuite, "test_show_firewall", test_show_firewall) == NULL)
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

void tlr_event_log(tlr_log_t const priority, char const * const process_name, char const * const format, ...)
{

}

FILE *popen(const char *command, const char *type)
{
    static FILE fd;
    return (popen_fail) ? NULL : &fd;
}

int pclose(FILE *fp)
{
    return 0;
}

ssize_t getline(char **a, size_t *b, FILE *c)
{
    return -1;
}

