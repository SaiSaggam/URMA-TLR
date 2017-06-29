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

extern "C"
{
    extern int get_cpu_usage(void);
    extern struct
    {
        int current;
        int min;
        int max;
        int average;
    } cpu_usage;
}

#include <CppUTest/TestHarness.h>
#include "CppUTestExt/MockSupport.h"
#include <stdio.h>

FILE * fopen(const char * path, const char * mode)
{
    mock().actualCall("fopen").withParameter("path", path).withParameter("mode", mode);

    return (FILE *)mock().pointerReturnValue();
}

ssize_t getline(char ** lineptr, size_t * n, FILE * stream)
{
    mock().actualCall("getline").withOutputParameter("lineptr", lineptr).withOutputParameter("n", n);

    return mock().longIntReturnValue();
}

int fclose(FILE * fp)
{
    return 0;
}

#define cpu_usage_filename "/tmp/cpu_usage"

#define CURRENT_INIT    1234
#define MIN_INIT        2345
#define MAX_INIT        3456
#define AVERAGE_INIT    4567

TEST_GROUP(get_cpu_usage)
{
    void setup(void)
    {
        cpu_usage.current = CURRENT_INIT;
        cpu_usage.min = MIN_INIT;
        cpu_usage.max = MAX_INIT;
        cpu_usage.average = AVERAGE_INIT;
    }
};

TEST(get_cpu_usage, fopen_returns_null_results_in_failure)
{
    int result;

    mock().expectOneCall("fopen").withParameter("path", cpu_usage_filename).withParameter("mode", "r").andReturnValue((void *)NULL);

    result = get_cpu_usage();

    LONGS_EQUAL(-1, result);
}

TEST(get_cpu_usage, empty_file_leaves_output_untouched)
{
    int result;

    mock().expectOneCall("fopen").withParameter("path", cpu_usage_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr = NULL;
    size_t n = 0;
    mock().expectOneCall("getline").andReturnValue(-1).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_cpu_usage();

    LONGS_EQUAL(0, result);
    LONGS_EQUAL(CURRENT_INIT, cpu_usage.current);
    LONGS_EQUAL(MIN_INIT, cpu_usage.min);
    LONGS_EQUAL(MAX_INIT, cpu_usage.max);
    LONGS_EQUAL(AVERAGE_INIT, cpu_usage.average);
}

TEST(get_cpu_usage, valid_file_parsed_correctly)
{
    int result;

    mock().expectOneCall("fopen").withParameter("path", cpu_usage_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr1;
    size_t n1 = asprintf(&lineptr1, "%s", "Total CPU Usage: 1.23%") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n1-1)).withOutputParameterReturning("lineptr", &lineptr1, sizeof lineptr1).withOutputParameterReturning("n", &n1, sizeof n1);

    char * lineptr2;
    size_t n2 = asprintf(&lineptr2, "%s", "Total CPU Min:   4.56%") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n2-1)).withOutputParameterReturning("lineptr", &lineptr2, sizeof lineptr2).withOutputParameterReturning("n", &n2, sizeof n2);

    char * lineptr3;
    size_t n3 = asprintf(&lineptr3, "%s", "Total CPU Max:   7.89%") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n3-1)).withOutputParameterReturning("lineptr", &lineptr3, sizeof lineptr3).withOutputParameterReturning("n", &n3, sizeof n3);

    char * lineptr4;
    size_t n4 = asprintf(&lineptr4, "%s", "Total CPU Avg:   10.11%") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n4-1)).withOutputParameterReturning("lineptr", &lineptr4, sizeof lineptr4).withOutputParameterReturning("n", &n4, sizeof n4);

    char * lineptr = NULL;
    size_t n = 0;
    mock().expectOneCall("getline").andReturnValue(-1).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_cpu_usage();

    LONGS_EQUAL(0, result);
    LONGS_EQUAL(1, cpu_usage.current);
    LONGS_EQUAL(4, cpu_usage.min);
    LONGS_EQUAL(7, cpu_usage.max);
    LONGS_EQUAL(10, cpu_usage.average);

    free(lineptr1);
    free(lineptr2);
    free(lineptr3);
    free(lineptr4);
}

