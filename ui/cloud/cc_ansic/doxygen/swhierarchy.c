/*! @page swhierarchypage Source Code Organization
 *
 * @htmlinclude nav.html
 *
 * @section swhierarchy Source Code Organization
 * The Cloud Connector source code is divided into two partitions, a private partition and a public Application Framework.  The private partition
 * includes the sources that implement the @ref api_overview "Cloud Connector public API".  The public Application Framework includes a
 * set of sample applications used for demonstration purposes.
 *
 * @note For forward compatibility the private partition should be treated as a black box and never changed or referenced directly.  It's recommended
 * after completing the @ref getting_started process that this private partition be converted into a library.   Note the term
 * "Cloud Connector library" is used synonymously for the "Cloud Connector private partition".
 *
 * @subsection DirectoryStructure Directory Structure
 *
 * When uncompressed the directory structure below will be created/.
 * The public directory is divided into a step and run branch to easily distinguish the different thread modeling
 * requirements.
 *
 *
 * @htmlonly
 * <table class="apitable">
 * <tr>
 *   <th class="title">Directory</th>
 *   <th class="title">Description</th>
 * </tr>
 * <tr>
 *   <td>private</td>
 *   <td>Cloud Connector Library Code<br></br>
 *   This directory contains all the private and internal files used to build Cloud Connector library.
 *   <b>You should never modify, directly call, debug, or reference any file from this directory</b>.</td>
 * </tr>
 * <tr>
 *   <td>public/include</td>
 *   <td> @endhtmlonly  @ref api_overview "Cloud Connector Public API" @htmlonly <br></br>
 *   Contains the public header:
 *      <ol>
 *          <li>@endhtmlonly  @ref connector_api.h @htmlonly</li>
 *      </ol>
 * </tr>
 * <tr>
 *   <td>public/include/api</td>
 *   <td>
 *      Contains the individual API headers that  @endhtmlonly @ref connector_api.h @htmlonly includes. This path is *not* required
 *      to be included specifically in the compiler path and these files can *not* be included directly. Use only @endhtmlonly @ref connector_api.h @htmlonly.
 *      <ol>
 *          <li>@endhtmlonly  @ref connector_api_config.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_data_point.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_data_service.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_file_system.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_firmware.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_network.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_os.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_remote.h @htmlonly</li>
 *          <li>@endhtmlonly  @ref connector_api_short_message.h @htmlonly</li>
 *      </ol>
 *   </td>
 * </tr>
 * <tr>
 *   <td>public/include/custom</td>
 *   <td>
 *     Contains the files that might be modified depending on the platform's architecture.
 *     <ol>
 *       <li>@endhtmlonly  @ref connector_debug.h @htmlonly</li>
 *       <li>@endhtmlonly  @ref connector_types.h @htmlonly</li>
 *     </ol>
 *     <p>
 *     All the machine device types are located in @endhtmlonly @ref connector_types.h @htmlonly and
 *     might require updating to match your platform's characteristics (i.e., data size and supported compiler data types).
 *
 *     Public header @endhtmlonly @ref connector_debug.h @htmlonly contains the prototype for @endhtmlonly @ref connector_debug_vprintf() @htmlonly which is a required
 *     porting implementation function when @endhtmlonly @ref CONNECTOR_DEBUG @htmlonly is defined.  Also contains appropriate definitions for the macro ASSERT(),
 *     which is used within the private folder.
 *     </p>
 *   </td>
 * </tr>
 * 
 * </tr>
 * <tr>
 *   <td>public/run</td>
 *   <td>Platforms and samples for running Cloud Connector as a separate thread <br></br>
 *   This directory contains platforms and samples that use @endhtmlonly connector_run() @htmlonly which runs
 *   Cloud Connector as a separate thread in a multitasking environment.</td>
 * </tr>
 * <tr>
 *   <td>public/run/platforms</td>
 *   <td>Platform files for running Cloud Connector as a separate thread<br></br>
 *   Platform dependent files that interface between Cloud Connector library (private) and the sample applications.
 *   These files include @endhtmlonly @ref network_tcp.c, file_system.c, @ref os.c, @ref config.c, and main.c. @htmlonly
 *   The Getting Started Procedure walks you through the process of porting and setting these platform files.</td>
 * </tr>
 * <tr>
 *   <td>public/run/samples</td>
 *   <td>Samples for Cloud Connector<br></br>
 *   Samples on how to use Cloud Connector, the compile_and_link sample is used to verify
 *   that your new environment is able to build. There is a sample for each major
 *   feature in Cloud Connector, there is documentation in this guide for each sample.</td>
 * </tr>
 * <tr>
 *   <td>public/step</td>
 *   <td>This directory contains platforms and samples that use @endhtmlonly connector_step() @htmlonly which makes
 *   repeated calls within a round robin control loop.</td>
 * </tr>
 * </table>
 * @endhtmlonly
 *
 * @subsection AppStructure Source Code Hierarchy
 * Cloud Connector is split into two separate partitions, a private partition and a public Application Framework.
 *
 * The private partition (Cloud Connector Library) includes source code that implements the @ref api_overview "Cloud Connector public API", plus all the internal code used to implement the
 * Device Cloud protocol.  For forward compatibility this private partition should be treated as a black box and never changed or referenced directly.
 *
 * The public Application Framework partition is further divided in two: a Platform and Sample section.  The Platform section is related to system
 * specific porting dependencies (i.e., fleshing out @ref os_callbacks "operating system" calls, @ref network_callbacks "networking", @ref config_callbacks "system configuration").  The Sample section contains an application
 * structure to cleanly interface between the Platform section and Cloud Connector private partition.
 *
 * For instance, in a linux run thread model, the main() routine starts two threads: connector_run_thread() and application_run_thread() in main.c.  This file is
 * located in the Platform section since it relies on threads (an operating system dependency).  The connector_run_thread() directly calls the @ref api_overview "Cloud Connector API"
 * connector_run(), and the application_run_thread() calls application_run().  The application_run() function has no system dependencies and contains
 * Cloud Connector specific functions, therefore, it resides within the Sample section.
 *
 * In the diagram below, Cloud Connector Library is shown encapsulated within the dotted line on top (in the Private Source Code Area).  The
 * bottom is the Platform section, where the bottom left side shows main() calling connector_init() and spawning the two threads.   Also
 * shown is the application_run_thread() calling application_run() in the Sample section.  The Sample section is encapsulated within the
 * dotted line on the center-right.
 *
 * @image html SWOverview.jpg
 *
 * Based on the particular sample, application_run() could either make calls to connector_initiate_action(), or could just return and complete.
 *
 * The diagram further shows Cloud Connector Library making callbacks into the Sample section.  The @ref connector_callback_t "application callback function", initially passed
 * to Cloud Connector library via the connector_init() call, will pass the callback request to the appropriate handler, which will either be in the Platform section for
 * @ref os_callbacks "operating system", @ref network_callbacks "networking", or @ref config_callbacks "configuration" callbacks; or remain locally
 * handled (in the Sample section) for the Data Service callbacks.
 *
 * @htmlinclude terminate.html
 */
