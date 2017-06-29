/*! @page status_callbacks Status Callbacks
 *
 * @htmlinclude nav.html
 *
 * @section status_callbacks_overview Status Callbacks Overview
 *
 * The status callbacks APIs are used to send and receive status messages to and from Device Cloud and to notify
 * application when TCP communication is established, keep alive message is not received from Device Cloud, keep alive
 * message is restored and received from Device Cloud, and Cloud Connector stops running.
 *
 * The following requests under @ref connector_class_id_status are listed below:
 *
 * @li @ref status_tcp
 * - This callback is called to notify the application that TCP communication has been established,
 * a keep-alive message was not received, or keep-alive message was received and recovered.
 *
 * @li @ref status_stop_completed
 * - This callback is called to notify the application that Cloud Connector has stopped a transport running.
 *
 * @section status_tcp TCP Status
 *
 * This callback is called to notify application the status of TCP communication.
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
 *   <td>@endhtmlonly @ref connector_class_id_status @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_status_tcp @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>data</td>
 *   <td> Pointer to @endhtmlonly connector_status_tcp_event_t @htmlonly:
 *        <dl> <dt>status</dt>
 *             <dd><dl>
 *             <dt><b>@endhtmlonly @ref connector_tcp_communication_started @htmlonly </b> </dt>
 *             <dd> - TCP communication has been established. User and application is able to start message and data transfer. </dd>
 *             <dt><b>@endhtmlonly @ref connector_tcp_keepalive_missed @htmlonly </b>
 *             <dd> - Cloud Connector has not received a keep alive message from Device Cloud
 *                    within the time specified in @endhtmlonly @ref connector_request_id_config_tx_keepalive callback or
 *                    @ref CONNECTOR_TX_KEEPALIVE_IN_SECONDS. See @ref tx_keepalive @htmlonly </dd>
 *             <dt><b>@endhtmlonly @ref connector_tcp_keepalive_restored @htmlonly </b>
 *             <dd> - Cloud Connector has received and recovered missed keep alive message from Device Cloud. </dd>
 *             </dl></dd>
 *        </dl></td>
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
 * </table>
 * @endhtmlonly
 *
 * @section status_stop_completed Stop Completion
 *
 * This callback is called when Cloud Connector has stopped running. It's invoked from connector_initiate_action() call on @ref connector_initiate_transport_stop.
 * When connector_initiate_action() is called, this callback will be called after the specified transport has stopped running.
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
 *   <td>@endhtmlonly @ref connector_class_id_status @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_id</td>
 *   <td>@endhtmlonly @ref connector_request_id_status_stop_completed @htmlonly</td>
 * </tr>
 * <tr>
 *   <td>request_data</td>
 *   <td> Pointer to @endhtmlonly connector_initiate_stop_request_t @htmlonly structure, where member:
 *        <ul>
 *        <li><b><i>transport</i></b>
 *        <ul><li> @endhtmlonly @ref connector_transport_tcp @htmlonly </li>
 *            <li> @endhtmlonly @ref connector_transport_udp @htmlonly </li>
 *            <li> @endhtmlonly @ref connector_transport_all @htmlonly </li> </ul>
 *        </li>
 *        <li><b><i>condition</i></b> Not used
 *        </li>
 *        <li><b><i>user_context</i></b> User's context from @endhtmlonly connector_initiate_action() @htmlonly call.</li>
 *        </ul></td>
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
 *   <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 *   <td>Busy and needs to be called back again</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 *
 * @htmlinclude terminate.html
 */
