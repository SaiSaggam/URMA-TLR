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

#include "security.h"
#include "digiutils.h"
#include "strlcpy.h"
#include "tlr_popen.h"
#include <stdio.h>
#include <string.h>

#define PASSWORD_PREFIX_LEN  (sizeof "$xx$")

/* Calculate a salted password given a clear text password. */
bool salt_hash_password(char const * const pwd, char const * const salt, char *salted_pwd, int maxlen)
{
    char *line = NULL;
    size_t line_len;
    bool success = false;
#ifdef TLR_MD5
    char const moption[] = "-mmd5";
#else
#ifdef __mips__
    char const moption[] = "-msha512";      // Target busybox has a slightly different parameter
#else
    char const moption[] = "-mSHA-512";
#endif
#endif

    if (pwd[0] == '\0')  /* reverting password? */
    {
        *salted_pwd = '\0';
        goto error;
    }

    {
        int result;

        if (salt == NULL)
            result = tlr_system_command_with_response(&line, &line_len, "mkpasswd %s %s", moption, pwd);
        else
            result = tlr_system_command_with_response(&line, &line_len, "mkpasswd %s %s %s", moption, pwd, salt);

        success = ((result != -1) && (line[0] == '$'));
    }

    if (success)
    {
        strlcpy(salted_pwd, line, maxlen);
    }

    free(line);

error:
    return success;
}

static char const * get_key_file(password_scope_t const scope)
{
    char const * file_name = NULL;

    switch (scope)
    {
        case password_scope_global:
            file_name = GLOBAL_KEY_FILE;
            break;

        case password_scope_device:
            file_name = DEVICE_KEY_FILE;
            break;
    }

    return file_name;
}

bool encrypt_password(password_scope_t const scope, char const * const clear_pwd, char * const encrypt_pwd, size_t const encrypt_pwd_length)
{
    bool success = false;
    char const * const key_file = get_key_file(scope);

    if (clear_pwd == NULL) goto error;
    if (encrypt_pwd == NULL) goto error;
    if (encrypt_pwd_length < PASSWORD_PREFIX_LEN) goto error;

    if (key_file != NULL)
    {
        char * line = NULL;
        size_t line_len;
        int const result = system_command_with_response(&line, &line_len, "echo \'%s\' | openssl enc -aes-256-cbc -a -salt -kfile %s", clear_pwd, key_file);

        success = (result != -1) && (line_len <= (encrypt_pwd_length - PASSWORD_PREFIX_LEN));
        if (success)
        {
            snprintf(encrypt_pwd, encrypt_pwd_length, PASSWORD_PREFIX_FORMAT "%s", scope, line);
        }

        free(line);
    }

error:
    return success;
}

bool decrypt_password(char const * const encrypt_pwd, char * const clear_pwd, size_t const clear_pwd_length)
{
    bool success = false;
    size_t const pwd_index = PASSWORD_PREFIX_LEN - 1;
    int scope;

    if (clear_pwd == NULL) goto error;
    if (encrypt_pwd == NULL) goto error;

    if (sscanf(encrypt_pwd, PASSWORD_PREFIX_FORMAT, &scope) != 1) goto error;

    {
        char * line = NULL;
        size_t line_len;
        char const * const key_file = get_key_file(scope);
        char const * const encrypted_data = encrypt_pwd + pwd_index;
        int result = system_command_with_response(&line, &line_len, "echo \'%s\' | openssl enc -aes-256-cbc -a -d -kfile %s", encrypted_data, key_file);

        success = (result != -1) && (line_len < clear_pwd_length);

        if (success)
        {
            strlcpy(clear_pwd, line, clear_pwd_length);
        }

        free(line);
    }

error:
    return success;
}
