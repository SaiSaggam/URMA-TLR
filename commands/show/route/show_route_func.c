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

/********************
	Functions
********************/

int show(char * filename, FILE * const process, tlr_response_format_t const format, int const instance) {
	int ret = -1;
	int num_rows = 0;
	char line[CMD_LEN];
	char ** values = calloc(sizeof(char *), value_count);
	char ** route  = calloc(sizeof(char *), route_count);
	json_object * parent_obj = json_object_new_object();

	// ignoring first two lines
	fgets(line, sizeof line, process);
	fgets(line, sizeof line, process);
	while (fgets(line, sizeof line, process) != NULL) {
		if (parse_route(line, values))
			goto ERROR;
		if (process_route(filename, route, values))
			goto ERROR;

        switch (format)
        {
            case tlr_response_format_ascii:
                if (num_rows++ == 0)
                    print_header(format);
                print_ascii(route);
                break;

            case tlr_response_format_json:
               build_json_route(route, parent_obj, num_rows++);
               break;

           default:
                fprintf(stderr, "Unsupported format: '%d'\n", format);
                goto ERROR;
        }

	}
	if (num_rows == 0)
		print_message("routing table is empty.");
	else if (format == tlr_response_format_json)
		print_json(parent_obj);

	ret = 0;
	ERROR:
		if (route != NULL) {
			for (int j = 0; j < route_count; j++)
				if (route[j] != NULL)
					free(route[j]);
			free(route);
		}
		if (values != NULL) {
			for (int i = 0; i < value_count; i++)
				if (values[i] != NULL)
					free(values[i]);
			free(values);
		}
		if (parent_obj != NULL)
			free(parent_obj);
		return ret;
}

int parse_route(char * const line, char * values[]) {
	int ret = -1;
	char * token;
	char * cp = line;
	cp[strlen(cp)-1]='\0';//remove the line feed from end of line
	int i = 0;

	while ((token = strsep(&cp, " ")) != NULL)
		if (*token != '\0') {
			values[i] = malloc(VAL_LEN * sizeof(char));
			strcpy(values[i], token);
			i++;
		}
	if (i == value_count) // a correct route entry should have 8 words in line.
		ret = 0;
	if (token != NULL)
		free(token);
	if (cp != NULL)
		free(cp);
	return ret;
}

int process_route(char * filename, char * route[], char * const values[]) {
	int ret = -1;

	for (int i = 0; i < route_count; i++)
		route[i] = calloc(sizeof(char), VAL_LEN);

	int cidr = 0;
	if (mask_to_cidr(&cidr, values[value_genmask]))
		goto ERROR;

    if (strcmp(values[value_destination], "0.0.0.0") == 0)
    {
        strcpy(route[route_destination], "default");
    }
    else
    {
        snprintf(route[route_destination], VAL_LEN, "%s/%d", values[value_destination], cidr); // 19 is 'sizeof 255.255.255.255/32'
    }

	strcpy(route[route_gateway], values[value_gateway]);
	strcpy(route[route_metric], values[value_metric]);
	strcpy(route[route_protocol], (strchr(values[value_flags], 'G') != NULL) ? "Static" : "Connected");

	int index = 0;
	get_route_index(filename, &index, values);
	if (index != -1)
		snprintf(route[route_index], sizeof "nn", "%d", index);

	strcpy(route[route_interface], values[value_iface]);
	strcpy(route[route_status], (strchr(values[value_flags], 'U') != NULL) ? "UP" : "DOWN");

	ret = 0;
	ERROR:
		return ret;
}

int mask_to_cidr(int * cidr, char * const mask) {
	int ret = -1;
	* cidr = 0;
	char * token, * cp;
	cp = strdup(mask);
	while ((token = strsep(&cp, ".")) != NULL) {
		ret = 0;
		int num = atoi(token);
		for (int j = 7; j >= 0; j--) {
			int k = num >> j;
			if (k & 1)
				(* cidr)++;
			else
				break;
		}
	}
	return ret;
}

int get_route_index(char * filename, int * index, char * const values[]) {
	int ret = -1;
	FILE * file = NULL;
	char line[CMD_LEN];
	char entry_expected[CMD_LEN];

	*index = -1;

	if (access(filename, R_OK)) {
		ret = 0;
		goto ERROR;
	}
	file = fopen(filename, "r");
	if (file == NULL)
		goto ERROR;


	// XXX string format must match the format specified in apply_route_func.c instance_add() (except for the trailing '\n')
	snprintf(entry_expected, sizeof entry_expected , "%s/%s via %s metric %s", values[value_destination], values[value_genmask], values[value_gateway], values[value_metric]);

	while (fgets ((char *) line, sizeof line, file) != NULL) {
		char const * pindex;
		char const * pentry;
		char * saveptr;

		/* first field is the index, and the rest of the line is the route */
		pindex = strtok_r(line, " ", &saveptr);
		pentry = strtok_r(NULL, "\n", &saveptr);
		if (pindex != NULL && pentry != NULL)
		{
			if (strcmp(entry_expected, pentry) == 0)
			{
				*index = atoi(pindex);
				break;
			}
		}

	}

	if (*index == -1)
		goto ERROR;
	ret = 0;

ERROR:
	if (file != NULL)
		fclose(file);
	return ret;
}

int build_json_route(char * const route[], json_object * parent_obj, int const i) {
	char instance[2];
	snprintf(instance, sizeof instance, "%d", i);
	json_object * json_route = json_object_new_object();
	json_object_object_add(parent_obj, instance, json_route);
	json_object_object_add(json_route, "Destination", json_object_new_string(route[route_destination]));
	json_object_object_add(json_route, "Gateway", json_object_new_string(route[route_gateway]));
	json_object_object_add(json_route, "Metric", json_object_new_string(route[route_metric]));
	json_object_object_add(json_route, "Protocol", json_object_new_string(route[route_protocol]));
	json_object_object_add(json_route, "Index", json_object_new_string(route[route_index]));
	json_object_object_add(json_route, "Interface", json_object_new_string(route[route_interface]));
	json_object_object_add(json_route, "Status", json_object_new_string(route[route_status]));
	return 0;
}
