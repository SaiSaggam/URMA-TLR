/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <pthread.h>
#include "connector_api.h"
#include "platform.h"
#include <unistd.h>
#include <errno.h>


extern unsigned int put_file_active_count;

connector_handle_t connector_handle;

pthread_t connector_thread;
pthread_t application_thread;

#define  STACK_INIT_VALUE   0xEF

__thread void * stack_top;
__thread void * stack_bottom;
__thread size_t stack_size_used = INT32_C(0);
__thread size_t stack_size = INT32_C(0);

connector_status_t connector_run_thread_status = connector_success;

void * PrintThreadStackInit(size_t * StackSize, size_t * GuardSize)
{
    pthread_attr_t Attributes;
    void *StackTop;
    void *StackBottom;

    /* Get the pthread attributes */
    memset (&Attributes, 0, sizeof (Attributes));
    pthread_getattr_np (pthread_self(), &Attributes);
    /* From the attributes, get the stack info */
    pthread_attr_getstack (&Attributes, &StackTop, StackSize);
    pthread_attr_getguardsize(&Attributes, GuardSize);

    /* Done with the attributes */
    pthread_attr_destroy (&Attributes);

    StackBottom = (void *)((size_t)StackTop + *StackSize);

    APP_DEBUG ("-------------------------------------\n");
    APP_DEBUG ("Thread Stack top:        %p\n", StackTop);
    APP_DEBUG ("Thread Stack size:       %" PRIsize " bytes\n", *StackSize);
    APP_DEBUG ("Thread Stack guard size: %" PRIsize " bytes\n", *GuardSize);
    APP_DEBUG ("Thread Min Stack size:   %d bytes\n", PTHREAD_STACK_MIN);
    APP_DEBUG ("Thread Stack bottom:     %p\n", StackBottom);
    APP_DEBUG ("-------------------------------------\n");

    return StackBottom;
}


void check_stack_size(void)
{
    if (pthread_self() == connector_thread)
    {
        /* address of 'nowhere' approximates end of stack */
        char nowhere;
        void* stack_end = (void*)&nowhere;
        /* may want to double check stack grows downward on your platform */
        size_t size = (size_t)stack_bottom - (size_t)stack_end;
        /* update max_stack_size for this thread */

        if (size > stack_size_used)
        {
            stack_size_used = size;
        }
    }
}

void clear_stack_size(void)
{
    if (pthread_self() == connector_thread)
    {
        unsigned char * ptr;
        size_t size;

        char nowhere;

        /* address of 'nowhere' approximates end of stack */
        void* stack_end = (void*)((long)&nowhere & 0xFFFFFFF4);

        /* may want to double check stack grows downward on your platform */
        size = (size_t)stack_bottom - (size_t)stack_end;

        if (size > stack_size_used)
        {
            stack_size_used = size;
        }

        for (ptr = stack_top; ptr < (unsigned char *)((size_t)stack_end-sizeof stack_end); ptr++)
        {
            *ptr = STACK_INIT_VALUE;
        }

    }
}

size_t PrintSummaryStack(void)
{
    uint8_t * ptr = (uint8_t *)stack_top;

    while (*ptr == STACK_INIT_VALUE)
    {
        ptr++;
    }
    APP_DEBUG("======================================\n");
    APP_DEBUG("connector_run Stack End        = %p\n", stack_top);
    APP_DEBUG("connector_run Stack Ptr        = %p\n", ptr);
    APP_DEBUG("connector_run Stack Start      = %p\n", stack_bottom);
    APP_DEBUG("connector_run Stack Size Used  = %d bytes\n", (unsigned)((uint8_t *)stack_bottom-ptr));
    return (size_t)((uint8_t *)stack_bottom-ptr);
}


void * connector_run_thread(void * arg)
{
    void * threadStackStart;
    size_t threadGuardSize;
    unsigned char * ptr;

    char nowhere;
    stack_bottom = (void*)&nowhere;

    threadStackStart = PrintThreadStackInit(&stack_size, &threadGuardSize);

    {
        size_t stackused = (size_t)((long)threadStackStart - (long)stack_bottom);
        stack_size -= stackused;
        stack_size -= threadGuardSize;
    }

    stack_top = (void *)((long)stack_bottom - stack_size);
    //memset(stack_top , STACK_INIT_VALUE, stack_size);
    for (ptr = stack_top; ptr < (unsigned char *)stack_bottom; ptr++)
    {
        *ptr = STACK_INIT_VALUE;
    }

    APP_DEBUG("connector_run_thread starts %d Stack = %p to %p (size = %" PRIsize ")\n", getpid(), stack_top, stack_bottom, stack_size);

    connector_run_thread_status = connector_run(arg);

    APP_DEBUG("connector_run thread exits %d\n", connector_run_thread_status);

    PrintSummaryStack();
    APP_DEBUG("connector_run - callback: stack size = %" PRIsize "\n", stack_size_used);
    APP_DEBUG("\nconnector_run_thread:\n");
    PrintThreadStackInit(&stack_size, &threadGuardSize);

    pthread_exit(arg);
}


void * application_run_thread(void * arg)
{
    connector_status_t status;
    size_t threadGuardSize;
    size_t threadStackSize;

    APP_DEBUG("application_run_thread starts\n");

    status = application_run(arg);

    APP_DEBUG("application_run thread exits %d\n", status);
    APP_DEBUG("\napplication_run_thread:\n");
    PrintThreadStackInit(&threadStackSize, &threadGuardSize);

    pthread_exit(arg);
}

void print_error(int error_no, char * const error_message)
{
    errno = error_no;
    perror("Error:");
    perror(error_message);

    ASSERT(0);
}
int main (void)
{
    int rc = 0;

    APP_DEBUG("Start Etherios Connector\n");
    connector_handle = connector_init(app_connector_callback, NULL);
    if (connector_handle != NULL)
    {
        int ccode;
        ccode = pthread_create(&connector_thread, NULL, connector_run_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on connector_process_thread %d\n", ccode);
            goto done;
        }

        ccode = pthread_create(&application_thread, NULL, application_run_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on connector_process_thread %d\n", ccode);
            goto done;
        }

        pthread_join(connector_thread, NULL);
        pthread_join(application_thread, NULL);

        APP_DEBUG("Etherios Cloud Connector terminated\n");
    }
    else
    {
        APP_DEBUG("Unable to initialize the connector\n");
    }
done:
    return rc;
}
