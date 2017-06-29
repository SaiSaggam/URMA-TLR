/*****************************************************************************
 * Copyright (c) 2015 Digi International Inc., All Rights Reserved
 *
 * This software contains proprietary and confidential information of Digi
 * International Inc.  By accepting transfer of this copy, Recipient agrees
 * to retain this software in confidence, to prevent disclosure to others,
 * and to make no use of this software other than that for which it was
 * delivered.  This is an unpublished copyrighted work of Digi International
 * Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
 * prohibited.
 *
 * Restricted Rights Legend
 *
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7031 or
 * subparagraphs (c)(1) and (2) of the Commercial Computer Software -
 * Restricted Rights at 48 CFR 52.227-19, as applicable.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 *
 *****************************************************************************/

#define	USE_BASIC_STYLE		1
#define	USE_AUTOMATED_STYLE	0
#define	USE_CONSOLE_STYLE	0

#define SHOW_LAN "show_lan"

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
#include "show_lan_work.h"
#include "tlr_common.h"
#include "config_mgr.h"
#include "config_status.h"
#include "show_parse_helper.h"
#include "show_status_helper.h"
#include "interfaces.h"

#include <stdlib.h>
#include <dirent.h>

/********************
	Stub Functions
********************/
static size_t allocated_memory_count = 0;

static config_status_t tlr_get_element_alloc_value_return = config_status_success;
unsigned int tlr_get_element_alloc_value_state;
char * tlr_get_element_alloc_value_description = NULL;
char * tlr_get_element_alloc_value_interfaces = NULL;

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
    {
        srclen = size;
    }

    memcpy(dst, src, srclen);
    dst[srclen] = '\0';

    return srclen;
}

config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void ** value,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{

    if (tlr_get_element_alloc_value_return == config_status_success)
    {
        switch (element_id)
        {
            case config_lan_state:
                *value = &tlr_get_element_alloc_value_state;
                break;
            case config_lan_description:
                *value = tlr_get_element_alloc_value_description;
                break;
            case config_lan_interfaces:
                *value = tlr_get_element_alloc_value_interfaces;
                break;
        }
        allocated_memory_count++;

    }
    return tlr_get_element_alloc_value_return;
}

void show_lan_free(void * ptr)
{
    if (ptr != NULL)
        allocated_memory_count--;

}

uint64_t get_rx_packets_value;
uint64_t get_tx_packets_value;
uint64_t get_rx_bytes_value;
uint64_t get_tx_bytes_value;
int get_rx_packets(char const * const interface, uint64_t * const value)
{
    *value = get_rx_packets_value;
    return 0;
}

int get_tx_packets(char const * const interface, uint64_t * const value)
{
    *value = get_tx_packets_value;
    return 0;
}

int get_rx_bytes(char const * const interface, uint64_t * const value)
{
    *value = get_rx_bytes_value;
    return 0;
}

int get_tx_bytes(char const * const interface, uint64_t * const value)
{
    *value = get_tx_bytes_value;
    return 0;
}

int get_mtu_value;
void get_mtu (char const * const ifname, int * const buf)
{
    *buf = get_mtu_value;
}

char const * get_ipaddr_value = NULL;
void get_ipaddr (char const * const ifname, char * const buf, size_t const bufsize)
{
    if (get_ipaddr_value != NULL)
    {
        snprintf(buf, bufsize, "%s", get_ipaddr_value);
    }
}

char const * get_newmaskaddr_value;
void get_newmaskaddr (char const * const ifname, char * const buf, size_t const bufsize )
{
    if (get_newmaskaddr_value != NULL)
    {
        snprintf(buf, bufsize, "%s", get_newmaskaddr_value);
    }

}

char * tlr_get_sys_class_net_value;

char * tlr_get_sys_class_net(char const * const interface_name, char const * const object)
{
    allocated_memory_count++;

    return tlr_get_sys_class_net_value;

}
char * tlr_get_mac_address_value;

