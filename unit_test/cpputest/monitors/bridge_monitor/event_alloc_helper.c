#include "event_alloc_helper.h"
#include <stdlib.h>

#define MAX_FREED_POINTERS 2
#define BOGUS_MEMORY_ADDRESS (void *)0xffffffff
static void * calloc_result;
static void * pointers_freed[MAX_FREED_POINTERS];
static size_t pointers_freed_count;

void event_alloc_spy_set_calloc_result(void * result)
{
    calloc_result = result;
}

void * calloc(size_t __nmemb, size_t __size)
{
    return calloc_result;
}

void event_alloc_spy_init(void)
{
    calloc_result = NULL;
    pointers_freed_count = 0;
}

void * event_alloc_spy_get_freed_pointer(size_t index)
{
    void * result;

    if (index < pointers_freed_count && index < MAX_FREED_POINTERS)
    {
        result = pointers_freed[index];
    }
    else
    {
        result = BOGUS_MEMORY_ADDRESS;
    }
    return result;
}

size_t event_alloc_spy_get_freed_count(void)
{
    return pointers_freed_count;
}

void unit_test_free(void * __ptr)
{
    if (pointers_freed_count < MAX_FREED_POINTERS)
    {
        pointers_freed[pointers_freed_count] = __ptr;
    }
    pointers_freed_count++;
}
