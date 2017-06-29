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

#ifndef HAVE_STRLCPY

#include "strlcpy.h"

#include <stdlib.h>
#include <string.h>

size_t                  	/* O - Length of string */
strlcpy(char       *dst,    /* O - Destination string */
        const char *src,    /* I - Source string */
        size_t      size)   /* I - Size of destination string buffer */
{
	size_t    srclen;       /* Length of source string */


	/*
	* Figure out how much room is needed...
	*/

	size --;

	srclen = strlen(src);

	/*
	* Copy the appropriate amount...
	*/

	if (srclen > size)
		srclen = size;

	memcpy(dst, src, srclen);
	dst[srclen] = '\0';

	return srclen;
}

#endif /* !HAVE_STRLCPY */

