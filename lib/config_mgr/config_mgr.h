/**
 * Copyright (c) 2011, 2013, 2015 Digi International Inc. All rights reserved
 *
 * config_mgr.h
 */

#ifndef _CONFIG_MGR_H_
#define _CONFIG_MGR_H_

#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "config_table.h"
#include "config_status.h"

#define MAX_USERNAME_LEN    33  // 32 + a null-terminator

#define UCI_DEFAULT_PATH        "/usr/config"
#define UCI_MODIFIED_PATH       "/etc/config"
//#define UCI_DEFAULT_PATH        "../../lib/config_mgr/uci_default";
//#define UCI_MODIFIED_PATH       "../../lib/config_mgr/uci_modified";
#define BASE_DIGI_DIR			"/home/digi"
#define USER_DIR				BASE_DIGI_DIR "/user"
#define CONFIG_FILE_PTR 		BASE_DIGI_DIR "/configfile"
#define DEFAULT_CONFIG_DA0_FILE USER_DIR "/config.da0"
#define CONFIG_PATH     		USER_DIR "/"

#define DESTDIR_STR     MACRO_TO_STRING(DESTDIR)

// Tags used in config.da0 to indicate the firewall section
#define FIREWALL_TAG "[FIREWALL]"
#define FIREWALL_END_TAG "[FIREWALL_END]"
#define MAX_FIREWALL_RULE 255

#define TLR_PLATFORM        "LR54DWC1"

typedef enum {
    config_mgr_access_level_read_only = config_user_access_read_only,
    config_mgr_access_level_read_write = config_user_access_read_write,
    config_mgr_access_level_super = config_user_access_super,
} config_mgr_access_level_t;

typedef enum
{
    config_mgr_table_type_config,
    config_mgr_table_type_show,
    config_mgr_table_type_action
} config_mgr_table_type_t;

typedef enum
{
    config_mgr_cmd_get_config_group,
    config_mgr_cmd_get_config_element,
    config_mgr_cmd_set_config_element,
    config_mgr_cmd_have_value,
    config_mgr_cmd_get_error,
    config_mgr_cmd_apply,
    config_mgr_cmd_save,
    config_mgr_cmd_get_table,
    config_mgr_cmd_have_group,
    config_mgr_cmd_have_element,
    config_mgr_cmd_have_enum,
    config_mgr_cmd_have_action,
    config_mgr_cmd_have_action_element,
    config_mgr_cmd_have_param,
    config_mgr_cmd_action_state_group,
    config_mgr_cmd_action_state_element,
    config_mgr_cmd_action_element,
    config_mgr_cmd_action_parameter,
    config_mgr_cmd_COUNT
} config_mgr_commands_t;

#define WAIT_FOREVER (size_t)-1

typedef struct {
    /* count must be 1st field */
    size_t count;
    unsigned int value[1];
} config_mgr_enum_multi_value_t;

extern char const * const config_mgr_command_format[config_mgr_cmd_COUNT];

/*
 * Show APIs
 */
tlr_handle_t tlr_show_open(char const * const format, int const id, size_t const instance, char const * const parameter, config_mgr_access_level_t const access);
ssize_t tlr_show_read(tlr_handle_t const handle, char * const data, size_t const data_length, size_t const timeout_millisecs);
char * tlr_show_read_line_alloc(tlr_handle_t const handle, size_t const timeout_millisecs);
config_status_t tlr_show_close(tlr_handle_t const handle);
tlr_handle_t tlr_status_open(size_t const group_id, size_t const instance);
config_status_t tlr_status_read(tlr_handle_t const handle, size_t const element_id, void * const data, size_t const bytes);
config_status_t tlr_status_get_string_ptr(tlr_handle_t const handle, size_t const element_id, char const ** const string_ptr);
void tlr_status_close(tlr_handle_t handle);

/*
 * Show object APIs
 */
typedef struct tlr_show_object_t tlr_show_object_t;

tlr_show_object_t * tlr_show_object_create(size_t const group_id, size_t const instance);
void tlr_show_object_add_element(tlr_show_object_t * const show_object, size_t const element_id, void const * const value);
char const * tlr_show_object_to_json_string(tlr_show_object_t const * const show_object);
void tlr_show_object_destroy(tlr_show_object_t * const show_object);

