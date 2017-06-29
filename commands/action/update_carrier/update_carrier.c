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

#include "cellular_api.h"
#include "cellular_status.h"
#include "tlr_print.h"
#include "digiutils.h"
#include "tlr_file.h"
#include "unittest_support.h"

#include <strings.h>
#include <libgen.h>
#include <getopt.h>
#include <stdarg.h>

#define CARRIER_COMPARE(src_name, dst_name)  strcasecmp((src_name), (dst_name))

static char const * progname;
static cellular_firmware_info_t current_carrier_image;
static tlr_response_format_t response_format = tlr_response_format_unknown;

static void show_usage(void)
{
    fprintf(stderr, "Usage: %s [-f <json|ascii>] [carrier name]\n", progname);
}

static int get_usb_index(size_t const index_3g, size_t * const index_usb)
{
    int ret;
    int const fd = cellular_open_status();

    if (fd < 0)
    {
        ret = -1;
        goto error;
    }

    if (!cellular_get_usb_ready(fd, index_3g))
    {
        ret = -1;
        goto error;
    }

    if (cellular_get_usb_index(fd, index_3g, index_usb) < 0)
    {
        ret = -1;
        goto error;
    }

    ret = 0;

error:
    cellular_close_status(fd);
    return ret;

}


STATIC_FUNCTION int get_supported_carrier_images(size_t const index_usb, cellular_firmware_info_t ** carrier_images, size_t * images_count)
{
    int const ret = cellular_get_firmware_list(index_usb, carrier_images, images_count);

    if (ret != 0)
    {
        tlr_debug("get_supported_carrier_images: cellular_get_firmware_list failed (index_usb = %zu)\n", index_usb);
        goto done;
    }

done:
    return ret;
}


STATIC_FUNCTION bool valid_carrier_images(cellular_firmware_info_t * const carrier_images, size_t const images_count, char const * const carrier_name)
{
    bool valid_carrier = false;

    for (size_t i = 0; i < images_count; ++i)
    {
        if (CARRIER_COMPARE(carrier_images[i].carrier_name, carrier_name) == 0)
        {
            valid_carrier = true;
            break;
        }
    }

    return valid_carrier;
}


STATIC_FUNCTION int switch_carrier_image(size_t const index_usb, char const * const carrier_name)
{
    int ret;

    if (CARRIER_COMPARE(current_carrier_image.carrier_name, carrier_name) != 0)
    {
        goto do_switch;
    }

    {
        cellular_firmware_info_t perferred_carrier_image;

        ret = cellular_get_preferred_firmware(index_usb, &perferred_carrier_image);
        if (ret != 0)
        {
            tlr_debug("switch_carrier_image: cellular_get_preferred_firmware failed (index_usb = %zu)\n", index_usb);
            goto done;
        }

        if (CARRIER_COMPARE(perferred_carrier_image.carrier_name, carrier_name) != 0)
        {
            goto do_switch;
        }
    }

    /* no switching */
    tlr_info("No carrier switching\n");
    ret = 0;
    goto done;

do_switch:
    ret = cellular_set_current_firmware_carrier(index_usb, carrier_name);

done:
    return ret;
}

static void show_message_in_ascii(char const * format, va_list args)
{
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
}

static void show_carrier_images_in_ascii(cellular_firmware_info_t const * const carrier_images, size_t const images_count)
{
    int const spacing = 18;

    fprintf(stdout, "%-*s %-*s %-*s\n", spacing, "Carrier names", spacing, "Firmware Version", spacing, "Unique ID");
    for (size_t i = 0; i < images_count; ++i)
    {
        char * msg = "";

        if ((CARRIER_COMPARE(carrier_images[i].carrier_name, current_carrier_image.carrier_name) == 0)
            && (CARRIER_COMPARE(carrier_images[i].version, current_carrier_image.version) == 0)
            && (CARRIER_COMPARE(carrier_images[i].unique_id, current_carrier_image.unique_id) == 0))
        {
            msg = "*Current";
        }

        fprintf(stdout, "%-*s %-*s %.*s %s\n",
                                        spacing, carrier_images[i].carrier_name,
                                        spacing, carrier_images[i].version,
                                        spacing, carrier_images[i].unique_id,
                                        msg);
    }

}

static void show_carrier_images(cellular_firmware_info_t const * const carrier_images, size_t const images_count)
{
    switch (response_format)
    {
        case  tlr_response_format_ascii:
            show_carrier_images_in_ascii(carrier_images, images_count);
            break;
        case tlr_response_format_json:
            /* TODO: */
            fprintf(stderr, "format not implemented\n");
            break;
        case tlr_response_format_unknown:
            break;

    }
}

static void  show_message(char const * const format,...)
{
    va_list args;

    va_start(args, format);

    switch (response_format)
    {
        case  tlr_response_format_ascii:
            show_message_in_ascii(format, args);
            break;
        case tlr_response_format_json:
            tlr_debug("format not implemented\n");
            /* TODO: */
            break;
        case tlr_response_format_unknown:
            break;
    }

    va_end(args);
}

STATIC_FUNCTION int update_carrier_main(int argc, char *argv[])
{
    int ret;
    size_t const index_3g = CELLULAR_INDEX_3G; /* only support 1 modem */
    cellular_firmware_info_t * carrier_images = NULL;
    size_t images_count;
    char const * carrier_name = NULL;
    char const * user;
    size_t index_usb;
    int fd = -1;

    progname = basename(argv[0]);

    openlog(progname, LOG_PID, LOG_USER);

    if ((!tlr_parse_args(argc, argv, &response_format, &user))
        || (response_format == tlr_response_format_unknown))
    {
        show_usage();
        tlr_debug("Invalid arguments\n");
        ret = EXIT_FAILURE;
        goto done;
    }

    fd = tlr_file_lock(progname, false);
    if (fd < 0)
    {
        show_message("Unable to run %s. It may be running.", progname);
        ret = EXIT_FAILURE;
        goto done;
    }

    if (optind < argc)
    {
        carrier_name = argv[optind];
    }

    if (get_usb_index(index_3g, &index_usb) < 0)
    {
        show_message("Unable to access the modem. Please try again.");
        ret = EXIT_FAILURE;
        goto done;
    }

    if (get_supported_carrier_images(index_usb, &carrier_images, &images_count) < 0)
    {
        show_message("Unable to access the modem. Please try again.");
        ret = EXIT_FAILURE;
        goto done;
    }

    if (cellular_get_current_firmware(index_usb, &current_carrier_image) < 0)
    {
        show_message("Unable to access the modem. Please try again.");
        ret = EXIT_FAILURE;
        goto done;
    }

    if (carrier_name == NULL)
    {
        /* just list all supported carriers */
        show_carrier_images(carrier_images, images_count);
        ret = EXIT_SUCCESS;
        goto done;
    }

    if (!valid_carrier_images(carrier_images, images_count, carrier_name))
    {
        show_message("Invalid carrier");
        ret = EXIT_FAILURE;
        goto done;
    }

    if (switch_carrier_image(index_usb, carrier_name) < 0)
    {
        show_message("Unable to switch carrier to %s", carrier_name);
        ret = EXIT_FAILURE;
        goto done;
    }

    show_message("Modem is resetting and carrier will switch to %s", carrier_name);
    ret = EXIT_SUCCESS;

done:
    free(carrier_images);
    tlr_file_unlock(fd);
    closelog();

    return ret;
}

#ifndef _CUNIT_
int main(int argc, char *argv[])
{
    return update_carrier_main(argc, argv);
}
#endif
