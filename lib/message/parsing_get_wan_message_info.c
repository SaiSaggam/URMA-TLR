#include "parsing.h"
#include "messages.h"
#include "tlr_print.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "unittest_support.h"

STATIC_FUNCTION bool gwmi_get_unsigned_integer_from_string(char const * const str, size_t * const unsigned_int)
{
    bool result;
    unsigned long value;

    if (str[0] == '-')
    {
        result = false;
        goto done;
    }
    errno = 0;
    value = strtoul(str, NULL, 10);

    if (errno == EINVAL || errno == ERANGE || value > SIZE_MAX)
    {
        result = false;
        goto done;
    }
    /*
     * If we don't clear errno once strtoul sets it, we see that the calling process will exit at
     * some later point. As I'm not certain that we've just checked all values that strtoul will set
     * errno to, I'll clear it no matter what value it might have.
     */
    if (errno != 0)
    {
        errno = 0;
    }
    *unsigned_int = (size_t)value;
    result = true;

done:
    return result;
}

int get_wan_message_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size)
{
    int result;
    wan_info_st * wan_info = NULL;

    if (argc < 1)
    {
        tlr_debug("%s: too few args to get wan info\n", __func__);
        goto error;
    }
    wan_info = calloc(1, sizeof *wan_info);
    if (wan_info == NULL)
    {
        goto error;
    }

    if (!gwmi_get_unsigned_integer_from_string(argv[0], &wan_info->wan_instance))
    {
        tlr_debug("%s: failed to get wan instance from '%s'\n", __func__, argv[0]);
        goto error;
    }

    /* everything is OK if we get here */
    *info_result = wan_info;
    *info_size = sizeof * wan_info;
    result = 0;
    goto done;

error:
    free(wan_info);
    result = -1;
done:
    return result;
}
