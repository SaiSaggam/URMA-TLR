//
// Shell script helper to get ODM info
//
//

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "odm.h"

#define PROGRAM		"getodm"
#define VERSION		"v1.0"

#define MAC_INDEX_MAX	( MAX_NUM_OF_MAC - 1 )

const struct option long_options[] = {
	{ "version",	no_argument,		NULL, 'v' },
	{ "help",	no_argument,		NULL, 'h' },
	{ "mac",	required_argument,	NULL, 'm' },
	{ 0, 0, 0, 0 },
};

static void show_usage_and_exit(int exitval)
{
	fprintf(exitval ? stderr : stdout,
		"Usage: %s [-hv] [-m index]\n"					\
		"%s %s\n\n"							\
		"Utility to get ODM information\n"				\
		"\n"								\
		"   -m  --mac=index      Get MAC address (index = 0..%d)\n"	\
		"   -h  --help           Print help and exit\n"			\
		"   -v  --version        Print version and exit\n"		\
		"\n", PROGRAM, PROGRAM, VERSION, MAC_INDEX_MAX);
	exit(exitval);
}

int main(int argc, char **argv)
{
	int opt;
	int opt_num = 0;
	int mac_index = -1;
	int ret;
	unsigned char buf[MAX_LEN];

	while ( (opt = getopt_long(argc, argv, "m:vh", long_options, 0)) != EOF ) {
		switch ( opt ) {
		case 'm':
			mac_index = atoi(optarg);
			/* Check MAC index */
			if ( mac_index < 0 || mac_index > MAC_INDEX_MAX ) {
				fprintf(stderr, "Invalid MAC index, should be between (0..%d)\n", MAC_INDEX_MAX);

				exit(EXIT_FAILURE);
			}
			break;

		case 'v':
			printf("%s %s\n", PROGRAM, VERSION);
			exit(EXIT_SUCCESS);

		case 'h':
			show_usage_and_exit(EXIT_SUCCESS);

		default:
			show_usage_and_exit(EXIT_FAILURE);
		}

		++opt_num;
	}

	if ( opt_num == 0 ) {
		show_usage_and_exit(EXIT_FAILURE);
	}

	/* Get MAC address */
	if ( mac_index >= 0 ) {
		memset(buf, 0, sizeof(buf));

		ret = get_valid_MAC(mac_index, buf, 6);
		if ( ret <= 0 ) {
			fprintf(stderr, "Couldn't get MAC[%d] address\n", mac_index);

			exit(EXIT_FAILURE);
		}

		printf("MAC[%d]: %02X:%02X:%02X:%02X:%02X:%02X\n", mac_index,
		       buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
	}

	return 0;
}
