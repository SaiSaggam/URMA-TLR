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

extern "C"
{
    #include "bank.h"
}

#include <CppUTest/TestHarness.h>
#include "CppUTestExt/MockSupport.h"
#include <stdio.h>

#define cmdline_filename "/proc/cmdline"
#define mtdblock_param "root=/dev/mtdblock"

FILE * fopen(const char * path, const char * mode)
{
    mock().actualCall("fopen").withParameter("path", path).withParameter("mode", mode);

    return (FILE *)mock().pointerReturnValue();
}

ssize_t getline(char ** lineptr, size_t * n, FILE * stream)
{
    mock().actualCall("getline").withOutputParameter("lineptr", lineptr).withOutputParameter("n", n);

    return mock().longIntReturnValue();
}

int fclose(FILE * fp)
{
    return 0;
}

TEST_GROUP(get_tlr_bank)
{
};

TEST(get_tlr_bank, fopen_returns_null_results_in_failure)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)NULL);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(-1, result);
}

TEST(get_tlr_bank, getline_returns_minus_one_results_in_failure)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr = NULL;
    size_t n = 0;
    mock().expectOneCall("getline").andReturnValue(-1).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(-1, result);
}

TEST(get_tlr_bank, getline_returns_zero_results_in_failure)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr = NULL;
    size_t n = 0;
    mock().expectOneCall("getline").andReturnValue(0).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(-1, result);
}

TEST(get_tlr_bank, cmdline_does_not_contain_mtdblock_param_results_in_failure)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr;
    size_t n = asprintf(&lineptr, "%s", "any=old cmd_line=here") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n-1)).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(-1, result);
}

TEST_GROUP(get_tlr_bank_cmdline_ok)
{
};

TEST(get_tlr_bank_cmdline_ok, mtdblock3_param_at_start)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr;
    size_t n = asprintf(&lineptr, "%s", mtdblock_param "3") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n-1)).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(0, result);
    LONGS_EQUAL(0, bank);
}

TEST(get_tlr_bank_cmdline_ok, mtdblock3_param_at_end)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr;
    size_t n = asprintf(&lineptr, "%s", "console=ttyS1,57600n8" mtdblock_param "3") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n-1)).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(0, result);
    LONGS_EQUAL(0, bank);
}

TEST(get_tlr_bank_cmdline_ok, mtdblock3_param_in_middle)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr;
    size_t n = asprintf(&lineptr, "%s", "console=ttyS1,57600n8" mtdblock_param "3" " isolcpus=2,3") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n-1)).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(0, result);
    LONGS_EQUAL(0, bank);
}

TEST(get_tlr_bank_cmdline_ok, mtdblock5_param)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr;
    size_t n = asprintf(&lineptr, "%s", mtdblock_param "5") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n-1)).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(0, result);
    LONGS_EQUAL(1, bank);
}

TEST(get_tlr_bank_cmdline_ok, mtdblock0_param)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr;
    size_t n = asprintf(&lineptr, "%s", mtdblock_param "0") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n-1)).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(-1, result);
}

TEST(get_tlr_bank_cmdline_ok, mtdblock30_param)
{
    int result;
    int bank;

    mock().expectOneCall("fopen").withParameter("path", cmdline_filename).withParameter("mode", "r").andReturnValue((void *)1);

    char * lineptr;
    size_t n = asprintf(&lineptr, "%s", mtdblock_param "30") + 1;
    mock().expectOneCall("getline").andReturnValue((ssize_t)(n-1)).withOutputParameterReturning("lineptr", &lineptr, sizeof lineptr).withOutputParameterReturning("n", &n, sizeof n);

    result = get_tlr_bank(&bank);
    LONGS_EQUAL(-1, result);
}

