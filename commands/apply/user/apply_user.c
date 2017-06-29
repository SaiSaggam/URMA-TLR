#include "apply_user.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "tlr_print.h"
#include <stdarg.h>

#include "digiutils.h"
#include "config_def.h"
#include "config_status.h"
#include "tlr_common.h"
#include "unittest_support.h"
#include "tlr_popen.h"

static int const group_id = 2000;

#ifdef _CUNIT_
static char const group_name[] = "testers";
#define PASSWD_FILE "test_passwd"
#define GROUP_FILE "test_group"
#else
static char const group_name[] = "users";
#define PASSWD_FILE "/etc/passwd"
#define GROUP_FILE "/etc/group"
#endif

STATIC_FUNCTION bool is_entry_found(char const * const filename, char const * const entryname)
{
    bool found = false;
    FILE * const fd = fopen(filename, "r");
    size_t const entry_len = strlen(entryname);

    if (fd == NULL) goto done;

    do
    {
    	char line[MAX_HASHED_PW_LEN];

    	if (fgets(line, sizeof line, fd) == NULL) break;

    	if (strncmp(line, entryname, entry_len) == 0)
                found = (line[entry_len] == ':');

    } while (!found);

    fclose(fd);

done:
    return found;
}

STATIC_FUNCTION void delete_user(char const * const username)
{
    if (username[0] != '\0')
    {
	    if (is_entry_found(PASSWD_FILE, username))
	    {
            tlr_system_command("/usr/sbin/deluser %s", username);
	    }
    }
}

/* @TODO: Add this at build time */
STATIC_FUNCTION void add_group(void)
{
	if (!is_entry_found(GROUP_FILE, group_name))
		tlr_system_command("/usr/sbin/addgroup -g %d %s", group_id, group_name);
}

STATIC_FUNCTION void add_user(char const * const user, size_t const instance)
{
    size_t const user_id = group_id + instance;

    tlr_system_command("/usr/sbin/adduser -DHh /home/digi/user -s /bin/sh_wrapper -u %zu %s -G %s", user_id, user, group_name);
}

STATIC_FUNCTION void add_password(char const * const username, char const * const password)
{
    system_command("echo \'%s:%s\' | /usr/sbin/chpasswd -e >/dev/null", username, password);
}

STATIC_FUNCTION void add_entry(size_t const instance, user_info_t const * const pinfo)
{
    if ((pinfo->username[0] != '\0') && (pinfo->password_hash[0] != '\0'))
    {
        add_group();
        add_user(pinfo->username, instance);
        add_password(pinfo->username, pinfo->password_hash);
    }
}

STATIC_FUNCTION bool user_exists(size_t const instance, char *username)
{
    static char const * blocked_user[] = {"root", "boot", "sshd"};
    static const size_t blocked_user_count = asizeof(blocked_user);
    size_t index;
    bool result = true;

    if (username[0] == '\0')
    {
        result = false;
        goto done;
    }

    for (index = 0; index < blocked_user_count; index++)
    {
        if (strcmp(username, blocked_user[index]) == 0)
        {
    	    tlr_error("username %s not permitted\n", username);
            goto done;
        }
    }

    {
        config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);
        config_mgr_group_t * const group_ptr = &table_ptr->groups[config_user];

        for (index = 1; index <= group_ptr->instances; index++)
        {
            config_status_t status;
            char name[MAX_USERNAME_LEN];

            if (index == instance) continue;
            status = tlr_get_element_value(config_user, index, config_user_name, name, sizeof name,
                                            config_mgr_element_type_string, config_mgr_access_level_read_only);
            if (status != config_status_success) continue;
            if (name[0] == '\0') continue;

            if (strcmp(name, username) == 0)
            {
                tlr_printf(tlr_response_format_ascii, "Name conflicts with user %d", index);
                goto done;
            }
        }
    }

    result = false;

done:
    return result;
}

STATIC_FUNCTION config_status_t revert_user_info(size_t const instance, user_info_t * const pinfo)
{
    config_status_t status = tlr_set_element_value(config_user, instance, config_user_name,
                                                   pinfo->username, config_mgr_access_level_read_write);

    if (status != config_status_success) goto error;

    status = tlr_set_element_value(config_user, instance, config_user_password,
                                   pinfo->password_hash, config_mgr_access_level_super);

    if (status != config_status_success) goto error;

    status = tlr_set_element_value(config_user, instance, config_user_access,
                                   pinfo->access, config_mgr_access_level_super);

error:
    return status;
}

