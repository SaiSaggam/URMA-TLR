#ifndef __PSEC_H__
#define __PSEC_H__

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include "rds/00043.h"
#include "rds/00014.h"
#include "unilog.h"
#include "flash.h"

//FIXME!!
//undocumented routine in libupgbuf.so
unsigned short icksum(unsigned short cascade_sum, char *addr, int count);

// cmake=0x02, sectionid=0x02, magic=0x2B24
#if BYTE_ORDER == LITTLE_ENDIAN
	#define PSECTION_SIGNATURE      0x2B240202
#elif BYTE_ORDER == BIG_ENDIAN
	#define PSECTION_SIGNATURE      0x02022B24
#else
	#error "Unknown byte order"
#endif

/* Utility */
struct csid_t {
	uint32_t csid;
	uint16_t size;
	char data[1];
} __attribute__((__packed__));

static const uint32_t CSID_END = 0;

#define STAG_CSXF_HDRLEN	(sizeof (RDS00014_STAG_HEADER) + sizeof(RDS00043_CSXF_HEADER))

/* putpsec RW command, RDS00043 */
#define COMMAND_RAM				0x00000000
#define COMMAND_RAM2FLASH		0x80000000
#define COMMAND_MASK			0x80000000

#define	GET_CMD(x)				((x) & COMMAND_MASK)
#define	GET_ENC(x)				((x) & ~COMMAND_MASK)

/* function definition */
#define	PSEC_MAX_SIZE			2048
int putpsec (unsigned long search_from, struct csid_t *inid, unsigned int RW);
int getpsec_CSID (unsigned long search_from, struct csid_t *inid, unsigned int RW); //Bruce 2014/01/17 for IDE701AM
int getpsec (unsigned long search_from, char *fname);
int upgpsec (unsigned long search_from, unsigned char *buf);

/* Utility functions */
void dump_csxf (RDS00043_CSXF_HEADER *csxf);
void dump_csid (struct csid_t *pcsid, int len);
void dump_stag (RDS00014_STAG_HEADER *stag);
unsigned long int find_p_sec (unsigned long int start);

/* dearrange/arragne RDS00035 */
void jse_encode(uint32_t *src, uint32_t *enc, int size, uint32_t seed);
void jse_decode(uint32_t *src, uint32_t *dec, int size, uint32_t seed);

#define	ARRANGE_SIZE		64
#define	SEED				0x20081205
#define	ENCODING_SIZE(x)	( ((int)(x/ARRANGE_SIZE) + 1) * (ARRANGE_SIZE) )

/* This function includes all of the decoding methods. */
int decoding (unsigned char *buf, unsigned char *dec);

#define	TRUE		1
#define	FALSE		(-1)

#endif // __PSEC_H__
