#ifndef __PROBING_H__
#define __PROBING_H__

#include <stddef.h>
#include <stdint.h>

typedef struct probe_context_st probe_context_st;

probe_context_st * send_probe(char const * const host,
                              char const * const interface,
                              uint32_t probe_id,
                              size_t const size,
                              void (*success_cb)(void * arg, uint32_t probe_id),
                              void * const arg,
                              size_t timeout_seconds);
void free_probe(probe_context_st * probe_context);



#endif /* __PROBING_H__ */