STATIC_FUNCTION config_status_t get_current_config(size_t const instance, user_info_t * const pinfo)
{
    config_status_t status = tlr_get_element_value(config_user, instance, config_user_name,
                                                   pinfo->username, sizeof pinfo->username,
                                                   config_mgr_element_type_string, config_mgr_access_level_read_only);

    if (status != config_status_success) goto error;

    status = tlr_get_element_value(config_user, instance, config_user_password,
                                   pinfo->password_hash, sizeof pinfo->password_hash,
                                   config_mgr_element_type_string, config_mgr_access_level_super);


    if (status != config_status_success) goto error;

    status = tlr_get_element_value(config_user, instance, config_user_access,
                                   pinfo->access, sizeof pinfo->access,
                                   config_mgr_element_type_string, config_mgr_access_level_super);

error:
    return status;
}

STATIC_FUNCTION bool have_permission(config_mgr_access_level_t const access, char const * const cur_login,  char const * const username, bool const need_super)
{
    bool result = false;

    switch (access)
    {
        case config_mgr_access_level_read_only:
            break;

        case config_mgr_access_level_read_write:
            if (!need_super)
            {
                result = (strcmp(cur_login, username) == 0);
            }
            break;

        case config_mgr_access_level_super:
            result = true;
            break;
    }

    return result;
}

STATIC_FUNCTION int apply_user_config_change(size_t instance, char const * const cur_login, config_mgr_access_level_t const access)
{
    int result = EXIT_FAILURE;
    user_info_t last_info = {.username = {0}, .password_hash = {0}, .access = {0}};
    user_info_t current_info;
    config_status_t status = get_current_config(instance, &current_info);

    if (status != config_status_success)
    {
        tlr_debug("failed to get current config\n");
        goto done;
    }

    status = tlr_get_last_config(config_user, instance, &last_info, sizeof last_info);
    if ((status == config_status_file_not_found) && (access < config_mgr_access_level_super))
    {
        tlr_printf(tlr_response_format_ascii, "Insufficient access");
        goto done;
    }

    if (user_exists(instance, current_info.username)) goto revert;

    switch (status)
    {
        case config_status_success:
        {
            bool const same_username = (strcmp(last_info.username, current_info.username) == 0);
            bool const same_password = (strcmp(last_info.password_hash, current_info.password_hash) == 0);
            bool const same_access = (strcmp(last_info.access, current_info.access) == 0);
            bool const need_super = !same_username || !same_access;

            if (same_username && same_password && same_access)
            {
                result = EXIT_SUCCESS;
                goto done;
            }

            if (!have_permission(access, cur_login, last_info.username, need_super))
            {
                tlr_printf(tlr_response_format_ascii, "Insufficient access");
                goto revert;
            }

            delete_user(last_info.username);
        }
        /* fall through */

        case config_status_file_not_found:

            add_entry(instance, &current_info);

            status = tlr_set_last_config(config_user, instance, &current_info, sizeof current_info);
            if (status != config_status_success)
            {
                tlr_error("failed to set last config\n");
            }

            result = EXIT_SUCCESS;
            break;

        default:
            tlr_debug("failed to get last config\n");
            break;
    }

    goto done;

revert:
    status = revert_user_info(instance, &last_info);
    if (status != config_status_success)
    {
        tlr_error("failed to revert to last entry\n");
    }

done:
    tlr_info("user %zu apply %s.\n", instance, (result == 0) ? "success" : "failed");
    return result;
}

#ifndef _CUNIT_

int main(int argc, char *argv[])
{
    int result = EXIT_FAILURE;
    size_t instance = 0;
    char cur_login[MAX_USERNAME_LEN] = {0};
    config_mgr_access_level_t cur_access = config_mgr_access_level_read_only;
    int opt;

    opterr = 0;
    openlog("apply_user", 0, LOG_USER);

    while ((opt = getopt(argc, argv, "i:u:a:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            instance = strtol(optarg, NULL, 10);
            break;

        case 'u':
            strlcpy(cur_login, optarg, sizeof cur_login);
            break;

        case 'a':
            cur_access = (config_mgr_access_level_t)strtol(optarg, NULL, 10);
            break;

        default:
            break;
        }
    }

    result = apply_user_config_change(instance, cur_login, cur_access);

    closelog();

    return result;
}

#endif
