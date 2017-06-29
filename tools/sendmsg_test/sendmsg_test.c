#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <strlcat.h>
#include <stdint.h>
#include <stdbool.h>
#include <messages.h>
#include <parsing.h>
#include <errno.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <timespec_helpers.h>
#include <tlr_common.h>
#include <tlr_print.h>

#define DEFAULT_MILLISECONDS_TO_WAIT 500
#define DEFAULT_MILLISECONDS_BETWEEN_MESSAGES 0

struct outstanding_ping_st
{
    unsigned int message_id;
    struct timespec time_sent;
    bool in_list;
	TAILQ_ENTRY(outstanding_ping_st) entry;
};
typedef struct outstanding_ping_st outstanding_ping_st;

static sem_t outstanding_ping_list_sem;
static pthread_mutex_t wait_lock;
static pthread_cond_t wait_list;
static TAILQ_HEAD(, outstanding_ping_st) outstanding_ping_list;
static struct timespec minimum_response_time;
static struct timespec maximum_response_time;
static bool minimum_response_time_set;
static message_thread_st * message_thread_context;
static volatile sig_atomic_t ctrl_c_caught;
static unsigned long number_received;
static char    msg[512];
static unsigned long number_to_send = 1;
static unsigned long number_sent = 0;
static unsigned long number_outstanding;
static unsigned long send_failures;
static bool timed_out;
static message_destination_t destination;
static unsigned int instance;
static char const * message_test_monitor_name;
static int send_message_result;
static outstanding_ping_st * ping;
static struct timespec start_time;
static struct timespec all_sent_time;
static struct timespec stop_time;
static struct timespec elapsed_time;
static struct timespec time_to_wait;
static unsigned int delay_between_messages = DEFAULT_MILLISECONDS_BETWEEN_MESSAGES;
static unsigned int milliseconds_to_wait_for_responses = DEFAULT_MILLISECONDS_TO_WAIT;
static unsigned int please_respond_time = 0;    // non zero if we want monitor to respond in a little while
static char const * monitor_name;

static void control_c_handler(int sig)
{
    ctrl_c_caught = 1;
}

static void insert_entry_into_active_list(outstanding_ping_st * entry)
{
	sem_wait(&outstanding_ping_list_sem);

    TAILQ_INSERT_TAIL(&outstanding_ping_list, entry, entry);
    entry->in_list = true;

	sem_post(&outstanding_ping_list_sem);

	return;
}

static void free_ping(outstanding_ping_st * ping)
{
	free(ping);
}

static int remove_entry_from_active_list(outstanding_ping_st * const ping)
{
    int result = -1;

    sem_wait(&outstanding_ping_list_sem);

    if (ping->in_list)
    {
        TAILQ_REMOVE(&outstanding_ping_list, ping, entry);
        ping->in_list = false;
        result = 0;
    }

    sem_post(&outstanding_ping_list_sem);

    return result;
}

static int free_all_entries_in_outstanding_list(void)
{
    outstanding_ping_st * ping;
    int list_count = 0;

    sem_wait(&outstanding_ping_list_sem);

    while ((ping = TAILQ_FIRST(&outstanding_ping_list)) != NULL)
    {
        TAILQ_REMOVE(&outstanding_ping_list, ping, entry);
        free_ping(ping);
        list_count++;
    }

    sem_post(&outstanding_ping_list_sem);

    return list_count;
}

static outstanding_ping_st * find_ping_by_id(unsigned int id)
{
    outstanding_ping_st * ping;

    sem_wait(&outstanding_ping_list_sem);

    TAILQ_FOREACH(ping, &outstanding_ping_list, entry)
    {
        if (ping->message_id == id)
        {
            TAILQ_REMOVE(&outstanding_ping_list, ping, entry);
            break;
        }
    }

    sem_post(&outstanding_ping_list_sem);

    return ping;
}

static outstanding_ping_st * create_ping(void)
{
    outstanding_ping_st * ping = calloc(1, sizeof *ping);

    if (ping == NULL)
        goto done;
    ping->in_list = false;

done:
    return ping;
}

