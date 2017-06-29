/**
 * @file rds/00042.h
 * Common C Section Structure.
 */

#ifndef _RDS00042_H
#define _RDS00042_H

#include <stdint.h>

typedef struct __attribute__ ((__packed__)) {
	uint16_t magic;         //0x3432
	uint16_t checksum;      //checksum, include header & vidi area
	uint32_t length;        //length, include header, vidi & config data
	uint16_t data_start;    //offset of config data area
	uint8_t  data_type;     //type of config data area
	uint8_t  reserved[5];
} RDS00042_CCSS_HEADER;

#define RDS00042_CCSS_MAGIC             0x3432

#define RDS00042_DATA_TYPE_RESERVED     0
#define RDS00042_DATA_TYPE_ACONFIG      1
#define RDS00042_DATA_TYPE_RDS00043     2

#endif // ifndef _RDS00042_H
