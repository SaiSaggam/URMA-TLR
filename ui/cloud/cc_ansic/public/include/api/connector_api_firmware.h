/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#ifndef CONNECTOR_API_FIRMWARE_H
#define CONNECTOR_API_FIRMWARE_H

#if (defined CONNECTOR_FIRMWARE_SERVICE)

/**
* @defgroup connector_request_id_firmware_t Firmware Request IDs
* @{
*/
/**
* Firmware Facility Request ID passed to the application's callback for firmware update interface.
* The class id for this connector_request_id_firmware_t is connector_class_id_firmware.
*/
typedef enum {
    connector_request_id_firmware_target_count,         /**< Requesting callback to return number of supported target for firmware update */
    connector_request_id_firmware_info,                 /**< Requesting callback to return the version number,
                                                             maximum code size, description and
                                                             regular expression for firmware update image name of specific target */
    connector_request_id_firmware_download_start,       /**< Requesting callback to start firmware update of specific target */
    connector_request_id_firmware_download_data,        /**< Callback is passed with image data for firmware update. This is called for each chunk of image data */
    connector_request_id_firmware_download_complete,    /**< Callback is called to complete firmware update. */
    connector_request_id_firmware_download_abort,       /**< Requesting callback to abort firmware update */
    connector_request_id_firmware_target_reset          /**< Requesting callback to reset the target */
} connector_request_id_firmware_t;
/**
* @}
*/


/**
* @defgroup connector_firmware_count_t Firmware Target Count
* @{
*/
/**
* Firmware target count for connector_request_id_firmware_count callback which
* is called to return number of supported targets.
*/
typedef struct {
    uint8_t count;             /**< Callback writes number of targets supported */
} connector_firmware_count_t;
/**
* @}
*/

/**
* @defgroup connector_firmware_version_t Firmware Version n
* @{
*/
/**
* Firmware version structure for @ref connector_request_id_firmware_info callback to return
* the firmware version about the specific target.
*/
typedef struct {
    uint8_t major;      /**< Major number */
    uint8_t minor;      /**< Minor number */
    uint8_t revision;   /**< revision number */
    uint8_t build;      /**< build number */
} connector_firmware_version_t;
/**
* @}
*/

/**
* @defgroup connector_firmware_info_t Firmware Information
* @{
*/
/**
* Firmware information structure for @ref connector_request_id_firmware_info callback to return
* information about the specific target.
*/
typedef struct {

    connector_firmware_version_t version; /**< Version number of the target */
    char const * description;   /**< An ASCII description string of the target */
    char const * filespec;      /**< Regular expression for the firmware image name for  the target */

    unsigned int CONST target_number; /**< Target number which target the firmware information is for*/
} connector_firmware_info_t;
/**
* @}
*/

/**
* @defgroup connector_firmware_status_t Firmware Status Codes
* @{
*/
/**
* Firmware status codes are used for @ref connector_request_id_firmware_download_start,
* @see @ref connector_request_id_firmware_download_data and @ref connector_request_id_firmware_download_abort callbacks.
*/
typedef enum {
   connector_firmware_status_success,                        /**< No error */
   connector_firmware_status_download_denied,                /**< Callback denied firmware update */
   connector_firmware_status_download_invalid_size,          /**< Callback returns invalid size */
   connector_firmware_status_download_invalid_version,       /**< Callback returns invalid version */
   connector_firmware_status_download_unauthenticated,       /**< Device Cloud has not been authenticated */
   connector_firmware_status_download_not_allowed,           /**< Device Cloud is not allowed to provided updates */
   connector_firmware_status_download_configured_to_reject,  /**< Callback rejects firmware update */
   connector_firmware_status_encountered_error,              /**< Callback encountered an error that precludes the firmware update */
   connector_firmware_status_user_abort,                     /**< User aborted firmware update */
   connector_firmware_status_device_error,                   /**< Device or Device Cloud encountered an error in the download data */
   connector_firmware_status_invalid_offset,                 /**< connector_request_id_firmware_download_data callback found invalid offset. */
   connector_firmware_status_invalid_data,                   /**< connector_request_id_firmware_download_data callback found invalid data block.*/
   connector_firmware_status_hardware_error                  /**< Callback found permanent hardware error */
} connector_firmware_status_t;
/**
* @}
*/


