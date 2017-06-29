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

#include "lr54leds.h"
#include "digiutils.h"
#include "strlcpy.h"
#include "config_mgr.h"
#include "tlr_common.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static char const * const led_name_mappings[] =
{
    [led_power] = "power",
    [led_signal_green] = "signal_green",
    [led_signal_yellow] = "signal_yellow",
    [led_service_green] = "service_green",
    [led_service_yellow] = "service_yellow",
    [led_sim1] = "sim1",
    [led_sim2] = "sim2",
    [led_wifi_24] = "wifi_24",
    [led_wifi_5] = "wifi_5",
    [led_dsl] = "dsl"
};

static char const * const led_state_mappings[] =
{
    [led_state_off] = "off",
    [led_state_on] = "on",
    [led_state_blink_slow] = "blink_slow",
    [led_state_blink_fast] = "blink_fast",
};

// Defines the GPIO numbers for each LED.
typedef struct
{
    int const gpio[MAX_LED];
} gpio_mapping_t;

static gpio_mapping_t const led_gpio_mapping[] =
{
    [platform_lr54] =
    {
        {
            [led_power] = 22,
            [led_signal_green] = 23,
            [led_signal_yellow] = 24,
            [led_service_green] = 25,
            [led_service_yellow] = 26,
            [led_sim1] = 27,
            [led_sim2] = 31,
            [led_wifi_24] = 28,
            [led_wifi_5] = 29,
            [led_dsl] = 29,
        },
    },
    [platform_lr54w] =
    {
        {
            [led_power] = 22,
            [led_signal_green] = 23,
            [led_signal_yellow] = 24,
            [led_service_green] = 25,
            [led_service_yellow] = 26,
            [led_sim1] = 27,
            [led_sim2] = 31,
            [led_wifi_24] = 28,
            [led_wifi_5] = 29,
        },
    },
    [platform_lr54d] =
    {
        {
            [led_power] = 22,
            [led_signal_green] = 23,
            [led_signal_yellow] = 24,
            [led_service_green] = 25,
            [led_service_yellow] = 26,
            [led_sim1] = 27,
            [led_sim2] = 31,
            [led_dsl] = 28,
        },
    },
    [platform_lr54dwc1] =
    {
        {
            [led_power] = 22,
            [led_signal_green] = 23,
            [led_signal_yellow] = 24,
            [led_service_green] = 25,
            [led_service_yellow] = 26,
            [led_sim1] = 27,
            [led_sim2] = 31,
            [led_wifi_24] = 28,
            [led_wifi_5] = 29,
            [led_dsl] = 29,
        },
    },
};

// Structure to contain the LED on/off counts for each LED state.
typedef struct
{
    int     on_count;
    int     off_count;
    int     blinks;
    int     rests;
    int     times;
} ledstate_gpio_t;

//gpio l <gpio> <on> <off> <blinks> <rests> <times>
//
//<gpio> : The gpio to control
// <on> : The on time. This appears to be in units of 100 milliseconds, so a value of 10 gives an on time of 1 second
// <off> : The off time. This appears to be in units of 100ms, so value of 10 gives an off time of 1 second.
// <blinks> : Sets the number of times to blink the led. NOTE that the value must be set to TWICE the number of desired blinks, so if you want to blink 3 times, set this to 6.
// If set to 0 or 1, you get one blink, but it seems to be at a different (higher) rate than when set to 2. If set to an odd number, this results in (n+1)/2 blinks.
// <rests> : This appears to be the delay between blink cycle (set by <times>). The units don't appear to be fixed, and rather, related to the on/off times.
// So if the on/off times are larger, the rest time increases
// <times> : The number of times to repeat the blink cycle. Values 0 and 1 mean that the blink cycle isn't repeated. There is a delay of <rests> time between cycles.

