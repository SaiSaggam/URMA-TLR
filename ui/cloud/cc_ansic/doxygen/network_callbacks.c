
/*! @page network_callbacks Network Callbacks
 *
 * @htmlinclude nav.html
 *
 * @section net Network Callbacks
 * Cloud Connector interfaces to the platform's network interface through the callbacks listed below.  These
 * are called through the application callback described in the @ref api1_overview.
 *
 *  -# @ref open
 *  -# @ref send
 *  -# @ref receive
 *  -# @ref close
 * <br /><br />
 *
 * @section open Open
 *
 * This callback is invoked to start a transport for communicating with Device Cloud. For
 * @ref connector_class_id_network_tcp transport it establishes a connection between Cloud Connector
 * and Device Cloud.
 *
 * The @ref connector_class_id_network_udp transport does not necessarily establish a connection, but it can resolve
 * a domain name and open a communication socket in this callback. Remember that UDP does not guarantee reliable data
 * delivery.
 *
 * If the @ref connector_class_id_network_sms transport does not establish a connection, it's very likely that 
 * during this callback the user configures the Device Cloud phone number where to send SMSs available through 
 * the data->device_cloud_url parameter. Final implementation has to be done by the user as a way to 
 * communicate with the hardware in charge of sending and receiving SMSs is not standard at all. As a 
 * reference, for the Linux platform sample, network_sms.c file implements support for @htmlonly <a href="http://wammu.eu/gammu/">Gammu</a>@endhtmlonly, a cellular
 * manager for mobile phones/modems.
 * @note This callback is called also after an SMS provisioning message arrives from the server to let the user
 * reconfigure the Device Cloud phone number if necessary.
 *
 * The callback is responsible of setting up any socket options for TCP and UDP transports.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function:
 * <ul>
 *   <li> @ref app_network_tcp_open() in network_tcp.c</li>
 *   <li> @ref app_network_udp_open() in network_udp.c</li>
 *   <li> @ref app_network_sms_open() in network_sms.c</li>
 * </ul>
 *
 * @see @ref network_tcp_start "Start TCP transport automatically or manually"
 * @see @ref network_udp_start "Start UDP transport automatically or manually"
 * @see @ref network_sms_start "Start SMS transport automatically or manually"
 * <br /><br />
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>
 *    <ul>
 *       <li>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly - Uses
 *         <ul>
 *           <li>@endhtmlonly @ref CONNECTOR_PORT @htmlonly for non-secure port number.</li>
 *           <li>@endhtmlonly @ref CONNECTOR_SSL_PORT @htmlonly for secure port number.</li>
 *         </ul>
 *       </li>
 *       <br />
 *       <li>@endhtmlonly @ref connector_class_id_network_udp @htmlonly - Uses
 *         <ul>
 *           <li>@endhtmlonly @ref CONNECTOR_UDP_PORT @htmlonly for non-secure port number.</li>
 *         </ul>
 *       </li>
 *       <br />
 *       <li>@endhtmlonly @ref connector_class_id_network_sms @htmlonly - Does not use any defined macro.</li>
 *    </ul>
 * </td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_network_open @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_network_open_t "connector_network_open_t" @htmlonly structure
 *        <ul>
 *          <li><b><i>device_cloud.url</i></b> - [IN] Pointer to Device Cloud URL. Used for TCP and UDP transports</li>
 *          <li><b><i>device_cloud.phone</i></b> - [IN] Pointer to Device Cloud Phone Number where to send SMSs. Used for SMS transport</li>
 *          <li><b><i>handle</i></b> - [OUT] Returned @endhtmlonly @ref connector_network_handle_t "network handle" @htmlonly which is used throughout network callbacks </li>
 *        </ul>
 * </td>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully established a connection</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback is busy connecting and needs to be called back again</td>
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
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 * <br /><br />
 *
 * @section send Send
 *
 * Callback is called to send data to Device Cloud. This function must not block.
 * Number of bytes actually sent could be less than the requested number.
 *
 * If the callback could not send any data because it encountered EAGAIN or EWOULDBLOCK error,
 * it must return @ref connector_callback_busy and Cloud Connector would continue
 * calling this function.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function:
 * <ul>
 *   <li> @ref app_network_tcp_send() in network_tcp.c</li>
 *   <li> @ref app_network_udp_send() in network_udp.c</li>
 *   <li> @ref app_network_sms_send() in network_sms.c</li>
 * </ul>
 * <br />
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>
 *    <ul>
 *       <li>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly</li>
 *       <li>@endhtmlonly @ref connector_class_id_network_udp @htmlonly</li>
 *       <li>@endhtmlonly @ref connector_class_id_network_sms @htmlonly</li>
 *    </ul>
 * </td>
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
 * <td>Callback successfully sent data to Device Cloud</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback could not send data due to temporary unavailability of resources. It needs to be called again to send data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 * <td>Callback was unable to send data due to irrecoverable communications error.
 *     Cloud Connector will @endhtmlonly @ref close "close" @htmlonly the network handle</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 * <br /><br />
 *
 * @section receive Receive
 *
 * Callback called to receive a specified number of data bytes from
 * Device Cloud.  This function must not block.

 * Number of bytes actually received could be less than the size of the buffer number. If there was no data pending
 * and the callback has encountered EAGAIN or EWOULDBLOCK error, it must return @ref connector_callback_busy
 * and Cloud Connector would continue calling this function.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function:
 * <ul>
 *   <li> @ref app_network_tcp_receive() in network_tcp.c</li>
 *   <li> @ref app_network_udp_receive() in network_udp.c</li>
 *   <li> @ref app_network_sms_receive() in network_sms.c</li>
 * </ul>
 * <br />
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>
 *    <ul>
 *       <li>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly</li>
 *       <li>@endhtmlonly @ref connector_class_id_network_udp @htmlonly</li>
 *       <li>@endhtmlonly @ref connector_class_id_network_sms @htmlonly</li>
 *    </ul>
 * </td>
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
 * <td>There is no data pending to receive. Callback will be called again to receive data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 * <td> Callback was unable to receive data due to irrecoverable communications error.
 *     Cloud Connector will @endhtmlonly @ref close "close" @htmlonly the network handle</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 * <br /><br />
 *
 * @section close Close
 *
 * Application callback request to close a network handle
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function:
 * <ul>
 *   <li> @ref app_network_tcp_close() in network_tcp.c</li>
 *   <li> @ref app_network_udp_close() in network_udp.c</li>
 *   <li> @ref app_network_sms_close() in network_sms.c</li>
 * </ul>
 * <br />
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>
 *    <ul>
 *       <li>@endhtmlonly @ref connector_class_id_network_tcp @htmlonly</li>
 *       <li>@endhtmlonly @ref connector_class_id_network_udp @htmlonly</li>
 *       <li>@endhtmlonly @ref connector_class_id_network_sms @htmlonly</li>
 *    </ul>
 * </td>
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
 *                                              to @endhtmlonly @ref connector_false @htmlonly to avoid restarting the transport.
 *                                              This value is ignored if transport is closed by a @endhtmlonly @ref connector_initiate_transport_stop @htmlonly action.</li>
 *        </ul>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully closed the connection </td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback was unable to complete the network close (most likely due to a blocking network handle)
 *     and will be called again</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @htmlinclude terminate.html
 */
