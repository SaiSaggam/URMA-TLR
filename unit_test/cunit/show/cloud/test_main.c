
#define USE_BASIC_STYLE      1
#define USE_AUTOMATED_STYLE  0
#define USE_CONSOLE_STYLE    0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include <stdio.h>
#include <string.h>
#include "test_show_cloud.h"

#define STRINGIFY(x) #x

#define ADD_TEST(test) \
    if (test) \
    { \
        fprintf(stderr, "failed to add test %s\n", STRINGIFY(test)); \
        number_of_failures = -1; \
        goto error; \
    }


int main(void)
{
    unsigned int number_of_failures = 0;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        goto error;

    /* add the tests to the suite */
    ADD_TEST(add_show_cloud_tests());

    /* Run all tests using the basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    /* Clean up registry and return */
    number_of_failures = CU_get_number_of_failures();
    if (number_of_failures > 0)
        printf("%d tests failed\n", number_of_failures);

error:
    CU_cleanup_registry();

    // Ensure we return an error if any tests failed
    return (number_of_failures == 0) ? CU_get_error() : -1;
}

