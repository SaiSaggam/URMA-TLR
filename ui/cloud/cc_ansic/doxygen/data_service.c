/*! @page data_service Data Service
 *
 * @htmlinclude nav.html
 *
 * @section data_service_overview Data Service Overview
 *
 * The Data Service API is used to send data to and from Device Cloud.  Data service transfers
 * are either initiated from Device Cloud (@ref device_request) or the device itself (@ref send_data_request).
 *
 * @li @ref send_data_request : Data transfers initiated by the device and used to write files on to Device Cloud.
 * Device Cloud may send a response back to the device indicating the transfer status.
 * @li @ref device_request : Transfers initiated from a web services client connected to Device Cloud which
 * hosts the device. This transfer is used to send data to the device and the device may send a response back.
 *
 * @note See @ref data_service_support under Configuration to disable the data service.
 *
 * @section send_data_request Send data
 *
 * This is one way the device can send data to Device Cloud. The other method is using @ref data_point.
 * It is the device originated transaction, starts when the connector_initiate_action() is called with request ID
 * @ref connector_initiate_send_data.
 *
 * Cloud Connector invokes the application-defined callback to get the actual data to send and to pass
 * Device Cloud response.
 *      -# @ref initiate_send_data
 *      -# @ref send_data_length_callback
 *      -# @ref send_data_callback
 *      -# @ref send_data_response_callback
 *      -# @ref send_data_status_callback
 *
 * Once the @ref initiate_send_data is called the callbacks will be called in following sequence:
 *      -# Cloud Connector calls application-defined callback @ref send_data_callback to get the user data (chunk of data).
 *      -# Cloud Connector calls application-defined callback @ref send_data_response_callback to pass Device Cloud response.
 *      -# Cloud Connector calls application-defined callback @ref send_data_status_callback to inform session complete if
 *         the response is not requested or error occurs while sending the data.
 *
 * For a non-tcp transport session, before making the very first call to get the user data, Cloud
 * Connector will call the @ref send_data_length_callback to get the total length of the response data.
 *
 * Cloud Connector calls @ref send_data_callback repeatedly to get all data.
 *
 * @note See @ref data_service_support under Configuration to enable the data service.
 * @note See @ref CONNECTOR_TRANSPORT_TCP and @ref network_tcp_start to enable and start TCP.
 * @note See @ref CONNECTOR_TRANSPORT_UDP and @ref network_udp_start to enable and start UDP.
 * @note See @ref CONNECTOR_TRANSPORT_SMS and @ref network_sms_start to enable and start SMS.
 *
 * @subsection initiate_send_data Initiate Send Data
 *
 * The application initiates the send data to Device Cloud by calling connector_initiate_action()
 * with @ref connector_initiate_send_data request and @ref connector_request_data_service_send_t request_data.
 *
 * The connector_initiate_action() is called with the following arguments:
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
 *   <td>@endhtmlonly @ref connector_handle_t @htmlonly returned from the @endhtmlonly connector_init() @htmlonly function.</td>
 * </tr>
 * <tr>
 *   <td>request</td>
 *   <td>@endhtmlonly @ref connector_initiate_send_data @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_data</td>
 *   <td>Pointer to @endhtmlonly connector_request_data_service_send_t @htmlonly structure, where member:
 *        <ul>
 *        <li><b><i>transport</i></b>, a method to use to send data </li>
 *        <li><b><i>user_context</i></b>, is the user owned context pointer </li>
 *        <li><b><i>path</i></b> is Device Cloud file path where data will be stored (shouldn't be stack variable) </li>
 *        <li><b><i>content_type</i></b> is "text/plain", "text/xml", "application/json", etc (shouldn't be stack variable) </li>
 *        <li><b><i>option</i></b>, is to inform Device Connector on what to do with the data </li>
 *        <li><b><i>response_required</i></b>, set to connector_true if the response is needed </li>
 *        <li><b><i>request_id</i></b>, pointer to where to store the session's Request ID. This value is saved by Cloud Connector after a successful connector_initiate_action()
 *                                      and might be used for @endhtmlonly @ref initiate_session_cancel "canceling the session" @htmlonly. <b>Only valid for SM</b>. Set to NULL if cancel is not going to be used. </li>
 *        <li><b><i>timeout_in_seconds</i></b>, outgoing sessions timeout in seconds. <b>Only valid for SM</b>. Use SM_WAIT_FOREVER to wait forever for the complete request/response </li>
 *        </ul></td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 *
 * An example of initiating a device transfer is shown below:
 * @code
 *   static connector_request_data_service_send_t header;
 *   static char file_path[] = "testdir/testfile.txt";
 *   static char file_type[] = "text/plain";
 *
 *   header.transport = connector_transport_tcp;
 *   header.path  = file_path;
 *   header.content_type = file_type;
 *   header.response_required = connector_true;
 *   header.option = connector_data_service_send_option_append;
 *   header.user_context = file_path;
 *
 *   // Begin a file transfer to Device Cloud
 *   status = connector_initiate_action(handle, connector_initiate_send_data, &header);
 * @endcode
 *
 * This example will invoke Cloud Connector to initiate a data transfer to Device
 * Cloud.  The result of this operation creates a file testfile.txt in the testdir directory
 * on Device Cloud.  Once Device Cloud is ready to receive data
 * from the device the application callback is called requesting data.
 *
 * @note The header, file_path and file_type above are not stack variables. Either you can use a heap or a memory (global or static)
 * variable to hold these values. You can release them when you get a @ref connector_request_id_data_service_send_response "response"
 * or @ref connector_request_id_data_service_send_status "status" callback. The value passed as the user_context will be returned
 * in every callback for this session. User is free to update this at any point during the session.
 *
 * @subsection send_data_length_callback Get total length callback
 *
 * For a non-tcp transport session, before making the very first @ref connector_request_id_data_service_send_data "Send Data"
 * callback, Cloud Connector will call the @ref connector_request_id_data_service_send_length "total length" callback to get the total
 * length of the send buffer.
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_send_length @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_data_service_length_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>, a method chosen to send data </li>
 *       <li><b><i>user_context</i></b>, is the user owned context pointer </li>
 *       <li><b><i>total_bytes</i></b>, the total size of the user data to send in bytes </li>
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
 * <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 * <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref connector_callback_busy @htmlonly</th>
 * <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection send_data_callback Send data callback
 *
 * After calling connector_initiate_action(), Cloud Connector will make @ref connector_request_id_data_service_send_data "Send Data"
 * @ref connector_callback_t "callbacks" to retrieve the application data. These callbacks will continue
 * until the user sets more_data flag to connector_false or an error is encountered.
 *
 * The @ref connector_request_id_data_service_send_data "Send Data" @ref connector_callback_t "callback" is called with the following information:
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_send_data @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_data_service_send_data_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>, a method chosen to send data </li>
 *       <li><b><i>user_context</i></b>, is the user owned context pointer </li>
 *       <li><b><i>buffer</i></b> pointer to store user data </li>
 *       <li><b><i>bytes_available</i></b>, the maximum number of bytes the user can copy to the buffer </li>
 *       <li><b><i>bytes_used</i></b>, the number of bytes filled, cannot be more than the bytes_available </li>
 *       <li><b><i>more_data</i></b>, set to connector_true if more data to send, the callback will be called again in that case </li>
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
 * <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 * <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref connector_callback_busy @htmlonly</th>
 * <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection send_data_response_callback Send data response callback
 *
 * Cloud Connector will make @ref connector_request_id_data_service_send_response "response"
 * @ref connector_callback_t "callback" to pass Device Cloud response to the send data request.
 *
 * The @ref connector_request_id_data_service_send_response "response" @ref connector_callback_t "callback"
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_send_response @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_data_service_send_response_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>, a method chosen to send data </li>
 *       <li><b><i>user_context</i></b>, holds the user owned context pointer </li>
 *       <li><b><i>response</i></b> response code returned from Device Cloud </li>
 *       <li><b><i>hint</i></b>, incase of error, Device Connector will return a cause for failure </li>
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
 * <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 * <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref connector_callback_busy @htmlonly</th>
 * <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection send_data_status_callback Send data status callback
 *
 * Cloud Connector will make @ref connector_request_id_data_service_send_status "status"
 * @ref connector_callback_t "callback" to pass the reason for session complete. User will receive this
 * callback when Device Connector finishes with the session. User can free their user_context and any other
 * reserved data as soon as they receive this callback.
 * In case of error, the error from the application level is returned via 
 * @ref connector_request_id_data_service_send_response "response".
 *
 * The @ref connector_request_id_data_service_send_status "status" @ref connector_callback_t "callback"
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_send_status @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td>Pointer to @endhtmlonly connector_data_service_status_t @htmlonly structure:
 *     <ul>
 *       <li><b><i>transport</i></b>, a method chosen to send data </li>
 *       <li><b><i>user_context</i></b>, holds the user owned context pointer </li>
 *       <li><b><i>status</i></b> reason to end the session, returned from Device Connector </li>
 *       <li><b><i>session_error</i></b>, this field will carry the session error code
 *                 when the status is set to connector_data_service_status_session_error </li>
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
 * <th>@endhtmlonly @ref connector_callback_abort @htmlonly</th>
 * <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref connector_callback_busy @htmlonly</th>
 * <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 *
 * @section device_request Device Request
 *
 * Device requests are data transfers initiated by Device Cloud. They are used to
 * send data from Device Cloud to the device, and the device may send back a reply.
 * The user will receive the application callback when a device request is received from Device Cloud.
 * The application callback is continually called with the data from Device Cloud
 * until the transfer is complete or an error is encountered.
 *
 * Cloud Connector invokes the application-defined callback to process and respond a device request.
 *      -# @ref ds_receive_target
 *      -# @ref ds_receive_data
 *      -# @ref ds_receive_reply_data
 *      -# @ref ds_receive_reply_length
 *      -# @ref ds_receive_status
 *
 * The sequence calling an application-defined callback for device request is:
 *      -# Cloud Connector calls application-defined callback @ref ds_receive_target to process the target of the device request. 
 *          Target can be used freely by the user and it's normally used to multiplex the data destination/purpose.
 *      -# Cloud Connector calls application-defined callback @ref ds_receive_data to process the data of the device request
 *      -# Cloud Connector calls application-defined callback @ref ds_receive_reply_length to get the total length in bytes for the response data.
 *         This is only callback when device request is from UDP or SMS transports. See @ref CONNECTOR_TRANSPORT_UDP to enable UDP transport and
 *         @ref CONNECTOR_TRANSPORT_SMS to enable SMS transport.
 *      -# Cloud Connector calls application-defined callback @ref ds_receive_reply_data for response data sent back to Device Cloud.
 *
 * Cloud Connector calls step 2 repeatedly for all device request data and step 4 for all response data.
 *
 * For a non-tcp transport session, before making the very first call to get the user response (only if
 * Device Cloud requested for the response), Cloud Connector will call the
 * @ref ds_receive_reply_length to get the total length of the response data.
 *
 * @note See @ref data_service_support under Configuration to enable the data service.
 * @note See @ref CONNECTOR_TRANSPORT_TCP and @ref network_tcp_start to enable and start TCP.
 * @note See @ref CONNECTOR_TRANSPORT_UDP and @ref network_udp_start to enable and start UDP.
 * @note See @ref CONNECTOR_TRANSPORT_SMS and @ref network_sms_start to enable and start SMS.
 *
 * @subsection ds_receive_target  Device Request Target Callback
 *
 * This callback is called first with the device request target.
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_receive_target @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td> Pointer to @endhtmlonly connector_data_service_receive_target_t @htmlonly structure where:
 *       <dl><dt>transport</dt><dd> - Transport method from where the device request is originated.</dd>
 *           <dt>user_context</dt><dd> - Callback writes its own context which will be passed back to
 *                                       subsequent callback.</dd>
 *           <dt>target</dt><dd> - Contains pointer to requested target string (matching the 'target_name' attribute 
 *                                 of the device request issued on the Cloud). Target can be used freely by the user. 
 *                                 It's normally used to multiplex the data destination/purpose. If the Cloud request 
 *                                 does not have contains 'target_name' attribute, this callback will be called with
 *                                 'target' pointing to NULL.</dd>
 *           <dt>response_required</dt><dd>
 *               <ul><li>@endhtmlonly @ref connector_true @htmlonly if Device Cloud requests a response and
 *                       @endhtmlonly @ref ds_receive_reply_data @htmlonly will be called for response data.</li>
 *                   <li>@endhtmlonly @ref connector_false @htmlonly if Device Cloud does not require a response and
 *                       @endhtmlonly @ref ds_receive_reply_data @htmlonly will not be called.</li></ul>
 *       </dd></dl>
 * </td>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 *   <td>Application encountered error.
 *       <br> @endhtmlonly @ref ds_receive_reply_data @htmlonly will be called
 *       to send error data back to Device Cloud.
 *       <br><b>Note:</b> The status in the response will be set to zero (not handled) if the request is originated from TCP transport.
 *   </td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection ds_receive_data  Device Request Data Callback
 *
 * This callback is called to process device request data. This callback will be called repeatedly until the last chuck of data.
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_receive_data @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td> Pointer to @endhtmlonly connector_data_service_receive_data_t @htmlonly structure where:
 *       <dl><dt>transport</dt><dd> - Transport method from where the device request is originated.</dd>
 *           <dt>user_context</dt><dd> - Pointer to callback's context returned from previous device request callback.
 *                                       Callback may write its own context which will be passed back to
 *                                       subsequent callback.</dd>
 *           <dt>buffer</dt><dd> - Pointer to device request data.</dd>
 *           <dt>bytes_used</dt><dd> - Contains the length of device request data in bytes.</dd>
 *           <dt>more_data</dt><dd> - Contains @endhtmlonly @ref connector_true @htmlonly for more data and this callback
 *                                    will be called again or @endhtmlonly @ref connector_false @htmlonly if this is last chuck
 *                                    of data.
 *       </dd></dl>
 * </td>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 *   <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 *   <td>Application encountered error.
 *       <br>@endhtmlonly @ref ds_receive_reply_data @htmlonly will be called
 *       to send error data back to Device Cloud.
 *       <br><b>Note:</b> The status in the response will be set to zero (not handled) if the request is originated from TCP transport.
 *   </td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection ds_receive_reply_data  Device Request Response Data Callback
 *
 * This callback is called for response or error data to be sent back to Device Cloud.
 * This callback will be called repeatedly until there is no more data.
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_receive_reply_data @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td> Pointer to @endhtmlonly connector_data_service_receive_reply_data_t @htmlonly structure where:
 *       <dl><dt>transport</dt><dd> - Transport method from where the device request is originated.</dd>
 *           <dt>user_context</dt><dd> - Pointer to callback's context returned from previous device request callback.
 *                                       Callback may write its own context which will be passed back to
 *                                       subsequent callback.</dd>
 *           <dt>buffer</dt><dd> - Pointer to memory where callback writes response data to.</dd>
 *           <dt>bytes_available</dt><dd> - Contains number of bytes available in the buffer.</dd>
 *           <dt>bytes_used</dt><dd> - Callback writes number of bytes written onto the buffer.</dd>
 *           <dt>more_data</dt><dd> - Callback writes @endhtmlonly @ref connector_true @htmlonly for more response data and this callback
 *                                    will be called again or @endhtmlonly @ref connector_false @htmlonly if this is last chuck
 *                                    of data.
 *       </dd></dl>
 * </td>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>ds_receive_status
 *   <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 *   <td>@endhtmlonly @ref connector_callback_error @htmlonly</td>
 *   <td>Application encountered error. Cloud Connector will cancel the device request.</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection ds_receive_reply_length  Device Request Response Length Callback
 *
 * This callback is called to get the total number of bytes for the response data before calling @ref ds_receive_reply_data.
 * This is only called when the device request is originated from UDP transport and response is required.
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_receive_reply_length @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td> Pointer to @endhtmlonly connector_data_service_length_t @htmlonly structure where:
 *       <dl><dt>transport</dt><dd> - Transport method from where the device request is originated.</dd>
 *           <dt>user_context</dt><dd> - Pointer to callback's context returned from previous device request callback.
 *                                       Callback may write its own context which will be passed back to
 *                                       subsequent callback.</dd>
 *           <dt>total_bytes</dt><dd> - Callback writes total number of bytes for the response data.
 *       </dd></dl>
 * </td>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_unrecognized @htmlonly</td>
 *   <td>Application doesn't implement this callback</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection ds_receive_status  Device Request Status Callback
 *
 * This callback is called when Cloud Connector is done with the device request,
 * encounters error or receives error from Device Cloud. Application should
 * free any memory associated with the request in this callback.
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
 *   <td>@endhtmlonly @ref connector_class_id_data_service @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_data_service_receive_status @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td> Pointer to @endhtmlonly connector_data_service_status_t @htmlonly structure where:
 *       <dl><dt>transport</dt><dd> - Transport method from where the device request is originated.</dd>
 *           <dt>user_context</dt><dd> - Pointer to callback's context returned from previous device request callback.</dd>
 *           <dt>status</dt><dd> - Contains one of the following status:
 *                              <ul><li><b>connector_data_service_status_complete</b> - Cloud Connector is done with the device
 *                                       request. This is called only when Device Cloud doesn't request a response and
 *                                        @endhtmlonly @ref ds_receive_data @htmlonly will not be called. </li>
 *                                  <li><b>connector_data_service_status_cancel</b> - The device request was canceled by the user.</li>
 *                                  <li><b>connector_data_service_status_timeout</b> - The device request was timed out.</li>
 *                                  <li><b>connector_data_service_status_session_error</b> - Cloud Connector encountered error.
 *                                         See session_error. </li></dd>
 *           <dt>session_error</dt><dd> - Contains @endhtmlonly @ref connector_session_error_t @htmlonly when status is
 *                                        <b>connector_data_service_status_session_error</b>.</li>
 *       </dd></dl>
 * </td>
 * <tr>
 *   <th colspan="2" class="title">Return Values</th>
 * </tr>
 * <tr>
 *   <th class="subtitle">Values</th> <th class="subtitle">Description</th>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 *   <td>Continue</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 *   <td>Aborts Cloud Connector</td>
 * </tr>
 * <tr>
 *   <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * A user uses the SCI web service to send a device request to Device Cloud, which
 * in turn sends it to the device. An example of an application callback for a device
 * request is show below:
 *
 *
 * @section zlib Optional Data Compression Support
 * Cloud Connector has an optional Data Compression switch that reduces the amount of network traffic.  This option requires applications
 * to link with the zlib library and add the zlib header file (zlib.h) to Cloud Connector include path.
 *
 * @note Enabling this option greatly increases the application code size and memory required to execute.
 *
 * If your application requires Data compression, but your development environment does not include the zlib library,
 * you will need to download and build the library.  The zlib home page is located at: http://zlib.net/.   Instructions
 * on how to build zlib are provided with the package.
 *
 * @note The zlib library is required only if your application enables the @ref CONNECTOR_COMPRESSION "Data Compression switch".
 *
 * @htmlinclude terminate.html
 */
