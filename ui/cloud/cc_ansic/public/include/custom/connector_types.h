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

/**
 * @file
 *  @brief Types required by Cloud Connector
 *
 */

#ifndef CONNECTOR_TYPES_H_
#define CONNECTOR_TYPES_H_

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>

/**
 * The Cloud Connector uses this function to produce output string according to the format.
 * This function behaves exactly as snprintf except that the variable argument list is passed
 * as a va_list instead of a succession of arguments.
 *
 * In the C library the prototype for vsnprintf is defined as vsnprintf(char * str, size_t size, const char *format, va_list ap);
 *
 */
int connector_snprintf(char * const str, size_t const size, char const * const format, ...);

#if __STDC_VERSION__ >= 199901L
  /* If the compiler is C99 complaint, then we have stdint.h. */
  #define CONNECTOR_HAS_STDINT_HEADER
#endif

#if (defined __MWERKS__) && (defined __MQX__)
  #if (!defined CONNECTOR_HAS_STDINT_HEADER)
    #define CONNECTOR_HAS_STDINT_HEADER
  #endif
#else
  #if !defined IAR
    #include <stdio.h>
  #endif
#endif

#if defined CONNECTOR_HAS_STDINT_HEADER
  #include <stdint.h>
  #include <inttypes.h>

#else
/**
* @defgroup user_types User Defined C types
* If your compiler is C89 complaint these defines are used: you will need
* to define them appropriately for your system.  If your compiler is C99 complaint
* then the types from stdint.h and inttypes.h are used.
* @{
*/
/**
 *  Unsigned 8 bit value.
 */
  typedef unsigned char uint8_t;

/**
 *  Appends the correct suffix to an 8-bit signed integer literal.
 */
#define INT8_C(c)       c
/**
 *  Appends the correct suffix to an 8-bit unsigned integer literal.
 */
#define UINT8_C(c)      c

#ifndef UINT8_MAX
/**
*  Unsigned 8 bit maximum value.
*/
#define UINT8_MAX  UINT8_C(0xFF)
#endif

/**
 *  Unsigned 16 bit value.
 */
  typedef unsigned short uint16_t;

/**
 *  Appends the correct suffix to a 16-bit signed integer literal.
 */
#define INT16_C(c)      c
/**
 *  Appends the correct suffix to a 16-bit unsigned integer literal.
 */
#define UINT16_C(c)     c

#ifndef UINT16_MAX
/**
*  Unsigned 16 bit maximum value.
*/
#define UINT16_MAX  UINT16_C(0xFFFF)
#endif

/**
 *  Unsigned 32 bit value.
 */
  typedef unsigned long int uint32_t;

/**
 *  Appends the correct suffix to a 32-bit signed integer literal.
 */
#define INT32_C(c)      c
/**
 *  Appends the correct suffix to a 32-bit unsigned integer literal.
 */
#define UINT32_C(c)     c ## U

#ifndef UINT32_MAX
/**
*  Unsigned 32 bit maximum value.
*/
#define UINT32_MAX UINT32_C(4294967295)
#endif

#ifndef SCNu32
/**
*  Scan format specifier for unsigned 32 bit value.
*/
#define SCNu32 "lu"
#endif

#ifndef PRIu32
/**
*  Print format specifier for unsigned 32 bit value.
*/
#define PRIu32 "lu"
#endif

/**
*  Signed 32 bit value.
*/
  typedef long int int32_t;

#ifndef INT32_MIN
/**
*  Signed 32 bit minimum value.
*/
#define INT32_MIN -INT32_C(2147483648)
#endif

#ifndef INT32_MAX
/**
*  Signed 32 bit maximum value.
*/
#define INT32_MAX INT32_C(2147483647)
#endif

#ifndef SCNd32
/**
*  Scan format specifier for signed 32 bit value.
*/
#define SCNd32 "ld"
#endif

#ifndef PRId32
/**
*  Print format specifier for signed 32 bit value.
*/
#define PRId32 "ld"
#endif

#ifndef SCNx32
/**
*  Scan format specifier for 32 bit hex value.
*/
#define SCNx32 "lx"
#endif

#ifndef PRIx32
/**
*  Print format specifier for 32 bit hex value.
*/
#define PRIx32 "lx"
#endif

#if (defined CONNECTOR_SUPPORTS_64_BIT_INTEGERS)

/**
 *  Unsigned 64 bit value.
 */
  typedef unsigned long long int uint64_t;

/**
 *  Appends the correct suffix to a 64-bit signed integer literal.
 */
#define INT64_C(c)      c ## LL
/**
 *  Appends the correct suffix to a 64-bit unsigned integer literal.
 */
#define UINT64_C(c)     c ## ULL

#ifndef UINT64_MAX
/**
*  Unsigned 64 bit maximum value.
*/
#define UINT64_MAX UINT64_C(18446744073709551615)
#endif


