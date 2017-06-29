/* From http://www-theorie.physik.unizh.ch/~dpotter/howto/daemonize,
   released into the public domain.
   To properly daemonize, the following steps must be followed.
    * The fork() call is used to create a separate process.
    * The setsid() call is used to detach the process from the parent (normally a shell).
    * The file mask should be reset.
    * The current directory should be changed to something benign.
    * The standard files (stdin,stdout and stderr) need to be reopened.

Failure to do any of these steps will lead to a daemon process that can
misbehave. The typical symptoms are as follows.

    * Starting the daemon and then logging out will cause the terminal to hang. This is particularly nasty with ssh.
    * The directory from which the daemon was launched remains locked.
    * Spurious output appears in the shell from which the daemon was started.
*/
#include "tlr_print.h"
#include "daemonize.h"
#include "tlr_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
/* Change this to the user under which to run */
//#define RUN_AS_USER "root"

static volatile sig_atomic_t sig_from_child = 0;
static int running_confirmation_pipe[2] = {-1, -1};

#define MAXIMUM_CONFIRMATION_WAIT_TIME_SECONDS  10

static void sigalarm_handler(int signum)
{
    /* Ignore this signal. It will cause the call to pause() to return and the code will then check
     * the value of sig_from_child.
     */
}

static void sigusr1_handler(int signum)
{
    sig_from_child = 1;
}

static void sigchld_handler(int signum)
{
    exit(EXIT_FAILURE);
}

static bool wait_for_pipe_to_be_readable(int const pipe_fd, unsigned int max_seconds_to_wait)
{
    fd_set read_fd_set;
    bool pipe_readable;
    struct timeval timeout;
    int select_result;

    timeout.tv_sec = max_seconds_to_wait;
    timeout.tv_usec = 0;

    FD_ZERO(&read_fd_set);
    FD_SET(pipe_fd, &read_fd_set);

    do
    {
        select_result = select(pipe_fd + 1, &read_fd_set, NULL, NULL, &timeout);
    }
    while (select_result == -1 && errno == EINTR);

    if (select_result == -1)
    {
        tlr_critical("Error (%s) waiting for daemon to start\n", strerror(errno));
    }

    /* Treat an error like a timeout. i.e. pipe isn't readable. */
    pipe_readable = select_result > 0;

    return pipe_readable;
}

static bool wait_for_daemon_started_confirmation(void)
{
    bool const got_confirmation = wait_for_pipe_to_be_readable(running_confirmation_pipe[0], MAXIMUM_CONFIRMATION_WAIT_TIME_SECONDS);

    if (!got_confirmation)
    {
        tlr_debug("Timed out waiting for daemon to start\n");
    }

    return got_confirmation;
}

