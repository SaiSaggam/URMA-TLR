
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
#include <config_mgr.h>
#include "uci_interface.h"

#define NB_ELEMENTS(array)  (sizeof((array))/sizeof((array[0])))

void simu_fail_open();
void simu_pass_open();
void simu_fail_read();
void simu_pass_read(char *buf, int len);

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }


/************* Test case functions ****************/
#if defined(HAS_STRING_VALIDATION_FUNCTION)
static void validate_string_test()
{
    char * string_to_validate;
    size_t min_length, max_length;
    bool isvalid;

    string_to_validate = "123";

    /* input string length is within max and min values */
    /* setup */

    min_length = strlen(string_to_validate) - 1;
    max_length = strlen(string_to_validate) + 1;

    /* perform test */
    isvalid = validate_string(string_to_validate, min_length, max_length);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* input string length matches min values */
    /* setup */
    min_length = strlen(string_to_validate);
    max_length = strlen(string_to_validate) + 1;

    /* perform test */
    isvalid = validate_string(string_to_validate, min_length, max_length);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* input string length matches max values */
    min_length = strlen(string_to_validate);
    max_length = strlen(string_to_validate);

    /* perform test */
    isvalid = validate_string(string_to_validate, min_length, max_length);

    /* check result */
    CU_ASSERT_TRUE_FATAL(isvalid);

    /* input string length less than min values */
    /* setup */
    min_length = strlen(string_to_validate) + 1;
    max_length = strlen(string_to_validate);

    /* perform test */
    isvalid = validate_string(string_to_validate, min_length, max_length);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

    /* input string length greater than max value */
    /* setup */
    min_length = strlen(string_to_validate);
    max_length = strlen(string_to_validate) - 1;

    /* perform test */
    isvalid = validate_string(string_to_validate, min_length, max_length);

    /* check result */
    CU_ASSERT_FALSE(isvalid);
}
#endif

#if defined(HAS_UINT32_VALIDATION_FUNCTION)
static void validate_uint32_test()
{
    uint32_t value_to_validate;
    uint32_t min_value, max_value;
    bool isvalid;

    value_to_validate = 1;

    /* input value is within max and min values */
    /* setup */
    min_value = value_to_validate - 1;
    max_value = value_to_validate + 1;

    /* perform test */
    isvalid = validate_uint32(value_to_validate, min_value, max_value);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* input value matches min value */
    /* setup */
    min_value = value_to_validate;
    max_value = value_to_validate + 1;

    /* perform test */
    isvalid = validate_uint32(value_to_validate, min_value, max_value);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* input value matches max value */
    min_value = value_to_validate;
    max_value = value_to_validate;

    /* perform test */
    isvalid = validate_uint32(value_to_validate, min_value, max_value);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* input string length less than min values */
    /* setup */
    min_value = value_to_validate + 1;
    max_value = value_to_validate;

    /* perform test */
    isvalid = validate_uint32(value_to_validate, min_value, max_value);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

    /* input value greater than max value */
    /* setup */
    min_value = value_to_validate;
    max_value = value_to_validate - 1;

    /* perform test */
    isvalid = validate_uint32(value_to_validate, min_value, max_value);

    /* check result */
    CU_ASSERT_FALSE(isvalid);
}
#endif

