
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

#include <stdio.h>
#include <string.h>



/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }


/************* Test case functions ****************/
void whatever_tests()
{

}

void more_whatever_tests()
{

}


/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;
   unsigned int number_of_failures;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "eth_status_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (CU_add_test(pSuite, "get_ip_address", whatever_tests)==NULL) ||
        (CU_add_test(pSuite, "get_rx_packets", more_whatever_tests)==NULL)

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

   /* Clean up registry and return */
   number_of_failures = CU_get_number_of_failures();
   CU_cleanup_registry();

   // Ensure we return an error if any tests failed.
   if (number_of_failures)
      return -1;

   return CU_get_error();
}



/****************  Stubs   ***************************/