char const * tlr_get_mac_address(char const * const interface_name)
{
    allocated_memory_count++;

    return tlr_get_mac_address_value;
}

size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type)
{
    return 1;
}

char const * interface_get_name_from_type(interfaces_t const type)
{
    return "lan";
}

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{

}

void fprintf_status_header(FILE * const stream, size_t const divider_width, char const * const format, ...)
{
}

DIR * opendir_value;
DIR  * show_lan_opendir(const char *filename)
{
    allocated_memory_count++;

    return opendir_value;
}

int show_lan_closedir(DIR * dirp)
{
    return 0;
}

size_t tlr_get_config_value_max_length(config_mgr_table_type_t const type, size_t const config_id, size_t const element_id)
{
    return 64;
}

size_t readdir_value_index = 0;
struct dirent * readdir_value;
struct dirent  * show_lan_readdir(DIR * dirp)
{
    struct dirent * dir = NULL;

    if (readdir_value_index == 0)
    {
        allocated_memory_count++;
    }

    if ((readdir_value != NULL) && (*readdir_value[readdir_value_index].d_name != '\0'))
        dir = &readdir_value[readdir_value_index++];

    return dir;
}


/********************
	Test Suite Setup
********************/

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/********************
	Test Case Functions
********************/

bool get_lan_state(size_t const instance);

void test_get_lan_state()
{
    bool state;

    tlr_get_element_alloc_value_return = config_status_no_memory;
    state = get_lan_state(1);

    CU_ASSERT_FALSE(state);
    CU_ASSERT_EQUAL(allocated_memory_count, 0);

    {
       unsigned int element_value[] = {1, 0};
       size_t i;
       for (i = 0; i < ARRAY_SIZE(element_value); i++)
       {
           tlr_get_element_alloc_value_return = config_status_success;
           tlr_get_element_alloc_value_state = element_value[i];

           state = get_lan_state(1);
           CU_ASSERT_EQUAL(state, (element_value[i] == 1));
           CU_ASSERT_EQUAL(allocated_memory_count, 0);
       }
    }

}

char * get_lan_description(size_t const instance);

void test_get_lan_description()
{
    char * description = NULL;

    tlr_get_element_alloc_value_return = config_status_no_memory;
    description = get_lan_description(1);

    CU_ASSERT_PTR_NULL(description);
    CU_ASSERT_EQUAL(allocated_memory_count, 0);

    {
        char const element_value[] = "Description";
        tlr_get_element_alloc_value_return = config_status_success;
        tlr_get_element_alloc_value_description = (char *)element_value;
       description = get_lan_description(1);

       CU_ASSERT_PTR_NOT_NULL(description);
       CU_ASSERT_EQUAL(allocated_memory_count, 1);
       CU_ASSERT_STRING_EQUAL(description, element_value);
       show_lan_free(description);
    }

}


char * get_lan_interfaces(char const * const interface_name);

void test_get_lan_interfaces()
{
    char * interfaces = NULL;

    opendir_value = NULL;
    allocated_memory_count = 0;
    interfaces = get_lan_interfaces("lan1");

    CU_ASSERT_PTR_NULL(interfaces);
    CU_ASSERT_EQUAL(allocated_memory_count, 0);

    {
        int opendir_handle;
        struct dirent interfaces_directoires[4];
        char const element_value[] = "eth1,eth2,dsl";

        strcpy(interfaces_directoires[0].d_name, "eth1");
        strcpy(interfaces_directoires[1].d_name, "eth2");
        strcpy(interfaces_directoires[2].d_name, "dsl");
        *interfaces_directoires[3].d_name = '\0';

        opendir_value = (DIR *)&opendir_handle;
        readdir_value = &interfaces_directoires[0];


        tlr_get_element_alloc_value_interfaces = (char *)element_value;

        interfaces = get_lan_interfaces("lan1");

       CU_ASSERT_PTR_NOT_NULL(interfaces);
       CU_ASSERT_EQUAL(allocated_memory_count, 1);
       CU_ASSERT_STRING_EQUAL(interfaces, element_value);
       show_lan_free(interfaces);

    }

}

