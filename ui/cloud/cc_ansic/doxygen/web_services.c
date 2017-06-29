/*! @page web_services Web Services
 *
 * @htmlinclude nav.html
 *
 * @section services Web Services
 *
 * Applications can communicate to devices through Device Cloud using Web Service
 * requests, which are sent to Device Cloud using the SCI.
 * The diagram below shows how a Web Services Client can send an SCI command to Device Cloud.
 * In this example, the Web Services call will address Device ID 00409dFFFF010202:
 *
 * @image html WebServices.jpg
 *
 * @section SCIservices Supported SCI Operations
 *
 * The Cloud Connector supports a subset of the SCI operations supported by Device Cloud, the
 * supported SCI operations are listed in the table below:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Name</th>
 * <th  class="title">Description</th>
 * </tr>
 * <tr>
 * <td>update_firmware</td>
 * <td>Updates the firmware of the device</td>
 * </tr>
 * <tr>
 * <td>disconnect</td>
 * <td>Sends a request to the device to disconnect from Device Cloud</td>
 * </tr>
 * <tr>
 * <td>query_firmware_targets</td>
 * <td>Gets a list of firmware targets on the device</td>
 * </tr>
 * <tr>
 * <td>data_service</td>
 * <td>Sends messages to devices over the data service</td>
 * </tr>
 * <tr>
 * <td>file_system</td>
 * <td>Used to interact with a file system on a connected Device Cloud device</td>
 * </tr>
 * <tr>
 * <td>send_message</td>
 * <td>Used to send an RCI request pay load (rci_request) to a connected Device Cloud device.</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 *
 * To learn more about Device Cloud Web Services, see chapter SCI (Server Command Interface) in the
 * @htmlonly <a href="http://ftp1.digi.com/support/documentation/html/90002008">Web Services Programming Guide</a>@endhtmlonly.
 *
 * @section Sample Web Services Applications
 *
 * Most sample Applications have associated Web Services client code that demonstrates how
 * a Web Services client can interact with Device Cloud.  These samples are written using
 * Python.
 *
 * @htmlinclude terminate.html
 *
 */
