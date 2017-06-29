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

extern "C"
{
#include "tlr_popen.h"
#include <unistd.h>
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// And some internal functions we will test.

struct process_id_st
{
	FILE 	* read_process_stream_fh;
	FILE 	* write_process_stream_fh;
	pid_t 	child_pid;
    int     read_file_handle;
    int     write_file_handle;
	TAILQ_ENTRY(process_id_st) entry;
};



void add_into_pid_list(pid_t child_pid, FILE * read_process_fh, FILE * write_process_fh,
                                      int const read_file_handle, int const write_file_handle);

struct process_id_st * find_in_pid_list(FILE * process_fh);
void free_from_pid_list(struct process_id_st	* pid_entry);
bool make_args_list(char const * const  cmdstring, char *** argslist);
void free_args_list(char ** argslist);

}


#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


TEST_GROUP(tlr_popen)
{
};


TEST(tlr_popen, simple_adding_into_pid_list)
{
    process_id_st * process;

    // First check we cannot find
    process = find_in_pid_list((FILE *)1);
    POINTERS_EQUAL((FILE *)0, process);
    process = find_in_pid_list((FILE *)3);
    POINTERS_EQUAL((FILE *)0, process);

    // simple add
    add_into_pid_list(2, (FILE *)3, (FILE *)NULL, 4, 5 );
    process = find_in_pid_list((FILE *)3);
    CHECK_TRUE(process != NULL);
    if (process != NULL)
    {
        LONGS_EQUAL(2, process->child_pid);
        POINTERS_EQUAL((FILE *)3, process->read_process_stream_fh);
        POINTERS_EQUAL((FILE *)NULL, process->write_process_stream_fh);
        LONGS_EQUAL(4, process->read_file_handle);
        LONGS_EQUAL(5, process->write_file_handle);
    }
    add_into_pid_list(12, (FILE *)NULL, (FILE *)4, 5, 6 );
    process = find_in_pid_list((FILE *)4);
    CHECK_TRUE(process != NULL);
    if (process != NULL)
    {
        LONGS_EQUAL(12, process->child_pid);
        POINTERS_EQUAL((FILE *)NULL, process->read_process_stream_fh);
        POINTERS_EQUAL((FILE *)4, process->write_process_stream_fh);
        LONGS_EQUAL(5, process->read_file_handle);
        LONGS_EQUAL(6, process->write_file_handle);
    }

    process = find_in_pid_list((FILE *)3);
    CHECK_TRUE(process != NULL);
    if (process != NULL)
    {
        LONGS_EQUAL(2, process->child_pid);
    }
    // now remove and check its gone.
    free_from_pid_list(process);
    process = find_in_pid_list((FILE *)3);
    POINTERS_EQUAL((FILE *)0, process);
}


TEST(tlr_popen, multiple_adding_into_pid_list)
{
    int const  	pid_list[] =   { 3, 6, 8, 12, 4 };
    int 	idx;
    int const 	list_len = sizeof pid_list;
    process_id_st * process;

    for (idx = 0; idx < list_len; idx++)
    {
        add_into_pid_list(pid_list[idx], (FILE *)(pid_list[idx] + 20L), NULL, 0, 0);
    }
    for (idx = 0; idx < list_len; idx++)
    {
        process = find_in_pid_list((FILE *)(pid_list[idx] + 20L));
        CHECK_TRUE(process != NULL);
        if (process != NULL)
        {
            LONGS_EQUAL(pid_list[idx], process->child_pid);
        }
    }
    for (idx = list_len - 1; idx >= 0; idx--)
    {
        process = find_in_pid_list((FILE *)(pid_list[idx] + 20L));
        CHECK_TRUE(process != NULL);
        if (process != NULL)
        {
            LONGS_EQUAL(pid_list[idx], process->child_pid);
        }
    }
    for (idx = 0; idx < list_len; idx++)
    {
        process = find_in_pid_list((FILE *)(pid_list[idx] + 20L));
        CHECK_TRUE(process != NULL);
        free_from_pid_list(process);
    }
    for (idx = 0; idx < list_len; idx++)
    {
        process = find_in_pid_list((FILE *)(pid_list[idx] + 20L));
        CHECK_TRUE(process == NULL);
    }

}

