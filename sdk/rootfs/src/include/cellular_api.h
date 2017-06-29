#ifndef _CELLULAR_API_H_
#define _CELLULAR_API_H_

#include <stddef.h>

#define OMA_MAX_STRING_LEN			256
#define	FIRMWARE_MAX_STRING_LEN		64


typedef struct {
	char	HostManufacture[OMA_MAX_STRING_LEN];
	char	HostModelName[OMA_MAX_STRING_LEN];
	char	HostSoftwareVersion[OMA_MAX_STRING_LEN];
	char	HostPlasmaID[OMA_MAX_STRING_LEN];
	char	OSName[OMA_MAX_STRING_LEN];
	char	OSVersion[OMA_MAX_STRING_LEN];
}oma_info_t;


typedef struct {
	char 	version[FIRMWARE_MAX_STRING_LEN];
	char 	unique_id[FIRMWARE_MAX_STRING_LEN];
	char 	carrier_name[FIRMWARE_MAX_STRING_LEN];
} cellular_firmware_info_t;



int oma_set_info(
	size_t		const USB_index,
	oma_info_t	const * const OMA);
	
	
int oma_get_info(
	size_t		const USB_index,
	oma_info_t	* const OMA);
	

int cellular_get_firmware_list(
	size_t  					const USB_index,
	cellular_firmware_info_t 	** firmware_info, 
	size_t 						* const firmware_info_count);
	

int cellular_get_current_firmware(
	size_t  					const USB_index,
	cellular_firmware_info_t	* const firmware_info);
	

int cellular_get_preferred_firmware(
	size_t  					const USB_index,
	cellular_firmware_info_t	* const firmware_info);
	

/*
* carrier_name = contains name of the carrier to switch to
*/
int cellular_set_current_firmware_carrier(
	size_t 		const USB_index, 
	char const 	* const carrier_name);
	

#endif