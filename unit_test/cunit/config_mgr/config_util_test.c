
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
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "config_table.h"
#include "config_mgr.h"
#include "uci_interface.h"

config_mgr_element_type_t element_type;
char * element_value_string;
uint32_t element_value_uint32;
int32_t element_value_int32;
char * element_value_enum_multi;
char * element_value_enum;
char * element_value_on_off = "on";
char * element_value_boolean = "true";
config_status_t config_status;


char const * element_option_value = "";

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }


/************* Test case functions ****************/

extern config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void ** value,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access);

void test_validate_type()
{
    config_mgr_group_table_t const * const table =  tlr_get_table(config_mgr_table_type_config);
    size_t group_id;

    for (group_id =0; group_id < table->count; group_id++)
    {
        config_mgr_group_t const * const group = &table->groups[group_id];
        size_t element_id;

        for (element_id = 0; element_id < group->elements.count; element_id++)
        {
            config_status_t status;
            void * ptr = NULL;
            config_mgr_element_t const * const element = &group->elements.element[element_id];

            config_status = config_status_success;
            element_option_value = (element->default_value != NULL) ? element->default_value : "";

            //printf("%s->%s = %s type %d\n",  group->name, element->name, element_option_value, element->type);

            status = tlr_get_element_alloc_value(group_id,
                                              1,
                                              element_id,
                                              &ptr,
                                              element->type,
                                              config_mgr_access_level_super);

            if (status != config_status_success)
                printf("%s->%s = \"%s\" status = %d\n", group->name, element->name, element_option_value, status);

            CU_ASSERT_FALSE(status);
            CU_ASSERT_PTR_NOT_NULL(ptr);

            free(ptr);

        }
    }
}

void test_password_no_access()
{
    config_mgr_group_table_t const * const table =  tlr_get_table(config_mgr_table_type_config);
    size_t group_id;

    for (group_id =0; group_id < table->count; group_id++)
    {
        config_mgr_group_t const * const group = &table->groups[group_id];
        size_t element_id;

        for (element_id = 0; element_id < group->elements.count; element_id++)
        {
            config_status_t status;
            void * ptr = NULL;
            config_mgr_element_t const * const element = &group->elements.element[element_id];

            config_status = config_status_success;
            element_option_value = (element->default_value != NULL) ? element->default_value : "";

            //printf("%zd/%zd: %s->%s = %s\n", element_id+1, group->elements.count, group->name, element->name, element_option_value);

            if ((element->type == config_mgr_element_type_password)
                || (element->type == config_mgr_element_type_encpassword))
            {
                /* expect error  no access */
                status = tlr_get_element_alloc_value(group_id,
                                                  1,
                                                  element_id,
                                                  &ptr,
                                                  config_mgr_element_type_string,
                                                  config_mgr_access_level_read_write);

                CU_ASSERT_NOT_EQUAL(status, 0);
                free(ptr);

                ptr = NULL;
                status = tlr_get_element_alloc_value(group_id,
                                                  1,
                                                  element_id,
                                                  &ptr,
                                                  config_mgr_element_type_string,
                                                  config_mgr_access_level_read_only);

                CU_ASSERT_NOT_EQUAL(status, 0);
                free(ptr);

            }
        }
    }
}

