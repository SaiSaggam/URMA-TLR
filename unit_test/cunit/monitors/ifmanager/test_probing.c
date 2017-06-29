#include "probing.h"
#include <tlr_common.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

static int create_work_thread_result;

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

/* prototype of static functions under test */
bool read_until_line_contains_string(FILE * fp, char const * const search_string);


/* stubs */
int create_worker_thread(pthread_t * const thread_id,
                         bool create_detached,
                         size_t maximum_tries,
                         size_t retry_delay_milliseconds,
                         void * (*thread_fn)(void *),
                         void * const thread_fn_arg)
{
    return create_work_thread_result;

}

/************* Test case functions ****************/
static void test_send_probe_handles_create_worker_thread_results(void)
{
    probe_context_st * actual_result;

    /* handles successful create_worker_thread() */
    /* setup */
    create_work_thread_result = 0;
    /* perform test */
    actual_result = send_probe("", "", 0, 0, NULL, NULL, 0);
    /* check result */
    CU_ASSERT_PTR_NOT_NULL(actual_result);
    /* cleanup*/
    free_probe(actual_result);

    /* handles failed create_worker_thread() */
    /* setup */
    create_work_thread_result = -1;
    /* perform test */
    actual_result = send_probe("", "", 0, 0, NULL, NULL, 0);
    /* check result */
    CU_ASSERT_PTR_NULL(actual_result);
    /* cleanup*/
    free_probe(actual_result);
}

#define SEARCH_STRING "search string"
#define SOME_OTHER_STRING "some other string"

static char * response_contains_search_string_first_table[] =
{
    SEARCH_STRING"\n",
    SOME_OTHER_STRING"\n",
    NULL
};

static char * response_contains_search_string_middle_table[] =
{
    SOME_OTHER_STRING"\n",
    SEARCH_STRING"\n",
    SOME_OTHER_STRING"\n",
    NULL
};

static char * response_contains_search_string_last_table[] =
{
    SOME_OTHER_STRING"\n",
    SEARCH_STRING"\n",
    NULL
};

static char * response_doesnt_contain_search_string_table[] =
{
    SOME_OTHER_STRING"\n",
    NULL
};

static char * * current_search_string_table;
static size_t search_string_index;

ssize_t getline(char ** lineptr, size_t * n, FILE * stream)
{
    /* free memory from a previous call */
    free(*lineptr);
    if (current_search_string_table[search_string_index] != NULL)
    {
        *lineptr = strdup(current_search_string_table[search_string_index]);
    }
    else
    {
        *lineptr = NULL;
    }
    search_string_index++;

    return (*lineptr == NULL) ? -1 : strlen(*lineptr);
}

struct file_contains_string_tests_st
{
    char * * search_string_table;
    bool expected_result;
};

static struct file_contains_string_tests_st file_contains_string_tests[] =
{
    {
        .search_string_table = response_contains_search_string_first_table,
        .expected_result = true
    },
    {
        .search_string_table = response_contains_search_string_middle_table,
        .expected_result = true
    },
    {
        .search_string_table = response_contains_search_string_last_table,
        .expected_result = true
    },
    {
        .search_string_table = response_doesnt_contain_search_string_table,
        .expected_result = false
    }
};

static void test_read_until_line_contains_string(void)
{
    bool expected_result;
    bool actual_result;
    int test_index;

    for (test_index = 0; test_index < ARRAY_SIZE(file_contains_string_tests); test_index++)
    {
        /* setup */
        search_string_index = 0;
        current_search_string_table = file_contains_string_tests[test_index].search_string_table;
        expected_result = file_contains_string_tests[test_index].expected_result;
        /* perform test */
        actual_result = read_until_line_contains_string(NULL, SEARCH_STRING);
        /* check result */
        CU_ASSERT_EQUAL(actual_result, expected_result);
    }
}

/************* Test Runner Code goes here **************/

int add_probing_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "probing_test_suite", init_suite, clean_suite );
    if (pSuite == NULL)
        goto error;

    /* add the tests to the suite */
    if (0
        || CU_add_test(pSuite, "test_send_probe_handles_create_worker_thread_results", test_send_probe_handles_create_worker_thread_results) == NULL
        || CU_add_test(pSuite, "test_read_until_line_contains_string", test_read_until_line_contains_string) == NULL
       )
    {
        goto error;
    }

    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/************* Stubs **************/

FILE *tlr_popen(const char *command, const char *type)
{
    return NULL;
}

int tlr_pclose(FILE *fp)
{
    return 0;
}
