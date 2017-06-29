
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <stdio.h>
#include <string.h>
#include "test_probing.h"

int main(void)
{
    unsigned int number_of_failures;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        goto error;

    /* add the tests to the suite */
    if (add_probing_tests())
        goto error;


    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");


    /* Clean up registry and return */
    number_of_failures = CU_get_number_of_failures();

    // Ensure we return an error if any tests failed
    if (number_of_failures)
        return -1;

error:
    CU_cleanup_registry();
    return CU_get_error();
}

