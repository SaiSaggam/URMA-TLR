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

extern "C"
{
    #include "dsl_event_handlers.h"
    #include "dsl_context.h"
    #include "dsl_event.h"
    #include "dsl_modem.h"
    #include "dsl_monitor.h"
    #include "dsl_ppp.h"
    #include "dsl_timer.h"
    #include "messages.h"
    #include "config_mgr.h"
    #include "tlr_common.h"
}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdint.h>
#include <stdlib.h>

static dsl_modem_config_t test_modem_config;
dsl_modem_config_t * dsl_modem_config = &test_modem_config;

char const * dsl_monitor_interface_name(void)
{
    return "dsl";
}

void dsl_timer_start(dsl_timer_t * const timer, size_t const milliseconds)
{
    mock().actualCall("dsl_timer_start")
          .withParameter("timer", timer);
}

void dsl_timer_stop(dsl_timer_t * const timer)
{
    mock().actualCall("dsl_timer_stop")
          .withParameter("timer", timer);
}

bool dsl_timer_is_running(dsl_timer_t * const timer)
{
    dsl_context_t * const context = (dsl_context_t *)mock().getData("context").getPointerValue();
    bool timer_is_running = false;

    if (timer == &context->connecting_timer)
    {
        timer_is_running = mock().getData("connecting_timer_is_running").getIntValue();
    }
    else if (timer == &context->delay_up_timer)
    {
        timer_is_running = mock().getData("delay_up_timer_is_running").getIntValue();
    }
    else if (timer == &context->disconnecting_timer)
    {
        timer_is_running = mock().getData("disconnecting_timer_is_running").getIntValue();
    }

    return timer_is_running;
}

void dsl_ppp_start(dsl_context_t * const context)
{
    mock().actualCall("dsl_ppp_start");
}

void dsl_ppp_stop(dsl_context_t * const context)
{
    mock().actualCall("dsl_ppp_stop");
}

int send_message(message_destination_t const destination, size_t instance, message_codes_t const message_code, char const * const str, char ** const response_buffer)
{
    mock().actualCall("send_message")
          .withParameter("message_code", message_code);

    return 0;
}

TEST_GROUP(dsl_state_disconnected)
{
    dsl_context_t * context;
    bool expected_is_line_up;
    dsl_state_t expected_desired_state;
    dsl_state_t expected_current_state;
    dsl_state_t expected_notified_state;

    void setup()
    {
        context = (dsl_context_t *)calloc(1, sizeof *context);

        mock().strictOrder();
        mock().setData("context", context);

        expected_is_line_up = context->is_line_up = false;
        expected_desired_state = context->desired_state = dsl_state_disconnected;
        expected_current_state = context->current_state = dsl_state_disconnected;
        expected_notified_state = context->notified_state = dsl_state_disconnected;
    }

    void teardown()
    {
        LONGS_EQUAL(expected_is_line_up, context->is_line_up);
        LONGS_EQUAL(expected_desired_state, context->desired_state);
        LONGS_EQUAL(expected_current_state, context->current_state);
        LONGS_EQUAL(expected_notified_state, context->notified_state);

        free(context);
    }
};

TEST(dsl_state_disconnected, dsl_event_line_up_handler_when_desired_state_is_dsl_state_disconnected)
{
    dsl_event_line_up_handler(context);

    expected_is_line_up = true;
}

TEST(dsl_state_disconnected, dsl_event_line_up_handler_when_desired_state_is_dsl_state_connected)
{
    expected_desired_state = context->desired_state = dsl_state_connected;

    mock().expectOneCall("dsl_ppp_start");

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->connecting_timer);

    dsl_event_line_up_handler(context);

    expected_is_line_up = true;
    expected_current_state = dsl_state_connecting;
}

TEST(dsl_state_disconnected, dsl_event_line_down_handler)
{
    dsl_event_line_down_handler(context);
}

TEST(dsl_state_disconnected, dsl_event_connecting_timeout_handler)
{
    dsl_event_connecting_timeout_handler(context);
}

TEST(dsl_state_disconnected, dsl_event_delay_up_timeout_handler)
{
    dsl_event_delay_up_timeout_handler(context);
}

TEST(dsl_state_disconnected, dsl_event_disconnecting_timeout_handler)
{
    dsl_event_disconnecting_timeout_handler(context);
}

TEST(dsl_state_disconnected, dsl_event_connect_message_handler_when_line_is_down)
{
    dsl_event_connect_message_handler(context);

    expected_desired_state = dsl_state_connected;
}

