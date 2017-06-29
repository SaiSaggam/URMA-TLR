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
#include <sys/file.h>
#include <ctype.h>
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>


#define SIMU_REBOOT         0


static pid_t spawn (char* program, char** arg_list);
static int callprogram (char* program, char** arg_list);
static int createPidFile(const char *pidFile );
static void delete_pid_file (const char *pidFile );

static char     const * const pidfile = "/var/run/reboot_tmr.pid";

void sig_handler(int signo)
{
  delete_pid_file( pidfile );   
  exit (0);  
}


/**
 * take over the usual signals so we can delete our PID file when we are gone.
 */
int get_signals()
{
    if ( (signal(SIGINT, sig_handler) == SIG_ERR) ||
         (signal(SIGTERM, sig_handler) == SIG_ERR) ||
         (signal(SIGQUIT, sig_handler) == SIG_ERR) )
        return -1;
    else
        return 0;
}


/**
 * reboot_tmr main.  the starting point to do the"reboot_tmr" operation
 */
int main (int argc, char **argv)
{
  int     mins;

  // Argument is simply the minutes to wait....before we then reboot  
  if ( argc == 2 )
    {
    if ( sscanf ( argv[1], "%d", &mins ) )
        {
        if ( get_signals() == 0 )
            {
            if ( createPidFile( pidfile ) == 0 )
                {
                if ( mins )
                    sleep ( mins*60 );
                delete_pid_file( pidfile );
#if SIMU_REBOOT==0
                {
                char* arg_list[] = 
                         {
                           "/sbin/reboot",     /* argv[0], the name of the program.  */
                           NULL      /* The argument list must end with a NULL.  */ 
                         }; 
                callprogram ("/sbin/reboot", arg_list);
                }
#else
                printf ( "would reboot\n" );
#endif
                }
            }
        }
    else
        goto format_err;
    }
  else
    goto format_err;
        
    return 0;
format_err:
    printf ( "Syntax:reboot_tmr mins\n" );
    return 1;

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
        close(0);   // close stdin, stdout, stderr in child so our parent keeps them.
        close(1);
        close(2);
        execvp (program,  arg_list); 
        /* The execvp  function returns only if an error occurs.  */ 
        abort (); 
        } 

} 


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


#define     BUF_SIZE        40

/**
 * Create a PID file so other processes can see we are running.
 */
static int createPidFile(const char *pidFile )
{
     int fd;
     char buf[BUF_SIZE];
 
     fd = open(pidFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
     if (fd == -1)
        {
         printf ("Could not open PID file %s\n", pidFile);
         return -1;
        }
     
     if ( flock(fd, LOCK_EX) == -1 )
        {
         printf ("Could not open lock PID file %s\n", pidFile);
         close(fd);
         return -1;
        }

     if (ftruncate(fd, 0) == -1)
        {
         printf ("Could not truncate PID file '%s'\n", pidFile);
         close(fd);
         return -1;
        }
 
     snprintf(buf, BUF_SIZE, "%ld\n", (long) getpid());
     if (write(fd, buf, strlen(buf)) != strlen(buf))
        {
         printf ("Error Writing to PID file '%s'\n", pidFile);
         close(fd);
         return -1;
        }
 
     close(fd);
     return 0;
}

/**
 * We'll need to delete this PID when we leave for whatever reason.
 */
static void delete_pid_file (const char *pidFile )
{
    unlink(pidFile );
}

