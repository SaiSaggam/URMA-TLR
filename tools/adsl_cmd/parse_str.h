#ifndef __PARSE_STR_H__
#define __PARSE_STR_H__

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "ci_cmd.h"

#define	TYPE_U8		0
#define TYPE_U16	1
#define	TYPE_U32	2
#define TYPE_STR	3
#define TYPE_MAC	4

struct item {
	char *item_name;
	unsigned long item_CSID;
	unsigned char type;
};

int parse_str(char *str);
#endif //__PARSE_STR_H__
