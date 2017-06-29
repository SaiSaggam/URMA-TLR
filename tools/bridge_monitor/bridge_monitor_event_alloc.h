#ifndef __BRIDGE_MONITOR_EVENT_ALLOC_H__
#define __BRIDGE_MONITOR_EVENT_ALLOC_H__

#include "bridge_monitor_event.h"
#include <stddef.h>

bridge_monitor_event_t * bridgemon_event_alloc(int code, size_t instance, void * * const info, size_t const info_size);
void bridgemon_event_free(bridge_monitor_event_t * event);

#if defined(UNIT_TEST)
void unit_test_free(void * __ptr);
#define EVENT_ALLOC_FREE unit_test_free
#else
#define EVENT_ALLOC_FREE free
#endif

#endif /* __BRIDGE_MONITOR_EVENT_ALLOC_H__ */
