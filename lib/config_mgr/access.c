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
****************************************************************************/

#include <string.h>
#include "validate.h"
#include "config_mgr.h"
#include "security.h"
#include "tlr_common.h"

/*
    Check a given password is the same as configured salted password.
*/
static bool check_salted_pwd(char const * const cfg_salted_pwd, char const * const entered_password)
{
#define SALT_BUF_LEN  24
    char calculated_pwd[MAX_HASHED_PW_LEN];
    char salt[SALT_BUF_LEN];
    bool result = false;

#ifdef TLR_MD5
    if (sscanf(cfg_salted_pwd, "$1$%10[^$]", salt) != 1) goto done;
#else
    if (sscanf(cfg_salted_pwd, "$6$%20[^$]", salt) != 1) goto done;
#endif

    result = salt_hash_password(entered_password, salt, calculated_pwd, sizeof calculated_pwd);

    if (!result) goto done;

    result = (strcmp(cfg_salted_pwd, calculated_pwd) == 0);

done:
    return result;
}


bool tlr_user_is_authorized(char const * const username, char const * const password, config_mgr_access_level_t * level, bool const user_is_logged_in)
{
    bool authorized = false;
    int inst;
    config_status_t status;
    config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
    config_mgr_group_t const * const group = &table->groups[config_user];

    for (inst=1; inst <= group->instances; inst++)
    {
        char value[MAX_HASHED_PW_LEN];
        bool user_access_granted = false;

        status = tlr_get_element_value(group->id, inst, config_user_name, value, sizeof value, config_mgr_element_type_string, config_mgr_access_level_super);
    	if (status != config_status_success) break;
        if (strcmp(value, username) != 0) continue;

        status = tlr_get_element_value(group->id, inst, config_user_password, value, sizeof value, config_mgr_element_type_string, config_mgr_access_level_super);
    	if (status != config_status_success) break;

        user_access_granted = user_is_logged_in || check_salted_pwd(value, password);
        if (user_access_granted)
    	{
    	    config_user_access_t access;

            *level = config_user_access_read_only;

    	    status = tlr_get_element_value(group->id, inst, config_user_access, value, sizeof value, config_mgr_element_type_string, config_mgr_access_level_super);
    	    if (status != config_status_success) break;

            status = tlr_config_string_to_value(config_user, config_user_access, value, &access);
    	    if (status != config_status_success) break;

    	    *level = access;
    	    authorized = true;
    	    goto done;
        }
    }

done:
    return authorized;
}

/*
  This function determines if the Password is set to a typical default "PASSWORD" or "password".
  It returns <default> if set. (i.e. not one of those useless defaults)
*/
char const * tlr_user_show_password(config_mgr_element_type_t type, char const * const password)
{
    static char const * const useless_pwds[] = { "PASSWORD", "password" };
    static char const pwd_configured[] = "<configured>";
    static char const pwd_default[] = "<default>";
    char const * result = "";
    size_t index;

    if ((password == NULL) || (password[0] == '\0')) goto done;
    if (type != config_mgr_element_type_password)
    {
        result = pwd_configured;
        goto done;
    }

    for (index = 0; index < asizeof(useless_pwds); index++)
    {
        char const * const pwd = useless_pwds[index];

        if (check_salted_pwd(password , pwd))
        {
            result = pwd_default;
            goto done;
        }
    }

    result = pwd_configured;

done:
    return result;
}
