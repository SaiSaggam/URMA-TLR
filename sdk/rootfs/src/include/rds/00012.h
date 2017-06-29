/**
 * @file rds/00012.h
 * ARM Upgrade Header Version 2.0.
 */

#ifndef _RDS00012_H
#define _RDS00012_H

#include <stdint.h>

typedef struct __attribute__ ((__packed__)) {

	char     rom_id[14];            //ASCIIZ
	uint16_t image_checksum;

	char     reserved_10[10];

	uint8_t  lpvs;
	uint8_t  mbz;                   //must be zero, 0x00
	uint32_t time_stamp;

	uint32_t erase_start;
	uint32_t erase_length;
	uint32_t data_offset;
	uint32_t data_length;

	char     reserved_30[16];       //cc_xxx are obsoleted

	uint16_t header_id;             //0x4842
	uint8_t  header_major_version;  //0x02
	uint8_t  header_minor_version;

	char     reserved_44[2];        //header_length is obsoleted

	uint8_t  section_id;
	uint8_t  image_info_type;
	uint32_t image_info_offset;
	uint16_t family_member;
	uint16_t header_checksum;

} RDS00012_AUH20_HEADER;

//the length of CUSTOMER ID in ROM ID (refer RDS/00017)
#define RDS00012_ROMID_CUST_ID_LEN      3

#define RDS00012_AUH20_MAGIC            0x4842
#define RDS00012_AUH20_MAJOR_VERSION    0x02

#define RDS00012_ERR_ARRANGE            -100
#define RDS00012_ERR_HEADER_ID          -101
#define RDS00012_ERR_HEADER_CHECKSUM    -103
#define RDS00012_ERR_MAJOR_VERSION      -104
#define RDS00012_ERR_IMAGE_CHECKSUM     -105

#define RDS00012_ERR_ERASE_DIRECTIVE    -130
#define RDS00012_ERR_DATA_DIRECTIVE     -131
#define RDS00012_ERR_IMAGE_INFO_OFFSET  -132

#define RDS00012_ERR_FAMILY_MEMBER      -150
#define RDS00012_ERR_ROM_ID             -151

#define RDS00012_ERR_LPVS               -200
#define RDS00012_ERR_IMAGE_INFO_TYPE    -221

#endif // ifndef _RDS00012_H
