
#define USE_BASIC_STYLE         1
#define USE_AUTOMATED_STYLE     0
#define USE_CONSOLE_STYLE       0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include "password_gen.h"

// And other functions will also unit test.
int validate_mac_addr(char const * const mac_addr);
int validate_serial_number(char const * const serial_number);

static void validate_mac_addr_test(void)
{
    CU_ASSERT_EQUAL(validate_mac_addr("012345678901"), 0);		// All digits
    CU_ASSERT_EQUAL(validate_mac_addr("ABCDEFABCDEF"), 0);		// All uppercase
    CU_ASSERT_EQUAL(validate_mac_addr("00ABCDEF0123"), 0);		// Mix of digits and uppercase

    CU_ASSERT_EQUAL(validate_mac_addr("01234567890"), -1);		// Too short
    CU_ASSERT_EQUAL(validate_mac_addr("0123456789012"), -1);	// Too long
    CU_ASSERT_EQUAL(validate_mac_addr("0123456789d2"), -1);		// Lowercase
    CU_ASSERT_EQUAL(validate_mac_addr("012345679D2R"), -1);		// Non hex digi
}

static void validate_serial_number_test(void)
{
    CU_ASSERT_EQUAL(validate_serial_number("LR123456"), 0);
    CU_ASSERT_EQUAL(validate_serial_number("DT001123"), 0);

    CU_ASSERT_EQUAL(validate_serial_number("LR12345"), -1);		// Too short
    CU_ASSERT_EQUAL(validate_serial_number("LR1234567"), -1);	// Too long
    CU_ASSERT_EQUAL(validate_serial_number("Lr123456"), -1);	// Lowercase
    CU_ASSERT_EQUAL(validate_serial_number("lr123456"), -1);	// Lowercase
    CU_ASSERT_EQUAL(validate_serial_number("LRR23456"), -1);	// Invalid format
    CU_ASSERT_EQUAL(validate_serial_number("LR1234f6"), -1);	// Invalid format
}

static void generate_default_password_test(void)
{
	char password[PASSWORD_GEN_PASSWORD_LEN];

	CU_ASSERT_EQUAL(generate_default_password("001122334455", "LR123456", password, sizeof password), 0);
	CU_ASSERT_STRING_EQUAL(password, "S1TEAK5Q9V");

	CU_ASSERT_EQUAL(generate_default_password("001D36FE9001", "LR000034", password, sizeof password), 0);
	CU_ASSERT_STRING_EQUAL(password, "4XWDYXY41S");

	CU_ASSERT_EQUAL(generate_default_password("00112233445", "LR123456", password, sizeof password), -1);	// Invalid MAC address
	CU_ASSERT_EQUAL(generate_default_password("001122334455", "LR12345", password, sizeof password), -1);	// Invalid serial number

}

int add_unit_tests(CU_pSuite pSuite)
{
    if ((CU_add_test(pSuite, "validate_mac_addr", validate_mac_addr_test) == NULL))
        return CU_get_error();

    if ((CU_add_test(pSuite, "validate_serial_number", validate_serial_number_test) == NULL))
        return CU_get_error();

    if ((CU_add_test(pSuite, "generate_default_password", generate_default_password_test) == NULL))
        return CU_get_error();

    return CUE_SUCCESS;
}

int init_suite(void)
{
	return 0;
}

int clean_suite(void)
{
	return 0;
}

int main (int argc, char *argv[])
{
	CU_pSuite pSuite = NULL;

	/* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    if (((pSuite = CU_add_suite("password_generator", init_suite, clean_suite)) == NULL) ||
		(add_unit_tests(pSuite) != CUE_SUCCESS))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    /* Clean up registry and return */
    CU_cleanup_registry();
    return CU_get_error();
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

