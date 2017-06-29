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

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <strlcat.h>

#include "messages.h"

static int     debug = 0;



int main(int argc, char ** argv)
{
    int     c;
    int   index, param_nb;
    char  * monitorField = NULL;
    char  * instanceField = NULL;
    char  * messageField = NULL;
    int     instance;
//	int     sock;
    char 	* file_input_name = NULL;
    char    msg[512];
    FILE 	* fpin = NULL;
    char * fline = NULL;
    size_t flen = 0;
    int 	use_stdin = 0;
    connection_handle * conn;
    char 	* response = NULL;
    int 	retries = 0;
#if defined(REPEAT_MESSAGE_DEBUG)
    int number_to_send = 1;
    int number_sent;
#endif

    opterr = 0;

    while ((c = getopt(argc, argv,
#if defined(REPEAT_MESSAGE_DEBUG)
                       "n:"
#endif
                       "dsf:"
                      )) != -1)
    {
        switch (c)
        {
        case 'd':
            debug = 1;
            break;
        case 's':
            use_stdin = 1;
            break;
#if defined(REPEAT_MESSAGE_DEBUG)
        case 'n':
            number_to_send = strtoul(optarg, NULL, 10);
            break;
#endif
        case 'f':
            file_input_name = optarg;
            break;
        }
    }

#if defined(REPEAT_MESSAGE_DEBUG)
    for (number_sent = 0; number_sent < number_to_send; number_sent++)
#endif
    {
        /* check for the minimum number of required args */
        index = optind;
        param_nb = 0;
        msg[0] = '\0';
        while (index < argc)
        {
            if (param_nb > 1)
                strlcat(msg, " ", sizeof msg);

            switch (param_nb)
            {
            case 0:
                monitorField = argv[index];
                break;
            case 1:
                instanceField = argv[index];
                break;
            case 2:
                messageField = argv[index];
                break;
            }
            if (param_nb > 0)
                strlcat(msg, argv[index], sizeof msg);

            index++;
            param_nb++;
        }
        strlcat(msg, "\n", sizeof msg);

        if (monitorField == NULL || instanceField == NULL || (messageField == NULL && file_input_name == NULL && use_stdin == 0))
            goto format_err;
        if (sscanf(instanceField, "%d", &instance) != 1)
            goto format_err;

        if (debug)
            printf("monitor:%s instance:%s message:%s\n", monitorField, instanceField, messageField);

        if (use_stdin)
        {
            fpin = stdin;
        }
        else if (file_input_name != NULL)
        {
            if ((fpin = fopen(file_input_name, "r")) == NULL)
            {
                goto format_err;
            }
        }


        if ((msg[0] == '\0') && fpin != NULL)
        {
            if (getline(&fline, &flen, fpin) != -1)
            {
                snprintf(msg, sizeof msg, "%d %s", instance, fline);
            }
        }

        do
        {
            conn = open_connection(monitorField);
            if (conn == NULL)
                usleep(10000);
            retries++;
        }
        while (conn == NULL && retries < 2);


        if (conn != NULL)
        {
            do
            {
                if (msg[0] != '\0')
                {
                    if (transact_with_connection(conn, msg, &response) < 0)
                        break;
                }
                if (response != NULL)
                {
                    printf("%s\n", response);
                    free(response);
                    response = NULL;
                }
                if (fpin != NULL)
                {
                    // Keep reading from the input stream
                    if (getline(&fline, &flen, fpin) != -1)
                    {
                        snprintf(msg, sizeof msg, "%d %s", instance, fline);
                    }
                    else
                    {
                        // all done with the input stream now.
                        if (fpin != stdin)
                            fclose(fpin);
                        fpin = NULL;
                    }
                }
            }
            while (fpin != NULL);
            close_connection(conn);
        }
    }

    if (fline != NULL)
    {
        free(fline);
        fline = NULL;
        flen = 0;
    }
    if (fpin != NULL && fpin != stdin)
        fclose(fpin);
    return 0;

format_err:
    {
        char * mons =  get_monitor_names_string();
        char * msgs = get_global_msgs_string();

        printf("Format: sendmsg [-f inputfile] [-s] [-d] <monitor> <instance> <message> [extra] \n");
        printf("where:-\n");

        printf("-s     := Use stdin for the message\n");
        printf("-d     := print debug information to console\n");
        if (mons != NULL)
        {
            printf("monitor:= %s\n", mons);
            free(mons);
        }
        if (msgs != NULL)
        {
            printf("message:= %s\n", msgs);
            free(msgs);
        }
    }
    return 1;
}



