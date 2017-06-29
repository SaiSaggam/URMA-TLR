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

#include "test_ddp.h"
#include <stdlib.h>
#include <stdarg.h>
#include "ddp_device.h"
#include "test_ddp_tlr.h"
#include "config_mgr.h"

#include <libxml/parser.h>

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

/************* Test case functions ****************/
static void test_ddp_get_group_count(void)
{
    bool const is_setting = true;
    size_t expected = config_mgr_group_table[config_mgr_set_group].count;

    size_t count = ddp_get_group_count(is_setting);
    CU_ASSERT_EQUAL(count, expected);

    expected = config_mgr_group_table[config_mgr_get_group].count;
    count = ddp_get_group_count(!is_setting);
    CU_ASSERT_EQUAL(count, expected);
}

static void test_ddp_get_group_instance(void)
{
    bool const is_setting = true;
    size_t const group_id = 2;
    size_t expected = config_mgr_group_table[config_mgr_set_group].groups[group_id].instances;

    size_t count = ddp_get_group_instance(group_id, is_setting);
    CU_ASSERT_EQUAL(count, expected);

    expected = config_mgr_group_table[config_mgr_get_group].groups[group_id].instances;
    count = ddp_get_group_instance(group_id, !is_setting);
    CU_ASSERT_EQUAL(count, expected);
}

static void test_ddp_get_group_id(void)
{
    bool const is_setting = true;
    size_t id;

    ddp_test_info.config.name = "eth_test";
    ddp_test_info.config.id = config_eth;
    ddp_test_info.status.name = "eth_status";
    ddp_test_info.status.id = config_state_eth;

    id = ddp_get_group_id(ddp_test_info.config.name, is_setting);
    CU_ASSERT_EQUAL(id, ddp_test_info.config.id);

    id = ddp_get_group_id(ddp_test_info.status.name, !is_setting);
    CU_ASSERT_EQUAL(id, ddp_test_info.status.id);
}

static void test_read_config_group(void)
{
    char * result;
    char * expected;
    bool const is_config = true;
    bool at_init = false;

    ddp_test_info.sys_command = NULL;
    ddp_test_info.config.name = "lan";
    ddp_test_info.config.id = config_lan;
    ddp_test_info.config.instance = 2;
    ddp_test_info.config.value = "config_value";

    result = ddp_read_group(ddp_test_info.config.id, ddp_test_info.config.instance, is_config, at_init);

    expected = strstr(result, "<settings><lan index=\"2\">");
    CU_ASSERT_PTR_NOT_NULL(expected);

    expected = strstr(result, "</lan></settings>");
    CU_ASSERT_PTR_NOT_NULL(expected);

    expected = strstr(result, "<mtu>1500</mtu>");
    CU_ASSERT_PTR_NOT_NULL(expected);
}

static void test_read_state_group(void)
{
    char * result;
    char * expected;
    bool const is_config = false;
    bool at_init = false;

    ddp_test_info.status.name = "system";
    ddp_test_info.status.id = config_state_system;
    ddp_test_info.status.instance = 1;
    ddp_test_info.status.value = "<\"waltham&>\'";

    result = ddp_read_group(ddp_test_info.status.id, ddp_test_info.status.instance, is_config, at_init);

    expected = strstr(result, "<state><system index=\"1\">");
    CU_ASSERT_PTR_NOT_NULL(expected);

    expected = strstr(result, "</system></state>");
    CU_ASSERT_PTR_NOT_NULL(expected);

    expected = strstr(result, "<location>&lt;&quot;waltham&amp;&gt;&apos;</location>");
    CU_ASSERT_PTR_NOT_NULL(expected);
}

static void test_read_state_group_at_init(void)
{
    char * result;
    char * expected;
    bool const is_config = false;
    bool at_init = true;

    result = ddp_read_group(ddp_test_info.status.id, ddp_test_info.status.instance, is_config, at_init);

    expected = strstr(result, "<state><system index=\"1\">");
    CU_ASSERT_PTR_NOT_NULL(expected);

    expected = strstr(result, "</system></state>");
    CU_ASSERT_PTR_NOT_NULL(expected);

    expected = strstr(result, "<location>&lt;&quot;waltham&amp;&gt;&apos;</location>");
    CU_ASSERT_PTR_NULL(expected);
}

