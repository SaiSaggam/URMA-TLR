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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <ctype.h>
#include <linux/limits.h>


unsigned int parse_size_option(char * const option, char * const value, int * const want_help)
{
    #define MIN_SIZE        (0)
    #define MAX_SIZE        (8192) /* max value allowed by Busybox ping*/
    long int size = 0;
    char *endp = NULL;
    unsigned int result = 0;

    size = strtol(value, &endp, 10);
    if (*endp == 0) {
        if ((size >= MIN_SIZE) && (size <= MAX_SIZE)) {
            result = (unsigned int) size;
        } else {
            fprintf(stderr, "Ping size must be between %d and %d bytes.\n", MIN_SIZE, MAX_SIZE);
            *want_help = 1;
        }
    } else {
        *want_help = 1;
    }

    return result;
}

unsigned int parse_count_option(char * const option, char * const value, int * const want_help)
{
    #define MIN_COUNT           (1)
    #define MAX_COUNT           (3600)
    long int count = 0;
    char *endp = NULL;
    unsigned int result = 0;

    count = strtol(value, &endp, 10);
    if (*endp == 0) {
        if ((count >= MIN_COUNT) && (count <= MAX_COUNT)) {
            result = (unsigned int) count;
        } else {
            fprintf(stderr, "Ping count must be between %d and %d.\n", MIN_COUNT, MAX_COUNT);
            *want_help = 1;
        }
    } else {
        *want_help = 1;
    }

    return result;
}


void parse_interface_option(char * const option, char * const value, char * const interface, unsigned int size, int * const want_help)
{
    if (if_nametoindex(value) > 0) {    /* if a valid network interface */
        strncpy(interface, value, size);
        interface[size - 1] = 0;
    } else {
        fprintf(stderr, "Invalid interface %s\n", value);
        *want_help = 1;
    }
}


/*
 * Destination must be a dotted IP address, or a DNS name.  A DNS name consists of one or more
 * labels seperated by dots.  Labels can have digits, letters, and hyphens, but cannot start or
 * end with a hyphen.
 */
int check_destination(char const * const dest)
{
    unsigned int i;
    char last_char = 0;
    char const hyphen = '-';
    char const dot = '.';
    int failure = ((dest[0] == hyphen) || (dest[0] == dot));   /* DNS name can't start with a dot or hyphen*/

    if (failure) printf("DNS name started with hyphen\n");
    for (i = 0; (!failure) && (i < strlen(dest)); i++) {
        char this_char = dest[i];

        if ((!isalpha(this_char)) && (!isdigit(this_char)) && (this_char != dot) && (this_char != hyphen)) {
            failure = 1;    /* illegal char */
        }
        if ((last_char == dot) && (this_char == hyphen)) {
            failure = 1;    /* label started with a hyphen */
        }
        if ((last_char == hyphen) && (this_char == dot)) {
            failure = 1;    /* label ended with a hyphen */
        }
        last_char = this_char;
    }

    return ((failure) || (last_char == dot) || (last_char == hyphen)); /* DNS name can't end with a dot or hyphen*/
}


void parse_command_line(int argc, char **argv, int *want_json, int *want_help, unsigned int *count, char * const interface, const unsigned int interface_size,
                        unsigned int *size, char * const destination, const unsigned int destination_size)
{
    char c;
    int next_arg = 0;

    *want_json = 0;
    *want_help = 0;

    optind = 0;
    while ((c = getopt (argc, argv, "hf:u:")) != -1) {
        switch (c) {
            case 'f':
                *want_json = (strcmp(optarg, "json") == 0);
                break;
            case 'u':
            	/* user */
            	break;
            case 'h':
            case '?':
                *want_help = 1;
        }
    }
    next_arg = optind;
    while ((!*want_help) && ((next_arg + 1) < argc)) {
        char *option = argv[next_arg++];
        char *value = argv[next_arg++];

        if (strcmp("count", option) == 0) {
            *count = parse_count_option(option, value, want_help);
        } else if (strcmp("interface", option) == 0) {
            parse_interface_option(option, value, interface, interface_size, want_help);
        } else if (strcmp("size", option) == 0) {
           *size = parse_size_option(option, value, want_help);
        } else {
            *want_help = 1;
        }
    }
    if ((!*want_help) && (next_arg == (argc - 1))) {      /* dest is last item on command line*/
        strncpy(destination, argv[next_arg], destination_size);
        destination[destination_size-1] = 0;
        *want_help = check_destination(destination);
    } else {
        *want_help = 1;
    }
}


int digi_ping_main(int argc, char **argv)
{
    int want_json = 0;
    int want_help = 0;
    FILE *output = NULL;
    char buffer[2*PATH_MAX+1];
    unsigned int count = 10, size = 56;
    char interface[PATH_MAX] = "", destination[PATH_MAX];

    parse_command_line(argc, argv, &want_json, &want_help, &count, interface, sizeof(interface), &size, destination, sizeof(destination));
    if (want_help) {
        fprintf(stderr, "Usage:  ping [count n] [interface ifname] [size bytes] destination\n");
        goto fn_exit;
    }
	// XXX it appears that we're redirecting the output to stderr so as to avoid CLI pagination from interfering
	// with long responses.
    if (interface[0] == 0) {
        snprintf(buffer, sizeof(buffer), "ping -c %u -s %u %s 1>&2", count, size, destination);
    } else {
        snprintf(buffer, sizeof(buffer), "ping -c %u -s %u -I %s %s 1>&2", count, size, interface, destination);
    }
    output = popen(buffer, "w");
    pclose(output);

fn_exit:
    return 0;
}
