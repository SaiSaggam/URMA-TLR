

#ifndef _AMITFLASH_
#define _AMITFLASH_


#define FLASH_DONE	0
#define FLASH_OPEN_ERR -1
#define FLASH_ERASE_ERR -2
#define FLASH_WRITE_ERR -3
#define FLASH_VERIFY_ERR -4

void flash_find_sector(unsigned long address_start, unsigned long *sector_start, unsigned long *sector_size);
int flash_check_the_same_sector(unsigned long erase_start, unsigned long write_start);

int flash_writeonesector(uint32_t *, uint32_t *, unsigned char *upgbuf);
int flash_eraseonesector(uint32_t *, uint32_t *, unsigned char *upgbuf);

int flash_open(void);
int flash_close(void);

int flash_write(unsigned char *buf, unsigned long start, unsigned long length);
int flash_read (unsigned char *buf, unsigned long start, unsigned long length);

int flash_info_bottom(void);
unsigned long flash_info_size(void);

#endif /* _AMITFLASH_ */

