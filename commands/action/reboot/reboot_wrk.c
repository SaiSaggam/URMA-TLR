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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>


#define     DEBUG_PARMS         0

int nstrcmpi(char const *str1, char const *str2 ); // annoyingly we don't have strcmpi in C library
static int is_reboot_active(void);
static pid_t get_reboot_pid(void);
static pid_t spawn (char* program, char** arg_list);
//static int callprogram (char* program, char** arg_list);

static int reboot_now ( void );
static int reboot_at ( char * const reboot_time );
static int reboot_in ( char * const reboot_in_minutes);
static void cancel_if_active(void);
static int reboot_cancel ( void );
static void logevent ( char *str );


static char     const * const syntax = "[[in M][at HH:MM][cancel]]";

/**
 * digi_reboot_main .  the starting point to do the"reboot" commands
 */

int digi_reboot_main (int argc, char **argv)
{
  int   index, param_nb;
  int   c;
  char  *param1 = NULL;
  char  *param2 = NULL;

  opterr = 0;

#if DEBUG_PARMS
    printf ( "argc:%d\n", argc );
        {
        int     i;

        for (i=0; i<argc; i++ )
            printf ( "arg:%d:%s\n", i, argv[i] );
        }
#endif


  while ((c = getopt (argc, argv, "f:u:")) != -1)
    {
    }


  for (index = optind, param_nb=0; index < argc; index++, param_nb++)
    {
    switch ( param_nb )
        {
        case 0:  param1=argv[index];    break;
        case 1:  param2=argv[index];    break;
        // Only two parameters for our date command
        }
    }

    if ( param1 != NULL )
        {
        if ( nstrcmpi(param1, "at") == 0 )
            {
            if ( param2 != NULL )
                {
                if ( reboot_at ( param2 )==1 )
                    goto format_err;
                }
            else
                goto format_err;
            }
        else if ( nstrcmpi(param1, "in") == 0 )
            {
            if ( param2 != NULL )
                {
                if ( reboot_in ( param2 )==1 )
                    goto format_err;
                }
            else
                goto format_err;
            }
        else if ( nstrcmpi(param1, "cancel") == 0 )
            {
            if ( reboot_cancel() == 1 )
                goto format_err;
            }
        else
            goto format_err;
        }
    else
        {
        if ( reboot_now() )
            goto format_err;
        }

    return 0;
format_err:
    printf ( "Syntax:reboot %s\n", syntax );
    return 1;

}



/**
 * Reboots quite quickly.. Returns 0 if all ok
 */
static int reboot_now ( void )
{
    char* arg_list[] =
    {
      "reboot_tmr",     /* argv[0], the name of the program.  */
      "0",
      NULL      /* The argument list must end with a NULL.  */
    };

    cancel_if_active();
    logevent ( "rebooting now" );
    spawn ("reboot_tmr", arg_list);
    return 0;
}

/**
 * Reboots at a certain time. Returns 0 if all ok
 */
static int reboot_at ( char * const reboot_time )
{
    int     num_scan, hour, min;

    cancel_if_active();

    num_scan = sscanf(reboot_time, "%2d:%2d", &hour,&min);
    if ( (num_scan == 2) && (hour>=0&&hour<=23) && (min>=0&&min<=59) )
        {
         char    dbuf[20];
         char    logmsg[40];
         struct tm *tm;
	     time_t     now;
         int        delay;

         char* arg_list[] =
         {
           "reboot_tmr",     /* argv[0], the name of the program.  */
           "30",
           NULL      /* The argument list must end with a NULL.  */
         };

         /* Subtract the current time to get the delay.
          * Add a whole day if we go negative */
         now = time (NULL);
         tm = localtime (&now);
         delay = (((hour * 60) + min)
              - ((tm->tm_hour * 60) + tm->tm_min));
         if (delay < 0)
             delay += 1440;

         sprintf ( dbuf, "%d", delay );
         arg_list[1] = dbuf;
         sprintf ( logmsg, "rebooting at %d:%02d", hour, min  );
         logevent ( logmsg );
         spawn ("reboot_tmr", arg_list);
         printf ( "Reboot scheduled at %d:%02d\n", hour, min );
         return 0;
        }
    return 1;

}

/**
 * Reboots in a few minutes. Returns 0 if all ok
 */