TEST(dsl_state_disconnected, dsl_event_connect_message_handler_when_line_is_up)
{
    expected_is_line_up = context->is_line_up = true;

    mock().expectOneCall("dsl_ppp_start");

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->connecting_timer);

    dsl_event_connect_message_handler(context);

    expected_desired_state = dsl_state_connected;
    expected_current_state = dsl_state_connecting;
}

TEST(dsl_state_disconnected, dsl_event_disconnect_message_handler)
{
    dsl_event_disconnect_message_handler(context);
}

TEST(dsl_state_disconnected, dsl_event_connected_message_handler_when_desired_state_is_dsl_state_disconnected)
{
    mock().expectOneCall("dsl_ppp_stop");

    dsl_event_connected_message_handler(context);
}

TEST(dsl_state_disconnected, dsl_event_connected_message_handler_when_desired_state_is_dsl_state_connected)
{
    expected_desired_state = context->desired_state = dsl_state_connected;

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->delay_up_timer);

    dsl_event_connected_message_handler(context);

    expected_current_state = dsl_state_connected_delay_up;
}

TEST(dsl_state_disconnected, dsl_event_disconnected_message_handler)
{
    dsl_event_disconnected_message_handler(context);
}

TEST_GROUP(dsl_state_connecting)
{
    dsl_context_t * context;
    bool expected_is_line_up;
    dsl_state_t expected_desired_state;
    dsl_state_t expected_current_state;
    dsl_state_t expected_notified_state;

    void setup()
    {
        context = (dsl_context_t *)calloc(1, sizeof *context);

        mock().strictOrder();
        mock().setData("context", context);
        mock().setData("connecting_timer_is_running", true);

        expected_is_line_up = context->is_line_up = true;
        expected_desired_state = context->desired_state = dsl_state_connected;
        expected_current_state = context->current_state = dsl_state_connecting;
        expected_notified_state = context->notified_state = dsl_state_disconnected;
    }

    void teardown()
    {
        LONGS_EQUAL(expected_is_line_up, context->is_line_up);
        LONGS_EQUAL(expected_desired_state, context->desired_state);
        LONGS_EQUAL(expected_current_state, context->current_state);
        LONGS_EQUAL(expected_notified_state, context->notified_state);

        free(context);
    }
};

TEST(dsl_state_connecting, dsl_event_line_up_handler)
{
    dsl_event_line_up_handler(context);
}

TEST(dsl_state_connecting, dsl_event_line_down_handler)
{
    mock().expectOneCall("dsl_ppp_stop");

    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->connecting_timer);

    dsl_event_line_down_handler(context);

    expected_is_line_up = false;
    expected_current_state = dsl_state_disconnected;
}

TEST(dsl_state_connecting, dsl_event_connecting_timeout_handler)
{
    mock().setData("connecting_timer_is_running", false);

    mock().expectOneCall("dsl_ppp_stop");

    mock().expectOneCall("dsl_ppp_start");

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->connecting_timer);

    dsl_event_connecting_timeout_handler(context);
}

TEST(dsl_state_connecting, dsl_event_delay_up_timeout_handler)
{
    dsl_event_delay_up_timeout_handler(context);
}

TEST(dsl_state_connecting, dsl_event_disconnecting_timeout_handler)
{
    dsl_event_disconnecting_timeout_handler(context);
}

TEST(dsl_state_connecting, dsl_event_connect_message_handler)
{
    dsl_event_connect_message_handler(context);
}

TEST(dsl_state_connecting, dsl_event_disconnect_message_handler)
{
    mock().expectOneCall("dsl_ppp_stop");

    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->connecting_timer);

    dsl_event_disconnect_message_handler(context);

    expected_desired_state = dsl_state_disconnected;
    expected_current_state = dsl_state_disconnected;
    expected_notified_state = dsl_state_disconnected;
}

TEST(dsl_state_connecting, dsl_event_connected_message_handler)
{
    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->connecting_timer);

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->delay_up_timer);

    dsl_event_connected_message_handler(context);

    expected_current_state = dsl_state_connected_delay_up;
}

TEST(dsl_state_connecting, dsl_event_disconnected_message_handler)
{
    dsl_event_disconnected_message_handler(context);
}

