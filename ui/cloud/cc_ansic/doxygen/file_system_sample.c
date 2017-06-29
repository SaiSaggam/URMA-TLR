/*! @page file_system_sample File System Sample
 *
 * @htmlinclude nav.html
 *
 * @section fs_sample_overview Overview
 *
 * This sample demonstrates how to read, write, and delete files and list files or directories on your device,
 * using the @ref file_system_overview1 "file system API".
 * <br /><br /> 
 *
 * @section fs_sample_location Source Code Location
 *      - For the connector_run() version:
 *           - @htmlonly<I>/connector/public/run/samples/file_system</I>@endhtmlonly
 *
 *      - For the connector_step() version:
 *          - @htmlonly<I>/connector/public/step/samples/file_system</I>@endhtmlonly
 *
 * @note Check @ref threading "here for more information" on using the connector_run()
 * versus connector_step() model.
 * 
 * <br /> 
 *
 * @section fs_sample_description Code Flow
 *
 * The routine main() in the platform directory initializes Cloud Connector and registers the application callback
 * app_connector_callback() as part of @ref AppStructure "Public Application Framework". The app_connector_callback()
 * is defined in the application.c file. The app_connector_callback() callback calls app_file_system_handler(),
 * defined in the file_system.c file in the platform directory, when a file system request is received.
 *
 * All file system requests are initiated by Device Cloud.
 *
 * The app_file_system_handler() routine calls the following routines to perform different file I/O operations:
 * <br /><br />
 * 
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Routine</th>
 * <th class="title">Operation</th>
 * <th class="title">Request Id</th>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_open()@htmlonly</th><td>@endhtmlonly @ref file_system_open @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_open @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_lseek()@htmlonly</th><td>@endhtmlonly @ref file_system_lseek @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_lseek @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_read()@htmlonly</th><td>@endhtmlonly @ref file_system_read @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_read @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_write()@htmlonly</th><td>@endhtmlonly @ref file_system_write @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_write @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_ftruncate()@htmlonly</th><td>@endhtmlonly @ref file_system_truncate @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_ftruncate @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_close()@htmlonly</th><td>@endhtmlonly @ref file_system_close @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_close @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_remove()@htmlonly</th><td>@endhtmlonly @ref file_system_remove @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_remove @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_opendir()@htmlonly</th><td>@endhtmlonly @ref file_system_opendir @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_opendir @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_readdir()@htmlonly</th><td>@endhtmlonly @ref file_system_readdir @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_readdir @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_closedir()@htmlonly</th><td>@endhtmlonly @ref file_system_closedir @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_closedir @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_stat()@htmlonly</th><td>@endhtmlonly @ref file_system_stat @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_stat @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_stat_dir_entry()@htmlonly</th><td>@endhtmlonly @ref file_system_stat_dir_entry @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_stat_dir_entry @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_hash()@htmlonly</th><td>@endhtmlonly @ref file_system_hash @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_hash @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_get_error()@htmlonly</th><td>@endhtmlonly @ref file_system_get_error @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_get_error @htmlonly</td>
 * </tr>
 * <tr>
 * <th>@endhtmlonly @ref app_process_file_session_error()@htmlonly</th><td>@endhtmlonly @ref file_system_session_error @htmlonly</td><td>@endhtmlonly @ref connector_request_id_file_system_session_error @htmlonly</td>
 * </tr>
 * </table>
 * @endhtmlonly
 * <br /><br />
 * 
 * @section fs_sample_hash_support File Hash Values Support
 *
 * By default the file system sample does not support file hash values. MD5 support can be enabled using the following define:
 *
 * -DAPP_ENABLE_MD5=true
 *
 * When MD5 support is enabled the sample uses MD5_Init(), MD5_Update(), and MD5_Final() functions
 * from the openssl library.
 * <br /><br />
 * 
 * @section running Running
 *
 * Once successfully built, run the executable, in Linux type ./connector to run the
 * sample.
 *
 * File system transfers can be initiated by Device Cloud using <a href="http://www.etherios.com/devicecloud/devicemanager">Device Management</a>
 * or by issuing a Web Services call.  See @ref fs_sample_python_script "here" for more information on
 * web service calls.
 * <br /><br />
 *
 * @subsection fs_gui Running file system using Device Management
 *
 * Log on to Device Cloud at http://login.etherios.com/ and click on your device
 * (described in the @ref connector_login "Getting Started Section").
 * Once you are logged to see your device, click the Refresh button.
 * The device's status should indicate as 'Connected'.
 *
 * @image html cloud_device_fs_connected.png
 * <br />
 * 
 * Select device and double click the device.
 * 
 * @image html cloud_device_fs_menu.png
 * <br />
 * 
 * Double click "File Management".
 * 
 * @image html cloud_device_fs_browse.png
 * <br />
 * 
 * You see the listinf of "/" folder on the right. You can browse folders on your device, download, upload and delete.
 * You can't create or remove
 * file folders.
 * <br /><br />
 * 
 * @section fs_sample_python_script Using a Web Services client to write, read, and list a file
 *
 * The sample provides a very basic Web Services script file_system.py, written in python, to
 * write, read, and list a file, using @htmlonly <a href="web_services.html">Web Services</a> @endhtmlonly.
 *
 * The script sends a request to login.etherios.com.  The file "test_file.txt" is created in the
 * current directory on the device with the file data "Connector file system sample\n".
 *
 * To execute file_system.py python script:
 *
 * @code
 * python file_system.py <Username> <Password> <Device ID> [<Device Cloud URL>]
 * @endcode
 *
 *
 * The part of the python script which contains file system commands looks like:
 *
 * @code
 *
 *  expected_content = "Connector file system sample\n"
 *  put_data = base64.encodestring(expected_content)[:-1]
 *
 *  # file system message to send to Device Cloud
 *  message = """<sci_request version="1.0">
 *      <file_system>
 *          <targets>
 *              <device id="%s"/>
 *          </targets>
 *          <commands>
 *              <put_file path="test_file.txt" offset = "0" truncate = "true">
 *                <data>%s</data>
 *              </put_file>
 *              <get_file path="test_file.txt" offset = "0" length = "4294967294"/>
 *              <ls path="test_file.txt"/>
 *          </commands>
 *      </file_system>
 *  </sci_request>
 *  """ % (device_id, put_data, ls_path)
 *
 * @endcode
 * <br />
 *
 * @section fs_sample_put_file The put_file command
 *
 * The put_file command initiates a file system session, which writes a file on the device.
 * Cloud Connector will invoke the following application callbacks:
 *
 *  -# @ref file_system_open "app_process_file_open()" is called with the "test_file.txt" path and
 *          @ref CONNECTOR_FILE_O_WRONLY | @ref CONNECTOR_FILE_O_CREAT oflag
 *  -# @ref file_system_write "app_process_file_write()" is called with the file descriptor and file data
 *  -# @ref file_system_close "app_process_file_close()" is called with the file descriptor
 *
 * The put_file command could be used to write part of the file.
 * If the put_file command is issued with non-zero offset, Cloud Connector then would call
 * @ref file_system_lseek "app_process_file_lseek()" callback to set the file position.
 * <br /><br />
 * 
 * @section fs_sample_get_file The get_file command
 *
 * The get_file command initiates a file system session, which reads a file and sends file data
 * to Device Cloud. Cloud Connector will invoke the following application callbacks:
 *
 *  -# @ref file_system_open "app_process_file_open()" is called with the "test_file.txt" path and
 *          @ref CONNECTOR_FILE_O_RDONLY oflag
 *  -# @ref file_system_read "app_process_file_read()" is called with the file descriptor and pointer to the data buffer
 *  -# @ref file_system_close "app_process_file_close()" is called with the file descriptor
 *
 * The get_file command could be used to read part of the file. If the get_file command is issued with non-zero offset,
 * Cloud Connector then would call @ref file_system_lseek "app_process_file_lseek()" callback to set the file position.
 * <br /><br />
 * 
 * @section fs_sample_ls_file The ls command
 *
 * The ls command initiates a file system session, which lists a file or directory.
 * In this example Cloud Connector will invoke the following application callback:
 *
 *  @ref file_system_stat "app_process_file_stat()" is called with the "test_file.txt" path and
 *          @ref connector_file_system_hash_none hash_algorithm.requested.
 *
 * The directory path could be used instead of the file path to list a directory. The following command
 * can be used to list a current directory and use MD5 hash algorithm:
 *
 * @code
 *
 *      <ls path="./" hash = "md5"/>
 *
 * @endcode
 * <br />
 *
 * Cloud Connector will invoke the following application callbacks:
 *
 *  -# @ref file_system_stat "app_process_file_stat()" called with the "./" path and
 *          @ref connector_file_system_hash_md5 hash_algorithm.requested
 *  -# @ref file_system_opendir "app_process_file_opendir()" called with the "./" path
 *  -# For each entry in the directory:
 *      -# @ref file_system_readdir "app_process_file_readdir()", called with directory handle, returned in
 *              @ref file_system_opendir "app_process_file_opendir()"
 *      -# @ref file_system_stat "app_process_file_stat_dir_entry()" is called with an entry path
 *      -# @ref file_system_hash "app_process_file_hash()" is called with an entry path and @ref connector_file_system_hash_md5 hash_algorithm
 *  -# @ref file_system_closedir "app_process_file_closedir()" is called with directory handle
 *
 * The file_system.c sample uses the re-entrant function readdir_r() to read the next directory entry.
 * @ref file_system_opendir "app_process_file_opendir()" allocates memory to hold
 * a directory entry and a pointer to a directory stream and returns this pointer in the response_data->handle.
 * The @ref file_system_closedir "app_process_file_closedir()" callback frees this memory.
 *
 * The memory buffer, used for MD5 calculations for each file in the directory, is allocated in the
 * @ref file_system_stat "app_process_file_stat()" function called with the directory path "./" and then used
 * for each file in this directory. The pointer to the memory buffer is passed between callbacks in user_context.
 * The @ref file_system_closedir "app_process_file_closedir()" callback frees the memory buffer.
 * <br /><br />
 * 
 * @section fs_sample_rm_file The rm command
 *
 * The sample python script does not remove the file. The following command could be added to remove test_file.txt:
 *
 * @code
 *
 *    <rm path="test_file.txt"/>
 *
 * @endcode
 *
 * Cloud Connector will invoke the @ref file_system_remove "app_process_file_remove()" callback.
 * <br /><br />
 * 
 * @section fs_sample_output Sample output
 *
 * Application output is:
 *
 * @code
 *
 * Open test_file.txt, 577, returned 4
 * write 4, 25, returned 25
 * close 4 returned 0
 * Open test_file.txt, 0, returned 4
 * read 4, 496, returned 25
 * read 4, 471, returned 0
 * close 4 returned 0
 * stat for test_file.txt returned 0, filesize 25
 *
 * @endcode
 *
 * Output from the file_system.py is:
 *
 * @code
 *
 * Response:
 * <sci_reply version="1.0">
 *    <file_system>
 *       <device id="00000000-00000000-00000000-00000000">
 *          <commands>
 *             <put_file/>
 *             <get_file><data>aURpZ2kgZmlsZSBzeXN0ZW0gc2FtcGxlCg==</data></get_file>
 *             <ls hash="none"><file path="test_file.txt" last_modified="1334009380" size="25"/></ls>
 *          </commands>
 *       </device>
 *    </file_system>
 * </sci_reply>
 *
 * @endcode
 * <br />
 * 
 * @section fs_sample_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/file_system
 * directory and type: @htmlonly"<I>make</I>"@endhtmlonly.
 * If you are not using Linux you will need to update your environment with
 * the information below and then build the image for your platform.
 *
 * @note To build this example for Linux with MD5 checksums,
 * type: @htmlonly"<I>make APP_ENABLE_MD5=true</I>"@endhtmlonly.
 * 
 * <br />
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
 * <td>samples/file_system</td>
 * </tr>
 * <tr>
 *  <th>status.c</th>
 *  <td>Status calls</td>
 *  <td>samples/file_system</td>
 * </tr>
 * <tr>
 *  <th>file_system.py</th>
 *  <td>Python script to write, read, and list a file</td>
 *  <td>samples/file_system</td>
 * </tr>
 * <tr>
 *   <th>connector_config.h</th>
 *   <td> Cloud Connector options</td>
 *   <td>samples/file_system</td>
 * </tr>
 * <tr>
 * <th>connector_api.c</th>
 * <td>Code for Cloud Connector </td>
 * <td>private</td>
 * </tr>
 * <tr>
 *  <th>file_system.c</th>
 *  <td>Application callbacks for file system</td>
 *  <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *  <th>os.c</th>
 *  <td>Operating system calls</td>
 *  <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *  <th>network_dns.c</th>
 *  <td>DNS routines for Network interface</td>
 *  <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *  <th>network_tcp.c</th>
 *  <td>Network interface callbacks for TCP</td>
 *  <td>platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 *  <th>config.c</th>
 *  <td>Configuration routines</td>
 *  <td>platforms/<i>my_platform</i></td>
 *  </tr>
 * <tr>
 *  <th>main.o</th>
 *  <td>Starting point of program, dependent on build environment</td>
 *  <td>platforms/<i>my_platform</i></td>
 * </tr>
 * </table>
 * @endhtmlonly
 * <br /><br />
 * 
 * @subsection include_paths Include Paths
 *
 * The following include paths are required:
 *
 * @li public/include
 * @li public/include/custom
 * @li run/platforms/@a my_platform
 *
 * @htmlinclude terminate.html
 */
