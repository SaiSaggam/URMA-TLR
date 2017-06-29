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
    #include "bridge_monitor_event.h"
    #include "bridge_monitor_event_alloc.h"

    /* include helper code header files */
    #include "event_alloc_helper.h"
}

/* tests */
TEST_GROUP(event_alloc)
{
    bridge_monitor_event_t event_data;

    void setup()
    {
        memset(&event_data, 0, sizeof event_data);
        event_alloc_spy_init();
    }
    void teardown()
    {
    }

    void test_calloc(bridge_monitor_event_t * const calloc_result)
    {
        bridge_monitor_event_t * result;
        event_alloc_spy_set_calloc_result(calloc_result);

        result = bridgemon_event_alloc(0, 0, NULL, 0);

        POINTERS_EQUAL(calloc_result, result);
    }

    void test_assignments(int code, size_t instance, void * * info, size_t info_size)
    {
        void * saved_info = NULL;
        event_alloc_spy_set_calloc_result(&event_data);

        if (info != NULL)
        {
            saved_info = *info;
        }

        (void)bridgemon_event_alloc(code, instance, info, info_size);

        LONGS_EQUAL(code, event_data.code);
        LONGS_EQUAL(instance, event_data.instance);
        if (info)
        {
            POINTERS_EQUAL(saved_info, event_data.info);
            POINTERS_EQUAL(NULL, *info);
        }
        else
        {
            POINTERS_EQUAL(NULL, event_data.info);
        }
        LONGS_EQUAL(info_size, event_data.info_size);
    }

};

TEST(event_alloc, successful_calloc)
{
    test_calloc(&event_data);
}

TEST(event_alloc, failed_calloc)
{
    test_calloc(NULL);
}

TEST(event_alloc, code_assigned)
{
    test_assignments(1, 0, NULL, 0);
}

TEST(event_alloc, instance_assigned)
{
    test_assignments(0, 1, NULL, 0);
}

TEST(event_alloc, info_size_assigned)
{
    test_assignments(0, 0, NULL, 1);
}

TEST(event_alloc, info_assigned)
{
    char any_old_thing[] = "abc";
    void * test_info = any_old_thing;

    test_assignments(0, 0, &test_info, 1);
}

TEST_GROUP(event_free)
{
    bridge_monitor_event_t event_data;
    void setup()
    {
        event_alloc_spy_init();
    }
    void teardown()
    {
    }

    void check_free_calls(void * event_info)
    {
        event_data.info = event_info;

        bridgemon_event_free(&event_data);
        LONGS_EQUAL(2, event_alloc_spy_get_freed_count());
        POINTERS_EQUAL(event_info, event_alloc_spy_get_freed_pointer(0));
        POINTERS_EQUAL(&event_data, event_alloc_spy_get_freed_pointer(1));
    }
};

TEST(event_free, null_event_doesnt_call_free)
{
    bridgemon_event_free(NULL);
    LONGS_EQUAL(0, event_alloc_spy_get_freed_count());
}

TEST(event_free, event_calls_free_twice)
{
    check_free_calls(NULL);
}

TEST(event_free, event_info_freed)
{
    int event_info;
    check_free_calls(&event_info);
}
