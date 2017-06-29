#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

#include "config_mgr.h"
#include "plain_socket.h"
#include "tlr_print.h"
#include "cc_status.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdarg.h>

/* Test Suite setup and cleanup functions: */
static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }
extern bool show_cloud(int argc, char *argv[]);

static bool string_found;
static tlr_response_format_t expected_format;
static char const * expected_str = NULL;
static bool read_complete = false;

/************* Test case functions ****************/
static void test_show_cloud_format(tlr_response_format_t format, char * format_str)
{
    char * argv[] = {"show_cloud", "-i", "1", "-f", format_str};
    int argc = (sizeof argv) / (sizeof argv[0]);
    bool result;

    string_found = false;
    expected_str = format_str;
    expected_format = format;
    read_complete = false;
    result = show_cloud(argc, argv);

    CU_ASSERT_TRUE(result);
    CU_ASSERT_TRUE(read_complete);
    CU_ASSERT_TRUE(string_found);
}

static void test_show_cloud_ascii(void)
{
    test_show_cloud_format(tlr_response_format_ascii, TLR_FORMAT_ASCII);
}

static void test_show_cloud_json(void)
{
    test_show_cloud_format(tlr_response_format_json, TLR_FORMAT_JSON);
}

static void test_show_cloud_invalid(void)
{
    char * argv[] = {"show_cloud", "-i", "1", "-f", "unknown"};
    int argc = (sizeof argv) / (sizeof argv[0]);
    bool result;

    read_complete = false;
    expected_format = tlr_response_format_unknown;
    result = show_cloud(argc, argv);
    CU_ASSERT_FALSE(result);

    read_complete = true;
    result = show_cloud(argc, argv);
    CU_ASSERT_FALSE(result);
}

static void test_show_cloud(void)
{
    test_show_cloud_ascii();
    test_show_cloud_json();
    test_show_cloud_invalid();
}

/************* Test Runner Code goes here **************/
int add_show_cloud_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "show_cloud_test_suite", init_suite, clean_suite );
    if (pSuite == NULL) goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_show_cloud", test_show_cloud) == NULL) goto error;
    /* newly added function tests goes here */

    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* ----------------------------------------------------------------------------  */
/* stub functions */
int connect_to_plain_socket(char const * const socket_name, bool const use_abstract_namespace)
{
    CU_ASSERT_STRING_EQUAL(socket_name, CC_STATUS_SOCKET_NAME);
    CU_ASSERT_TRUE(use_abstract_namespace);

    return open("Makefile", O_RDONLY);
}

int dprintf(int fd, const char *format, ...)
{
    char * data = NULL;
    int result = 0;
    va_list args;

    va_start(args, format);
    if (vasprintf(&data, format, args) > 0)
    {
        if (strstr(data, expected_str) != NULL)
        {
            string_found = true;
        }

        result = strlen(data);
        free(data);
    }

    return result;
}

size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type)
{
    CU_ASSERT_EQUAL(group_id, config_state_cloud);
    CU_ASSERT_EQUAL(type, config_mgr_table_type_show);

    return 1;
}

int getlinefd_timeout(char ** const output_buffer, size_t * n, int const fd, struct timeval * const timeout)
{
    int result = 0;

    switch (expected_format)
    {
        case tlr_response_format_ascii:
            if (!read_complete)
            {
                *output_buffer = strdup("Server    : tlr.ascii.com\n");
                result = strlen(*output_buffer);
                read_complete = true;
            }
            break;

        case tlr_response_format_json:
            if (!read_complete)
            {
                *output_buffer = strdup("\"server\":\"tlr.json.com\"\n");
                result = strlen(*output_buffer);
                read_complete = true;
            }
            break;

        default:
            *output_buffer = NULL;
            break;
    }

    CU_ASSERT_PTR_NOT_NULL(output_buffer);
    CU_ASSERT_PTR_NOT_NULL(n);
    CU_ASSERT_EQUAL(timeout->tv_sec, CC_STATUS_MSG_TIMEOUT);
    CU_ASSERT_NOT_EQUAL(fd, -1);

    return result;
}

int parse_show_options(int const argc, char * const *argv, tlr_response_format_t *format, int *instance, int const min_instance, int const max_instance)
{
    int result = 0;
    char * const format_str = argv[argc-1];

    CU_ASSERT(argc > 4);
    CU_ASSERT_PTR_NOT_NULL(argv);
    CU_ASSERT_PTR_NOT_NULL(format);
    CU_ASSERT_PTR_NOT_NULL(instance);
    CU_ASSERT_EQUAL(max_instance, 1);

    if (strcmp(format_str, TLR_FORMAT_ASCII) == 0)
    {
        *format = tlr_response_format_ascii;
    }
    else if (strcmp(format_str, TLR_FORMAT_JSON) == 0)
    {
        *format = tlr_response_format_json;
    }
    else
    {
        *format = tlr_response_format_unknown;
        if (read_complete)
        {
            result = -1;
        }
    }

    CU_ASSERT_EQUAL(*format, expected_format);

    return result;
}

void usage_show(char const * const progname)
{
    CU_ASSERT_PTR_NOT_NULL(progname);
}

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    CU_ASSERT_EQUAL(group_id, config_cloud);
    CU_ASSERT_EQUAL(element_id, config_cloud_state);
    CU_ASSERT_EQUAL(bytes, sizeof(unsigned int));
    CU_ASSERT_PTR_NOT_NULL(value);

    *(unsigned int *)value = 1;
    return config_status_success;
}
