/*! @page memorymetricspage Code Space and memory usage estimates
 *
 * @htmlinclude nav.html
 *
 * @section CodespaceRequirements Flash Usage
 * This data was generated using Kubuntu 10.04 and gcc v4.4.3 for 32-bit i486 architecture
 * using the @ref step3 "compile_and_link" or @ref rci_sample "simple_remote_config" sample applications.
 *
 * The build was optimized for size (-Os) with @ref CONNECTOR_COMPRESSION disabled in all cases.
 * The metrics exclude any code space required for application layer calls.  This information should be treated as a typical use case.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Sample</th>
 * <th class="title">Options</th>
 * <th class="title">.text</th>
 * <th class="title">.rodata</th>
 * <th class="title">.data</th>
 * <th class="title">.bss</th>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref rci_sample "simple_remote_config" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_DEBUG, @ref CONNECTOR_RCI_SERVICE, @ref CONNECTOR_FILE_SYSTEM, @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE enabled, plus @ref CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH and @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH @htmlonly set to 256.</td>
 * <td>38305</td>
 * <td>11683</td>
 * <td>0</td>
 * <td>444</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref rci_sample "simple_remote_config" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_RCI_SERVICE, @ref CONNECTOR_FILE_SYSTEM, @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE enabled, plus @ref CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH and @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH @htmlonly set to 256.</td>
 * <td>32298</td>
 * <td>1236</td>
 * <td>0</td>
 * <td>440</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref rci_sample "simple_remote_config" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_DEBUG, @ref CONNECTOR_RCI_SERVICE, @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE enabled and @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH @htmlonly set to 256.</td>
 * <td>32311</td>
 * <td>11274</td>
 * <td>0</td>
 * <td>444</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref rci_sample "simple_remote_config" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_RCI_SERVICE, @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE enabled and @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH @htmlonly set to 256.</td>
 * <td>26513</td>
 * <td>1156</td>
 * <td>0</td>
 * <td>440</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref rci_sample "simple_remote_config" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_DEBUG, @ref CONNECTOR_RCI_SERVICE enabled and @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH @htmlonly set to 256.</td>
 * <td>26946</td>
 * <td>9450</td>
 * <td>0</td>
 * <td>444</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref rci_sample "simple_remote_config" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_RCI_SERVICE enabled and @ref CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH @htmlonly set to 256.</td>
 * <td>22038</td>
 * <td>1020</td>
 * <td>0</td>
 * <td>440</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_DEBUG, @ref CONNECTOR_FILE_SYSTEM, @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE enabled and @ref CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH @htmlonly set to 256.</td>
 * <td>31366</td>
 * <td>9147</td>
 * <td>0</td>
 * <td>24</td>
 * </tr>
  * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_FILE_SYSTEM, @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE enabled and @ref CONNECTOR_FILE_SYSTEM_MAX_PATH_LENGTH @htmlonly set to 256.</td>
 * <td>26755</td>
 * <td>672</td>
 * <td>0</td>
 * <td>20</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_DEBUG, @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE @htmlonly enabled.</td>
 * <td>25247</td>
 * <td>8702</td>
 * <td>0</td>
 * <td>24</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE @htmlonly enabled.</td>
 * <td>20867</td>
 * <td>592</td>
 * <td>0</td>
 * <td>20</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_FIRMWARE_SERVICE, @ref CONNECTOR_DATA_SERVICE enabled.  @ref connector_config_data_options used. @htmlonly </td>
 * <td>19477</td>
 * <td>592</td>
 * <td>0</td>
 * <td>20</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_FIRMWARE_SERVICE enabled.  @ref connector_config_data_options used. @htmlonly </td>
 * <td>11795</td>
 * <td>472</td>
 * <td>0</td>
 * <td>20</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_DATA_SERVICE enabled.  @ref connector_config_data_options used. @htmlonly </td>
 * <td>17085</td>
 * <td>460</td>
 * <td>0</td>
 * <td>20</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref step3 "compile_and_link" @htmlonly </td>
 * <td>@endhtmlonly @ref CONNECTOR_DATA_SERVICE, @ref CONNECTOR_DATA_POINTS enabled.  @ref connector_config_data_options used. @htmlonly </td>
 * <td>19608</td>
 * <td>524</td>
 * <td>4</td>
 * <td>28</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Options are explained in @ref connector_config_data_options and defined in connector_config.h.
 * See @ref CONNECTOR_DEVICE_TYPE, @ref CONNECTOR_CLOUD_URL, @ref CONNECTOR_TX_KEEPALIVE_IN_SECONDS,
 * @ref CONNECTOR_RX_KEEPALIVE_IN_SECONDS, @ref CONNECTOR_WAIT_COUNT, @ref CONNECTOR_VENDOR_ID, @ref CONNECTOR_MSG_MAX_TRANSACTION, @ref CONNECTOR_CONNECTION_TYPE,
 * @ref CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND, @ref CONNECTOR_WAN_PHONE_NUMBER_DIALED, @ref CONNECTOR_FIRMWARE_SUPPORT, and @ref CONNECTOR_DATA_SERVICE_SUPPORT.
 *
 * @section RAMRequirements RAM Usage
 * The following dynamic RAM usage measurements were taken using the @ref fw_download_sample by monitoring the
 * high-water mark during the @ref connector_request_id_os_malloc @ref connector_callback_t "application-defined callback".
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Description</th>
 * <th class="title">CONNECTOR_COMPRESSION enabled</th>
 * <th class="title">CONNECTOR_COMPRESSION disabled</th>
 * <th class="title">Notes</th>
 * </tr>
 * <tr>
 * <td>Base usage</td>
 * <td>2540</td>
 * <td>2540</td>
 * <td>Internal state machines, infrastructure and communication buffers.</td>
 * </tr>
 * <tr>
 * <td>With @endhtmlonly @ref CONNECTOR_FIRMWARE_SERVICE @htmlonly enabled</td>
 * <td>104</td>
 * <td>104</td>
 * <td></td>
 * </tr>
 * <tr>
 * <td>With @endhtmlonly @ref CONNECTOR_DATA_SERVICE @htmlonly enabled</td>
 * <td>3376</td>
 * <td>96</td>
 * <td></td>
 * </tr>
 * <tr>
 * <td>Total</td>
 * <td>6020</td>
 * <td>2856</td>
 * <td>With @endhtmlonly @ref CONNECTOR_FIRMWARE_SERVICE and @ref CONNECTOR_DATA_SERVICE @htmlonly enabled. </td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * Two additional measurements with the @ref rci_sample "Simple RCI" and @ref file_system_sample "File System samples"
 * with @ref CONNECTOR_FIRMWARE_SERVICE disabled:
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Description</th>
 * <th class="title">CONNECTOR_COMPRESSION enabled</th>
 * <th class="title">CONNECTOR_COMPRESSION disabled</th>
 * </tr>
 * <tr>
 * <td>With @endhtmlonly @ref CONNECTOR_RCI_SERVICE and @ref CONNECTOR_DATA_SERVICE @htmlonly enabled.</td>
 * <td>9424</td>
 * <td>3408</td>
 * </tr>
 * <tr>
 * <td>With @endhtmlonly @ref CONNECTOR_FILE_SYSTEM and @ref CONNECTOR_DATA_SERVICE @htmlonly enabled.</td>
 * <td>6424</td>
 * <td>2884</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @note All measurements above were taken with @ref CONNECTOR_MSG_MAX_TRANSACTION set to 1
 *
 * @section StackRequirements Program Stack Usage
 * This data was generated using Kubuntu 10.04 and gcc v4.4.3 for 32-bit i486 architecture
 * on a build optimized for size (-Os) with @ref CONNECTOR_RCI_SERVICE, @ref CONNECTOR_DATA_SERVICE,
 * @ref CONNECTOR_FILE_SYSTEM and @ref CONNECTOR_FIRMWARE_SERVICE enabled.
 *
 * This includes all stack usage from within the private Cloud Connector library, but none of the code implemented in the @ref connector_callback_t
 * "application-defined callback".  These metrics should be treated as typical.  It is recommended that the actual program stack size
 * used to call @ref connector_run() or connector_step() be these metrics plus the amount of memory typical for handling a TCP network client,
 * or a thread that accesses local configuration, or makes malloc/free OS calls, whichever is the worst case.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Build Details </th>
 * <th class="title"> Program stack size</th>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_DEBUG @htmlonly disabled</td>
 * <td>1091</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref CONNECTOR_DEBUG @htmlonly enabled</td>
 * <td>1027</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @note We acknowledge the metric for the @ref CONNECTOR_DEBUG enabled versus disabled is unexpectedly
 * lower.  We assume this is due to a compiler optimization.
 *
 *
 *
 * @htmlinclude terminate.html
 */
