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

#ifndef show_route_h
#define show_route_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json.h>
#include "show_parse_helper.h"

#define CMD_LEN 128
#define VAL_LEN 32
#define FILE_NAME "/var/log/route.log"

typedef enum {
	route_destination,
	route_gateway,
	route_metric,
	route_protocol,
	route_index,
	route_interface,
	route_status,
	route_count
} route_t;

typedef enum {
	value_destination,
	value_gateway,
	value_genmask,
	value_flags,
	value_metric,
	value_ref,
	value_use,
	value_iface,
	value_count
} value_t;

int get_format(int const argc, char * const argv[], tlr_response_format_t * format, int * instance);
int show(char * filename, FILE * const process, tlr_response_format_t const format, int const instance);
int parse_route(char * const line, char * values[]);
int process_route(char * filename, char * route[], char * const values[]);
int mask_to_cidr(int * cidr, char * const mask);
int get_route_index(char * filename, int * index, char * const values[]);
int build_json_route(char * const route[], json_object * parent_obj, int const i);
void print_error(char * message);
void print_message(char * message);
void print_header(tlr_response_format_t const format);
void print_ascii(char * const route[]);
void print_json(json_object * json_obj);

#endif /* show_route_h */