bool get_operational_status(char const * const interface_name);
void test_get_operational_status()
{
    {
        char const oper_state[] ="Down";
        bool oper_status;
        tlr_get_sys_class_net_value = (char *)oper_state;
        allocated_memory_count= 0;

        oper_status = get_operational_status(NULL);

        CU_ASSERT_FALSE(oper_status);
        CU_ASSERT_EQUAL(allocated_memory_count, 0);
    }

    {
        char const oper_state[] ="Down";
        bool oper_status;
        tlr_get_sys_class_net_value = (char *)oper_state;
        allocated_memory_count= 0;

        oper_status = get_operational_status("lan1");

        CU_ASSERT_FALSE(oper_status);
        CU_ASSERT_EQUAL(allocated_memory_count, 0);
    }

    {
        char const oper_state[] ="Unknown";
        bool oper_status;
        tlr_get_sys_class_net_value = (char *)oper_state;
        allocated_memory_count= 0;

        oper_status = get_operational_status("lan1");

        CU_ASSERT_TRUE(oper_status);
        CU_ASSERT_EQUAL(allocated_memory_count, 0);
    }

    {
        char const oper_state[] ="Up";
        bool oper_status;
        tlr_get_sys_class_net_value = (char *)oper_state;
        allocated_memory_count= 0;

        oper_status = get_operational_status("lan1");

        CU_ASSERT_TRUE(oper_status);
        CU_ASSERT_EQUAL(allocated_memory_count, 0);
    }


}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    (void)group_id;
    (void)instance;
    (void)access;

    return NULL;
}
config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    (void)handle;
    (void)element_id;
    (void)data;
    (void)bytes;

    return config_status_system_error;
}

void tlr_config_close(tlr_handle_t const handle)
{
    (void)handle;
}

void test_get_lan_status(void)
{
    lan_status_t lan_status;

    char const description[] = "MyDescription";
    char const mac_address[] = "MyMacAddress";
    char const oper_state[] = "Down";
    char const ip_address[] = "10.10.10.1";
    char const subnet_mask[] = "123.0.0.0";
    char const interfaces[]="eth1,dsl";
    int opendir_handle;
    struct dirent interfaces_directories[4];

    strcpy(interfaces_directories[0].d_name, "eth1");
    strcpy(interfaces_directories[1].d_name, "dsl");
    *interfaces_directories[2].d_name = '\0';

    opendir_value = (DIR *)&opendir_handle;
    readdir_value = &interfaces_directories[0];

    readdir_value_index = 0;
    allocated_memory_count = 0;


    tlr_get_element_alloc_value_return = config_status_success;
    tlr_get_element_alloc_value_state = 1;
    tlr_get_element_alloc_value_description = (char *)description;
    tlr_get_element_alloc_value_interfaces = (char *)interfaces;
    tlr_get_mac_address_value = (char *)mac_address;
    tlr_get_sys_class_net_value = (char *)oper_state;
    get_mtu_value = 1000;
    get_ipaddr_value = ip_address;
    get_newmaskaddr_value = subnet_mask;

    get_rx_packets_value = 0;
    get_tx_packets_value = 5;
    get_rx_bytes_value = 10;
    get_tx_bytes_value = 15;

    get_lan_status(1, &lan_status);

    CU_ASSERT_EQUAL(allocated_memory_count, 3);
    CU_ASSERT_EQUAL(lan_status.admin_status, (tlr_get_element_alloc_value_state == 1));
    CU_ASSERT_STRING_EQUAL(interfaces, lan_status.interfaces);
    CU_ASSERT_STRING_EQUAL(description, lan_status.description);
    CU_ASSERT_STRING_EQUAL(mac_address, lan_status.mac_address);
    CU_ASSERT_EQUAL(lan_status.oper_status, 0);

    CU_ASSERT_EQUAL(get_mtu_value, lan_status.mtu);
    CU_ASSERT_STRING_EQUAL(get_ipaddr_value, lan_status.ip_address);
    CU_ASSERT_STRING_EQUAL(get_newmaskaddr_value, lan_status.mask);
    CU_ASSERT_EQUAL(get_rx_packets_value, lan_status.rx_packets);
    CU_ASSERT_EQUAL(get_tx_packets_value, lan_status.tx_packets);
    CU_ASSERT_EQUAL(get_rx_bytes_value, lan_status.rx_bytes);
    CU_ASSERT_EQUAL(get_tx_bytes_value, lan_status.tx_bytes);
}

