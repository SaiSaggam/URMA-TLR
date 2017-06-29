/*! @page connect_on_ssl Connect over SSL
 *
 * @htmlinclude nav.html
 *
 * @section ssl_overview Overview
 *
 * This sample demonstrates how one can connect to Device Cloud securely. The sample includes
 * network_tcp_ssl.c, instead of network_tcp.c, to provide network specific user callbacks. That means
 * any application can run on ssl by including network_tcp_ssl.c.
 *
 * @section connect_on_ssl_location Source Code Location
 *      - For the connector_run() version:
 *           - @a /connector/public/run/samples/connect_on_ssl
 *
 *      - For the connector_step() version:
 *           - @a /connector/public/step/samples/connect_on_ssl
 *
 * @note Check @ref threading "here for more information" on using the connector_run()
 * versus connector_step() model.
 *
 * @section ssl_connect_description Code Flow
 *
 * -# The main(), defined in main.c, calls application_start()
 * -# The application_start(), defined in application.c, registers app_connector_callback()
 * -# The app_connector_callback(), defined in application.c, calls app_network_tcp_handler() for all network related Cloud Connector requests
 * -# The app_network_tcp_handler(), defined in network_tcp_ssl.c, calls subsequent openssl functions to communicate
 *    with Device Cloud over SSL
 *
 * @section connect_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/connect_on_ssl
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
 *   <th class="title">Name</th>
 *   <th class="title">Description</th>
 *   <th class="title">Location</th>
 * </tr>
 * <tr>
 *   <td>application.c</td>
 *   <td>Contains application_run() and the application callback</td>
 *   <td>samples/connect_on_ssl</td>
 * </tr>
 * <tr>
 *   <td>status.c</td>
 *   <td>Status calls</td>
 *   <td>samples/connect_on_ssl</td>
 * </tr>
 * <tr>
 *   <td>connector_api.c</td>
 *   <td>Code for the ICC </td>
 *   <td>private</td>
 * </tr>
 * <tr>
 *   <td>os.c</td>
 *   <td>Operating system calls</td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *   <td>network_tcp_ssl.c</td>
 *   <td> Network interface for TCP</td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *   <td>config.c</td>
 *   <td>Configuration routines</td>
 *   <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *   <td>main.c</td>
 *   <td>Starting point of program, dependent on build environment</td>
 *   <td>platforms/<i>my_platform</i></td>
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
 * <b> How to get CA certificate: </b>
 *
 * -# The Device Cloud CA Certificate is located in ./public/certificates
 *
 * @note To convert other samples to use SSL, change the makefile to replace
 * network_tcp.c with network_tcp_ssl.c and include the libssl.a library.
 *
 * @htmlinclude terminate.html
 *
 */
