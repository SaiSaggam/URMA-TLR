#ifndef __EVENT_ALLOC_HELPER_H__
#define __EVENT_ALLOC_HELPER_H__

#include <stddef.h>

void event_alloc_spy_set_calloc_result(void * result);
void event_alloc_spy_init(void);
void * event_alloc_spy_get_freed_pointer(size_t index);
size_t event_alloc_spy_get_freed_count(void);



#endif /* __EVENT_ALLOC_HELPER_H__ */
