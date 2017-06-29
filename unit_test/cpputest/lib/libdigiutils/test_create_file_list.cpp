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
    #include "create_file_list.h"
    #include <stddef.h>
    #include "strlcpy.h"
    #include "digiutils.h"
    #include <fcntl.h>
    #include <stdio.h>
    #include <string.h>
}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdint.h>

ssize_t getline(char ** lineptr, size_t * n, FILE * stream)
{
    mock().actualCall("getline").withOutputParameter("lineptr", lineptr).withOutputParameter("n", n);

    return mock().longIntReturnValue();
}


TEST_GROUP(tlr_create_file_list)
{

    void setup()
    {

    }

    void teardown()
    {

    }
};

TEST(tlr_create_file_list, tlr_get_remote_firmware_filenames_as_xml_returns_correct_list)
{
    char *actual_result;
    char *getline_buffer;
    char good_html_buffer[] = "<html><head><title>ftp1.digi.com - /support/firmware/branded/sieH7/latest/</title></head><body><H1>ftp1.digi.com - /support/firmware/branded/sieH7/latest/</H1><hr>\r\n <pre><A HREF=\"/support/firmware/branded/sieH7/\">[To Parent Directory]</A><br><br> 3/14/2016 9:48 AM 22643547 <A HREF=\"/support/firmware/branded/sieH7/latest/lr54-0.0.0.1.bin\">lr54-0.0.0.1.bin</A><br> 3/16/2016 8:05 AM 22643905 <A HREF=\"/support/firmware/branded/sieH7/latest/lr54-0.0.0.2.bin\">lr54-0.0.0.2.bin</A><br> 3/16/2016 10:23 AM 22643907 <A HREF=\"/support/firmware/branded/sieH7/latest/lr54-1.2.3.4.bin\">lr54-1.2.3.4.bin</A><br></pre><hr></body></html>";
    char expected_result[] = "<file index=\"1\"><name>lr54-0.0.0.1.bin</name><size>22643547</size><date>3/14/2016</date><version>0.0.0.1</version></file><file index=\"2\"><name>lr54-0.0.0.2.bin</name><size>22643905</size><date>3/16/2016</date><version>0.0.0.2</version></file><file index=\"3\"><name>lr54-1.2.3.4.bin</name><size>22643907</size><date>3/16/2016</date><version>1.2.3.4</version></file>";
    size_t size = sizeof good_html_buffer;

    // Since we are using getline we need to malloc the data.
    getline_buffer = (char *)malloc(sizeof good_html_buffer+1);
    if (getline_buffer == NULL)
    {
        FAIL("malloc failed\n");
    }
    memcpy(getline_buffer, good_html_buffer, size);
    getline_buffer[size] = 0x0;

    mock().expectOneCall("getline")
            .withOutputParameterReturning("lineptr", &getline_buffer, sizeof(char *))
            .withOutputParameterReturning("n", &size, sizeof size)
            .andReturnValue(1);


    actual_result = tlr_get_remote_firmware_filenames_as_xml();

    STRCMP_EQUAL(expected_result, actual_result);

    free(actual_result);

}

TEST(tlr_create_file_list, tlr_get_remote_firmware_filenames_as_xml_returns_error_when_getline_fails)
{
    char *actual_result;
    char *getline_buffer;
    char good_html_buffer[] = "not used";
    char *expected_result = NULL;
    size_t size = sizeof good_html_buffer;

    // Since we are using getline we need to malloc the data.
    getline_buffer = (char *)malloc(sizeof good_html_buffer+1);
    if (getline_buffer == NULL)
    {
        FAIL("malloc failed\n");
    }
    memcpy(getline_buffer, good_html_buffer, size);
    getline_buffer[size] = 0x0;

    mock().expectOneCall("getline")
            .withOutputParameterReturning("lineptr", &getline_buffer, sizeof(char *))
            .withOutputParameterReturning("n", &size, sizeof size)
            .andReturnValue(0);

    actual_result = tlr_get_remote_firmware_filenames_as_xml();

    POINTERS_EQUAL(expected_result, actual_result);

}

TEST(tlr_create_file_list, tlr_get_remote_firmware_filenames_as_xml_returns_null_when_passed_bad_html)
{
    char *actual_result;
    char *getline_buffer;
    char bad_html_buffer[] = "<html><head><title>ftp1.digi.com - /support/firmware/branded/sieH7/latest/</title></head><body><H1>ftp1.digi.com - /support/firmware/branded/sieH7/latest/</H1><hr>\r\n <pre><A HREF=\"/support/firmware/branded/sieH7/\">[To Parent Directory]</A> 3/14/2016 9:48 AM 22643547 <A HREF=\"/support/firmware/branded/sieH7/latest/lr54-0.0.0.1.bin\">lr54-0.0.0.1.bin</A><br> 3/16/2016 8:05 AM 22643905 <A HREF=\"/support/firmware/branded/sieH7/latest/lr54-0.0.0.2.bin\">lr54-0.0.0.2.bin</A><br> 3/16/2016 10:23 AM 22643907 <A HREF=\"/support/firmware/branded/sieH7/latest/lr54-1.2.3.4.bin\">lr54-1.2.3.4.bin</A><br></pre><hr></body></html>";
    char *expected_result = NULL;
    size_t size = sizeof bad_html_buffer;

    // Since we are using getline we need to malloc the data.
    getline_buffer = (char *)malloc(sizeof bad_html_buffer+1);
    if (getline_buffer == NULL)
    {
        FAIL("malloc failed\n");
    }
    memcpy(getline_buffer, bad_html_buffer, size);
    getline_buffer[size] = 0x0;

    mock().expectOneCall("getline")
            .withOutputParameterReturning("lineptr", &getline_buffer, sizeof(char *))
            .withOutputParameterReturning("n", &size, sizeof size)
            .andReturnValue(1);

    actual_result = tlr_get_remote_firmware_filenames_as_xml();

    POINTERS_EQUAL(expected_result, actual_result);
}









