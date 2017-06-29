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

#include "unittest_support.h"
#include "odm_specific.h"
#include "serial_number.h"
#include "firmware_version.h"
#include "bootloader_version.h"
#include "bank.h"
#include "ifuptime_lib.h"
#include "tlr_print.h"
#include "tlr_common.h"
#include "config_mgr.h"
#include "show_parse_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <libgen.h>
#include <linux/limits.h>
#include <json.h>

// Set the following to display CPU usage as a single line rather than
// separately for each core.
#define COMBINE_CPU_USAGE   1

#ifndef UNIT_TEST
static char const * model_name;
static char const * part_number = "Not available";   // TODO: initialise when available from ODM
static char * serial_number;
static char const * hardware_version = "Not available";   // TODO: initialise when available from ODM
static int bank;
static char * firmware_version;
static char * bootloader_version;
static char config_file[PATH_MAX];
static char * config_file_basename;
static char uptime[sizeof "ddddd Days, hh Hours, mm Minutes, ss Seconds"];
static char system_time[sizeof "dd September yyyy, hh:mm:ss"];
#endif

#if COMBINE_CPU_USAGE
STATIC struct
{
    int current;
    int min;
    int max;
    int average;
} cpu_usage;
#else
#define MAX_CPUS 4
static struct
{
    int current;
    int min;
    int max;
    int average;
} cpu_usage[MAX_CPUS];
static int num_cpus;
#endif

#ifndef UNIT_TEST
static char const * temperature = "Not available";   // TODO: initialise when available from resource monitor
static char * description;
static char * location;
static char * contact;
#endif

