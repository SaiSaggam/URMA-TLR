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

#include "dsl_ppp.h"
#include "dsl_context.h"
#include "dsl_log.h"
#include "digiutils.h"
#include "queue.h"
#include "tlr_assert.h"

#include <pthread.h>

#define PPPOE_ACTION_QUEUE_SIZE   4

static queue_st * volatile pppoe_action_queue;
static pthread_t ppp_thread_id;

static void execute_pppoe_action_command(char const * const action)
{
    dsl_debug("executing 'pppoe-action %s'\n", action);

    system_command("pppoe-action %s", action);
}

static void * dsl_ppp_thread(void * data)
{
    while (true)
    {
        char const * const action = get_queue_item(pppoe_action_queue);

        tlr_assert(action != NULL);

        execute_pppoe_action_command(action);
    }

    {
        queue_st * pppoe_action_queue_copy = pppoe_action_queue;
        pppoe_action_queue = NULL;
        free_queue(pppoe_action_queue_copy);
    }

	pthread_exit(NULL);
}

void dsl_ppp_start(dsl_context_t * const context)
{
    put_queue_item(pppoe_action_queue, "start");
}

void dsl_ppp_stop(dsl_context_t * const context)
{
    put_queue_item(pppoe_action_queue, "stop");
}

bool dsl_ppp_thread_init(void)
{
    bool dsl_ppp_thread_ok;

    pppoe_action_queue = alloc_queue(PPPOE_ACTION_QUEUE_SIZE);
    if (pppoe_action_queue == NULL)
    {
        dsl_debug("alloc_queue() failed for pppoe_action_queue\n");
        dsl_ppp_thread_ok = false;
        goto done;
    }

    /* create thread to serialise calls to the pppoe-action script */
    if (pthread_create(&ppp_thread_id, NULL, dsl_ppp_thread, NULL) != 0)
    {
        dsl_ppp_thread_ok = false;
        goto done;
    }

    dsl_ppp_thread_ok = true;

done:
    return dsl_ppp_thread_ok;
}