#if defined(HAS_ENUM_VALIDATION_FUNCTION)
static void validate_enum_test()
{
    char const * string_to_validate;
    char const * const first_enum_value = "enum0";
    char const * const second_enum_value = "enum1";
    char const * const last_enum_value = "enum2";
    char const * const invalid_enum_value = "invalidvalue";
    config_mgr_element_enum_t values[] = {
        [0] = { .name = first_enum_value },
        [1] = { .name = second_enum_value },
        [2] = { .name = last_enum_value }
    };
    config_mgr_element_enum_limit_t enum_limit = {
        .count = NB_ELEMENTS(values),
        .value = values
    };
    bool isvalid;

    // XXX test assumes that there are at least two enum values

    /* string_to_validate matches first enum */
    /* setup */
    string_to_validate = first_enum_value;

    /* perform test */
    isvalid = validate_enum(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate matches last enum */
    /* setup */
    string_to_validate = last_enum_value;

    /* perform test */
    isvalid = validate_enum(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate matches enum that isn't first or last */
    /* setup */
    string_to_validate = second_enum_value;

    /* perform test */
    isvalid = validate_enum(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate doesn't match */
    /* setup */
    string_to_validate = invalid_enum_value;

    /* perform test */
    isvalid = validate_enum(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

}
#endif

#if defined(HAS_ON_OFF_VALIDATION_FUNCTION)
static void validate_on_off_test()
{
    char const * string_to_validate;
    char const * const on_string = "on";
    char const * const off_string = "off";
    char const * const invalid_string = "blah";
    bool isvalid;

    /* string_to_validate matches on string */
    /* setup */
    string_to_validate = on_string;

    /* perform test */
    isvalid = validate_on_off(string_to_validate);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate matches off string */
    /* setup */
    string_to_validate = off_string;

    /* perform test */
    isvalid = validate_on_off(string_to_validate);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate is invalid */
    /* setup */
    string_to_validate = invalid_string;

    /* perform test */
    isvalid = validate_on_off(string_to_validate);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

    /* string_to_validate is zero length */
    /* setup */
    string_to_validate = "";

    /* perform test */
    isvalid = validate_on_off(string_to_validate);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

}
#endif

#if defined(HAS_BOOLEAN_VALIDATION_FUNCTION)
static void validate_boolean_test()
{
    char const * string_to_validate;
    char const * const true_string = "true";
    char const * const false_string = "false";
    char const * const invalid_string = "blah";
    bool isvalid;

    /* string_to_validate matches on string */
    /* setup */
    string_to_validate = true_string;

    /* perform test */
    isvalid = validate_boolean(string_to_validate);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate matches off string */
    /* setup */
    string_to_validate = false_string;

    /* perform test */
    isvalid = validate_boolean(string_to_validate);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate is invalid */
    /* setup */
    string_to_validate = invalid_string;

    /* perform test */
    isvalid = validate_boolean(string_to_validate);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

    /* string_to_validate is zero length */
    /* setup */
    string_to_validate = "";

    /* perform test */
    isvalid = validate_boolean(string_to_validate);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

}
#endif

#if defined(HAS_INT32_VALIDATION_FUNCTION)
static void validate_int32_test()
{
    int32_t value_to_validate;
    int32_t min_value, max_value;
    bool isvalid;

    /* validate -ve, 0 and +ve numbers */
    for (value_to_validate = -1; value_to_validate < 2; value_to_validate++)
    {
        /* input value is within max and min values */
        /* setup */
        min_value = value_to_validate - 1;
        max_value = value_to_validate + 1;

        /* perform test */
        isvalid = validate_int32(value_to_validate, min_value, max_value);

        /* check result */
        CU_ASSERT_TRUE(isvalid);

        /* input value matches min value */
        /* setup */
        min_value = value_to_validate;
        max_value = value_to_validate + 1;

        /* perform test */
        isvalid = validate_int32(value_to_validate, min_value, max_value);

        /* check result */
        CU_ASSERT_TRUE(isvalid);

        /* input value matches max value */
        min_value = value_to_validate;
        max_value = value_to_validate;

        /* perform test */
        isvalid = validate_int32(value_to_validate, min_value, max_value);

        /* check result */
        CU_ASSERT_TRUE(isvalid);

        /* input string length less than min values */
        /* setup */
        min_value = value_to_validate + 1;
        max_value = value_to_validate;

        /* perform test */
        isvalid = validate_int32(value_to_validate, min_value, max_value);

        /* check result */
        CU_ASSERT_FALSE(isvalid);

        /* input value greater than max value */
        /* setup */
        min_value = value_to_validate;
        max_value = value_to_validate - 1;

        /* perform test */
        isvalid = validate_int32(value_to_validate, min_value, max_value);

        /* check result */
        CU_ASSERT_FALSE(isvalid);
    }
}
#endif

#if defined(HAS_IPADDR_VALIDATION_FUNCTION)
static void validate_ipaddr_test()
{
    typedef struct ipaddr_test_st
    {
        char const * const value_to_validate;
        bool expected_result;
    } ipaddr_test_st;
    ipaddr_test_st values_to_validate[] =
    {
        { .value_to_validate = "0.0.0.0", .expected_result = true },
        { .value_to_validate = "00.00.00.00", .expected_result = false },
        { .value_to_validate = "000.000.000.000", .expected_result = false },
        { .value_to_validate = "192.168.0.1", .expected_result = true },
        { .value_to_validate = "192.168.0.256", .expected_result = false },
        { .value_to_validate = "192.168.256.0", .expected_result = false },
        { .value_to_validate = "192.256.0.1", .expected_result = false },
        { .value_to_validate = "256.168.0.1", .expected_result = false },
        { .value_to_validate = "invalid", .expected_result = false },
        { .value_to_validate = "", .expected_result = true },           /* empty string considered to be valid */
        { .value_to_validate = "0", .expected_result = false },
        { .value_to_validate = "3232235521", .expected_result = false },
        { .value_to_validate = "192.168.0", .expected_result = false },
        { .value_to_validate = "192.168", .expected_result = false },
        { .value_to_validate = "192,168,0,1", .expected_result = false }
    };
    int test_index;

    for (test_index = 0; test_index < NB_ELEMENTS(values_to_validate); test_index++)
    {
        bool isvalid;
        /* setup */
        ipaddr_test_st * test = &values_to_validate[test_index];

        /* perform test */
        isvalid = validate_ipaddr(test->value_to_validate);

        /* check result */
        CU_ASSERT(isvalid == test->expected_result);

    }
}
#endif

#if defined(HAS_MAC_ADDR_VALIDATION_FUNCTION)
static void validate_mac_addr_test()
{
    typedef struct mac_addr_test_st
    {
        char const * const value_to_validate;
        bool expected_result;
    } mac_addr_test_st;
    mac_addr_test_st values_to_validate[] =
    {
        { .value_to_validate = "00-00-00-00-00-00",     .expected_result = true },      /* using '-' as separator */
        { .value_to_validate = "f0-00-00-00-00-00",     .expected_result = true },
        { .value_to_validate = "01-02-03-04-05-06",     .expected_result = true },
        { .value_to_validate = "g0-00-00-00-00-00",     .expected_result = false },     /* characters must be 0-9, a-f, A-F */
        { .value_to_validate = "00-00-00-00-00",        .expected_result = false },     /* must be 6 pairs of values */
        { .value_to_validate = "00-00-00-00-00-00-00",  .expected_result = false },     /* must be 6 pairs of values */
        { .value_to_validate = "00:00:00:00:00:00",     .expected_result = true },      /* using ':' as separator */
        { .value_to_validate = "00-00:00:00:00:00",     .expected_result = true },      /* mixed ':' and '-' is OK too */
        { .value_to_validate = "aa:BB:cc:DD:ee:FF",     .expected_result = true },      /* lower and upper case is OK */
        { .value_to_validate = "aaBBccDDeeFF",          .expected_result = true },      /* allow the case where no separators are used */
        { .value_to_validate = "aaBB:cc:DD:ee:FF",      .expected_result = false },     /* if using separators, 5 must be used */
        { .value_to_validate = "",                      .expected_result = false }      /* empty string is invalid (XXX should this actually be valid?) */
    };
    int test_index;

    for (test_index = 0; test_index < NB_ELEMENTS(values_to_validate); test_index++)
    {
        bool isvalid;
        /* setup */
        mac_addr_test_st * test = &values_to_validate[test_index];

        /* perform test */
        isvalid = validate_mac_addr(test->value_to_validate);

        /* check result */
        CU_ASSERT(isvalid == test->expected_result);
	}
}
#endif

#if defined(HAS_FQDN_VALIDATION_FUNCTION)
static void validate_fqdn_test()
{
    typedef struct fqdn_test_st
    {
        char const * const value_to_validate;
        bool expected_result;
    } fqdn_test_st;
    char str_len_ok[256];
    char str_too_long[257];
    fqdn_test_st values_to_validate[] =
    {
    	// Currently, anything goes for FQDN parameters.
        { .value_to_validate = "0.0.0.0", .expected_result = true },
        { .value_to_validate = "192.168.0.1", .expected_result = true },
        { .value_to_validate = "", .expected_result = true },           /* empty string considered to be valid */
        { .value_to_validate = "a", .expected_result = true },
        { .value_to_validate = "a.", .expected_result = true },
        { .value_to_validate = "a.b", .expected_result = true },
        { .value_to_validate = "a.b.", .expected_result = true },
        { .value_to_validate = "a.b.c", .expected_result = true },
        { .value_to_validate = "a.b.c.", .expected_result = true },
        { .value_to_validate = "a.b.c.d.e.f", .expected_result = true },
        { .value_to_validate = "aaa.bbb.ccc.ddd", .expected_result = true },
        { .value_to_validate = str_len_ok, .expected_result = true },
        { .value_to_validate = str_too_long, .expected_result = false },
    };
    int test_index;

    memset(str_len_ok, 'a', sizeof(str_len_ok)); str_len_ok[sizeof(str_len_ok)-1] = 0;
    memset(str_too_long, 'b', sizeof(str_too_long)); str_too_long[sizeof(str_too_long)-1] = 0;

    for (test_index = 0; test_index < NB_ELEMENTS(values_to_validate); test_index++)
    {
        bool isvalid;
        /* setup */
        fqdn_test_st * test = &values_to_validate[test_index];

        /* perform test */
        isvalid = validate_fqdn(test->value_to_validate);

        /* check result */
        CU_ASSERT(isvalid == test->expected_result);

    }
}
#endif

#if defined(HAS_ENUM_MULTI_VALIDATION_FUNCTION)
static void validate_enum_multi_test()
{
    char const * string_to_validate;
    char const * const first_enum_value = "enum0";
    char const * const second_enum_value = "enum1";
    char const * const last_enum_value = "enum2";
    char const * const invalid_enum_value_1 = "invalid";
    char const * const invalid_enum_value_2 = "enum0,invalid,enum1";
    char const * const invalid_enum_value_3 = "enum0,enum1,invalid";
    char const * const invalid_enum_value_4 = "invalid,enum0,enum1";

    char const * const valid_enum_value_1 = "enum1,enum2";
    char const * const valid_enum_value_2 = "enum1,,enum1,  enum2,  ";

    config_mgr_element_enum_t values[] = {
        [0] = { .name = first_enum_value },
        [1] = { .name = second_enum_value },
        [2] = { .name = last_enum_value }
    };
    config_mgr_element_enum_limit_t enum_limit = {
        .count = NB_ELEMENTS(values),
        .value = values
    };
    bool isvalid;

    // XXX test assumes that there are at least two enum values

    /* string_to_validate matches first enum */
    /* setup */
    string_to_validate = first_enum_value;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate matches last enum */
    /* setup */
    string_to_validate = last_enum_value;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate matches enum that isn't first or last */
    /* setup */
    string_to_validate = second_enum_value;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate doesn't match */
    /* setup */
    string_to_validate = valid_enum_value_1;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);

    /* string_to_validate doesn't match */
    /* setup */
    string_to_validate = valid_enum_value_2;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_TRUE(isvalid);


    /* string_to_validate doesn't match */
    /* setup */
    string_to_validate = invalid_enum_value_1;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

    /* string_to_validate doesn't match */
    /* setup */
    string_to_validate = invalid_enum_value_2;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);
    printf("string: %s valid = %d\n", string_to_validate, isvalid);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

    /* string_to_validate doesn't match */
    /* setup */
    string_to_validate = invalid_enum_value_3;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

    /* string_to_validate doesn't match */
    /* setup */
    string_to_validate = invalid_enum_value_4;

    /* perform test */
    isvalid = validate_enum_multi(string_to_validate, &enum_limit);

    /* check result */
    CU_ASSERT_FALSE(isvalid);

}
#endif

void test_fail_openfile()
{
	char 	file[80];

	simu_fail_open();
	tlr_get_config_file(file, sizeof file);
	CU_ASSERT_STRING_EQUAL(file, "/home/digi/user/config.da0" );
}

void test_std_ok()
{
	char 	file[80];
	char 	*text = "anotherconfig.da0";

	simu_pass_open();
	simu_pass_read(text, strlen(text)+1);

	tlr_get_config_file(file, sizeof file);
	CU_ASSERT_STRING_EQUAL(file, "/home/digi/user/anotherconfig.da0" );
}

void test_std_missingnull()
{
	char 	file[80];
	char 	*text = "anotherconfig.da0";

	simu_pass_open();
	simu_pass_read(text, strlen(text));

	tlr_get_config_file(file, sizeof file);
	CU_ASSERT_STRING_EQUAL(file, "/home/digi/user/anotherconfig.da0" );
}

void test_fail_read()
{
	char 	file[80];

	simu_pass_open();
	simu_fail_read();
	tlr_get_config_file(file, sizeof file);
	CU_ASSERT_STRING_EQUAL(file, "/home/digi/user/config.da0" );
}

void test_no_overflow()
{
	char 	file[80];
	char 	*text = "anotherconfig.da0";

	simu_pass_open();
	simu_pass_read(text, strlen(text));

	strcpy ( file, "helloworld" );
	tlr_get_config_file(file, 4);
	CU_ASSERT_STRING_EQUAL(file, "/ho" );
	CU_ASSERT_STRING_EQUAL(&file[4], "oworld" );

}


void test_white_space_at_end()
{
	char 	file[80];
	char 	*text = "anotherconfig.da0    ";

	simu_pass_open();
	simu_pass_read(text, strlen(text)+1);
	tlr_get_config_file(file, sizeof file);
	CU_ASSERT_STRING_EQUAL(file, "/home/digi/user/anotherconfig.da0" );
}


void test_white_space_at_front()
{
	char 	file[80];
	char 	*text = "    anotherconfig.da0";

	simu_pass_open();
	simu_pass_read(text, strlen(text)+1);
	tlr_get_config_file(file, sizeof file);
	CU_ASSERT_STRING_EQUAL(file, "/home/digi/user/anotherconfig.da0" );
}


void test_white_space_at_front_andend()
{
	char 	file[80];
	char 	*text = "    anotherconfig.da0    ";

	simu_pass_open();
	simu_pass_read(text, strlen(text)+1);
	tlr_get_config_file(file, sizeof file);
	CU_ASSERT_STRING_EQUAL(file, "/home/digi/user/anotherconfig.da0" );
}

static void config_filename_test()
{
	test_fail_openfile();
	test_std_ok();
	test_std_missingnull();
	test_fail_read();
	test_no_overflow();
	test_white_space_at_end();
	test_white_space_at_front();
	test_white_space_at_front_andend();
}

/************* Test Runner Code goes here **************/

int main(void)
{
    CU_pSuite pSuite = NULL;
    unsigned int number_of_failures;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("ConfigValid_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
#if defined(HAS_STRING_VALIDATION_FUNCTION)
         || CU_add_test(pSuite, "validate_string", validate_string_test) == NULL
#endif
#if defined(HAS_UINT32_VALIDATION_FUNCTION)
        || CU_add_test(pSuite, "validate_uint32", validate_uint32_test) == NULL
#endif
#if defined(HAS_ENUM_VALIDATION_FUNCTION)
        || CU_add_test(pSuite, "validate_enum", validate_enum_test) == NULL
#endif
#if defined(HAS_ON_OFF_VALIDATION_FUNCTION)
         || CU_add_test(pSuite, "validate_on_off", validate_on_off_test) == NULL
#endif
#if defined(HAS_BOOLEAN_VALIDATION_FUNCTION)
         || CU_add_test(pSuite, "validate_boolean", validate_boolean_test) == NULL
#endif
#if defined(HAS_INT32_VALIDATION_FUNCTION)
         || CU_add_test(pSuite, "validate_int32", validate_int32_test) == NULL
#endif
#if defined(HAS_IPADDR_VALIDATION_FUNCTION)
         || CU_add_test(pSuite, "validate_ipaddr", validate_ipaddr_test) == NULL
#endif
#if defined(HAS_MAC_ADDR_VALIDATION_FUNCTION)
         || CU_add_test(pSuite, "validate_mac_addr", validate_mac_addr_test) == NULL
#endif
#if defined(HAS_FQDN_VALIDATION_FUNCTION)
         || CU_add_test(pSuite, "validate_fqdn", validate_fqdn_test) == NULL
#endif
#if defined(HAS_ENUM_MULTI_VALIDATION_FUNCTION)
        || CU_add_test(pSuite, "validate_enum_multi", validate_enum_multi_test) == NULL
#endif
 	 || CU_add_test(pSuite, "config_filename", config_filename_test) == NULL
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

    // return CU_get_error();  //@TODO: looks like there are some open thread or process is still running. The test was not breaking.
    _exit(CU_get_error());
}

/****************  Stub Simulations ******************/
static int open_return = -1;
static int read_return = -1;
static char *read_data = NULL;

void simu_fail_open()
{
	open_return = -1;
}

void simu_pass_open()
{
	open_return = 1;
}


void simu_fail_read()
{
	read_return = -1;
}

void simu_pass_read(char *buf, int len)
{
	read_return = len;
	if ( read_data != NULL )
		free(read_data);
	read_data = malloc(len);
	if ( read_data != NULL )
		memcpy (read_data, buf, len);
}


/****************  Stubs   ***************************/

int open (const char * file, int oflag, ...)
{
	return open_return;
}

ssize_t read (int fd, void * tobuf, size_t maxlen)
{
	int len;

	if (read_return == -1 )
		return -1;
	if ( read_return > maxlen )
		len = maxlen;
	else
		len = read_return;
	if ( read_data != NULL )
		{
		memcpy(tobuf, read_data, len );
		free(read_data);
		read_data = NULL;
		}
	return len;
}

int close(int fd)
{
    return 0;
}

config_status_t set_password(void * const handle, config_mgr_element_type_t const type, uci_config_request_t * const request)
{
    return config_status_success;
}

int get_tlr_platform_name(char const * * model_name_ptr)
{
    static char const model_name[] = "LR54W";
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
