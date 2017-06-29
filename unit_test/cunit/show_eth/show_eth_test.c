/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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
#include <json.h>

#include "config_mgr.h"
#include "ifuptime_lib.h"
#include "show_eth.h"
#include "messages.h"
#include "tlr_print.h"

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

static ethstat_t stats[6];

/************* Test case functions ****************/
void parse_eth_tests()
{

    if ( parse_eth_stat( &stats[0], "esw_cnt") != 0 )
        {
        CU_FAIL("open esw_cnt error");
        return;
        }

    CU_ASSERT_EQUAL ( stats[0].tx_drop_pak, 1 );
    CU_ASSERT_EQUAL ( stats[0].tx_crc_error, 3 );
    CU_ASSERT_EQUAL ( stats[0].tx_unicast_pak, 2 );
    CU_ASSERT_EQUAL ( stats[0].tx_multicast_pak, 5076 );
    CU_ASSERT_EQUAL ( stats[0].tx_bcast_pak, 3295 );
    CU_ASSERT_EQUAL ( stats[0].tx_coll_pak, 4 );
    CU_ASSERT_EQUAL ( stats[0].tx_pause_pak, 5 );
    CU_ASSERT_EQUAL ( stats[0].rx_drop_pak, 6 );
    CU_ASSERT_EQUAL ( stats[0].rx_filt_pak, 7 );
    CU_ASSERT_EQUAL ( stats[0].rx_unicast_pak, 8 );
    CU_ASSERT_EQUAL ( stats[0].rx_multicast_pak, 9 );
    CU_ASSERT_EQUAL ( stats[0].rx_bcast_pak, 10 );
    CU_ASSERT_EQUAL ( stats[0].rx_align_err, 11 );
    CU_ASSERT_EQUAL ( stats[0].rx_crc_err, 12 );
    CU_ASSERT_EQUAL ( stats[0].rx_und_err, 13 );
    CU_ASSERT_EQUAL ( stats[0].rx_frag_err, 14 );
    CU_ASSERT_EQUAL ( stats[0].rx_oversize_err, 15 );
    CU_ASSERT_EQUAL ( stats[0].rx_jabber_err, 16 );
    CU_ASSERT_EQUAL ( stats[0].rx_pause_pak, 17 );

    CU_ASSERT_EQUAL ( stats[1].tx_drop_pak, 18 );
    CU_ASSERT_EQUAL ( stats[1].tx_crc_error, 19 );
    CU_ASSERT_EQUAL ( stats[1].tx_unicast_pak, 937 );
    CU_ASSERT_EQUAL ( stats[1].tx_multicast_pak, 375 );
    CU_ASSERT_EQUAL ( stats[1].tx_bcast_pak, 346 );
    CU_ASSERT_EQUAL ( stats[1].tx_coll_pak, 20 );
    CU_ASSERT_EQUAL ( stats[1].tx_pause_pak, 21 );
    CU_ASSERT_EQUAL ( stats[1].rx_drop_pak, 22 );
    CU_ASSERT_EQUAL ( stats[1].rx_filt_pak, 28 );
    CU_ASSERT_EQUAL ( stats[1].rx_unicast_pak, 1121 );
    CU_ASSERT_EQUAL ( stats[1].rx_multicast_pak, 4704 );
    CU_ASSERT_EQUAL ( stats[1].rx_bcast_pak, 2950 );
    CU_ASSERT_EQUAL ( stats[1].rx_align_err, 23 );
    CU_ASSERT_EQUAL ( stats[1].rx_crc_err, 24 );
    CU_ASSERT_EQUAL ( stats[1].rx_und_err, 25 );
    CU_ASSERT_EQUAL ( stats[1].rx_frag_err, 26 );
    CU_ASSERT_EQUAL ( stats[1].rx_oversize_err, 27 );
    CU_ASSERT_EQUAL ( stats[1].rx_jabber_err, 28 );
    CU_ASSERT_EQUAL ( stats[1].rx_pause_pak, 29 );

    CU_ASSERT_EQUAL ( stats[2].tx_drop_pak, 30 );
    CU_ASSERT_EQUAL ( stats[2].tx_crc_error, 31 );
    CU_ASSERT_EQUAL ( stats[2].tx_unicast_pak, 1111 );
    CU_ASSERT_EQUAL ( stats[2].tx_multicast_pak, 920 );
    CU_ASSERT_EQUAL ( stats[2].tx_bcast_pak, 433 );
    CU_ASSERT_EQUAL ( stats[2].tx_coll_pak, 32 );
    CU_ASSERT_EQUAL ( stats[2].tx_pause_pak, 33 );
    CU_ASSERT_EQUAL ( stats[2].rx_drop_pak, 34 );
    CU_ASSERT_EQUAL ( stats[2].rx_filt_pak, 35 );
    CU_ASSERT_EQUAL ( stats[2].rx_unicast_pak, 937 );
    CU_ASSERT_EQUAL ( stats[2].rx_multicast_pak, 371 );
    CU_ASSERT_EQUAL ( stats[2].rx_bcast_pak, 346 );
    CU_ASSERT_EQUAL ( stats[2].rx_align_err, 36 );
    CU_ASSERT_EQUAL ( stats[2].rx_crc_err, 37 );
    CU_ASSERT_EQUAL ( stats[2].rx_und_err, 38 );
    CU_ASSERT_EQUAL ( stats[2].rx_frag_err, 39 );
    CU_ASSERT_EQUAL ( stats[2].rx_oversize_err, 40 );
    CU_ASSERT_EQUAL ( stats[2].rx_jabber_err, 41 );
    CU_ASSERT_EQUAL ( stats[2].rx_pause_pak, 42 );

    CU_ASSERT_EQUAL ( stats[3].tx_drop_pak, 50 );
    CU_ASSERT_EQUAL ( stats[3].tx_crc_error, 51 );
    CU_ASSERT_EQUAL ( stats[3].tx_unicast_pak, 52 );
    CU_ASSERT_EQUAL ( stats[3].tx_multicast_pak, 53 );
    CU_ASSERT_EQUAL ( stats[3].tx_bcast_pak, 54 );
    CU_ASSERT_EQUAL ( stats[3].tx_coll_pak, 55 );
    CU_ASSERT_EQUAL ( stats[3].tx_pause_pak, 56 );
    CU_ASSERT_EQUAL ( stats[3].rx_drop_pak, 57 );
    CU_ASSERT_EQUAL ( stats[3].rx_filt_pak, 58 );
    CU_ASSERT_EQUAL ( stats[3].rx_unicast_pak, 59 );
    CU_ASSERT_EQUAL ( stats[3].rx_multicast_pak, 60 );
    CU_ASSERT_EQUAL ( stats[3].rx_bcast_pak, 61 );
    CU_ASSERT_EQUAL ( stats[3].rx_align_err, 62 );
    CU_ASSERT_EQUAL ( stats[3].rx_crc_err, 63 );
    CU_ASSERT_EQUAL ( stats[3].rx_und_err, 64 );
    CU_ASSERT_EQUAL ( stats[3].rx_frag_err, 65 );
    CU_ASSERT_EQUAL ( stats[3].rx_oversize_err, 66 );
    CU_ASSERT_EQUAL ( stats[3].rx_jabber_err, 67 );
    CU_ASSERT_EQUAL ( stats[3].rx_pause_pak, 68 );

    CU_ASSERT_EQUAL ( stats[4].tx_drop_pak, 70 );
    CU_ASSERT_EQUAL ( stats[4].tx_crc_error, 71 );
    CU_ASSERT_EQUAL ( stats[4].tx_unicast_pak, 72 );
    CU_ASSERT_EQUAL ( stats[4].tx_multicast_pak, 5073 );
    CU_ASSERT_EQUAL ( stats[4].tx_bcast_pak, 3294 );
    CU_ASSERT_EQUAL ( stats[4].tx_coll_pak, 73 );
    CU_ASSERT_EQUAL ( stats[4].tx_pause_pak, 74 );
    CU_ASSERT_EQUAL ( stats[4].rx_drop_pak, 75 );
    CU_ASSERT_EQUAL ( stats[4].rx_filt_pak, 76 );
    CU_ASSERT_EQUAL ( stats[4].rx_unicast_pak, 77 );
    CU_ASSERT_EQUAL ( stats[4].rx_multicast_pak, 78 );
    CU_ASSERT_EQUAL ( stats[4].rx_bcast_pak, 79 );
    CU_ASSERT_EQUAL ( stats[4].rx_align_err, 80 );
    CU_ASSERT_EQUAL ( stats[4].rx_crc_err, 81 );
    CU_ASSERT_EQUAL ( stats[4].rx_und_err, 82 );
    CU_ASSERT_EQUAL ( stats[4].rx_frag_err, 83 );
    CU_ASSERT_EQUAL ( stats[4].rx_oversize_err, 84 );
    CU_ASSERT_EQUAL ( stats[4].rx_jabber_err, 85 );
    CU_ASSERT_EQUAL ( stats[4].rx_pause_pak, 86 );

    CU_ASSERT_EQUAL ( stats[5].tx_drop_pak, 90 );
    CU_ASSERT_EQUAL ( stats[5].tx_crc_error, 91 );
    CU_ASSERT_EQUAL ( stats[5].tx_unicast_pak, 100 );
    CU_ASSERT_EQUAL ( stats[5].tx_multicast_pak, 101 );
    CU_ASSERT_EQUAL ( stats[5].tx_bcast_pak, 102 );
    CU_ASSERT_EQUAL ( stats[5].tx_coll_pak, 103 );
    CU_ASSERT_EQUAL ( stats[5].tx_pause_pak, 104 );
    CU_ASSERT_EQUAL ( stats[5].rx_drop_pak, 105 );
    CU_ASSERT_EQUAL ( stats[5].rx_filt_pak, 106 );
    CU_ASSERT_EQUAL ( stats[5].rx_unicast_pak, 107 );
    CU_ASSERT_EQUAL ( stats[5].rx_multicast_pak, 108 );
    CU_ASSERT_EQUAL ( stats[5].rx_bcast_pak, 109 );
    CU_ASSERT_EQUAL ( stats[5].rx_align_err, 110 );
    CU_ASSERT_EQUAL ( stats[5].rx_crc_err, 111 );
    CU_ASSERT_EQUAL ( stats[5].rx_und_err, 112 );
    CU_ASSERT_EQUAL ( stats[5].rx_frag_err, 113 );
    CU_ASSERT_EQUAL ( stats[5].rx_oversize_err, 114 );
    CU_ASSERT_EQUAL ( stats[5].rx_jabber_err, 115 );
    CU_ASSERT_EQUAL ( stats[5].rx_pause_pak, 116 );


    // And set these up for next test
    memcpy ( &ethstat[0], &stats[0], sizeof ethstat );
    memcpy ( &acc_ethstat[0], &stats[0], sizeof acc_ethstat );


}