/**
* @defgroup connector_fw_download_complete_status_t Firmware Complete Status Codes
* @{
*/
/**
* Firmware Update Complete status codes are used in @see connector_firmware_download_complete callback.
*/
typedef enum {
   connector_firmware_download_success,           /**< Callback returns this for firmware download finished successfully
                                                       and calculated checksum matched the checksum sent in the callback */
   connector_firmware_download_checksum_mismatch, /**< Callback returns this for download completed successfully, but
                                                       the calculated checksum did not match the checksum sent in the callback */
   connector_firmware_download_not_complete       /**< Callback did not complete download successfully */
} connector_firmware_download_status_t;
/**
* @}
*/


/**
* @defgroup connector_firmware_download_start_t Firmware Download Start Structure
* @{
*/
/**
* Firmware download start structure for @ref connector_request_id_firmware_download_start callback which
* is called when the device is requested for firmware download.
*/
typedef struct {
    unsigned int CONST target_number;   /**< Target number which firmware target the image data is for */

    char CONST * filename;              /**< Pointer to filename of the image to be downloaded */

    uint32_t code_size;                 /**< size of the code that is ready to be sent to the target */

    connector_firmware_status_t status; /**< Callback writes error status if error is encountered */

} connector_firmware_download_start_t;
/**
* @}
*/


/**
* @defgroup connector_firmware_download_data_t Firmware Download Image Data Structure
* @{
*/
/**
* Firmware download image data structure for connector_request_id_firmware_download_data callback which
* is called when the connector receives a block of image data for firmware download.
*/
typedef struct {
    unsigned int CONST target_number;  /**< Target number which firmware target the image data is for */

    struct {
        uint32_t CONST offset;    /**< Offset value where this particular block of image data fits into the download */
        uint8_t const * data;     /**< Pointer binary image data */
        size_t CONST bytes_used;  /**< Length of binary image data in bytes */
    } image;                      /**< Image data */

    connector_firmware_status_t status; /**< Callback writes error status if error is encountered */

} connector_firmware_download_data_t;
/**
* @}
*/


/**
* @defgroup connector_firmware_download_complete_t Firmware Download Complete Structure
* @{
*/
/**
* Firmware download complete structure containing information about firmware image data
* for connector_request_id_firmware_download_complete callback which is called when Device Cloud is done
* sending all image data.
*/
typedef struct {
    unsigned int CONST target_number;  /**< Target number which firmware target the image data is for */

    connector_firmware_download_status_t status;     /**< Status code regarding the download completion */

} connector_firmware_download_complete_t;
/**
* @}
*/


/**
* @defgroup connector_firmware_download_abort_t Firmware Download Abort Structure
* @{
*/
/**
* Firmware download abort structure for connector_request_id_firmware_abort callback which
* is called when firmware download process is aborted.
*/
typedef struct {
    unsigned int CONST target_number;         /**< Target number which target the firmware download is aborted */

    connector_firmware_status_t CONST status; /**< Abort reason or status */
} connector_firmware_download_abort_t;
/**
* @}
*/


/**
* @defgroup connector_firmware_reset_t Firmware Target Reset Structure
* @{
*/
/**
* Firmware target reset structure for connector_request_id_firmware_reset callback which
* is called to reset the target.
*/
typedef struct {
    unsigned int CONST target_number;     /**< Target number which target the firmware to reset */
} connector_firmware_reset_t;
/**
* @}
*/
#endif

#if !defined _CONNECTOR_API_H
#error  "Illegal inclusion of connector_api_firmware.h. You should only include connector_api.h in user code."
#endif

#else
#error  "Illegal inclusion of connector_api_firmware.h. You should only include connector_api.h in user code."
#endif