TEST_GROUP(dsl_state_connected_delay_up)
{
    dsl_context_t * context;
    bool expected_is_line_up;
    dsl_state_t expected_desired_state;
    dsl_state_t expected_current_state;
    dsl_state_t expected_notified_state;

    void setup()
    {
        context = (dsl_context_t *)calloc(1, sizeof *context);

        mock().strictOrder();
        mock().setData("context", context);
        mock().setData("delay_up_timer_is_running", true);

        expected_is_line_up = context->is_line_up = true;
        expected_desired_state = context->desired_state = dsl_state_connected;
        expected_current_state = context->current_state = dsl_state_connected_delay_up;
        expected_notified_state = context->notified_state = dsl_state_disconnected;
    }

    void teardown()
    {
        LONGS_EQUAL(expected_is_line_up, context->is_line_up);
        LONGS_EQUAL(expected_desired_state, context->desired_state);
        LONGS_EQUAL(expected_current_state, context->current_state);
        LONGS_EQUAL(expected_notified_state, context->notified_state);

        free(context);
    }
};

TEST(dsl_state_connected_delay_up, dsl_event_line_up_handler)
{
    dsl_event_line_up_handler(context);
}

TEST(dsl_state_connected_delay_up, dsl_event_line_down_handler)
{
    mock().expectOneCall("dsl_ppp_stop");

    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->delay_up_timer);

    dsl_event_line_down_handler(context);

    expected_is_line_up = false;
    expected_current_state = dsl_state_disconnected;
}

TEST(dsl_state_connected_delay_up, dsl_event_connecting_timeout_handler)
{
    dsl_event_connecting_timeout_handler(context);
}

TEST(dsl_state_connected_delay_up, dsl_event_delay_up_timeout_handler)
{
    mock().setData("disconnecting_timer_is_running", false);

    mock().expectOneCall("send_message")
          .withParameter("message_code", connected_message);

    dsl_event_delay_up_timeout_handler(context);

    expected_current_state = dsl_state_connected;
    expected_notified_state = dsl_state_connected;
}

TEST(dsl_state_connected_delay_up, dsl_event_disconnecting_timeout_handler)
{
    dsl_event_disconnecting_timeout_handler(context);
}

TEST(dsl_state_connected_delay_up, dsl_event_connect_message_handler)
{
    dsl_event_connect_message_handler(context);
}

TEST(dsl_state_connected_delay_up, dsl_event_disconnect_message_handler)
{
    mock().expectOneCall("dsl_ppp_stop");

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->disconnecting_timer);

    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->delay_up_timer);

    dsl_event_disconnect_message_handler(context);

    expected_desired_state = dsl_state_disconnected;
    expected_current_state = dsl_state_disconnecting;
}

TEST(dsl_state_connected_delay_up, dsl_event_connected_message_handler)
{
    dsl_event_connected_message_handler(context);
}

TEST(dsl_state_connected_delay_up, dsl_event_disconnected_message_handler)
{
    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->delay_up_timer);

    mock().expectOneCall("dsl_ppp_start");

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->connecting_timer);

    dsl_event_disconnected_message_handler(context);

    expected_current_state = dsl_state_connecting;
}

TEST_GROUP(dsl_state_connected)
{
    dsl_context_t * context;
    bool expected_is_line_up;
    dsl_state_t expected_desired_state;
    dsl_state_t expected_current_state;
    dsl_state_t expected_notified_state;

    void setup()
    {
        context = (dsl_context_t *)calloc(1, sizeof *context);

        mock().strictOrder();
        mock().setData("context", context);

        expected_is_line_up = context->is_line_up = true;
        expected_desired_state = context->desired_state = dsl_state_connected;
        expected_current_state = context->current_state = dsl_state_connected;
        expected_notified_state = context->notified_state = dsl_state_connected;
    }

    void teardown()
    {
        LONGS_EQUAL(expected_is_line_up, context->is_line_up);
        LONGS_EQUAL(expected_desired_state, context->desired_state);
        LONGS_EQUAL(expected_current_state, context->current_state);
        LONGS_EQUAL(expected_notified_state, context->notified_state);

        free(context);
    }
};

TEST(dsl_state_connected, dsl_event_line_up_handler)
{
    dsl_event_line_up_handler(context);
}

TEST(dsl_state_connected, dsl_event_line_down_handler)
{
    mock().expectOneCall("dsl_ppp_stop");

    mock().expectOneCall("send_message")
          .withParameter("message_code", disconnected_message);

    dsl_event_line_down_handler(context);

    expected_is_line_up = false;
    expected_current_state = dsl_state_disconnected;
    expected_notified_state = dsl_state_disconnected;
}

TEST(dsl_state_connected, dsl_event_connecting_timeout_handler)
{
    dsl_event_connecting_timeout_handler(context);
}

TEST(dsl_state_connected, dsl_event_delay_up_timeout_handler)
{
    dsl_event_delay_up_timeout_handler(context);
}

