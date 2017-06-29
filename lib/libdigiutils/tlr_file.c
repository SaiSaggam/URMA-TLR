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

#include "tlr_file.h"
#include "tlr_common.h"
#include "tlr_print.h"
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int tlr_file_lock(char const * const lock_file, bool const exclusive_lock)
{
#define EXCLUSIVE_LOCK      LOCK_EX
#define NON_BLOCKING_LOCK   LOCK_EX | LOCK_NB
    static char const lock_path[] = "/var/lock/";

    char full_path_file[sizeof lock_path + strlen(lock_file)];

    int const operation = (exclusive_lock) ? EXCLUSIVE_LOCK : NON_BLOCKING_LOCK;
    int fd;

    snprintf(full_path_file, sizeof full_path_file, "%s%s", lock_path, lock_file);

    fd = open(full_path_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        tlr_debug("Could not open PID file %s\n", full_path_file);
        goto error;
    }

    if ( flock(fd, operation) == -1 )
    {
        tlr_debug("Could not open lock PID file %s\n", full_path_file);
        goto error;
    }

    if (ftruncate(fd, 0) == -1)
    {
        tlr_debug("Could not truncate PID file '%s'\n", full_path_file);
        goto error;
    }

    {
        char buf[ULONG_STRING_LENGTH];
        int len = snprintf(buf, sizeof buf, "%ld\n", (long) getpid());

        if (write(fd, buf, len) != len)
        {
            tlr_debug("Error Writing to PID file '%s'\n", full_path_file);
            goto error;
        }
    }

    goto done;

error:
    tlr_file_unlock(fd);
    fd = -1;

done:
    return fd;

}

void tlr_file_unlock(int const fd)
{
    if (fd >= 0)
    {
        if (ftruncate(fd, 0) == -1)
        {
            tlr_debug("Could not truncate PID file\n");
        }

        if (lockf(fd, F_ULOCK, 0) == -1)
        {
            tlr_debug("unable to unlock\n");
        }

        close(fd);
    }
}


