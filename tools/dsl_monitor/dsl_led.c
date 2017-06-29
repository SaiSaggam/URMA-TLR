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

#include "dsl_led.h"
#include "lr54leds.h"

static led_state_t get_desired_led_state(dsl_context_t const * const context)
{
    led_state_t led_state;

    if (context->desired_state == dsl_state_disconnected)
    {
        led_state = led_state_off;
    }
    else if (context->current_state == dsl_state_connected)
    {
        led_state = led_state_on;
    }
    else if (context->is_line_up)
    {
        led_state = led_state_blink_fast;
    }
    else
    {
        led_state = led_state_blink_slow;
    }

    return led_state;
}

void dsl_led_update_state(dsl_context_t * const context)
{
    led_state_t const desired_led_state = get_desired_led_state(context);

    if (context->current_led_state != desired_led_state)
    {
        context->current_led_state = desired_led_state;
        led_control(led_dsl, desired_led_state);
    }
}

