/*! @page upgrade_existing Upgrade an existing application
 *
 * @htmlinclude nav.html
 *
 * @section UpgradeFromPreviousVersion Upgrade an existing application
 *
 * There are two ways of upgrading an existing application from a previous version:
 * if you are only interested in bug fixing and want to do the minimum effort for upgrading, follow the steps
 * in section @ref BugFixing but if you want to get advantage of the new features and APIs included in newer version
 * follow section @ref UpgradingAPI.
 *
 * @section BugFixing Upgrade for bug fixing
 *
 * In this upgrade only the @c private folder will be modified and no extra changes are needed in the application
 * or "public" folder. These changes affect only to the internal behavior of Cloud Connector and do not add or
 * remove functionality to the Cloud Connector.
 * To upgrade your @c private folder follow these steps:
 *      -# Back up your existing code.
 *      -# Decompress the newer release in a temporal directory.
 *      -# Overwrite your code's @c private folder with the one you will find where you decompressed the newer release.
 *      -# Re-build your application.
 *
 * @section UpgradingAPI Upgrade APIs
 * @subsection v2_1_0_x v2.1.0.x
 * Version 2.1.0.x adds new features like the Device ID autoprovisioning, possibility of
 * canceling user-initiated SM sessions and the new SM over SMS support.
 * To include these features to an existing application, follow these steps:
 *      -# Back up your existing code.
 *      -# Decompress the newer release in a temporal directory.
 *      -# Overwrite your code's @c private folder with the one you will find where you decompressed the newer release.
 *      -# Overwrite your code's @c public/include folder (except for @c connector_types.h and @c connector_debug.h)
 *         with the one you will find where you decompressed the newer release.
 *      -# Check the following changes:
 *              - @ref send_data_status_callback is now called @b every @b time a send data request finishes, regardless
 * its status. Previous implementation did not call this callback if the request was succesful, making it more unclear where
 * to release the allocated resources. Check the newer version's sample applications to learn more about how to handle this change.
 *              - Add the new @ref config to your existing code:
 *                      - @ref set_device_id needed for Device ID autoprovisioning, this involves the use of @ref get_device_id.
 *                      - @ref network_sms_start
 *                      - @ref device_cloud_service_id
 *                      - If you are using @c CONNECTOR_DEBUG do not forget to replace @c app_class_to_string() and @c app_class_to_string()
 * functions with the updated version from @c public/run/platforms/linux/config.c.
 *              - If you were using @ref shortmessaging you must check the new API:
 *                      - The structures @ref connector_request_data_point_single_t "connector_request_data_point_single_t",
 * @ref connector_request_data_point_binary_t "connector_request_data_point_binary_t", @ref connector_request_data_point_single_t "connector_request_data_point_single_t",
 * and @ref connector_request_data_point_single_t "connector_request_data_point_single_t" now include a new field @c request_id which must be init to @c NULL if your application does
 * not make use of new @ref connector_initiate_session_cancel feature.
 *                      - If your application uses @ref CONNECTOR_SM_MULTIPART "Multipart", then you should check the new @ref CONNECTOR_SM_MAX_RX_SEGMENTS parameter.
 *                      - If your application uses @ref data_point, then you should check the new @ref CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS parameter.
 *      -# Re-build your application.
 *
 * @subsection v2_2_0_x v2.2.0.x
 * Version 2.2.0.x adds a new feature: it extends the API to allow sending Data Points to
 * multiple Data Streams in one message.
 * To include these features to an existing application, follow these steps:
 *      -# Back up your existing code.
 *      -# Decompress the newer release in a temporal directory.
 *      -# Overwrite your code's @c private folder with the one you will find where you decompressed the newer release.
 *      -# Overwrite your code's @c public/include folder (except for @c connector_types.h and @c connector_debug.h)
 *         with the one you will find where you decompressed the newer release.
 *      -# Check the following changes:
 *                      - If you are using @c CONNECTOR_DEBUG do not forget to replace @c app_class_to_string() and @c app_class_to_string()
 * functions with the updated version from @c public/run/platforms/linux/config.c.
 *                      - Check how @ref connector_initiate_data_point_multiple is used. Two new @ref connector_request_id_data_point_t have been added: @ref connector_request_id_data_point_multiple_response and @ref connector_request_id_data_point_multiple_status that need to be implemented.
 *                      - Sample code in @ref data_point_sample has been updated to show how this new API is used.
 *      -# Re-build your application.
 *
 * @htmlinclude terminate.html
 *
 */
