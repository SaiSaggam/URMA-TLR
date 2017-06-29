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

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

static char firmware_name[] = "lr54-1.0.0.2.bin";
uint8_t fw_data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};

/************* Test case functions ****************/

static void test_fw_target_count(void)
{
    connector_callback_status_t result;
    connector_firmware_count_t info;

    result = app_firmware_handler(connector_request_id_firmware_target_count, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.count, 1);
}

static void test_fw_valid_target_in_info(void)
{
    connector_callback_status_t result;
    connector_firmware_info_t info = {.target_number = 0};

    result = app_firmware_handler(connector_request_id_firmware_info, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_STRING_EQUAL(info.description, "Binary Image");
    CU_ASSERT_STRING_EQUAL(info.filespec, "[-+.a-zA-Z0-9]+\\.[bB][iI][nN]");
}

static void test_fw_invalid_target_in_info(void)
{
    connector_callback_status_t result;
    connector_firmware_info_t info = {.target_number = 3};

    result = app_firmware_handler(connector_request_id_firmware_info, &info);
    CU_ASSERT_EQUAL(result, connector_callback_error);
}

static void test_fw_firmware_info(void)
{
    test_fw_valid_target_in_info();
    test_fw_invalid_target_in_info();
}

static void test_valid_target_in_start(void)
{
    connector_callback_status_t result;
    connector_firmware_download_start_t info = {.target_number = 0};

    info.filename = firmware_name;
    result = app_firmware_handler(connector_request_id_firmware_download_start, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.status, connector_firmware_status_success);
}

static void test_invalid_firmware_name_in_start(void)
{
    connector_callback_status_t result;
    connector_firmware_download_start_t info = {.target_number = 0};

    info.filename = NULL;
    result = app_firmware_handler(connector_request_id_firmware_download_start, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.status, connector_firmware_status_download_denied);
}

static void test_invalid_target_in_start(void)
{
    connector_callback_status_t result;
    connector_firmware_download_start_t info = {.target_number = 3};

    info.filename = firmware_name;
    result = app_firmware_handler(connector_request_id_firmware_download_start, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.status, connector_firmware_status_download_denied);
}

static void test_fw_download_start(void)
{
    test_invalid_target_in_start();
    test_invalid_firmware_name_in_start();
    test_valid_target_in_start();
}

static void test_fw_download_data(void)
{
    connector_callback_status_t result;
    connector_firmware_download_data_t info = {.target_number = 0, .image = {.offset = 0, .data = fw_data, .bytes_used = sizeof fw_data}};

    result = app_firmware_handler(connector_request_id_firmware_download_data, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.status, connector_firmware_status_success);
}

static void test_valid_target_in_complete(void)
{
    connector_callback_status_t result;
    connector_firmware_download_complete_t info = {.target_number = 0};

    result = app_firmware_handler(connector_request_id_firmware_download_complete, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.status, connector_firmware_status_success);
}

static void test_invalid_target_in_complete(void)
{
    connector_callback_status_t result;
    connector_firmware_download_complete_t info = {.target_number = 3};

    result = app_firmware_handler(connector_request_id_firmware_download_complete, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.status, connector_firmware_download_not_complete);
}

static void test_fw_download_complete(void)
{
    test_valid_target_in_complete();
    test_invalid_target_in_complete();
}

static void test_fw_download_abort(void)
{
    connector_callback_status_t result;
    connector_firmware_download_abort_t info = {.target_number = 0};

    result = app_firmware_handler(connector_request_id_firmware_download_abort, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
    CU_ASSERT_EQUAL(info.status, connector_firmware_status_success);
}

static void test_fw_target_reset(void)
{
    connector_callback_status_t result;
    connector_firmware_reset_t info = {.target_number = 0};

    result = app_firmware_handler(connector_request_id_firmware_target_reset, &info);
    CU_ASSERT_EQUAL(result, connector_callback_continue);
}

/************* Test Runner Code goes here **************/
int add_cc_firmware_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "ddp_firmware_test_suite", init_suite, clean_suite );
    if (pSuite == NULL) goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_fw_target_count", test_fw_target_count) == NULL) goto error;
    if (CU_add_test(pSuite, "test_fw_firmware_info", test_fw_firmware_info) == NULL) goto error;
    if (CU_add_test(pSuite, "test_fw_download_start", test_fw_download_start) == NULL) goto error;
    if (CU_add_test(pSuite, "test_fw_download_data", test_fw_download_data) == NULL) goto error;
    if (CU_add_test(pSuite, "test_fw_download_complete", test_fw_download_complete) == NULL) goto error;
    if (CU_add_test(pSuite, "test_fw_download_abort", test_fw_download_abort) == NULL) goto error;
    if (CU_add_test(pSuite, "test_fw_target_reset", test_fw_target_reset) == NULL) goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* ----------------------------------------------------------------------------  */
/* stub functions */
int get_tlr_firmware_version(char ** version)
{
    *version = strdup("1.2.3.4-hash");
    return 0;
}

int system_command(char const * const format, ...)
{
    char * command = NULL;
    va_list args;
    int result = -1;

    va_start(args, format);

    if (vasprintf(&command, format, args) > 0)
    {
        if (strstr(command, "reboot") != NULL)
        {
            CU_ASSERT_STRING_EQUAL(command, "action_reboot");
        }
        else
        {
            CU_ASSERT_STRING_EQUAL(command, "action_update_firmware -u cloud /tmp/lr54-1.0.0.2.bin");
        }

        result = 0;
        free(command);
    }

    va_end(args);
    return result;
}
