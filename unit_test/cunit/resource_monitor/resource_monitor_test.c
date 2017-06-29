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

#define OUTFILE "/tmp/cpu_usage"

char * get_proc_stat_file_name(void);

#define MAX_CPU 5

// main loop in resource monitor daemon
int res_main(int num_iterations);

static struct cpu_status_t
{
    double current;
    double min;
    double max;
    double average;
} cpu_status[MAX_CPU];


int init_suite(void)
{
    return 0;
}
int clean_suite(void)
{
    return 0;
}

// We create a /tmp/proc/stat file with known values which is then
// fed to the resource monitor's main look and the output file is checked.
// Two sets of readings are required to calculate CPU usage.
char const * normal_proc_stat1 =
    "cpu  27617493 41787 3747150 688783447 142893 40 145214 0 0 0 \r\n"\
    "cpu0 8170947 14690 1175612 170579617 36257 6 70752 0 0 0 \r\n"\
    "cpu1 7356681 6520 991528 171604289 37457 2 54582 0 0 0 \r\n"\
    "cpu2 6153994 10892 798858 173098845 39099 11 13390 0 0 0 \r\n"\
    "cpu3 5935870 9684 781151 173500695 30078 19 6488 0 0 0";

char const * normal_proc_stat2 =
    "cpu  28111074 43619 4006106 715618535 144047 41 153305 0 0 0 \r\n"\
    "cpu0 8325378 14804 1256183 177232474 36562 6 75953 0 0 0 \r\n"\
    "cpu1 7481748 7394 1056883 178307091 37806 2 56960 0 0 0 \r\n"\
    "cpu2 6260795 11624 856474 179834488 39417 12 13767 0 0 0 \r\n"\
    "cpu3 6043151 9796 836565 180244480 30260 19 6624 0 0 0";


// A bad proc stat (missing lines)
char const * invalid_proc_stat =
    "cpu 27617493 41787 3747150 688783447 142893 40 145214 0 0 0 \r\n"\
    "cpu2 6153994 10892 798858 173098845 39099 11 13390 0 0 0 \r\n"\
    "cpu3 5935870 9684 781151 173500695 30078 19 6488 0 0 0";

/************* Test case functions ****************/
/* stubs */
unsigned int sleep(unsigned int seconds)
{
    return 0;
}

// We create a /tmp/proc/stat file used for testing
// instead of using the real /proc/stat
static int create_proc_stat(char const * buf)
{
    FILE * fp;
    int ret = -1;
    size_t len = strlen(buf);

    if ((fp = fopen(get_proc_stat_file_name(), "w+")) == NULL)
    {
        printf("fopen %s\r\n", get_proc_stat_file_name());
        goto error;
    }

    if ((fwrite(buf, 1, len, fp) != len))
    {
        perror("fwrite failed for /tmp/proc/stat");
        goto error;
    }

    fclose(fp);

    ret = 0;
error:

    return ret;
}

// Has the format
// 'Total CPU Usage: 3.29%'
static double get_total_cpu_usage(FILE * fp)
{
    char buf[16], buf1[16], buf2[16], buf3[16];
    char * ptr;
    char * line = NULL;
    size_t len = 0;

    if (getline(&line, &len, fp) != -1)
    {
        sscanf(line, "%s %s %s %s", buf, buf1, buf2, buf3);
    }
    ptr = strstr(buf3, "%");
    if (ptr != NULL)
    {
        *ptr = 0x0;
    }
    else
    {
        printf("Invalid format for %s\r\n", OUTFILE);
    }

    free(line);
    return atof(buf3);
}

static double get_cpu_usage(FILE * fp)
{
    char buf1[16], buf2[16], buf3[16], *ptr;
    char * line = NULL;
    size_t len = 0;

    if (getline(&line, &len, fp) != -1)
    {
        sscanf(line, "%s %s %s", buf1, buf2, buf3);
    }
    ptr = strstr(buf3, "%");

    if (ptr != NULL)
    {
        *ptr = 0x0;
    }

    free(line);
    return atof(buf3);
}

// Read the values from /tmp/cpu_usage
static int read_cpu_usage(void)
{
    FILE * fp;
    int ret = -1, i;
    char * line = NULL;
    size_t len = 0;

    if ((fp = fopen(OUTFILE, "r")) == NULL)
    {
        perror("fopen");
        goto error;
    }

    for (i = 0; i < MAX_CPU; i++)
    {
        if (i == 0)
        {
            cpu_status[0].current = get_total_cpu_usage(fp);
            cpu_status[0].min     = get_total_cpu_usage(fp);
            cpu_status[0].max     = get_total_cpu_usage(fp);
            cpu_status[0].average = get_total_cpu_usage(fp);
        }
        else
        {
            cpu_status[i].current = get_cpu_usage(fp);
            cpu_status[i].min     = get_cpu_usage(fp);
            cpu_status[i].max     = get_cpu_usage(fp);
            cpu_status[i].average = get_cpu_usage(fp);

            // Skip the blank line in between
            if (getline(&line, &len, fp) <= 0)
            {
                return -1;
            }
        }
        //printf("cpu %d current = %f \r\n", i, cpu_status[i].current);
        //printf("cpu %d min = %f \r\n", i, cpu_status[i].min);
        //printf("cpu %d max = %f \r\n", i, cpu_status[i].max);
        //printf("cpu %d average = %f \r\n", i, cpu_status[i].average);
    }

    fclose(fp);

    ret = 0;
error:
    if (line != NULL)
    {
        free(line);
    }

    return ret;
}

/******* Test Cases ******/
void test_valid_proc_stat(void)
{
    create_proc_stat(normal_proc_stat1);
    res_main(1); // Iterate the main loop
    create_proc_stat(normal_proc_stat2);
    res_main(1);
    read_cpu_usage();

    // Compare against pre-calculated cpu usage values
    CU_ASSERT_EQUAL(cpu_status[0].average, 3.98);
    CU_ASSERT_EQUAL(cpu_status[0].current, 2.76);
    CU_ASSERT_EQUAL(cpu_status[0].max, 4.38);
    CU_ASSERT_EQUAL(cpu_status[0].min, 2.76);
}

void test_invalid_proc_stat(void)
{
    struct stat sb;

    remove(get_proc_stat_file_name());
    remove(OUTFILE);
    create_proc_stat(invalid_proc_stat);
    res_main(1); // Iterate the main loop
                 // Assert that the file was not created
    CU_ASSERT_EQUAL(stat(OUTFILE, &sb), -1);
}

void test_missing_proc_stat(void)
{
    struct stat sb;
    int status;

    remove(get_proc_stat_file_name());
    remove(OUTFILE);
    res_main(1); // Iterate the main loop
                 // Assert that the output file was not created
    status = stat(OUTFILE, &sb);
    printf("status %d\r\n", status);
    CU_ASSERT_EQUAL(stat(OUTFILE, &sb), -1);
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
        || (CU_add_test(pSuite, "test_valid_proc_stat", test_valid_proc_stat) == NULL)
        || (CU_add_test(pSuite, "test_missing_proc_stat", test_missing_proc_stat) == NULL)
        || (CU_add_test(pSuite, "test_invalid_proc_stat", test_invalid_proc_stat) == NULL)
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
char * get_proc_stat_file_name(void)
{
    static char * proc_stat_file_name = "/tmp/proc/stat";

    if (mkdir("/tmp/proc", S_IRWXU))
    {
        if (errno != EEXIST)
        {
            perror("Could not create /tmp/proc\r\n");
        }
    }

    return proc_stat_file_name;
}