void daemonize(char const * const daemon_name, bool expect_confirmation_daemon_is_running)
{
    pid_t pid, sid, parent;
    FILE *pfp;
	char pid_filename[PATH_MAX];

    /* already a daemon */
    if (getppid() == 1)
    {
        return;
    }

    tlr_debug("Start daemon %s\n", daemon_name);

    if (expect_confirmation_daemon_is_running)
    {
        if (pipe(running_confirmation_pipe) == -1)
        {
            tlr_error("pipe error");
            exit(EXIT_FAILURE);
        }
    }

    /* Open .pid file while root. We can write it once we know child pid. */
    umask(0022);
	snprintf(pid_filename, sizeof pid_filename, "/var/run/%s.pid", daemon_name);
    pfp = fopen(pid_filename, "w+");
    if (pfp == NULL)
    {
		tlr_error("Unable to open pid file, %d\n", errno);
		exit(EXIT_FAILURE);
    }


#ifdef RUN_AS_USER
    /* Drop user if there is one, and we were run as root */
    if ( getuid() == 0 || geteuid() == 0 )
    {
        struct passwd *pw = getpwnam(RUN_AS_USER);
        if ( pw )
        {
            tlr_notice("setting user to " RUN_AS_USER );
    	    if (setregid(pw->pw_gid, pw->pw_gid) < 0)
    	    {
    		    tlr_error("Unable to set gids %d (%s)", errno, strerror(errno));
    		    exit(EXIT_FAILURE);
    	    }
            if (setuid( pw->pw_uid ) < 0)
            {
                tlr_error("unable to switch to user " RUN_AS_USER ", code %d (%s)",
                       errno, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    }
#endif

    /* Trap signals that we expect to receive */
    signal(SIGCHLD, sigchld_handler);
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGALRM, sigalarm_handler);

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
	{
        tlr_error("unable to fork daemon, code=%d (%s)",
                errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) /* If we got a good PID, then we can exit the parent process. */
	{
        bool fork_succeeded;
        bool daemon_started;
        int exit_code;

		/* parent process */
        if (expect_confirmation_daemon_is_running)
        {
            /* the parent won't be writing to the confirmation pipe, so close its copy of it */
            close(running_confirmation_pipe[1]);
        }

        /* write the pid of the newly created child process into the file */
        if (fprintf(pfp, "%d\n", pid) <= 0 || fclose(pfp) != 0)
		{
			tlr_error("Unable to write pid file, %d\n", errno);
            /* Do we want to do anything about the child that has just started at this point. */
			exit(EXIT_FAILURE);
		}

        fork_succeeded = false;
        /* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
         * for two seconds to elapse (SIGALRM). Pause should return after two seconds at most.
         */
        alarm(2);
        if (sig_from_child)
        {
            fork_succeeded = true;
        }
        pause();

        if (sig_from_child)
        {
            fork_succeeded = true;
        }
        if (fork_succeeded)
        {
            if (expect_confirmation_daemon_is_running)
            {
                daemon_started = wait_for_daemon_started_confirmation();
            }
            else
            {
                daemon_started = true;
            }
        }
        else
        {
            daemon_started = false;
        }

        exit_code = daemon_started ? EXIT_SUCCESS : EXIT_FAILURE;
        exit(exit_code);
    }

    /* child process */
    if (expect_confirmation_daemon_is_running)
    {
        /* the child won't be reading from the confirmation pipe, so close its copy of it */
        close(running_confirmation_pipe[0]);
    }

    /* OK for the child to just close this file */
    if (pfp != NULL)
    {
        (void)fclose(pfp);	/* ignore any error */
    }

    /* At this point we are executing as the child process */
    parent = getppid();

    /* Cancel certain signals */
    signal(SIGCHLD,SIG_DFL); /* A child process dies */
    signal(SIGTSTP,SIG_IGN); /* Various TTY signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */
    signal(SIGTERM,SIG_DFL); /* Die on SIGTERM */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        tlr_error("unable to create a new session, code %d (%s)",
                  errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0)
    {
        tlr_error("unable to change directory to %s, code %d (%s)",
                "/", errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null. Log an error, but otherwise ignore it. */
    if (freopen( "/dev/null", "r", stdin) == NULL)
    {
        tlr_error("Failed to redirect stdin to /dev/null");
    }
    if (freopen("/dev/null", "w", stdout) == NULL)
    {
        tlr_error("Failed to redirect stdout to /dev/null");
    }
    if (freopen("/dev/null", "w", stderr) == NULL)
    {
        tlr_error("Failed to redirect stderr to /dev/null");
    }

    /* Tell the parent process that we are A-okay */
    kill( parent, SIGUSR1 );
}

static void usage(char const * const cmdname)
{
    printf("usage: %s [-d]\n", cmdname);
}

int start_daemon(int argc, char * argv[], int (*main_fn)(void), bool expect_confirmation_daemon_is_running)
{
    int result = EXIT_FAILURE;
    char * cmdname = basename(argv[0]);
    int opt;
    bool daemonise = false;
    int lock_fd = -1;

    while ((opt = getopt(argc, argv, "d")) != -1)
    {
        switch (opt)
        {
            case 'd':
                daemonise = true;
                break;

            default:
                usage(cmdname);
                goto done;
        }
    }

    // Initialize the logging interface.
    openlog(cmdname, LOG_PID, LOG_USER);

    // Daemonise if requested.
    if (daemonise)
    {
        lock_fd = tlr_file_lock(cmdname, false);

        if (lock_fd < 0)
        {
            tlr_debug("Unable to start %s. It may be currently running.\n", cmdname);
            goto error;
        }
        daemonize(cmdname, expect_confirmation_daemon_is_running);
    }

    // Do the real work.
    main_fn();

error:
    // Clean up.
    tlr_notice("terminated");
    tlr_file_unlock(lock_fd);
    closelog();
    result = EXIT_SUCCESS;

done:
    return result;
}

void daemon_confirm_is_running(void)
{
    if (running_confirmation_pipe[1] >= 0)
    {
        char const confirmation_value = 'C';

        if (write(running_confirmation_pipe[1], &confirmation_value, sizeof confirmation_value) == -1)
        {
           tlr_debug("Error signalling that daemon has started\n");
        }
        close(running_confirmation_pipe[1]);
        running_confirmation_pipe[1] = -1;
    }
}
