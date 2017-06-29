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
****************************************************************************/
#include "connector_api.h"
#include "platform.h"
#include "cc_status.h"
#include "strlcpy.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "network_dns.h"

/* Global structure of connected interface */
static struct sockaddr_in interface_addr;

typedef struct
{
    int sfd;
    SSL_CTX * ctx;
    SSL * ssl;
} app_ssl_t;

connector_callback_status_t app_get_interface_ip_address(uint8_t ** const ip_address, size_t * const size)
{
    *size = sizeof(interface_addr.sin_addr.s_addr);
    *ip_address = (uint8_t *)&interface_addr.sin_addr.s_addr;

    return connector_callback_continue;
}

static int app_setup_socket(void)
{
    int const protocol = 0;
    int sd = socket(AF_INET, SOCK_STREAM, protocol);

    if (sd < 0) goto done;

    {
        int enabled = 1;

        if (setsockopt(sd, SOL_SOCKET, SO_KEEPALIVE, &enabled, sizeof enabled) < 0)
        {
            APP_DEBUG("open_socket: setsockopt SO_KEEPALIVE failed, errno %d\n", errno);
            goto error;
        }

        enabled = 1;
        if (setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof enabled) < 0)
        {
            APP_DEBUG("open_socket: setsockopt TCP_NODELAY failed, errno %d\n", errno);
            goto error;
        }
    }

    goto done;

error:
    close(sd);
    sd = -1;

done:
    return sd;
}

static int app_connect_to_device_cloud(int const fd, in_addr_t const ip_addr)
{
    int ret;
    struct sockaddr_in sin = {0};

    memcpy(&sin.sin_addr, &ip_addr, sizeof sin.sin_addr);
    sin.sin_port   = htons(CONNECTOR_SSL_PORT);
    sin.sin_family = AF_INET;
    ret = connect(fd, (struct sockaddr *)&sin, sizeof sin);

    if (ret < 0)
    {
        int err = errno;

        switch (err)
        {
            case EAGAIN:
            case EINTR:
            case EINPROGRESS:
                ret = 0;
                break;

            default:
                APP_DEBUG("network_connect: connect() failed, errno %d\n", err);
                break;
        }
    }

    return ret;
}

static int app_is_connect_complete(int const fd)
{
    int ret = -1;
    struct timeval timeout = {.tv_sec = 5};
    fd_set read_set;
    fd_set write_set;

    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);
    write_set = read_set;

    /* wait for 2 seconds to connect */
    if (select(fd+1, &read_set, &write_set, NULL, &timeout) <= 0) goto error;

    /* Check whether the socket is now writable (connection succeeded). */
    if (FD_ISSET(fd, &write_set))
    {
        /* We expect "socket writable" when the connection succeeds. */
        /* If we also got a "socket readable" we have an error. */
        if (FD_ISSET(fd, &read_set)) goto error;

        ret = 0;
    }

error:
    return ret;
}

#if (defined APP_SSL_CLNT_CERT)
static int get_user_passwd(char * const buf, int const buf_size, int const rwflag, void * const password)
{
  char const passwd[] = APP_SSL_CLNT_CERT_PASSWORD;
  int const pwd_bytes = ARRAY_SIZE(passwd) - 1;
  int const copy_bytes = (pwd_bytes < buf_size) ? pwd_bytes : buf_size - 1;

  UNUSED_ARGUMENT(rwflag);
  UNUSED_ARGUMENT(password);

  ASSERT_GOTO(copy_bytes >= 0, error);
  memcpy(buf, passwd, copy_bytes);
  buf[copy_bytes] = '\0';

error:
  return copy_bytes;
}
#endif

static bool app_load_certificate_and_key(SSL_CTX * const ctx)
{
    bool success = false;
    int ret = SSL_CTX_load_verify_locations(ctx, APP_SSL_CA_CERT_PATH, NULL);;

    if (ret != 1)
    {
        APP_DEBUG("Failed to load CA cert %d\n", ret);
        ERR_print_errors_fp(stderr);
        goto error;
    }

#if (defined APP_SSL_CLNT_CERT)
    SSL_CTX_set_default_passwd_cb(ctx, get_user_passwd);
    ret = SSL_CTX_use_certificate_file(ctx, APP_SSL_CLNT_KEY, SSL_FILETYPE_PEM);
    if (ret != 1)
    {
        APP_DEBUG("SSL_use_certificate_file() Error [%d]\n", ret);
        goto error;
    }

    ret = SSL_CTX_use_RSAPrivateKey_file(ctx, APP_SSL_CLNT_CERT, SSL_FILETYPE_PEM);
    if (ret != 1)
    {
        APP_DEBUG("SSL_use_RSAPrivateKey_file() Error [%d]\n", ret);
        goto error;
    }
#endif

    success = true;

error:
    return success;
}

