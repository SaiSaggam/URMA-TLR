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

#ifndef apply_route_h
#define apply_route_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CMD_LEN 128
#define MIN_INSTANCE 1
#define MAX_INSTANCE 32

typedef enum {
	route_destination,
	route_mask,
	route_gateway,
	route_metric,
	route_count
} route_t;

int read_uci(config_route_t * const parameters, int const instance, char * values[], size_t const size);
int check_log_file(char * filename);
int instance_found(FILE * const file, int const instance, char * line, size_t const bytes);
int route_remove(char * const line);
int instance_remove(FILE * const file, int const instance, char * const filename);
int values_filled(char * const values[], size_t const size);
int route_add(int const instance, char * const values[]);
int instance_add(FILE * const file, int const instance, char * const values[]);
int run_command(char * command);
void print_error(char * message);

#endif /* apply_route_h */
