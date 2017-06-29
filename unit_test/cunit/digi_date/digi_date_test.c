
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
#include <time.h>


int set_date_time ( struct tm *ptm_time );



/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }


/************* Test case functions ****************/
void set_date_tests()
{
    struct tm test_time;
    int     retv;

    memset ( &test_time, 0, sizeof test_time );
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);       // should be a failure

    // Now setup something that should be acceptable
    test_time.tm_sec = 0;
    test_time.tm_min = 0;
    test_time.tm_hour = 0;
    test_time.tm_mday = 1;
    test_time.tm_mon = 0;
    test_time.tm_year = 2015-1900;
    test_time.tm_wday = 0;
    test_time.tm_yday = 0;
    test_time.tm_isdst = -1;
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 0);       // should be ok this time

    // Bad seconds.
    test_time.tm_sec = 60;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1); 
    test_time.tm_sec = -1;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);     
    // And should be ok again after clearing seconds.
    test_time.tm_sec = 0;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 0);      

    // Bad minutes.
    test_time.tm_min = 60;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_min = -1;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_min = 0;       // Minutes back to ok.

    // Bad hours.
    test_time.tm_hour = 60;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_hour = -1;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_hour = 0;

    // And should be ok again
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 0);      

    // Bad day of month
    test_time.tm_mday = 0;
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_mday = 32;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_mday = 1;
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 0);   // Just checking our time is ok again at this point.

    // And finally month.... 
    test_time.tm_mon = 12;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_mon = -1;    
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 1);  
    test_time.tm_mon = 0;
    retv = set_date_time ( &test_time );
    CU_ASSERT_EQUAL(retv, 0);  

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
   pSuite = CU_add_suite( "digi_time_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (CU_add_test(pSuite, "set_date_tests", set_date_tests)==NULL) )
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


