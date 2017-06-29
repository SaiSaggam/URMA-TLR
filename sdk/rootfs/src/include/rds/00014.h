/**
 * @file rds/00014.h
 * Section Tag (Type 4 Image Info).
 */

#ifndef _RDS00014_H
#define _RDS00014_H

#include <stdint.h>

typedef struct __attribute__ ((__packed__)) {
	uint8_t  cmark;          //complete mark
	uint8_t  section_id;     //refer RDS/00018
	uint16_t magic;          //0x2B24 ({'$','+'} for little endian)
	uint32_t time_stamp;     //time stamp of section
	uint32_t image_length;   //length of section image
	uint16_t image_checksum; //for section image
	uint16_t tag_checksum;   //for section tag
} RDS00014_STAG_HEADER;

#define RDS00014_STAG_MAGIC     0x2B24

#endif // ifndef _RDS00014_H
