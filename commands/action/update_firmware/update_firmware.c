#include "update_firmware.h"
#include "tlr_print.h"
#include "config_mgr.h"
#include "flashbuf.h"
#include "digiutils.h"
#include "unittest_support.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

STATIC_FUNCTION bool parse_command(int argc, char * const argv[], char const ** const file_ptr, tlr_response_format_t * const format_ptr)
{
    char const * user = NULL;
    bool success = false;

    *file_ptr = NULL;
    if ((argc < 2) || (argc % 2)) goto done;  /* expects filename at the end */

    success = tlr_parse_args(argc, argv, format_ptr, &user);
    if (!success) goto done;

    *file_ptr = argv[argc - 1];
    tlr_event_log(tlr_log_info, "firmware_update", "Firmware update started by %s", (user != NULL) ? user : "(none)");

done:
    if (!success)
    {
        tlr_printf(*format_ptr, "Usage: update firmware <file name>\n");
    }

    return success;
}

STATIC_FUNCTION image_type_t verify_firmware_file(char const * const file_path)
{
    image_type_t type = image_type_unknown;
    int const result = check_firmware(file_path, 1);	// flags=1 means this is a SDK(GPL) firmware, not AMIT official.

    switch(result)
    {
    case 0:
        tlr_debug("Firmware update: firmware checking failed\n");
        break;

    case -1:
        tlr_debug("Firmware update: failed to open image\n");
        break;

    case 1:
        type = image_type_bootloader;
        tlr_event_log(tlr_log_info, "firmware_update", "updating bootloader\n");
        break;

    case 2:
        type = image_type_firmware;
        tlr_event_log(tlr_log_info, "firmware_update", "updating firmware\n");
        break;

    default:
        tlr_debug("Firmware update: unknown return %d\n", result);
        break;
    }

    return type;
}

STATIC_FUNCTION bool set_bootbit(void)
{
    bool success = false;
    FILE * const fd = FOPEN("/proc/cmdline", "r");
    int next_bootbit;

    if (fd != NULL)
    {
        char * line = NULL;
        size_t len;

        if (getline(&line, &len, fd) > 0)
        {
            if (strstr(line, "mtdblock5") != NULL)
            {
                next_bootbit = 0;
                success = true;
            }
            else if (strstr(line, "mtdblock3") != NULL)
            {
                next_bootbit = 1;
                success = true;
            }
        }

        free(line);
        FCLOSE(fd);
    }

    if (success)
    {
        system_command("/usr/sbin/amit_bootbit s %d", next_bootbit);
        tlr_event_log(tlr_log_info, "firmware_update", "updated partition %d\n", next_bootbit + 1);
    }
    else
    {
        tlr_debug("Firmware update: set bootbit failed\n");
    }

    return success;
}

STATIC_FUNCTION bool update_firmware_main(int argc, char *argv[])
{
#define MIN_US_DELAY_NEEDED  2000
    char * absolute_path = NULL;
    char const * file_path = NULL;
    char const * file_name = NULL;
    image_type_t image_type;
    tlr_response_format_t format = tlr_response_format_ascii;
    bool success = parse_command(argc, argv, &file_name, &format);

    if (!success) goto error;

    if (*file_name != '/')
    {
        success = asprintf(&absolute_path, "%s/%s", USER_DIR, file_name) >= 0;
        if (!success) goto done;

        file_path = absolute_path;
    }
    else
    {
        file_path = file_name;
    }

    tlr_printf(format, "Verifying %s, please wait ...\n", file_name);
    usleep(MIN_US_DELAY_NEEDED);

    image_type = verify_firmware_file(file_path);
    if (image_type == image_type_unknown)
    {
        success = false;
        goto done;
    }

    tlr_printf(format, "Verified %s\n", file_name);
    tlr_printf(format, "Updating firmware using %s, please wait ...\n", file_name);
    usleep(MIN_US_DELAY_NEEDED);

    success = (exec_upgfile(file_path, 1) == 0);
    if (!success) goto done;

    if (image_type == image_type_firmware)
        success = set_bootbit();

done:
    free(absolute_path);

    if (success)
        tlr_printf(format, "Firmware update complete. Please reboot to run new firmware.\n");
    else
    	tlr_printf(format, "Firmware update failed.\n");

    tlr_event_log(tlr_log_info, "firmware_update", "%s", success ? "success" : "failed");

error:
    return success;
}

#ifndef _CUNIT_
int main(int argc, char *argv[])
{
	bool const success = update_firmware_main(argc, argv);

	return (success ? EXIT_SUCCESS : EXIT_FAILURE);
}
#endif
