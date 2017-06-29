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

#ifndef __TLR_POPEN_H__
#define __TLR_POPEN_H__

#include <stdio.h>
#include <stdbool.h>

FILE * tlr_popen(char  const * const command, char const * const mode);
// tlr_popen_read_write() provides both read and write FILE streams with the process, only need to close once with the same
// tlr_pclose() with either of the FILE stream pointers that are returned.
bool tlr_popen_read_write(char  const * const command, char const * const mode, FILE ** const read_handle, FILE ** const write_handle);
int tlr_pclose (FILE * const process);
int tlr_system_command(char const * const format, ...);
int tlr_system_command_with_response(char ** const response, size_t * const resp_len, char const * const format, ...);


#endif /* __TLR_POPEN_H__ */
