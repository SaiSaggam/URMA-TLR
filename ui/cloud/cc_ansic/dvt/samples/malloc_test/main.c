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
#include "application.h"
#include "connector_config.h"

/* #define THREAD_STACK_SIZE_DEBUG */

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

#ifdef THREAD_STACK_SIZE_DEBUG
    APP_DEBUG ("-------------------------------------\n");
    APP_DEBUG ("Thread Stack top:        %p\n", StackTop);
    APP_DEBUG ("Thread Stack size:       %" PRIsize " bytes\n", *StackSize);
    APP_DEBUG ("Thread Stack guard size: %" PRIsize " bytes\n", *GuardSize);
    APP_DEBUG ("Thread Min Stack size:   %d bytes\n", PTHREAD_STACK_MIN);
    APP_DEBUG ("Thread Stack bottom:     %p\n", StackBottom);
    APP_DEBUG ("-------------------------------------\n");
#endif
    return StackBottom;
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
#ifdef THREAD_STACK_SIZE_DEBUG
    APP_DEBUG("connector_run thread Stack End        = %p\n", stack_top);
    APP_DEBUG("connector_run thread Stack Ptr        = %p\n", ptr);
    APP_DEBUG("connector_run thread Stack Start      = %p\n", stack_bottom);
#endif
    APP_DEBUG("connector_run thread Stack Size Used  = %d bytes\n", (unsigned)((uint8_t *)stack_bottom-ptr));
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

    connector_run_thread_status = connector_run((connector_handle_t)arg);

    APP_DEBUG("connector_run thread exits %d\n", connector_run_thread_status);

    PrintSummaryStack();
    APP_DEBUG("stack size between connector_run and callback = %" PRIsize "\n", stack_size_used);
    APP_DEBUG("\nconnector_run_thread:\n");
    PrintThreadStackInit(&stack_size, &threadGuardSize);

    connector_run_thread_status = connector_device_terminated;

    pthread_exit(arg);
}


void * application_run_thread(void * arg)
{
    connector_status_t status;
    size_t threadGuardSize;
    size_t threadStackSize;

    APP_DEBUG("application_run_thread starts\n");

    status = application_run((connector_handle_t)arg);

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

int start_connector_thread(void)
{
    int ccode = -1;

    connector_handle = connector_init(app_connector_callback, NULL);
    if (connector_handle != NULL)
    {
        ccode = pthread_create(&connector_thread, NULL, connector_run_thread, connector_handle);
        if (ccode != 0)
        {
            APP_DEBUG("thread_create() error on connector_process_thread %d\n", ccode);
        }
    }
    else
    {
        APP_DEBUG("Unable to initialize the connector\n");
    }

    return ccode;
}

int start_application_thread(void)
{
    int ccode = 0;

    ccode = pthread_create(&application_thread, NULL, application_run_thread, connector_handle);
    if (ccode != 0)
    {
        APP_DEBUG("thread_create() error on connector_process_thread %d\n", ccode);
    }

    return ccode;
}

#ifdef CONNECTOR_DATA_SERVICE
#define CNT_DS  1
#else
#define CNT_DS  0
#endif

#ifdef CONNECTOR_FILE_SYSTEM
#define CNT_FS  1
#else
#define CNT_FS  0
#endif

#ifdef CONNECTOR_FIRMWARE_SERVICE
#define CNT_FW  1
#else
#define CNT_FW 0
#endif

#define MODE_CNT (CNT_DS+CNT_FS+CNT_FW)

void write_usage_file(char const * text)
{
    FILE * fp = NULL;
    char const * filename = HEAP_USAGE_FILE;
    static int call_cnt = 0;

    call_cnt++;
    if (call_cnt < MODE_CNT)
        return;

    fp = fopen(filename, "a");

   if (fp == NULL)
    {
        APP_DEBUG("Failed to open %s\n", filename);
        goto done;
    }
    APP_DEBUG("Writing file %s\n", filename);

#ifdef CONNECTOR_DATA_SERVICE
        fprintf(fp, "DATA_SERVICE: Put Requests %d, ", DS_MAX_USER);
#endif

#ifdef CONNECTOR_FILE_SYSTEM
#ifdef APP_ENABLE_MD5
    fprintf(fp, "FILE_SYSTEM: MD5, ");
#else
    fprintf(fp, "FILE_SYSTEM, ");
#endif
#endif

#ifdef CONNECTOR_FIRMWARE_SERVICE
    fprintf(fp, "FIRMWARE_SERVICE, ");
#endif
#ifdef CONNECTOR_RCI_SERVICE
    fprintf(fp, "RCI, ");
#endif
#ifdef CONNECTOR_COMPRESSION
    fprintf(fp, "COMPRESSION, ");
#endif
#ifdef CONNECTOR_DEBUG
    fprintf(fp, "DEBUG, ");
#endif
    fprintf(fp, "\n%s: heap used %" PRIsize "\n\n", text, total_max_malloc_size);

    fclose(fp);

done:
    return;
}

int main (void)
{
    int rc = 0;

    for (;;)
    {
        APP_DEBUG("Start Etherios Connector\n");

        rc = start_connector_thread();
        if (rc != 0)
        {
           goto done;
        }

        rc = start_application_thread();
        if (rc != 0)
        {
           goto done;
        }

        pthread_join(connector_thread, NULL);
        pthread_join(application_thread, NULL);

        if (total_malloc_size != 0)
        {
            /* terminate Cloud Connector so it will not reconnect to Device Cloud */
            APP_DEBUG("total malloc memory = %" PRIsize " after all threads are canceled\n", total_malloc_size);
            if (connector_run_thread_status == connector_device_terminated)
            {
                APP_DEBUG("Error: connector_run has been terminated by connector_initiate_terminate but total malloc memory is not 0 after all threads are canceled\n");
            }
            if (put_file_active_count > 0)
            {
                APP_DEBUG("Error: %d active put requests\n", put_file_active_count);
            }
            rc = -1;
        }
        APP_DEBUG("Allocated memory size = %" PRIsize "\n", total_max_malloc_size);
        write_usage_file("main");
            /* continue and reconnect Device Cloud so python test will not fail */
    }

done:
    return rc;
}