void acc_tests()
{
    int     x;
    add_in_accumulated();

    // Now check its doubled...
    for (x=0; x<5; x++ )
        {
        CU_ASSERT_EQUAL ( ethstat[x].tx_drop_pak, 2*stats[x].tx_drop_pak );
        CU_ASSERT_EQUAL ( ethstat[x].tx_crc_error, 2*stats[x].tx_crc_error );
        CU_ASSERT_EQUAL ( ethstat[x].tx_unicast_pak, 2*stats[x].tx_unicast_pak );
        CU_ASSERT_EQUAL ( ethstat[x].tx_multicast_pak, 2*stats[x].tx_multicast_pak );
        CU_ASSERT_EQUAL ( ethstat[x].tx_bcast_pak, 2*stats[x].tx_bcast_pak );
        CU_ASSERT_EQUAL ( ethstat[x].tx_coll_pak, 2*stats[x].tx_coll_pak );
        CU_ASSERT_EQUAL ( ethstat[x].tx_pause_pak, 2*stats[x].tx_pause_pak );
        CU_ASSERT_EQUAL ( ethstat[x].rx_drop_pak, 2*stats[x].rx_drop_pak );
        CU_ASSERT_EQUAL ( ethstat[x].rx_filt_pak, 2*stats[x].rx_filt_pak );
        CU_ASSERT_EQUAL ( ethstat[x].rx_unicast_pak, 2*stats[x].rx_unicast_pak );
        CU_ASSERT_EQUAL ( ethstat[x].rx_multicast_pak, 2*stats[x].rx_multicast_pak );
        CU_ASSERT_EQUAL ( ethstat[x].rx_bcast_pak, 2*stats[x].rx_bcast_pak );
        CU_ASSERT_EQUAL ( ethstat[x].rx_align_err, 2*stats[x].rx_align_err );
        CU_ASSERT_EQUAL ( ethstat[x].rx_crc_err, 2*stats[x].rx_crc_err );
        CU_ASSERT_EQUAL ( ethstat[x].rx_und_err, 2*stats[x].rx_und_err );
        CU_ASSERT_EQUAL ( ethstat[x].rx_frag_err, 2*stats[x].rx_frag_err );
        CU_ASSERT_EQUAL ( ethstat[x].rx_oversize_err, 2*stats[x].rx_oversize_err );
        CU_ASSERT_EQUAL ( ethstat[x].rx_jabber_err, 2*stats[x].rx_jabber_err);
        CU_ASSERT_EQUAL ( ethstat[x].rx_pause_pak, 2*stats[x].rx_pause_pak);
        }


}


