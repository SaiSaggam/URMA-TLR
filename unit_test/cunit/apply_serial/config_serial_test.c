
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>


#include "config_mgr.h"

extern int set_baud(tlr_handle_t tlr_handle, struct termios * tio);
extern int set_databits(tlr_handle_t tlr_handle, struct termios * tio);
extern int set_parity(tlr_handle_t tlr_handle, struct termios * tio);
extern int set_stopbits(tlr_handle_t tlr_handle, struct termios * tio);
extern int set_flowcontrol(tlr_handle_t tlr_handle, struct termios * tio);
extern int config_serial_main (int argc, char *argv[]);


/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

typedef struct {
    char * value;
    size_t id;
} element_t;

static element_t current_element;

void simulate_element_value(char const * value, size_t const id)
{
    current_element.value = (char *)value;
    current_element.id = id;
}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    static unsigned int fd;

    return ((tlr_handle_t) &fd);
}

config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    config_status_t rc = config_status_element_not_found;


    if ((current_element.value != NULL) && (bytes > strlen(current_element.value)))
    {
        snprintf(data, bytes, "%s", current_element.value);
        rc = config_status_success;
    }
    return rc;
}

void tlr_config_close(tlr_handle_t const handle)
{
    return;
}

config_status_t tlr_config_string_to_value(size_t const group_id, size_t const element_id, char const * const buffer, void * const value)
{
    config_status_t rc = config_status_element_not_found;

    if ((buffer != NULL) && (value != NULL))
    {
        int * integer_value = value;
        *integer_value = current_element.id;
        rc = config_status_success;
    }
    return rc;
}

/************* Test case functions ****************/

void baud_rates_tests()
{
    struct termios tio;
    tlr_handle_t tlr_handle = NULL;
    size_t i;
    int retv;

    char const *baud_rates[] = {"110", "300", "600", "1200", "2400", "4800", "9600",
                                "19200", "38400", "57600", "115200", "230400", "460800", "921600"};

    config_serial_baud_t baud_rates_id[] = {
        config_serial_baud_110,
        config_serial_baud_300,
        config_serial_baud_600,
        config_serial_baud_1200,
        config_serial_baud_2400,
        config_serial_baud_4800,
        config_serial_baud_9600,
        config_serial_baud_19200,
        config_serial_baud_38400,
        config_serial_baud_57600,
        config_serial_baud_115200,
        config_serial_baud_230400,
        config_serial_baud_460800,
        config_serial_baud_921600
    };

    size_t baud_rates_count = sizeof baud_rates / sizeof baud_rates[0];

    CU_ASSERT(baud_rates_count == (sizeof baud_rates_id / sizeof baud_rates_id[0]));

    for (i=0; i < baud_rates_count; i++)
    {
        simulate_element_value(baud_rates[i], baud_rates_id[i]);
        retv = set_baud(tlr_handle, &tio);
        CU_ASSERT_EQUAL(retv, 0);
    }

    simulate_element_value(baud_rates[0], i);
    retv = set_baud(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);

    simulate_element_value(NULL, 0);
    retv = set_baud(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);

}

void data_bits_tests()
{
    struct termios tio;
    tlr_handle_t tlr_handle = NULL;
    size_t i;
    int retv;

    static char const *databits[] = {"8", "7"};
    size_t databits_count = sizeof databits / sizeof databits[0];

    config_serial_databits_t databits_id[] = {
            config_serial_databits_8,
            config_serial_databits_7
    };

    CU_ASSERT(databits_count == (sizeof databits_id / sizeof databits_id[0]));

    for (i=0; i < databits_count; i++)
    {
        simulate_element_value(databits[i], databits_id[i]);
        retv = set_databits(tlr_handle, &tio);
        CU_ASSERT_EQUAL(retv, 0);
    }

    simulate_element_value(databits[0], i);
    retv = set_databits(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);

    simulate_element_value(NULL, 0);
    retv = set_databits(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);
}

void parity_tests()
{
    struct termios tio;
    tlr_handle_t tlr_handle = NULL;
    size_t i;
    int retv;

    static char const *parity[] = {"none", "odd", "even"};
    size_t parity_count = sizeof parity / sizeof parity[0];

    config_serial_parity_t parity_id[] = {
        config_serial_parity_none,
        config_serial_parity_odd,
        config_serial_parity_even
    } ;

    CU_ASSERT(parity_count == (sizeof parity_id / sizeof parity_id[0]));

    for (i=0; i < parity_count; i++)
    {
        simulate_element_value(parity[i], parity_id[i]);
        retv = set_parity(tlr_handle, &tio);
        CU_ASSERT_EQUAL(retv, 0);
    }

    simulate_element_value(parity[0], i);
    retv = set_parity(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);

    simulate_element_value(NULL, 0);
    retv = set_parity(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);
}

