/*****************************************************************************
 * Copyright (c) 2013 Digi International Inc., All Rights Reserved
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

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <inttypes.h>

#include "cli.h"
#include "cli_cmd.h"
#include "cli_timeout.h"

#define     TICK_COUNT_METHOD       1       // as method 0 fails the changing of time test.

static uint32_t cli_get_timeout_value(cli_t * const cli)
{
    uint32_t timeout;
    char * data;

    if (cli_get_config(cli, "system", 1, "timeout", &data) != cli_success)
    {
        timeout = 180; // seconds
    }
    else
    {
        sscanf(data, "%" SCNu32, &timeout);
        free(data);
    }

    return timeout;
}

/**
 * Gets the currently configured timeout and save it into the cli context
 *
 */
void cli_get_timeout(cli_t * const cli)
{
    cli->cli_timeout_secs = cli_get_timeout_value(cli);
}


long getcpuseconds ()
{
    long        ticks=0;

#if TICK_COUNT_METHOD==0
    ticks = getmsectickcount0();
#elif TICK_COUNT_METHOD==1
    ticks = getmsectickcount1();
#endif
    return (ticks+500)/1000;
}

long getmsectickcount()
{
#if TICK_COUNT_METHOD==0
    return getmsectickcount0();
#elif TICK_COUNT_METHOD==1
    return getmsectickcount1();
#else
    return 0;
#endif
}


// Got 3 methods to try out for getting cpu tick(msec) counts..
// We may need a different method between host and target.. So
// I'll keep them all here for now.
long getmsectickcount0()
{
    struct timeval tv;
    if( gettimeofday(&tv, NULL) != 0 )
        return 0;

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


long getmsectickcount1()
{
    struct timespec     ts;

    if ( clock_gettime ( CLOCK_MONOTONIC, &ts ) != 0 )
        return 0;

    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}


#if 0
This one is rubbish.. .keep it for now in case it is needed for target or as a reference
long getmsectickcount2()
{
    clock_t     cl;
    long        lcl;

    cl = clock();
    lcl = cl;

    printf ( "lcl is %ld\n", lcl );

    return ( lcl*1000 / (CLOCKS_PER_SEC) );

}

#endif
