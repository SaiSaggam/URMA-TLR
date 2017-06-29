 /*! @mainpage Cloud Connector
 *
 * @htmlinclude nav.html
 *
 * @section table_of_contents Overview 
 *     -# @ref intro
 *     -# @ref language
 *     -# @ref requirements
 *     -# @ref features
 *     -# @ref communicating
 *     -# @ref threading
 *     -# @ref code_organization
 *     -# @ref PortingFocus
 *         -# @ref DebugTips
 *     -# @ref the_getting_started_process
 *
 * @section intro Introduction
 *
 * Cloud Connector is a software development package used to enable a device to
 * exchange information with Device Cloud.
 * Cloud Connector enables application to device data interaction (messaging),
 * application and device data storage, and remote management of devices. Devices
 * are associated with Device Cloud through the Internet or other wide
 * area network connections, which allows for communication between the device and
 * applications, via Device Cloud.  An important part of this communication
 * is the transfer of data from a device to Device Cloud.
 * Device Cloud leverages a cloud computing model that provides on-demand scalability so
 * you can rest assured that when you need additional computing and storage, Device Cloud will
 * scale to meet your needs.  Device Cloud is designed using a high-availability
 * architecture, with redundancy and failover characteristics in mind.
 * Using Cloud Connector, you can now easily develop cloud connected devices and
 * applications that quickly scale from dozens to hundreds, thousands or even
 * millions of endpoints.
 *
 * @image html cloud.jpg
 *
 * @section language Language Support
 *
 * The Cloud Connector software provided is ANSI X3.159-1989 (ANSI C89) and ISO/IEC 9899:1999 (ANSI C99)
 * compliant. The sample platforms provided use standard ANSI C calls which are available in most operating systems.
 * If you are running on a Linux i486 based platform and using the GNU toolchain the Linux platform
 * and samples can be run without significant modifications.
 *
 * @note
 * Compilers that are not strictly compliant with C89 or C99 will most likely generate compilation
 * errors.  In that case, we recommend careful evaluation of each error and appropriate adjustments be
 * made within the public include area.  Cloud Connector was designed for portability.  This would
 * be the ideal place to make updates.
 *
 * @note
 * Some C89 compilers include elements of C99.  In particular, those
 * C89 compilers that also include stdint.h, which is a part of C99, will
 * result in compilation failures.  These compilation failures can be resolved
 * by defining CONNECTOR_HAS_STDINT_HEADER in your make or build system.
 *
 * @section requirements Platform Memory Requirements
 * Cloud Connector requires both persistent storage (Flash) and RAM.  Flash is used to store instructions and variables.  RAM is used for 
 * dynamic allocation and program stack.  @ref memorymetricspage "Metrics are available" for typical Flash and RAM usage.
 *
 * @section features Device Cloud Features
 * By integrating Cloud Connector into your device, you instantly enable the power of Device Cloud for your device: 
 *     - Send data to the cloud
 *     - Receive data from the cloud
 *     - Enable remote control of the device from the cloud to:
 *         - Update the firmware
 *         - Change the configuration
 *         - Access the file system
 *         - Reboot
 *
 * @section communicating Communicating with your device
 * To manage your device remotely you can use <a href="http://www.etherios.com/devicecloud/devicemanager">Device Management</a>
 * by logging into your @ref step4 "Device Cloud account".  Alternatively, you can communicate with
 * your device programmatically by using @ref web_services "Device Cloud Web Services".
 *
 * @ref web_services "Device Cloud Web Services" requests are used to send data from a remote application to
 * Device Cloud, which then communicates to the device.  This
 * allows for bidirectional machine to machine communication.  Each Cloud Connector sample includes
 * a Python application demonstrating how to communicate to a device using the
 * @ref web_services "Device Cloud Web Services".
 *
 * @section threading Threading Model
 *
 * Cloud Connector can be deployed in a multithreaded or round robin control loop environment.
 *
 * In multithreaded environments that include preemptive threading, Cloud Connector can be implemented as a separate stand-alone thread
 * by calling connector_run().  This is a blocking call that only returns due to a major system failure.
 *
 * Alternatively, when threading is unavailable, typically in a round robin control loop or fixed state machine, Cloud Connector can
 * be implemented using the non-blocking connector_step() call within the round robin control loop.
 *
 * Note in a cooperative, non-preemptive multithreaded environment, either connector_run() or connector_step() can used, based on
 * system determinism and the potential for a non-cooperative thread to exceed Cloud Connector system timing requirements.
 *
 * @note You should decide before proceeding how you intend to call Cloud Connector (within a round robin control loop or running
 * as a separate thread).  In a limited services OS with no real time threading, you should use the connector_step() routine.
 * Otherwise you should use the connector_run() routine.
 *
 * @section code_organization Source Code Organization
 * The @ref swhierarchypage "Cloud Connector source code" is divided into two partitions, a private partition and a public Application Framework.  The private partition
 * includes the sources that implement the @ref api_overview "Cloud Connector public API".  The public Application Framework includes a
 * set of sample applications used for demonstration purposes.
 *
 * @note For forward compatibility the private partition should be treated as a black box and never changed or referenced directly.  It's recommended
 * after completing the @ref getting_started process that this private partition be converted into a library.   Note the term
 * "Cloud Connector library" is used synonymously for the "Cloud Connector private partition".
 *
 * @section PortingFocus Porting Guidelines
 * The Cloud Connector @ref getting_started process includes pulling Cloud Connector into your local build environment, getting the private partition
 * compiled and linked (using the @ref step3 "compile_and_link" sample) and then your @ref step5 "platform ported".  Once your platform
 * is ported, you will verify and confirm your port using the @ref step7 "connect_to_device_cloud" sample.
 *
 * When porting, it is strongly recommended that you maintain the structure of the public Application Framework.  Once porting, compilation and testing
 * are complete, the you can dismantle this framework and incorporate Cloud Connector into your environment as you see fit.
 *
 * When reusing the Application Framework, the largest effort will be updating the lowest layer of the Platform code.  Specifically, the
 * static @ref os_routines "operating system" functions located in os.c, the @ref network_routines "networking" functions in network_tcp.c and
 * the @ref configuration_routines "configuration" functions in config.c.
 *
 * There is no expectation that you will need to port any other code, with the exception of the @ref step1 "changes" required to get your code compiled.
 *
 * For example, the Application Framework includes code to handle an @ref connector_request_id_os_system_up_time callback in os.c.  The function requires a
 * stable system timer at one second resolution.  In the linux platform example, the function is implemented using the POSIX standard
 * time() function:
 *
 * @code
 * connector_callback_status_t app_os_get_system_time(unsigned long * const uptime)
 * {
 *    time((time_t *)uptime);
 *
 *    return connector_callback_continue;
 * }
 * @endcode
 *
 * However, a platform that does not support POSIX time() might port this function as such:
 *
 * @code
 * connector_callback_status_t app_os_get_system_time(unsigned long * const uptime)
 * {
 *    // Note mysys_GetTickTime() returns the system up time in milliseconds
 *    extern unsigned long mysys_GetTickTime(void);
 *
 *    *uptime = mysys_GetTickTime()/1000;
 *
 *    return connector_callback_continue;
 * }
 * @endcode
 *
 * After converting all the static @ref os_routines "operating system", @ref network_routines "networking", and @ref configuration_routines "configuration"
 * functions, your porting effort is complete.
 *
 * @subsection DebugTips How to debug your Port
 * After porting and compiling, you will run the @ref step7 "connect_to_device_cloud" sample.  We recommend that you carefully
 * observe the standard output from the application.  In particular, the @ref error_status callback will display error
 * data when porting errors are detected in the callbacks.
 *
 * Examples of @ref good_results_output "successful" and @ref bad_results_output "erroneous" output streams are available.
 *
 * @section the_getting_started_process Getting Started
 * To get started, follow along the steps of the @ref getting_started process.
 *
 * @htmlinclude terminate.html
 */