void test_show_lan_work_missing_format_results_in_error()
{
    int status;

    allocated_memory_count = 0;
    get_ipaddr_value = NULL;
    get_newmaskaddr_value = NULL;

    {
        char * argv[] = {SHOW_LAN, "-i", "20"};

        status = show_lan_work(ARRAY_SIZE(argv), argv);

        CU_ASSERT_EQUAL(status, EXIT_FAILURE);
        CU_ASSERT_EQUAL(allocated_memory_count, 0);
    }
}

void test_show_lan_work_format_ascii_results_in_success()
{
    int status;

    allocated_memory_count = 0;
    get_ipaddr_value = NULL;
    get_newmaskaddr_value = NULL;

    {
        char * argv[] = {SHOW_LAN, "-i", "1", "-f", "ascii"};


        opendir_value = NULL;
        allocated_memory_count = 0;

        status = show_lan_work(ARRAY_SIZE(argv), argv);

        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
        CU_ASSERT_EQUAL(allocated_memory_count, 0);
    }
}

void test_show_lan_work_format_json_results_in_success()
{
    int status;

    allocated_memory_count = 0;
    get_ipaddr_value = NULL;
    get_newmaskaddr_value = NULL;

    {
        char * argv[] = { SHOW_LAN, "-i", "1", "-f", "json" };


        opendir_value = NULL;
        allocated_memory_count = 0;

        status = show_lan_work(ARRAY_SIZE(argv), argv);

        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
        CU_ASSERT_EQUAL(allocated_memory_count, 0);
    }
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
	CU_pSuite pSuite = CU_add_suite( "show_lan_test_suite", init_suite, clean_suite );
	if ( pSuite == NULL ) {
		CU_cleanup_registry();
		return CU_get_error();
	}


	// Add the tests to the suite
	if ( 0 ||
		(CU_add_test(pSuite, "test_get_lan_state", test_get_lan_state)==NULL) ||
        (CU_add_test(pSuite, "test_get_lan_description", test_get_lan_description)==NULL) ||
        (CU_add_test(pSuite, "test_get_lan_interfaces", test_get_lan_interfaces)==NULL) ||
        (CU_add_test(pSuite, "test_get_operational_status", test_get_operational_status)==NULL) ||
        (CU_add_test(pSuite, "test_get_lan_status", test_get_lan_status)==NULL) ||
         (CU_add_test(pSuite, "test_show_lan_work_missing_format_results_in_error", test_show_lan_work_missing_format_results_in_error)==NULL) ||
         (CU_add_test(pSuite, "test_show_lan_work_format_ascii_results_in_success", test_show_lan_work_format_ascii_results_in_success)==NULL) ||
         (CU_add_test(pSuite, "test_show_lan_work_format_json_results_in_success", test_show_lan_work_format_json_results_in_success)==NULL)
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


/** Stubs    */

tlr_show_object_t * tlr_show_object_create(size_t const group_id, size_t const instance)
{
    return NULL;
}
void tlr_show_object_add_element(tlr_show_object_t * const show_object, size_t const element_id, void const * const value)
{
}
char const * tlr_show_object_to_json_string(tlr_show_object_t const * const show_object)
{
    return NULL;
}
void tlr_show_object_destroy(tlr_show_object_t * const show_object)
{
}


config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    return NULL;
}
