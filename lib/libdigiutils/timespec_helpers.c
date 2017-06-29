#include "timespec_helpers.h"
#include <stdio.h>
#define NANOSECONDS_PER_SECOND  1000000000

int timerspec_compare(struct timespec const * const a, struct timespec const * const b)
{
  int result;

  if (a->tv_sec < b->tv_sec)
	  result = -1;
  else if (a->tv_sec > b->tv_sec)
	  result = 1;
  else if (a->tv_nsec < b->tv_nsec)
	  result = -1;
  else if (a->tv_nsec > b->tv_nsec)
	  result = 1;
  else
	  result = 0;

  return result;
}

void timerspec_add(struct timespec const * const a, struct timespec const * const b, struct timespec * const result)
{
	result->tv_sec = a->tv_sec + b->tv_sec;
	result->tv_nsec = a->tv_nsec + b->tv_nsec;
	if (result->tv_nsec >= NANOSECONDS_PER_SECOND)
	{
		result->tv_sec++;
		result->tv_nsec -= NANOSECONDS_PER_SECOND;
	}
}

void timerspec_sub(struct timespec const * const a, struct timespec const * const b, struct timespec * const result)
{
	result->tv_sec = a->tv_sec - b->tv_sec;
	result->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (result->tv_nsec < 0)
	{
		--result->tv_sec;
		result->tv_nsec += NANOSECONDS_PER_SECOND;
	}
}



