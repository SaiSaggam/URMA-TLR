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
#include "cellular_led.h"
#include "cellular_modem.h"
#include "monitor_debug.h"
#include "lr54leds.h"
#include "unittest_support.h"

typedef enum {
    SIGNAL_STRENGTH_NO_SERVICE,
    SIGNAL_STRENGTH_POOR_FAIR,
    SIGNAL_STRENGTH_GOOD_EXCELLENT
} signal_strength_status_t;

static bool cellular_connection_type_4g(int const fd)
{

    bool ret = false;
    cellular_connection_type_t connection_type = cellular_connection_type_unknown;

    cellular_get_connection_type(fd, CELLULAR_INDEX_3G, &connection_type);
    if (connection_type == cellular_conenction_type_4g)
        ret = true;

    return ret;
}

static int32_t cellular_signal_strength(int const fd)
{

    int32_t signal_strength = -110;

    if (fd < 0)
    {
        goto done;
    }

    cellular_get_rssi(fd, CELLULAR_INDEX_3G, &signal_strength);

done:
    return signal_strength;
}

static void service_leds_action(led_state_t yellow_state, led_state_t green_state)
{
    led_control(led_service_yellow, yellow_state);
    led_control(led_service_green, green_state);
}

static void signal_leds_action(led_state_t yellow_state, led_state_t green_state)
{
    led_control(led_signal_yellow, yellow_state);
    led_control(led_signal_green, green_state);
}

static void sim_leds_action(led_state_t sim1_state, led_state_t sim2_state)
{
    led_control(led_sim1, sim1_state);
    led_control(led_sim2, sim2_state);
}

STATIC_FUNCTION void update_cellular_service_leds(int const fd, size_t const active_sim, cellular_context_t * const context)
{
    static cellular_state_t old_state = state_initialize;
    led_state_t yellow_state = led_state_off;
    led_state_t green_state = led_state_off;
    static bool old_type_4g = false;
    static bool current_type_4g = false;

    current_type_4g = cellular_connection_type_4g(fd);
    if (is_current_context_active_sim(context, active_sim) &&
        ((old_state != context->current_state) || (old_type_4g != current_type_4g)))
    {
        old_state = context->current_state;
        old_type_4g = current_type_4g;
        switch (context->current_state)
        {
            case state_connected:
                if (current_type_4g)
                {
                    yellow_state = led_state_off;
                    green_state = led_state_on;
                }
                else
                {
                    yellow_state = led_state_on;
                    green_state = led_state_off;
                }
                break;

            case state_disconnected:
            case state_disconnecting:
            case state_initialize:
            case state_idle:
            case state_disconnect:
                yellow_state = led_state_off;
                green_state = led_state_off;
                break;

            case state_connect:
            case state_connecting:
            case state_wait_modem:
                yellow_state = led_state_off;
                green_state = led_state_blink_slow;
                break;
        }

        service_leds_action(yellow_state, green_state);
    }
}

STATIC_FUNCTION void update_cellular_signal_strength_leds(int const fd)
{
    static signal_strength_status_t previous_signal_strength_status = SIGNAL_STRENGTH_NO_SERVICE;
    int32_t signal_strength = cellular_signal_strength(fd);
    signal_strength_status_t signal_strength_status;

    if(cellular_connection_type_4g(fd))
    {
        if (signal_strength > -106)
        {
            signal_strength_status = SIGNAL_STRENGTH_GOOD_EXCELLENT;
        }
        else if (signal_strength > -121)
        {
            signal_strength_status = SIGNAL_STRENGTH_POOR_FAIR;
        }
        else
        {
            signal_strength_status = SIGNAL_STRENGTH_NO_SERVICE;
        }
    }
    else
    {
        if (signal_strength > -86)
        {
            signal_strength_status = SIGNAL_STRENGTH_GOOD_EXCELLENT;
        }
        else if (signal_strength <= -110)
        {
            signal_strength_status = SIGNAL_STRENGTH_NO_SERVICE;
        }
        else
        {
            signal_strength_status = SIGNAL_STRENGTH_POOR_FAIR;
        }
    }

    if (signal_strength_status != previous_signal_strength_status)
    {
        led_state_t yellow_state;
        led_state_t green_state;

        switch (signal_strength_status)
        {
            case SIGNAL_STRENGTH_GOOD_EXCELLENT:
                yellow_state = led_state_off;
                green_state = led_state_on;
                break;
            case SIGNAL_STRENGTH_POOR_FAIR:
                yellow_state = led_state_on;
                green_state = led_state_off;
                break;
            default:
                yellow_state = led_state_off;
                green_state = led_state_off;
                break;
        }
        signal_leds_action(yellow_state, green_state);
        previous_signal_strength_status = signal_strength;
    }
}

STATIC_FUNCTION void update_cellular_sim_leds(int const fd, size_t const active_sim)
{
#define INVALID_SIM_STATUS 0xFF

    static size_t previous_sim_status = INVALID_SIM_STATUS;

    size_t sim_status = active_sim;

    /* Handle SIM leds */
    switch (get_cellular_state(active_sim))
    {
        case config_cellular_state_off:
            sim_status = 0;
            break;
        case config_cellular_state_on_demand:
            switch (cellular_get_connection_status(fd, CELLULAR_INDEX_3G))
            {
                case cellular_disconnecting:
                case cellular_disconnected:
                    sim_status = 0;
                    break;
                case cellular_connecting:
                case cellular_connected:
                    break;
            }
            break;
        case config_cellular_state_on:
            break;
    }

    if (sim_status != previous_sim_status)
    {
        led_state_t sim1_state;
        led_state_t sim2_state;

        switch (sim_status)
        {
            case 1:
                sim1_state = led_state_on;
                sim2_state = led_state_off;
                break;
            case 2:
                sim1_state = led_state_off;
                sim2_state = led_state_on;
                break;
            default:
                sim1_state = led_state_off;
                sim2_state = led_state_off;
                break;
        }

        sim_leds_action(sim1_state, sim2_state);
        previous_sim_status = sim_status;
    }
}

void cellular_update_leds(int const fd, size_t const active_sim, cellular_context_t * const context)
{
    update_cellular_service_leds(fd, active_sim, context);
    update_cellular_signal_strength_leds(fd);
    update_cellular_sim_leds(fd, active_sim);
}
void cellular_all_leds_off(void)
{
    service_leds_action(led_state_off, led_state_off);
    signal_leds_action(led_state_off, led_state_off);
    sim_leds_action(led_state_off, led_state_off);
}

void monitor_cellular_led(int const fd, cellular_context_t * const context)
{
    size_t active_sim;

    if (CELLULAR_VALID_CSMAN_FD(fd) && cellular_get_active_sim(fd, context->index_3g, &active_sim) == 0)
    {
        cellular_update_leds(fd, active_sim,  context);
    }
    else
    {
        monitor_debug_printf("cellular status: active sim = %d (not ready)\n", active_sim);
    }

}
