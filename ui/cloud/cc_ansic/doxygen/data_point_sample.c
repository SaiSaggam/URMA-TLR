/*! @page data_point_sample Data Point Sample
 *
 * @htmlinclude nav.html
 *
 * @section data_point_sample_overview Overview
 *
 * This sample demonstrates how to send streaming data points from a device to Device Cloud.
 * This application sends data points periodically to three different data streams: 'cpu_usage, 'cpu_temp' and 'incremental'.
 * Device Cloud (with Data Stream enabled) on the other hand collects these data and the data shows up under Data Streams
 * page inside the Data Service tab.
 *
 * To learn more about Device Cloud Data Streams, see Device Cloud Data Streams section in the
 * @htmlonly<a href="http://ftp1.digi.com/support/documentation/html/90001150/">Device Cloud User's Guide</a>@endhtmlonly.
 *
 * @section data_point_location Source Code Location
 *      - For the connector_run() version:
 *           - @a /connector/public/run/samples/data_point
 *
 *      - For the connector_step() version:
 *           - @a /connector/public/step/samples/data_point
 *
 * @note Check @ref threading "here for more information" on using the connector_run()
 * versus connector_step() model.
 *
 * @section connect_description Code Flow
 *
 * The routine main() in the platform directory initializes Cloud Connector and
 * registers the application-defined callback. In the file application.c is the
 * application-defined callback app_connector_callback(), this routine calls app_data_point_handler()
 * when a data service request is received.
 *
 * The routine application_run() (or application_step()) allocates and configures three data streams: "cpu_usage",
 * "cpu_temp" and "incremental". Then, once every 2 seconds, it calls app_update_point() to
 * update one data point per data stream (in this sample: "cpu_usage", "cpu_temp" and "incremental").
 * When 5 data points are collected for each stream (it takes 10 seconds), the application calls app_send_data()
 * which calls connector_initiate_action() to initiate the actual transfer of the data points. The sample
 * waits for the response before sending the next set of points.
 *
 * If the app_data_point_handler() is called with @ref connector_request_id_data_point_response
 * then this indicates a response from Device Cloud is received (this callback will only be called if response
 * was requested). The @ref connector_request_id_data_point_status is allways issued indicating the
 * status of the request after it's been completed. This sample uses linux, /proc/stat file
 * to calculate the "CPU usage"; the "CPU temperature" is just a random float number and "incremental" is an
 * incremental integer.
 *
 * @section connect_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/data_point
 * directory and type: @htmlonly"<I>make</I>"@endhtmlonly.
 * If you are not using Linux you will need to update your environment with
 * the information below and then build the image for your platform.
 *
 * @subsection Source Files
 *
 * The following is a list of source files required to build this sample:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 *   <th class="title">Name</th>
 *   <th class="title">Description</th>
 *   <th class="title">Location</th>
 * </tr>
 * <tr>
 *   <th>application.c</th>
 *   <td>Contains application_run() and the application callback</td>
 *   <td>samples/data_point</td>
 * </tr>
 * <tr>
 *   <th>data_point.c</th>
 *   <td>data point callback, app_allocate_data, app_setup_stream, app_update_point and app_send_data() which starts the data transfer</td>
 *   <td>samples/data_point</td>
 * </tr>
 * <tr>
 *   <th>connector_config.h</th>
 *   <td>Cloud Connector options</td>
 *   <td>samples/data_point</td>
 * </tr>
 * <tr>
 *   <th>connector_api.c</th>
 *   <td>Code for Cloud Connector </td>
 *   <td>private</td>
 * </tr>
 * <tr>
 *   <th>os.c</th>
 *   <td>Operating system calls</td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *   <th>network_tcp.c</th>
 *   <td> Network interface for TCP </td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *   <th>config.c</th>
 *   <td>Configuration routines</td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *   <th>main.c</th>
 *   <td>Starting point of program, dependent on build environment</td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection Include Paths
 *
 * The following include paths are required:
 *
 * @li public/include
 * @li public/include/custom
 * @li run/platforms/@a my_platform
 *
 * @section web_gui Viewing results from Device Cloud
 *
 * To view the results for this sample, log on to Device Cloud as described
 * in the @ref add_your_device_to_the_cloud "Getting Started" section.
 *
 * Once logged in click on the Data Services tab as shown below. Then select the Data
 * Streams tab. This should show your streams cpu_usage, cpu_temp and incremental.
 * Select each stream to see the Data Stream charts and raw data.
 *
 * @image html cloud_data_stream_selection.png
 *
 * @htmlinclude terminate.html
 *
 */