/*
 * Action APIs
 */
tlr_handle_t tlr_action_open(char const * const format, int const id, char const * const user, char const * const parameter, config_mgr_access_level_t const access);
//ssize_t tlr_action_read(void * const handle, char const * data, size_t const data_length, size_t const timeout);
//int tlr_action_close(void * const handle);

#define tlr_action_read tlr_show_read
#define tlr_action_close tlr_show_close

/*
 * table APIs
 */
tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access);
config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes);
config_status_t tlr_config_write(tlr_handle_t const handle, size_t const element_id, char const * const data);
config_status_t tlr_config_read_native(tlr_handle_t const handle, size_t const element_id, void * const data);
config_status_t tlr_config_write_native(tlr_handle_t const handle, size_t const element_id, void const * const value);
void tlr_config_close(tlr_handle_t const handle);
char const * tlr_config_get_error(config_status_t const status);

tlr_handle_t tlr_apply_open(size_t const id, size_t const instance, char const * const user, config_mgr_access_level_t const access);
config_status_t tlr_apply_read(tlr_handle_t const handle, char * const data, size_t const data_length, size_t const timeout);
int tlr_apply_close(tlr_handle_t const handle);

config_status_t tlr_run_all_apply_processes(void);

/*
 * Configuration group APIs
 */
void const * tlr_get_table(config_mgr_table_type_t const type);
config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index);
config_status_t tlr_config_string_to_value(size_t const group_id, size_t const element_id, char const * const buffer, void * const value);
size_t tlr_get_num_groups(config_mgr_group_type_t const table_index);

ssize_t tlr_get_group_id(char const * const group_name);
ssize_t tlr_get_show_group_id(char const * const group_name);
ssize_t tlr_get_element_id(size_t const group_id, char const * const element_name);
config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access);

config_status_t tlr_set_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      char const * const value,
                                      config_mgr_access_level_t const access);

platform_type_t tlr_get_platform(void);
char const * tlr_get_platform_name(void);

bool tlr_user_is_authorized(char const * const username, char const * const password, config_mgr_access_level_t * level, bool const user_is_logged_in);
char const * tlr_user_show_password(config_mgr_element_type_t type, char const * const password);


void tlr_update_table_tag(config_tag_t const tag, unsigned int value);
config_status_t tlr_set_uci_tag(config_tag_t const tag_id, bool const enable_tag);
config_status_t tlr_get_uci_tag(config_tag_t const tag_id, bool * const enable_tag);


void replace_char(char const * const string_src, char * const string_dest, size_t const length, char src, char dest);
void tlr_get_config_file(char *cfg_file, int maxlen);
bool tlr_get_salted_pwd(char const * const pwd, char const * const salt, char *salted_pwd, int maxlen);

size_t tlr_get_element_size(config_mgr_element_t const * const element);
size_t tlr_get_config_value_max_length(config_mgr_table_type_t const type, size_t const config_id, size_t const element_id);
size_t enum_max_name_length(config_mgr_element_enum_limit_t const * const enum_limit);
config_mgr_element_enum_limit_t const * tlr_get_element_enum_limit(config_mgr_table_type_t const type, size_t const group_id, size_t const element_id);

config_status_t tlr_get_group_instance(size_t const group_id,
                                       size_t const element_id,
                                       char const * const value,
                                       size_t const value_size,
                                       size_t * const instance);

config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void ** value,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access);

config_status_t tlr_config_read_alloc_value(tlr_handle_t const handle, size_t const element_id, void * * data, config_mgr_element_type_t const type);

void tlr_log_set_event(size_t const group_id,
                       size_t const instance,
                       size_t const element_id,
                       char const * const value,
                       char const * const user,
                       bool const success);
bool tlr_apply_group(size_t const group_id, size_t const instance, char const * const user, config_mgr_access_level_t const access);

config_status_t tlr_get_last_config(size_t const group_id, size_t const instance, void * const ptr, size_t const size);
config_status_t tlr_set_last_config(size_t const group_id, size_t const instance, void const * const ptr, size_t const size);
size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type);

unsigned int tlr_get_group_loglevel(size_t const group_id);
bool tlr_is_json_supported(size_t const group_id);
#endif /* _CONFIG_MGR_H_ */
