/*! @page config_callbacks Configuration Callbacks
 *
 * @htmlinclude nav.html
 *
 * @section config Configuration Callbacks
 * Cloud Connector interfaces to the platform's configuration through the callbacks listed below.  These
 * are called through the application callback described in the @ref api1_overview.
 *
 *  -# @ref device_id
 *  -# @ref get_device_id
 *  -# @ref set_device_id
 *  -# @ref vendor_id
 *  -# @ref device_type
 *  -# @ref device_cloud_url
 *  -# @ref connection_type
 *  -# @ref mac_address
 *  -# @ref link_speed
 *  -# @ref phone_number
 *  -# @ref tx_keepalive
 *  -# @ref rx_keepalive
 *  -# @ref wait_count
 *  -# @ref ip_address
 *  -# @ref error_status
 *  -# @ref firmware_support
 *  -# @ref data_service_support
 *  -# @ref file_system_support
 *  -# @ref rci_support
 *  -# @ref max_msg_transactions
 *  -# @ref network_tcp_start
 *  -# @ref network_udp_start
 *  -# @ref network_sms_start
 *  -# @ref identity_verification
 *  -# @ref password
 *  -# @ref get_device_cloud_phone
 *  -# @ref set_device_cloud_phone
 *  -# @ref device_cloud_service_id
 *  -# @ref sm_udp_max_sessions
 *  -# @ref sm_sms_max_sessions
 *  -# @ref sm_udp_max_rx_segments
 *  -# @ref sm_sms_max_rx_segments
 *  -# @ref sm_udp_rx_timeout
 *  -# @ref sm_sms_rx_timeout
 *  -# @ref rci_descriptor_data
 *
 * @section device_id Obtaining the Device ID
 * 
 * Device IDs are a globally unique 16-octet value identifier for Device Cloud clients.
 * These identifiers can be derived from the following sources:
 * 
 * There are three valid sources to generate the Device ID:
 *      - Device's MAC address
 *      - Device's modem's IMEI
 *      - Device's MEID
 *      - Device's ESN
 *      - When any of the previous sources are available, Cloud Connector can ask Device Cloud to assign the Device
 * a unique randomly generated Device ID for it.
 * 
 * This section provides some reference functions to convert MACs, IMEIs, MEIDs and ESNs values into valid Device IDs. These functions assume 
 * that the origin values are stored in a byte array in ascending order. However, some functions to translate an hexadecimal string to a binary buffer are provided at the end of this appendix.
 * As a reminder, Cloud Connector expects the Device ID to be stored in a 16-bytes array of unsigned 8-bit integers.
 * 
 * The Device IDs differ when generated from the different sources:
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">MAC Address</th> </tr>
 * <tr><th class="subtitle">Example</th> <th class="subtitle">Device ID mapping</th></tr>
 * <tr>
 * <th>11:22:33:44:55:66</th>
 * <td>00<b>00</b>0000-00000000-112233FF-FF445566</td>
 * </tr>
 * <tr> <th colspan="2" class="title">GSM IMEI</th> </tr>
 * <tr><th class="subtitle">Example</th> <th class="subtitle">Device ID mapping</th></tr>
 * <tr>
 * <th>AA-BBBBBB-CCCCCC-D</th>
 * <td>00<b>01</b>0000-00000000-0AABBBBB-BCCCCCCD</td>
 * </tr>
 * <tr> <th colspan="2" class="title">ESN</th> </tr>
 * <tr><th class="subtitle">Example</th> <th class="subtitle">Device ID mapping</th></tr>
 * <tr>
 * <th>MM-SSSSSS</th>
 * <td>00<b>02</b>0000-00000000-00000000-MMSSSSSS</td>
 * </tr>
 * <tr> <th colspan="2" class="title">MEID</th> </tr>
 * <tr><th class="subtitle">Example</th> <th class="subtitle">Device ID mapping</th></tr>
 * <tr>
 * <th>RR-XXXXXX-ZZZZZZ-C</th>
 * <td>00<b>04</b>0000-00000000-00RRXXXX-XXZZZZZZ</td>
 * </tr>
 * <tr> <th colspan="2" class="title">Auto generated</th> </tr>
 * <tr><th class="subtitle">Example</th> <th class="subtitle">Reference</th></tr>
 * <tr>
 * <th>00<b>08</b>cccc-eeeeeeee-vvvvvvvv-gggggggg</th>
 * <td><ul>
 *     <li><b>cccc</b>: a unique value set per cluster, so it will be different depending on which cluster the id was generated</li>
 *     <li><b>eeeeeeee</b>: is typically all zeroes, but may be randomly assigned.</li>
 *     <li><b>vvvvvvvv</b>: represents a "provision id" for the customer, currently the Vendor ID.</li>
 *     <li><b>gggggggg</b>: randomly assigned.</li>
 *     </ul>
 * </td>
 * </tr>
 * </table>
 * @endhtmlonly
 * 
 * @subsection device_id_from_mac Device ID generation from device's MAC address
 * 
 * @code
 * #define DEVICE_ID_LENGTH                16
 * #define MAC_ADDR_LENGTH                 6
 * 
 * 
 * void app_get_device_id_from_mac(uint8_t * const device_id, uint8_t const * const mac_addr)
 * {
 *     memset(device_id, 0x00, DEVICE_ID_LENGTH);
 *     device_id[8] = mac_addr[0];
 *     device_id[9] = mac_addr[1];
 *     device_id[10] = mac_addr[2];
 *     device_id[11] = 0xFF;
 *     device_id[12] = 0xFF;
 *     device_id[13] = mac_addr[3];
 *     device_id[14] = mac_addr[4];
 *     device_id[15] = mac_addr[5];
 * }
 * @endcode 
 * 
 * @subsection device_id_from_imei Device ID generation from device's IMEI
 * @code
 * #define DEVICE_ID_LENGTH                16
 * #define IMEI_LENGTH                     8
 * #define DEVICE_ID_FROM_IMEI_PREFIX      1
 * 
 * 
 * void app_get_device_id_from_imei(uint8_t * const device_id, uint8_t const * const imei)
 * {
 *     memset(device_id, 0x00, DEVICE_ID_LENGTH);
 *     device_id[1] = 0x01;
 *     
 *     device_id[8] = imei[0];
 *     device_id[9] = imei[1];
 *     device_id[10] = imei[2];
 *     device_id[11] = imei[3];
 *     device_id[12] = imei[4];
 *     device_id[13] = imei[5];
 *     device_id[14] = imei[6];
 *     device_id[15] = imei[7];
 * }
 * @endcode
 * 
 * @subsection device_id_from_meid Device ID generation from device's MEID
 * 
 * @code
 * #define DEVICE_ID_LENGTH                16
 * #define MEID_LENGTH                     8
 * #define DEVICE_ID_FROM_MEID_PREFIX      4
 * 
 * 
 * void app_get_device_id_from_meid(uint8_t * const device_id, uint8_t const * const meid)
 * {
 *     memset(device_id, 0x00, DEVICE_ID_LENGTH);
 *     device_id[1] = 0x04;
 *     
 *     device_id[8] = meid[0];
 *     device_id[9] = meid[1];
 *     device_id[10] = meid[2];
 *     device_id[11] = meid[3];
 *     device_id[12] = meid[4];
 *     device_id[13] = meid[5];
 *     device_id[14] = meid[6];
 *     device_id[15] = meid[7];
 * }
 * @endcode
 * 
 * @subsection device_id_from_meid Device ID generation from device's ESN
 * 
 * @code
 * #define DEVICE_ID_LENGTH                16
 * #define ESN_LENGTH                      4
 * #define DEVICE_ID_FROM_ESN_PREFIX       2
 * 
 * void app_get_device_id_from_esn(uint8_t * const device_id, uint8_t const * const esn)
 * {
 *     memset(device_id, 0x00, DEVICE_ID_LENGTH);
 *     device_id[1] = 0x02;
 *     
 *     device_id[12] = esn[0];
 *     device_id[13] = esn[1];
 *     device_id[14] = esn[2];
 *     device_id[15] = esn[3];
 * }
 * @endcode
 * 
 * @subsection values_from_strings Converting strings into binary buffers  
 * 
 * Finally, some values such as IMEI and MEID might be retrieved as a string value that needs to be converted into a binary array.
 * In these situations the following piece of code might be useful to do the conversion:
 * @code
 * // Converts the first digit char ('0' to '9') to a nibble starting at index and working backwards.
 * unsigned int digit_to_nibble(char const * const string, int * const index)
 * {
 *     unsigned int nibble = 0;
 *     int current;
 *      
 *     for (current = *index; current >= 0; current--)
 *     {
 *         int const ch = string[current];
 *          
 *         if (isxdigit(ch))
 *         {
 *             if (isdigit(ch))
 *                 nibble = ch - '0';
 *             else
 *                 nibble = toupper(ch) - 'A' + 0xA;
 *             break;
 *         }
 *     }
 *     *index = current - 1;
 *  
 *     return nibble;
 * }
 * 
 * //Parse a string with non-digit characters into an array (i.e.: 0123456-78-901234-5 will be stored in {0x01, 0x23, 0x45, 0x67, 0x89, 0x01, 0x23, 0x45})
 * int string_to_byte_array(char const * const str, uint8_t * const array, size_t const array_size)
 * {
 *     int i;
 *     int const string_length = strlen(str);
 *     int index = string_length - 1;
 *  
 *     for (i = array_size - 1; i >= 0; i--)
 *     {
 *         unsigned int const ls_nibble = digit_to_nibble(str, &index);
 *         unsigned int const ms_nibble = digit_to_nibble(str, &index);
 *  
 *         array[i] = (ms_nibble << 4) + ls_nibble;
 *     }
 *  
 *     return 0;
 * }
 * @endcode
 * 
 * @subsubsection string_to_mac Convert a MAC address string to a binary buffer
 * @code
 *
 * void foo(void)
 * {
 *     uint8_t device_id[DEVICE_ID_LENGTH] = {0};
 *     char const * const mac_string = "01:23:45:67:89:AB";
 *     uint8_t mac_addr[MAC_ADDR_LENGTH] = {0};
 * 
 *     string_to_byte_array(mac_string, mac_addr, sizeof mac_addr);
 *     get_device_id_from_mac(device_id, mac_addr);
 * }
 * @endcode
 * 
 * @subsubsection string_to_imei Convert an IMEI string to a binary buffer
 * @code
 * void foo(void)
 * {
 *     uint8_t device_id[DEVICE_ID_LENGTH] = {0};
 *     char const * const imei_string = "0123456-78-901234-5";
 *     uint8_t imei[IMEI_LENGTH] = {0};
 * 
 *     string_to_byte_array(imei_string, imei, sizeof imei);
 *     get_device_id_from_imei(device_id, imei);
 * }
 * @endcode
 * 
 * @subsubsection string_to_meid Convert an MEID string to a binary buffer
 * @code
 * void foo(void)
 * {
 *     uint8_t device_id[DEVICE_ID_LENGTH] = {0};
 *     char const * const meid_string = "A000000000529247";
 *     uint8_t meid[MEID_LENGTH] = {0};
 * 
 *     string_to_byte_array(meid_string, meid, sizeof imei);
 *     get_device_id_from_meid(device_id, meid);
 * }
 * @endcode
 * 
 * @subsubsection string_to_esn Convert an ESN string to a binary buffer
 * @code
 * void foo(void)
 * {
 *     uint8_t device_id[DEVICE_ID_LENGTH] = {0};
 *     char const * const esn_string = "FAD16100";
 *     uint8_t esn[ESN_LENGTH] = {0};
 * 
 *     string_to_byte_array(esn_string, esn, sizeof esn);
 *     get_device_id_from_esn(device_id, esn);
 * }
 * @endcode
 * 
 * @section get_device_id Load the Device ID
 * Returns a unique Device ID used to identify the device.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_load_device_id() in config.c. It is called to get a unique device ID 
 * which is used to identify the device, typically, it will be read from a non-volatile storage.
 *
 * If this function sets the pointer to a zeroed buffer, the Cloud Connector will ask Device Cloud for a 
 * Device ID and then call function @ref app_save_device_id so the user saves to a non-volatile
 * storage that provisioned Device ID. In future starts, this Device Cloud-assigned Device ID must be returned
 * in this function.
 * @note If Device Cloud provides a Device ID, it automatically adds that Device ID to the Device Cloud
 * account which @ref vendor_id is provided.
 * @note Provision a Device ID can only be done by TCP transport.
 * @note This feature will only work if you enable your account for allowing Device ID provision. To do this, please
 * refer to the @htmlonly<a href="http://ftp1.digi.com/support/documentation/html/90001150/">Device Cloud User's Guide</a>@endhtmlonly,
 * section "User Account Administration", subsection "About the Account Settings Menu", paragraph "Managing your Vendor Information".
 *
 * @see @ref add_your_device_to_the_cloud "Adding your Device to Device Cloud"
 * @see app_load_device_id()
 * @see app_save_device_id()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_device_id @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_data_t @htmlonly:
 *          <dl>
 *              <dt><i>data</i></dt>
 *              <dd> - Callback sets the pointer which contains the Device ID</dd>
 *              <dt><i>bytes_required</i></dt><dd> - Length of the Device ID in bytes.</dd>
 *            </dl>
 * </td>
 *
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned device ID</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get Device ID and Cloud Connector will be aborted</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 * #define APP_DEVICE_ID_SOURCE         MAC
 * #define DEVICE_ID_LENGTH             16
 *
 * static uint8_t device_id[DEVICE_ID_LENGTH];
 *
 * 
 * #define MAC_LENGTH      6
 * #define IMEI_LENGTH     8
 * #define MEID_LENGTH     8
 * #define ESN_LENGTH      4
 *
 * static const uint8_t device_mac[MAC_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 * static const uint8_t device_imei[IMEI_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 * static const uint8_t device_meid[MEID_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 * static const uint8_t device_esn[ESN_LENGTH] = {0x00, 0x00, 0x00, 0x00};
 *
 * static connector_callback_status_t app_load_device_id(connector_config_pointer_data_t * const config_device_id)
 * {
 *     switch (APP_DEVICE_ID_SOURCE)
 *     {
 *         case MAC:
 *             app_get_device_id_from_mac(device_id, device_mac);
 *             break;
 *         case IMEI:
 *             app_get_device_id_from_imei(device_id, device_imei);
 *             break;
 *         case MEID:
 *             app_get_device_id_from_meid(device_id, device_meid);
 *             break;
 *         case ESN:
 *             app_get_device_id_from_esn(device_id, device_esn);
 *             break;
 *         case AUTOPROVISION:
 *             if (device_id_file_exists())
 *             {
 *                 read_device_id_from_file(device_id);
 *             }
 *             else
 *             {
 *                 memset(device_id, 0x00, sizeof device_id, device_esn);
 *             }
 *             break;
 *     }
 * 
 *     config_device_id->data = device_id;
 * 
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @section set_device_id Save the Device ID
 * 
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_save_device_id() in config.c. It is called to save the Device ID 
 * given by Device Cloud to the device. It should be saved to a non-volatile storage to be recovered in future sessions.
 * 
 * Saves the Device ID into a non-volatile storage (EEPROM, file, NVRAM, etc.).
 *
 * This routine is called when a zeroed Device ID is provided in @ref app_load_device_id.
 * In this function the provided Device ID must be saved to a non-volatile storage to be read in future
 * access by @ref app_load_device_id function.
 * @note Provision a Device ID can only be done by TCP transport.
 * @param [in] config_device_id  pointer to memory containing the device ID to be stored in non-volatile storage.
 *
 * @retval connector_callback_continue  Device ID was successfully written.
 * @retval connector_callback_abort     Could not set the device ID, abort Cloud Connector.
 *
 * @see @ref device_id API Configuration Callback
 * @see @ref app_load_device_id
 * @see @ref app_save_device_id
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_device_id @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_data_t @htmlonly:
 *          <dl>
 *              <dt><i>data</i></dt>
 *              <dd> - Pointer which contains application's Device ID</dd>
 *              <dt><i>bytes_required</i></dt><dd> - Length of the Device ID (16 bytes).</dd>
 *            </dl>
 * </td>
 *
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned device ID</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to save Device ID and Cloud Connector will be aborted</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 * #define DEVICE_ID_LENGTH    16
 * #define DEVICE_ID_FILENAME  "device_id.cfg"
 * 
 * static uint8_t provisioned_device_id[DEVICE_ID_LENGTH];
 * 
 * static connector_callback_status_t app_load_device_id(connector_config_pointer_data_t * const config_device_id)
 * {
 *     if (access(DEVICE_ID_FILENAME, F_OK) != -1)
 *     {
 *         FILE *file;
 *         int bytes_written;
 * 
 *         file = fopen(DEVICE_ID_FILENAME, "w+");
 *         bytes_written = fwrite(config_device_id->data, sizeof config_device_id->data[0], sizeof provisioned_device_id / sizeof provisioned_device_id[0], file);
 *
 *         ASSERT(bytes_written == sizeof provisioned_device_id);
 *         fclose(file);
 *     }
 * 
 *     config_device_id->data = provisioned_device_id;
 * 
 *     return connector_callback_continue;
 * }
 * @endcode
 * 
 * @section vendor_id Vendor ID
 *
 * Return vendor ID which is a unique code identifying the manufacturer of a device.
 * Vendor IDs are assigned to manufacturers by Device Cloud.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_vendor_id() in config.c.
 *
 * @note If @ref CONNECTOR_VENDOR_ID configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see @ref connector_vendor_id "Obtaining an Device Cloud Vendor ID"
 * @see app_get_vendor_id()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_vendor_id @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_vendor_id_t @htmlonly:
 *          <dl>
 *              <dt><i>id</i></dt>
 *              <dd> - Callback writes 4-byte vendor ID </dd>
 *            </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned vendor ID</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get vendor ID and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor)
 * {
 *
 *   static uint32_t const device_vendor_id = 0xFE000000;
 *
 *   config_vendor->id  =  device_vendor_id;
 *
 *   return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section device_type Device Type
 *
 * Returns a pointer to the device type which is an iso-8859-1 encoded string.
 * This string should be chosen by the device manufacturer as a name that uniquely
 * identifies this model of device  to Device Cloud. When Device Cloud finds two devices
 * with the same device type, it can infer that they are the same product and
 * product-scoped data may be shared among all devices with this device type.
 * A device's type cannot be an empty string, nor contain only whitespace.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_device_type() in config.c.
 *
 * @note If @ref CONNECTOR_DEVICE_TYPE configuration is defined in @ref connector_config.h, this callback
 * will not be called.
 *
 * @see @ref connector_config_data_options
 * @see app_get_device_type()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_device_type @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_string_t @htmlonly:
 *          <dl>
 *              <dt><i>string</i></dt>
 *              <dd> - Callback returns the pointer which contains device type</dd>
 *              <dt><i>length</i></dt><dd> - Callback returns number of bytes in the device type</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned device type</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get device type and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void  * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_device_type)
 *     {
 *         static char const device_type[] = "Linux Application";
 *
 *         connector_config_pointer_string_t * const config_type = data;
 *         // Return pointer to device type.
 *         config_device_type->string = (char *)device_type;
 *         config_device_type->length = sizeof device_type -1;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section device_cloud_url Device Cloud URL
 *
 * Return Device Cloud FQDN.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_device_cloud_url() in config.c.
 *
 * @note If @ref CONNECTOR_CLOUD_URL configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_device_cloud_url()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_device_cloud_url @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_string_t @htmlonly:
 *          <dl>
 *              <dt><i>string</i></dt>
 *              <dd> - Callback returns the pointer to FQDN of Device Cloud to be connected.</dd>
 *              <dt><i>length</i></dt><dd> - Callback returns number of bytes of Device Cloud FQDN. Maximum is 64 bytes.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned Device Cloud URL</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get Device Cloud URL and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_device_cloud_url)
 *     {
 *         // Return pointer to Device Cloud url.
 *         static  char const device_cloud_url[] = "login.etherios.com";
 *         connector_config_pointer_string_t * const config_url = data;
 *
 *         config_url->string = (char *)device_cloud_url;
 *         config_url->length = sizeof device_cloud_url -1;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section connection_type Connection Type
 *
 * Return connection type (LAN or WAN) used to connect to Device Cloud.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_connection_type() in config.c.
 *
 * If Connection Type is @ref connector_connection_type_lan Cloud Connector will report to Device Cloud the MAC address. If @ref connector_connection_type_lan 
 * is used instead, then Cloud Connector will report the the phone number dialed and the link speed.
 * 
 * @note If @ref CONNECTOR_CONNECTION_TYPE configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_connection_type()
 * @see mac_address
 * @see phone_number
 * @see link_speed
 * 
 * @htmlonly
 * <table class="apitable">
 *   <tr>
 *     <th colspan="2" class="title">Arguments</th>
 *   </tr>
 *   <tr>
 *     <th class="subtitle">Name</th> <th class="subtitle">Description</th>
 *    </tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_connection_type @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_connection_type_t @htmlonly:
 *          <dl>
 *              <dt><i>type</i></dt>
 *              <dd> <ul><li> @endhtmlonly @ref connector_connection_type_lan @htmlonly - Callback returns this for LAN connection type</li>
 *                       <li> @endhtmlonly @ref connector_connection_type_wan @htmlonly - Callback returns this for WAN connection type</li></ul>
 *              </dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned connection type</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get connection type and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void  * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_config_connection_type)
 *     {
 *         connector_config_connection_type_t * const config_connection = data;
 *
 *         config_connection->type = connector_connection_type_lan;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section mac_address MAC Address
 *
 * Return the device's MAC address.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_mac_addr() in config.c.
 *
 * @see app_get_mac_addr()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_mac_addr @htmlonly</td>
 * </tr>
 * <tr>
* <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_data_t @htmlonly:
 *          <dl>
 *              <dt><i>data</i></dt>
 *              <dd> - Callback returns the pointer which contains device's MAC address</dd>
 *              <dt><i>bytes_required</i></dt><dd> - Contains 6 bytes which is required for the device's MAC address in the data pointer</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned MAC address</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get MAC address and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * #define MAC_ADDR_LENGTH     6
 * static uint8_t const device_mac_addr[MAC_ADDR_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 *
 * static connector_callback_status_t app_get_mac_addr(connector_config_pointer_data_t * const config_mac)
 * {
 *
 *   ASSERT(config_mac->bytes_required == MAC_ADDR_LENGTH);
 *
 *   config_mac->data = (uint8_t *)device_mac_addr;
 *
 *   return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section link_speed Link Speed
 *
 * Return link speed for a WAN connection type. If connection type is LAN, Cloud Connector will not request this value.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_link_speed() in config.c.
 *
 * @note If @ref CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND configuration is defined in @ref connector_config.h,
 * this callback will not be called. See @ref connector_config_data_options
 *
 * @see app_get_link_speed()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_link_speed @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_link_speed_t @htmlonly:
 *          <dl>
 *              <dt><i>speed</i></dt>
 *              <dd> - Callback writes 4-byte integer speed of the link in bits per second. All zeros represent unknown speed.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned link speed</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get link speed and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_config_link_speed)
 *     {
 *         connector_config_link_speed_t * const config_link = data;
 *         config_link->speed = 0;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section phone_number Phone number
 *
 * Return the phone number dialed for a WAN connection type.
 * The phone number is a variable length, non null-terminated string.
 * If connection type is LAN, Cloud Connector will not request this value.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_phone_number() in config.c.
 *
 * @note If @ref CONNECTOR_WAN_PHONE_NUMBER_DIALED configuration is defined in @ref connector_config.h,
 * this callback will not be called.  See @ref connector_config_data_options
 *
 * @see app_get_phone_number()
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
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_phone_number @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_string_t @htmlonly:
 *          <dl>
 *              <dt><i>string</i></dt>
 *              <dd> - Callback returns pointer to memory which contains the phone number dialed, including any dialing prefixes. </dd>
 *              <dt><i>length</i></dt>
 *              <dd> - Callback returns number of bytes of the phone number in the string pointer.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned phone number</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get phone number and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_config_phone_number)
 *     {
 *         static char const phone_number[] ="000-000-0000";
 *         connector_config_pointer_string_t * const config_phone_number = data;
 *
 *         config_phone_number->string = (char *)phone_number;
 *         config_phone_number->length = sizeof phone_number -1;
 *
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section tx_keepalive TX Keepalive Interval
 *
 * Return TX keepalive interval in seconds. This tells how
 * often Device Cloud sends keepalive messages to the device.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_tx_keepalive_interval() in config.c.
 *
 * @note If @ref CONNECTOR_TX_KEEPALIVE_IN_SECONDS is defined in @ref connector_config.h,
 * this callback will not be called.  See @ref connector_config_data_options
 *
 * @see app_get_tx_keepalive_interval()
 *
 * @htmlonly
 * <table class="apitable">
 *   <tr>
 *     <th colspan="2" class="title">Arguments</th>
 *   </tr>
 *   <tr>
 *     <th class="subtitle">Name</th>
 *     <th class="subtitle">Description</th>
 *   </tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_tx_keepalive @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_keepalive_t @htmlonly:
 *          <dl>
 *              <dt><i>interval_in_seconds</i></dt>
 *              <dd> - Callback writes 2-byte integer TX keep alive interval in seconds. It must be between 5 and 7200 seconds.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned TX keepalive interval</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get TX keepalive interval and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_tx_keepalive)
 *     {
 *         #define DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS     90
 *         connector_config_keepalive_t * const config_keepalive = data;
 *
 *         // Return Tx keepalive interval in seconds
 *         config_keepalive->interval_in_seconds = DEVICE_TX_KEEPALIVE_INTERVAL_IN_SECONDS;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section rx_keepalive RX Keepalive Interval
 *
 * Return RX keepalive interval in seconds. This tells how
 * often Cloud Connector sends keepalive messages to Device Cloud (device to Device Cloud).
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_rx_keepalive_interval() in config.c.
 *
 * @note If @ref CONNECTOR_RX_KEEPALIVE_IN_SECONDS configuration is defined in @ref connector_config.h,
 * this callback will not be called.  See @ref connector_config_data_options
 *
 * @see app_get_rx_keepalive_interval()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_rx_keepalive @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_keepalive_t @htmlonly:
 *          <dl>
 *              <dt><i>interval_in_seconds</i></dt>
 *              <dd> - Callback writes 2-byte integer RX keep alive interval in seconds. It must be between 5 and 7200 seconds.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned RX keepalive interval</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get RX keepalive interval and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_rx_keepalive)
 *     {
 *         #define DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS     60
 *         connector_config_keepalive_t * const config_keepalive = data;
 *
 *         // Return Tx keepalive interval in seconds
 *         config_keepalive->interval_in_seconds = DEVICE_RX_KEEPALIVE_INTERVAL_IN_SECONDS;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section wait_count Wait Count
 *
 * Return the number of times of not receiving a keepalive message which a connection
 * should be considered lost.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_wait_count() in config.c.
 *
 * @note If @ref CONNECTOR_WAIT_COUNT configuration is defined in connector_config.h,
 * this callback will not be called.  See @ref connector_config_data_options
 *
 * @see app_get_wait_count()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_wait_count @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_wait_count_t @htmlonly:
 *          <dl>
 *              <dt><i>count</i></dt>
 *              <dd> - Callback writes 2-byte wait count. It must be between 2 and 64 times.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned wait count</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get wait count and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_wait_count)
 *     {
 *         #define DEVICE_WAIT_COUNT     5
 *         connector_config_wait_count_t * const config_wait = data;
 *
 *          // Return wait count (number of times not receiving Tx keepalive
 *          // from Device Cloud is allowed).
 *         config_wait->count = DEVICE_WAIT_COUNT;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section ip_address Device IP Address
 *
 * Return a unique device IP address. The IP address must be either
 * an IPv4 or IPv6 address.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_ip_address() in config.c.
 *
 * @see app_get_ip_address()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_ip_addr @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_ip_address_t @htmlonly:
 *          <dl>
 *              <dt><i>address</i></dt>
 *              <dd> - Callback returns pointer to device's IP address.</dd>
 *              <dt><i>ip_address_type</i></dt>
 *              <dd> - <ul><li>@endhtmlonly @ref connector_ip_address_ipv4 @htmlonly - Callback returns this for IPv4 address. The size of the address returned must be 4 bytes.</li>
 *                         <li>@endhtmlonly @ref connector_ip_address_ipv6 @htmlonly - Callback returns this for IPv6 address. The size of the address returned must be 16 bytes.</li></ul></dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned IP address</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get IP address and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_ip_addr)
 *     {
 *         extern uint32_t device_ip_address;
 *
 *         connector_config_ip_address_t * const config_ip = data;

 *         config_ip->ip_address_type = connector_ip_address_ipv4;
 *         config_ip->address = (uint8_t *)&device_ip_address;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section error_status  Error Status Notification
 *
 * This callback is called to notify the user that Cloud Connector encountered an error. When Cloud Connector finds an
 * error, Cloud Connector will call this callback indicating the error status
 *
 * @note If @ref CONNECTOR_DEBUG  is not defined in connector_config.h, this callback
 * will not be called.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_error_status @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_error_status_t @htmlonly containing
 *     the class id, request id, and error status which Cloud Connector encountered error with:
 *          <dl>
 *              <dt><i>class_id</i></dt><dd> - Class ID </dd>
 *              <dt><i>request_id</i></dt><dd> - Request ID</dd>
 *              <dt><i>status</i></dt><dd> - Error status</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>None</td>
 * <td>None</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * void app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_error_status)
 *     {
 *         connector_config_error_status_t * error_data = data;
 *         printf("connector_error_status: unsupport class_id = %d request_id = %d status = %d\n",
 *                   error_data->class_id, error_data->request_id.config_request, error_data->status);
 *     }
 *     return;
 * }
 *
 * @endcode
 *
 * @section firmware_support  Firmware Access Support
 *
 * Return @ref connector_config_supported_t status to enable or disable Firmware
 * download capability. If it's supported, callback for @ref connector_request_id_firmware_t
 * must be implemented for firmware download. This callback allows application to enable
 * or disable firmware download capability during runtime.
 *
 * @note If @ref CONNECTOR_FIRMWARE_SERVICE configuration is not defined in @ref connector_config.h, this callback
 * will not be called and Firmware Download is not supported. Cloud Connector does not include firmware support.
 *
 * @note If @ref CONNECTOR_FIRMWARE_SUPPORT is defined in @ref connector_config.h, this callback is not needed.
 * It enables firmware download capability. See @ref connector_config_data_options. CONNECTOR_FIRMWARE_SERVICE must be defined.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_firmware_facility @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_supported_t @htmlonly:
 *          <dl>
 *              <dt><i>supported</i></dt><dd> -
 *                     <ul><li>@endhtmlonly @ref connector_false @htmlonly - callback writes this if it doesn't support firmware download.</li>
 *                         <li>@endhtmlonly @ref connector_true @htmlonly - callback writes this if it supports firmware download.</li></ul></dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_firmware_facility)
 *     {
 *         connector_config_supported_t * const config_firmware = data;
 *         config_firmware->supported = connector_true;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section data_service_support  Data Service Support
 *
 * Enable or disable data service capability. If it's supported, callback
 * for @ref connector_request_id_config_data_service must be
 * implemented for data service. This callback allows application to enable
 * or disable data service capability during runtime.
 *
 * @note If @ref CONNECTOR_DATA_SERVICE configuration is not defined in @ref connector_config.h, this callback
 * will not be called and Data Service is not supported. Cloud Connector does not include data service.
 *
 * @note If @ref CONNECTOR_DATA_SERVICE_SUPPORT is defined in @ref connector_config.h, this callback is not needed.
 * It enables data service capability. See @ref connector_config_data_options. CONNECTOR_DATA_SERVICE must be defined.
 *
 * @note Define @ref CONNECTOR_COMPRESSION in @ref connector_config.h for compression transfer.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_data_service @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_supported_t @htmlonly:
 *          <dl>
 *              <dt><i>supported</i></dt><dd> -
 *                     <ul><li>@endhtmlonly @ref connector_false @htmlonly - callback writes this if it doesn't support data service.</li>
 *                         <li>@endhtmlonly @ref connector_true @htmlonly - callback writes this if it supports data service.</li></ul></dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_data_service)
 *     {
 *         connector_config_supported_t * const config_data_service = data;
 *         config_data_service->supported = connector_true;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section file_system_support  File System Support
 *
 * Enable or disable file system capability. If it's supported, callback for
 * @ref connector_request_id_config_file_system must be
 * implemented for file system. This callback allows application to enable
 * or disable file system capability during runtime.
 *
 * @note If @ref CONNECTOR_FILE_SYSTEM configuration is not defined in @ref connector_config.h, this callback
 * will not be called and File System is not supported. Cloud Connector does not include file system.
 *
 * @note If @ref CONNECTOR_FILE_SYSTEM_SUPPORT is defined in @ref connector_config.h, this callback is not needed.
 * It enables file system capability. See @ref connector_config_data_options. CONNECTOR_FILE_SYSTEM must be defined.
 *
 * @note Define @ref CONNECTOR_COMPRESSION in @ref connector_config.h for compression transfer.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_file_system @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_supported_t @htmlonly:
 *          <dl>
 *              <dt><i>supported</i></dt><dd> -
 *                     <ul><li>@endhtmlonly @ref connector_false @htmlonly - callback writes this if it doesn't support file system.</li>
 *                         <li>@endhtmlonly @ref connector_true @htmlonly - callback writes this if it supports file system. </li></ul></dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_file_system)
 *     {
 *         connector_config_supported_t * const config_file_system = data;
 *         config_file_system->supported = connector_true;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section rci_support  Remote Configuration Support
 *
 * Enable or disable remote configuration capability. If it's supported, callback for
 * @ref connector_request_id_config_remote_configuration must be
 * implemented for remote configuration. This callback allows application to enable
 * or disable remote configuration capability during runtime.
 *
 * @note If @ref CONNECTOR_RCI_SERVICE is not defined in @ref connector_config.h, this callback
 * will not be called and remote configuration is not supported. Cloud Connector does not include remote configuration.
 *
 * @note If @ref CONNECTOR_REMOTE_CONFIGURATION_SUPPORT is defined in @ref connector_config.h, this callback is not needed.
 * It enables remote configuration capability. See @ref connector_config_data_options.
 *
 * @note If using the @ref CONNECTOR_NO_MALLOC version, then the @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH macro must be set to the maximum content length in bytes of an element's value in @ref connector_config.h.
 *
 * @note Define @ref CONNECTOR_COMPRESSION in @ref connector_config.h for compression transfer.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_remote_configuration @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_supported_t @htmlonly:
 *          <dl>
 *              <dt><i>supported</i></dt><dd> -
 *                     <ul><li>@endhtmlonly @ref connector_false @htmlonly - callback writes this if it doesn't support remote configuration.</li>
 *                         <li>@endhtmlonly @ref connector_true @htmlonly - callback writes this if it supports remote configuration.</li></ul></dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_remote_configuration)
 *     {
 *         connector_config_supported_t * const config_rci = data;
 *         config_rci->supported = connector_true;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section max_msg_transactions Maximum Message Transactions
 *
 * Return maximum simultaneous transactions for data service and file system to receive message from
 * Device Cloud. This configuration is required if @ref data_service_support or @ref file_system_support
 * is enabled.
 *
 * @note If @ref CONNECTOR_MSG_MAX_TRANSACTION configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_max_transaction @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_max_transaction_t @htmlonly:
 *          <dl>
 *              <dt><i>count</i></dt><dd>Callback writes maximum simultaneous transactions. Use 0 for unlimited transactions.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned maximum transactions</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                   connector_request_id_t const request_id
 *                                                   void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_max_transaction)
 *     {
 *         #define CONNECTOR_MAX_MSG_TRANSACTIONS   1
 *         connector_config_max_transaction_t * const config_max_transaction = data;
 *
 *         config_max_transaction->count = CONNECTOR_MAX_MSG_TRANSACTIONS;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section network_tcp_start  Start network TCP
 *
 * Return @ref connector_config_connect_type_t to automatic or manual start TCP transport.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_start_network_tcp() in config.c.
 *
 * @note If @ref CONNECTOR_TRANSPORT_TCP configuration is not defined in @ref connector_config.h, this callback
 * will not be called and TCP transport is not supported. Cloud Connector does not include TCP transport.
 *
 * @note If @ref CONNECTOR_NETWORK_TCP_START is defined in @ref connector_config.h, this callback is not needed.
 * See @ref connector_config_data_options. CONNECTOR_TRANSPORT_TCP must be defined.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_network_tcp @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_connect_type_t @htmlonly:
 *          <dl>
 *              <dt><i>type</i></dt>
 *              <dd>
 *                  <ul><li>@endhtmlonly @ref connector_connect_auto @htmlonly - Callback returns this to automatic connect to Device Cloud.</li>
 *                      <li>@endhtmlonly @ref connector_connect_manual @htmlonly - Callback returns this to manual connect to Device Cloud.
 *                          @endhtmlonly Note: Call @ref connector_initiate_action with @ref connector_initiate_transport_start @htmlonly to start TCP transport.</li></ul></dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr><th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static connector_callback_status_t app_start_network_tcp(connector_config_connect_type_t * const device_connect)
 * {
 *    device_connect->type = connector_connect_auto;
 *
 *    return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section network_udp_start  Start network UDP
 *
 * Return @ref connector_config_connect_type_t to automatically or manually start UDP transport.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_start_network_udp() in config.c.
 *
 * @note If @ref CONNECTOR_TRANSPORT_UDP configuration is not defined in @ref connector_config.h, this callback
 * will not be called and UDP transport is not supported. Cloud Connector does not include UDP transport.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_network_udp @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_connect_type_t @htmlonly:
 *          <dl>
 *              <dt><i>type</i></dt><dd>
 *                  <ul><li>@endhtmlonly @ref connector_connect_auto @htmlonly - Callback returns this to automatic establish UDP connection to Device Cloud.</li>
 *                      <li>@endhtmlonly @ref connector_connect_manual @htmlonly - Callback returns this to manual establish UDP connection to Device Cloud.
 *                          @endhtmlonly Note: Call @ref connector_initiate_action with
 *                                                  @ref connector_initiate_transport_start @htmlonly to start UPD transport.</li></ul>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static connector_callback_status_t app_start_network_udp(connector_config_connect_type_t * const device_connect)
 * {
 *    device_connect->type = connector_connect_auto;
 *
 *    return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section network_sms_start  Start network SMS
 *
 * Return @ref connector_config_connect_type_t to automatically or manually start SMS transport.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_start_network_sms() in config.c.
 *
 * @note If @ref CONNECTOR_TRANSPORT_SMS configuration is not defined in @ref connector_config.h, this callback
 * will not be called and SMS transport is not supported. Cloud Connector does not include SMS transport.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_network_sms @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_connect_type_t @htmlonly:
 *          <dl>
 *              <dt><i>type</i></dt><dd>
 *                  <ul><li>@endhtmlonly @ref connector_connect_auto @htmlonly - Callback returns this to automatic establish SMS connection to Device Cloud.</li>
 *                      <li>@endhtmlonly @ref connector_connect_manual @htmlonly - Callback returns this to manual establish SMS connection to Device Cloud.
 *                          @endhtmlonly Note: Call @ref connector_initiate_action with
 *                                                  @ref connector_initiate_transport_start @htmlonly to start UPD transport.</li></ul>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static connector_callback_status_t app_start_network_sms(connector_config_connect_type_t * const device_connect)
 * {
 *    device_connect->type = connector_connect_auto;
 *
 *    return connector_callback_continue;
 * }
 *
 * @endcode
 *
 *
 * @section identity_verification Identity verification form
 *
 * Return simple or password identity verification form.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_identity_verification() in config.c.
 *
 * @note If @ref CONNECTOR_IDENTITY_VERIFICATION configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_identity_verification()
 *
 * @htmlonly
 * <table class="apitable">
 *   <tr>
 *     <th colspan="2" class="title">Arguments</th>
 *   </tr>
 *   <tr>
 *     <th class="subtitle">Name</th> <th class="subtitle">Description</th>
 *    </tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_identity_verification @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_identity_verification_t @htmlonly:
 *          <dl>
 *              <dt><i>type</i></dt>
 *              <dd> <ul><li>@endhtmlonly @ref connector_identity_verification_simple @htmlonly -  Callback returns this type for simple identity verification.</li>
 *                       <li>@endhtmlonly @ref connector_identity_verification_password @htmlonly - Callback returns this type for password identity verification.</li></ul></dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned identity verification form</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get identity verification form and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 *    static connector_callback_status_t app_get_identity_verification(connector_config_identity_verification_t * const identity)
 *    {
 *        identity->type = connector_identity_verification_simple;
 *
 *        return connector_callback_continue;
 *    }
 *
 * @endcode
 *
 * @section password Password
 *
 * Return the password for password identity verification form.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_password() in config.c.
 *
 * @note This callback will not be called for simple identity verification form. See @ref identity_verification
 *
 * @see app_get_password()
 *
 * @htmlonly
 * <table class="apitable">
 *   <tr>
 *     <th colspan="2" class="title">Arguments</th>
 *   </tr>
 *   <tr>
 *     <th class="subtitle">Name</th> <th class="subtitle">Description</th>
 *    </tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_password @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_data_t @htmlonly:
 *          <dl>
 *              <dt><i>data</i></dt>
 *              <dd> - Callback returns the pointer to the password.</dd>
 *              <dt><i>length</i></dt><dd> - Callback writes the length of the password in bytes.</dd>
 *            </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned the password</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get the password and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static connector_callback_status_t app_get_password(connector_config_pointer_string_t * const config_password)
 * {
 *     static  char const connector_password[] = "password";
 *
 *      // Return pointer to password.
 *     config_password->string = (char *)connector_password;
 *     config_password->length = sizeof connector_password -1;
 *
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section get_device_cloud_phone Device Cloud phone number (Get)
 *
 * Returns Device Cloud phone number where to send SMSs (Only used for SMS transport).
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_device_cloud_phone() in config.c.
 *
 * @note If @ref CONNECTOR_CLOUD_PHONE configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_device_cloud_phone()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_get_device_cloud_phone @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_string_t @htmlonly:
 *          <dl>
 *              <dt><i>string</i></dt>
 *              <dd> - Callback returns the pointer to the Device Cloud phone number where to send SMSs (Only used for SMS transport).</dd>
 *              <dt><i>length</i></dt><dd> - Callback returns number of bytes of Device Cloud phone number. Maximum is 64 bytes.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned Device Cloud phone number</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get Device Cloud phone number and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static char connector_cloud_phone[] = "447786201216";
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_get_device_cloud_phone)
 *     {
 *         // Return pointer to Device Cloud phone number.
 *         connector_config_pointer_string_t * const config_phone = data;
 *
 *         config_phone->string = (char *)connector_cloud_phone;
 *         config_phone->length = sizeof connector_cloud_phone -1;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section set_device_cloud_phone Device Cloud phone number (Set)
 *
 * This config callback is used to set (not to get) the Device Cloud phone number where to send SMSs (Only used for SMS transport).
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_set_device_cloud_phone() in config.c.
 *
 * @note If @ref CONNECTOR_CLOUD_PHONE configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_set_device_cloud_phone()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_set_device_cloud_phone @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_string_t @htmlonly:
 *          <dl>
 *              <dt><i>string</i></dt>
 *              <dd> - Pointer passed to the Callback containing the Device Cloud phone number where to send SMSs in order to 
 *              </dd>  be stored in persistent storage. (Only used for SMS transport)
 *              <dt><i>length</i></dt><dd> - Number of bytes of Device Cloud phone number passed. Maximum is 64 bytes.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully set Device Cloud phone number</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set Device Cloud phone number and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static char connector_cloud_phone[16] = "";	// empty: will require a provisioning message from the server for initialization
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_set_device_cloud_phone)
 *     {
 *         if (config_phone->length > (sizeof connector_cloud_phone -1))
 *         {
 *             return connector_callback_error;
 *         }
 *
 *         strcpy(connector_cloud_phone, config_phone->string);
 *
 *         // Maybe user want to save connector_cloud_phone to persistent storage
 *
 *         return connector_callback_continue;
 *     }
 * }
 *
 * @endcode
 *
 *
 * @section device_cloud_service_id Device Cloud phone service-id
 *
 * Return Device Cloud service-id (if required) used during SMS transport.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function app_get_device_cloud_service_id() in config.c.
 *
 * @note If @ref CONNECTOR_CLOUD_SERVICE_ID configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_device_cloud_service_id()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_device_cloud_service_id @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly connector_config_pointer_string_t @htmlonly:
 *          <dl>
 *              <dt><i>string</i></dt>
 *              <dd> - Callback returns the pointer to Device Cloud phone number service-id (if required) used during SMS transport.</dd>
 *              <dt><i>length</i></dt><dd> - Callback returns number of bytes of Device Cloud service-id. Maximum is 64 bytes.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned Device Cloud service-id</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to get Device Cloud service-id and callback aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id,
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_config && request_id.config_request == connector_request_id_config_device_cloud_service_id)
 *     {
 *         // Return pointer to Device Cloud service_id.
 *         static char connector_cloud_service_id[] = "";	// empty: No shared code
 *         connector_config_pointer_string_t * const config_service_id = data;
 *
 *         config_service_id->string = (char *)connector_cloud_service_id;
 *         config_service_id->length = sizeof connector_cloud_service_id -1;
 *     }
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section sm_udp_max_sessions Short Messaging over UDP maximum active sessions
 *
 * Set the maximum concurrent sessions of Short Messaging over UDP transport.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_get_sm_udp_max_sessions() in config.c.
 *
 * @note If @ref CONNECTOR_SM_UDP_MAX_SESSIONS configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_sm_udp_max_sessions()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_sm_udp_max_sessions @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_config_sm_max_sessions_t @htmlonly:
 *          <dl>
 *              <dt><i>max_sessions</i></dt>
 *              <dd> - Callback returns the maximum sessions for UDP transport.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned requested data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set data and aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * #define APP_MAX_SM_UDP_SESSIONS      32
 *
 * static connector_callback_status_t app_get_sm_udp_max_sessions(connector_config_sm_max_sessions_t * const config_max_sessions)
 * {
 *     config_max_sessions->max_sessions = APP_MAX_SM_UDP_SESSIONS;
 * 
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 * 
 * @section sm_sms_max_sessions Short Messaging over SMS maximum active sessions
 *
 * Set the maximum concurrent sessions of Short Messaging over SMS transport.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_get_sm_sms_max_sessions() in config.c.
 *
 * @note If @ref CONNECTOR_SM_SMS_MAX_SESSIONS configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_sm_sms_max_sessions()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_sm_sms_max_sessions @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_config_sm_max_sessions_t @htmlonly:
 *          <dl>
 *              <dt><i>max_sessions</i></dt>
 *              <dd> - Callback returns the maximum sessions for SMS transport.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned requested data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set data and aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * #define APP_MAX_SM_SMS_SESSIONS      32
 *
 * static connector_callback_status_t app_get_sm_sms_max_sessions(connector_config_sm_max_sessions_t * const config_max_sessions)
 * {
 *     config_max_sessions->max_sessions = APP_MAX_SM_SMS_SESSIONS;
 * 
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section sm_udp_max_rx_segments Short Messaging over UDP Maximum incoming segments
 *
 * Set the maximum number of segments in which an incoming session's data in SM over UDP might be split.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_get_sm_udp_max_rx_segments() in config.c.
 *
 * @note If @ref CONNECTOR_SM_UDP_MAX_RX_SEGMENTS configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_sm_udp_max_rx_segments()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_sm_sms_max_rx_segments @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_config_sm_max_rx_segments_t @htmlonly:
 *          <dl>
 *              <dt><i>max_rx_segments</i></dt>
 *              <dd> - Callback returns the maximum incoming segments for UDP transport.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned requested data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set data and aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * #define APP_MAX_SM_UDP_RX_SEGMENTS      5
 *
 * static connector_callback_status_t app_get_sm_udp_max_rx_segments(connector_config_sm_max_rx_segments_t * const config_max_rx_segments)
 * {
 *     config_max_rx_segments->max_rx_segments = APP_MAX_SM_UDP_RX_SEGMENTS;
 * 
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section sm_sms_max_rx_segments Short Messaging over SMS Maximum incoming segments
 *
 * Set the maximum number of segments in which an incoming session's data in SM over SMS might be split.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_get_sm_sms_max_rx_segments() in config.c.
 *
 * @note If @ref CONNECTOR_SM_SMS_MAX_RX_SEGMENTS configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_sm_sms_max_rx_segments()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_sm_sms_max_rx_segments @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_config_sm_max_rx_segments_t @htmlonly:
 *          <dl>
 *              <dt><i>max_rx_segments</i></dt>
 *              <dd> - Callback returns the maximum incoming segments for SMS transport.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned requested data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set data and aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * #define APP_MAX_SM_SMS_RX_SEGMENTS      3
 *
 * static connector_callback_status_t app_get_sm_sms_max_rx_segments(connector_config_sm_max_rx_segments_t * const config_max_rx_segments)
 * {
 *     config_max_rx_segments->max_rx_segments = APP_MAX_SM_SMS_RX_SEGMENTS;
 * 
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section sm_udp_rx_timeout Short Messaging over UDP incoming sessions' timeout
 *
 * Set the how many seconds to wait before dropping incomplete incoming sessions. It can be set to SM_WAIT_FOREVER for 
 * infinite wait time but it is not recommended as delivery is not guaranteed in Short Messaging.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_get_sm_udp_rx_timeout() in config.c.
 *
 * @note If @ref CONNECTOR_SM_UDP_RX_TIMEOUT configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_sm_udp_rx_timeout()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_sm_udp_rx_timeout @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_config_sm_rx_timeout_t @htmlonly:
 *          <dl>
 *              <dt><i>rx_timeout</i></dt>
 *              <dd> - Callback returns the timeout for incoming SM over UDP sessions.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned requested data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set data and aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * #define APP_MAX_SM_UDP_RX_TIMEOUT      30
 *
 * static connector_callback_status_t app_get_sm_udp_rx_timeout(connector_config_sm_rx_timeout_t * const config_rx_timeout)
 * {
 *     config_max_rx_segments->rx_timeout = APP_MAX_SM_UDP_RX_TIMEOUT;
 * 
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 *
 * @section sm_sms_rx_timeout Short Messaging over SMS incoming sessions' timeout
 *
 * Set the how many seconds to wait before dropping incomplete incoming sessions. It can be set to SM_WAIT_FOREVER for 
 * infinite wait time but it is not recommended as delivery is not guaranteed in Short Messaging.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_get_sm_sms_rx_timeout() in config.c.
 *
 * @note If @ref CONNECTOR_SM_SMS_RX_TIMEOUT configuration is defined in @ref connector_config.h, this callback
 * will not be called. See @ref connector_config_data_options
 *
 * @see app_get_sm_sms_rx_timeout()
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_sm_sms_rx_timeout @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_config_sm_rx_timeout_t @htmlonly:
 *          <dl>
 *              <dt><i>rx_timeout</i></dt>
 *              <dd> - Callback returns the timeout for incoming SM over SMS sessions.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned requested data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set data and aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * #define APP_MAX_SM_SMS_RX_TIMEOUT      (3 * 60)
 *
 * static connector_callback_status_t app_get_sm_sms_rx_timeout(connector_config_sm_rx_timeout_t * const config_rx_timeout)
 * {
 *     config_max_rx_segments->rx_timeout = APP_MAX_SM_SMS_RX_TIMEOUT;
 * 
 *     return connector_callback_continue;
 * }
 *
 * @endcode
 * @section rci_descriptor_data Remote Configuration Interface Descriptor Data
 *
 * This callback is used by Cloud Connector when @ref rci_service is enabled to load all the information automatically generated by @ref rci_tool.
 *
 * This callback is trapped in application.c, in the @b Sample section of @ref AppStructure "Public Application Framework"
 * and implemented in the @b Platform function @ref app_load_rci_descriptor_data() in config.c.
 *
 * @see app_load_rci_descriptor_data()
 * @see @ref CONNECTOR_RCI_SERVICE
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <th>class_id</th>
 * <td>@endhtmlonly @ref connector_class_id_config @htmlonly</td>
 * </tr>
 * <tr>
 * <th>request_id</th>
 * <td>@endhtmlonly @ref connector_request_id_config_rci_descriptor_data @htmlonly</td>
 * </tr>
 * <tr>
 * <th>data</th>
 * <td> Pointer to @endhtmlonly @ref connector_config_rci_descriptor_data_t @htmlonly:
 *          <dl>
 *              <dt><i>rci_data</i></dt>
 *              <dd> - Callback returns a pointer to Cloud Connector's internal RCI data structure. This is obtained from autogenerated remote_config.c file.
 *                     The "rci_descriptor_data" pointer is available when including connector_api.h and except that you know what you are doing, is the only value to be used.</dd>
 *          </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned requested data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_abort @htmlonly</td>
 * <td>Callback was unable to set data and aborted Cloud Connector</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Example:
 *
 * @code
 *
 * static connector_callback_status_t app_load_rci_descriptor_data(connector_config_rci_descriptor_data_t * const config_rci_descriptor_data)
 * {
 *     connector_callback_status_t status = connector_callback_continue;
 * 
 *     config_rci_descriptor_data->rci_data = rci_descriptor_data; /* Autogenerated in remote_config.c by ConfigGenerator.jar tool */
 *     return status;
 * }
 *
 * @endcode
 *
 * @htmlinclude terminate.html
 */
