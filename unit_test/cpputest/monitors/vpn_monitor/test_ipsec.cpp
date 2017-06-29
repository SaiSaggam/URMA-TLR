#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"

#define EXPECTED_PKILL_PATH "/usr/bin/pkill"
/*
 * This is where we include the header files of the C code under test
 */
extern "C"
{
    /* include C header files of code under test */
    #include <ipsec.h>
    #include <digiutils.h>
}

/* mock functions */
int system_command(char const * const format, ...)
{
    char * command = NULL;
    va_list args;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        mock().actualCall("system_command").withParameter("command", command);
        free(command);
    }
    va_end(args);

    return mock().returnValue().getIntValue();
}

/* tests */
TEST_GROUP(ipsec_start_strongswan)
{
    void setup()
    {
        mock().clear();
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(ipsec_start_strongswan, normal)
{
    int expected_value;
    int actual_value;

    /* setup */
    expected_value = 0;
    mock().expectOneCall("system_command").withParameter("command", "/usr/etc/init.d/init_strongswan.sh start").andReturnValue(0);
    /* perform test */
    actual_value = ipsec_start_strongswan();

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();

}

TEST(ipsec_start_strongswan, failed_system_call_results_in_error)
{
    int expected_value;
    int actual_value;

    /* setup */
    expected_value = -1;
    mock().expectOneCall("system_command").withParameter("command", "/usr/etc/init.d/init_strongswan.sh start").andReturnValue(-1);
    /* perform test */
    actual_value = ipsec_start_strongswan();

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();

}

TEST_GROUP(ipsec_reload_configuration)
{
    void setup()
    {
        mock().clear();
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(ipsec_reload_configuration, normal)
{
    int expected_value;
    int actual_value;

    /* setup */
    expected_value = 0;
    mock().expectOneCall("system_command").withParameter("command", "/usr/etc/init.d/init_strongswan.sh reload").andReturnValue(0);
    /* perform test */
    actual_value = ipsec_reload_configuration();

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();

}

TEST(ipsec_reload_configuration, failed_system_call_results_in_error)
{
    int expected_value;
    int actual_value;

    /* setup */
    expected_value = -1;
    mock().expectOneCall("system_command").withParameter("command", "/usr/etc/init.d/init_strongswan.sh reload").andReturnValue(-1);
    /* perform test */
    actual_value = ipsec_reload_configuration();

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();

}

TEST_GROUP(ipsec_start_tunnel)
{
    void setup()
    {
        mock().clear();
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(ipsec_start_tunnel, normal)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = 0;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", EXPECTED_PKILL_PATH " -f \"/usr/libexec/strongswan/stroke up test_name\"").andReturnValue(0);
    mock().expectOneCall("system_command").withParameter("command", "/usr/sbin/ipsec up test_name &").andReturnValue(0);
    /* perform test */
    actual_value = ipsec_start_tunnel(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(ipsec_start_tunnel, failed_pkill_should_not_result_in_error)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = 0;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", EXPECTED_PKILL_PATH " -f \"/usr/libexec/strongswan/stroke up test_name\"").andReturnValue(-1);
    mock().expectOneCall("system_command").withParameter("command", "/usr/sbin/ipsec up test_name &").andReturnValue(0);
    /* perform test */
    actual_value = ipsec_start_tunnel(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(ipsec_start_tunnel, failed_stop_tunnel_should_not_result_in_error)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = 0;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", EXPECTED_PKILL_PATH " -f \"/usr/libexec/strongswan/stroke up test_name\"").andReturnValue(0);
    mock().expectOneCall("system_command").withParameter("command", "/usr/sbin/ipsec up test_name &").andReturnValue(0);
    /* perform test */
    actual_value = ipsec_start_tunnel(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(ipsec_start_tunnel, failed_ipsec_up_results_in_error)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = -1;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", EXPECTED_PKILL_PATH " -f \"/usr/libexec/strongswan/stroke up test_name\"").andReturnValue(0);
    mock().expectOneCall("system_command").withParameter("command", "/usr/sbin/ipsec up test_name &").andReturnValue(-1);
    /* perform test */
    actual_value = ipsec_start_tunnel(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST_GROUP(ipsec_stop_tunnel)
{
    void setup()
    {
        mock().clear();
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(ipsec_stop_tunnel, normal)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = 0;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", "/usr/sbin/ipsec down test_name").andReturnValue(0);
    /* perform test */
    actual_value = ipsec_stop_tunnel(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(ipsec_stop_tunnel, failed_system_call_results_in_error)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = -1;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", "/usr/sbin/ipsec down test_name").andReturnValue(-1);
    /* perform test */
    actual_value = ipsec_stop_tunnel(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST_GROUP(ipsec_update_tunnel_configuration)
{
    void setup()
    {
        mock().clear();
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(ipsec_update_tunnel_configuration, normal)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = 0;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", "/usr/etc/init.d/init_strongswan.sh update").andReturnValue(0);

    /* perform test */
    actual_value = ipsec_update_tunnel_configuration(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(ipsec_update_tunnel_configuration, failed_update_should_result_in_error)
{
    int expected_value;
    int actual_value;
    vpn_tunnel_context_st context;

    /* setup */
    expected_value = -1;
    strcpy(context.name, "test_name");
    mock().expectOneCall("system_command").withParameter("command", "/usr/etc/init.d/init_strongswan.sh update").andReturnValue(-1);

    /* perform test */
    actual_value = ipsec_update_tunnel_configuration(&context);

    /* check result */
    LONGS_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}


