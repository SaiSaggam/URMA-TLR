#ifndef __TLR_COMMON_H__
#define __TLR_COMMON_H__

#include <time.h>
#include <limits.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef asizeof
#define asizeof(x)  (sizeof(x) / sizeof((x)[0]))
#endif

#define STRING_IS_EMPTY(string) (((string) == NULL) || ((string)[0] == '\0'))

static inline long milliseconds_to_microseconds(unsigned int milliseconds)
{
    return milliseconds * 1000;
}

static inline time_t milliseconds_to_seconds(unsigned int milliseconds, long * remainder)
{
    time_t const seconds = milliseconds / 1000;
    if (remainder != NULL)
    {
        *remainder = milliseconds - seconds * 1000;
    }

    return seconds;
}

static inline long milliseconds_to_nanoseconds(unsigned int milliseconds)
{
    return milliseconds * 1000000;
}

#define seconds_to_milliseconds(seconds)  ((seconds) * 1000)

#define VALUE_TO_STRING(value)   # value
#define MACRO_TO_STRING(macro)   VALUE_TO_STRING(macro)

#define INTEGER_STRING_LENGTH   sizeof(MACRO_TO_STRING(UINT_MAX))
#define UCHAR_STRING_LENGTH   sizeof(MACRO_TO_STRING(CHAR_MAX))
#define ULONG_STRING_LENGTH   sizeof(MACRO_TO_STRING(ULONG_MAX))
#define UINT64_STRING_LENGTH   sizeof(MACRO_TO_STRING(UINT64_MAX))

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define IPADDR_STRING_LENGTH (sizeof "0000:0000:0000:0000:0000:0000:127.127.127.127")
#define MACADDR_STRING_LENGTH (sizeof "01:02:03:04:05:06")

#define MIN(x,y)  (((x) < (y)) ? (x) : (y))
#define MAX(x,y)  (((x) > (y)) ? (x) : (y))

#define FIRST_INSTANCE 1
#define INDEX_TO_INSTANCE(index)	((index) + FIRST_INSTANCE)
/* Default to index 0 if supplied with an invalid instance */
#define INSTANCE_TO_INDEX(instance)	(((instance) >= FIRST_INSTANCE) ? (instance) - FIRST_INSTANCE : 0)
#define INSTANCE_IS_VALID(instance, max_instance)	((instance) >= FIRST_INSTANCE && (instance) <= (max_instance))


#endif /* __TLR_COMMON_H__ */
