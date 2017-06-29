/**
 * @file rds/00043.h
 * Simple CSID and Data Items Exchange Format.
 */

#ifndef _RDS00043_H
#define _RDS00043_H

#include <stdint.h>

typedef struct __attribute__ ((__packed__)) {
	uint16_t magic;         //0x5343, 'CS' in little endian
	uint16_t checksum;      //checksum, include header & body
	uint32_t body_length;   //length of body
	uint8_t  body_encoding; //encoding method of body
	uint8_t  reserved[3];
	uint32_t raw_length;    //length of body before encoded
} RDS00043_CSXF_HEADER;

#define RDS00043_CSXF_MAGIC              0x5343

#define RDS00043_BODY_ENCODING_CSIDPACK  0
#define RDS00043_BODY_ENCODING_RDS00035	 7
#define RDS00043_BODY_ENCODING_DEFLATED  8

#endif // ifndef _RDS00043_H
