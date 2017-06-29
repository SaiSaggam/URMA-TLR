#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"
#include "eth_status_helper.h"


static char const *do_system(char const *cmd, char const *arg)
{
    char command[100];
    FILE *pipe;
    char *result = NULL;

    snprintf(command, sizeof command, "%s %s", cmd, arg);
    command[(sizeof command) - 1] = 0;
    pipe = popen(command, "r");
    if (pipe) {
        char *p;
        result = (char *) malloc(2048);
        if (fread(result, 1, 2047, pipe) < 1) {
            free(result);
            result = NULL;
        }
        pclose(pipe);
    }
    
    return result;
}

    
char const *ifconfig(char const *interface)
{
    char command[100];
    FILE *pipe;
    return do_system("ifconfig", interface);
}

char const *mii_tool(char const *interface)
{
    char command[100];
    FILE *pipe;
    return do_system("mii-tool", interface);
}

