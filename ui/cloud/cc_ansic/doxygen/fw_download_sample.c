/*! @page fw_download_sample Firmware Download Sample
 *
 * @htmlinclude nav.html
 *
 * @section fw_download_sample_overview Overview
 *
 * This sample demonstrates how to download firmware to your device using the
 * @ref fw_overview1 "firmware download API".  This is a generic sample which
 * does not update the firmware on the device but demonstrates how to use this
 * API.
 *
 * @section firmware_download_location Source Code Location
 *      - For the connector_run() version:
 *           - @a /connector/public/run/samples/firmware_download
 *
 *      - For the connector_step() version:
 *           - @a /connector/public/step/samples/firmware_download
 *
 * @note Check @ref threading "here for more information" on using the connector_run()
 * versus connector_step() model.
 *
 * @section connect_description Code Flow
 *
 * The @ref AppStructure "Public Application Framework" @b Sample section file application.c contains
 * @ref connector_callback_t "application-defined callback app_connector_callback()", which traps the
 * @ref fw_overview1 "Firmware Download" callbacks.  These callbacks are then processed in
 * app_firmware_handler() when a firmware request callback is received.
 *
 * All interaction with Cloud Connector is through the callback in this sample; Firmware download
 * is initiated fully from Device Cloud.  The file firmware.c contains the routines which process
 * the firmware download callbacks passed to app_firmware_handler().
 *
 * This sample defines all supported image files with .a and .bin file extensions.
 * The file extension is only validated when updating firmware from Device Cloud.
 *
 * @code
 * // list of all supported firmware target info
 * static firmware_list_t firmware_list[] = {
 *   // version        name_spec            description
 *   {  {1, 0, 0, 0}, ".*\\.[bB][iI][nN]", "Binary Image" }  // any .bin image
 *   {  {0, 0, 0, 0}, ".*\\.a",            "Library Image"}, // any .a image
 * };
 * @endcode
 *
 * The routine app_firmware_download_request() is called when the download is first
 * initiated.  It receives information about the download and returns @ref connector_firmware_status_success
 * status indicating that is ready to accept the FW download.
 *
 * The app_firmware_image_data() routine is repeatedly called with the image data
 * until the download is complete, the routine app_firmware_download_complete()
 * is called when the download is complete.  At this point the FW can verify
 * that the downloaded image is valid.
 *
 * When app_firmware_download_complete() returns @ref connector_firmware_download_success status,
 * it indicates the firmware image has been updated. Device Cloud will reset the unit,
 * the routine app_firmware_reset() is called for resetting.
 *
 * The routine app_firmware_download_abort() is called when Device Cloud encounters error.
 *
 * @section connect_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/firmware_download
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
 * <td>Contains application_run() and the application-defined callback</td>
 * <td>samples/firmware_download</td>
 * </tr>
 * <tr>
 * <th>firmware.c</th>
 * <td>Routines used to process the FW download requests</td>
 * <td>samples/firmware_download</td>
 * </tr>
 * <tr>
 * <th>status.c</th>
 * <td>Status calls</td>
 * <td>samples/firmware_download</td>
 * </tr>
 * <tr>
 * <th>update_firmware.py</th>
 * <td>Python script for firmware download</td>
 * <td>samples/firmware_download</td>
 * </tr>
 * <tr>
 * <th>query_firmware.py</th>
 * <td>Python script for query supported firmware information</td>
 * <td>samples/firmware_download</td>
 * </tr>
 * <tr>
 *   <th>connector_config.h</th>
 *   <td> Cloud Connector options</td>
 *   <td>samples/firmware_download</td>
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
 *
 * @section running Running
 *
 * Once successfully built, run the executable, in Linux type ./connector to run the
 * sample.
 *
 * Firmware downloads can be initiated by Device Cloud using <a href="http://www.etherios.com/devicecloud/devicemanager">Device Management</a>
 * or by issuing a Web Services call.  See @ref services "here" for more information on
 * Web Service calls.
 *
 * @subsection fw_gui Firmware Download using Device Management
 *
 * Log on to Device Cloud at http://login.etherios.com/ and click on your device
 * (described in the @ref connector_login "Getting Started Section").
 * Once you are logged to see your device, click the Refresh button.
 * The device's status should indicate as 'Connected'. Select the device for firmware update
 *
 * @image html cloud_device_connected.png
 *
 * Then select Update Firmware to update your firmware under the More button, and then select
 * the file that is your firmware image on your local machine.  The image will then
 * start to upload to your Cloud Connector application.  The GUI will indicate when it's complete.
 *
 * @note The firmware image name must match the regular expression given to
 * filter names, see @ref fw_info.
 *
 * @image html cloud_firmware_download.png
 *
 * The results in this application are displayed in the output console of the
 * application.  Since this is only a sample the firmware is not updated on the
 * device.  The application prints out the target, code size and checksum of the
 * downloaded image.
 *
 * @subsection fw_python Using a Web Services client to query a device for target information and perform a Firmware Download
 *
 * The sample provides two Web Services scripts, written in python, to simulate a
 * firmware download and to query a device for firmware target information using
 * @htmlonly <a href="web_services.html">Web Services.</a> @endhtmlonly
 *
 *
 * To perform a Web Services "Download firmware" operation, execute the update_firmware.py python script:
 *
 * @code
 * python update_firmware.py <Username> <Password> <Device ID> [<Device Cloud URL>]
 * @endcode
 *
 * Example output from update_firmware.py:
 *
 * @code
 *
 * Response:
 * <sci_reply version="1.0">
 *     <update_firmware>
 *         <device id="00000000-00000000-00000000-00000000">
 *         <submitted/>
 *         </device>
 *     </update_firmware>
 * </sci_reply>
 *
 * @endcode
 *
 * To perform a Web Services query for a device's firmware target information, execute
 * the query_firmware.py python script:
 *
 * @code
 * python query_firmware.py <Username> <Password> <Device ID> [<Device Cloud URL>]
 * @endcode
 *
 * An output of query_firmware.py:
 * @code
 *
 * Response:
 * <sci_reply version="1.0">
 *    <query_firmware_targets>
 *    <device id="00000000-00000000-00000000-00000000">
 *       <targets>
 *           <target number="0">
 *               <name>Library Image</name>
 *               <pattern>.*\.a</pattern>
 *               <version>1.0.0.0</version>
 *               <code_size>4294967295</code_size>
 *           </target>
 *           <target number="1">
 *               <name>Binary Image</name>
 *               <pattern>.*\.[bB][iI][nN]</pattern>
 *               <version>0.0.1.0</version>
 *               <code_size>4294967295</code_size>
 *           </target>
 *         </targets>
 *       </device>
 *   </query_firmware_targets>
 * </sci_reply>
 *
 * @endcode
 *
 *
 * @htmlinclude terminate.html
 *
 */