static char    const test_str[] = "Now is the time for all good men to come to the aid of the party.\n";

void test_popen(void)
{
    char 	const * const cmd = "cat popen_test.txt";
    FILE 	* process;
    size_t 	size = 0;
    char    buf[sizeof test_str ];
    int     line_count = 0;
    int const lines_2_check = 6;
    int     pclose_result;

    process = tlr_popen(cmd, "r");
    CHECK_TRUE(process != NULL);
    if (process != NULL)
    {
        do
        {
            size =  fread(buf, 1, sizeof buf - 1, process);
            CHECK_TRUE(size > 0);

            if (size > 0)
            {
                buf[size] = '\0';
                STRCMP_EQUAL(test_str, buf);
            }
            line_count++;
        }
        while (size > 0 && line_count < lines_2_check);

        pclose_result = tlr_pclose(process);
        CHECK_TRUE(pclose_result == 0);
    }
}


void verify_output_file(char const * const fname, int const lines_expected)
{
    int     file_handle;
    char    buf[sizeof test_str + 1];
    ssize_t bytes;
    int     got_lines = 0;

    file_handle = open(fname, 0);
    if (file_handle != -1)
    {
        while ((bytes = read(file_handle, buf, sizeof buf - 1)) > 0)
        {
            buf[bytes] = '\0';
            BYTES_EQUAL('A' + got_lines, buf[0]);
            STRCMP_EQUAL(test_str, buf + 1);
            got_lines++;
        }
        close(file_handle);
    }
    LONGS_EQUAL(got_lines, lines_expected);
}


void test_popen_w(void)
{
    char 	const * const cmd = "sort -o t.txt";
    FILE 	* process;
    size_t 	size = 0;
    char    buf[sizeof test_str + 2 ];
    int     lines_2_write = 6;
    int     pclose_result;

    process = tlr_popen(cmd, "w");
    CHECK_TRUE(process != NULL);
    if (process != NULL)
    {
        do
        {
            buf[0] = 'A' + lines_2_write-1;
            strcpy(&buf[1], test_str);

            size =  fwrite(buf, 1, strlen(buf), process);
            CHECK_TRUE(size > 0);
            lines_2_write--;
        }
        while (size > 0 && lines_2_write > 0);

        pclose_result = tlr_pclose(process);
        CHECK_TRUE(pclose_result == 0);
    }
    verify_output_file("t.txt", 6);
}

void test_popen_fail(void)
{
    char 	const * const cmd = "blahblah";
    FILE 	* process;
    int     pclose_result;

    process = tlr_popen(cmd, "r");
    CHECK_TRUE(process != NULL);
    if (process != NULL)
    {
        pclose_result = tlr_pclose(process);
        CHECK_TRUE(pclose_result != 0);
    }
}


TEST(tlr_popen, overall_popen_test)
{
    test_popen();
    test_popen_w();
    test_popen_fail();
}


void check_args(char ** cmd_args, char const **expect)
{
    int     idx;
    int     expected_arg_count = 0;
    while (*(expect + expected_arg_count) != NULL)
    {
        expected_arg_count++;
    }
    for (idx = 0; idx < expected_arg_count; idx++)
    {
        STRCMP_EQUAL(*(expect+idx), *(cmd_args+idx));
    }
    CHECK_TRUE(*(cmd_args + expected_arg_count) == NULL);
}


void check_with_cmd(char const * test_cmd, char const **expect)
{
    bool    result;
    char 	** cmd_args;

    result = make_args_list(test_cmd, &cmd_args);
    CHECK_TRUE(result);
    if (result)
    {
        check_args(cmd_args, expect);
        free_args_list(cmd_args);
    }
}

