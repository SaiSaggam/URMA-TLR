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

#include "test_cc.h"
#include "connector_api.h"
#include "platform.h"
#include "cc_status.h"
#include "plain_socket.h"
#include "config_mgr.h"
#include "tlr_print.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

/* Test Suite setup and cleanup functions: */
static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }
extern void cc_show_callback(int const fd, void * const user_context);

static bool generate_error_condition;
static char dummy_url[] = "tlr.unittest.com";
static char expected_str[64];
static bool string_found;

/************* Test case functions ****************/
static void test_cc_socket_init(void)
{
    bool result;

    generate_error_condition = true;
    result = cc_socket_init();
    CU_ASSERT_FALSE(result);

    generate_error_condition = false;
    result = cc_socket_init();
    CU_ASSERT_TRUE(result);
}

static void test_cc_show_ascii(void)
{
    int const fd = open("Makefile", O_RDONLY);
    cloud_status_t * const status_ptr = cc_get_cloud_status();

    strcpy(dummy_url, "tlr.ascii.com");
    strcpy(status_ptr->server_name, dummy_url);
    strcpy(expected_str, "Server    : tlr.ascii.com");
    string_found = false;
    cc_show_callback(fd, status_ptr);
    CU_ASSERT_TRUE(string_found);
}

static void test_cc_show_json(void)
{
    int const fd = open("Makefile", O_RDONLY);
    cloud_status_t * const status_ptr = cc_get_cloud_status();

    strcpy(dummy_url, "tlr.json.com");
    strcpy(status_ptr->server_name, dummy_url);
    strcpy(expected_str, "\"server\":\"tlr.json.com\"");
    string_found = false;
    cc_show_callback(fd, status_ptr);
    CU_ASSERT_TRUE(string_found);
}

static void test_cc_restart(void)
{
    int const fd = open("Makefile", O_RDONLY);
    cloud_status_t * const status_ptr = cc_get_cloud_status();

    strcpy(expected_str, "restart");
    string_found = false;
    cc_show_callback(fd, status_ptr);
    CU_ASSERT_TRUE(string_found);
}

static void test_cc_show_callback(void)
{
    test_cc_show_ascii();
    test_cc_show_json();
    test_cc_restart();
}

/************* Test Runner Code goes here **************/
int add_cc_show_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "cc_show_test_suite", init_suite, clean_suite );
    if (pSuite == NULL) goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_cc_socket_init", test_cc_socket_init) == NULL) goto error;
    if (CU_add_test(pSuite, "test_cc_show_callback", test_cc_show_callback) == NULL) goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* ----------------------------------------------------------------------------  */
/* stub functions */

struct plain_socket_context_st
{
    char const * socket_name;
    bool use_abstract_namespace;
    plain_socket_callback_fn callback;
    void const * user_context;
};

plain_socket_context_st * open_plain_socket_context(char const * const socket_name, bool use_abstract_namespace, plain_socket_callback_fn callback, void const * const user_context)
{
    struct plain_socket_context_st test_result = {socket_name, use_abstract_namespace, callback, user_context};

    CU_ASSERT_STRING_EQUAL(socket_name, CC_STATUS_SOCKET_NAME);
    CU_ASSERT_TRUE(use_abstract_namespace);
    CU_ASSERT_PTR_NOT_NULL(callback);
    CU_ASSERT_PTR_NOT_NULL(user_context);

    return generate_error_condition ? NULL : &test_result;
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

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    return &config_mgr_group_table[table_index].groups[group_id];
}

int getlinefd_timeout(char ** const output_buffer, size_t * n, int const fd, struct timeval * const timeout)
{
    int result;

    if (strstr(expected_str, "ascii") != NULL)
    {
        *output_buffer = strdup(cc_msg_show_ascii);
        *n = strlen(cc_msg_show_ascii);
    }
    else if (strstr(expected_str, "json") != NULL)
    {
        *output_buffer = strdup(cc_msg_show_json);
        *n = strlen(cc_msg_show_json);
    }
    else if (strstr(expected_str, "restart") != NULL)
    {
        *output_buffer = strdup(cc_msg_restart);
        *n = strlen(cc_msg_restart);
        string_found = true;
    }
    else
    {
        *output_buffer = NULL;
        *n = 0;
    }

    CU_ASSERT_PTR_NOT_NULL(output_buffer);
    CU_ASSERT_PTR_NOT_NULL(n);
    CU_ASSERT_PTR_NOT_NULL(*output_buffer);
    CU_ASSERT_EQUAL(timeout->tv_sec, CC_STATUS_MSG_TIMEOUT);
    CU_ASSERT_NOT_EQUAL(fd, -1);

    result = (int)(*n);
    return result;
}

void format_ifuptime(uint64_t const input_seconds, char * const buf, size_t const bufsize)
{
    CU_ASSERT(input_seconds > 0);
    CU_ASSERT_PTR_NOT_NULL(buf);
    CU_ASSERT(bufsize > 32);

    strcpy(buf, "5 Minutes, 10 seconds");
}
