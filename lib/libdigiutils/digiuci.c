/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#ifdef __DIGIUCI_TEST__
#include <stdio.h>
#endif
#include <stdlib.h>
#include <strlcpy.h>
#include <str_replace_char.h>

/*
	compare_digi_name_with_uci_name
    compare a 'Digi' string to a 'UCI' string
    A Digi string may contain '-' characters
    For the UCI string to match, the UCI character in the same position as the '-'
    must be a '_'
    For other characters, they must match exactly (case sensitive).
*/
int digi_name_matches_uci_name(char const * const digi_name, char const * const uci_name)
{
	char const * digi = digi_name;
	char const * uci = uci_name;

	while (*digi != '\0')
	{
		if (*digi == '-')
		{
			if (*uci != '_')
				break;
		}
		else if (*digi != *uci)
			break;
		digi++;
		uci++;
	}

	return *uci == '\0' && *digi == '\0';
}

/*
	strcpy_uci_to_digi
	copy the string in uci to digi,
	size should be set to the size of the destination buffer
	'_' chars in uci are converted to '-' which is required
	just like strlcpy except for the character conversion.
*/
int strlcpy_uci_to_digi(char * const digi, char const * const uci, size_t size)
{
	int result;

	result = strlcpy(digi, uci, size);
	str_replace_char(digi, '_', '-');

	return result;
}

/*
	strcpy_uci_to_digi
	copy the string in uci to digi,
	size should be set to the size of the destination buffer
	'_' chars in uci are converted to '-' which is required
	just like strlcpy except for the character conversion.
*/
int strlcpy_digi_to_uci(char * const digi, char const * const uci, size_t size)
{
	int result;

	result = strlcpy(digi, uci, size);
	str_replace_char(digi, '-', '_');

	return result;
}

#ifdef __DIGIUCI_TEST__
// make command:
// gcc -D__DIGIUCI_TEST__ -o test digiucicmp.c

int main(int argc, char ** argv)
{
	int result = -1;
	if (argc == 3)
	{
		printf("%s %s %d\n", argv[1], argv[2], digi_name_matches_uci_name(argv[1], argv[2]));
		result = 0;
	}
	else
	{
		printf("usage %s <digi_str> <uci_str>\n", argv[0]);
	}

	return result;
}
#endif
