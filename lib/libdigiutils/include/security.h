#ifndef _SECURITY_H_
#define _SECURITY_H_

#include <stdlib.h>
#include <stdbool.h>
#include "config_mgr.h"

#define MAX_HASHED_PW_LEN   256

#define PASSWORD_PREFIX_FORMAT  "$%02d$"

#ifdef _CUNIT_
#define GLOBAL_KEY_FILE  "./global.key"
#define DEVICE_KEY_FILE "./device.key"
#else
#define GLOBAL_KEY_FILE  "/usr/config/global.key"
#define DEVICE_KEY_FILE BASE_DIGI_DIR "/device.key"
#endif

typedef enum
{
    password_scope_global,
    password_scope_device
} password_scope_t;

bool encrypt_password(password_scope_t const scope, char const * const clear_pwd, char * const encrypt_pwd, size_t const length);
bool decrypt_password(char const * const encrypt_pwd, char * const clear_pwd, size_t const length);
bool salt_hash_password(char const * const pwd, char const * const salt, char *salted_pwd, int maxlen);

#endif /* _SECURITY_H_ */
