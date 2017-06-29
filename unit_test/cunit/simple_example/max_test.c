
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

//#include "CUnit/Automated.h"
//#include "CUnit/Console.h"

#include "max.h"

#include <stdio.h>  // for printf

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test case functions ****************/
void max_test_1(void) {
  CU_ASSERT_EQUAL( max(1,2), 2);
  CU_ASSERT_EQUAL( max(2,1), 2);
}

void max_test_2(void) {
  CU_ASSERT_EQUAL( max(2,2), 2);
  CU_ASSERT_EQUAL( max(0,0), 0);
  CU_ASSERT_EQUAL( max(-1,-1), -1);
}

void max_test_3(void) {
  CU_ASSERT_EQUAL( max(-1,-2), -1);
  CU_ASSERT_EQUAL( max(-1,4), 4);
  CU_ASSERT_EQUAL( max(4,-1), 4);
}

void max_test_4(void) {
  // This checks ints don't go round the clock at 64000
  int       i;
  int       j = 33000;
  int       k = 60000;

  for (i=0; i<10000; i++)
    {
    CU_ASSERT_EQUAL( max(j,k), k);
    CU_ASSERT_EQUAL( max(k,j), k);
    k++;
    }
}


/************* Test Runner Code goes here **************/

int main ( void )
{
    int result;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        result = CU_get_error();
        goto error;
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite( "max_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite )
    {
        result = CU_get_error();
        goto error;
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "max_test_1", max_test_1)) ||
        (NULL == CU_add_test(pSuite, "max_test_2", max_test_2)) ||
        (NULL == CU_add_test(pSuite, "max_test_3", max_test_3)) ||
        (NULL == CU_add_test(pSuite, "max_test_4", max_test_4)))
    {
        result = CU_get_error();
        goto error;
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
	if (CU_basic_run_tests() != CUE_SUCCESS)
	{
        result = CU_get_error();
        goto error;
	}
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    result = CU_get_number_of_failures();

error:
	CU_cleanup_registry();
	return result;
}
