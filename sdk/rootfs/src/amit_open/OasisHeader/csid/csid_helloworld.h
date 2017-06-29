//#This section is for RELEASE IN SDK COPY
/*
 * @file Member ID Definition of Group helloworld
 * 
 * Moderator: 
 * Group ID: 0x00A00000/0x80A00000
 */

#ifndef _CSID_HELLOWORLD_H
#define _CSID_HELLOWORLD_H

#include "csid_gid.h"

#define _CFG_HELLOWORLD(x)     (CSID_GIDC_HELLOWORLD|(x))	//save in ROM
#define _STA_HELLOWORLD(x)     (CSID_GIDS_HELLOWORLD|(x))	//save in RAM

/* Configuration Items */
#define CSID_C_HELLOWORLD_ROM_STRING      	_CFG_HELLOWORLD(0x0001) //T=str, for demo
#define CSID_C_HELLOWORLD_ROM_ENABLE       	_CFG_HELLOWORLD(0x0002) //T=u8, HELLOWORLD enable, 1:yes 0:no
#define CSID_C_HELLOWORLD_ROM_SELECT        _CFG_HELLOWORLD(0x0003) //T=u8,  select

/* For TEST rule setting, reserved 256 items */
#define CSID_C_HELLOWORLD_ROM_RULE_ENABLE  	_CFG_HELLOWORLD(0x3000) //T=u8,  enable/disable HELLOWORLD rules
#define CSID_C_HELLOWORLD_ROM_RULE_IP      	_CFG_HELLOWORLD(0x3100) //T=ipv4, HELLOWORLD IP
#define CSID_C_HELLOWORLD_ROM_RULE_ID      	_CFG_HELLOWORLD(0x3200) //T=u16, HELLOWORLD ID
#define CSID_C_HELLOWORLD_ROM_RULE_NAME    	_CFG_HELLOWORLD(0x3300) //T=str, HELLOWORLD NAME

/*****************/

/* Status Items */
#define CSID_S_HELLOWORLD_RAM_ALTERED	_STA_HELLOWORLD(0x0001) //T=u8, ui altered

// To test csid output format
#define CSID_S_HELLOWORLD_RAM_U32		_STA_HELLOWORLD(0x0010)	//T=u32, u32 type csid
#define CSID_S_HELLOWORLD_RAM_U16		_STA_HELLOWORLD(0x0020)	//T=u16, u16 type csid
#define CSID_S_HELLOWORLD_RAM_U8		_STA_HELLOWORLD(0x0030)	//T=u8, u8 type csid
#define CSID_S_HELLOWORLD_RAM_S32		_STA_HELLOWORLD(0x0040)	//T=s32, s32 type csid
#define CSID_S_HELLOWORLD_RAM_S16		_STA_HELLOWORLD(0x0050)	//T=s16, s16 type csid
#define CSID_S_HELLOWORLD_RAM_S8		_STA_HELLOWORLD(0x0060)	//T=s8, s8 type csid
#define CSID_S_HELLOWORLD_RAM_IPV4		_STA_HELLOWORLD(0x0070)	//T=ipv4, ipv4 type csid
#define CSID_S_HELLOWORLD_RAM_MAC		_STA_HELLOWORLD(0x0080)	//T=mac, mac type csid
#define CSID_S_HELLOWORLD_RAM_BIN		_STA_HELLOWORLD(0x0090)	//T=bin, bin type csid
#define CSID_S_HELLOWORLD_RAM_STR		_STA_HELLOWORLD(0x00A0)	//T=str, str type csid

// To test default value (in csidbase)
#define CSID_S_HELLOWORLD_RAM_U32_D		_STA_HELLOWORLD(0x0110)	//T=u32, D=32, u32 with default
#define CSID_S_HELLOWORLD_RAM_U16_D		_STA_HELLOWORLD(0x0120)	//T=u16, D=16, u16 with default
#define CSID_S_HELLOWORLD_RAM_U8_D		_STA_HELLOWORLD(0x0130)	//T=u8, D=8, u8 with default
#define CSID_S_HELLOWORLD_RAM_S32_D		_STA_HELLOWORLD(0x0140)	//T=s32, D=-32, s32 with default
#define CSID_S_HELLOWORLD_RAM_S16_D		_STA_HELLOWORLD(0x0150)	//T=s16, D=-16, s16 with default
#define CSID_S_HELLOWORLD_RAM_S8_D		_STA_HELLOWORLD(0x0160)	//T=s8, D=-8, s8 with default
#define CSID_S_HELLOWORLD_RAM_IPV4_D	_STA_HELLOWORLD(0x0170)	//T=ipv4, D={192.168.10.24}, ipv4 with default
#define CSID_S_HELLOWORLD_RAM_MAC_D		_STA_HELLOWORLD(0x0180)	//T=mac, D={00-50-18-03-06-14}, mac with default
#define CSID_S_HELLOWORLD_RAM_BIN_D		_STA_HELLOWORLD(0x0190)	//T=bin, D={05-04-11-12-10-24-05-30}, bin with default
#define CSID_S_HELLOWORLD_RAM_STR_D		_STA_HELLOWORLD(0x01A0)	//T=str, D="Default String", str with default



#endif //ifndef _CSID_HELLOWORLD_H