// Information required for each led state.
static ledstate_gpio_t const ledstate_def[] =
{
    [led_state_off] =
    {
        .on_count = 0,
        .off_count = 4000,
        .blinks = 1,
        .rests = 0,
        .times = 4000,
    },
    [led_state_on] =
    {
        .on_count = 4000,
        .off_count = 0,
        .blinks = 1,
        .rests = 0,
        .times = 4000,
    },
    [led_state_blink_slow] =
    {
        .on_count = 10,
        .off_count = 10,
        .blinks = 4000,
        .rests = 0,
        .times = 4000,
    },
    [led_state_blink_fast] =
    {
        .on_count = 5,
        .off_count = 5,
        .blinks = 4000,
        .rests = 0,
        .times = 4000,
    }
};

static gpio_mapping_t const * get_gpio_mapping(void)
{
    platform_type_t const platform_type = tlr_get_platform();

    return &led_gpio_mapping[platform_type];
}

static bool led_is_supported(gpio_mapping_t const * const gpio_mapping, led_t const led)
{
    bool is_supported = false;

    if (led < 0 || led >= MAX_LED)
    {
        goto done;
    }

    is_supported = gpio_mapping->gpio[led] != 0;

done:
    return is_supported;
}

static bool led_state_is_supported(led_state_t const led_state)
{
    return led_state >= 0 && led_state < MAX_LED_STATE;
}

static void add_led_name(led_t const led, char const *** led_names, size_t * const num_leds)
{
    *led_names = realloc(*led_names, sizeof(char const *) * ((*num_leds)+1));
    (*led_names)[*num_leds] = led_name_mappings[led];
    (*num_leds)++;
}

static void alloc_supported_led_names(char const *** supported_led_names, size_t * const num_supported_leds)
{
    gpio_mapping_t const * const gpio_mapping = get_gpio_mapping();
    led_t led;

    for (led = 0; led < ARRAY_SIZE(gpio_mapping->gpio); led++)
    {
        if (led_is_supported(gpio_mapping, led))
        {
            add_led_name(led, supported_led_names, num_supported_leds);
        }
    }
}

void led_name_strings(char const * const ** led_names, size_t * const num_leds)
{
    static char const ** supported_led_names;
    static size_t num_supported_leds;

    if (supported_led_names == NULL)
    {
        alloc_supported_led_names(&supported_led_names, &num_supported_leds);
    }

    *led_names = supported_led_names;
    *num_leds = num_supported_leds;
}

void led_state_name_strings(char const * const ** led_state_names, size_t * const num_led_states)
{
    *led_state_names = led_state_mappings;
    *num_led_states = ARRAY_SIZE(led_state_mappings);
}

// Converts a led name string to a led_t type
led_t led_name_string_to_type(char * const led_name)
{
    int index;
    led_t led = MAX_LED;

    for (index = 0; index < ARRAY_SIZE(led_name_mappings); index++)
    {
        if (strcmp(led_name, led_name_mappings[index]) == 0)
        {
            led = (led_t)index;
            break;
        }
    }

    return led;
}

// Converts a led state name string to a led_state_t type
led_state_t led_state_string_to_type(char * const led_state_name)
{
    int index;
    led_state_t led_state = MAX_LED_STATE;

    for (index = 0; index < ARRAY_SIZE(led_state_mappings); index++)
    {
        if (strcmp(led_state_name, led_state_mappings[index]) == 0)
        {
            led_state = (led_state_t)index;
            break;
        }
    }

    return led_state;
}

int led_control(led_t const led, led_state_t const led_state)
{
    int result = -1;
    ledstate_gpio_t const * leddef;
    gpio_mapping_t const * const gpio_mapping = get_gpio_mapping();

    if (!led_is_supported(gpio_mapping, led) || !led_state_is_supported(led_state))
    {
        goto done;
    }

    leddef = &ledstate_def[led_state];

    system_command("gpio l %d %d %d %d %d %d", gpio_mapping->gpio[led],
                   leddef->on_count,
                   leddef->off_count,
                   leddef->blinks,
                   leddef->rests,
                   leddef->times);

    result = 0;

done:
    return result;
}

