#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tlr_print.h"
#include "show_parse_helper.h"

int parse_show_options(int const argc, char * const *argv, tlr_response_format_t *format, int *instance, int const min_instance, int const max_instance)
{
    optind = 1;
    opterr = 0;

    int ret = -1;
    int opt = 0;

    *format = tlr_response_format_ascii;   /* default to ascii format if not specified on the command line */
    *instance = min_instance;     /* default to first instance if not specified on the command line */
    while ((opt = getopt(argc, argv, "f:i:")) != -1) {
        switch (opt) {
            case 'f':
                if (strcmp(optarg, TLR_FORMAT_JSON) == 0)
                    *format = tlr_response_format_json;
                else if (strcmp(optarg, TLR_FORMAT_ASCII) == 0);
                else {
                    fprintf(stderr, "Unsupported format: '%s'\n", optarg);
                    goto error;
                }
                break;
            case 'i':
                *instance = strtol(optarg, NULL, 10);
                if (* instance < min_instance || * instance > max_instance) {
                    fprintf(stderr, "instance out of range");
                    goto error;
                }
                break;
            default:
                tlr_debug("Unsupported show option: '%c'\n", opt);
                goto error;
        }
    }
    ret = 0;

error:
    return ret;
}

void usage_show(char const * const progname)
{
    fprintf(stderr, "Usage: %s [-i <instance>] [-f <json|ascii>]\n", progname);
}
