#ifndef __TLR_EINTR_H__
#define __TLR_EINTR_H__

#include "tlr_print.h"
#define TLR_TEMP_FAILURE_RETRY(expression) \
  (__extension__ \
    ({ long int __result; \
       do { \
           __result = (long int) (expression); \
           if (__result == -1 && errno == EINTR) \
           { \
              tlr_debug("EINTR caught and handled\n"); \
           } \
       } \
       while (__result == -1L && errno == EINTR); \
       __result; }))


#endif /* __TLR_EINTR_H__ */