static outstanding_ping_st * add_ping_into_list(unsigned int message_id)
{
    outstanding_ping_st * const ping = create_ping();

    ping->message_id = message_id;
    clock_gettime(CLOCK_MONOTONIC, &ping->time_sent);

    insert_entry_into_active_list(ping);

    return ping;
}

static bool get_unsigned_integer_from_string(char const * const str, size_t * const instance)
{
    bool result;
    unsigned long value;

    errno = 0;
    value = strtoul(str, NULL, 10);

    if (errno == EINVAL || errno == ERANGE || value > SIZE_MAX)
    {
        result = false;
        goto done;
    }
    if (errno != 0)
        errno = 0;
    *instance = (size_t)value;
    result = true;

done:
    return result;
}

static void handle_pong_message(void * const info, size_t const info_size)
{
    pong_info_st * pong_info;
    outstanding_ping_st * ping;

    if (info_size != sizeof *pong_info)
    {
        fprintf(stderr, "bad info in pong message");
        goto done;
    }
    pong_info = info;

    if (please_respond_time > 0)
    {
        tlr_debug("got ping response id %u\n", pong_info->message_id);
    }

    ping = find_ping_by_id(pong_info->message_id);
    if (ping != NULL)
    {
        struct timespec now;
        struct timespec elapsed;

        number_received++;  // XXX mutex required?
        clock_gettime(CLOCK_MONOTONIC, &now);
        timerspec_sub(&now, &ping->time_sent, &elapsed);

        if (!minimum_response_time_set || timerspec_compare(&elapsed, &minimum_response_time) < 0)
        {
            minimum_response_time_set = true;
            minimum_response_time.tv_sec = elapsed.tv_sec;
            minimum_response_time.tv_nsec = elapsed.tv_nsec;
        }
        if (timerspec_compare(&elapsed, &maximum_response_time) > 0)
        {
            maximum_response_time.tv_sec = elapsed.tv_sec;
            maximum_response_time.tv_nsec = elapsed.tv_nsec;
        }
        free_ping(ping);
        pthread_cond_signal(&wait_list);
    }
    else
    {
        fprintf(stderr, "ping entry not found\n");
    }

done:
    return;
}

int message_test_callback(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size)
{
    switch (message_code)
    {
        /*
         * We won't normally send ourselves a ping messages, but respond to it if one is ever received.
         */
        case ping_message:
            if (info != NULL && *info != NULL)
            {
                ping_info_st * pinfo = *info;

                (void)send_pong_message(pinfo->pong_destination, pinfo->message_id);
            }
            break;
        case pong_message:
            if (info != NULL && *info != NULL)
            {
                handle_pong_message(*info, info_size);
            }
            break;
        default:
            fprintf(stderr, "got unexpected message code (%d)\n", message_code);
            break;
    }

    return 0;
}

/*
 * create a message handler that will receive all messages to the message test app.
 * We should get a callback whenever a daemon under test sends a PONG message.
 */
int start_response_message_handler(void)
{
	int result;

	message_thread_context = open_messaging_context(message_test_app, message_test_callback);
	if (message_thread_context == NULL)
	{
		result = -1;
		goto done;
	}

    result = 0;

done:
	return result;
}

static unsigned int get_next_unique_id(void)
{
    static unsigned int next_id;

    if (++next_id == 0)
        next_id++;

    return next_id;
}

static void usage(char const * const progname)
{
    printf("Format: %s OPTIONS <monitor> <instance>\n", progname);
    printf("Options:\n");
    printf("-n <number to send>            - Number of PING messages to send (default: 1, 0: continuous)\n");
    printf("-d <inter-message delay>       - Delay between messages sent (milliseconds) (default: %u)\n", DEFAULT_MILLISECONDS_BETWEEN_MESSAGES);
    printf("-s <wait time>                 - Maximum time to wait for responses (milliseconds) (default: %u)\n", DEFAULT_MILLISECONDS_TO_WAIT);
    printf("-t <please respond time>       - Requested time in milleseconds for a delay in response\n");
}

static bool initialise(void)
{
    bool result = false;

    /* initialise all the semaphores, mutexes and lists used by the app. */
    TAILQ_INIT(&outstanding_ping_list);
    pthread_mutex_init(&wait_lock, NULL);
    pthread_cond_init(&wait_list, NULL);
    sem_init(&outstanding_ping_list_sem, 0, 1);
    if (start_response_message_handler() != 0)
    {
        fprintf(stderr, "error starting message handler\n");
        goto error;
    }
    result = true;

error:
    return result;
}