STATIC_FUNCTION int get_cpu_usage(void)
{
    int result;
#if !COMBINE_CPU_USAGE
    int max_cpu = 0;
#endif
    char * line = NULL;
    size_t len;

    FILE * const fp = fopen("/tmp/cpu_usage", "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }

    while (getline(&line, &len, fp) != -1)
    {
        int value;

#if COMBINE_CPU_USAGE
        if (sscanf(line, "Total CPU Usage: %d.%*d%%", &value) == 1)
        {
            cpu_usage.current = value;
        }
        else if (sscanf(line, "Total CPU Min: %d.%*d%%", &value) == 1)
        {
            cpu_usage.min = value;
        }
        else if (sscanf(line, "Total CPU Max: %d.%*d%%", &value) == 1)
        {
            cpu_usage.max = value;
        }
        else if (sscanf(line, "Total CPU Avg: %d.%*d%%", &value) == 1)
        {
            cpu_usage.average = value;
        }
#else
        int cpu;
        #define CPU_IS_VALID(cpu)   ((cpu) > 0 && (cpu) <= ARRAY_SIZE(cpu_usage))

        if (sscanf(line, " CPU%d Usage: %d.%*d%%", &cpu, &value) == 2)
        {
            if (CPU_IS_VALID(cpu))
            {
                cpu_usage[cpu-1].current = value;
                max_cpu = MAX(cpu, max_cpu);
            }
        }
        else if (sscanf(line, " CPU%d Min: %d.%*d%%", &cpu, &value) == 2)
        {
            if (CPU_IS_VALID(cpu))
            {
                cpu_usage[cpu-1].min = value;
                max_cpu = MAX(cpu, max_cpu);
            }
        }
        else if (sscanf(line, " CPU%d Max: %d.%*d%%", &cpu, &value) == 2)
        {
            if (CPU_IS_VALID(cpu))
            {
                cpu_usage[cpu-1].max = value;
                max_cpu = MAX(cpu, max_cpu);
            }
        }
        else if (sscanf(line, " CPU%d Avg: %d.%*d%%", &cpu, &value) == 2)
        {
            if (CPU_IS_VALID(cpu))
            {
                cpu_usage[cpu-1].average = value;
                max_cpu = MAX(cpu, max_cpu);
            }
        }
#endif
    }

#if !COMBINE_CPU_USAGE
    num_cpus = max_cpu;
#endif

    result = 0;

done:
    free(line);
    if (fp != NULL)
    {
        fclose(fp);
    }
    return result;
}


#ifndef UNIT_TEST

static void get_system_info(void)
{
    if (get_tlr_platform_name(&model_name) != 0)    // NB. model_name does not need to be freed
    {
        tlr_debug("failed to get model name\n");
    }

    if (get_tlr_serial_number(&serial_number) != 0)  // LZ. serial_number does not need to be freed
    {
        tlr_debug("failed to get serial number\n");
    }

    if (get_tlr_bank(&bank) != 0)
    {
        tlr_debug("failed to get bank\n");
    }

    if (get_tlr_firmware_version(&firmware_version) != 0)
    {
        tlr_debug("failed to get firmware version\n");
    }

    if (get_tlr_bootloader_version(&bootloader_version) != 0)
    {
        tlr_debug("failed to get bootloader version\n");
    }

    tlr_get_config_file(config_file, sizeof config_file);
    config_file_basename = basename(config_file);

    uint64_t uptime_secs;
    if (get_system_uptime(&uptime_secs) != ifuptime_ok)
    {
        tlr_debug("failed to get uptime\n");
    }
    else
    {
        format_ifuptime(uptime_secs, uptime, sizeof uptime);
    }

    time_t const time_secs = time(NULL);
    strftime(system_time, sizeof system_time, "%d %B %Y, %T", localtime(&time_secs));

    if (get_cpu_usage() != 0)
    {
        tlr_debug("failed to get cpu usage\n");
    }

    if (tlr_get_element_alloc_value(config_system, 1, config_system_description, (void **)&description, config_mgr_element_type_string, config_mgr_access_level_read_only) != config_status_success)
    {
        tlr_debug("failed to get description\n");
    }

    if (tlr_get_element_alloc_value(config_system, 1, config_system_location, (void **)&location, config_mgr_element_type_string, config_mgr_access_level_read_only) != config_status_success)
    {
        tlr_debug("failed to get location\n");
    }

    if (tlr_get_element_alloc_value(config_system, 1, config_system_contact, (void **)&contact, config_mgr_element_type_string, config_mgr_access_level_read_only) != config_status_success)
    {
        tlr_debug("failed to get contact\n");
    }
}

static void cleanup_system_info(void)
{
    free(firmware_version);
    free(bootloader_version);
    free(description);
    free(location);
    free(contact);
}



static void show_system_info_ascii(void)
{
    printf(" Model             : %s\n", model_name);
    printf(" Part Number       : %s\n", part_number);
    printf(" Serial Number     : %s\n", serial_number);
    printf("\n");
    printf(" Hardware Version  : %s\n", hardware_version);
    printf(" Using Bank        : %d\n", bank);
    printf(" Firmware Version  : %s\n", firmware_version);
    printf(" Bootloader Version: %s\n", bootloader_version);
    printf(" Using Config File : %s\n", config_file_basename);
    printf("\n");
    printf(" Uptime            : %s\n", uptime);
    printf(" System Time       : %s\n", system_time);
    printf("\n");
#if COMBINE_CPU_USAGE
    printf(" CPU               : %d%% (min %d%%, max %d%%, avg %d%%)\n", cpu_usage.current, cpu_usage.min, cpu_usage.max, cpu_usage.average);
#else
    for (int i = 0; i < num_cpus; i++)
    {
        printf(" CPU Core %d        : %d%% (min %d%%, max %d%%, avg %d%%)\n", i, cpu_usage[i].current, cpu_usage[i].min, cpu_usage[i].max, cpu_usage[i].average);
    }
#endif
    printf(" Temperature       : %s\n", temperature);
    printf("\n");
    printf(" Description       : %s\n", description);
    printf(" Location          : %s\n", location);
    printf(" Contact           : %s\n", contact);
    printf("\n");
}


static void insert_json_information(tlr_show_object_t * const show_obj, config_state_system_t const info_element)
{
    void const * value = NULL;

    switch (info_element)
    {
        case config_state_system_model:
            value = model_name;
            break;
        case config_state_system_part_number:
            value = part_number;
            break;
        case config_state_system_serial_number:
            value = serial_number;
            break;
        case config_state_system_hardware_version:
            value = hardware_version;
            break;
        case config_state_system_bank:
            value = &bank;
            break;
        case config_state_system_firmware_version:
            value = firmware_version;
            break;
        case config_state_system_bootloader_version:
            value = bootloader_version;
            break;
        case config_state_system_config_file:
            value = config_file_basename;
            break;
        case config_state_system_uptime:
            value = uptime;
            break;
        case config_state_system_system_time:
            value = system_time;
            break;
        case config_state_system_cpu_usage:
            value = &cpu_usage.current;
            break;
        case config_state_system_cpu_min:
            value = &cpu_usage.min;
            break;
        case config_state_system_cpu_max:
            value = &cpu_usage.max;
            break;
        case config_state_system_cpu_avg:
            value = &cpu_usage.average;
            break;
        case config_state_system_temperature:
            value = temperature;
            break;
        case config_state_system_description:
            value = description;
            break;
        case config_state_system_location:
            value = location;
            break;
        case config_state_system_contact:
            value = contact;
            break;
    }
    tlr_show_object_add_element(show_obj, info_element, value);
}

static void add_system_json(tlr_show_object_t * const show_obj, int const instance)
{
    int     element_idx;
    config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(config_state_system, instance, config_mgr_get_group);

    if (group_ptr != NULL)
    {
        for (element_idx = 0; element_idx < group_ptr->elements.count; element_idx++)
        {
            config_mgr_element_t * const element_ptr = &group_ptr->elements.element[element_idx];
            if (element_ptr->access != config_mgr_access_no_access)
            {
                insert_json_information(show_obj, element_ptr->id);
            }
        }
    }
}

static void show_system_info_json(int const instance)
{
    tlr_show_object_t * const show_obj = tlr_show_object_create(config_state_system, instance);

    if (show_obj == NULL)
        goto done;

    add_system_json(show_obj, instance);
    printf("%s\n", tlr_show_object_to_json_string(show_obj));

done:
    if (show_obj != NULL)
        tlr_show_object_destroy(show_obj);
}

static void show_system_info(tlr_response_format_t const format, int const instance)
{
    if (format == tlr_response_format_json)
        show_system_info_json(instance);
    else
        show_system_info_ascii();
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

    get_system_info();
    show_system_info(format, instance);
    cleanup_system_info();
    ret = EXIT_SUCCESS;
done:
    return ret;
}

#endif
