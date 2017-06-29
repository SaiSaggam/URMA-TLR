#include "args.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INCREMENT	1

args_st * malloc_args_st(size_t initial_size)
{
	args_st *args;

	args = calloc(1, sizeof *args);
    if (args == NULL)
    {
        errno = ENOMEM;
		goto error;
    }
	if (initial_size > 0)
	{
		args->max_args = initial_size;
		args->argv = calloc(initial_size, sizeof (char *));
        if (args->argv == NULL)
        {
            errno = ENOMEM;
			goto error;
        }
	}

	goto done;

error:
	if (args != NULL)
	{
		if (args->argv != NULL)
		{
			free(args->argv);
		}
		free(args);
		args = NULL;
	}
done:
	return args;
}

void free_args_st(args_st *args)
{
	if (args != NULL)
	{
		if (args->argv != NULL)
		{
			int i;

			for (i=0; i < args->argc; i++)
			{
				free(args->argv[i]);
			}
			free(args->argv);
		}
		free(args);
	}
}

int add_arg(args_st * const args, char const * const arg)
{
    int result;

	if (args->argc == args->max_args)
	{
		args->argv = realloc(args->argv, (args->max_args + INCREMENT) * sizeof (char *));
        if (args->argv == NULL)
        {
            errno = ENOMEM;
            result = -1;
            goto done;
        }
        args->max_args += INCREMENT;
	}
    if (arg != NULL)
    {
		args->argv[args->argc] = strdup(arg);
        if (args->argv[args->argc] == NULL)
        {
            errno = ENOMEM;
            result = -1;
            goto done;
        }
        args->argc++;
    }
	else
		args->argv[args->argc] = NULL;

    result = 0;

done:
	return result;
}

