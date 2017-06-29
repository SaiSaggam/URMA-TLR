/*****************************************************************************
 * Copyright (c) 2013 Digi International Inc., All Rights Reserved
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

#ifndef CONFIG_TREE_H_
#define CONFIG_TREE_H_

#include <stdlib.h>
#include "config_table.h"


#ifdef UNIT_TESTING
void ipm_call_free(void * ptr);
void config_tree_free(void * ptr);
void * config_tree_malloc(size_t size);
void * config_tree_strdup(void * const ptr);
void config_tree_free_strdup(void * ptr);

#else
#define config_tree_free_strdup free
#define config_tree_strdup   strdup
#define config_tree_free     free
#define config_tree_malloc   malloc
#define ipm_call_free            free
#endif

typedef enum {
    config_tree_success,
    config_tree_error_resource,
    config_tree_error_memory,
    config_tree_error_invalid_action,
    config_tree_error_invalid_group,
    config_tree_error_invalid_param,
    config_tree_error_invalid_value,
    config_tree_error_invalid_instance,
    config_tree_error_access,
    config_tree_error_ipm
} config_tree_status_t;

typedef enum {
    cli_tree_type_command,
    cli_tree_type_show,
    cli_tree_type_COUNT
} cli_tree_type_t;

typedef enum {
    cli_tree_data_type_group,
    cli_tree_data_type_element,
    cli_tree_data_type_enum,
    cli_tree_data_type_action,
    cli_tree_data_type_show,
    cli_tree_data_type_action_element,
    cli_tree_data_type_COUNT
} cli_tree_data_type_t;

typedef struct {
    char const * name;
    void * context;
    void * callback;
    void * data_tree;
    void * search_tree;
    size_t max_name_length;
    size_t max_value_length;
    cli_tree_data_type_t type;
} cli_tree_search_t;


typedef struct {
  char const * name;
  void * data;
  cli_tree_data_type_t tree_data_type;
  void * search_tree;
  size_t max_name_length;
} cli_tree_data_t;

typedef struct {
    size_t action;

    cli_tree_data_t group;

    cli_tree_data_t element;

} config_tree_request_t;

/*
 * data_type: config_ipm_tree_data_type_group or config_ipm_tree_data_type_action
 * data: pointer to config_ipm_command_data_t
 */

typedef void (* config_tree_walk_tree_callback_t)(cli_tree_data_type_t const tree_type,
                                                 char const * walk_name,
                                                 void * const data_tree,
                                                 void * const context);

typedef void (* config_tree_group_callback_t)(size_t const group_id,
                                             size_t const index, char const * const element_name,
                                             char const * const value, void * const context);

typedef void (* config_tree_action_callback_t)(size_t const action_id, size_t const group_id,
                                              size_t const index, char const * const element_name,
                                              char const * const value, void * const context);

int cli_tree_init(config_mgr_access_level_t const access_level);
int cli_tree_destroy(void);
int cli_tree_walk(cli_tree_type_t const tree_type, char const * const name, config_tree_walk_tree_callback_t const tree_callback, void * const context);
int cli_tree_find(cli_tree_type_t tree_type, char const * name, void ** data, cli_tree_data_type_t * const data_type);
int cli_tree_find_subtree(void * const search_tree, char const * name, void ** data, cli_tree_data_type_t * const data_type);
int cli_tree_walk_subtree(void * search_tree, char const * name, config_tree_walk_tree_callback_t tree_callback, void * const context);

#endif /* CONFIG_TREE_H_ */
