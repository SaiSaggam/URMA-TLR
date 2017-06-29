/*! @page rci_advance_sample Remote Configuration Sample
 *
 * @htmlinclude nav.html
 *
 * @section rci_advance_sample_overview Overview
 *
 * This sample demonstrates how to retrieve and set device configurations using @ref rci_service "remote configuration callbacks".
 * This sample does not write on the device but demonstrates the remote configuration callback.
 *
* @section rci_sample_location Source Code Location
 *      - For the connector_run() version:
 *           - @a /connector/public/run/samples/remote_config
 *
 *      - For the connector_step() version:
 *           - @a /connector/public/step/samples/remote_config
 *
 * @note Check @ref threading "here for more information" on using the connector_run()
 * versus connector_step() model.
 *
 * @section connect_description Code Flow
 *
 * The routine main() in the platform directory initializes Cloud Connector and
 * registers the application callback.  In the file application.c is the
 * application-defined callback app_connector_callback(), this routine calls app_remote_config_handler()
 * when a remote command interface (RCI) request is received.
 *
 * A User can use the included python script to send a device request to the device.
 *
 * This sample defines a configuration group with the following configurations:
 * - Serial
 *   - Baud rate
 *   - Parity
 *   - Databits
 *   - xbreak
 *   - TX Bytes
 * - Ethernet
 *   - IP Address
 *   - Subnet mask
 *   - Gateway
 *   - DHCP
 *   - DNS
 *   - Duplex
 * - Device Time
 *   - Current time
 * - Device Info
 *   - Version
 *   - Product
 *   - Model
 *   - Company
 *   - Description
 * - System
 *   - Description
 *   - Contact
 *   - Location
 * - Device security
 *   - Connection security
 *   - Password
 * - Device State
 *   - System up time
 *   - Signed value
 * - GPS
 *   - Latitude
 *   - Longitude
 *
 * @subsection config_file Cloud Connector configuration file
 *
 * The following is Cloud Connector configuration file:
 *
 * @code
 * globalerror load_fail "Load fail"
 * globalerror save_fail "Save fail"
 * globalerror memory_fail "Insufficient memory"
 *
 * group setting serial 2 "Serial Port" "Port 1 is used for printf"
 *     element baud "Baud rate" type enum access read_write
 *         value 2400
 *         value 4800
 *         value 9600
 *         value 19200
 *         value 38400
 *         value 57600
 *         value 115200
 *         value 230400
 *     element parity "Parity" type enum access  read_write
 *         value none
 *         value odd
 *         value even
 *     element databits "Data bits" type uint32 access  read_write  min  5 max  8
 *     element xbreak "Tx Break" type on_off access  read_write
 *     element txbytes "Tx bytes" type uint32 access  read_only
 *     error invalid_baud "Invalid baud rate "
 *     error invalid_databits "Invalid data bits"
 *     error invalid_parity  " Invalid parity"
 *     error invalid_xbreak "Invalid xbreak setting"
 *     error invalid_databits_parity "Invalid combination of data bits and parity"
 *
 * group setting ethernet "Ethernet interface"
 *     element ip "IP Address" type ipv4
 *     element subnet "Subnet" type ipv4
 *     element gateway "Gateway" type ipv4
 *     element dhcp "DHCP" type boolean
 *     element dns "DNS" type fqdnv4 max 127
 *     element duplex "Duplex" type enum
 *         value auto  "Automatic"
 *         value half  "Half"
 *         value full  "Full"
 *     error invalid_duplex "Invalid ethernet duplex setting"
 *     error invalid_ip "Invalid IP address"
 *     error invalid_subnet "Invalid subnet mask"
 *     error invalid_gateway "Invalid gateway address"
 *     error invalid_dns "Invalid DNS address"
 *
 *
 * group setting device_time  "Device Time"
 *     element curtime "Current time" type datetime access  read_write
 *     error invalid_time "Invalid time"
 *
 * group setting device_info  "Device info"
 *     element version "Version" type 0x_hex32 access  read_only
 *     element product "Product" type string access  read_write  min  1 max  63
 *     element model "Model" type string access  read_write  min  0 max  31
 *     element company "Company" type string access  read_write  max 63
 *     element desc "Description" type multiline_string access  read_write max 127
 *
 * # Device info that shows up in Device Cloud device summary
 * # Device Cloud queries this system setting to display the information in root folder
 * group setting system "System"
 *     element description "Description" type string max 63
 *     element contact "Contact" type string max 63
 *     element location "Location" type string max 63
 *
 * # EDP password
 * # Device Cloud uses this devicesecurity setting for password authentication
 * # Note, as with all password, password type is a write-only field
 * group setting devicesecurity "Device Cloud device security"
 *     element identityVerificationForm "Connection security" type enum
 *         value simple "No connection security"
 *         value password "Connection is password authenticated"
 *     element password "Device connection password" type password max 133 access write_only
 *
 * group state device_state  "Device State"
 *     element system_up_time "System Up time" type uint32 access read_only units "seconds"
 *     element signed_integer "Signed integer" type int32 min -100 max 100
 *     error invalid_integer "Invalid integer value"
 *
 * # Device location
 * # State configuration for GPS
 * # Must setup the following group for Latitude and Longitude to be shown in Device Cloud.
 * group state gps_stats "GPS"
 *     element latitude "Latitude" type string access read_only
 *     element longitude "Longitude" type string access read_only
 * @endcode
 *
 * Run @ref rci_tool to generate @b connector_api_remote.h and @b remote_config.c files:
 * @code
 *
 *   java -jar /connector/tools/ConfigGenerator.jar username:password -url=login.etherios.com "Linux Application" 1.0.0.0 config.rci
 *
 * @endcode
 * @note To run the above line you need to have Java Runtime Environment installed in your system. If you
 * are not sure or do not know how to install it properly, please follow the instructions in 
 * @htmlonly <a href=http://www.java.com/en/download/help/download_options.xml>this link</a>@endhtmlonly
 * 
 * When Device Cloud sends a RCI request to the device, app_remote_config_handler() is called with the following request ID:
 * -# @ref connector_request_id_remote_config_session_start
 * -# @ref connector_request_id_remote_config_action_start
 * -# @ref connector_request_id_remote_config_group_start
 * -# @ref connector_request_id_remote_config_group_process
 * -# @ref connector_request_id_remote_config_group_end
 * -# @ref connector_request_id_remote_config_action_end
 * -# @ref connector_request_id_remote_config_session_end
 *
 * The callback with @ref connector_request_id_remote_config_group_process request ID is continually called
 * until all elements in the configuration group is processed.
 *
 * When the callback is called with @ref connector_request_id_remote_config_session_cancel request ID it indicates
 * an error is encountered and needs to cancel the RCI. In this example it cancels
 * the request by freeing any memory used.
 *
 *
 * @section connect_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/remote_config
 * directory and run @ref rci_tool to generate the missing files @b connector_api_remote.h and @b remote_config.c. The typical
 * arguments are:
 * @code
 * java -jar ConfigGenerator.jar username:password -url=login.etherios.com "Linux Application" 1.0.0.0 config.rci
 * @endcode
 * Then type:
 * @code
 * make clean all
 * @endcode
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
 * <th>config.rci</th>
 * <td>Remote Configuration file</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>application.c</th>
 * <td>Contains application_run() and the application-defined callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>connector_api_remote.h</th>
 * <td>Remote configuration API definition generated by the @endhtmlonly @ref rci_tool @htmlonly </td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>remote_config.c</th>
 * <td>Remote configuration structure generated by the @endhtmlonly @ref rci_tool @htmlonly, it must be passed to @endhtmlonly @ref rci_descriptor_data @htmlonly callback </td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>remote_config_cb.c</th>
 * <td>Routines used to remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>remote_config_cb.h</th>
 * <td>Header for remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>device_info.c</th>
 * <td>Device info remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>devicesecurity.c</th>
 * <td>Device security remote configuration callback (optional)</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>device_state.c</th>
 * <td>Device state remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>device_time.c</th>
 * <td>Device time remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>ethernet.c</th>
 * <td>Ethernet remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>firmware.c</th>
 * <td>Firmware calls</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>gps_stats.c</th>
 * <td>GPS remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>serial.c</th>
 * <td>Serial remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>status.c</th>
 * <td>Status calls</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <tr>
 * <th>system.c</th>
 * <td>System remote configuration callback</td>
 * <td>samples/remote_config</td>
 * </tr>
 * <th>connector_config.h</th>
 * <td>Cloud Connector options</td>
 * <td>samples/remote_config</td>
 * </tr>
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
 * <tr>
 * <th>network_tcp.c</th>
 * <td> Network interface </td>
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
 * @li public/run/samples/remote_config
 * @li public/include
 * @li public/include/custom
 * @li run/platforms/@a my_platform
 *
 * @section running Running
 *
 * Once successfully built, run the executable, in Linux type ./connector to run the
 * sample.
 *
 * Log on to @htmlonly <a href="http://login.etherios.com/">Device Cloud</a>@endhtmlonly
 * (described in the @ref connector_login "Getting Started Section").
 * Once you are logged, go to Device Management tab and click the Refresh button.
 * The device's status should show a 'green' light indicating the device is connected.
 *
 * @image html cloud_device_connected.png
 *
 * Then double click the device. Click the Refresh button to update the configurations.
 *
 * @image html cloud_rci_refresh.png
 *
 * Click the System item under the Configuration link. Click the Save button
 * after updating the configurations.
 *
 * @image html cloud_rci_advance_save.png
 *
 * Go back to Home page and click the Refresh button, it will show the new configuration data.
 *
 * @image html cloud_rci_advance_home.png
 *
 * @htmlinclude terminate.html
 *
 */