static void send_all_messages(void)
{
    for (number_sent = 0; ctrl_c_caught == 0 && (number_to_send == 0 || number_sent < number_to_send); number_sent++)
    {
        unsigned int message_id = get_next_unique_id();

        snprintf(msg, sizeof msg, "%s %u", message_test_monitor_name, message_id);
        if (please_respond_time != 0)
        {
            int     sl = strlen(msg);
            snprintf(&msg[sl], sizeof msg-sl, " time=%u", please_respond_time );
        }
        ping = add_ping_into_list(message_id);
        if (ping != NULL)
        {
            if (please_respond_time > 0)
            {
                tlr_debug("sending ping to %s-%u\n", monitor_name, instance);
            }
            send_message_result = send_message(destination, instance, ping_message, msg, NULL);
            if (send_message_result != 0)
            {
                remove_entry_from_active_list(ping);
                free_ping(ping);
                send_failures++;
            }
            if (delay_between_messages > 0 && (number_to_send == 0 || number_sent < (number_to_send - 1)))
            {
                usleep(delay_between_messages);
            }
        }
    }
}

static void display_results(void)
{
    if (ctrl_c_caught != 0)
        printf("cancelled by user\n");
    printf("%s %u ping statistics\n", monitor_name, instance);
    printf("%-50s: %lu\n", "sent", number_sent);
    printf("%-50s: %lu\n", "send failures", send_failures);
    printf("%-50s: %lu\n", "received", number_received);
    printf("%-50s: %u.%03us\n", "time to send messages", (int)elapsed_time.tv_sec, (int)(elapsed_time.tv_nsec / 1000000) % 1000);
    if (number_received > 0)
    {
        timerspec_sub(&stop_time, &all_sent_time, &elapsed_time);
        printf("%-50s: %u.%03us\n", "additional time required to receive responses", (int)elapsed_time.tv_sec, (int)(elapsed_time.tv_nsec / 1000000) % 1000);
    }
    timerspec_sub(&stop_time, &start_time, &elapsed_time);
    printf("%-50s: %u.%03us\n", "total time", (int)elapsed_time.tv_sec, (int)(elapsed_time.tv_nsec / 1000000) % 1000);
    if (timed_out)
        printf("timed out waiting for responses\n");
    if (number_received > 0)
    {
        printf("%-50s: %u.%03us\n", "minimum response time", (int)minimum_response_time.tv_sec, (int)(minimum_response_time.tv_nsec / 1000000) % 1000);
        printf("%-50s: %u.%03us\n", "maximum response time", (int)maximum_response_time.tv_sec, (int)(maximum_response_time.tv_nsec/1000000)%1000);
    }
}

/* wait until a pong message is received.
 * Don't wait forever. Time out after the specified amount of time.
 * If a response is received, this will be signalled by the response handler by a
 * call to pthread_cond_signal() and this function will return before the timeout.
 */
static bool wait_for_response(void)
{
    bool timed_out;
    int timed_wait_result;
    struct timespec timeout_time;
    struct timespec now;
    struct timeval now_tv;

    /*
     * This sets up a maximum time to wait between responses.
     * Note that pthread_cond_timedwait operates on absolute time.
     * This means that it's necessary to add the time we'll wait for onto the current time
     */
    gettimeofday(&now_tv, NULL);
    TIMEVAL_TO_TIMESPEC(&now_tv, &now);
    timerspec_add(&now, &time_to_wait, &timeout_time);

    timed_wait_result = pthread_cond_timedwait(&wait_list, &wait_lock, &timeout_time);
    if (timed_wait_result == ETIMEDOUT)
    {
        /* no response received for the specified wait time */
        timed_out = true;
    }
    else
    {
        timed_out = false;
    }

    return timed_out;
}

/*
 * Wait for any outstanding responses to arrive. If no responses are received for the specified
 * amount of time remember that a timeout has occurred and return.
 */
