#define USE_BASIC_STYLE      1
#define USE_AUTOMATED_STYLE  0
#define USE_CONSOLE_STYLE    0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ddp_device.h"
#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "tlr_print.h"
#include "test_ddp.h"

ddp_test_info_t ddp_test_info;

size_t tlr_get_num_groups(config_mgr_group_type_t const type)
{
    return config_mgr_group_table[type].count;
}

size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type)
{
    return config_mgr_group_table[type].groups[group_id].instances;
}

ssize_t tlr_get_group_id(char const * const group_name)
{
    if (ddp_test_info.config.name != NULL)
    {
        CU_ASSERT_STRING_EQUAL(group_name, ddp_test_info.config.name);
    }

    return ddp_test_info.config.id;
}

ssize_t tlr_get_show_group_id(char const * const group_name)
{
    if (ddp_test_info.status.name != NULL)
    {
        CU_ASSERT_STRING_EQUAL(group_name, ddp_test_info.status.name);
    }

    return ddp_test_info.status.id;
}

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    return &config_mgr_group_table[table_index].groups[group_id];
}

tlr_handle_t tlr_status_open(size_t const group_id, size_t const instance)
{
    if (ddp_test_info.status.instance > 0)
    {
       CU_ASSERT_EQUAL(group_id, ddp_test_info.status.id);
       CU_ASSERT_EQUAL(instance, ddp_test_info.status.instance);
    }

    return &ddp_test_info.status;
}

config_status_t tlr_status_get_string_ptr(tlr_handle_t const handle, size_t const element_id, char const ** const string_ptr)
{
    CU_ASSERT_PTR_EQUAL(handle, &ddp_test_info.status);

    switch (element_id)
    {
        case config_state_system_location:
            *string_ptr = ddp_test_info.status.value;
            break;

        case config_state_system_uptime:
            *string_ptr = "";
            break;

        default:
            *string_ptr = NULL;
            break;
    }

    return config_status_success;
}

void tlr_status_close(tlr_handle_t handle)
{
    CU_ASSERT_PTR_EQUAL(handle, &ddp_test_info.status);
}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    if (ddp_test_info.config.instance > 0)
    {
        CU_ASSERT_EQUAL(group_id, ddp_test_info.config.id);
        CU_ASSERT_EQUAL(instance, ddp_test_info.config.instance);
    }

    return &ddp_test_info.config;
}

config_status_t tlr_config_read_alloc_value(tlr_handle_t const handle, size_t const element_id, void * * data, config_mgr_element_type_t const type)
{
    CU_ASSERT_PTR_EQUAL(handle, &ddp_test_info.config);

    switch (element_id)
    {
        case config_lan_mtu:
            *(char **)data = strdup("1500");
            break;

        case config_lan_mask:
            *(char **)data = strdup("255.255.255.0");
            break;

        default:
            *(char **)data = NULL;
            break;
    }

    return config_status_success;
}

void tlr_config_close(tlr_handle_t const handle)
{
    CU_ASSERT_PTR_EQUAL(handle, &ddp_test_info.config);
}

ssize_t tlr_get_element_id(size_t const group_id, char const * const element_name)
{
    if (ddp_test_info.config.instance > 0)
    {
        CU_ASSERT_EQUAL(group_id, ddp_test_info.config.id);
        CU_ASSERT_STRING_EQUAL(element_name, "mtu");
    }

    return config_eth_mtu;
}

config_status_t tlr_set_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      char const * const value,
                                      config_mgr_access_level_t const access)
{
    CU_ASSERT_EQUAL(group_id, ddp_test_info.config.id);
    CU_ASSERT_EQUAL(element_id, config_eth_mtu);
    CU_ASSERT_STRING_EQUAL(value, "1499");

    return config_status_success;
}

void tlr_event_log(tlr_log_t const priority, char const * const process_name, char const * const format, ...)
{
    CU_ASSERT_EQUAL(priority, tlr_log_info);
    CU_ASSERT_STRING_EQUAL(process_name, "test");
}

int system_command(char const * const format, ...)
{
    char * command = NULL;
    va_list args;
    int result = -1;

    va_start(args, format);

    if (vasprintf(&command, format, args) > 0)
    {
        if (ddp_test_info.sys_command != NULL)
        {
            CU_ASSERT_STRING_EQUAL(command, ddp_test_info.sys_command);
            if (strstr(ddp_test_info.sys_command, "fw_update_with_download") != NULL)
            {
                ddp_test_info.sys_command = "action_update_firmware -u cunit /tmp/fw/lr54-1.2.3.4.bin";
            }
        }
        result = 0;
        free(command);
    }

    va_end(args);

    return result;
}

int strlcpy_digi_to_uci(char * const digi, char const * const uci, size_t size)
{
    memcpy(digi, uci, size);
    return 0;
}

bool tlr_is_json_supported(size_t const group_id)
{
    return true;
}

bool tlr_user_is_authorized(char const * const username, char const * const password, config_mgr_access_level_t * level, bool const user_is_logged_in)
{
    CU_ASSERT_STRING_EQUAL(username, ddp_test_info.config.name);
    CU_ASSERT_STRING_EQUAL(password, ddp_test_info.config.value);
    CU_ASSERT_PTR_NOT_NULL(level);
    CU_ASSERT_FALSE(user_is_logged_in);

    if (ddp_test_info.error == 0)
    {
        *level = config_mgr_access_level_super;
    }

    return (ddp_test_info.error == 0);
}