char const *expectations[] =
        {
        "Rx Unicast Packet   ",
        "Tx Unicast Packet   ",
        "Rx Broadcast Packet ",
        "Tx Broadcast Packet ",
        "Rx Multicast Packet ",
        "Tx Multicast Packet ",
        "Rx CRC Error        ",
        "Tx CRC Error        ",
        "Rx Drop Packet      ",
        "Tx Drop Packet      ",
        "Rx Pause Packet     ",
        "Tx Pause Packet     ",
        "Rx Filtering Packet ",
        "Tx Collision Event  ",
        "Rx Alignment Error  ",
        "Rx Undersize Error  ",
        "Rx Fragment Error   ",
        "Rx Oversize Error   ",
        "Rx Jabber Error     ",
        };
#define     NUM_EXPECTS   (sizeof expectations / sizeof expectations[0])

uint64_t const values_expected[4][NUM_EXPECTS] = {
    {   // port 1
    2242,1874,5900,692,9408,750,48,38,44,
    36,58,42,56,40,46,50,52,
    54,56
    },
    {   // port 2
    1874,2222,692,866,742,1840,74,62,68,
    60,84,66,70,64,72,76,78,
    80,82
    },
    {   // port 3
    118,104,122,108,120,106,126,102,114,
    100,136,112,116,110,124,128,130,
    132,134
    },
    {  // port 4
    154,144,158,6588,156,10146,162,142,150,
    140,172,148,152,146,160,164,166,
    168,170
    } };


