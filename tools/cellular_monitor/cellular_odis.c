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
****************************************************************************/
#include "cellular_api.h"
#include "cellular_status.h"
#include "odm_specific.h"
#include "firmware_version.h"
#include <stdlib.h>
#include <string.h>
#include "tlr_print.h"
#include "cellular_odis.h"

#define Host_Manufacturer   "Digi International"
#define Host_Plasma_ID      "10kNZUkBB9"
#define Host_OS_Name        "Linux"
#define Host_OS_Version     "2.6.36"
//Host Model Name: Transport LR54W-MC7455
//Host Software Version: 1.0.0.2
static char const * module_model = NULL;

static bool is_odis_supported(int const fd)
{
    bool ret = false;
    
    module_model = cellular_get_model(fd, CELLULAR_INDEX_3G);
    if (module_model != NULL && strcmp(module_model, "MC7455") == 0)
    {
        ret = true;
    }

    return ret;
}

int cellular_set_odis_info(int const fd)
{
    oma_info_t  oma;
    size_t index_usb = 0;
    char const * hw_model = NULL;
    char * sf_version = NULL;
    int ret = -1;
    
    if(!is_odis_supported(fd))
    {
        tlr_debug("odis is not supported");
        ret = 0;
        goto done;
    }

    if (cellular_get_usb_index(fd, CELLULAR_INDEX_3G, &index_usb) != 0)
    {
        tlr_debug("could not get usb index");
        goto done;
    }

    if (get_tlr_platform_name(&hw_model) != 0)
    {
        tlr_debug("could not get platform name");
        goto done;
    }

    if (get_tlr_firmware_version(&sf_version) != 0)
    {
        tlr_debug("could not get firmware version");
        goto done;
    }

    memset(&oma, 0, sizeof oma);
    snprintf(oma.HostManufacture, sizeof oma.HostManufacture, Host_Manufacturer);
    snprintf(oma.HostModelName, sizeof oma.HostModelName, "%s %s-%s", "Transport", hw_model, module_model);
    snprintf(oma.HostSoftwareVersion, sizeof oma.HostSoftwareVersion, sf_version);
    snprintf(oma.HostPlasmaID, sizeof oma.HostPlasmaID, Host_Plasma_ID);
    snprintf(oma.OSName, sizeof oma.OSName, Host_OS_Name);
    snprintf(oma.OSVersion, sizeof oma.OSVersion, Host_OS_Version);
    if (oma_set_info(index_usb, &oma) != 0)
    {
        tlr_debug("could not set odis info, oma_set_info() failed");
        goto done;
    }

    ret = 0;
done:
    free(sf_version);

    return ret;
}

int cellular_get_odis_info(int const fd, oma_info_t * const oma)
{
    unsigned int index_usb = 0;
    int ret = -1;

    if(!is_odis_supported(fd))
    {
        tlr_debug("odis is not supported");
        ret = 0;
        goto done;
    }

    if (cellular_get_usb_index(fd, CELLULAR_INDEX_3G, &index_usb) != 0)
    {
        tlr_debug("could not get usb index");
        goto done;
    }

    memset(oma, 0, sizeof *oma);
    if(oma_get_info(index_usb, oma) != 0)
    {
        tlr_debug("could not get odis info, oma_get_info() failed");
        goto done;
    }    
/*
    printf("oma->HostManufacture = %s\n", oma->HostManufacture);
    printf("oma->HostModelName = %s\n", oma->HostModelName);
    printf("oma->HostSoftwareVersion = %s\n", oma->HostSoftwareVersion);
    printf("oma->HostPlasmaID = %s\n", oma->HostPlasmaID);
    printf("oma->OSName = %s\n", oma->OSName);
    printf("oma->OSVersion = %s\n", oma->OSVersion);
*/    
    ret = 0;
done:
    return ret;
}
