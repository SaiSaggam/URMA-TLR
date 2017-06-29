#ifndef __main_h_
#define __main_h_
#include <mqx.h>
#include <bsp.h>

#include <rtcs.h>
#ifdef APPLICATION_HAS_SHELL
#include <sh_rtcs.h>
#endif
#include <ipcfg.h>
#include <connector_config.h>

#define MAIN_TASK 				1
#define CONNECTOR_TASK 			2
#define FLASH_TASK 				3
#define SDCARD_TASK				4
#define USB_TASK				5

#ifdef USE_SSL
#define CONNECTOR_THREAD_STACK_SIZE		(4096 * 5)
#else
#define CONNECTOR_THREAD_STACK_SIZE		4096
#endif

#define ENET_DEVICE 0

extern void Main_task(uint_32);
extern void connector_thread(unsigned long initial_data);
extern void flash_task(unsigned long initial_data);
extern int application_start(void);

extern void sdcard_task(uint_32);
extern void USB_task(uint_32);
#endif

