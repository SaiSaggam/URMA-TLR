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

#include "show_route.h"
#include "show_parse_helper.h"
#include "config_mgr.h"

/********************
	Functions
********************/

int main (int argc, char *argv[]) {
	int ret = EXIT_FAILURE;
	tlr_response_format_t format;
	FILE * process = NULL;
	int instance;
    int max_instance = tlr_get_max_instance(config_state_route, config_mgr_table_type_show);

    if (max_instance <= 0)
        goto ERROR;

	if (parse_show_options(argc, argv, &format, &instance, FIRST_INSTANCE, max_instance))
		goto ERROR;
	process = popen("route -n", "r");
	if (process == NULL) {
		print_error("unable to open subprocess");
		goto ERROR;
	}
	if (show(FILE_NAME, process, format, instance))
		goto ERROR;
	ret = EXIT_SUCCESS;
	ERROR:
		if (process != NULL)
			pclose(process);
		return ret;
}

void print_error(char * message) {
	printf("Show Error: %s.\n", message);
}
void print_message(char * message) {
	printf("%s.\n", message);
}

void print_header(tlr_response_format_t const format) {
	if (format == tlr_response_format_ascii) {
		printf("%-20s%-15s%-10s%-15s%-10s%-10s%-10s\n", "Destination", "Gateway", "Metric", "Protocol", "Idx", "Interface", "Status");
		printf("--------------------------------------------------------------------------------------\n");
	}
}

void print_ascii(char * const route[]) {
	printf("%-20s", route[route_destination]);
	printf("%-15s", route[route_gateway]);
	printf("%-10s", route[route_metric]);
	printf("%-15s", route[route_protocol]);
	printf("%-10s", route[route_index]);
	printf("%-10s", route[route_interface]);
	printf("%-10s", route[route_status]);
	printf("\n");
}

void print_json(json_object * json_obj) {
	printf("%s", json_object_to_json_string(json_obj));
}
