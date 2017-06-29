
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
#include <unistd.h>
#include <time.h>

#include    "cli.h"
#include    "cli_timeout.h"


/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

struct tm     original_tm_time;
static unsigned long seconds_to_simulate;
static unsigned long nseconds_to_simulate;

void remember_current_time()
{
    time_t    tm;

    time(&tm);
    memcpy(&original_tm_time, localtime(&tm), sizeof(original_tm_time));
    original_tm_time.tm_isdst = -1;
}

void putback_original_time()
{
    time_t    tm;

    tm = mktime(&original_tm_time);
    stime(&tm);
}


int set_bogus_date_time (  )
{
    time_t    tm;
    struct tm tm_time;
    struct tm *ptm_time = &tm_time;

    memset ( ptm_time, 0, sizeof *ptm_time);
    ptm_time->tm_hour = 4;
    ptm_time->tm_mday = 1;
    ptm_time->tm_mon = 4;
    ptm_time->tm_year = 2015 - 1900;
    ptm_time->tm_isdst = -1;

    tm = mktime(ptm_time);
    if ( stime(&tm) < 0 )
        {
        printf ( "need root access to test they work with changing time\n" );
        return 1;
        }
    return 0;
}

int clock_gettime(clockid_t clock_id, struct timespec * tp)
{
    tp->tv_sec = seconds_to_simulate;
    tp->tv_nsec = nseconds_to_simulate;

    return 0;
}

/************* Test case functions ****************/
void test_get_cpu_seconds()
{
//   printf ( "got %ld\n", getcpuseconds() );
    long    cnt1;
    long    ecnt1;
    long    dcnt1;
    long milliseconds_to_simulate;

    /* simulate 1 second passing */
    /* setup */
    seconds_to_simulate = 0;
    nseconds_to_simulate = 0;
    cnt1 = getmsectickcount1();
    seconds_to_simulate = 1;
    nseconds_to_simulate = 0;
    ecnt1 = getmsectickcount1();
    dcnt1 = ecnt1-cnt1;
    /* check result */
    CU_ASSERT_EQUAL (dcnt1, 1000);

    /* simulate < 1 second passing */
    /* setup */
    milliseconds_to_simulate = 500;
    seconds_to_simulate = 0;
    nseconds_to_simulate = 0;
    cnt1 = getmsectickcount1();
    seconds_to_simulate = 0;
    nseconds_to_simulate = milliseconds_to_simulate * 1000000;
    ecnt1 = getmsectickcount1();
    dcnt1 = ecnt1-cnt1;
    /* check result */
    CU_ASSERT_EQUAL (dcnt1, milliseconds_to_simulate);

    /* simulate < 1 millisecond passing */
    /* setup */
    milliseconds_to_simulate = 0;
    seconds_to_simulate = 0;
    nseconds_to_simulate = 0;
    cnt1 = getmsectickcount1();
    seconds_to_simulate = 0;
    nseconds_to_simulate = milliseconds_to_simulate * 1000000 + 500;    /* some short time < 1ms */
    ecnt1 = getmsectickcount1();
    dcnt1 = ecnt1-cnt1;
    /* check result */
    CU_ASSERT_EQUAL (dcnt1, milliseconds_to_simulate);
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
   pSuite = CU_add_suite( "cli_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "test_get_cpu_seconds", test_get_cpu_seconds))

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

#endif


/*
   // Run all tests using the automated interface
   CU_automated_run_tests();
   CU_list_tests_to_file();

   // Run all tests using the console interface
   CU_console_run_tests();
*/

   /* Clean up registry and return */
   number_of_failures = CU_get_number_of_failures();
   CU_cleanup_registry();

   // Ensure we return an error if any tests failed.
   if (number_of_failures)
      return -1;

   return CU_get_error();
}



/****************  Stubs   ***************************/



