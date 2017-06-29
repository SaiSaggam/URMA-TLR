/*****************************************************************************
 * Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <strlcpy.h>
#include <openssl/hmac.h>


#include "config_status.h"
#include "unittest_support.h"
#include "base32.h"
#include "password_gen.h"

static uint8_t const key[] = {
	0x0d, 0x36, 0x96, 0xf5, 0xad, 0xd1, 0x2e, 0x0c,
	0x0b, 0x1a, 0x1c, 0x6d, 0xc2, 0x66, 0x30, 0x66,
	0x7c, 0x98, 0x50, 0x95, 0x53, 0xb4, 0xac, 0xd4,
	0xf7, 0x36, 0x93, 0xda, 0xd1, 0x46, 0x94, 0xce,
	0x91, 0xf6, 0xc2, 0x31, 0xbb, 0x82, 0x71, 0x84,
	0xfe, 0x41, 0x89, 0xd1, 0x98, 0xf3, 0xc7, 0x10,
	0xd4, 0xd8, 0xca, 0xb6, 0xbe, 0xca, 0xa5, 0x97,
	0x16, 0x97, 0xaf, 0x37, 0x53, 0x06, 0x3e, 0xf7,
	0x92, 0xfd, 0x08, 0xec, 0xbe, 0x66, 0xe3, 0x46,
	0xdb, 0xb6, 0xac, 0x41, 0xe5, 0xa6, 0x9b, 0xd8,
	0xec, 0xf3, 0x58, 0x44, 0x8d, 0x7b, 0x28, 0x6e,
	0x42, 0x81, 0x0f, 0x74, 0xbd, 0xa0, 0x79, 0x03,
	0x00, 0x6d, 0xf7, 0x73, 0x34, 0x48, 0xa4, 0xfa,
	0x0e, 0x3f, 0x8d, 0x4e, 0x99, 0xe3, 0x17, 0xa0,
	0xac, 0x77, 0x79, 0xf6, 0x9c, 0x2d, 0xd4, 0xca,
	0x12, 0x2d, 0xd0, 0xc7, 0xdd, 0x61, 0x68, 0xea
};


/**
 * Expects the MAC address to be in the form 0123456789AB
 * i.e. 12 hex characters, upper case, no separators
 *
 * Returns
 *   0 if the MAC address is valid
 *  -1 if it is not valid
 */
STATIC_FUNCTION int validate_mac_addr(char const * const mac_addr)
{
    int retval = -1;
    int i;

    if (strlen(mac_addr) != PASSWORD_GEN_MAC_ADDR_LEN)
        goto error;

    for (i = 0; i < PASSWORD_GEN_MAC_ADDR_LEN; i++)
    {
        if (!isxdigit(mac_addr[i]))
            goto error;

        if (isalpha(mac_addr[i]) && !isupper(mac_addr[i]))
            goto error;
    }

    retval = 0;

error:
    return retval;
}

/**
 * Expects the serial number to be in the form XXnnnnnn
 * where X are uppercase chars and n are digits
 *
 * Returns
 *   0 if the serial number is valid
 *  -1 if it is not valid
 */
STATIC_FUNCTION int validate_serial_number(char const * const serial_number)
{
    int retval = -1;
    int i;

    if (strlen(serial_number) != PASSWORD_GEN_SERIAL_NUM_LEN)
        goto error;

    if (!isupper(serial_number[0]) || !isupper(serial_number[1]))
        goto error;

    for (i = 2; i < PASSWORD_GEN_SERIAL_NUM_LEN; i++)
    {
        if (!isdigit(serial_number[i]))
            goto error;
    }

    retval = 0;

error:
    return retval;
}

int generate_default_password(char const * const mac_addr, char const * const serial_number, char * const password, size_t const max_password_len)
{
    uint8_t * hmac;
    uint8_t  data[PASSWORD_GEN_MAC_ADDR_LEN + PASSWORD_GEN_SERIAL_NUM_LEN];
    uint8_t  tmp_password[PASSWORD_GEN_PASSWORD_LEN * 2];
    int      retval = -1;
    size_t   pwd_length = (max_password_len >= PASSWORD_GEN_PASSWORD_LEN) ? PASSWORD_GEN_PASSWORD_LEN : max_password_len;

    if (validate_mac_addr(mac_addr) < 0)
    {
        tlr_config_debug ("Invalid MAC address for generating default password\n");
        goto error;
    }

    if (validate_serial_number(serial_number) < 0)
    {
        tlr_config_debug("Invalid serial number for generating default password\n");
        goto error;
    }

    memcpy(&data[0], mac_addr, PASSWORD_GEN_MAC_ADDR_LEN);
    memcpy(&data[PASSWORD_GEN_MAC_ADDR_LEN], serial_number, PASSWORD_GEN_SERIAL_NUM_LEN);

    if ((hmac = HMAC(EVP_sha512(),
                     key, sizeof key,
                     data, sizeof data,
                     NULL, 0)) == NULL)
    {
        tlr_config_debug ( "HMAC failed\n");
        goto error;
    }

    base32_encode(hmac, PASSWORD_GEN_PASSWORD_LEN, tmp_password);

    strlcpy(password, (char *)tmp_password, pwd_length);

    retval = 0;

error:

    return retval;
}