TEST(tlr_popen, make_args_list_test)
{
    char    const * expectation[] = { "hello", "world", "and", "everyone", NULL };

    check_with_cmd("hello world and everyone", expectation);
    check_with_cmd("hello  world and everyone", expectation);
    check_with_cmd("hello  world       and             everyone", expectation);
    check_with_cmd("              hello world and everyone", expectation);
    check_with_cmd("              hello     world  and         everyone", expectation);
    check_with_cmd("hello world and everyone ", expectation);
    check_with_cmd("hello world and everyone    ", expectation);
}

TEST(tlr_popen, make_args_small_words_test)
{
    char    const * expectation[] = { "a", "b", "c", "d", NULL };

    check_with_cmd("a b c d", expectation);
    check_with_cmd("a b           c d", expectation);
    check_with_cmd("      a    b c d", expectation);
    check_with_cmd("      a    b c d ", expectation);
    check_with_cmd("      a    b c d   ", expectation);
}


TEST(tlr_popen, make_args_quotes_test)
{
    char    const * expectation[] = { "hello", "there", NULL };
    char    const * expectation2[] = { "hello", "how    are    you", NULL };
    char    const * expectation3[] = { "hello", "   how    are    you", NULL };
    char    const * expectation4[] = { "hello", "   how    are    you   ", NULL };
   char    const * expectation5[] = { "how    are    you", NULL };
    char    const * expectation6[] = { "   how    are    you", NULL };
    char    const * expectation7[] = { "   how    are    you   ", NULL };
    char    const * expectation8[] = { "a", "b", "c", "d", NULL };

    check_with_cmd ("hello there", expectation);
    check_with_cmd ("\"hello\"    there", expectation);
    check_with_cmd ("hello    \"there\"", expectation);
    check_with_cmd ("\"hello\" \"there\"", expectation);

    check_with_cmd ("\"hello\" \"how    are    you\"", expectation2);
    check_with_cmd ("\"hello\" \"   how    are    you\"", expectation3);
    check_with_cmd ("\"hello\" \"   how    are    you   \"", expectation4);

    check_with_cmd ("\"how    are    you\"", expectation5);
    check_with_cmd ("\"   how    are    you\"", expectation6);
    check_with_cmd ("\"   how    are    you   \"", expectation7);

    check_with_cmd ("\"a\" \"b\" \"c\" \"d\"", expectation8);
    check_with_cmd ("\"a\"\"b\"\"c\"\"d\"", expectation8);
}


TEST(tlr_popen, make_args_quotes_errors_test)
{
    char    const * expectation[] = { "noendquote", NULL };
    char    const * expectation2[] = { "noendquote   ", NULL };
    char    const * expectation3[] = { "gidday   ", "ab ", "cd", NULL };
    char    const * expectation4[] = { "noendquote   gidday   ", NULL };

    check_with_cmd ("\"noendquote", expectation);
    check_with_cmd ("\"noendquote   ", expectation2);
    check_with_cmd ("\"gidday   \" \"ab \"  \"cd\"", expectation3);
    check_with_cmd ("\"gidday   \" \"ab \"  cd", expectation3);
    check_with_cmd ("\"noendquote   gidday   ", expectation4);
}



TEST(tlr_popen, make_argslist_empty_checks)
{
    bool    result;
    char 	** cmd_args;

    result = make_args_list("", &cmd_args);
    CHECK_TRUE(result);
    if (result)
    {
        STRCMP_EQUAL("", *cmd_args);
        CHECK_TRUE(*(cmd_args + 1) == NULL);
        free_args_list(cmd_args);
    }

    result = make_args_list("            ", &cmd_args);
    CHECK_TRUE(result);
    if (result)
    {
        STRCMP_EQUAL("", *cmd_args);
        CHECK_TRUE(*(cmd_args + 1) == NULL);
        free_args_list(cmd_args);
    }

    result = make_args_list(NULL, &cmd_args);
    CHECK_FALSE(result);
}


