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
#include "snmp_utils.h"

// The index element is not stored in the config manager tables, so we
// allocate a variable here which is used for the index.
uint8_t * snmp_get_index_element(int const instance)
{
    int * ptr_int = (int *)malloc(sizeof(int));

    if (ptr_int == NULL)
    {
        goto error;
    }
    *ptr_int = instance;

error:
    return (uint8_t *)ptr_int;
}

bool snmp_is_index_element(char const * const name, u_char const asn_type)
{
    return (memcmp(name, "index", sizeof("index")) == 0) && (asn_type == ASN_INTEGER);
}

config_mgr_element_t *snmp_get_index_config_element(void)
{
    /*
     * Pseudo element used for groups that have an index element which is not
     * auto-generated in the command tables
     */
    static config_mgr_element_t const index_element = {
        .name = "index",
        .type = config_mgr_element_type_int32,
        .id   = 0,
    };

    return (config_mgr_element_t *)&index_element;
}