void walk_tests()
{
    int         port;
    char        const *name;
    char        const **expect;
    uint64_t  value;
    uint64_t  const * expectvalue;
    int         itemscnt;

    for (port=1; port<=4; port++ )
        {
        element_walk_init(port);
        expect = &expectations[0];
        expectvalue = &values_expected[port-1][0];
        itemscnt=0;
        while ( walk_next_element ( &name, &value ) ==  0 )
            {
            CU_ASSERT_STRING_EQUAL(name, *expect );
            CU_ASSERT_EQUAL ( value, *expectvalue);
            expect++;
            expectvalue++;
            itemscnt++;
            }
        CU_ASSERT_EQUAL ( itemscnt, NUM_EXPECTS);
        }

}



/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;
   unsigned int number_of_failures;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "eth_status_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (CU_add_test(pSuite, "parsing esw_cnt tests", parse_eth_tests)==NULL) ||
        (CU_add_test(pSuite, "statistic accumulation tests", acc_tests)==NULL) ||
        (CU_add_test(pSuite, "walking statistic tests", walk_tests)==NULL)
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



/****************  Stubs   ***************************/


tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    return 0;
}
config_status_t tlr_config_read(void const * const handle, size_t const element_id, char * const data, size_t const bytes)
{
    return 0;
}
config_status_t tlr_config_write(void const * const handle, size_t const element_id, char const * const data)
{
    return 0;
}

void tlr_config_close(tlr_handle_t const handle)
{

}


struct json_object* json_object_new_object(void)
{
    return NULL;
}

void json_object_object_add(struct json_object* obj, const char *key,
				   struct json_object *val)
{
}


struct json_object* json_object_new_string(const char *s)
{
    return NULL;
}

struct json_object* json_object_new_int(int32_t i)
{
    return NULL;
}

struct json_object* json_object_new_int64(int64_t i)
{
    return NULL;

}

const char* json_object_to_json_string_ext(struct json_object *obj, int flags)
{
	return NULL;
}

int json_object_put(struct json_object *obj)
{
	return 0;
}

get_ifuptime_result_t get_ifuptime(char const * const interface_name, uint64_t * const uptime_in_seconds)
{
	return 0;
}

void format_ifuptime(uint64_t const input_seconds, char * const buf, size_t const bufsize)
{
}

int send_message(message_destination_t const destination, size_t instance, message_codes_t const message_code,
                 char const * const additional_info, char ** const response_buffer)
{
    return 0;
}

bool tlr_printf(tlr_response_format_t const type, char const * const format, ...)
{
    return true;
}

void tlr_event_log(tlr_log_t const priority, char const * const process_name, char const * const format, ...)
{
}

