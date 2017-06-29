
/**
	這份 .h 文件必須同步於 ../include/ 目錄中的 .h
	
	flash 的 Sector size 為 64KB ==> BUF_DATA_SIZE
	flashbuf 的 implemetation 有幾項規則
	0.flashbuf是以 UPGBUF_SIZE 為一個基本單位.至於畸零Sector的處理是交給flash.c處理.
	1.一旦，資料收滿一個 UPGBUF_SIZE 後,才會寫入flash.
	2.收到完整的image後也會寫入flash.
	3. erase_start+eraselength 的範圍 必須涵蓋 write_start+write_length 
**/

#ifndef _AMITFLASHBUF_
#define _AMITFLASHBUF_

#include "rds/00012.h"

#if NAND_FLASH_SELECT
/* Here, define some parameters */
#define SECTOR_SIZE	0x20000		//64KBytes
#else
/* Here, define some parameters */
#define SECTOR_SIZE	0x10000		//64KBytes
#endif

#define UPGBUF_SIZE	SECTOR_SIZE

#define UPG_HANDLE	3347		//it just a random number, great than 0  

/*****************************************************/

#define UPG_HEADER_MAJOR 		RDS00012_AUH20_MAJOR_VERSION
#define UPG_HEADER_ID 			RDS00012_AUH20_MAGIC
#define ROMID_CID_LEN			RDS00012_ROMID_CUST_ID_LEN

int open_upgbuf(int flags);
int write_upgbuf(int handle, const void *buf, size_t count);
int close_upgbuf(int handle);  
int read_upgbuf(int handle, unsigned char *buf , unsigned long start, unsigned long length);

#if defined(GET_FW_BEFORE_UPG)
int open_upgfile(int flags);
int write_upgfile(int handle, const void *buf, size_t count);
int read_upgfile(int handle, unsigned char *buf , unsigned long start, unsigned long length);
int close_upgfile(int handle);

int exec_upgfile(const char *filename, const int flags);
#endif

#if defined(ENABLE_FWUPG_FILE_CHECKSUM)
int check_upgbuf(int handle, const void *buf, size_t count, int ResetFlag);
int check_firmware(const char* filepath, const int flags);
#endif

typedef RDS00012_AUH20_HEADER	UPGRADE_HEADER;

#define UPGHEADER_SIZE			(sizeof(UPGRADE_HEADER))
#define UPGHEADER_SIZE_EXTRA	40 

/*
	5.1 檔案錯誤  ( -100 ~ -199 ) => defined by RDS/00012

    5.2 版本錯誤 ( -200 ~ -299 )  => defined by RDS/00012

    5.3 硬體錯誤 ( -300 ~ -399 )

    5.4 其他錯誤 ( -400 ~ -499 )

*/


#define SUC_UPGBUF_DONE			0

#define ERR_ARRANGE				RDS00012_ERR_ARRANGE
#define ERR_HDR_ID  			RDS00012_ERR_HEADER_ID			/* header_id error */
#define ERR_HDR_CHKSUM 			RDS00012_ERR_HEADER_CHECKSUM	/* header checksum error */
#define ERR_HDR_MAJOR			RDS00012_ERR_MAJOR_VERSION		/* Not MAJOR=2.x version */
#define ERR_IMAGE_CHKSUM		RDS00012_ERR_IMAGE_CHECKSUM		/* image checksum error */
#define ERR_ERASE_RANGE			RDS00012_ERR_ERASE_DIRECTIVE	/* write 的範圍超出 erase 的範圍 */
#define ERR_DATA_RANGE			RDS00012_ERR_DATA_DIRECTIVE		/* write 的範圍超出 erase 的範圍 */
#define ERR_IMG_INFO_OFFSET		RDS00012_ERR_IMAGE_INFO_OFFSET
#define ERR_FAMILY_MEMBER		RDS00012_ERR_FAMILY_MEMBER		/* family member error */
#define ERR_ROM_ID				RDS00012_ERR_ROM_ID				/* romid error */

#define ERR_LPVS				RDS00012_ERR_LPVS
#define ERR_IMG_INFO_TYPE		RDS00012_ERR_IMAGE_INFO_TYPE	/* not supported image info type */

#define ERR_FLASH_ERASE			-300	/* flash erase error */
#define ERR_FLASH_WRITE			-301 	/* flash write error */
#define ERR_FLASH_READ			-302	/* flash read error */ 	

#define ERR_UPGBUF_BADFD		-400	/* the handle of upgbuf 錯誤 */
#define ERR_UPGBUF_OPEN			-401    /* open upgbuf 錯誤 */

#endif		/* _AMITFLASHBUF_ */