void test_arguments()
{
    config_mgr_group_table_t const * const table =  tlr_get_table(config_mgr_table_type_config);
    size_t group_id;
    void * ptr = NULL;
    config_status_t status;

    config_status = config_status_no_memory;

    /* expect error */
    status = tlr_get_element_alloc_value(1,
                                      1,
                                      1,
                                      &ptr,
                                      config_mgr_element_type_string,
                                      config_mgr_access_level_super);

    CU_ASSERT_EQUAL(status, config_status);
    CU_ASSERT_PTR_NULL(ptr);

    free(ptr);
    ptr = NULL;

    config_status = config_status_success;

    /* expect ok */
    status = tlr_get_element_alloc_value(1,
                                      1,
                                      1,
                                      &ptr,
                                      config_mgr_element_type_string,
                                      config_mgr_access_level_super);

    CU_ASSERT_FALSE(status);
    CU_ASSERT_PTR_NOT_NULL(ptr);
    free(ptr);
    ptr = NULL;


    /* expect error invalid group id */
    status = tlr_get_element_alloc_value(table->count,
                                      1,
                                      1,
                                      &ptr,
                                      config_mgr_element_type_string,
                                      config_mgr_access_level_super);

    CU_ASSERT_NOT_EQUAL(status, 0);
    free(ptr);
    ptr = NULL;

    for (group_id =0; group_id < table->count; group_id++)
    {
        config_mgr_group_t const * const group = &table->groups[group_id];

        {

            config_status = config_status_success;

            /* expect error invalid type */
             status = tlr_get_element_alloc_value(group_id,
                                               1,
                                               0,
                                               &ptr,
                                               config_mgr_element_type_none,
                                               config_mgr_access_level_super);

             CU_ASSERT_NOT_EQUAL(status, 0);

            /* expect error invalid element id */
            status = tlr_get_element_alloc_value(group_id,
                                              1,
                                              group->elements.count,
                                              &ptr,
                                              config_mgr_element_type_string,
                                              config_mgr_access_level_super);

            CU_ASSERT_NOT_EQUAL(status, 0);

            /* expect error invalid instance */
            status = tlr_get_element_alloc_value(group_id,
                                              group->instances+1,
                                              1,
                                              &ptr,
                                              config_mgr_element_type_string,
                                              config_mgr_access_level_super);

            CU_ASSERT_NOT_EQUAL(status, 0);

            /* expect error invalid instance */
            status = tlr_get_element_alloc_value(group_id,
                                              group->instances+1,
                                              1,
                                              NULL,
                                              config_mgr_element_type_string,
                                              config_mgr_access_level_super);

            CU_ASSERT_NOT_EQUAL(status, 0);

        }
    }
}

/************* Test Runner Code goes here **************/

int main(void)
{
    CU_pSuite pSuite = NULL;
    unsigned int number_of_failures;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("ConfigUtil_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
        || CU_add_test(pSuite, "test_validate_type", test_validate_type) == NULL
        || CU_add_test(pSuite, "test_password_no_access", test_password_no_access) == NULL
        || CU_add_test(pSuite, "test_arguments", test_arguments) == NULL
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

    /* Clean up registry and return */
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

	// Ensure we return an error if any tests failed.
    if (number_of_failures)
    	return -1;

    return CU_get_error();
}



/****************  Stub Simulations ******************/

config_status_t uci_config_get_option(void * const handle, uci_config_request_t * const request)
{
    config_status_t status = config_status;

    printf("element_option_value = %p\n", element_option_value);
    request->value = element_option_value;

    return status;
}

config_status_t uci_config_set_option(void * const handle, uci_config_request_t const * const request, bool const is_list)
{
    config_status_t status = config_status;
    element_option_value = request->value;
    return status;
}
/****************  Stubs   ***************************/
size_t strlcpy(char *dst, const char *src, size_t size)
{
    size_t    srclen;       /* Length of source string */


    /*
    * Figure out how much room is needed...
    */

    size --;

    srclen = strlen(src);

    /*
    * Copy the appropriate amount...
    */

    if (srclen > size)
        srclen = size;

    memcpy(dst, src, srclen);
    dst[srclen] = '\0';

    return srclen;
}

int uci_config_handle;

void * uci_config_alloc(void)
{
    return &uci_config_handle;
}


void uci_config_free(void * const handle)
{

}
config_status_t set_password(void * const handle, config_mgr_element_type_t const type, uci_config_request_t * const request)
{
    return config_status_success;
}
int get_tlr_platform_name(char const * * model_name_ptr)
{
    static char const model_name[] = "LR54DWC1";
    *model_name_ptr = model_name;
    return 0;
}


bool get_ifr(char const * const ifname, int request, void * ifr)
{
    return false;
}

int get_tlr_serial_number(char * * serial_number_ptr)
{
    return -1;
}


int generate_default_password(char const * const mac_addr, char const * const serial_number, char * const password)
{
    return -1;
}

