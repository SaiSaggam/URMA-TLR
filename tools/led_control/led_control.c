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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <strlcat.h>
#include <stdint.h>
#include <stdbool.h>

#include <lr54leds.h>

static void printf_name_strings(char const * const * const names, size_t const num)
{
    size_t index;

    for (index = 0; index < num; index++)
    {
        printf(" %s", names[index]);
    }
}

static void usage(char const * const progname)
{
    char const * const * led_names = NULL;
    size_t num_leds;
    char const * const * led_state_names = NULL;
    size_t num_led_states;

    led_name_strings(&led_names, &num_leds);
    led_state_name_strings(&led_state_names, &num_led_states);

    printf("Usage: %s -l <led> -s <state>\n", progname);

    printf("Supported leds:");
    printf_name_strings(led_names, num_leds);
    printf("\n");

    printf("Supported states:");
    printf_name_strings(led_state_names, num_led_states);
    printf("\n");
}

int main(int argc, char ** argv)
{
    int c;
    led_t led = MAX_LED;
    led_state_t led_state = led_state_off;
    int required = 0;

    opterr = 0;

    while ((c = getopt(argc, argv, "l:s:")) != -1)
    {
        switch (c)
        {
        case 'l':
            led = led_name_string_to_type(optarg);
            required++;
            break;
        case 's':
            led_state = led_state_string_to_type(optarg);
            required++;
            break;
        default:
            goto error;
        }
    }

    if (required != 2)
        goto error;
    if (led_control(led, led_state) < 0)
        goto error;
    exit(EXIT_SUCCESS);
error:
    usage(basename(argv[0]));
    exit(EXIT_FAILURE);
}

