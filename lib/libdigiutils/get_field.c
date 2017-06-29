/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/

#include "get_field.h"
#include "strip_whitespace.h"
#include <string.h>

/*
    search for a field in str using 'delim' as the token separator
    whitespace at the beginning and end of the field will be removed
    NB: The original string buffer may be modified.
    str: the string to be searched
    delim: the delimiter characters
    field_number: the field to search for (first field is 1, not 0)
    returns:
    	NULL if the if the field wasn't found
    	pointer to the field
*/
char * get_field(char * str, char const * const delim, int const field_number)
{
	char * field, *saveptr;
	int i = 0;

	do
	{
		field = strtok_r(str, delim, &saveptr);
		str = NULL;
		i++;
	} while (field != NULL && i < field_number);

	if (field != NULL)
		field = strip_whitespace(field);

	return field;
}

