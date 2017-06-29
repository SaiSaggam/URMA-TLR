#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"

/*
 * This is where we include the header files of the C code under test
 */
extern "C"
{
    /* include C header files of code under test */
    #include "config_utils.h"
    #include "wifi_radio.h"
    #include "wifi24/wifi_radio_24.h"
    #include "wifi_monitor.h"
    #include "wifi_monitor_config.h"
    #include "wifi_monitor_event_handlers.h"
    #include "wifi_monitor_radio_config_commands.h"
}

int instance = 1234;


void wifi_handle_connected_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_connected_event")
        .withParameter("context", (void *)context);
}

void wifi_handle_disconnected_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_disconnected_event")
        .withParameter("context", (void *)context);
}


bool wifi_radio_implement_config_change_24(size_t const instance, wifi_common_elements_t * const wifi_configuration, bool * const connection_state)
{
    mock()
        .actualCall("wifi_radio_implement_config_change_24")
        .withParameter("instance", (int)instance)
        .withParameter("wifi_configuration", (void *)wifi_configuration)
        .withOutputParameter("connection_state", connection_state);


    return (bool)mock().returnIntValueOrDefault(false);
}

TEST_GROUP(config)
{
    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(config, wifimon_config_check_for_change_returns_false_if_configs_match)
{
    wifi_common_elements_t old_config;
    wifi_common_elements_t new_config;
    bool result;
    char * const ssid1 = (char *)"SSID1";
    char * const password1 = (char *)"PASSWORD1";

    old_config.wifi_param_ssid = ssid1;
    old_config.wifi_param_security = config_wifi_security_wpa2_personal;
    old_config.wifi_param_password = password1;

    new_config.wifi_param_ssid = ssid1;
    new_config.wifi_param_security = config_wifi_security_wpa2_personal;
    new_config.wifi_param_password = password1;

    /* perform test */
    result = wifi_config_check_for_change(instance, &old_config, &new_config);

    /* check results */
    CHECK_FALSE(result);
    mock().checkExpectations();
}

TEST(config, wifimon_config_check_for_change_returns_true_if_old_config_is_null)
{
    wifi_common_elements_t new_config;
    char * const new_password = (char *)"PASSWORD2";
    char * const new_ssid = (char *)"SSID2";
    bool result;

    new_config.wifi_param_ssid = new_ssid;
    new_config.wifi_param_security = config_wifi_security_wpa2_personal;
    new_config.wifi_param_password = new_password;

    /* perform test */
    result = wifi_config_check_for_change(instance, NULL, &new_config);

    /* check results */
    CHECK_TRUE(result);
    mock().checkExpectations();
}

TEST(config, wifimon_config_check_for_change_returns_true_if_new_config_is_null)
{
    wifi_common_elements_t old_config;
    bool result;

    char * const ssid1 = (char *)"SSID1";
    char * const password1 = (char *)"PASSWORD1";

    old_config.wifi_param_ssid = ssid1;
    old_config.wifi_param_security = config_wifi_security_wpa2_personal;
    old_config.wifi_param_password = password1;

    /* perform test */
    result = wifi_config_check_for_change(instance, &old_config, NULL);

    /* check results */
    CHECK_TRUE(result);
    mock().checkExpectations();
}

TEST(config, wifimon_config_check_for_change_returns_true_if_passwords_differ)
{
    wifi_common_elements_t old_config;
    wifi_common_elements_t new_config;
    char * const ssid1 = (char *)"SSID1";
    char * const password1 = (char *)"PASSWORD1";
    char * const password2 = (char *)"PASSWORD2";
    bool result;

    old_config.wifi_param_ssid = ssid1;
    old_config.wifi_param_security = config_wifi_security_wpa2_personal;
    old_config.wifi_param_password = password1;

    new_config.wifi_param_ssid = ssid1;
    new_config.wifi_param_security = config_wifi_security_wpa2_personal;
    new_config.wifi_param_password = password2;

    /* perform test */
    result = wifi_config_check_for_change(instance, &old_config,  &new_config);

    /* check results */
    CHECK_TRUE(result);
    mock().checkExpectations();
}

TEST(config, wifimon_config_check_for_change_returns_true_if_security_differs)
{
    wifi_common_elements_t old_config;
    wifi_common_elements_t new_config;
    char * const ssid1 = (char *)"SSID1";
    char * const password1 = (char *)"PASSWORD1";
    bool result;

    old_config.wifi_param_ssid = ssid1;
    old_config.wifi_param_security = config_wifi_security_wpa2_personal;
    old_config.wifi_param_password = password1;

    new_config.wifi_param_ssid = ssid1;
    new_config.wifi_param_security = config_wifi_security_wpa_wpa2_personal;
    new_config.wifi_param_password = password1;

    /* perform test */
    result = wifi_config_check_for_change(instance, &old_config,  &new_config);

    /* check results */
    CHECK_TRUE(result);
    mock().checkExpectations();
}

TEST(config, wifimon_config_check_for_change_returns_true_if_ssids_differ)
{
    wifi_common_elements_t old_config;
    wifi_common_elements_t new_config;
    char * const ssid1 = (char *)"SSID1";
    char * const ssid2 = (char *)"SSID2";
    char * const password1 = (char *)"PASSWORD1";
    bool result;

    old_config.wifi_param_ssid = ssid1;
    old_config.wifi_param_security = config_wifi_security_wpa2_personal;
    old_config.wifi_param_password = password1;

    new_config.wifi_param_ssid = ssid2;
    new_config.wifi_param_security = config_wifi_security_wpa2_personal;
    new_config.wifi_param_password = password1;

    /* perform test */
    result = wifi_config_check_for_change(instance, &old_config,  &new_config);

    /* check results */
    CHECK_TRUE(result);
    mock().checkExpectations();
}

TEST_GROUP(config_has_changed)
{
    wifi_context_st context;

    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(config_has_changed, does_nothing_if_implement_config_returns_false)
{
    int instance = 1234;
    wifi_common_elements_t wifi_mon_config;
    bool connection_state;

    /* setup */
    connection_state = false;
    context.instance = instance;

    mock().expectOneCall("wifi_radio_implement_config_change_24")
        .withOutputParameterReturning("connection_state", &connection_state, sizeof connection_state)
        .withParameter("instance", instance)
        .withParameter("wifi_configuration", (void *)&wifi_mon_config)
        .andReturnValue(false);


    // perform test
    wifi_config_has_changed(&context, &wifi_mon_config);

    /* check results */
    mock().checkExpectations();
}

TEST(config_has_changed, calls_handle_connected_event_if_connection_state_is_true)
{
    int instance = 1234;
    wifi_common_elements_t wifi_mon_config;
    bool connection_state;

    /* setup */
    connection_state = true;
    context.instance = instance;

    mock().expectOneCall("wifi_radio_implement_config_change_24")
        .withOutputParameterReturning("connection_state", &connection_state, sizeof connection_state)
        .withParameter("instance", instance)
        .withParameter("wifi_configuration", (void *)&wifi_mon_config)
        .andReturnValue(true);
    mock()
        .expectOneCall("wifi_handle_connected_event")
        .withParameter("context", &context);


    // perform test
    wifi_config_has_changed(&context, &wifi_mon_config);

    /* check results */
    mock().checkExpectations();
}

TEST(config_has_changed, calls_handle_disconnected_event_if_connection_state_is_false)
{
    int instance = 1234;
    wifi_common_elements_t wifi_mon_config;
    bool connection_state;

    /* setup */
    connection_state = false;
    context.instance = instance;

    mock().expectOneCall("wifi_radio_implement_config_change_24")
        .withOutputParameterReturning("connection_state", &connection_state, sizeof connection_state)
        .withParameter("instance", instance)
        .withParameter("wifi_configuration", (void *)&wifi_mon_config)
        .andReturnValue(true);
    mock()
        .expectOneCall("wifi_handle_disconnected_event")
        .withParameter("context", &context);


    // perform test
    wifi_config_has_changed(&context, &wifi_mon_config);

    /* check results */
    mock().checkExpectations();
}

