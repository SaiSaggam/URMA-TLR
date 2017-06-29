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

#include "test_ddp_firmware.h"
#include "test_ddp.h"
#include <stdlib.h>
#include <stdarg.h>
#include "ddp_device.h"
#include "tlr_print.h"
#include <sys/stat.h>

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

/************* Test case functions ****************/
static void test_get_fw_status_case(ddp_firmware_update_status_t status, unsigned int progress, char const * const result_str)
{
    char * result;
    char const format[] = "<state><firmware_status><status>%d</status><progress>%u</progress><result/></firmware_status></state>";
    char const format_result[] = "<state><firmware_status><status>%d</status><progress>%u</progress><result>%s</result></firmware_status></state>";
    char * expected = NULL;

    ddp_firmware_update_info.status = status;
    ddp_firmware_update_info.progress = progress;
    ddp_firmware_update_info.result = result_str;

    result = ddp_get_firmware_status();

    if (result_str == NULL)
    {
        asprintf(&expected, format, status, progress);
    }
    else
    {
        asprintf(&expected, format_result, status, progress, result_str);
    }

    if (strcmp(result, expected) != 0)
    {
        printf("%s\n", result);
        printf("%s\n", expected);
    }

    CU_ASSERT_STRING_EQUAL(result, expected);
    free(result);
    free(expected);
}

static void test_ddp_get_firmware_status(void)
{
    test_get_fw_status_case(firmware_update_status_none, 0, NULL);
    test_get_fw_status_case(firmware_update_status_downloading, 25, NULL);
    test_get_fw_status_case(firmware_update_status_verifying, 0, NULL);
    test_get_fw_status_case(firmware_update_status_applying, 99, NULL);
    test_get_fw_status_case(firmware_update_status_complete, 100, "success");
    test_get_fw_status_case(firmware_update_status_rebooting, 0, NULL);
    test_get_fw_status_case(firmware_update_status_error, 0, "expected error");
}

static void test_ddp_get_firmware_files(void)
{
    char * result;

    ddp_test_info.error = -1;
    result = ddp_get_firmware_files();
    CU_ASSERT_STRING_EQUAL(result, "<state><firmware_files/></state>");
    free(result);

    ddp_test_info.error = 0;
    result = ddp_get_firmware_files();
    CU_ASSERT_STRING_EQUAL(result, "<state><firmware_files><available_images>&lt;file index=&quot;1&quot;&gt;&lt;name&gt; lr54-0.1.0.1.bin&lt;/name&gt;&lt;size&gt;23456780&lt;/size&gt;&lt;date&gt;3/14/2016 9:45AM&lt;/date&gt;&lt;version&gt;0.1.0.1&lt;/version&gt;&lt;/file&gt;</available_images></firmware_files></state>");
    free(result);
}

static void test_ddp_update_firmware(void)
{
    bool result;
    bool remote = false;

    ddp_test_info.sys_command = "action_update_firmware -u cunit /tmp/firmware.bin";
    result = ddp_update_firmware("cunit", "/tmp/firmware.bin", remote);
    CU_ASSERT_TRUE(result);

    remote = true;
    ddp_test_info.sys_command = "fw_update_with_download.sh -n lr54-1.2.3.4.bin -a 0";
    result = ddp_update_firmware("cunit", "lr54-1.2.3.4.bin", remote);
    CU_ASSERT_TRUE(result);
}

static void test_ddp_reboot(void)
{
    bool result;

    ddp_test_info.sys_command = "action_reboot in 4";
    result = ddp_reboot(4);
    CU_ASSERT_TRUE(result);
}

/************* Test Runner Code goes here **************/
int add_ddp_firmware_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "ddp_firmware_test_suite", init_suite, clean_suite );
    if (pSuite == NULL) goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_ddp_get_firmware_files", test_ddp_get_firmware_files) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_get_firmware_status", test_ddp_get_firmware_status) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_update_firmware", test_ddp_update_firmware) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_reboot", test_ddp_reboot) == NULL) goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/** stubs **/
char const * tlr_get_remote_firmware_filenames_as_xml(void)
{
    char const * result = NULL;

    if (ddp_test_info.error == 0)
    {
        result = strdup("<file index=\"1\"><name> lr54-0.1.0.1.bin</name><size>23456780</size><date>3/14/2016 9:45AM</date><version>0.1.0.1</version></file>");
    }

    return result;
}

int stat(const char *path, struct stat *buf)
{
    CU_ASSERT_STRING_EQUAL(path, "/tmp/fw/status.log");
    CU_ASSERT_PTR_NOT_NULL(buf);
    return 0;
}

int system_command_with_response(char ** response, size_t * const resp_len, char const * const format, ...)
{
    int bytes;

    CU_ASSERT_PTR_NOT_NULL(response);
    CU_ASSERT_PTR_NOT_NULL(resp_len);
    CU_ASSERT_PTR_NOT_NULL(format);

    bytes = asprintf(response, "%d", ddp_firmware_update_info.progress);
    *resp_len = bytes;

    return bytes;
}
