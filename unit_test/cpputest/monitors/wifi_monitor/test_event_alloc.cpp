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
    #include "wifi_monitor_event.h"
    #include "wifi_monitor_event_alloc.h"

}

void * unit_test_malloc(size_t const size)
{
    void * mem;

    mock().actualCall("malloc").withParameter("size", size);

    if (mock().hasReturnValue())
    {
        mem = mock().returnValue().getPointerValue();
    }
    else
    {
        mem = malloc(size);
    }
    return mem;
}

void unit_test_free(void * const mem)
{
    mock().actualCall("free").withParameter("mem", (void *)mem);

    free(mem);
}

/* tests */
TEST_GROUP(event_alloc)
{
    wifi_monitor_event_t * event;

    void setup()
    {
        event = NULL;
    }

    void teardown()
    {
        mock().disable();
        wifi_monitor_event_free(event);
        mock().enable();
        mock().clear();
    }
};

TEST(event_alloc, wifi_monitor_event_alloc_assigns_fields_as_expected)
{
    int code;
    size_t instance;
    void * info;
    size_t info_size;
    void * info_expected;

    /* setup */
    code = 1234;
    instance = 5678;
    info_expected = info = malloc(1);
    info_size = 1111;
    mock().expectOneCall("malloc").withParameter("size", sizeof(*event));

    /* perform test */
    event = wifi_monitor_event_alloc(code, instance, &info, info_size);

    /* check results */
    CHECK_TRUE(event != NULL);
    CHECK_EQUAL(code, event->code);
    POINTERS_EQUAL(NULL, info);
    POINTERS_EQUAL(info_expected, event->info);
    CHECK_EQUAL(info_size, event->info_size);
    mock().checkExpectations();
}

TEST(event_alloc, wifi_monitor_event_alloc_null_info)
{
    int code;
    size_t instance;
    size_t info_size;
    void * info_expected;

    /* setup */
    code = 1234;
    instance = 5678;
    info_expected = NULL;
    info_size = 1111;
    mock().expectOneCall("malloc").withParameter("size", sizeof(*event));

    /* perform test */
    event = wifi_monitor_event_alloc(code, instance, NULL, info_size);

    /* check results */
    CHECK_TRUE(event != NULL);
    CHECK_EQUAL(code, event->code);
    POINTERS_EQUAL(info_expected, event->info);
    CHECK_EQUAL(info_size, event->info_size);
    mock().checkExpectations();
}

TEST(event_alloc, wifi_monitor_event_alloc_failed_malloc_returns_null)
{
    int code;
    size_t instance;
    void * info;
    size_t info_size;

    /* setup */
    code = 1234;
    instance = 5678;
    info = NULL;
    info_size = 1111;
    mock().expectOneCall("malloc")
        .withParameter("size", sizeof(*event))
        .andReturnValue((void *)NULL);

    /* perform test */

    event = wifi_monitor_event_alloc(code, instance, &info, info_size);

    /* check results */
    POINTERS_EQUAL(NULL, event);
    mock().checkExpectations();
}

TEST_GROUP(event_free)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(event_free, does_nothing_if_event_is_null)
{
    wifi_monitor_event_t * event;

    /* setup */
    event = NULL;

    /* perform test */
    wifi_monitor_event_free(event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_free, frees_info_followed_by_event)
{
    wifi_monitor_event_t * event;
    void const * info;

    /* setup */
    event = (wifi_monitor_event_t *)malloc(sizeof *event);
    CHECK_TRUE(event != NULL);
    event->info = malloc(1);
    CHECK_TRUE(event->info != NULL);
    info = event->info;
    mock().strictOrder();
    mock().expectOneCall("free").withParameter("mem", (void *)info);
    mock().expectOneCall("free").withParameter("mem", event);

    /* perform test */
    wifi_monitor_event_free(event);

    /* check results */
    mock().checkExpectations();
}
