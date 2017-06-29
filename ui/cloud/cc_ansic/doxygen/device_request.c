/*! @page device_request_sample Device Request Sample
 *
 * @htmlinclude nav.html
 *
 * @section device_request_overview Overview
 *
 * This sample demonstrates how to process data sent from Device Cloud.
 * Using the @ref data_service_overview "data service api" this application prints
 * out the data sent from Device Cloud.
 *
 * @section device_request_location Source Code Location
 *      - For the connector_run() version:
 *           - @a /connector/public/run/samples/device_request
 *
 *      - For the connector_step() version:
 *          - @a /connector/public/step/samples/device_request
 *
 * @note Check @ref threading "here for more information" on using the connector_run()
 * versus connector_step() model.
 *
 * @section connect_description Code Flow
 *
 * The routine main() in the platform directory initializes Cloud Connector and
 * registers the application callback. In file application.c the application-defined callback app_connector_callback()
 * can be found, this routine calls app_data_service_handler() when a device request is received.
 *
 * A user can use the included python script to send a device request to the device.
 *
 * When Device Cloud sends a device request to the device, app_data_service_handler() is called
 * with @ref connector_request_id_data_service_receive_target to process the target of the request, and
 * @ref connector_request_id_data_service_receive_data to process the data. This callback
 * is continually called with this @ref connector_request_id_data_service_receive_data callback until
 * the last chunk of data is processed.\n
 * This sample examines the given target and prints out the device request data.
 * It sends an error message if the target name is not "myTarget". 
 * Note that target matches the 'target_name' attribute of the device request issued on the Cloud.
 *
 * After the @ref connector_request_id_data_service_receive_data callback is called for the last chunk of data,
 * @ref connector_request_id_data_service_receive_reply_data callback is called to send response
 * data back to Device Cloud. This sample sends and prints out the request and response data.
 *
 * When @ref connector_request_id_data_service_receive_status callback is called it indicates
 * that the session has been completed. The user must check if an error was encountered or everything went as expected by
 * checking the "status" field of @ref connector_data_service_status_t. Also, all the resources allocated for the 
 * Device Request can be freed in this callback.
 *
 * @section connect_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/device_request
 * directory and type: @htmlonly"<I>make clean all</I>"@endhtmlonly.
 * If you are not using Linux you will need to update your environment with
 * the information below and then build the image for your platform.
 *
 * @subsection source_files Source Files
 *
 * The following is a list of source files required to build this sample:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Name</th>
 * <th class="title">Description</th>
 * <th class="title">Location</th>
 * </tr>
 * <tr>
 * <th>application.c</th>
 * <td>Contains application_run() and the application callback</td>
 * <td>samples/device_request</td>
 * </tr>
 * <tr>
 * <th>device_request.c</th>
 * <td>Routines used to process the data service device requests</td>
 * <td>samples/device_request</td>
 * </tr>
 * <tr>
 * <th>status.c</th>
 * <td>Status calls</td>
 * <td>samples/device_request</td>
 * </tr>
 * <tr>
 * <th>device_request.py</th>
 * <td>Python script to send device request</td>
 * <td>samples/device_request</td>
 * </tr>
 * <tr>
 *   <th>connector_config.h</th>
 *   <td>Cloud Connector options</td>
 *   <td>samples/device_request</td>
 * </tr>
 * <tr>
 * <th>connector_api.c</th>
 * <td>Code for Cloud Connector </td>
 * <td>private</td>
 * </tr>
 * <tr>
 * <th>os.c</th>
 * <td>Operating system calls</td>
 * <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 * <th>network_tcp.c</th>
 * <td> Network interface for TCP</td>
 * <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 * <th>config.c</th>
 * <td>Configuration routines</td>
 * <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 * <th>main.c</th>
 * <td>Starting point of program, dependent on build environment</td>
 * <td>platforms/<i>my_platform</i></td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection include_paths Include Paths
 *
 * The following include paths are required:
 *
 * @li public/include
 * @li public/include/custom
 * @li run/platforms/@a my_platform
 *
 * @section python_script2 Using a Web Services client to send a device request
 *
 * The sample provides a very basic Web Services script, written in python, to send a
 * data service device request to login.etherios.com using @htmlonly <a href="web_services.html">Web Services.</a> @endhtmlonly
 * To run the python script:
 *
 * @code
 * python device_request.py <Username> <Password> <Device ID> <Target> <Request Data> [<Device Cloud URL>]
 * @endcode
 *
 * Output from this sample:
 *
 * @code
 * Device request data: received data = "My device request data" for target = "myTarget"
 * Device response data: send response data = My device response data
 * @endcode

 * Output from the device_request.py:
 *
 * @code
 *
 * Response:
 * <sci_reply version="1.0">
 *    <data_service>
 *        <device id="00000000-00000000-00000000-00000000">
 *            <requests>
 *               <device_request target_name="myTarget" status="0">
 *               My device response data
 *               </device_request>
 *            </requests>
 *        </device>
 *    </data_service>
 * </sci_reply>
 * @endcode
 *
 * @htmlinclude terminate.html
 */