#ifndef SCNu64
/**
*  Scan format specifier for unsigned 64 bit value.
*/
#define SCNu64 "llu"
#endif

#ifndef PRIu64
/**
*  Print format specifier for unsigned 64 bit value.
*/
#define PRIu64 "llu"
#endif

/**
*  Signed 32 bit value.
*/
   typedef long long int int64_t;

#ifndef INT64_MIN
/**
*  Signed 64 bit minimum value.
*/
#define INT64_MIN -INT64_C(9223372036854775808)
#endif

#ifndef INT64_MAX
/**
*  Signed 64 bit maximum value.
*/
#define INT64_MAX INT64_C(9223372036854775807)
#endif

#ifndef SCNd64
/**
*  Scan format specifier for signed 64 bit value.
*/
#define SCNd64 "lld"
#endif

#ifndef PRId64
/**
*  Print format specifier for signed 64 bit value.
*/
#define PRId64 "lld"
#endif

#ifndef SCNx64
/**
*  Scan format specifier for 64 bit hex value.
*/
#define SCNx64 "llx"
#endif

#ifndef PRIx64
/**
*  Print format specifier for 64 bit hex value.
*/
#define PRIx64 "llx"
#endif

#endif

#ifndef SIZE_MAX
/**
*  size_t maximum value.
*/
#define SIZE_MAX  UINT32_MAX
#endif

/**
* @}
*/


#endif

#if __STDC_VERSION__ >= 199901L
#define PRIsize "zu"
#else
   /**
    *  Print format specifier for size_t.
    */
#define PRIsize "u"
#endif

/**
* @defgroup connector_network_handle_t Network Handle
* @{
*/
/**
 * Used to reference a network session and is passed into Cloud Connector API calls. This
 * type is defined as void * and is used by the application to 
 * keep track of and identify sessions.
 */
typedef void * connector_network_handle_t;
/**
* @}
*/

/**
* @defgroup CONNECTOR_NETWORK_HANDLE_NOT_INITIALIZED Network handle's value when not initialized or invalid
* @{
*/
/**
 * Value to which network handle is set when invalid or not initialized.
 * The @ref connector_network_handle_t type must be defined accordingly.
 */
#define CONNECTOR_NETWORK_HANDLE_NOT_INITIALIZED    NULL
/**
* @}
*/

/**
* @defgroup connector_filesystem_file_handle_t Filesystem's file Handle
* @{
*/
/**
 * Used to reference a file handle, passed into other file operations for Cloud Connector API calls.
 * Also the @ref CONNECTOR_FILESYSTEM_FILE_HANDLE_NOT_INITIALIZED macro must be defined accordingly.
 */
typedef long int connector_filesystem_file_handle_t;
/**
* @}
*/

/**
* @defgroup CONNECTOR_FILESYSTEM_FILE_HANDLE_NOT_INITIALIZED Filesystem's file handle's value when not initialized or invalid
* @{
*/
/**
 * Value to which file handle is set when invalid or not initialized.
 * The @ref connector_filesystem_file_handle_t type must be defined accordingly.
 */
#define CONNECTOR_FILESYSTEM_FILE_HANDLE_NOT_INITIALIZED    -1
/**
* @}
*/

/**
* @defgroup connector_filesystem_dir_handle_t Filesystem's directory Handle
* @{
*/
/**
 * Used to reference a dir handle, passed into other directory operations for Cloud Connector API calls.
 * Also the @ref CONNECTOR_FILESYSTEM_DIR_HANDLE_NOT_INITIALIZED macro must be defined accordingly.
 */
typedef void * connector_filesystem_dir_handle_t;
/**
* @}
*/

/**
* @defgroup CONNECTOR_FILESYSTEM_DIR_HANDLE_NOT_INITIALIZED Filesystem's directory handle's value when not initialized or invalid
* @{
*/
/**
 * Value to which directory handle is set when invalid or not initialized.
 * The @ref connector_filesystem_dir_handle_t type must be defined accordingly.
 */
#define CONNECTOR_FILESYSTEM_DIR_HANDLE_NOT_INITIALIZED     NULL
/**
* @}
*/

/**
* @defgroup connector_filesystem_errnum_t Filesystem's errnum type
* @{
*/
/**
 * Used among directory and file operations for Cloud Connector API calls to report errors to Device Cloud when an operation cannot be performed properly.
 * Also the @ref CONNECTOR_FILESYSTEM_ERRNUM_NONE macro must be defined accordingly.
 */
typedef long int connector_filesystem_errnum_t;
/**
* @}
*/

/**
* @defgroup CONNECTOR_FILESYSTEM_ERRNUM_NONE Filesystem's errnum for success or no-error
* @{
*/
/**
 * Value to which filesystem's errnum is set when no error ocurred.
 * The @ref connector_filesystem_errnum_t type must be defined accordingly.
 */
#define CONNECTOR_FILESYSTEM_ERRNUM_NONE    0
/**
* @}
*/

#endif
