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
*****************************************************************************/

#include "odm_specific.h"
#include <strip_whitespace.h>
#include <tlr_print.h>
#include <odm.h>
#include <config_mgr.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * return the platform name for this device (e.g. LR54, LR54DWC1)
 * return value:
 * 0: success *model_name_ptr will contain a pointer to the model name string
 * -1: error
 *  Note that the returned pointer does not need to be freed by the caller
 */
int get_tlr_platform_name(char const * * model_name_ptr)
{
#define MODEL_LEN  64
    static char model_name[MODEL_LEN];
    int result;

    result = get_odm_ModelName(model_name, sizeof model_name);
    if (result < 0)
    {
        result = -1;
        goto done;
    }

    if (model_name_ptr != NULL)
        *model_name_ptr = model_name;
    result = 0;

done:

    return result;
}

/*
 * return the ROM ID for this device
 * return value:
 * 0: success *rom_id_ptr will contain a pointer to the ROM ID string
 * -1: error
 *  Note that the returned pointer does not need to be freed by the caller
 */
int get_tlr_rom_id(char const * * rom_id_ptr)
{
    static char rom_id[100];
    int result;

    result = get_odm_ROMID(rom_id, sizeof rom_id);
    if (result <= 0)
    {
        result = -1;
        goto done;
    }

    *rom_id_ptr = rom_id;
    result = 0;

done:
    if (result != 0)
    {
        tlr_debug("error getting ROM ID\n");
    }

    return result;
}

