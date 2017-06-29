
/*! @page network_ssl_callbacks Network Callbacks for SSL connection
 *
 * @htmlinclude nav.html
 *
 * @section net_ssl Network Callbacks
 * From the top level these callbacks are analogous to the @ref network_callbacks. Internally they provide
 * interface to securely connect to Device Cloud on SSL. Any application can make use of these APIs to
 * obtain secure connection just by including network_tcp_ssl.c instead of network_tcp.c. The sample application
 * @ref connect_on_ssl is created to demonstrate how easy to enable SSL connections. Like @ref network_callbacks,
 * the SSL network interface module will provide the following callback APIs 
 * 
 *  -# @ref ssl_connect
 *  -# @ref ssl_send
 *  -# @ref ssl_receive
 *  -# @ref ssl_close
 * 
 * The SSL network callback APIs use openssl functions to communicate with Device Cloud over SSL connection.
 * These APIs can be found in network_tcp_ssl.c under platforms/<i>my_platform</i> to demonstrate how one can
 * write the network layer callback APIs to connect, send, receive and close the SSL connection.
 * As far as the top level calls are concerned there is no change between the SSL and non-SSL connection.
 *
 * The CA certificate is stored in public/certificates/ directory. A define APP_SSL_CA_CERT_PATH can be found
 * in platform.h. By default it is set to "../../../certificates/Digi_Int-ca-cert-public.crt".
 * 
 * @note the certificate name is correct, Etherios Inc. is a division of Digi International Inc..
 * @note the certificate is valid for 100 years (until 2109)
 * 
 * @section ssl_connect Open
 *
 * This callback is called to establish SSL connection between Cloud Connector and Device Cloud.
 * Callback is responsible to setup any socket options and SSL specific initial settings.
 * 
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_network_tcp_open() in network_tcp_ssl.c.
 *
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 *   <th colspan="2" class="title">Arguments</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Name</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <th>class_id</th>
 *   <td>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly</td>
 * </tr>
 * <tr>
 *   <th>request_id</th>
 *   <td>@endhtmlonly @ref connector_request_id_network_open @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_network_open_t "connector_network_open_t" @htmlonly structure
 *        <ul>
 *          <li><b><i>device_cloud_url</i></b> - [IN] Pointer to cloud URL that callback will make connection to on @endhtmlonly @ref CONNECTOR_SSL_PORT @htmlonly for secure communication </li>
 *          <li><b><i>handle</i></b> - [OUT] Returned @endhtmlonly @ref connector_network_handle_t "network handle" @htmlonly which is used throughout network callbacks </li>
 *        </ul>
 * </td>
 * </tr>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 *   <td>Callback successfully established a connection</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 *   <td>Callback is busy connecting and needs to be called back again</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 * <td>Callback was unable to connect to the Device Cloud; Cloud Connector will exit @endhtmlonly @ref connector_run "connector_run()"@htmlonly.
 *     <br /><br />
 *     If the transport is configured to start automatically 
 *     the callback will be retried when @endhtmlonly @ref connector_run "connector_run()" @htmlonly is called again.
 * </td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 *   <td>Callback was unable to connect to the Device Cloud and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 * 
 * #include "connector_api.h"
 * #include "platform.h"
 *
 * typedef struct
 * {
 *    int sfd;
 *    SSL_CTX * ctx;
 *    SSL * ssl;
 * } app_ssl_t;
 *
 * static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
 * {
 *
 *     // resolve address for data->device_cloud_url
 *     // create socket fd, set socket option for keep alive and no delay
 *     // connect to CONNECTOR_SSL_PORT on Device Cloud
 *
 *    SSL_library_init();
 *    OpenSSL_add_all_algorithms();
 *    SSL_load_error_strings();
 *    ssl_ptr->sfd = fd;
 *    ssl_ptr->ctx = SSL_CTX_new(TLSv1_client_method());    
 *    if (ssl_ptr->ctx == NULL)
 *    {
 *        ERR_print_errors_fp(stderr);
 *        goto error;
 *    } 
 *
 *    ssl_ptr->ssl = SSL_new(ssl_ptr->ctx);
 *    if (ssl_ptr->ssl == NULL)
 *    {
 *        ERR_print_errors_fp(stderr);
 *        goto error;
 *    }
 *
 *    SSL_set_fd(ssl_ptr->ssl, ssl_ptr->sfd);
 *
 *    ret = SSL_CTX_load_verify_locations(ctx, APP_SSL_CA_CERT_PATH, NULL);
 *    if (ret != 1) 
 *    {
 *        APP_DEBUG("Failed to load CA cert [%d]\n", ret);
 *        ERR_print_errors_fp(stderr);
 *        goto error;
 *    }
 *    
 *    SSL_set_options(ssl_ptr->ssl, SSL_OP_ALL);
 *    if (SSL_connect(ssl_ptr->ssl) <= 0)
 *    {
 *        ERR_print_errors_fp(stderr);
 *        goto error;
 *    }
 *
 *    {
 *        X509 * const cloud_cert = SSL_get_peer_certificate(ssl);
 *
 *        if (cloud_cert == NULL)
 *        {
 *            APP_DEBUG("No certificate is provided by Device Cloud\n");
 *            goto done;
 *        }
 *
 *        ret = SSL_get_verify_result(ssl);
 *        if (ret !=  X509_V_OK)
 *        {
 *            APP_DEBUG("Device Cloud certificate is invalid [%d]\n", ret);
 *           goto done;
 *        }
 *    }
 *
 * }
 *
 * @endcode
 *
 * @section ssl_send Send
 *
 * Callback is called to send data to Device Cloud over SSL connection. This function must not block.
 * If it encounters EAGAIN or EWOULDBLOCK error it must return @ref connector_callback_busy and Cloud Connector will
 * continue calling this function.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_network_tcp_send() in network_tcp.c.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr> 
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_network_send @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th> 
 * <td>Pointer to @endhtmlonly @ref connector_network_send_t "connector_network_send_t" @htmlonly structure
 *        <ul>
 *          <li><b><i>handle</i></b> - [In] @endhtmlonly @ref connector_network_handle_t "Network handle" @htmlonly </li>
 *          <li><b><i>buffer</i></b> - [In] Pointer to data to send </li>
 *          <li><b><i>bytes_available</i></b> - [In] Number of bytes to send </li>
 *          <li><b><i>bytes_used</i></b> - [OUT] Number of bytes sent </li>
 *        </ul>
 * </td>
 * </tr> 
 * <tr> <th colspan="2" class="title">Return Values</th> </tr> 
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully sent some data to Device Cloud</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback is busy and needs to be called again to send data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 * <td>Callback was unable to send data due to irrecoverable communications error. 
 *     Cloud Connector will @endhtmlonly @ref close "close" @htmlonly the network handle</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
 * {
 *     connector_callback_status_t status = connector_callback_continue;
 *     app_ssl_t * const ssl_ptr = data->handle;
 *     int const bytes_sent = SSL_write(ssl_ptr->ssl, data->buffer, data->bytes_available);
 *
 *     if (bytes_sent <= 0)
 *     {
 *         // call SSL_get_error() to find out exact cause
 *         APP_DEBUG("SSL_write failed %d\n", bytes_sent);
 *         status = connector_callback_error;
 *     }
 * 
 *     data->bytes_used = (size_t) bytes_sent;
 * 
 *     return status;
 * }
 * @endcode
 *
 * @section ssl_receive Receive
 *
 * Callback is called to receive a specified number of bytes of data from the Device
 * Cloud.  This function must not block.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_network_tcp_receive() in network_tcp_ssl.c.
 *
 * @note In the multithreaded model (connector_run()) this is the point where the ICC
 * will relinquish control of the CPU.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr> 
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_network_receive @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th> 
 * <td>Pointer to @endhtmlonly @ref connector_network_receive_t "connector_network_receive_t" @htmlonly structure
 *        <ul>
 *          <li><b><i>handle</i></b> - [In] @endhtmlonly @ref connector_network_handle_t "Network handle" @htmlonly </li>
 *          <li><b><i>buffer</i></b> - Pointer to memory where callback places received data </li>
 *          <li><b><i>bytes_available</i></b> - [In] Size of memory buffer </li>
 *          <li><b><i>bytes_used</i></b> - [OUT] Number of bytes received </li>
 *        </ul>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr> 
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully received some data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback is busy and needs to be called again to receive data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 * <td> Callback was unable to receive data due to irrecoverable communications error. 
 *     Cloud Connector will @endhtmlonly @ref close "close" @htmlonly the network handle</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 *  static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
 * {
 *     connector_callback_status_t status = connector_callback_continue;
 *     app_ssl_t * const ssl_ptr = data->handle;
 *     int bytes_read = 0;
 *
 *     if (SSL_pending(ssl_ptr->ssl) == 0)
 *     {
 *         int ready;
 *         struct timeval timeout;
 *         fd_set read_set;
 *
 *         timeout.tv_sec = read_data->timeout;
 *         timeout.tv_usec = 0;
 *
 *         FD_ZERO(&read_set);
 *         FD_SET(ssl_ptr->sfd, &read_set);
 *
 *         // Blocking point for Cloud Connector
 *         ready = select(ssl_ptr->sfd + 1, &read_set, NULL, NULL, &timeout);
 *         if (ready == 0)
 *         {
 *             status = connector_callback_busy;
 *             goto done;
 *         }
 *
 *         if (ready < 0)
 *         {
 *            APP_DEBUG("app_network_receive: select failed\n");
 *            goto done;
 *         }
 *     }
 *
 *     bytes_read = SSL_read(ssl_ptr->ssl, data->buffer, data->bytes_available);
 *     if (bytes_read <= 0)
 *     {
 *         // EOF on input: the connection was closed.
 *         APP_DEBUG("SSL_read failed %d\n", bytes_read);
 *         status = connector_callback_error;
 *     }
 * 
 *     data->bytes_used = (size_t)bytes_read;
 *
 * done:
 *     return status;
 * }
 *
 * @endcode
 *
 * @section ssl_close Close
 *
 * Callback is called to close a connection between a device and Device Cloud. This API
 * should release all the SSL based allocation.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_network_tcp_close() in network_tcp_ssl.c.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr> 
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_network_close @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th> 
 * <td>Pointer to @endhtmlonly @ref connector_network_close_t "connector_network_close_t" @htmlonly structure
 *        <ul>
 *          <li><b><i>handle</i></b> - [In] @endhtmlonly @ref connector_network_handle_t "Network handle" @htmlonly </li>
 *          <li><b><i>status</i></b> -  [IN] @endhtmlonly @ref connector_close_status_t "Reason for closing the network handle" @htmlonly </li>
 *          <li><b><i>reconnect</i></b> - [OUT] The callback must set it to @endhtmlonly @ref connector_true @htmlonly to restart the transport or 
 *                                              to @endhtmlonly @ref connector_false @htmlonly to avoid restarting the transport</li> 
 *        </ul>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr> 
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully closed the connection</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback is busy completing the network close and will be called again</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to close the connection and callback aborted ICC</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
 * {
 *
 *     connector_callback_status_t status = connector_callback_continue;
 *     app_ssl_t * const ssl_ptr = data->handle;
 *
 *     // send close notify to peer
 *     if (SSL_shutdown(ssl_ptr->ssl) == 0) 
 *         SSL_shutdown(ssl_ptr->ssl);  // wait for peer's close notify
 *
 *     if (ssl_ptr->ssl != NULL) 
 *     {
 *         SSL_free(ssl_ptr->ssl);
 *         ssl_ptr->ssl = NULL;
 *     }
 *
 *     if (ssl_ptr->ctx != NULL) 
 *     {
 *         SSL_CTX_free(ssl_ptr->ctx);
 *         ssl_ptr->ctx = NULL;
 *     }
 *
 *     if (ssl_ptr->sfd != -1) 
 *     {
 *         close(ssl_ptr->sfd);
 *         ssl_ptr->sfd = -1;
 *     }
 *     data->reconnect = connector_true;
 *     return status;
 * }
 *
 * @endcode
 *
 * @htmlinclude terminate.html
 */
