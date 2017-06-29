#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"

/*
 * This is where we include the header files of the C code under test
 */
extern "C"
{
    /* include C header files of code under test */
    #include <count_sas.h>
    #include <digiutils.h>
    #include <config_mgr.h>
    int count_sas_from_ipsec_status_output(FILE * const fp);
}

/* mocked functions */
enum test_type_t
{
    no_lines,
    one_line_without_installed,
    one_line_with_installed
};
static enum test_type_t test_type;
static int number_of_times_to_return_a_string;
static char const * string_to_return;

ssize_t getline(char ** lineptr, size_t * n, FILE * stream)
{
    mock().actualCall("getline");
    switch (test_type)
    {
        case one_line_with_installed:
        case one_line_without_installed:
            if (number_of_times_to_return_a_string-- == 0)
                return -1;
            if ( *lineptr != NULL)
            {
                free( *lineptr);
            }
            *lineptr = strdup(string_to_return);
            return strlen(*lineptr);
        case no_lines:
            return -1;
    }
}

/* tests */
TEST_GROUP(count_sas_from_ipsec_status_output)
{
    void setup()
    {
        mock().clear();
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(count_sas_from_ipsec_status_output, no_lines_read_gives_zero_count)
{
    int expected_count;
    int actual_count;
    /* setup */
    expected_count = 0;
    test_type = no_lines;
    mock().expectOneCall("getline");

    /* perform test */
    actual_count = count_sas_from_ipsec_status_output(NULL);

    /* check results */
    LONGS_EQUAL(expected_count, actual_count);
    mock().checkExpectations();
}

TEST(count_sas_from_ipsec_status_output, one_line_read_without_installed_gives_zero_count)
{
    int expected_count;
    int actual_count;

    /* setup */
    test_type = one_line_without_installed;
    string_to_return = "some line without the word we're searching for in it\n";
    number_of_times_to_return_a_string = 1;
    expected_count = 0;
    mock().expectNCalls(number_of_times_to_return_a_string+1, "getline");

    /* perform test */
    actual_count = count_sas_from_ipsec_status_output(NULL);

    /* check results */
    LONGS_EQUAL(expected_count, actual_count);
    mock().checkExpectations();
}

TEST(count_sas_from_ipsec_status_output, multiple_lines_read_without_installed_gives_zero_count)
{
    int expected_count;
    int actual_count;

    /* setup */
    test_type = one_line_without_installed;
    string_to_return = "some line without the word we're searching for in it\n";
    number_of_times_to_return_a_string = 10;
    expected_count = 0;
    mock().expectNCalls(number_of_times_to_return_a_string+1, "getline");

    /* perform test */
    actual_count = count_sas_from_ipsec_status_output(NULL);

    /* check results */
    LONGS_EQUAL(expected_count, actual_count);
    mock().checkExpectations();
}

TEST(count_sas_from_ipsec_status_output, one_line_read_with_installed_gives_one_count)
{
    int expected_count;
    int actual_count;

    /* setup */
    test_type = one_line_with_installed;
    string_to_return = "      ipsec1{4}:  INSTALLED, TUNNEL, reqid 4, ESP in UDP SPIs: a20b2975_i e8333465_o\n";
    number_of_times_to_return_a_string = 1;
    expected_count = number_of_times_to_return_a_string;
    mock().expectNCalls(number_of_times_to_return_a_string+1, "getline");

    /* perform test */
    actual_count = count_sas_from_ipsec_status_output(NULL);

    /* check results */
    LONGS_EQUAL(expected_count, actual_count);
    mock().checkExpectations();
}

TEST(count_sas_from_ipsec_status_output, multiple_lines_read_with_installed)
{
    int expected_count;
    int actual_count;

    /* setup */
    test_type = one_line_with_installed;
    string_to_return = "      ipsec1{4}:  INSTALLED, TUNNEL, reqid 4, ESP in UDP SPIs: a20b2975_i e8333465_o\n";
    number_of_times_to_return_a_string = 10;
    expected_count = number_of_times_to_return_a_string;
    mock().expectNCalls(number_of_times_to_return_a_string+1, "getline");

    /* perform test */
    actual_count = count_sas_from_ipsec_status_output(NULL);

    /* check results */
    LONGS_EQUAL(expected_count, actual_count);
    mock().checkExpectations();
}

