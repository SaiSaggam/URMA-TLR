#include "cc_status.h"
#include "plain_socket.h"
#include "config_mgr.h"
#include "digiutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <sys/stat.h>

typedef enum
{
    login_type_user,
    login_type_boot,
    login_type_cloud
} login_type_t;

static bool parse_options(int argc, char *argv[], login_type_t * const login_type)
{
    bool success = false;
    int opt;

    opterr = 0;

    while ((opt = getopt(argc, argv, "i:u:a:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            if (strtol(optarg, NULL, 10) != 1)
            {
                syslog(LOG_DEBUG, "invalid index");
                goto done;
            }
            break;

        case 'u':
            if (strcmp(optarg, "boot") == 0)
                *login_type = login_type_boot;
            else if (strcmp(optarg, "cloud") == 0)
                *login_type = login_type_cloud;
            else
                *login_type = login_type_user;
            break;

        case 'a':
            /* @TODO: check errno if strtol and strtoul returns 0, LONG_MIN and LONG_MAX */
            if (strtol(optarg, NULL, 10) < config_mgr_access_level_read_write)
            {
                syslog(LOG_DEBUG, "no access");
                goto done;
            }
            break;

        default:
            break;
        }
    }

    success = true;

done:
    return success;
}

static bool send_message_to_restart_cloud(void)
{
    char const * const socket_name = CC_STATUS_SOCKET_NAME;
    bool const use_abstract_namespace = true;
    int const socket_fd = connect_to_plain_socket(socket_name, use_abstract_namespace);
    bool const success = (socket_fd >= 0);

    if (!success) goto done;

    dprintf(socket_fd, "%s\n", cc_msg_restart);
    close(socket_fd);

done:
    return success;
}

int main (int argc, char *argv[])
{
    bool success = false;
    login_type_t login_type = login_type_user;

    openlog("apply_cloud", 0, LOG_USER);

    if (!parse_options(argc, argv, &login_type)) goto done;

    if (login_type == login_type_cloud)
    {
        syslog(LOG_DEBUG, "apply from Cloud, change will take effect at next cloud application reset");
        success = true;
        goto done;
    }

    success = send_message_to_restart_cloud();

done:
    closelog();
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
