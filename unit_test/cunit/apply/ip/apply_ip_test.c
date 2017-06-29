
#define	USE_BASIC_STYLE		1
#define	USE_AUTOMATED_STYLE	0
#define	USE_CONSOLE_STYLE	0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

/********************
	Header Files
********************/

#include "apply_ip.h"
#include "config_mgr.h"
#include "config_def.h"

#include <stdlib.h>

/********************
	Function Signatures
********************/

config_status_t tlr_get_element_value_return_value;
void * tlr_get_element_value_value = NULL;

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    if (value != NULL && tlr_get_element_value_value != NULL)
        memcpy(value, tlr_get_element_value_value, bytes);

    return tlr_get_element_value_return_value;
}

int system_command_return_value;

int system_command(char const * const format, ...)
{
    return system_command_return_value;
}

size_t tlr_get_config_value_max_length(config_mgr_table_type_t const type, size_t const config_id, size_t const element_id)
{
    return 10;
}

int get_instance(int const argc, char * const argv[], size_t * const instance, const size_t group_id)
{
    return 0;
}
/********************
	Test Suite Setup
********************/

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/********************
	Test Case Functions
********************/

int ip_read_hostname(size_t const instance, char * const hostname, size_t const length);

void test_ip_read_hostname()
{
    int status;

    tlr_get_element_value_return_value = config_status_no_memory;

    status = ip_read_hostname(1, NULL, 0);
    CU_ASSERT_EQUAL(status, -1);

    {
        char const expect_hostname[] = "hostname";

        char hostname[sizeof expect_hostname];

        tlr_get_element_value_value = (void *)expect_hostname;
        tlr_get_element_value_return_value = config_status_success;

        status =  ip_read_hostname(1,  hostname, sizeof hostname);
        CU_ASSERT_EQUAL(status, 0);
        CU_ASSERT_STRING_EQUAL(hostname, tlr_get_element_value_value);
    }

}

#define HOSTS_DEFAULT_STR	"127.0.0.1\tlocalhost\n"

#define TEST_HOSTNAME		"test_hostname"

/* Create a hosts file under /tmp to test with */
static int create_hosts_files(void)
{
    FILE *fp = NULL;
    int ret = EXIT_FAILURE;
    char const * const hosts_filename = SYSTEM_HOSTS_FILENAME;


    /* Create "old" /etc/hosts file */
    if ((fp = fopen(hosts_filename, "w")) == NULL)
    {
        fprintf(stderr, "fopen '%s' failed\n", hosts_filename);
        goto error;
    }

    if (fprintf(fp, HOSTS_DEFAULT_STR) <= 0)
    {
        fprintf(stderr, "Couldn't write '%s'\n", hosts_filename);
        goto error;
    }

    ret = EXIT_SUCCESS;

error:
    if (fp)
        fclose(fp);

    return ret;
}

static int check_modified_hosts_file(char const * const hostname)
{
    FILE *fp = NULL;
    int ret = EXIT_FAILURE;
    char const * const hosts_filename = SYSTEM_HOSTS_FILENAME;
    long fsize;
    char *buffer1 = NULL;
    char *buffer2 = NULL;

    if ((fp = fopen(hosts_filename, "r")) == NULL)
    {
        fprintf(stderr, "Couldn't open '%s'\n", hosts_filename);
        goto error;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer1 = malloc(fsize + 1);
    if (fread(buffer1, 1, fsize, fp) != fsize)
    {
        fclose(fp);

        fprintf(stderr, "Couldn't read '%s'\n", hosts_filename);
        goto error;
    }

    fclose(fp);

    buffer1[fsize] = 0;

    if (asprintf(&buffer2, SYSTEM_HOSTS_FILE_CONTENT, hostname) == -1)
    {
        fprintf(stderr, "Couldn't generate hosts test string\n");
        goto error;
    }

    if (strcmp(buffer1, buffer2) != 0)
    {
        fprintf(stderr, "Modified hosts file is bad\n");
        fprintf(stderr, "Expected:\n'%s'\n", buffer2);
        fprintf(stderr, "Modified:\n'%s'\n", buffer1);
    }

    ret = EXIT_SUCCESS;

error:
    if (buffer1)
        free(buffer1);

    if (buffer2)
        free(buffer2);

    return ret;
}


int ip_apply_hostname(char * const value);

void test_ip_apply_hostname()
{
    char const * const test_hostname = TEST_HOSTNAME;
    int status;

    create_hosts_files();

    system_command_return_value = EXIT_FAILURE;
    status = ip_apply_hostname((char * const)test_hostname);
	CU_ASSERT_EQUAL(status, EXIT_FAILURE);

    create_hosts_files();

    system_command_return_value = EXIT_SUCCESS;
    status = ip_apply_hostname((char * const)test_hostname);
    CU_ASSERT_EQUAL(status, EXIT_SUCCESS);

    CU_ASSERT_EQUAL(check_modified_hosts_file(test_hostname), EXIT_SUCCESS);
}

/********************
	Test Runner
********************/

int main() {
	unsigned int number_of_failures;

	//	Initialize the CUnit test registry
	if ( CUE_SUCCESS != CU_initialize_registry() )
		return CU_get_error();

	//	Add a suite to the registry
	CU_pSuite pSuite1 = CU_add_suite( "ip_test_suite", init_suite, clean_suite );
	if ( pSuite1 == NULL ) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add the tests to the suite
	if (
		(CU_add_test(pSuite1, "test_ip_read_hostname", test_ip_read_hostname)==NULL) ||
		(CU_add_test(pSuite1, "test_ip_apply_hostname", test_ip_apply_hostname)==NULL)
		)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	#if USE_BASIC_STYLE
		//	Run all tests using the basic interface
		CU_basic_set_mode(CU_BRM_SILENT);
		CU_basic_run_tests();
		CU_basic_show_failures(CU_get_failure_list());
	#elif USE_AUTOMATED_STYLE
		//	Run all tests using the automated interface
		CU_automated_run_tests();
		CU_list_tests_to_file();
	#else
		//	Run all tests using the console interface
		CU_console_run_tests();
	#endif

	//	Clean up registry and return
	number_of_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	//	Ensure we return an error if any tests failed
	if (number_of_failures)
		return -1;

	return CU_get_error();
}