static void wait_for_responses(void)
{
    pthread_mutex_lock(&wait_lock);
    while (!timed_out && (ping = TAILQ_FIRST(&outstanding_ping_list)) != NULL)
    {
        if (wait_for_response())
            timed_out = true;
    }
    pthread_mutex_unlock(&wait_lock);
}

int main(int argc, char ** argv)
{
    int c;
    int index;
    int result;
    char const * const progname = basename(argv[0]);

    openlog(progname, LOG_PID, LOG_USER);
    opterr = 0;

    while ((c = getopt(argc, argv, "d:n:s:t:")) != -1)
    {
        switch (c)
        {
            case 's':
                milliseconds_to_wait_for_responses = strtoul(optarg, NULL, 10);
                if (errno != 0)
                    errno = 0;
                break;
            case 'n':
                number_to_send = strtoul(optarg, NULL, 10);
                if (errno != 0)
                    errno = 0;
                break;
            case 'd':
                delay_between_messages = strtoul(optarg, NULL, 10);
                if (errno != 0)
                    errno = 0;
                break;
            case 't':
                please_respond_time = strtoul(optarg, NULL, 10);
                if (errno != 0)
                    errno = 0;
                break;
            default:
                usage(progname);
                goto error;
        }
    }

    /* check for the minimum number of required args */
    index = optind;

    if ((argc - index) < 2)
    {
        usage(progname);
        goto error;
    }

    /*
     * Get the messaging name for this application. This is supplied along with the ping so that the
     * server end knows where to send the pong response.
     */
    message_test_monitor_name = get_monitor_name_from_message_destination(message_test_app);
    if (message_test_monitor_name == NULL)
    {
        printf("unable to find messaging name for this application!\n");
        goto error;
    }

    /* convert delay_between_messages to microseconds */
    delay_between_messages = milliseconds_to_microseconds(delay_between_messages);

    /* get the ping destination and instance */
    monitor_name = argv[index];
    destination = get_message_destination_from_monitor_name(monitor_name);
    if (destination == invalid_message_destination)
    {
        printf("invalid message destination (%s)\n", monitor_name);
        usage(progname);
        goto error;
    }
    index++;
    if (!get_unsigned_integer_from_string(argv[index], &instance))
    {
        printf("invalid instance (%s)\n", argv[index]);
        usage(progname);
        goto error;
    }

    if (!initialise())
    {
        goto error;
    }

    /* setup timestamps etc */
    minimum_response_time_set = false;
    minimum_response_time.tv_sec = 0;
    minimum_response_time.tv_nsec = 0;
    maximum_response_time.tv_sec = 0;
    maximum_response_time.tv_nsec = 0;
    /* setup time to wait */
    {
        long remainder;

        time_to_wait.tv_sec = milliseconds_to_seconds(milliseconds_to_wait_for_responses+please_respond_time, &remainder);
        time_to_wait.tv_nsec = milliseconds_to_nanoseconds(remainder);
    }
    timed_out = false;
    send_failures = 0;

    clock_gettime(CLOCK_MONOTONIC, &start_time);


    /* replace the default SIGINT handler so we can break out of the main loop */
    ctrl_c_caught = 0;
    signal(SIGINT, control_c_handler);

    send_all_messages();

    /* save timestamp when all messages have been sent */
    clock_gettime(CLOCK_MONOTONIC, &all_sent_time);

    /* restore the default SIGINT handler */
    signal(SIGINT, SIG_DFL);

    /*
     * Now wait for all the responses to come in.
     */
    wait_for_responses();

    clock_gettime(CLOCK_MONOTONIC, &stop_time);

    /*
     * Free anything that hasn't received a response, and count how many of them there were.
     * Note that some more responses may have arrived after we stopped waiting for them but I don't
     * think that matters too much. Typically, all response messages will have been received by now.
     */
    number_outstanding = free_all_entries_in_outstanding_list();

    timerspec_sub(&all_sent_time, &start_time, &elapsed_time);

    /* Finally display the information that has been gathered. */
    display_results();

error:
    if (number_sent > 0 && number_sent == number_received)
    {
        result = EXIT_SUCCESS;
    }
    else
    {
        result = EXIT_FAILURE;
    }

    exit(result);

}



