/*! @page firmware_download Firmware Download
 *
 * @htmlinclude nav.html
 *
 * @section fw_overview1 Firmware Download
 *
 *  -# @ref fw_overview
 *  -# @ref fw_num_targets
 *  -# @ref fw_info
 *  -# @ref fw_download
 *  -# @ref fw_image_data
 *  -# @ref fw_complete
 *  -# @ref fw_abort
 *  -# @ref fw_reset
 *
 * @section fw_overview Overview
 *
 * The firmware access facility is an optional facility for applications to update their
 * firmware. Cloud Connector invokes the application-defined callback to query whether this
 * firmware access facility is supported or not.  The firmware access facility uses a target
 * number to distinguish application-dependent firmware images.  For example, a
 * stand-alone bootloader may be loaded separate from an application image, or a default file system
 * could be maintained as well.
 * Applications define an image to each target except target 0 (target 0 must be the firmware
 * image that is running Cloud Connector). Only one firmware upgrade can be in progress at any given
 * time. Cloud Connector will send a firmware target list to Device Cloud to identify the number
 * of target applications and the version number of each target.
 *
 * The firmware portion of the application-defined callback is used for the following:
 *  -# To obtain the number of firmware downloadable images.
 *  -# To obtain firmware image information and descriptions.
 *  -# To process firmware image upgrades.
 *  -# To reset firmware images.
 *
 * A typical application-defined callback sequence for downloading a firmware image would include:
 *  -# Cloud Connector calls application-defined callback to return firmware information which
 *     includes the firmware version number, maximum size of an image, firmware description, and file name spec.
 *  -# Cloud Connector calls application-defined callback to initiate firmware download.
 *  -# Cloud Connector calls application-defined callback for firmware binary data.
 *  -# Repeat step 3 until the entire firmware image data is completed.
 *  -# calls application-defined callback to complete firmware download.
 *  -# calls application-defined callback to reset and reboot the target to begin executing the new firmware.
 *  -# Cloud Connector calls application-defined callback to disconnect the current connection
 *
 * Applications may choose to begin writing their downloaded image to Flash either
 * during the Firmware Binary or Firmware Download Complete message. This is an application
 * level decision with clear tradeoffs (memory needs versus power loss protection).
 * If you write to Flash while the Firmware Binary data is being received, you could lose communication
 * or power at some later time.  If protection is built in (i.e., back up images) this would not be
 * problem. If protections are not built in, applications might choose to place the entire
 * image contents into RAM before writing to Flash. Back up image protection would still
 * protect against power loss after your write occurred, but the communication loss or corruption
 * could be avoided by having a checksum test before starting your Flash write.
 *
 * @note See @ref firmware_support under Configuration to enable or
 * disable firmware download.
 *
 * @section fw_num_targets Number of Firmware Targets
 *
 * Return the number of targets for firmware download.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>class_id</td>
 * <td>@endhtmlonly @ref connector_class_id_firmware @htmlonly</td>
 * </tr>
 * <tr>
 * <td>request_id</td>
 * <td>@endhtmlonly @ref connector_request_id_firmware_target_count @htmlonly</td>
 * </tr>
 * <tr>
 * <td>data</td>
 * <td>Pointer to @endhtmlonly connector_firmware_count_t @htmlonly:
 *     <dl><dt>count</dt><dd>Callback writes number of supported targets.</dd></dl>
 * </td>
 * </tr>
 * <tr>
* <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Successfully returned the number of targets supported</td>
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
 *     if (class_id == connector_class_id_firmware && request_id.firmware_request == connector_request_id_firmware_target_count)
 *     {
 *          connector_firmware_count_t * const config_firmware = data;
 *          // return total number of firmware update targets
 *          config_firmware->count = firmware_list_count;
 *     }
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @section fw_info Firmware Information of Firmware Target
 *
 * Return the firmware information of the target.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>class_id</td>
 * <td>@endhtmlonly @ref connector_class_id_firmware @htmlonly</td>
 * </tr>
 * <tr>
 * <td>request_id</td>
 * <td>@endhtmlonly @ref connector_request_id_firmware_info @htmlonly</td>
 * </tr>
 * <tr>
 * <td>data</td>
 * <td>Pointer to @endhtmlonly connector_firmware_info_t @htmlonly:
 *     <dl><dt>verion</dt>
 *         <dd> Callback writes the version number of the target:
 *             <ul><li><b>major</b> - Major version number.</li>
 *                 <li><b>minor</b> - Minor version number.</li>
 *                 <li><b>revision</b> - Revision number.</li>
 *                 <li><b>build</b> - Build number.</li></ul>
 *         </dd>
 *
 *         <dt>description</dt><dd>Callback writes an ASCII description string of the target.</dd>
 *         <dt>filespec</dt>
 *         <dd>Callback writes a regular expression for the firmware image name of the target.
 *             No file name spec can be returned if only a single target (target 0) is supported.
 *             If an application wants to match the file name "image.bin", it must return "image\.bin" for
 *             regular expression. Regular expressions are case sensitive. If file names for the given
 *             target are to be case insensitive, it must be specified as case insensitive in a regular
 *             expression such as: "[iI][mM][aA][gG][eE]\.[bB][iI][nN]".
 *             Note: The maximum length of the firmware description and file name spec combined is 127 bytes.</dd>
 *
 *         <dt>target_number</dt><dd>Contains the target number which target the information is requested.</dd>
 *     </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returned firmware information</td>
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
 *     if (class_id == connector_class_id_firmware && request_id.firmware_request == connector_request_id_firmware_info)
 *     {
 *          connector_firmware_info_t * config = data;
 *          config->version.major = firmware_list[config->target].version.major;
 *          config->version.minor = firmware_list[config->target].version.minor;
 *          config->version.revision = firmware_list[config->target].version.revision;
 *          config->version.build = firmware_list[config->target].version.build;
 *          config->description = firmware_list[config->target].description;
 *          config->filespec = firmware_list[config->target].filespec;
 *     }
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @section fw_download File Firmware Download Request
 *
 * Callback is called to start firmware download when Cloud Connector receives a firmware download request message.
 * Cloud Connector parses the information in the firmware download request and passes the information to the callback:
 *  -# A target number which target the firmware is intended for.
 *  -# The name of the file to be sent.
 *  -# The size of the code that is ready to be sent to the target. 
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>class_id</td>
 * <td>@endhtmlonly @ref connector_class_id_firmware @htmlonly</td>
 * </tr>
 * <tr>
 * <td>request_id</td>
 * <td>@endhtmlonly @ref connector_request_id_firmware_download_start @htmlonly</td>
 * </tr>
 * <tr>
 * <td>data</td>
 * <td>Pointer to @endhtmlonly connector_firmware_download_start_t @htmlonly
 *     <dl><dt>target_number</dt><dd>Contains the target number which target the firmware is intended for.</dd>
 *         <dt>filename</dt><dd>Contain a pointer to file name to be downloaded.</dd>
 *         <dt>code_size</dt><dd>Size of the code that is ready to be sent to the target.</dd>
 *         <dt>status</dt><dd>Callback writes  @endhtmlonly @ref connector_firmware_status_t @htmlonly status when error is encountered</dd>
 *     </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully set appropriate status</td>
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
 *     if (class_id == connector_class_id_firmware && request_id.firmware_request == connector_request_id_firmware_download_start)
 *     {
 *          connector_firmware_download_start_t  * const firmware_download = data;
 *
 *          if (firmware_download_started)
 *          {   // already started
 *              firmware_download->status = connector_firmware_status_download_denied;
 *              return connector_callback_continue;
 *          }
 *
 *          APP_DEBUG("target = %d\n",         download_info->target_number);
 *          APP_DEBUG("filename = %s\n",       download_info->filename);
 *          APP_DEBUG("code size = %d\n",      download_info->code_size);
 *          // initialize & prepare for firmware update
 *          total_image_size = 0;
 *          firmware_download_started = 1;
  *     }
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 *
 * @section fw_image_data Binary Image Data for Firmware Download
 *
 * Callback is called to process image data when Cloud Connector receives a firmware binary block
 * message. Cloud Connector calls this callback for each block of data received from Device Cloud.
 * Cloud Connector parses the information in the firmware binary block and passes the information to the callback:
 *  -# 4-octet offset that determines where the block of binary data fits into the download.
 *  -# Binary data.
 *  -# Length of the binary data.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>class_id</td>
 * <td>@endhtmlonly @ref connector_class_id_firmware @htmlonly</td>
 * </tr>
 * <tr>
 * <td>request_id</td>
 * <td>@endhtmlonly @ref connector_request_id_firmware_download_data @htmlonly</td>
 * </tr>
 * <tr>
 * <td>data</td>
 * <td>Pointer to @endhtmlonly connector_firmware_download_data_t @htmlonly
 *     <dl><dt>target_number</dt><dd>Contains the target number which target the firmware data is intended for.</dd>
 *         <dt>offset</dt><dd>Contain an offset value where this data block of image fits into the download.</dd>
 *         <dt>data</dt><dd>Contain a pointer to downloaded image data.</dd>
 *         <dt>bytes_used</dt><dd>Contain number of bytes of the image data.</dd>
 *         <dt>status</dt><dd>Callback writes  @endhtmlonly @ref connector_firmware_status_t @htmlonly status when error is encountered.</dd>
 *     </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully returns appropriate status</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback is busy and will be called again with same binary image data</td>
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
 * connector_callback_status_t app_connector_callback(connector_class_id_t const class_id, c
 *                                                    connector_request_id_t const request_id
 *                                                    void * const data)
 * {
 *
 *     if (class_id == connector_class_id_firmware && request_id.firmware_request == connector_request_id_firmware_download_data)
 *     {
 *          connector_firmware_download_data_t * const image_data = data;
 *
 *          fwStoreImage(image_data->target_number, image_data->data, image_data->bytes_used, image_data->offset);
 *          image_data->status = connector_fw_success;
 *     }
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @section fw_complete Firmware Download Complete
 *
 * Callback is called when Device Cloud is done sending all image data. This callback tells Cloud Connector
 * when target has been completely updated.
 * If this callback returns:
 *  -# BUSY status indicating the firmware download is still in process, Cloud Connector will
 * call this callback again. This usually indicates that image data is still being written onto flash.
 *  -# CONTINUE status indicating the firmware download has been successfully completed, Cloud Connector will
 * send a firmware download complete response to Device Cloud.
 *  -# ABORT status, Cloud Connector will send a firmware download abort message to Device Cloud, stop and exit with error status.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>class_id</td>
 * <td>@endhtmlonly @ref connector_class_id_firmware @htmlonly</td>
 * </tr>
 * <tr>
 * <td>request_id</td>
 * <td>@endhtmlonly @ref connector_request_id_firmware_download_complete @htmlonly</td>
 * </tr>
 * <tr>
 * <td>data</td>
 * <td>Pointer to @endhtmlonly connector_firmware_download_complete_t @htmlonly
 *     <dl><dt>target_number</dt><dd>Contains the target number which target the firmware download is completed.</dd>
 *         <dt>status</dt><dd>Callback writes  @endhtmlonly @ref connector_firmware_download_status_t @htmlonly status.</dd>
 *     </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback successfully set appropriate status in response_data</td>
 * </tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_busy @htmlonly</td>
 * <td>Callback is busy and will be called again</td>
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
 *     if (class_id == connector_class_id_firmware && request_id.firmware_request == connector_request_id_firmware_download_complete)
 *     {
 *         connector_firmware_download_complete_t * const firmware_complete = data;
 *
 *         firmware_complete->status = connector_fw_download_success;
 *         fwCloseImage(firmware_complete->target_number, firmware_complete->status);
 *         firmware_download_started = 0;
 *     }
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @section fw_abort Firmware Download Abort
 *
 * Called to abort firmware download when Cloud Connector receives a firmware download abort message. *
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>class_id</td>
 * <td>@endhtmlonly @ref connector_class_id_firmware @htmlonly</td>
 * </tr>
 * <tr>
 * <td>request_id</td>
 * <td>@endhtmlonly @ref connector_request_id_firmware_download_abort @htmlonly</td>
 * </tr>
 * <tr>
 * <td>data</td>
 * <td>Pointer to @endhtmlonly connector_firmware_download_abort_t @htmlonly
 *     <dl><dt>target_number</dt><dd>Contains the target number which target the firmware download is aborted.</dd>
 *         <dt>status</dt><dd>Contains the abort reason or status. See @endhtmlonly @ref connector_firmware_status_t @htmlonly.</dd>
 *     </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback acknowledged that the download process is aborted</td>
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
 *     if (class_id == connector_class_id_firmware && request_id.firmware_request == connector_request_id_firmware_abort)
 *     {
 *         connector_firmware_download_abort_t * const abort_data = data;
 *         fwCloseImage(abort_data->target_number, abort_data->status);
 *         firmware_download_started = 0;
 *     }
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @section fw_reset Firmware Target Reset
 *
 * Callback is called when Device Cloud resets target. The callback should not return if
 * it's resetting itself. It may return and continue. However, Device Cloud may disconnect the device.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr> <th colspan="2" class="title">Arguments</th> </tr>
 * <tr><th class="subtitle">Name</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>class_id</td>
 * <td>@endhtmlonly @ref connector_class_id_firmware @htmlonly</td>
 * </tr>
 * <tr>
 * <td>request_id</td>
 * <td>@endhtmlonly @ref connector_request_id_firmware_target_reset @htmlonly</td>
 * </tr>
 * <tr>
 * <td>data</td>
 * <td>Pointer to @endhtmlonly connector_firmware_reset_t @htmlonly:
 *     <dl><dt>target_number</dt><dd>Contains the target number which target to reset.</dd>
 *     </dl>
 * </td>
 * </tr>
 * <tr> <th colspan="2" class="title">Return Values</th> </tr>
 * <tr><th class="subtitle">Values</th> <th class="subtitle">Description</th></tr>
 * <tr>
 * <td>@endhtmlonly @ref connector_callback_continue @htmlonly</td>
 * <td>Callback acknowledged that Device Cloud reset the target</td>
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
 *     if (class_id == connector_class_id_firmware && request_id.firmware_request == connector_request_id_firmware_target_reset)
 *     {
 *         connector_firmware_reset_t * download_firmware = data;
 *         fwResetImage(download_firmware->target_number);
 *     }
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @htmlinclude terminate.html
 */
