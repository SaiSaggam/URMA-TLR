#ifndef __TIMERSPEC_HELPERS_H__
#define __TIMERSPEC_HELPERS_H__

#include <time.h>

int timerspec_compare(struct timespec const * const a, struct timespec const * const b);
void timerspec_add(struct timespec const * const a, struct timespec const * const b, struct timespec * const result);
void timerspec_sub(struct timespec const * const a, struct timespec const * const b, struct timespec * const result);


#endif /* __TIMERSPEC_HELPERS_H__ */
