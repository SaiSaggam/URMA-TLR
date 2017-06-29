/*! @page shortmessaging Short Messaging
 *
 * @htmlinclude nav.html
 *
 * @section secsmintroduction Introduction
 *
 * Short Messaging (SM) provides an alternative transport for communicating with the Cloud and is 
 * typically used only when data usage is costly and guaranteed data delivery is unnecessary. SM 
 * uses UDP or SMS as a transport rather than TCP transport normally used in device-to-cloud communications.
 *
 * TCP communication includes overhead to establish, maintain, and close connections as well as 
 * overhead to ensure delivery and integrity. The communication is reliable and can be 
 * @ref network_ssl_callbacks "secured". Most Cloud communications discussed in this guide leverage
 * standard TCP communications.
 * 
 * SM communicates over UDP or SMS and therefore does not incur the TCP connection overhead. 
 * Unfortunately, UDP and SMS include limitations such as unreliable delivery and the potential for 
 * duplicate packets. Applications that include SM will require consideration and handling
 * of these transports' limitations - a price you may choose to pay to reduce data usage on costly
 * networks such as cellular or satellite.
 *
 * @section smfeatures What services are supported by Short Messaging?
 * 
 * SM protocol also differs from EDP (TCP-transport only) in the facilities that are supported. See the following table:
 * 
* @htmlonly
 * <table class="apitable">
 * <tr>
 *   <th colspan="4" class="title">Features supported by each transport</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Feature</th>
 *   <th class="subtitle">TCP</th>
 *   <th class="subtitle">UDP</th>
 *   <th class="subtitle">SMS</th>
 * </tr>
 * <tr>
 *   <td>Reboot</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 * </tr>
 * <tr>
 *   <td>Data Services</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 * </tr>
 * <tr>
 *   <td>Data Points</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 * </tr>
 * <tr>
 *   <td>File system</td>
 *   <td>Yes</td>
 *   <td>No</td>
 *   <td>No</td>
 * </tr>
 * <tr>
 *   <td>Remote Configuration Interface (RCI)</td>
 *   <td>Yes</td>
 *   <td>No</td>
 *   <td>No</td>
 * </tr>
 * <tr>
 *   <td>Firmware download</td>
 *   <td>Yes</td>
 *   <td>No</td>
 *   <td>No</td>
 * </tr>
 * <tr>
 *   <td>Secure (SSL) connection</td>
 *   <td>Yes</td>
 *   <td>No</td>
 *   <td>No</td>
 * </tr>
 * <tr>
 *   <td>Ping</td>
 *   <td>No</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 * </tr>
 * <tr>
 *   <td>Command Line Interface (CLI)</td>
 *   <td>No</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 * </tr>
 * <tr>
 *   <td>Connect (start TCP)</td>
 *   <td>No</td>
 *   <td>Yes</td>
 *   <td>Yes</td>
 * </tr>
 * <tr>
 *   <td>Cloud to Connector Device ID provisioning</td>
 *   <td>Yes</td>
 *   <td>No</td>
 *   <td>No</td>
 * </tr>
 * <tr>
 *   <td>Cloud to Connector phone number provisioning</td>
 *   <td>No</td>
 *   <td>No</td>
 *   <td>Yes</td>
 * </tr>
 * <tr>
 *   <td>Connector to Cloud phone number provisioning</td>
 *   <td>No</td>
 *   <td>No</td>
 *   <td>No</td>
 * </tr>
 * </table>
 * @endhtmlonly
 * 
 * @section smsectionwhoneedsit Should I use Short Messaging?
 * 
 * Most devices will be deployed on networks with few restrictions on data usage. For these 
 * applications, TCP should be used and is the method demonstrated in all of the samples included in 
 * this kit. These applications will establish a TCP connection and maintain that connection so that 
 * the device is always securely accessible from the cloud.
 * 
 * Conversely, some devices could be deployed on networks that include data restrictions. Others may 
 * be battery-powered and must minimize active network time. For this latter group, Short Messaging 
 * should be considered.
 *
 * @subsection smsolutions How does Short Messaging solve my problem?
 *
 * A device does not need to always have a TCP connection. Further, some devices can eliminate
 * this connection altogether.  For example, a cellular-based device might send GPS data to
 * Device Cloud every few hours and then check for remote instructions.  Based on the instruction,
 * the application could then start a TCP connection and perform a standard Cloud Connector
 * operation.
 *
 * Alternatively, a battery-based device might power up, read a sensor,
 * send the data to Device Cloud, check for pending @ref pending_data "Web Service" instructions,
 * and then shut down if no instructions are queued.
 *
 * In these two examples, minimizing network traffic is essential.  In the latter example,
 * shutting off power as soon as possible is critical.
 *
 * The system requirements of your application will have radical effects on your SM design.
 * You can always optimize the power consumption, or network traffic, but this
 * comes with the additional application overhead.
 *
 * @subsection smcomplications How does Short Messaging complicate my application?
 *
 * <b>UDP and SMS Reliability </b>
 *
 * Depending on what you optimize, a device will need to consider
 * the unreliability of UDP and SMS.
 *
 * Suppose that your application collects data periodically and sends it to Device Cloud once
 * per hour.  What if the data is lost?  Can your system tolerate this?  If not, you will
 * need your application to check for acknowledgments.  After some period of time
 * you will have to timeout and re-send the data.  This adds complexity to your Cloud
 * Connector application.
 *
 * Now consider your @ref web_services application that pulls this data from Device Cloud.
 * What happens if the data sent from the last sample was received by Device Cloud, but the
 * acknowledgment was lost?  Now your data is duplicated on Device Cloud and your
 * @ref web_services application will need to handle this case.
 *
 * Suppose data integrity is a critical requirement.  In this case, a Cloud Connector
 * application might choose to start a TCP connection and send the data reliably
 * instead of using the UDP or SMS transports.
 *
 * Cloud Connector has the flexibility to support either case.
 *
 * <b>Punching through Ethernet Firewalls for SM over UDP transport</b>
 *
 * Considering most devices are deployed behind an Ethernet Firewall, Device Cloud is incapable of
 * initiating an SM over UDP message exchange.  All @ref web_services "Web Services" SM over UDP requests are queued
 * on Device Cloud, waiting for a Cloud Connector message.
 *
 * Cloud Connector @b must @b always @b initiate SM over UDP exchanges.  A key component of
 * your implementation must include a strategy to open the firewall and
 * drain the SM over UDP requests.
 *
 * If Cloud Connector has no pending message to send, the application can @ref initiate_ping_to_cloud.
 *
 * @section smsectioninitialconfig Short Messaging requires initial configuration on Device Cloud
 *
 * In order to use SM protocol, some initial configuration steps are required on the server in addition to the
 * ones required for TCP transport like @ref add_your_device_to_the_cloud "Adding your Device to Device Cloud". 
 *
 * If initial configuration steps are not done, none of the features described in following chapters will work:
 *      -# Initial configuration on the cloud for UDP transport: \n 
 *         It's required to enable this transport using @ref sm_upd_enable_dm "Device Management" or through 
 *         @ref sm_upd_enable_ws "Web Services". \n
 *         UDP transport allows an option for "Battery-backed" applications, that if desired, has to be enabled on the
 *         server using @ref sm_udp_enable_battery_dm "Device Management" or through @ref sm_udp_enable_battery_ws "Web Services".
 *      -# Initial configuration on the Cloud for SMS transport: \n 
 *         It's required to enable this transport by providing your device's phone number. This way Device Cloud 
 *         can match incoming SMS messages to a registered Device ID.\n
 *         This configuration can be done using @ref sm_sms_enable_dm "Device Management" or through 
 *         @ref sm_sms_enable_ws "Web Services".
 *
 * @subsection sm_upd_enable_dm Enable UDP transport using Device Management
 *
 * Log into your Device Cloud account and click on your device (described in the @ref connector_login "Getting Started Section").
 * It's not required that the device's status indicate 'Connected' (that only represents the TCP transport status). 
 * Select the device to configure.
 *
 * @image html cloud_device_connected.png
 *
 * Then click the @b More button and under @b SM/UDP select @b Configure.
 *
 * @image html cloud_sm_udp_configure.png
 *
 * Then click on the @b SM/UDP @b Service @b Enabled checkbox.
 *
 * @image html cloud_sm_udp_configure_enable.png
 *
 * To learn more about Device Cloud UDP configuration using Device Management, see SM/UDP chapter in the
 * @htmlonly <a href="http://ftp1.digi.com/support/documentation/html/90001150/index.html">Device Cloud User's Guide</a>@endhtmlonly.
 *
 * @subsection sm_upd_enable_ws Enable UDP transport using Web Services
 *
 * A python script is provided to Enable or Disable UDP transport for a device using
 * @htmlonly <a href="web_services.html">Web Services.</a> @endhtmlonly
 *
 * Execute EnableUDP.py python script located in @a @b /connector/tools/python directory as follows:
 * @code
 * python EnableUDP.py <Username> <Password> <Device Cloud URL> <Device ID> <EnableUDP>
 * @endcode
 * For example:
 * @code
 * python EnableUDP.py myusername mypassword login.etherios.com 00000000-00000000-00049DFF-FFAAAAAA true
 * @endcode
 *
 * To learn more about Device Cloud UDP configuration using Web Services, see SM/UDP chapter in the
 * @htmlonly <a href="http://ftp1.digi.com/support/documentation/html/90002008/index.html">Device Cloud Programming Guide</a>@endhtmlonly.
 *
 * @subsection sm_udp_enable_battery_dm Enable "Battery-backed" SM applications for UDP transport using Device Management
 *
 * Follow the same steps described in @ref sm_upd_enable_dm and click on
 * the 'Battery Operated Mode' checkbox additionally.
 *
 * @image html cloud_sm_udp_configure_enable_battery.png
 *
 * To learn more about Device Cloud UDP "Battery-backed" SM applications, see the "Battery operated mode" subchapter in the 
 * SM/UDP chapter in the@htmlonly <a href="http://ftp1.digi.com/support/documentation/html/90001150/index.html">Device 
 * Cloud User's Guide</a>@endhtmlonly.
 *
 * @subsection sm_udp_enable_battery_ws Enable "Battery-backed" SM applications for UDP transport using Web Services
 *
 * A Web Service script written in python is provided to Enable (or Disable) "Battery Operated Mode" option for UDP 
 * transport for a device using @htmlonly <a href="web_services.html">Web Services.</a> @endhtmlonly
 *
 * Execute EnableUDP_Battery.py python script located in @a @b /connector/tools/python directory as follows:
 * @code
 * python EnableUDP_Battery.py <Username> <Password> <Device Cloud URL> <Device ID> <EnableUDP_Battery(true or false)>
 * @endcode
 * For example:
 * @code
 * python EnableUDP_Battery.py myusername mypassword login.etherios.com 00000000-00000000-00049DFF-FFAAAAAA true
 * @endcode
 *
 * @subsection sm_sms_enable_dm Enable/Configure SMS transport using Device Management
 *
 * Log into your Device Cloud account and click on your device (described in the @ref connector_login "Getting Started Section").
 * It's not required that the device's status indicates that it is 'Connected' (that only represents the TCP transport status). 
 * Select the device to configure.
 *
 * @image html cloud_device_connected.png
 *
 * Then click the More button and under SMS select Configure.
 *
 * @image html cloud_sm_sms_configure.png
 *
 * Then type your device's phone number in the 'Phone Number' textbox.
 *
 * You can optionally type your device's ICCID number in the 'ID' textbox.
 *
 * @image html cloud_sm_sms_configure_enable.png
 *
 * To learn more about Device Cloud SMS configuration using Device Management, see SMS chapter in the
 * @htmlonly <a href="http://ftp1.digi.com/support/documentation/html/90001150/index.html">Device Cloud User's Guide</a>@endhtmlonly.
 *
 * @subsection sm_sms_enable_ws Enable/Configure SMS transport using Web Services
 *
 * A Web Service script written in python is provided to Configure (and hence enable SMS transport) your device's phone number using
 * @htmlonly <a href="web_services.html">Web Services.</a> @endhtmlonly
 * It will first try to find current phone configuration for the device-id to update it. If no entry is found, a new entry is created 
 * with the provided device's phone number.
 *
 * Execute ProvisionSMS.py python script located in @a @b /connector/tools/python directory as follows:
 * @code
 * python ProvisionSMS.py <Username> <Password> <Device Cloud URL> <Device ID> <Device phone Number>
 * @endcode
 * For example:
 * @code
 * python ProvisionSMS.py myusername mypassword login.etherios.com 00000000-00000000-00049DFF-FFAAAAAA 54123456789
 * @endcode
 * 
 * To learn more about Device Cloud SMS configuration using Web Services, see SMS chapter in the
 * @htmlonly <a href="http://ftp1.digi.com/support/documentation/html/90002008/index.html">Device Cloud Programming Guide</a>@endhtmlonly.
 *
 * @section smsectionexamples Short Messaging Features
 *
 * The SM protocol includes four major features:
 *      -# @ref initiate_ping_to_cloud "Initiate Ping" to Device Cloud. Especially useful for UDP transport 
 *                                     to open the Firewall between Cloud Connector and Device Cloud
 *      -# @ref data_service "Data transfer" between Cloud Connector and Device Cloud
 *      -# @ref cli_support "Command Line Interface" support for Device Management
 *      -# @ref sm_connect "Request Connection" from Device Cloud to start the Cloud Connector TCP transport
 *
 * An additional major feature is available for @ref sm_udp_enable_battery_dm "Battery-backed" SM applications over UDP:
 *      -# @ref pending_data "Message pending" to notify applications more messages are queued.
 *
 * An additional major function is available to do @ref sm_sms_enable_dm "SMS Provisioning" for SM over SMS:
 *      -# @ref config_request_from_cloud "SMS Provisioning" to notify applications that an SMS provisioning message
 *                                                           has been received from Device Cloud. This way the device
 *                                                           configures the phone number where to send SMS messages.
 *
 * Several @ref additional_apis "SM convenience APIs" are also described below.
 *
 * @section ping_request Ping Device Cloud
 *
 * A device can send a @ref initiate_ping_to_cloud "Ping request to Device Cloud" at any
 * time. It is especially useful for UDP transport to open a hole in a firewall and let Device Cloud know that the 
 * application is ready to receive pending or queued messages.
 *
 * @subsection initiate_ping_to_cloud  Initiate Ping Device Cloud
 *
 * When an application has only @ref network_udp_start "UDP started" (TCP has @ref network_tcp_start
 * "not started"), it's necessary to periodically contact Device Cloud so to open a hole in a firewall
 * and receive data or commands from Device Cloud or @ref web_services applications.  This can be achieved
 * by initiating a Ping Device Cloud.
 *
 * The application initiates a Ping Device Cloud by calling @ref connector_initiate_action()
 * using a @ref connector_initiate_ping_request type and a connector_sm_send_ping_request_t
 * request_data structure:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 *   <th colspan="2" class="title">Arguments</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Name</th>
 *   <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>handle</td>
 *   <td>@endhtmlonly @ref connector_handle_t "Handle" returned from the connector_init() @htmlonly function.</td>
 * </tr>
 * <tr>
 *   <td>request</td>
 *   <td>@endhtmlonly @ref connector_initiate_ping_request @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_send_ping_request_t @htmlonly structure, where member:
 *      <ul>
 *        <li><b><i>transport</i></b>: @endhtmlonly Transport to use: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *        <li><b><i>user_context</i></b>: An opaque application-defined context.  This pointer is returned
 *                                        during a @endhtmlonly @ref ping_response_callback "response callback". @htmlonly </li>
 *        <li><b><i>response_required</i></b>: Set to @endhtmlonly @ref connector_true if notification of ping receipt is needed,
 *                                             otherwise set to @ref connector_false.  See @ref ping_response_callback . @htmlonly </li>
 *        <li><b><i>request_id</i></b>, pointer to where to store the session's Request ID. This value is saved by Cloud Connector after a successful connector_initiate_action()
 *                                      and might be used for @endhtmlonly @ref initiate_session_cancel "canceling the session" @htmlonly. <b>Only valid for SM</b>. Set to NULL if cancel is not going to be used. </li>
 *        <li><b><i>timeout_in_seconds</i></b>, outgoing sessions timeout in seconds. <b>Only valid for SM</b>. Use SM_WAIT_FOREVER to wait forever for the complete request/response </li>
 *      </ul>
 *    </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_success @htmlonly</th>
 *   <td>Device Cloud Ping initiated</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_unavailable @htmlonly</th>
 *   <td>Cloud Connector is not running (not started or stopped) </td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_service_busy @htmlonly</th>
 *   <td>Previous SM request is not yet processed</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 *
 * An example of initiating a ping is shown below:
 * @code
 *   static connector_sm_send_ping_request_t ping_request;
 *
 *   ping_request.transport = connector_transport_udp;
 *   ping_request.response_required = connector_true;
 *   ping_request.timeout_in_seconds = SM_WAIT_FOREVER;
 *   ping_request.request_id = NULL;
 *
 *   // Send ping to Device Cloud
 *   status = connector_initiate_action(handle, connector_initiate_ping_request, &ping_request);
 * @endcode
 *
 * This example will invoke Cloud Connector to initiate a ping request to Device Cloud. The completion
 * of this request is indicated by a @ref ping_response_callback "response callback".
 *
 * @note The ping_request variable must remain in scope or allocated until the @ref ping_response_callback "response" callback. 
 *
 * @see @ref connector_initiate_action()
 * @see @ref ping_response_callback
 *
 * @subsection ping_response_callback  Ping Device Cloud Response callback
 *
 * The ping_response_callback is made for one of the following conditions: 
 * the ping was successfully sent or a ping response has been received
 * from the cloud, the ping timed out, or an error occurred. 
 *
 * @note When @ref initiate_ping_to_cloud operation has the response_required set @ref connector_false, this callback
 * is made with the @ref connector_sm_ping_response_t status set to @b connector_sm_ping_status_complete when the
 * message is sent.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_ping_response @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_ping_response_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Will be set to the transport configured in the ping request: @ref connector_transport_udp 
 *                                    or @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>user_context</i></b>: The user_context pointer from the  @endhtmlonly @ref initiate_ping_to_cloud. @htmlonly </li>
 *       <li><b><i>status</i></b>: The response code, where @endhtmlonly @b connector_sm_ping_status_success indicates reply
 *                 received, @b connector_sm_ping_status_complete indicates @ref initiate_ping_to_cloud sent. @htmlonly </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 * <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 * <td>Aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @note The response code connector_sm_ping_status_success will only be received if the @ref initiate_ping_to_cloud request has the 
 * @b response_required set to @ref connector_true and a ping response has been received from the Cloud.
 *
 * @see @ref initiate_ping_to_cloud
 *
 *
 * @section pending_data  Pending Data Available (Only used for UDP transport)
 *
 * Cloud Connector will make a @ref connector_request_id_sm_more_data @ref connector_callback_t "callback"
 * to notify the application that additional SM UPD messages are pending. Applications should
 * call @ref initiate_ping_to_cloud to retrieve pending messages from Device Cloud.
 *
 * This callback is geared towards @ref sm_udp_enable_battery_dm "Battery-backed" applications.   This callback
 * can be used to re-trigger a timer which initiates a power down sequence.  When this callback occurs, the
 * timer should be reset, extending the time before shutdown.
 *
 * The @ref connector_request_id_sm_more_data "pending data" @ref connector_callback_t "callback"
 * is called with the following information:
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_more_data @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_more_data_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Will be set to @ref connector_transport_udp. @htmlonly </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @note This callback is made only when @ref sm_udp_enable_battery_dm "Battery-backed" is enabled.
 *
 * @see @ref initiate_ping_to_cloud
 * @see @ref sm_udp_enable_battery_dm "Handling Battery-backed Applications"
 *
 * @section cli_support Command Line Interface Support
 *
 * Cloud Connector includes support for a Command Line Interface (CLI) support to be displayed on the Device
 * Cloud Device Management CLI.
 *
 * @subsection cli_cb_sequence CLI Callback Sequence
 * The following SM callbacks are made (in sequence) to support an individual CLI request:
 *      -# @ref cli_request_callback
 *      -# @ref cli_response_length_callback
 *      -# @ref cli_response_callback
 *      -# @ref cli_status_callback
 *
 * @subsection cli_request_callback  CLI request callback
 *
 * The @ref connector_request_id_sm_cli_request callback is the initial call
 * in a @ref cli_cb_sequence.  This callback is triggered by a Device Management CLI request and it
 * passes the Device Cloud command and arguments to the application in a connector_sm_cli_request_t data structure. 
 * Device Management sends the CLI request NULL terminated. 
 * If the CLI request is long enough to be split into several segments, this callback will be called several times. 
 * The last segment (end of CLI request) has 'more_data' field set to @ref connector_true and it's the only one ending 
 * with a NULL ('\0') character.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_cli_request @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_cli_request_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Transport to use: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>user_context</i></b>: An opaque application-defined context.  This pointer is passed to all subsequent
 *                                        callbacks during the CLI sequence for this command.
 *       <li><b><i>buffer</i></b>: Buffer containing the Device Cloud CLI command. Only last segment will be NULL terminated </li>
 *       <li><b><i>bytes_used</i></b>: Number of bytes used for the CLI command in the buffer. </li>
 *       <li><b><i>response_required</i></b>: Set to @endhtmlonly @ref connector_true if a response
 *                                            is requested for this CLI command.  @ref connector_false if no
 *                                            response required.  @htmlonly </li>
 *       <li><b><i>more_data</i></b>: Set to @endhtmlonly @ref connector_true @htmlonly if more cli requests are comming (this callback
 *                                    will be called again) or @endhtmlonly @ref connector_false @htmlonly if this is last chuck
 *                                    of cli request.
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 * <td>Continue</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_busy @htmlonly</th>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_error @htmlonly</th>
 *   <td>Application encountered error</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @see @ref cli_response_length_callback
 * @see @ref cli_response_callback
 * @see @ref cli_status_callback
 *
 * @subsection cli_response_length_callback  Response length callback
 *
 * The @ref connector_request_id_sm_cli_response_length callback is the second call
 * in a @ref cli_cb_sequence and used to get the desired maximum size of CLI response length in bytes.
 * This callback is made only if the @b response_required was set to @ref connector_true in
 * the initial @ref cli_request_callback call. The available response length can be later be limited 
 * if 'maxResponseSize' attribute was specified in the Device Management CLI request. 
 *
 * Cloud Connector will allocate this memory block to process the ensuing @ref cli_request_callback
 * sequence.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_cli_response_length @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_cli_response_length_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Transport to use: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>user_context</i></b>: The opaque application-defined context passed in from the @endhtmlonly @ref cli_request_callback @htmlonly </li>
 *       <li><b><i>total_bytes</i></b>: Maximum size in bytes of the CLI response to the initial @endhtmlonly @ref cli_request_callback @htmlonly </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_busy @htmlonly</th>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @note The actual bytes used during @ref cli_response_callback sequence must be less than or equal
 * to the @b total_bytes set by this callback.
 *
 * @see @ref cli_request_callback
 * @see @ref cli_response_callback
 *
 * @subsection cli_response_callback  CLI response callback
 *
 * The @ref connector_request_id_sm_cli_response callback is the third call
 * in a @ref cli_cb_sequence and is used to assemble a CLI response to send to Device Cloud.
 * This callback is made only if the @b response_required was set to @ref connector_true in
 * the initial @ref cli_request_callback call and after the @b cli_response_length_callback was
 * called to define the maximum CLI response size.
 *
 * Cloud Connector will continue to make @ref connector_request_id_sm_cli_response callbacks
 * until the @b more_data field is set to @ref connector_false.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_cli_response @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_cli_response_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Transport to use: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>user_context</i></b>: The opaque application-defined context passed in from the @endhtmlonly @ref cli_request_callback @htmlonly </li>
 *       <li><b><i>buffer</i></b>: the memory to copy your CLI response. </li>
 *       <li><b><i>bytes_available</i></b>: buffer size available in bytes. </li>
 *       <li><b><i>bytes_used</i></b>: number of bytes copied by the callback.</li>
 *       <li><b><i>more_data</i></b>: Set to @endhtmlonly @ref connector_true if more data expected.
 *                                    @ref connector_false if your response is complete.  @htmlonly
 *       </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_busy @htmlonly</th>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_error @htmlonly</th>
 *   <td>Application encountered error</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @note This callback is used to fill a private buffer allocated after the @ref cli_response_length_callback.
 * The initial size of the @b bytes_available will reflect that size and get decremented for subsequent callbacks.
 * The total number of @b bytes_used for all these callbacks must not exceed the maximum length set by cli_response_length_callback.
 *
 * @see @ref cli_request_callback
 * @see @ref cli_response_length_callback
 * @see @ref cli_status_callback
 *
 * @subsection cli_status_callback  CLI status callback
 *
 * Cloud Connector will make @ref connector_request_id_sm_cli_status callback to pass the reason for session complete. 
 * It can indicate success or error if the @ref connector_initiate_stop_request_t "stop transport"
 * was initiated while processing a CLI request or if Cloud Connector fails to allocate the required resources.
 * User will receive this callback when Device Connector finishes with the session. 
 * User can free their user_context and any other reserved data as soon as they receive this callback.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_cli_status @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_cli_status_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Transport to use: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>user_context</i></b>: The opaque application-defined context passed in from the @endhtmlonly @ref cli_request_callback @htmlonly </li>
 *       <li><b><i>status</i></b>: @endhtmlonly
 *                                 <ul>
 *                                 <li>@b connector_sm_cli_status_success session finished successfully</li>
 *                                 <li>@b connector_sm_cli_status_cancel if @ref connector_initiate_stop_request_t "transport stopped"</li>
 *                                 <li>@b connector_sm_cli_status_error if resource allocation failure. @htmlonly</li>
 *                                 </ul>
 *       </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @see @ref cli_request_callback
 * @see @ref cli_response_length_callback
 * @see @ref cli_response_callback
 *
 * @section additional_apis Additional SM APIs
 *
 * The following SM APIs are also available.
 *
 * @subsection sm_connect Request TCP start
 *
 * Requests the Cloud Connector to start its TCP transport.  This request is passed to the user by
 * a @ref connector_request_id_sm_request_connect request to allow the TCP transport to be started or not.
 *
 * Once the TCP transport @ref network_tcp_start "is started", devices can make use of
 * the TCP features like @ref firmware_download, @ref rci_service, or reliable
 * @ref data_point or @ref data_service over a @ref connector_transport_tcp "TCP transport".
 *
 * The following @ref web_services example shows how to request a device to start its TCP service:
 *
 * @code
 *    <sci_request version="1.0">
 *      <send_message synchronous="false">
 *        <targets>
 *          <device id="00000000-00000000-00409DFF-FF432311"/>
 *        </targets>
 *        <sm_udp>
 *          <request_connect/>
 *        </sm_udp>
 *      </send_message>
 *    </sci_request>
 * @endcode
 *
 * @note If @ref CONNECTOR_TRANSPORT_TCP "CONNECTOR_TRANSPORT_TCP is disabled" Cloud Connector
 * returns an error response to Device Cloud.
 * 
 * @see @ref start_tcp_transport
 *
 * @subsection sm_reboot Reboot device
 *
 * Requests a Cloud Connector reboot.  After receiving this request, Cloud Connector
 * will invoke a @ref reboot callback.
 *
 * The following @ref web_services example shows how to reboot a device:
 *
 * @code
 *    <sci_request version="1.0">
 *      <send_message synchronous="false">
 *        <targets>
 *          <device id="00000000-00000000-00409DFF-FF432311"/>
 *        </targets>
 *        <sm_udp>
 *          <reboot/>
 *        </sm_udp>
 *      </send_message>
 *    </sci_request>
 * @endcode
 *
 *
* @subsection opaque_response  Unsequenced Message callback
 *
 * Cloud Connector will make @ref connector_request_id_sm_opaque_response @ref connector_callback_t "callback"
 * to notify an application that a response was received with no known associated request.  The reason
 * for this is either the outgoing session "timed-out" (see timeout_in_seconds field of 
 * @ref connector_request_data_service_send_t "connector_request_data_service_send_t",
 * @ref connector_request_data_point_t "connector_request_data_point_t", 
 * @ref connector_request_data_point_binary_t "connector_request_data_point_binary_t" and 
 * @ref connector_sm_send_ping_request_t "connector_sm_send_ping_request_t" structures),
 * or the transport was @ref connector_initiate_stop_request_t "terminated" and the message context lost.
 *
 * The @ref connector_request_id_sm_opaque_response "pending data" @ref connector_callback_t "callback"
 * is called with the following information:
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_opaque_response @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_opaque_response_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Will be set to @ref connector_transport_udp. @htmlonly </li>
 *       <li><b><i>id</i></b>: request ID associated with the response </li>
 *       <li><b><i>data</i></b>: received payload </li>
 *       <li><b><i>bytes_used</i></b>: number of bytes in the payload </li>
 *       <li><b><i>error</i></b>: is connector_true for error response for Device Cloud </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * See timeout_in_seconds field of following structures:
 * @ref connector_request_data_service_send_t "connector_request_data_service_send_t",
 * @ref connector_request_data_point_t "connector_request_data_point_t", 
 * @ref connector_request_data_point_binary_t "connector_request_data_point_binary_t" and 
 * @ref connector_sm_send_ping_request_t "connector_sm_send_ping_request_t".
 *
 * @subsection ping_request_from_cloud  Device Cloud Ping Notification
 *
 * Cloud Connector will make a Ping Request @ref connector_request_id_sm_ping_request "callback" to
 * notify an application that a ping was received.  This callback is informational only and no action
 * is necessary.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_ping_request @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_receive_ping_request_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Transport to use: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>response_required</i></b>: Set to @endhtmlonly @ref connector_true if Device Cloud requests a response,
 *                                            @ref connector_false if no response was requested.  @htmlonly </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 * 
 * @subsection start_tcp_transport  Device Cloud start TCP transport request
 *
 * Cloud Connector will make a Request Connect @ref connector_request_id_sm_request_connect "callback" to
 * notify an application that Device Cloud requests to start the TCP transport and let it decide whether to
 * allow it or not.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_request_connect @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_request_connect_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Transport that received the request to start TCP communication: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>allow</i></b>: Set to @endhtmlonly @ref connector_true to allow starting TCP communication,
 *                                            @ref connector_false to ignore the request.  @htmlonly </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @see @ref sm_connect
 *
 *
 * @subsection config_request_from_cloud  Device Cloud Config Notification
 *
 * Cloud Connector will make a Config Request @ref connector_request_id_sm_config_request "callback" to
 * notify an application that an SMS configuration (Provisioning) message was received.  This callback 
 * is mainly informative as following steps have already been done:
 *     -# At configuration layer: @ref set_device_cloud_phone has been called to let the user a chance 
 *        to store the new server phone number in persistent storage.
 *     -# At SMS transport layer: @ref app_network_sms_close() and app_network_sms_open() callbacks in network_sms.c
 *        have been called so the new phone can be immediately established.
 *
 * In order to send SMS messages to the Cloud from the device, it's required that the device has properly 
 * configured the Cloud phone number. If it's known forehand, it has to be configured through 
 * @ref CONNECTOR_CLOUD_PHONE define or @ref get_device_cloud_phone callback. 
 * If it's not known forehand, an SMSs provisioning message must arrive first from the Cloud so it's configured automatically.
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
 *   <td>class_id</td>
 *   <td>@endhtmlonly @ref connector_class_id_short_message @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_sm_config_request @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_receive_config_request_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>: @endhtmlonly Will be set to @ref connector_transport_sms. @htmlonly </li>
 *       <li><b><i>phone_number</i></b>: ascii rendering of the Device Cloud phone number (eg. "32075") </li>
 *       <li><b><i>service_id</i></b>: service id may be empty if shared codes are not being used </li>
 *       <li><b><i>response_required</i></b>: Set to @endhtmlonly @ref connector_true if Device Cloud requests a response,
 *                                            @ref connector_false if no response was requested.  @htmlonly </li>
 *     </ul>
 *   </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_continue @htmlonly</th>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @note The @b response_required member is passing information on to the application.  There is no required
 * action necessary from the Cloud Connector application, regardless of the value for @b response_required.
 *

 * @subsection initiate_session_cancel  Initiate an SM (UDP or SMS) session cancel
 *
 * This method is used to cancel a session that:
 * <ul>
 *     <li> Was initiated through any of following actions (request): 
 *          <ul>
 *              <li>connector_initiate_send_data</li>
 *              <li>connector_initiate_data_point</li>
 *              <li>connector_initiate_data_point_binary</li>
 *              <li>connector_initiate_ping_request</li>
 *          </ul>
 *     </li>
 *     <li> Transport used was UDP or SMS.</li>
 *     <li> Request's timeout has not elapsed.</li>
 * </ul>
 *
 * The application initiates a Session Cancel by calling @ref connector_initiate_action()
 * using a @ref connector_initiate_session_cancel or @ref connector_initiate_session_cancel_all types and a connector_sm_cancel_request_t
 * request_data structure:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 *   <th colspan="2" class="title">Arguments</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Name</th>
 *   <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>handle</td>
 *   <td>@endhtmlonly @ref connector_handle_t "Handle" returned from the connector_init() @htmlonly function.</td>
 * </tr>
 * <tr>
 *   <td>request</td>
 *   <td>@endhtmlonly @ref connector_initiate_session_cancel or @ref connector_initiate_session_cancel_all @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_data</td>
 *   <td>Pointer to @endhtmlonly connector_sm_cancel_request_t @htmlonly structure, where member:
 *      <ul>
 *        <li><b><i>transport</i></b>: @endhtmlonly Transport used by the session to cancel: @ref connector_transport_udp or @ref connector_transport_sms. @htmlonly </li>
 *        <li><b><i>request_id</i></b>: The Request ID of the session to cancel (got from previous call to connector_initiate_action). 
 *                                      For connector_initiate_session_cancel_all this is ignored and all current sessions for the transport specified are cancelled. </li>
 *      </ul>
 *    </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_success @htmlonly</th>
 *   <td>Session cancel initiated</td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_unavailable @htmlonly</th>
 *   <td>Cloud Connector is not running (not started or stopped) </td>
 * </tr>
 * <tr>
 *   <th>@endhtmlonly @ref connector_service_busy @htmlonly</th>
 *   <td>Previous SM request is not yet processed</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @see @ref connector_initiate_action()
 * @see @ref connector_initiate_session_cancel
 * @see @ref connector_initiate_session_cancel_all
 *
 *
 * @htmlinclude terminate.html
 */
