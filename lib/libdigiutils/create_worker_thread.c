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
****************************************************************************/

#include "create_worker_thread.h"
#include <tlr_common.h>
#include <unistd.h>

int create_worker_thread(pthread_t * const thread_id,
                                bool create_detached,
                                size_t maximum_tries,
                                size_t retry_delay_milliseconds,
                                void * (*thread_fn)(void *),
                                void * const thread_fn_arg)
{
    int pthread_create_result;
    pthread_attr_t attr;
    int s;
    int tries;

    /* set up the attributes for the message handling threads */
    s = pthread_attr_init(&attr);
    if (s != 0)
    {
        pthread_create_result = -1;
        goto done;
    }

    if (create_detached)
    {
        s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if (s != 0)
        {
            pthread_create_result = -1;
            goto done;
        }
    }

    /*
     * It seems that every now and again pthread_create() fails. Generally, it seems that if we wait
     * a little while, the next attempt succeeds.
     */
    tries = 0;
    do
    {
        if (tries > 0 && retry_delay_milliseconds)
        {
            /* place a small delay between attempts to create a thread. It seems to help. */
            usleep(milliseconds_to_microseconds(retry_delay_milliseconds));
        }
        pthread_create_result = pthread_create(thread_id, &attr, thread_fn, thread_fn_arg);
        tries++;
    }
    while (pthread_create_result != 0 && tries < maximum_tries);

done:
    return pthread_create_result;
}


