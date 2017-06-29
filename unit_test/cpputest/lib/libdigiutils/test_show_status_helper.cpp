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
    #include "show_status_helper.h"

    #include <stdlib.h>
    #include <unistd.h>
}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

static int redirect_stdout(int new_fd)
{
    int const saved_stdout = dup(STDOUT_FILENO);

    dup2(new_fd, STDOUT_FILENO);

    return saved_stdout;
}

static void restore_stdout(int saved_stdout)
{
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}

static char * read_from_pipe(int fd)
{
    int const alloc_granularity = 4096;
    char * buf = NULL;
    size_t buf_size = 0;
    ssize_t bytes_read;
    int read_index = 0;

    do
    {
        int bytes_left_in_buffer = buf_size - read_index - 1;   // ensure there's room for terminating '\0'

        if (bytes_left_in_buffer <= 0)
        {
            buf_size += alloc_granularity;
            buf = (char *)realloc(buf, buf_size);
            bytes_left_in_buffer += alloc_granularity;
        }

        bytes_read = read(fd, buf + read_index, bytes_left_in_buffer);

        read_index += bytes_read;
    }
    while (bytes_read > 0);

    if (buf != NULL)
    {
        buf[read_index] = '\0';
    }

    return buf;
}

TEST_GROUP(show_status_helper)
{
	int pipe_fds[2];
    int saved_stdout;
    char const * expected_output;

    void setup()
    {
        pipe(pipe_fds);

        saved_stdout = redirect_stdout(pipe_fds[1]);

        close(pipe_fds[1]);
    }

    void compare_expected_and_actual_output(void)
    {
        char * actual_output = NULL;

        fflush(stdout);
        restore_stdout(saved_stdout);

        actual_output = read_from_pipe(pipe_fds[0]);

        STRCMP_EQUAL(expected_output, actual_output);

        free(actual_output);
    }

    void teardown()
    {
        compare_expected_and_actual_output();

        close(pipe_fds[0]);
    }
};

TEST(show_status_helper, test_printf_status_header_with_divider_width_auto)
{
    expected_output = " Test heading\n"
                      " ------------\n";

    printf_status_header(DIVIDER_WIDTH_AUTO, " Test heading\n");
}

TEST(show_status_helper, test_printf_status_header_with_divider_width_specified)
{
    expected_output = " Test heading\n"
                      " --------------------\n";

    printf_status_header(20, " Test heading\n");
}

TEST(show_status_helper, test_fprintf_status_header_with_divider_width_auto)
{
    expected_output = " Test heading\n"
                      " ------------\n";

    fprintf_status_header(stdout, DIVIDER_WIDTH_AUTO, " Test heading\n");
}

TEST(show_status_helper, test_fprintf_status_header_with_divider_width_specified)
{
    expected_output = " Test heading\n"
                      " --------------------\n";

    fprintf_status_header(stdout, 20, " Test heading\n");
}