static void test_ddp_read_group(void)
{
    test_read_config_group();
    test_read_state_group();
    test_read_state_group_at_init();
}

static void test_ddp_set_element(void)
{
    bool result;
    ddp_group_info_t group_info = {.group_name = "lan", .element_name = "mtu", .instance = 2, .value = "1499", .user = "test", .access_level = ddp_super_user};

    ddp_test_info.sys_command = NULL;
    ddp_test_info.config.id = config_lan;
    ddp_test_info.config.name = "lan";
    result = ddp_set_element(&group_info);
    CU_ASSERT_TRUE(result);
}

static void test_ddp_authenticate(void)
{
    ddp_access_t result;

    ddp_test_info.config.name = "username";
    ddp_test_info.config.value = "password";

    ddp_test_info.error = 0;
    result = ddp_authenticate(ddp_test_info.config.name, ddp_test_info.config.value);
    CU_ASSERT_EQUAL(result, ddp_super_user);

    ddp_test_info.error = -1;
    result = ddp_authenticate(ddp_test_info.config.name, ddp_test_info.config.value);
    CU_ASSERT_EQUAL(result, ddp_no_access);
}

static void test_ddp_apply_group(void)
{

}

static void test_ddp_save(void)
{
    bool result;

    result = ddp_save("cunit");
    CU_ASSERT_TRUE(result);
}


static void verify_xml_buffer(char * const buffer)
{
    xmlDocPtr xml_doc = xmlReadMemory(buffer, strlen(buffer), "temp.xml", NULL, 0);

    CU_ASSERT_PTR_NOT_NULL(xml_doc);

    xmlFreeDoc(xml_doc);
}

static void test_ddp_read_config_descriptor(void)
{
    char * xml_ptr;

    system("cp ../../../../tools/command_definition/descriptor_query_setting.xml ./");

    xml_ptr = ddp_read_config_descriptor();
    CU_ASSERT_PTR_NOT_NULL(xml_ptr);

    verify_xml_buffer(xml_ptr);

    xml_ptr = strstr(xml_ptr, "settings_descriptor");
    CU_ASSERT_PTR_NOT_NULL(xml_ptr);

    xml_ptr = strstr(xml_ptr, "/settings_descriptor");
    CU_ASSERT_PTR_NOT_NULL(xml_ptr);

    system("rm descriptor_query_setting.xml");
}

static void test_ddp_read_state_descriptor(void)
{
    char * xml_ptr;

    system("cp ../../../../tools/command_definition/descriptor_query_state.xml ./");
    xml_ptr = ddp_read_state_descriptor();
    CU_ASSERT_PTR_NOT_NULL(xml_ptr);

    verify_xml_buffer(xml_ptr);

    xml_ptr = strstr(xml_ptr, "state_descriptor");
    CU_ASSERT_PTR_NOT_NULL(xml_ptr);

    xml_ptr = strstr(xml_ptr, "/state_descriptor");
    CU_ASSERT_PTR_NOT_NULL(xml_ptr);

    system("rm descriptor_query_state.xml");
}

/************* Test Runner Code goes here **************/
int add_ddp_tlr_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "ddp_tlr_test_suite", init_suite, clean_suite );
    if (pSuite == NULL) goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_ddp_get_group_count", test_ddp_get_group_count) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_get_group_instance", test_ddp_get_group_instance) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_read_group", test_ddp_read_group) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_get_group_id", test_ddp_get_group_id) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_authenticate", test_ddp_authenticate) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_set_element", test_ddp_set_element) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_apply_group", test_ddp_apply_group) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_save", test_ddp_save) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_read_config_descriptor", test_ddp_read_config_descriptor) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_read_state_descriptor", test_ddp_read_state_descriptor) == NULL) goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}