static void app_free_ssl_info(app_ssl_t * const ssl_ptr)
{
    if (ssl_ptr->ssl != NULL)
    {
        SSL_free(ssl_ptr->ssl);
        ssl_ptr->ssl = NULL;
    }

    if (ssl_ptr->ctx != NULL)
    {
        SSL_CTX_free(ssl_ptr->ctx);
        ssl_ptr->ctx = NULL;
    }

    if (ssl_ptr->sfd != -1)
    {
        close(ssl_ptr->sfd);
        ssl_ptr->sfd = -1;
    }
}

static int app_verify_device_cloud_certificate(SSL * const ssl)
{
    int ret = -1;
    X509 * const device_cloud_cert = SSL_get_peer_certificate(ssl);

    if (device_cloud_cert == NULL)
    {
        APP_DEBUG("app_verify_device_cloud_certificate: No Device Cloud certificate is provided\n");
        goto done;
    }

    ret = SSL_get_verify_result(ssl);
    if (ret !=  X509_V_OK)
    {
        APP_DEBUG("Device Cloud certificate is invalid %d\n", ret);
        goto done;
    }

done:
    return ret;
}

static bool app_ssl_connect(app_ssl_t * const ssl_ptr)
{
    bool success = false;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ssl_ptr->ctx = SSL_CTX_new(TLSv1_client_method());
    if (ssl_ptr->ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        goto error;
    }

    ssl_ptr->ssl = SSL_new(ssl_ptr->ctx);
    if (ssl_ptr->ssl == NULL)
    {
        ERR_print_errors_fp(stderr);
        goto error;
    }

    SSL_set_fd(ssl_ptr->ssl, ssl_ptr->sfd);
    if (!app_load_certificate_and_key(ssl_ptr->ctx))
    {
        goto error;
    }

    SSL_set_options(ssl_ptr->ssl, SSL_OP_ALL);
    if (SSL_connect(ssl_ptr->ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        goto error;
    }

    if (app_verify_device_cloud_certificate(ssl_ptr->ssl) != X509_V_OK)
    {
        goto error;
    }

    success = true;

error:
    return success;
}

static int make_non_blocking(int fd)
{
    int enabled = 1;
    int result = ioctl(fd, FIONBIO, &enabled);

    if (result < 0)
    {
        APP_DEBUG("ioctl: FIONBIO failed, errno %d\n", errno);
    }

    return result;
}

static connector_callback_status_t app_tcp_connect(in_addr_t const ip_addr,
                                                   connector_network_open_t * const data)
{
    connector_callback_status_t status = connector_callback_error;
    static app_ssl_t ssl_info = {0};
    socklen_t interface_addr_len = sizeof(interface_addr);

    ssl_info.sfd = app_setup_socket();
    if (ssl_info.sfd < 0)
    {
        APP_DEBUG("Could not open socket\n");
        goto done;
    }

    if (app_connect_to_device_cloud(ssl_info.sfd, ip_addr) < 0) goto error;
    if (app_is_connect_complete(ssl_info.sfd) < 0) goto error;
    if (!app_ssl_connect(&ssl_info)) goto error;
    if (make_non_blocking(ssl_info.sfd) < 0) goto error;

    /* Get socket info of connected interface */
    if (getsockname(ssl_info.sfd, (struct sockaddr *)&interface_addr, &interface_addr_len))
    {
        APP_DEBUG("network_connect: getsockname error, errno %d\n", errno);
        goto done;
    }

    APP_DEBUG("network_connect: connected\n");
    data->handle = &ssl_info;
    status = connector_callback_continue;
    goto done;

error:
    app_free_ssl_info(&ssl_info);
    app_dns_set_redirected(connector_class_id_network_tcp, 0);

done:
    return status;
}

/*
 * Send data to Device Cloud, this routine must not block.
 */
static connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
{
    connector_callback_status_t status;
    app_ssl_t * const ssl_ptr = data->handle;
    int const bytes_sent = SSL_write(ssl_ptr->ssl, data->buffer, data->bytes_available);

    if (bytes_sent <= 0)
    {
        APP_DEBUG("SSL_write failed %d\n", bytes_sent);
        SSL_set_shutdown(ssl_ptr->ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
        app_dns_cache_invalidate(connector_class_id_network_tcp);
        status = connector_callback_error;
    }
    else
    {
        cloud_status_t * const status_ptr = cc_get_cloud_status();

        status = connector_callback_continue;
        status_ptr->bytes.sent += bytes_sent;
        status_ptr->packets.sent++;
    }

    data->bytes_used = bytes_sent;
    return status;
}

static connector_callback_status_t peek_socket_for_data(app_ssl_t * const ssl_ptr)
{
    fd_set read_set;
    struct timeval timeout = {.tv_sec = 0, .tv_usec = 0};
    connector_callback_status_t status;
    int ready = SSL_pending(ssl_ptr->ssl);

    if (ready > 0)
    {
        status = connector_callback_continue;
        goto done;
    }

    FD_ZERO(&read_set);
    FD_SET(ssl_ptr->sfd, &read_set);

    ready = select(ssl_ptr->sfd + 1, &read_set, NULL, NULL, &timeout);
    if (ready == 0)
    {
        status = connector_callback_busy;
        goto done;
    }

    if (ready > 0)
    {
        status = connector_callback_continue;
        goto done;
    }

    APP_DEBUG("app_network_receive: select failed\n");
    status = connector_callback_error;

done:
    return status;

}

/*
 * This routine reads a specified number of bytes from Device Cloud.
 */
static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
{
    int bytes_read;
    app_ssl_t * const ssl_ptr = data->handle;
    connector_callback_status_t status = peek_socket_for_data(ssl_ptr);

    if (status != connector_callback_continue) goto done;

    bytes_read = SSL_read(ssl_ptr->ssl, data->buffer, data->bytes_available);
    if (bytes_read <= 0)
    {
        int const ssl_error = SSL_get_error(ssl_ptr->ssl, bytes_read);

        if (ssl_error == SSL_ERROR_WANT_READ)
        {
            status = connector_callback_busy;
            goto done;
        }

        /* EOF on input: the connection was closed. */
        SSL_set_shutdown(ssl_ptr->ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
        APP_DEBUG("SSL_read failed %d\n", bytes_read);
        app_dns_cache_invalidate(connector_class_id_network_tcp);
        status = connector_callback_error;
    }
    else
    {
        cloud_status_t * const status_ptr = cc_get_cloud_status();

        status_ptr->bytes.received  += bytes_read;
        status_ptr->packets.received++;
    }

    data->bytes_used = (size_t)bytes_read;

done:
    return status;
}

static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
{
    app_ssl_t * const ssl_ptr = data->handle;

    /* send close notify to peer */
    if (SSL_shutdown(ssl_ptr->ssl) == 0)
    {
        SSL_shutdown(ssl_ptr->ssl);  /* wait for peer's close notify */
    }

    app_free_ssl_info(ssl_ptr);
    app_dns_set_redirected(connector_class_id_network_tcp, data->status == connector_close_status_cloud_redirected);
    data->reconnect = app_connector_reconnect(connector_class_id_network_tcp, data->status);

    return connector_callback_continue;
}

static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
{
    in_addr_t ip_addr;
    connector_callback_status_t status;

    status = app_dns_resolve(connector_class_id_network_tcp, data->device_cloud.url, &ip_addr);
    if (status != connector_callback_continue)
    {
        APP_DEBUG("app_network_tcp_open: Can't resolve DNS for %s\n", data->device_cloud.url);
        goto done;
    }

    status = app_tcp_connect(ip_addr, data);
    switch (status)
    {
        case connector_callback_continue:
        {
            cloud_status_t * const status_ptr = cc_get_cloud_status();

            strlcpy(status_ptr->server_name, data->device_cloud.url, sizeof status_ptr->server_name);
            APP_DEBUG("network_tcp_open: connected to %s\n", data->device_cloud.url);
            break;
        }

        case connector_callback_error:
            APP_DEBUG("network_tcp_open: failed to connect to %s\n", data->device_cloud.url);
            break;

        default:
            break;
    }

done:
    return status;

}

/*
 *  Callback routine to handle all networking related calls.
 */
connector_callback_status_t app_network_tcp_handler(connector_request_id_network_t const request_id,
                                                    void * const data)
{
    connector_callback_status_t status = connector_callback_error;

    switch (request_id)
    {
    case connector_request_id_network_open:
        status = app_network_tcp_open(data);
        break;

    case connector_request_id_network_send:
        status = app_network_tcp_send(data);
        break;

    case connector_request_id_network_receive:
        status = app_network_tcp_receive(data);
        break;

    case connector_request_id_network_close:
        status = app_network_tcp_close(data);
        break;
    }

    return status;
}

