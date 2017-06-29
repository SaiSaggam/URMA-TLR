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

#ifndef __LR54LEDS_H__
#define __LR54LEDS_H__

#include <stdlib.h>

typedef enum
{
    led_power,
    led_signal_green,
    led_signal_yellow,
    led_service_green,
    led_service_yellow,
    led_sim1,
    led_sim2,
    led_wifi_24,
    led_wifi_5,
    led_dsl,

    MAX_LED
} led_t;

typedef enum
{
    led_state_off,
    led_state_on,
    led_state_blink_slow,
    led_state_blink_fast,

    MAX_LED_STATE
} led_state_t;

int led_control(led_t const led, led_state_t const led_state);
void led_name_strings(char const * const ** led_names, size_t * const num_leds);
void led_state_name_strings(char const * const ** led_state_names, size_t * const num_led_states);
led_t led_name_string_to_type(char * const led_name);
led_state_t led_state_string_to_type(char * const ledstatename);

#endif /* __LR54LEDS_H__ */
