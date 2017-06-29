/*! @page health_metrics_sample Health Metrics Sample
 *
 * @htmlinclude nav.html
 *
 * @section health_metrics Introduction
 * 
 * Health Metrics are a group of predefined Data Streams that Device Cloud parses to trigger alarms and to show relevant information about a Device's operating status.
 * 
 * This sample demonstrates how to push these Health Metrics data to Device Cloud and how to configure the behavior using @ref rci_service "Remote Configuration Interface sertice".
 * It is @b not inteded to be a final implementation but a starting point on how to construct the CSV and push the data to Device Cloud. There are some behavior that are not defined
 * in the specifications and thus are not handled by this sample such as if it should gather information when the TCP transport is off, how many times should it retry to push, etc.
 * For more information about how this should behave please contact Device Cloud team.
 *
 * @note The first &lt;n&gt; in "eth" and "mobile" metrics specifies the interface's instance. The "mobile" ones also have a second &lt;n&gt; that stands for the SIM number, and the &lt;tech&gt; is
 * for the SIM's network technology (2G, 3G, or 4G).
 * 
 * The Data Streams are the following:
 * 
 * @htmlonly
 * <table class="apitable">
 *     <tr>
 *         <td class="title">Stream ID</td>
 *         <td class="title">Description</td>
 *         <td class="title">Unit</td>
 *     </tr>
 *     <tr>
 *         <td colspan=3 style="text-align:center; font-size:105%"><b>Mobile Metrics</b></td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/rxbytes</td>
 *         <td>Number of bytes received during the sample period</td>
 *         <td>bytes</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/txbytes</td>
 *         <td>Number of bytes transmitted during the sample period</td>
 *         <td>bytes</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/latency/min</td>
 *         <td>Minimum response time for (firewall) monitored ICMP/UDP packets during the sample period</td>
 *         <td>milliseconds</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/latency/avg</td>
 *         <td>Average response time for (firewall) modified ICMP/UDP packets during the sample period</td>
 *         <td>milliseconds</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/latency/max</td>
 *         <td>Maximum response time for (firewall) modified ICMP/UDP packets during the sample period</td>
 *         <td>milliseconds</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/transactions/count</td>
 *         <td>Number of times we have received a reply to (firewall) monitored ICMP/UDP packet</td>
 *         <td>occurrences</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/fdrop/count</td>
 *         <td>Number of times we have NOT received a reply to (firewall) monitored ICMP/UDP packet</td>
 *         <td>occurrences</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/losspercent</td>
 *         <td>Loss percentage of (firewall) monitored ICMP/UDP packet</td>
 *         <td>percentage</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/drop/count</td>
 *         <td>Number of times PPP interface is deactivated by anything other than firewall</td>
 *         <td>occurrences</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/oos/count</td>
 *         <td>Number of times link was deactivated due to firewall detecting no response to monitored ICMP/TCP/UDP data</td>
 *         <td>occurrences</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/&lt;tech&gt;/uptime</td>
 *         <td>Number of seconds the PPP interface is up per hour</td>
 *         <td>seconds</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/module</td>
 *         <td>{<br/> “modem_id”: “&lt;modem id&gt;”,<br/> &quot;imei&quot;: &quot;&lt;imei&gt;&quot;,<br/> “sims”: 2,<br/> }</td>
 *         <td>JSON</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/temperature</td>
 *         <td>Internal temperature of cellular module</td>
 *         <td>Celsius</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/active_sim</td>
 *         <td>“active_sim”: 1</td>
 *         <td>SIM name</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/cell_id/{id}</td>
 *         <td>“cell_id”: “&lt;cell ID&gt;”,</td>
 *         <td>cell id</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/network</td>
 *         <td>“network”: “verizon,at&amp;t,...”,</td>
 *         <td>network name</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/rssi</td>
 *         <td>Cell module received signal strength (RSSI) - 3G, CDMA/UMTS/EV-DO</td>
 *         <td>dBm</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/ecio</td>
 *         <td>Energy to interface Ratio - 3G, CDMA/UMTS/EV-DO</td>
 *         <td>
 *         </td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/rsrp</td>
 *         <td>Reference Signal Received Power - 4G LTE</td>
 *         <td>dBm</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/rsrq</td>
 *         <td>Reference Signal Received Quality - 4G LTE</td>
 *         <td>dB</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/sinr</td>
 *         <td>Signal to Interference plus Noise Ratio - 4G LTE</td>
 *         <td>dB</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/snr</td>
 *         <td>Signal to noise ratio</td>
 *         <td/>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/status</td>
 *         <td>{<br/> &quot;registration&quot;: &quot;&lt;registration status&gt;&quot;,<br/> &quot;attachment&quot;: &quot;&lt;attachment status&gt;&quot;<br/> }</td>
 *         <td>JSON</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/lac</td>
 *         <td>Location Area Code - 3G, CDMA/UMTS/EV-DO<br/> TBD: does this include the mobile country code (MCC) and mobile network code (MNC)?</td>
 *         <td>JSON</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/tac</td>
 *         <td>Tracking Area Code - 4G LTE</td>
 *         <td/>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/session</td>
 *         <td>Seconds cellular data session active</td>
 *         <td>seconds</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/info</td>
 *         <td>{<br/> “iccid”: “&lt;iccid&gt;”,<br/> “imsi”: “&lt;imsi&gt;”,<br/> “phone_num”: “&lt;phone number&gt;”,<br/> …<br/> }</td>
 *         <td>JSON</td>
 *     </tr>
 *     <tr>
 *         <td colspan=3 style="text-align:center; font-size:105%"><b>Ethernet Metrics</b></td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/tx/bytes</td>
 *         <td>Number of bytes transmitted during the sample period</td>
 *         <td>bytes</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/tx/packets</td>
 *         <td>Number of packets transmitted during the sample period</td>
 *         <td>packets</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/tx/dropped</td>
 *         <td>Number of packets dropped in transmission during the sample period</td>
 *         <td>occurrences</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/tx/overruns</td>
 *         <td>Number of packet overruns in transmission during the sample period</td>
 *         <td>occurrences</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/rx/bytes</td>
 *         <td>Number of bytes received during the sample period</td>
 *         <td>bytes</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/rx/packets</td>
 *         <td>Number of packets received during the sample period</td>
 *         <td>packets</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/rx/dropped</td>
 *         <td>Number of packets dropped in receiving during the sample period</td>
 *         <td>packets</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/link/down_duration</td>
 *         <td>The amount of time in seconds the interface has been down during the sample period</td>
 *         <td>seconds</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/eth/&lt;n&gt;/link/down_count</td>
 *         <td>The number of times the interface has been down during the sample period</td>
 *         <td>occurrences</td>
 *     </tr>
 *     <tr>
 *         <td colspan=3 style="text-align:center; font-size:105%"><b>System Metrics</b></td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/mem/available</td>
 *         <td>Total RAM</td>
 *         <td>bytes</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/mem/free</td>
 *         <td>Free RAM (=heap)</td>
 *         <td>bytes</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/reboots</td>
 *         <td>Number of (warm) reboots</td>
 *         <td>count/int</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/msg/free</td>
 *         <td>Free Messages (Transport)</td>
 *         <td>count/int</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/msg/min</td>
 *         <td>Minimum Messages (Transport)</td>
 *         <td>count/int</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/buf/free</td>
 *         <td>Free Buffers (Transport)</td>
 *         <td>count/int</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/buf/min</td>
 *         <td>Minimum Buffers (Transport)</td>
 *         <td>count/int</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/bigbuf/free</td>
 *         <td>Free Big Buffers (Transport)</td>
 *         <td>count/int</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/bigbuf/min</td>
 *         <td>Minimum Big Buffers (Transport)</td>
 *         <td>count/int</td>
 *     </tr>
 *     <tr>
 *         <td>metrics/sys/cpu/used</td>
 *         <td>Current cpu utilization</td>
 *         <td>percentage/int</td>
 *     </tr>
 * </table>
 * @endhtmlonly 
 *
 * @section health_metrics_sample_location Source Code Location
 *      - For the connector_run() version:
 *           - @a /connector/public/run/samples/health_metrics
 *
 *      - For the connector_step() version:
 *           - @a /connector/public/step/samples/health_metrics
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
 * The routine application_run() (or application_step()) is called after initialization and calls the routine
 * health_metrics_report_step() that, accordingly to the configuration structure passed, will call the health_metrics callbacks to generate the Data Points
 * and push them to Device Cloud when necessary.
 *
 * @subsection abstraction_callbacks Application abstraction callbacks
 * Because this code is intended to be ported into different platforms, the following functions must be provided:
 * - hm_print_line(): how to output info to stdout.
 * - hm_malloc_data(): to dynamically allocate a memory block.
 * - hm_free_data(): to free a dynamically allocated memory block.
 * - hm_realloc_data(): to increase the size of a dynamically allocated memory block.
 * - hm_get_system_time(): to gather the epoch time (POSIX) that the reports will use.
 * 
 * @subsection health_metrics_callbacks Device's Health Metrics Callbacks
 * 
 * As shown before, some Data Streams have some indexes "n", (i.e.: metrics/mobile/&lt;n&gt;/... or metrics/eth/&lt;n&gt;/...), this index stands for the number of instances of the interface.
 * So if a Device has three Ethernet, then it should report indexes 0, 1 and 3. Also, in "mobile" metrics there is another index in "net", this stands for the SIM card, if the
 * mobile module has support for two SIMs it should report "metrics/mobile/0/net/@b 0" and "metrics/mobile/0/net/@b 1". Finally, each SIM has some information that is different depending on what
 * network technology it is using (2G, 3G, or 4G), that is the &lt;tech&gt; variation (i.e.:metrics/mobile/&lt;n&gt;/net/&lt;n&gt;/rssi).
 * 
 * @note A stub implementation of all these values can be found at "dev_health_callbacks.c" file and the user is supposed to adapt it to provide actual values.
 * 
 * The health_metrics_report_step() routine will call the functions defined in health_metrics_api.h header file. Depending on the type, the functions have different prototypes. 
 * Before sampling an interface, it will call the user to know how many instances of it are there:
 * @code
 * unsigned int cc_dev_health_get_mobile_instances(unsigned int const upper_index);
 * unsigned int cc_dev_health_get_mobile_net_instances(unsigned int const upper_index);
 * unsigned int cc_dev_health_get_eth_instances(unsigned int const upper_index);
 * @endcode
 * 
 * These function must return the value of the maximum instance that should be queried (not necessarily how many exist). For example, if it returns 3 it will requests data for "metrics/mobile/0/...",
 * "metrics/mobile/1/...", and "metrics/mobile/2/...", then each function should return if the value is available or not (see below).
 * @note the "upper_index" argument is only meaningful for cc_dev_health_get_mobile_net_instances() function, to tell how many SIMs each instance has.
 * 
 * Most data gathering functions have the following prototype:
 * @code
 * connector_bool_t cc_dev_health_get_eth_tx_bytes(connector_indexes_t const * const indexes, uint64_t * const value);
 * connector_bool_t cc_dev_health_get_eth_tx_packets(connector_indexes_t const * const indexes, uint64_t * const value);
 * ...
 * @endcode
 * 
 * The return value of this functions tells health_metrics_report_step() whether the value is available. If it is, then it should be stored in the pointer pointed by "value". The "indexes" argument
 * has two values, "upper" and "lower" which stand for the first and second indexes in the Stream ID. For example, in all eth-related functions only the upper value is meaningful and stands for the
 * ethernet instances, while in the mobile-related functions the "lower" tells you which SIM is being used.
 * 
 * Also, some mobile-related functions have the "tech" argument to distinguish for which technology it is being queried. Notice that health_metrics_report_step() will call for <b>all</b> the technologies
 * and these functions must return "connector_false" for all those which are not being used. 
 * @code
 * connector_bool_t cc_dev_health_get_mobile_net_rxbytes(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
 * connector_bool_t cc_dev_health_get_mobile_net_txbytes(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
 * ...
 * @endcode
 * 
 * Finally, the JSON-type Data Streams are handled in a different way. First the application queries if the whole JSON is available:
 * @code
 * connector_bool_t cc_dev_health_mobile_module_present(connector_indexes_t const * const indexes);
 * connector_bool_t cc_dev_health_mobile_module_net_status_present(connector_indexes_t const * const indexes);
 * connector_bool_t cc_dev_health_get_mobile_net_info_present(connector_indexes_t const * const indexes);
 * ...
 * @endcode
 * 
 * And then it queries for each value sepparately. These functions do @b not return a "connector_bool_t" because the values must be always there. If not available, then an empty string must be returned.
 * @note All strings returned by these functions must be stored in dynamically-allocated memory (with the same method than hm_malloc_data()) because health_metrics_report_step() will call hm_free_data() 
 * once it is done with it.
 * 
 * @code
 * void cc_dev_health_get_mobile_module_imei(connector_indexes_t const * const indexes, char * * const value);
 * void cc_dev_health_get_mobile_module_modem_id(connector_indexes_t const * const indexes, char * * const value);
 * void cc_dev_health_get_mobile_module_sims(connector_indexes_t const * const indexes, unsigned int * const value);
 * 
 * void cc_dev_health_get_mobile_net_info_iccid(connector_indexes_t const * const indexes, char * * const value);
 * void cc_dev_health_get_mobile_net_info_imsi(connector_indexes_t const * const indexes, char * * const value);
 * void cc_dev_health_get_mobile_net_info_phone_num(connector_indexes_t const * const indexes, char * * const value);
 * 
 * void cc_dev_health_get_mobile_net_status_registration(connector_indexes_t const * const indexes, char * * const value);
 * void cc_dev_health_get_mobile_net_status_attachment(connector_indexes_t const * const indexes, char * * const value);
 * @endcode
 * 
 * @subsection config_file Cloud Connector configuration file
 * 
 * The RCI code is merely an extension of @ref rci_sample. Please refer to that section to learn more about this service. This is the configuration file used
 * by this sample, note the addition of "simple_enhanced_services" group to the device's settings:
 *
 * @code
 * globalerror load_fail "Load fail"
 * globalerror save_fail "Save fail"
 * globalerror memory_fail "Insufficient memory"
 * 
 * # Device info that shows up in Device Cloud device summary
 * # Device Cloud queries this system setting to display the information in root folder
 * group setting system "System"
 *     element description "Description" type string max 63
 *     element contact "Contact" type string max 63
 *     element location "Location" type string max 63
 *     error invalid_length "Invalid Length"
 *  
 * # Device location
 * # State configuration for GPS
 * # Must setup the following group for Latitude and Longitude to be shown in Device Cloud.
 * group state gps_stats "GPS"
 *     element latitude "Latitude" type string access read_only
 *     element longitude "Longitude" type string access read_only
 *     
 * # Health Data Metrics configuration
 * group setting simple_enhanced_services "Simple Health data reporting configuration"
 *     element report_rate "Global reporting interval (minutes)" type uint32
 *     element eth_metrics "Ethernet Metrics" type on_off
 *     element eth_sample_rate "Ethernet Sampling interval (minutes)" type uint32
 *     element mobile_metrics "Mobile Metrics" type on_off
 *     element mobile_sample_rate "Mobile Sampling interval (minutes)" type uint32
 *     element sys_metrics "System Metrics" type on_off
 *     element sys_sample_rate "System Sampling interval (minutes)" type uint32
 * @endcode
 *
 *
 * Run @ref rci_tool to generate  * Run @ref rci_tool to generate @b connector_api_remote.h and @b remote_config.c files:
 * @code
 *
 *   java -jar /connector/tools/ConfigGenerator.jar username:password -url=login.etherios.com "Linux Application" 1.0.0.0 config.rci
 *
 * @endcode
 * @note To run the above line you need to have Java Runtime Environment installed in your system. If you
 * are not sure or do not know how to install it properly, please follow the instructions in 
 * @htmlonly <a href=http://www.java.com/en/download/help/download_options.xml>this link</a>@endhtmlonly
 *
 * @section connect_build Building
 *
 * To build this example on a Linux system, go to the public/run/samples/health_metrics
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
 * <th>application.c</th>
 * <td>Contains application_run() and the application-defined callback</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>status.c</th>
 * <td>Status calls</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>health_metrics_api.h</th>
 * <td>Header file with the health metrics sample API.</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>health_metrics.c</th>
 * <td>Health Metrics code, it includes health_metrics_report_step() and all the auxiliar functions and structures </td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>health_metrics_data_push.c</th>
 * <td>Application's @endhtmlonly @ref connector_class_id_data_service @htmlonly request IDs implementations</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>health_metrics_remote_config.c</th>
 * <td>Remote Configuration callbacks to change Health Metrics groups, sample rates, and report rate</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>health_metrics_process.h</th>
 * <td>Health Metrics auxiliar functions for processing.</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>health_metrics_structs.h</th>
 * <td>Health Metrics structures for processing.</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>dev_health_callbacks.c</th>
 * <td>Stub functions called by health_metrics_report_step() to request user for data to generate the Data Points. These functions must be customized for each platform and/or device.</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <tr>
 * <th>connector_config.h</th>
 * <td>Cloud Connector options</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>config.rci</th>
 * <td>Remote Configuration file</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>connector_api_remote.h</th>
 * <td>Remote configuration API definition generated by the @endhtmlonly @ref rci_tool @htmlonly </td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>remote_config.c</th>
 * <td>Remote configuration structure generated by the @endhtmlonly @ref rci_tool @htmlonly, it must be passed to @endhtmlonly @ref rci_descriptor_data @htmlonly callback </td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <th>remote_config_cb.c</th>
 * <td>Routines used to remote configuration callback</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>remote_config_cb.h</th>
 * <td>Header for remote configuration callback</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>gps_stats.c</th>
 * <td>GPS remote configuration callback</td>
 * <td>samples/health_metrics</td>
 * </tr>
 * <tr>
 * <th>system.c</th>
 * <td>System remote configuration callback</td>
 * <td>samples/health_metrics</td>
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
 * @li public/run/samples/health_metrics
 * @li public/include
 * @li public/include/custom
 * @li run/platforms/@a my_platform
 *
 * @section running Running
 *
 * Log on to @htmlonly <a href="http://login.etherios.com/">Device Cloud</a>@endhtmlonly
 * (described in the @ref connector_login "Getting Started Section").
 * Once you are logged, add your Device to the list and run the executable, in Linux type ./connector to run the
 * sample. You should see it connected.
 *
 * @image html cloud_device_connected.png
 *
 * Go into "Data Services" tab, "Data Streams" section and filter by "Metrics Streams" to see the data pushed. They should be updated 
 * once per minute unless you have modified the code.
 * 
 * @image html cloud_health_metrics.png
 *
 * Go into Device Management, double click on your Device and navigate into "Configuration" -> "Simple Health data reporting configuration" to modify
 * which metrics are uploaded, at which rate are samples and how often are they reported. This is done through regular RCI, so please refer to @ref rci_sample
 * to learn more about this service.
 *
 * @image html cloud_health_metrics_config.png
 * @htmlinclude terminate.html
 *
 */
