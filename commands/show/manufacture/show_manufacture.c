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

#include "unittest_support.h"
#include "odm_specific.h"
#include "serial_number.h"
#include "firmware_version.h"
#include "firmware_checksum.h"
#include "interface_mac_addresses.h"
#include "cellular.h"
#include "cellular_status.h"
#include "usb_version.h"
#include "dsl_version.h"
#include "tlr_print.h"
#include "tlr_common.h"
#include "show_parse_helper.h"
#include "config_mgr.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <libgen.h>
#include <linux/limits.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#ifndef UNIT_TEST
#define MAX_3G_IMEI_BUFFER_LENGTH    100
#define MAX_3G_FIRMWARE_VERSION_BUFFER_LENGTH    100
#define MAX_3G_HARDWARE_VERSION_BUFFER_LENGTH    100
static char const * model_name;
static char * serial_number;
static char * firmware_version;
static char * firmware_checksum;
static char const * rom_id;

static interface_mac_address_st * ethernet_mac_addresses;
static interface_mac_address_st * wifi_2_4_g_mac_addresses;
static interface_mac_address_st * wifi_5_g_mac_addresses;

static char const * const ethernet_interface_label = "LAN";
static char const * const ethernet_interface_additional_label = NULL;

static char const * const wifi_2_4_g_interface_label = "WIFI";
static char const * const wifi_2_4_g_interface_additional_label = "2.4G";

static char const * const wifi_5_g_interface_label = "WIFI";
static char const * const wifi_5_g_interface_additional_label = "5G";


static char cellular_3g_modem_imei[MAX_3G_IMEI_BUFFER_LENGTH];
static char cellular_firmware_version[MAX_3G_FIRMWARE_VERSION_BUFFER_LENGTH];
static char cellular_hardware_version[MAX_3G_HARDWARE_VERSION_BUFFER_LENGTH];
static char const * dsl_firmware_version;
static char const * usb_serial_firmware_version;
#endif

#ifndef UNIT_TEST

static void get_manufacture_info(void)
{
    if (get_tlr_platform_name(&model_name) != 0)    // NB. model_name does not need to be freed
    {
        tlr_debug("failed to get model name\n");
    }

    if (get_tlr_serial_number(&serial_number) != 0) // LZ. serial_number does not need to be freed
    {
        tlr_debug("failed to get serial number\n");
    }

    if (get_tlr_firmware_version(&firmware_version) != 0)
    {
        tlr_debug("failed to get firmware version\n");
    }

    if (get_tlr_firmware_checksum(&firmware_checksum) !=0)
    {
        tlr_debug("failed to get firmware checksum\n");
    }

    if (get_tlr_rom_id(&rom_id) !=0)              // LZ. rom_id does not need to be freed
    {
        tlr_debug("failed to get rom_id\n");
    }

    ethernet_mac_addresses = interface_mac_addresses_alloc(config_eth);
    wifi_2_4_g_mac_addresses = interface_mac_addresses_alloc(config_wifi);
    wifi_5_g_mac_addresses = interface_mac_addresses_alloc(config_wifi5g);

    size_t const index_3g = CELLULAR_INDEX_3G;
    if (get_cellular_info(index_3g, cellular_hardware_version, sizeof cellular_hardware_version,
                              cellular_firmware_version, sizeof cellular_firmware_version,
                              cellular_3g_modem_imei, sizeof cellular_3g_modem_imei) != 0)
    {
        tlr_debug("failed to get cellular info\n");
    }

    if (get_tlr_dsl_version(&dsl_firmware_version) !=0)  // LZ. dsl_firmware_version does not need to be freed
    {
        tlr_debug("failed to get dsl firmware version\n");
    }

    if (get_tlr_usb_serial_version(&usb_serial_firmware_version) !=0)  // LZ. usb_serial_firmware_version does not need to be freed
    {
        tlr_debug("failed to get usb firmware version\n");
    }
}

static void cleanup_manufacture_info(void)
{
    free(firmware_version);
    free(firmware_checksum);
    interface_mac_addresses_free(ethernet_mac_addresses);
    interface_mac_addresses_free(wifi_2_4_g_mac_addresses);
    interface_mac_addresses_free(wifi_5_g_mac_addresses);
}

static void print_all_interface_mac_addresses(void)
{
    char const initial_label[] = "MAC Addresses      :";
    char const empty_label[] = "                    ";
    char const * label_to_print = initial_label;

    /* The very first MAC address printed comes after the label. All others have no label. Because
     * we can't be sure which interface type will be present, we need to remember the label to print
     * out. It will be updated to the empty label once the first MAC address has been printed.
     */
    write_interface_mac_addresses(stdout, ethernet_mac_addresses, &label_to_print, empty_label, ethernet_interface_label, ethernet_interface_additional_label);
    write_interface_mac_addresses(stdout, wifi_2_4_g_mac_addresses, &label_to_print, empty_label, wifi_2_4_g_interface_label, wifi_2_4_g_interface_additional_label);
    write_interface_mac_addresses(stdout, wifi_5_g_mac_addresses, &label_to_print, empty_label, wifi_5_g_interface_label, wifi_5_g_interface_additional_label);

}

static void show_manufacture_info_ascii(void)
{
    printf(" Manufacturing Data\n");
    printf(" ------------------\n");
    printf("\n");
    printf(" Model              : %s\n", model_name);
    printf(" Serial Number      : %s\n", serial_number);
    printf(" Firmware Version   : %s\n", firmware_version);
    printf(" Firmware Checksum  : %s\n", firmware_checksum);
    printf(" R-Data             : ROM ID=%s\n", rom_id);

    print_all_interface_mac_addresses();

    printf(" Cellular IMEI      : %s\n", cellular_3g_modem_imei);
    printf(" Cellular Firmware  : %s\n", cellular_firmware_version);
    //printf(" Cellular Hardware  : %s\n", cellular_hardware_version);
    if (strchr(model_name, 'D') != NULL)
        printf(" DSL RAS F/W        : %s\n", dsl_firmware_version);

    printf(" USB/Serial Firmware: %s\n", usb_serial_firmware_version);
    printf("\n");
}

static void show_manufacture_info(tlr_response_format_t const format, int const instance)
{
    switch (format)
    {
        case tlr_response_format_json:
            tlr_debug("Unsupported format %d\n", format);
            break;

        case tlr_response_format_ascii:
           show_manufacture_info_ascii();
           break;

        case tlr_response_format_unknown:
            tlr_debug("Invalid format %d\n", format);
            break;
    }
}

int main(int const argc, char * const argv[])
{
    int     ret;
    int     instance = -1;
    tlr_response_format_t format;
    int max_instance = tlr_get_max_instance(config_state_system, config_mgr_table_type_show);
    char *progname = basename(argv[0]);

    if (max_instance <= 0)
    {
        ret = EXIT_FAILURE;
        goto done;
    }

    if (parse_show_options(argc, argv, &format, &instance, FIRST_INSTANCE, max_instance) != 0)
    {
        usage_show(progname);
        ret = EXIT_FAILURE;
        goto done;
    }

    get_manufacture_info();
    show_manufacture_info(format, instance);
    cleanup_manufacture_info();
    ret = EXIT_SUCCESS;
done:
    return ret;
}

#endif
