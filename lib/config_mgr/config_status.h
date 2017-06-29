/**
 * Copyright (c) 2011, 2013 Digi International Inc. All rights reserved
 *
 * config_mgr.h
 */

#ifndef _CONFIG_STATUS_H_
#define _CONFIG_STATUS_H_

#include <stdio.h>
#include <syslog.h>

#include "config_table.h"

#ifndef ASSERT
#define ASSERT            assert
#endif

#define CONFIG_DEBUG    1

#define tlr_config_error(x...)     do { syslog(LOG_ERR, x); } while (0)
#define tlr_config_info(x...)      do { syslog(LOG_INFO, x); } while (0)

#if (CONFIG_DEBUG == 1)
#define tlr_config_debug(x...)      do { syslog(LOG_DEBUG, x); } while (0)

#elif (CONFIG_DEBUG == 2)
#define tlr_config_debug(x...)     do { printf("%s: ", __func__); printf (x); } while (0)

#else
#define tlr_config_debug(x...)
#endif


typedef enum
{
    config_status_success,
    config_status_invalid_parameter,
    config_status_no_memory,
    config_status_invalid_format,

    config_status_group_not_found,
    config_status_element_not_found,
    config_status_instance_not_found,
    config_status_invalid_value,
    config_status_action_not_found,
    config_status_access_error,
    config_status_send_error,

    config_status_file_not_found,
    config_status_file_write_error,
    config_status_file_read_error,
    config_status_no_entry_exist, /* group, element not available */
    config_status_set_error,
    config_status_commit_error,
    config_status_save_error,
    config_status_revert_error,

    config_status_command_not_found,
    config_status_system_error,
	config_status_read_complete,
	config_status_timeout,

    config_status_error_COUNT
} config_status_t;

typedef void const * tlr_handle_t;

#endif