TEST(dsl_state_connected, dsl_event_disconnecting_timeout_handler)
{
    dsl_event_disconnecting_timeout_handler(context);
}

TEST(dsl_state_connected, dsl_event_connect_message_handler)
{
    dsl_event_connect_message_handler(context);
}

TEST(dsl_state_connected, dsl_event_disconnect_message_handler)
{
    mock().expectOneCall("dsl_ppp_stop");

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->disconnecting_timer);

    dsl_event_disconnect_message_handler(context);

    expected_desired_state = dsl_state_disconnected;
    expected_current_state = dsl_state_disconnecting;
}

TEST(dsl_state_connected, dsl_event_connected_message_handler)
{
    dsl_event_connected_message_handler(context);
}

TEST(dsl_state_connected, dsl_event_disconnected_message_handler)
{
    mock().expectOneCall("send_message")
          .withParameter("message_code", disconnected_message);

    mock().expectOneCall("dsl_ppp_start");

    mock().expectOneCall("dsl_timer_start")
          .withParameter("timer", (void *)&context->connecting_timer);

    dsl_event_disconnected_message_handler(context);

    expected_current_state = dsl_state_connecting;
    expected_notified_state = dsl_state_disconnected;
}

TEST_GROUP(dsl_state_disconnecting)
{
    dsl_context_t * context;
    bool expected_is_line_up;
    dsl_state_t expected_desired_state;
    dsl_state_t expected_current_state;
    dsl_state_t expected_notified_state;

    void setup()
    {
        context = (dsl_context_t *)calloc(1, sizeof *context);

        mock().strictOrder();
        mock().setData("context", context);
        mock().setData("disconnecting_timer_is_running", true);

        expected_is_line_up = context->is_line_up = true;
        expected_desired_state = context->desired_state = dsl_state_disconnected;
        expected_current_state = context->current_state = dsl_state_disconnecting;
        expected_notified_state = context->notified_state = dsl_state_connected;
    }

    void teardown()
    {
        LONGS_EQUAL(expected_is_line_up, context->is_line_up);
        LONGS_EQUAL(expected_desired_state, context->desired_state);
        LONGS_EQUAL(expected_current_state, context->current_state);
        LONGS_EQUAL(expected_notified_state, context->notified_state);

        free(context);
    }
};

TEST(dsl_state_disconnecting, dsl_event_line_up_handler)
{
    dsl_event_line_up_handler(context);
}

TEST(dsl_state_disconnecting, dsl_event_line_down_handler)
{
    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->disconnecting_timer);

    mock().expectOneCall("send_message")
          .withParameter("message_code", disconnected_message);

    dsl_event_line_down_handler(context);

    expected_is_line_up = false;
    expected_current_state = dsl_state_disconnected;
    expected_notified_state = dsl_state_disconnected;
}

TEST(dsl_state_disconnecting, dsl_event_connecting_timeout_handler)
{
    dsl_event_connecting_timeout_handler(context);
}

TEST(dsl_state_disconnecting, dsl_event_delay_up_timeout_handler)
{
    dsl_event_delay_up_timeout_handler(context);
}

TEST(dsl_state_disconnecting, dsl_event_disconnecting_timeout_handler)
{
    mock().setData("disconnecting_timer_is_running", false);

    mock().expectOneCall("send_message")
          .withParameter("message_code", disconnected_message);

    dsl_event_disconnecting_timeout_handler(context);

    expected_current_state = dsl_state_disconnected;
    expected_notified_state = dsl_state_disconnected;
}

TEST(dsl_state_disconnecting, dsl_event_connect_message_handler)
{
    dsl_event_connect_message_handler(context);

    expected_desired_state = dsl_state_connected;
}

TEST(dsl_state_disconnecting, dsl_event_disconnect_message_handler)
{
    dsl_event_disconnect_message_handler(context);
}

TEST(dsl_state_disconnecting, dsl_event_connected_message_handler)
{
    mock().expectOneCall("dsl_ppp_stop");

    dsl_event_connected_message_handler(context);
}

TEST(dsl_state_disconnecting, dsl_event_disconnected_message_handler)
{
    mock().expectOneCall("dsl_timer_stop")
          .withParameter("timer", (void *)&context->disconnecting_timer);

    mock().expectOneCall("send_message")
          .withParameter("message_code", disconnected_message);

    dsl_event_disconnected_message_handler(context);

    expected_current_state = dsl_state_disconnected;
    expected_notified_state = dsl_state_disconnected;
}
