#ifndef __ARGS_H__
#define __ARGS_H__

#include <stddef.h>

typedef struct args_st
{
	size_t max_args;
	int argc;
	char **argv;
} args_st;

args_st * malloc_args_st(size_t initial_size);
void free_args_st(args_st *args);
int add_arg(args_st * const args, char const * const arg);

#endif /* __ARGS_H__ */