static int reboot_in ( char * const reboot_in_minutes)
{
   int     num_scan, min;

    cancel_if_active();
    num_scan = sscanf(reboot_in_minutes, "%d", &min);
    if ( num_scan == 1 )
        {
          char    dbuf[20];
          char    logmsg[40];
          char* arg_list[] =
          {
            "reboot_tmr",     /* argv[0], the name of the program.  */
            "",
            NULL      /* The argument list must end with a NULL.  */
          };
          sprintf ( dbuf, "%d", min );
          arg_list[1] = dbuf;
          sprintf ( logmsg, "rebooting in %d %s", min, min==1 ? "minute" : "minutes"  );
          logevent ( logmsg );
          spawn ("reboot_tmr", arg_list);
          printf ( "Reboot scheduled in %d %s\n", min, min==1 ? "minute" : "minutes" );
          return 0;
        }
    return 1;
}


/**
 * Cancels any pending reboot if one is active.
 */
static void cancel_if_active(void)
{
    if ( is_reboot_active() )
        reboot_cancel ();
}

/**
 * Cancels any pending reboot. Returns 0 if all ok
 */
static int reboot_cancel ( void )
{
    if ( is_reboot_active() == 0)
        {
        printf ( "No reboot active\n" );
        return 3;
        }
    else
        {
        pid_t     pid = get_reboot_pid();

        if ( pid )
            kill ( pid, SIGINT );
        printf ( "Reboot cancelled\n" );
        return 0;
        }
}


/**
 * Compare two strings; case insensitive.
 */
int nstrcmpi(char const *str1, char const *str2 )
{
    int     c1, c2;
    while ( *str1 && *str2 )
        {
        if ( (c1=toupper(*str1)) < (c2=toupper(*str2)) )
            return -1;
        else if (c1 > c2 )
            return 1;
        str1++;
        str2++;
        }
    if ( *str1 == *str2 )
        return 0;
    else if ( *str1 == 0 )
        return -1;
    else
        return 1;

}



/**
 * Return true if a reboot is already currently scheduled.
 */
static int is_reboot_active(void)
{
    struct stat fileStat;
    if(stat("/var/run/reboot_tmr.pid",&fileStat) < 0)
        return 0;
    else
        return 1;
}


#define     BUF_SIZE        40
/**
 * Return process id of reboot_tmr process
 */
static pid_t get_reboot_pid(void)
{
    pid_t   ret_val;
    int fd;
    ssize_t     bytes;
    char buf[BUF_SIZE];

    fd = open("/var/run/reboot_tmr.pid", O_RDONLY, S_IRUSR | S_IWUSR);
    if (fd == -1)
       {
        printf ("Could not open PID file\n");
        return 0;
       }

    if ((bytes=read(fd, buf, BUF_SIZE-1)) > 0)
        {
        unsigned long       pid;
        buf[bytes] = '\0';
        if ( sscanf ( buf, "%ld", &pid ) > 0 )
            {
            ret_val = pid;
            close(fd);
            return ret_val;
            }
        }
    close(fd);
    return 0;
}


/**
 * Starts up a program.. Don't wait for it to complete.
 */
static pid_t spawn (char* program, char** arg_list)
{
    pid_t child_pid;

    /* Duplicate this process. */
    child_pid = fork ();

    if (child_pid != 0)
        /* This is the parent process. */
        {
        return child_pid;
        }
    else
        {
        setsid();
        close(0);   // close stdin, stdout, stderr in child so our parent keeps them.
        close(1);
        close(2);
        execvp (program,  arg_list);
        /* The execvp  function returns only if an error occurs.  */
        abort ();
        }

}


#if 0
 /**
 * Starts up a program.. And we wait for it to complete.
 */
 static int callprogram (char* program, char** arg_list)
 {
     pid_t child_pid;
     int    status;

    if ( (child_pid=spawn(program, arg_list)) != 0 )
        {
        waitpid(child_pid,&status,0);
        return status;
        }
    return -1;
 }
#endif


/**
 * Log an event into the eventlog.
 */
static void logevent ( char *str )
{

    openlog ( "reboot", 0, LOG_LOCAL5 );
    syslog ( LOG_NOTICE, "%s", str );
    closelog();
}
