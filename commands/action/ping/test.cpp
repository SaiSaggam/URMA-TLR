#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"
#include <CppUTest/CommandLineTestRunner.h>

int digi_ping_main(int argc, char **argv);

FILE *mock_file = (FILE *) "mocked";

FILE *popen(const char *command, const char *type)
{
    mock().actualCall("popen").withParameter("command", command).withParameter("type", type);
    return mock_file;
}

int pclose(FILE *stream)
{
    mock().actualCall("pclose").withParameter("stream", stream);
    return 0;
}


char **return_argv(int num,...)
{
    va_list valist;
    static char *argv[21];
    int i;
    
    if (num > 20) {
        fprintf(stderr, "Too many arguments on command line...");
        exit(1);
    }
    /* initialize valist for num number of arguments */
    va_start(valist, num);
    
    /* access all the arguments assigned to valist */
    for (i = 0; i < num; i++)
    {
        argv[i] = va_arg(valist, char *);
    }
    argv[num] = NULL;
    /* clean memory reserved for valist */
    va_end(valist);
    for (i = 0; i < num; i++) {
        printf("argv[%d] = %s ", i, argv[i]);
    }
    printf("\n");
    return argv;
}

TEST_GROUP(alpha)
{
    void teardown()
    {
        mock().clear();
    }
};
TEST(alpha, json1)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 10 -s 56 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, json2)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 10 -s 56 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "1", "192.168.1.1"));
    mock().checkExpectations();
}

TEST(alpha, help1)
{
    digi_ping_main(2, return_argv(2, "action_ping", "-h"));
    mock().checkExpectations();
}
TEST(alpha, help2)
{
    digi_ping_main(2, return_argv(2, "action_ping", "-?"));
    mock().checkExpectations();
}

TEST(alpha, count1)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 100 -s 56 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "count", "100", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, count2)
{
    digi_ping_main(4, return_argv(4, "action_ping", "count", "0", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, count3)
{
    digi_ping_main(4, return_argv(4, "action_ping", "count", "3601", "192.168.1.1"));
    mock().checkExpectations();
}

TEST(alpha, count4)
{
    digi_ping_main(4, return_argv(4, "action_ping", "count", "10z", "192.168.1.1"));
    mock().checkExpectations();
}

TEST(alpha, size1)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 10 -s 100 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "size", "100", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, size2)
{
    digi_ping_main(4, return_argv(4, "action_ping", "size", "-1", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, size3)
{
    digi_ping_main(4, return_argv(4, "action_ping", "size", "8193", "192.168.1.1"));
    mock().checkExpectations();
}

TEST(alpha, size4)
{
    digi_ping_main(4, return_argv(4, "action_ping", "size", "10z", "192.168.1.1"));
    mock().checkExpectations();
}


TEST(alpha, interface)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 10 -s 56 -I eth0 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "interface", "eth0", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, interface2)
{
    digi_ping_main(4, return_argv(4, "action_ping", "interface", "junk4", "192.168.1.1"));
    mock().checkExpectations();
}

TEST(alpha, multiple1)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 99 -s 101 -I eth0 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(8, return_argv(8, "action_ping", "interface", "eth0", "count", "99", "size", "101", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, multiple2)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 3000 -s 44 -I eth0 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(8, return_argv(8, "action_ping", "size", "44", "count", "3000", "interface", "eth0", "192.168.1.1"));
    mock().checkExpectations();
}

TEST(alpha, ip1)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 10 -s 56 192.168.1.1 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "192.168.1.1"));
    mock().checkExpectations();
}
TEST(alpha, ip2)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 10 -s 56 google.com 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "google.com"));
    mock().checkExpectations();
}
TEST(alpha, ip3)
{
    mock().expectOneCall("popen").withParameter("command", "ping -c 10 -s 56 goo-gle.com 1>&2").withParameter("type", "w");
    mock().expectOneCall("pclose").withParameter("stream", mock_file);
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "goo-gle.com"));
    mock().checkExpectations();
}
TEST(alpha, ip4)
{
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "-google.com"));
    mock().checkExpectations();
}
TEST(alpha, ip5)
{
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "google-.com"));
    mock().checkExpectations();
}
TEST(alpha, ip6)
{
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "a.-google.com"));
    mock().checkExpectations();
}
TEST(alpha, ip7)
{
    digi_ping_main(4, return_argv(4, "action_ping", "-f", "0", "a.#google.com"));
    mock().checkExpectations();
}

TEST(alpha, unknown_opt)
{
    digi_ping_main(6, return_argv(6, "action_ping", "-f", "0", "unknown", "10", "192.168.1.1"));
    mock().checkExpectations();
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}



