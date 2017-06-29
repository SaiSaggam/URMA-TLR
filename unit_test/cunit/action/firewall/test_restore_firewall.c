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
#define USE_BASIC_STYLE     1
#define USE_AUTOMATED_STYLE 0
#define USE_CONSOLE_STYLE   0

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
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>
#include "config_def.h"

#include "restore_firewall.c"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

static bool popen_fail=false;
static bool asprintf_fail=false;
static bool fopen_fail=false;
static bool system_command_fail=false;

static void test_restore_firewall_rules_from_config_da0(void)
{
    restore_firewall_rules_t expected_value;
    restore_firewall_rules_t actual_value;

    expected_value = error;
    fopen_fail = true;
    actual_value = restore_firewall_rules_from_config_da0();
    CU_ASSERT_EQUAL(actual_value, expected_value);
    fopen_fail = false;
}

int main( void )
{
    int result;
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        result = CU_get_error();
        goto error;
    }

    pSuite = CU_add_suite( "test_restore_firewall", init_suite, clean_suite );
    if ( NULL == pSuite )
    {
        result = CU_get_error();
        goto error;
    }

    if ((CU_add_test(pSuite, "test_restore_firewall_main", test_restore_firewall_rules_from_config_da0) == NULL))
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

#elif USE_CONSOLE_STYLE
    // Run all tests using the console interface
    CU_console_run_tests();
#endif

    result = CU_get_number_of_failures();

error:
	CU_cleanup_registry();
	return result;
}



int asprintf(char **strp, const char *fmt, ...)
{
    return (asprintf_fail) ? -1 : 0;
}

FILE *fopen(const char  * __restrict fn, const char  * __restrict m)
{
    static FILE fd;
    return (fopen_fail) ? NULL : &fd;
}

int fclose(FILE *fp)
{
    return 0;
}

FILE *popen(const char *command, const char *type)
{
    static FILE fd;
    return (popen_fail) ? NULL : &fd;
}

int pclose(FILE *fp)
{
    return 0;
}

int system_command (char const * const format, ...)
{
    return (system_command_fail) ? -1 : 0;
}

static char const * const config_file = "/tmp/config.da0";

void tlr_get_config_file(char * cfg_file, int maxlen)
{
    int len=strlen(config_file);

    len = (len > maxlen) ? maxlen : len;
    strncpy(cfg_file, config_file, maxlen);
    cfg_file[maxlen - 1] = '\0';
}

ssize_t getline(char **a, size_t *b, FILE *c)
{
    return -1;
}

int fprintf(FILE *stream, const char *format, ...)
{
    return 1;
}

bool firewall_line_is_start_tag(char const * const line)
{
    return strstr(line, FIREWALL_TAG) != NULL;
}

bool firewall_line_is_end_tag(char const * const line)
{
    return strstr(line, FIREWALL_END_TAG) != NULL;
}
