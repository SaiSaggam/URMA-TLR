/*! @page getting_started Getting Started
 *
 * @htmlinclude nav.html
 *
 * @section table_of_contents_porting Getting Started
 *
 * The Getting Started process will walk you through the steps necessary to get
 * Cloud Connector integrated into your software development environment and running
 * applications that include a Device Cloud connection to Device Cloud.
 *
 * These Getting Started Process includes the following steps:
 *          -# @ref step1
 *              -# @ref i_have_C89_and_64bit
 *              -# @ref non_compliant_adjustments
 *                  -# @ref data_type_format_etc
 *                  -# @ref debug_routine
 *                  -# @ref snprintf_routine
 *          -# @ref step2
 *          -# @ref step3
 *              -# @ref connector_initialization
 *              -# @ref add_files
 *              -# @ref add_path
 *              -# @ref build_sample
 *              -# @ref resolving_compilation_issues
 *                  -# @ref errors_due_to_C89_and_stdint
 *                  -# @ref warnings_due_to_padding
 *          -# @ref step4
 *              -# @ref connector_login
 *              -# @ref connector_vendor_id
 *          -# @ref step5
 *              -# @ref os_routines
 *              -# @ref network_routines
 *              -# @ref configuration_routines
 *              -# @ref application_start
 *          -# @ref step6
 *              -# @ref add_c_files
 *              -# @ref add_include_paths
 *          -# @ref step7
 *          -# @ref step8
 *              -# @ref good_results_output
 *              -# @ref bad_results_output
 *              -# @ref add_your_device_to_the_cloud
 *              -# @ref view_result_on_cloud
 *          -# @ref step9
 *
 * Two sample applications, @ref step3 "compile_and_link" and @ref step7 "connect_to_device_cloud", will
 * be used to confirm your compilation and port, respectively.  When complete, your
 * device will be connected to Device Cloud and displayed in the
 * <a href="http://www.etherios.com/devicecloud/devicemanager">Device Management</a>.
 *
 * @section step1 Do you have a C89 or C99 compliant compiler?
 *
 * The Cloud Connector software is ANSI X3.159-1989 (ANSI C89) or ISO/IEC
 * 9899:1999 (ANSI C99) compliant.  If your compiler is C89 or C99 compliant and you
 * are running on a 32-bit processor you can skip to @ref step2 "Step 2".
 *
 * @subsection i_have_C89_and_64bit Are you trying to compile C89 on a 64-bit machine?
 *
 * If your target is a 64-bit machine and your compiler is C89 compliant, this configuration 
 * @htmlonly<B>is not supported!</B>@endhtmlonly
 *
 * You will either have to find a C99 compiler for your target or you will have
 * to make updates and adjustments as outlined in the following steps.
 *
 * @note This combination should be rare, given most 64-bit processors were unavailable
 * prior to 1999.  We expect 64-bit targets to have C99 support.
 *
 * @subsection non_compliant_adjustments What to do about non C89 or C99 compliant compilers
 *
 * Updates and adjustments will be necessary to properly compile the Cloud Connector software with
 * your compiler and tool chain.  To begin these updates, complete the following steps:
 *
 * @subsection data_type_format_etc Updates to connector_types.h
 *
 * You will have to review (and edit) @ref connector_types.h "public/include/custom/connector_types.h"
 * to update the standard ANSI data types, minimums, maximum, and standard format specifiers.
 *
 * -# Data types:
 *  @li uint8_t
 *  @li uint16_t
 *  @li uint32_t
 *
 * -# Minimum and maximum values:
 *  @li UINT16_MAX
 *  @li INT32_MIN
 *  @li INT32_MAX
 *  @li UINT32_MAX
 *  @li SIZE_MAX
 *
 * -# Format specifiers:
 *  @li SCNd32
 *  @li SCNu32
 *  @li SCNx32
 *  @li PRId32
 *  @li PRIu32
 *  @li PRIx32
 *
 * @note These are defined for C99 or typedefed/defined for C89.
 *
 * @subsection debug_routine Implement the debug routine and macro
 *
 * Open the file debug.c and implement the @ref connector_debug_vprintf.  For the linux
 * platform the file is located at public/run/platforms/linux/debug.c.  Click on the
 * routine to see a description, then modify to operate with your platform.  Cloud Connector
 * calls this routine to display debug information when @ref CONNECTOR_DEBUG is defined connector_config.h.
 *
 * The file public/include/custom/connector_debug.h implements the macros listed below, which
 * are also used to debug your port.
 *
 *  @li @ref ASSERT
 *
 * Review these definitions and modify these for your platform:
 *
 * If you don't have vprintf or assert available redefine these macros to call corresponding
 * routines for your platform.
 *
 * @subsection snprintf_routine Implement snprintf routine
 *
 * The function @ref connector_snprintf() is located in @ref os.c and used to build formatted
 * output strings with the protection of a maximum string length (preventing erroneous
 * buffer overwrites).  This maps one-to-one to C99 vsnprintf().
 *
 * For the linux platform, the function is fully implemented for either C99 or C89
 * (using vsprintf() and memcpy()).  If these functions are unavailable on your platform, we
 * recommend you look at our C89 implementation and mimic.
 *
 *
 * @section step2 Modify connector_config.h
 *
 * Open the file connector_config.h in the sample directory to configure processor endianess.
 *
 * The Cloud Connector default is little endian.  To reconfigure for big endian,
 * support comment out the @ref CONNECTOR_LITTLE_ENDIAN define.
 *
 * @section step3 Build the compile_and_link sample
 *
 * In this step we are going to build the compile_and_link sample.
 * This sample will verify that your build environment is set up correctly.
 * You need to add the source files and include paths to your build system listed
 * below.
 *
 * @subsection connector_initialization Cloud Connector Initialization
 *
 * The routine connector_init() is called to initialize Cloud Connector,
 * currently this is called from the routine main() in main.c; you will need to call
 * connector_init() at the initialization point for your application with an
 * application callback.
 *
 * For the compile_and_link sample, since this sample tests compilation and link and
 * not intended for execution, you can call connector_init() as:
 *
 * @code
 * // Call connector_init() with a NULL callback to confirm your build environment only
 * connector_init((connector_callback_t)0);
 * @endcode
 *
 * If you are not using main() you will need to add in the call to connector_init()
 * where appropriate.
 *
 * @subsection add_files Add the source files to your build system
 * The following is a list of files required to build.  There is a sample
 * linux Makefile provided (public/run/samples/compile_and_link/Makefile) which you
 * can use as a reference.  Add the following files to your make/build system.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Name</th>
 * <th class="title">Description</th>
 * <th class="title">Location</th>
 * </tr>
 * <tr>
 * <td>connector_api.c</td>
 * <td>Contains the Cloud Connector source code</td>
 * <td>private</td>
 * </tr>
 * <tr>
 * <td>main.c</td>
 * <td>Program entry point, calls @endhtmlonly connector_init() @htmlonly</td>
 * <td>public/run/samples/compile_and_link</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection add_path Add the include paths
 * The following is a list of include paths to add to your system:
 *
 * @li public/include
 * @li public/include/custom
 *
 * @subsection build_sample Build the sample
 *
 * Now that you have the build environment setup, verify the compilation and link process.
 * If using the Makefile provided, type @htmlonly"<I>make</I>"@endhtmlonly in your command shell from
 * the compile_and_link directory, otherwise perform a build according to your environment's
 * guidelines.
 *
 * @note See the @ref language for compilation tool requirements.
 *
 * @subsection resolving_compilation_issues Addressing compilation problems
 *
 * The Linux files included in the contain very strict warnings and error checking.
 * This is done to expose as many issues as possible before executing code.   In addition,
 * their is a clear guideline for @ref language "C Compiler language support".
 *
 * If you are experiencing problems building Cloud Connector software, first
 * double check the steps listed in the prior instructions.
 *
 * @subsection errors_due_to_C89_and_stdint C89 stdint.h compilation errors
 *
 * Some C89 compilers include elements that were eventually released in C99, in particular,
 * stdint.h.  For these hybrid compilers, the compilation phase will result in errors:
 *
 * @code
 * In file included from ../../../include/connector_api.h:66,
 *                  from ../../platforms/linux/config.c:20:
 * ../../../include/connector_types.h:83: error: conflicting types for uint32_t
 * /usr/include/stdint.h:52: note: previous declaration of uint32_t was here
 * : *** [../../platforms/linux/config.o] Error 1
 * @endcode
  *
 * These compilation errors can be resolved by defining  @ref CONNECTOR_HAS_STDINT_HEADER
 * in your  or build system.
 *
 * @note When updating your file, since it's not a dependency, you must perform a
 * @htmlonly"<I> clean all</I>"@endhtmlonly when rebuilding your application.
 *
 * @subsection warnings_due_to_padding Warnings due to "alignment boundary structure padding"
 *
 * The Linux makefiles that come with the package include very strict warnings and error checking.
 * Many processors, like ARM, have strict guidelines for word boundaries.  For these processors,
 * padding is required at the end of structure to prevent mis-aligned structures in array processing.
 *
 * @verbatim
 * In file included from ../../../../private/connector_def.h:140,
 *                  from ../../../../private/connector_api.c:27:
 * ../../../../private/connector_edp_def.h:129: warning: padding struct size to alignment boundary
 * ../../../../private/connector_edp_def.h:167: warning: padding struct size to alignment boundary
 * ../../../../private/connector_edp_def.h:204: warning: padding struct size to alignment boundary
 * ../../../../private/connector_edp_def.h:211: warning: padding struct size to alignment boundary
 * In file included from ../../../../private/connector_def.h:144,
 *                  from ../../../../private/connector_api.c:27:
 * ../../../../private/connector_sm_def.h:174: warning: padding struct size to alignment boundary
 * In file included from ../../../../private/connector_edp.h:23,
 *                  from ../../../../private/connector_api.c:44:
 * @endverbatim
 *
 * These warnings are perfectly safe.  The structures have extended to allow word alignment.
 * In general, you must review all warnings to confirm their safety.
 *
 * Once the build is successful you can proceed to the next step.
 *
 * @note Do not execute the compile_and_link sample, as this only verifies that the build environment
 * is set up correctly. The application will exit on a failed ASSERT when run.
 *
 * @section step4 Setup your Device Cloud Account
 *
 * @subsection connector_account Create a Device Cloud Account
 * Before getting started you will need to create a Device Cloud account and obtain a
 * unique Device Cloud Vendor ID (which is a unique identifier for your company).
 *
 * Later in this process, when you get the connect_to_device_cloud sample
 * @ref good_results_output "running successfully", you will be
 * @ref add_your_device_to_the_cloud "instructed to register your Device Cloud Device ID.
 *
 * To create an account, navigate to
 * https://myaccount.etherios.com/ and follow the instructions to create and register your
 * Device Cloud account.
 *
 * @note If you have already created and registered a Device Cloud Account, navigate here
 * https://login.etherios.com/ and follow the instructions in the next step.
 *
 * @image html cloud_registration.png
 *
 * @subsection connector_login Login to Device Cloud
 * 1. Now login to Device Cloud using your account credential's
 *
 * @image html cloud_login.png
 *
 * @subsection connector_vendor_id Obtain a Device Cloud Vendor ID
 * 2. If you are a first time Device Cloud developer, you will need to obtain a Device Cloud Vendor ID.  To
 * obtain a Device Cloud Vendor ID, select the Register for new vendor id button on the bottom of
 * the @b My @b Account section under @b Administration tab.
 *
 * @image html cloud_vendor_id.png
 *
 * After selecting the register for a Vendor ID button, the page will refresh and your unique vendor ID number
 * will be displayed in place of the button.  Record the Vendor ID, you will need it later.
 *
 * @note If you already have a vendor ID, it will be displayed instead of the button.
 *
 * @section step5 Porting your platform for the connect_to_device_cloud sample
 *
 * The @b connect_to_device_cloud sample validates the most fundamental Cloud Connector
 * porting aspects.  If you can successfully maintain a Device Cloud connection, all other
 * Cloud Connector functions (like @ref send_data "sending data" or @ref firmware_download "firmware download")
 * should work flawlessly.
 *
 * In the platforms directory you will see the linux platform.  This platform was included
 * since it's a well known operating system and has open source documentation readily
 * available on the web.  If you're using linux, you begin application development
 * by @ref application_start "skipping to the next step".
 *
 * Otherwise, you should cut and paste the linux platform and port as described in the
 * following sub sections.
 *
 * @note In this guide we use the public/run/platforms/linux as an example, but all
 * platforms should have the same structure.
  *
 * @subsection os_routines OS Routines
 * Open the file os.c and implement the routines listed below.  For the linux platform
 * the file is located at public/run/platforms/@a my_platform/os.c.  Click on the routine
 * to see a description, then modify to operate with your platform.  These routines
 * are callbacks which are described in the API section.
 *
 *  @li app_os_malloc()
 *  @li app_os_free()
 *  @li app_os_get_system_time()
 *  @li app_os_reboot()
 *  @li app_os_yield()
 *
 * @subsection network_routines Network Routines
 * Open the file network_tcp.c and implement the network interface routines.
 * The network interface routines provided in the Linux platform are implemented using standard
 * Berkeley socket calls and can be used as a reference for your platform.
 * You may have to modify the routines in this file based on your
 * platforms network implementation.
 *
 * The following is a list of network interface routines which must be implemented:
 *  @li app_network_tcp_open()
 *  @li app_network_tcp_send()
 *  @li app_network_tcp_receive()
 *  @li app_network_tcp_close()
 *
 * @subsection configuration_routines Configuration Routines
 *
 * In this step we set up the configuration routines for your platform. Each configuration
 * item has a corresponding get routine for retrieving its value. Open the file config.c
 * for your platform and implement the configuration routines.
 * All configuration routines are passed in a pointer to a pointer, along with a pointer to the size.
 * The address of the configuration item is stored in the pointer passed in, and the size is assigned to the pointer to the size variable.
 * For example, app_get_vendor_id() is the routine used to retrieve your Device Cloud Vendor ID
 * and is shown below.
 * @code
 * static connector_callback_status_t app_get_vendor_id(connector_config_vendor_id_t * const config_vendor)
 * {
 * #error  "Specify vendor id"
 *
 *     static uint32_t const device_vendor_id = 0x00000000;
 *
 *     config_vendor->id  =  device_vendor_id;
 *
 *     return connector_callback_continue;
 * }
 * @endcode
 *
 * @note The required routines contain the @htmlonly #error @endhtmlonly preprocessor directive
 * which must be removed before compiling.
 *
 * @note The memory assigned to configuration items must be @b statically @b allocated
 * and is accessed by Cloud Connector after the routine returns.
 *
 * The following is a list of configuration routines which need to be implemented for
 * your platform, please review each routine:
 *  @li app_get_ip_address()
 *  @li app_get_mac_addr()
 *  @li app_load_device_id()
 *  @li app_save_device_id()
 *  @li app_get_vendor_id()
 *  @li app_get_device_type()
 *  @li app_get_device_cloud_url()
 *  @li app_get_connection_type()
 *  @li app_get_link_speed()
 *  @li app_get_phone_number()
 *  @li app_get_tx_keepalive_interval()
 *  @li app_get_rx_keepalive_interval()
 *  @li app_get_wait_count()
 *  @li app_get_firmware_support()
 *  @li app_get_data_service_support()
 *  @li app_get_file_system_support()
 *  @li app_get_remote_configuration_support()
 *  @li app_get_max_message_transactions()
 *  @li app_get_device_id_method()
 *  @li app_get_wan_type()
 *  @li app_get_imei_number()
 *  @li app_get_esn()
 *  @li app_get_meid()
 *  @li app_get_identity_verification()
 *  @li app_get_password()
 *  @li app_config_error()
 *  @li app_get_device_cloud_phone()
 *  @li app_set_device_cloud_phone()
 *  @li app_get_device_cloud_service_id()
 *
 * @note For information on using hard coded values instead of these configuration callbacks,
 *       which helps to reduce Cloud Connector code space,  see @ref connector_config_data_options
 *
 * @subsection application_start Cloud Connector Initialization
 *
 * As in the @ref step3 "compile_and_link" sample, you must call connector_init().  However, in
 * this sample, you will execute the code, making all the networking, configuration, and operating
 * system callbacks.
 *
 * The connector_init() call, must now include an operational application callback:
 *
 * @code
 * // Initialize Cloud Connector with the application callback
 * connector_handle = connector_init(app_connector_callback);
 * @endcode
 *
 * and placed in location capable of starting a network client.
 *
 * Its recommended that you use the code inside main() and cut and paste, as is, into an appropriate spot.
 * In the Linux platform provided, the routine main() creates a thread which then calls connector_run().  You
 * will need to set up a similar thread in your environment (or call connector_step()) periodically.
 *
 * This thread is shown below:
 * @code
 * static void * connector_run_thread(void * arg)
 * {
 *     connector_status_t status;
 *
 *     APP_DEBUG("connector_run thread starts\n");
 *
 *     // Run Cloud Connector, this will only return on a Cloud Connector abort
 *     status = connector_run(arg);
 *
 *     APP_DEBUG("connector_run thread exits %d\n", status);
 *
 *     pthread_exit(arg);
 * }
 * @endcode
 *
 * @note The connector_init() call must successfully complete prior to any other
 * Cloud Connector call (i.e., connector_run(), connector_step()).
 *
 * @section step6 Setup your build environment
 *
 * @subsection add_c_files Add the source files to your build system
 *
 * To build the connect_to_device_cloud sample you will need to add the files shown below
 * to your build environment.
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 * <th class="title">Name</th>
 * <th class="title">Description</th>
 * <th class="title">Location</th>
 * </tr>
 * <tr>
 * <td>connector_api.c</td>
 * <td>Contains the Cloud Connector source code</td>
 * <td>private</td>
 * </tr>
 * <tr>
 * <td>application.c</td>
 * <td>Contains the code which runs the sample and main callback that calls
 * callbacks in os.c, network_tcp.c, and config.c</td>
 * <td>public/run/samples/connect_to_device_cloud</td>
 * </tr>
 * <tr>
 * <td>os.c</td>
 * <td>Operating System Routines</td>
 * <td>public/run/platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 * <td>network_tcp.c</td>
 * <td>Network Interface</td>
 * <td>public/run/platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 * <td>config.c</td>
 * <td>Configuration Routines</td>
 * <td>public/run/platforms/<i>my_platform</i></td>
 * </tr>
 * <tr>
 * <td>main.c (if applicable)</td>
 * <td>Program entry point, calls @endhtmlonly connector_init() @htmlonly</td>
 * <td>public/run/platforms/<i>my_platform</i></td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection add_include_paths Add the include paths
 *
 * The following is a list of include paths you will need to add to your build
 * system:
 *
 * @li public/include
 * @li public/include/custom
 * @li run/platforms/@a my_platform
 *
 * @section step7 Build the connect_to_device_cloud sample
 * Next, build the sample.  If running on Linux system type: @htmlonly"<I>make</I>"@endhtmlonly
 * in a command shell from the public/run/samples/connect_to_device_cloud> directory.  If you're
 * not using Linux, then you will have to build the connect_to_device_cloud application for your
 * environment.
 *
 * @section step8 Run the connect_to_device_cloud sample
 * The name of the executable generated by the Linux Makefile is called connector.
 * In Linux, type ./connector from a console to execute the program.
 *
* @note By default @ref CONNECTOR_DEBUG is defined in connector_config.h, which prints
* helpful Cloud Connector debug messages to the standard output.
 *
 * @subsection good_results_output Example output from a successful run of connect_to_device_cloud
 * Below is the standard output shown for a successful @ref step7 "connect_to_device_cloud" run:
 *
 * @code
 * >./connector
 * Start Cloud Connector for Embedded
 * Cloud Connector v2.0.0.0
 * Device ID  =  00 00 00 00 00 00 00 00 78 E7 D1 FF FF 84 47 82
 * application_run thread starts
 * application_run thread exits 0
 * connector_run thread starts
 * dns_resolve_name: ip address = [108.166.22.160]
 * app_tcp_connect: fd 3
 * app_network_tcp_open: connected to login.etherios.com
 * Send MT Version
 * Receive Mt version
 * Send keepalive params
 * Rx keepalive parameter = 60
 * Tx keepalive parameter = 90
 * Wait Count parameter = 5
 * Send protocol version
 * Receive protocol version
 * Send identity verification
 * Send Device Cloud url = login.etherios.com
 * Send vendor id = 0x01000001
 * Send device type = Linux Application
 * Connection Control: send redirect_report
 * Connection Control: send connection report
 * get_ip_address: Looking for current device IP address: found [2] entries
 * get_ip_address: 1: Interface name [lo]  IP Address [127.0.0.1]
 * get_ip_address: 2: Interface name [eth3]        IP Address [10.52.18.89]
 * Send device IP address =  0A 34 12 59
 * Sending MAC address =  78 E7 D1 84 47 82
 * Send complete
 * connector_tcp_communication_started
 * @endcode
 *
 * @subsection bad_results_output Locating helpful error info in Cloud Connector standard output
 *
 * When an error occurs, since APP_DEBUG is defined as printf in your platform.h file, critical errors
 * are displayed via the @ref connector_request_id_config_error_status callback.  Below is an example
 * with an output showing an error related to the @ref connector_request_id_config_device_cloud_url
 * callback.  By searching through your application code, you can easily trace this callback to the
 * app_get_device_cloud_url() porting function:
 *
 * @code
 * >./connector
 * Start Cloud Connector for Embedded
 * Cloud Connector v2.0.0.0
 * Device ID  =  00 00 00 00 00 00 00 00 78 E7 D1 FF FF 84 47 82
 * app_config_error: Class: connector_class_id_config (0) Request: connector_request_id_config_device_cloud_url (3) Error status: connector_invalid_data_size (2)
 * Unable to initialize the connector
 * >
 * @endcode
 *
 * Given this output, a thorough code review of the app_get_device_cloud_url() function should reveal a porting defect.
 *
 * @subsection add_your_device_to_the_cloud Add your Device to Device Cloud
 *
 * Device IDs are a globally unique identifier for Device Cloud clients.  The
 * next several sections walk through the steps required to add your device to
 * your Device Cloud account.
 *
 * @note Look @ref device_id "here" for more information on Device ID's.
 *
 *
 *  -# Navigate to http://login.etherios.com and log on to Device Cloud
 *  -# Click on Device Management tab.
 *  -# Click the Add Devices button to bring up the Add Devices dialog.
 * \n
 * \n  @image html cloud_add_device_button.png
 * \n
 *  -#  Within the Add Devices dialog click the Add Manually>> button.
 * \n
 * \n  @image html cloud_add_device_manually.png
 * \n
 *  -# Select Device ID: from the drop-down menu.
 * \n
 * \n @image html cloud_add_device_select_device_id.png
 * \n
 *  -# Enter the Device ID for your device then click the Add button.
 * \n
 * \n @image html cloud_add_device_enter_device_id.png
 * \n
 *  -# Click the OK button to add your device to Device Cloud.
 * \n
 * \n @image html cloud_add_device_click_ok.png
 *
 * @subsection view_result_on_cloud Viewing Results on Device Cloud
 *
 * Click the Refresh button. The device will reconnect to Device Cloud.
 * If successful your device Status will show connected and you can now communicate
 * with your device using Device Cloud.  The next step
 * is to integrate in one of the samples to add in more functionality to your
 * application.
 *
 * @note If you cannot connect to Device Cloud, the most likely problem
 * is in the Cloud Connector configuration.  Review the configuration routines in
 * config.c; the device ID and vendor ID must be valid to connect.
 *
 * @image html cloud_device_connected.png
 *
 * @section step9 Next Steps
 *
 * Now that you have a basic sample up and running with Device Cloud, you
 * can include more functionality to your application.  We first suggest
 * reading the @ref api1_overview "API" section of the documentation and then
 * select a sample which has the functionality you need.
 *
 * @note
 * The Linux makefiles were written to clean and rebuild each time they're invoked.
 * The was done because the object generated in the private tree is based on dependencies
 * from public applications.  Therefore, when switching between applications, unless
 * you @htmlonly"<I>make clean all</I>"@endhtmlonly, you otherwise run the risk of executing
 * inconsistent public and private objects.
 *
 * @htmlinclude terminate.html
 */