void stop_bits_tests()
{
    struct termios tio;
    tlr_handle_t tlr_handle = NULL;
    size_t i;
    int retv;

    static char const *stopbits[] = {"1", "2"};
    size_t stopbits_count = sizeof stopbits / sizeof stopbits[0];

    config_serial_stopbits_t stopbits_id[] = {
        config_serial_stopbits_1,
        config_serial_stopbits_2
    };

    CU_ASSERT(stopbits_count == (sizeof stopbits_id / sizeof stopbits_id[0]));

    for (i=0; i < stopbits_count; i++)
    {
        simulate_element_value(stopbits[i], stopbits_id[i]);
        retv = set_stopbits(tlr_handle, &tio);
        CU_ASSERT_EQUAL(retv, 0);
    }

    simulate_element_value(stopbits[0], i);
    retv = set_stopbits(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);

    simulate_element_value(NULL, 0);
    retv = set_stopbits(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);
}

void flowcontrol_tests()
{
    struct termios tio;
    tlr_handle_t tlr_handle = NULL;
    size_t i;
    int retv;

    static char const *flowcontrol[] = {"none", "software", "hardware"};
    static size_t const flowcontrol_count = sizeof flowcontrol / sizeof flowcontrol[0];

    static config_serial_flowcontrol_t const flowcontrol_id[] = {
        config_serial_flowcontrol_none,
        config_serial_flowcontrol_software,
        config_serial_flowcontrol_hardware
    };

    CU_ASSERT(flowcontrol_count == (sizeof flowcontrol_id / sizeof flowcontrol_id[0]));

    for (i=0; i < flowcontrol_count; i++)
    {
        simulate_element_value(flowcontrol[i], flowcontrol_id[i]);
        retv = set_flowcontrol(tlr_handle, &tio);
        CU_ASSERT_EQUAL(retv, 0);
    }

    simulate_element_value(flowcontrol[0], i);
    retv = set_flowcontrol(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);

    simulate_element_value(NULL, 0);
    retv = set_flowcontrol(tlr_handle, &tio);
    CU_ASSERT_EQUAL(retv, -1);
}

void config_serial_main_test()
{
    char * argv[3];
    int retv;

    argv[0] = "config_serial";

    retv = config_serial_main (1, argv);
    CU_ASSERT_NOT_EQUAL(retv, 0);

    argv[1] = "-p";
    retv = config_serial_main (2, argv);
    CU_ASSERT_NOT_EQUAL(retv, 0);

    argv[1] = "-i";
    retv = config_serial_main (2, argv);
    CU_ASSERT_NOT_EQUAL(retv, 0);

    argv[1] = "-p";
    argv[2] =  "0";
    retv = config_serial_main (3, argv);
    CU_ASSERT_NOT_EQUAL(retv, 0);

}

/************* Test Runner Code goes here **************/

int main ( void )
{
    int result;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        result = CU_get_error();
        goto error;
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite( "config_serial_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite )
    {
        result = CU_get_error();
        goto error;
    }

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "baud rate test", baud_rates_tests)==NULL) ||
        (CU_add_test(pSuite, "data bit test", data_bits_tests)==NULL) ||
        (CU_add_test(pSuite, "parity test", parity_tests)==NULL) ||
        (CU_add_test(pSuite, "stop bits test", stop_bits_tests)==NULL) ||
        (CU_add_test(pSuite, "flow control test", flowcontrol_tests)==NULL) ||
        (CU_add_test(pSuite, "config serial man", config_serial_main_test)==NULL))
    {
        result = CU_get_error();
        goto error;
    }

#if USE_BASIC_STYLE
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
	if (CU_basic_run_tests() != CUE_SUCCESS)
	{
        result = CU_get_error();
        goto error;
	}
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

    result = CU_get_number_of_failures();

error:
	CU_cleanup_registry();
	return result;
}



/****************  Stubs   ***************************/

int tcgetattr(int fd, struct termios *termios_p)
{
    return 0;
}

int tcsetattr(int fd, int optional_actions, const struct termios *termios_p)
{
    return 0;
}

