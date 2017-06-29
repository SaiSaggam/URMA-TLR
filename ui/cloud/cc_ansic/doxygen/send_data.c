/*! @page send_data Send Data Sample
 *
 * @htmlinclude nav.html
 *
 * @section send_data_overview Overview
 *
 * This sample demonstrates how to send data from a device to Device Cloud, using the
 * @ref data_service_overview "data service api" this application writes data to a file
 * on Device Cloud.
 *
 * @section send_data_location Source Code Location
 *      - For the connector_run() version:
 *           - @a /connector/public/run/samples/send_data
 *
 *      - For the connector_step() version:
 *           - @a /connector/public/step/samples/send_data
 *
 * @note Check @ref threading "here for more information" on using the connector_run()
 * versus connector_step() model.
 *
 * @section connect_description Code Flow
 *
 * The routine main() in the platform directory initializes Cloud Connector and registers the
 * application-defined callback. In file application.c the application-defined callback app_connector_callback()
 * can be found, this routine calls app_data_service_handler() when a data service request is received.
 *
 * The routine application_run() (or application_step()) which is called after initialization calls the routine
 * app_send_put_request() to initiate the data transfer to Device Cloud. This routine initiates the data transfer
 * by calling connector_initiate_action() which provides also the Device Cloud file path where data has to be stored. 
 * Once Device Cloud is ready to receive data, the app_data_service_handler()
 * routine is called. The callback then returns the string "Connector data service sample!\n" back to
 * Device Cloud. This data is @b copied @b into @b the @b callback's @b buffer, by setting more_data to
 * connector_false indicates the only message in this data transfer. If you set more_data to connector_true then
 * the callback is called again to get the next chunk of data, this can be repeated to transfer larger data.
 *
 * If app_data_service_handler() is called with @ref connector_request_id_data_service_send_response then this
 * indicates a response from Device Cloud (if transport is set to TCP, this callback will be always called).
 * 
 * When the session is completed, app_data_service_handler() is called with @ref connector_request_id_data_service_send_status. 
 * In that callback the user must check the "status" field of @ref connector_data_service_status_t to determine if the
 * session was completed successfully or not. Also, in this callback all the allocated resources might be freed.
 *
 * @section connect_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/send_data
 * directory and type: @htmlonly"<I>make clean all</I>"@endhtmlonly.
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
 *   <td>samples/send_data</td>
 * </tr>
 * <tr>
 *   <th>put_request.c</th>
 *   <td>data service callback and send_put_request() which starts the data transfer</td>
 *   <td>samples/send_data</td>
 * </tr>
 * <tr>
 *   <th>status.c</th>
 *   <td>Status calls</td>
 *   <td>samples/send_data</td>
 * </tr>
 * <tr>
 *   <th>get_file.py</th>
 *   <td>Python script to retrieve a file</td>
 *   <td>samples/send_data</td>
 * </tr>
 * <tr>
 *   <th>connector_config.h</th>
 *   <td>Cloud Connector options</td>
 *   <td>samples/send_data</td>
 * </tr>
 * <tr>
 *   <th>connector_api.c</th>
 *   <td>Cloud Connector source</td>
 *   <td>private</td>
 * </tr>
 * <tr>
 *   <th>os.c</th>
 *   <td>Operating system calls</td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *   <th>network_tcp.c</th>
 *   <td> Network interface </td>
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
 * To view the results of this sample, log on to Device Cloud as
 * @ref add_your_device_to_the_cloud "described earlier".  Next, click
 * the Data Services tab.  There you will see your device's and your device's
 * data folders.
 *
 * @image html cloud_data_services_folder.png
 *
 * This sample creates 'test/test.txt' file (the path specified in the connector_initiate_action()) in the Device Cloud.  
 * You can view the contents of the file which is located in the particular device's test folder.
 *
 *
 * @section python_script1 Using a Web Services client to retrieve the file from Device Cloud
 *
 * The sample provides a very basic Web Services script, written in python, to retrieve test/test.txt
 * from Device Cloud.
 *
 * Run the python script in the sample:
 *
 * @code
 * python ./get_file.py <Username> <Password> <Device ID> [<Device Cloud URL>]
 * @endcode
 *
 * The script will retrieve test/test.txt and then print the file contents.
 *
 * @htmlinclude terminate.html
 *
 */
