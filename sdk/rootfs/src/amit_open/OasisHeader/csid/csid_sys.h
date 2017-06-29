/*
 * @file Member ID Definition of Group SYS.
 *
 * Moderator: WumingWu/RupertLi
 * Group ID: 0x80000000
 */

#ifndef _CSID_SYS_H
#define _CSID_SYS_H

#include "csid_gid.h"

#define _CFG_SYS(x)         (CSID_GIDC_SYS|(x))
#define _STA_SYS(x)         (CSID_GIDS_SYS|(x))

//AMIT default LAN IP and netmask (in byte order of host)
#define DEFAULT_AMIT_LAN_IP         0xC0A87BFE //192.168.123.254
#define DEFAULT_AMIT_LAN_NETMASK    0xFFFFFF00 //255.255.255.0

/* Config Items */
#define CSID_C_SYS_INFO_TELNET_PASSWD   _CFG_SYS(0x0001)   //T=str, telnet password
#define CSID_C_SYS_WATCHDOG_DISABLE     _CFG_SYS(0x0002)   //T=u8, watchdog (0: enable, 1: disable)
#define CSID_C_SYS_WATCHDOG_TIMEOUT     _CFG_SYS(0x0003)   //T=u32, watchdog (0: default 60sec, other value: sec)
#define CSID_C_SYS_ENABLE_GIGA_SWITCH   _CFG_SYS(0x0004)   //T=u8, D=0, whether to enable Giga switch (if any)
#define CSID_C_SYS_GIGA_SWITCH_TYPE   	_CFG_SYS(0x0005)   //T=str, what giga switch it is.
#define CSID_C_SYS_WATCHDOG_KMODE       _CFG_SYS(0x0006)   //T=u8, kernel mode watchdog

//jay add code for FW1 or FW2 check 120420
#define CSID_C_TEST_FW_STR	_CFG_SYS(0x0007)	//T=u32, start FW section (For check start FW section), 1:FW2 0:FW1
#define CSID_C_TEST_UPG_SUC	_CFG_SYS(0x0008)	//T=u32, FW upgrade success flag (For check FW upgrade success flag), 1:FW2 0:FW1

/* D-Link reset button mode */
#define CSID_C_SYS_DLK_RESET_MODE_ENABLE   _CFG_SYS(0x0009)   //T=u32, D=0, RESET button mode 0:disable, 1:enable
#define CSID_C_SYS_RGMII_SPEED   		       _CFG_SYS(0x000A)   //T=u32, D=0, 0:1000M, 1:100M, 2:10M
#define CSID_C_SYS_WORLDWIDE_MODE_SETTED   _CFG_SYS(0x000B)   //T=u32, D=0, Whether world wild mode is setted, 0: Setted, 1: Not Set

/* add for set system*/
#define CSID_C_SYS_CUSTOMIZE_FLAG	   _CFG_SYS(0x000C)   //T=u32, D=0, add for set system

/* system name in System Related UI*/
#define CSID_C_SYS_SYSTEM_NAME  _CFG_SYS(0x000D)    //T=str, system name in System Related UI

#define CSID_C_SYS_CPU_NUM     _CFG_SYS(0x000E)   //T=u8, D=4, number of CPUs

/* usb storage*/
#define CSID_C_SYS_USB_STORAGE_ENABLE  _CFG_SYS(0x0010)    //T=u32, usb storage enable

/*data usage*/
#define CSID_C_SYS_DATA_USAGE_CHKTIME            _CFG_SYS(0x00f0)    //T=u32, D=0, data usage check time
#define CSID_C_SYS_DATA_USAGE_EMAIL_ENABLE       _CFG_SYS(0x00f1)    //T=u32, D=0, data usage send email enable
#define CSID_C_SYS_DATA_USAGE_ENABLE             _CFG_SYS(0x0100)    //T=u32, D=0, data usage enable
#define CSID_C_SYS_DATA_USAGE_NAME               _CFG_SYS(0x0120)    //T=str, data usage carrier name
#define CSID_C_SYS_DATA_USAGE_3G_INDEX           _CFG_SYS(0x0140)    //T=u32, D=0, data usage sim select 3G index
#define CSID_C_SYS_DATA_USAGE_SIM                _CFG_SYS(0x0160)    //T=u32, D=0, data usage sim select, simA:1 simB:2
#define CSID_C_SYS_DATA_USAGE_CYCLE_PERIOD       _CFG_SYS(0x0180)    //T=u32, D=0, data usage billing cycle period value
#define CSID_C_SYS_DATA_USAGE_CYCLE_PERIOD_UNIT  _CFG_SYS(0x01a0)    //T=u32, D=0, data usage billing cycle period unit, hours:1 weekly:2 monthly:3
#define CSID_C_SYS_DATA_USAGE_STARTTIME          _CFG_SYS(0x01c0)    //T=u32, D=0, data usage cycle period start
#define CSID_C_SYS_DATA_USAGE_DATA_LIMIT         _CFG_SYS(0x01e0)    //T=u32, D=0, data usage cycle period data limitation value
#define CSID_C_SYS_DATA_USAGE_DATA_LIMIT_UNIT    _CFG_SYS(0x0200)    //T=u32, D=0, data usage cycle period data limitation unit, kb:1 mb:2 gb:3
#define CSID_C_SYS_DATA_USAGE_CONNECT_RESTRICT   _CFG_SYS(0x0220)    //T=u32, D=0, data usage billing cycle period limit enable


/* Status Items */

//==========================================================
// 0x0000~0x3FFF: Reserved.
//==========================================================

//==========================================================
// 0x4000~0x40FF: Section Address (please ref RDS/00018)
//==========================================================

#define CSID_S_SYS_SECTION_B        _STA_SYS(0x4001) //T=u32, The address of the B section
#define CSID_S_SYS_SECTION_P        _STA_SYS(0x4002) //T=u32, The address of the P section
#define CSID_S_SYS_SECTION_R        _STA_SYS(0x4003) //T=u32, The address of the R section
#define CSID_S_SYS_SECTION_A        _STA_SYS(0x4004) //T=u32, The address of the A section
#define CSID_S_SYS_SECTION_D        _STA_SYS(0x4005) //T=u32, The address of the D section
#define CSID_S_SYS_SECTION_E        _STA_SYS(0x4006) //T=u32, The address of the E section
#define CSID_S_SYS_SECTION_C        _STA_SYS(0x4007) //T=u32, The address of the C section
#define CSID_S_SYS_SECTION_F        _STA_SYS(0x4008) //T=u32, The address of the F section
#define CSID_S_SYS_SECTION_X        _STA_SYS(0x4009) //T=u32, The address of the X section

#define CSID_S_SYS_SECTION_X_SIZE   _STA_SYS(0x40FE) //T=u32, D=65536, The size of the X section, Default 64k
#define CSID_S_SYS_SECTION_C_SIZE	_STA_SYS(0x40FF) //T=u32, D=65536, The size of the C section, Default 64k 

//==========================================================
// 0x4100~0x41FF: VIDI's CSID (maximum count is 256)
//==========================================================

#define CSID_S_SYS_VIDI_CSID        _STA_SYS(0x4100) //T=u32, Base CSID for VIDI CSIDs

//==========================================================
// 0x4200~0x42FF: System Information
//==========================================================

//items defined by SPO or FW engineers (permanent)
#define CSID_S_SYS_INFO_MASCOT      _STA_SYS(0x4200) //T=str, Codename of the project
#define CSID_S_SYS_INFO_FMID        _STA_SYS(0x4201) //T=u16, Family/Member ID
#define CSID_S_SYS_INFO_TP			_STA_SYS(0x4202) //T=u8, TARGET_PSERIES

//items defined by SPO or FW engineers (per each release)
#define CSID_S_SYS_INFO_FW_VER      _STA_SYS(0x4208) //T=str, firmware version (section A)
#define CSID_S_SYS_INFO_FW_REV      _STA_SYS(0x4209) //T=str, firmware source code revision (svn)
#define CSID_S_SYS_INFO_CSID_REV    _STA_SYS(0x420A) //T=str, firmware CSID revision (svn)
#define CSID_S_SYS_INFO_BOOT_VER    _STA_SYS(0x420B) //T=str, firmware boot version (section B)
#define CSID_S_SYS_INFO_BOOT_PNAME  _STA_SYS(0x420C) //T=str, firmware boot project name (section B)
//items defined by SPO or UI engineers (permanent)
#define CSID_S_SYS_INFO_ROM_ID      _STA_SYS(0x4210) //T=str, ID of ROM image

//items defined by SPO or UI engineers (per each release)
#define CSID_S_SYS_INFO_ECN_NO      _STA_SYS(0x4218) //T=str, ECN number
#define CSID_S_SYS_INFO_PV          _STA_SYS(0x4219) //T=u8, platform version

//items defined by customer (permanent)
#define CSID_S_SYS_INFO_MODEL_NAME  _STA_SYS(0x4220) //T=str, model name
#define CSID_S_SYS_INFO_EID_RULE    _STA_SYS(0x4221) //T=u16, rule to apply Ether ID
#define CSID_S_SYS_INFO_DEFAULT_IP  _STA_SYS(0x4222) //T=ipv4, default IP address
#define CSID_S_SYS_INFO_DEFAULT_AP_IP	_STA_SYS(0x4223) //T=ipv4, default IP address
#define CSID_S_SYS_INFO_DEVICE_MODE		_STA_SYS(0x4224) //T=u32, Device Mode, 1:Router Mode, 2:AP Mode, 3: STA Mode
#define CSID_S_SYS_INFO_DEVICE_NAME		_STA_SYS(0x4225) //T=str, Device Name in AP Mode
#define CSID_S_SYS_INFO_MODEL_HW_NAME	_STA_SYS(0x4226) //T=str, model name and hardware version 
#define CSID_S_SYS_INFO_MODEL_NAME_EXTRA	_STA_SYS(0x4227) //T=str, extra string append to modem name 

//items defined by customer (per each release)
#define CSID_S_SYS_INFO_UI_VER      _STA_SYS(0x4228) //T=str, UI version (section D)

//reset to default flag
#define CSID_S_SYS_RESET_MODE_ENABLE      _STA_SYS(0x422a) //T=u32, reset to default mode

//items defined by MP lines (per device)
#define CSID_S_SYS_INFO_BEID        _STA_SYS(0x4230) //T=mac, burn-in Ether ID

//UPNP items defined by customer 
#define CSID_S_SYS_UPNP_ICON_NAME          _STA_SYS(0x4231)   //T=str, define customer icon name
#define CSID_S_SYS_UPNP_MODEL_NAME         _STA_SYS(0x4232)   //T=str, define customer mode name
#define CSID_S_SYS_UPNP_MODEL_NUMBER       _STA_SYS(0x4233)   //T=str, define customer mode number
#define CSID_S_SYS_UPNP_MANUFACTURER       _STA_SYS(0x4234)   //T=str, define customer manufacturer
#define CSID_S_SYS_UPNP_DESCRIPTIONS       _STA_SYS(0x4235)   //T=str, define customer descriptions
#define CSID_S_SYS_UPNP_MANUFACTURER_URL   _STA_SYS(0x4236)   //T=str, define custormer manufacturer URL
#define CSID_S_SYS_UPNP_SERIAL_NUMBER      _STA_SYS(0x4237)   //T=str, define custormer serial number
#define CSID_S_SYS_UPNP_MODELURL           _STA_SYS(0x4238)   //T=str, define custormer model URL
#define CSID_S_SYS_INFO_RTCPPM             _STA_SYS(0x4239)   //T=u8,  define rtc ppm calculator

//==========================================================
// Hardware Variation
//==========================================================
#define HW_VARIATION_PCBLAYER_4 0x00000000
#define HW_VARIATION_PCBLAYER_2 0x00000001

#define CSID_S_SYS_HW_VARIATION     _STA_SYS(0x4240) //T=u32, Hardware Variation

#define CSID_S_SYS_INFO_DRMNB        _STA_SYS(0x4250) //T=mac, burn-in Ether ID

#define CSID_S_SYS_INFO_BEID_VERSION		_STA_SYS(0x4260) //T=u16, burn-in Ether ID Version
#define CSID_S_SYS_INFO_BEID_STATUS			_STA_SYS(0x4261) //T=u16, burn-in Ether ID Status
#define CSID_S_SYS_INFO_MFC_LOCK			_STA_SYS(0x4262) //T=u8, mfc command lock on flag

//==========================================================
// UPNP AV items defined by customer
//==========================================================
#define CSID_S_SYS_UPNP_AV_MODEL_NAME         _STA_SYS(0x4270)   //T=str, define customer mode name
#define CSID_S_SYS_UPNP_AV_MODEL_NUMBER       _STA_SYS(0x4271)   //T=str, define customer mode number
#define CSID_S_SYS_UPNP_AV_MANUFACTURER       _STA_SYS(0x4272)   //T=str, define customer manufacturer
#define CSID_S_SYS_UPNP_AV_DESCRIPTIONS       _STA_SYS(0x4273)   //T=str, define customer descriptions
#define CSID_S_SYS_UPNP_AV_MANUFACTURER_URL   _STA_SYS(0x4274)   //T=str, define custormer manufacturer URL
#define CSID_S_SYS_UPNP_AV_SERIAL_NUMBER   _STA_SYS(0x4275)   //T=str, define custormer serial number
#define CSID_S_SYS_UPNP_AV_MODELURL   _STA_SYS(0x4276)   //T=str, define custormer model URL

/* Royalty Key */
#define CSID_S_SYS_INFO_ROYALTY_KEY   		_STA_SYS(0x4280)   //T=str, royalty key

/* WPA PSK Key */
#define CSID_S_SYS_WPAPSK   		_STA_SYS(0x4281)   //T=str, define custormer wpa psk value

/* WPS Key */
#define CSID_S_SYS_WPS_KEY   		_STA_SYS(0x4282)   //T=str, define custormer wps key value

/* LOGIN Key */
#define CSID_S_SYS_LOGIN_KEY   		_STA_SYS(0x4283)   //T=str, define custormer login key value

/*data usage*/
#define CSID_S_SYS_DATA_USAGE_RESTRICT_ENABLE  _STA_SYS(0x4300)   //T=u32, D=0, data usage tx bytes
#define CSID_S_SYS_DATA_USAGE_RESTRICT_SIM     _STA_SYS(0x4310)   //T=u32, D=0, data usage rx tx total bytes
#define CSID_S_SYS_DATA_USAGE_SAVE_ALTER	   _STA_SYS(0x4320)   //T=u32, D=0, data usage rx tx total bytes
#define CSID_S_SYS_DATA_USAGE_RXDATA           _STA_SYS(0x4400)   //T=u32, D=0, data usage rx bytes
#define CSID_S_SYS_DATA_USAGE_TXDATA           _STA_SYS(0x4500)   //T=u32, D=0, data usage tx bytes
#define CSID_S_SYS_DATA_USAGE_TOTALDATA        _STA_SYS(0x4600)   //T=u32, D=0, data usage rx tx total bytes
#define CSID_S_SYS_DATA_USAGE_HOUR_INDEX         _STA_SYS(0x4700)   //T=u32, D=0, data usage rx bytes
#define CSID_S_SYS_DATA_USAGE_WEEKLY_INDEX       _STA_SYS(0x0720)   //T=u32, D=0, data usage tx bytes
#define CSID_S_SYS_DATA_USAGE_MONTHLY_INDEX      _STA_SYS(0x0740)   //T=u32, D=0, data usage rx tx total bytes
#define CSID_S_SYS_DATA_USAGE_LAST_LIMIT         _STA_SYS(0x0760)   //T=u32, D=0, data usage rx bytes
#define CSID_S_SYS_DATA_USAGE_EMAIL_80          _STA_SYS(0x0780)   //T=u32, D=0, data usage 80% send email
#define CSID_S_SYS_DATA_USAGE_EMAIL_100         _STA_SYS(0x07a0)   //T=u32, D=0, data usage 100% send email


//==========================================================
// 0xF000~0xF1FF:
//==========================================================

#define CSID_S_SYS_PORT_COUNT_ETH   _STA_SYS(0xF101) //T=u8, count of Ethernet ports
//#define CSID_S_SYS_PORT_COUNT_WLAN  _STA_SYS(0xF102) //T=u8, count of WLAN ports
//#define CSID_S_SYS_PORT_COUNT_USB   _STA_SYS(0xF103) //T=u8, count of USB ports
//#define CSID_S_SYS_PORT_COUNT_PRN   _STA_SYS(0xF104) //T=u8, count of PRINT ports
//#define CSID_S_SYS_PORT_COUNT_COM   _STA_SYS(0xF105) //T=u8, count of COM ports

//==========================================================
// 0xF200~0xF2FF: Network Hardware Configuration
//==========================================================

#define CSID_S_SYS_LAN_INTERFACE_NUM	_STA_SYS(0xF200)	//T=u8, number of LAN interface
#define CSID_S_SYS_LAN0_INTERFACE		_STA_SYS(0xF201)	//T=str, name of LAN0 interface
#define CSID_S_SYS_WAN_INTERFACE_NUM	_STA_SYS(0xF230)	//T=u8, number of WAN interface
#define CSID_S_SYS_WAN0_INTERFACE		_STA_SYS(0xF231)	//T=str, name of WAN0 interface
#define CSID_S_SYS_WAN1_INTERFACE		_STA_SYS(0xF232)	//T=str, name of WAN1 interface

//==========================================================
// 0xF300~0xF3FF: Logic Interface Configuration
//==========================================================

#define CSID_S_SYS_IF_BR_NUM			_STA_SYS(0xF300)	//T=u8, number of Bridge interface
#define CSID_S_SYS_IF_BR0				_STA_SYS(0xF301)	//T=str, name of the BR0
#define CSID_S_SYS_IF_BR1				_STA_SYS(0xF302)	//T=str, name of the BR1
#define CSID_S_SYS_IF_LAN_NUM			_STA_SYS(0xF330)	//T=u8, number of LAN interface
#define CSID_S_SYS_IF_LAN0				_STA_SYS(0xF331)	//T=str, name of LAN0 
#define CSID_S_SYS_IF_LAN0_MAC			_STA_SYS(0xF341)	//T=mac, MAC of LAN0
#define CSID_S_SYS_IF_WAN_NUM			_STA_SYS(0xF360)	//T=u8, number of WAN interface
#define CSID_S_SYS_IF_WAN0				_STA_SYS(0xF361)	//T=str, name of WAN0
#define CSID_S_SYS_IF_WAN1				_STA_SYS(0xF362)	//T=str, name of WAN1 
#define CSID_S_SYS_IF_WAN0_MAC			_STA_SYS(0xF371)	//T=mac, MAC of WAN0
#define CSID_S_SYS_IF_WAN1_MAC			_STA_SYS(0xF372)	//T=mac, MAC of WAN1
#define CSID_S_SYS_IF_WLAN_NUM			_STA_SYS(0xF390)	//T=u8, number of WLAN interface
#define CSID_S_SYS_IF_WLAN0				_STA_SYS(0xF391)	//T=str, name of WLAN0 
#define CSID_S_SYS_IF_WLAN0_MAC			_STA_SYS(0xF3A1)	//T=mac, MAC of WLAN0
#define CSID_S_SYS_IF_WLAN1				_STA_SYS(0xF392)	//T=str, name of WLAN0 
#define CSID_S_SYS_IF_WLAN1_MAC			_STA_SYS(0xF3A2)	//T=mac, MAC of WLAN1
#define CSID_S_SYS_IF_WLAN2				_STA_SYS(0xF393)	//T=str, name of WLAN2
#define CSID_S_SYS_IF_WLAN2_MAC			_STA_SYS(0xF3A3)	//T=mac, MAC of WLAN2
#define CSID_S_SYS_IF_WLAN3				_STA_SYS(0xF394)	//T=str, name of WLAN3
#define CSID_S_SYS_IF_WLAN3_MAC			_STA_SYS(0xF3A4)	//T=mac, MAC of WLAN4
#define CSID_S_SYS_IF_WLAN4				_STA_SYS(0xF395)	//T=str, name of WLAN4
#define CSID_S_SYS_IF_WLAN4_MAC			_STA_SYS(0xF3A5)	//T=mac, MAC of WLAN5
#define CSID_S_SYS_IF_WLAN5				_STA_SYS(0xF396)	//T=str, name of WLAN5
#define CSID_S_SYS_IF_WLAN5_MAC			_STA_SYS(0xF3A6)	//T=mac, MAC of WLAN5
#define CSID_S_SYS_IF_WLAN6				_STA_SYS(0xF397)	//T=str, name of WLAN6
#define CSID_S_SYS_IF_WLAN6_MAC			_STA_SYS(0xF3A7)	//T=mac, MAC of WLAN6
#define CSID_S_SYS_IF_WLAN7				_STA_SYS(0xF398)	//T=str, name of WLAN7
#define CSID_S_SYS_IF_WLAN7_MAC			_STA_SYS(0xF3A8)	//T=mac, MAC of WLAN7
#define CSID_S_SYS_IF_WLAN8				_STA_SYS(0xF399)	//T=str, name of WLAN8
#define CSID_S_SYS_IF_WLAN8_MAC			_STA_SYS(0xF3A9)	//T=mac, MAC of WLAN8
#define CSID_S_SYS_IF_WLAN9				_STA_SYS(0xF39A)	//T=str, name of WLAN9
#define CSID_S_SYS_IF_WLAN9_MAC			_STA_SYS(0xF3AA)	//T=mac, MAC of WLAN9
#define CSID_S_SYS_IF_WLAN10			_STA_SYS(0xF39B)	//T=str, name of WLAN10
#define CSID_S_SYS_IF_WLAN10_MAC		_STA_SYS(0xF3AB)	//T=mac, MAC of WLAN10
#define CSID_S_SYS_IF_WLAN11			_STA_SYS(0xF39C)	//T=str, name of WLAN11
#define CSID_S_SYS_IF_WLAN11_MAC		_STA_SYS(0xF3AC)	//T=mac, MAC of WLAN11
#define CSID_S_SYS_IF_WLAN12			_STA_SYS(0xF39D)	//T=str, name of WLAN12
#define CSID_S_SYS_IF_WLAN12_MAC		_STA_SYS(0xF3AD)	//T=mac, MAC of WLAN12
#define CSID_S_SYS_IF_WLAN13			_STA_SYS(0xF39E)	//T=str, name of WLAN13
#define CSID_S_SYS_IF_WLAN13_MAC		_STA_SYS(0xF3AE)	//T=mac, MAC of WLAN13
#define CSID_S_SYS_IF_WLAN14			_STA_SYS(0xF39F)	//T=str, name of WLAN14
#define CSID_S_SYS_IF_WLAN14_MAC		_STA_SYS(0xF3AF)	//T=mac, MAC of WLAN14
#define CSID_S_SYS_IF_WLAN15			_STA_SYS(0xF3A0)	//T=str, name of WLAN15
#define CSID_S_SYS_IF_WLAN15_MAC		_STA_SYS(0xF3B0)	//T=mac, MAC of WLAN15
#define CSID_S_SYS_IF_GLAN_NUM			_STA_SYS(0xF3C0)	//T=u8, number of GLAN interface
#define CSID_S_SYS_IF_GLAN0				_STA_SYS(0xF3C1)	//T=str, name of GLAN0
#define CSID_S_SYS_IF_GLAN0_MAC			_STA_SYS(0xF3D1)	//T=mac, MAC of GLAN0
#define CSID_S_SYS_IF_WAN_MAC			_STA_SYS(0xF3E1)	//T=mac, MAC of WAN

//==========================================================
// 0xF400~0xF40F: System Memory Information
//==========================================================

#define CSID_S_SYS_MEM_TOTAL			_STA_SYS(0xF400)	//T=u32, System Total memory
#define CSID_S_SYS_MEM_USED			_STA_SYS(0xF401)	//T=u32, System Used memory
#define CSID_S_SYS_MEM_FREE			_STA_SYS(0xF402)	//T=u32, System Free memory
#define CSID_S_SYS_CPU_USAGE_STR		_STA_SYS(0xF403)    //T=str, System CPU Usage, it will show string format, e.g. 6%
#define CSID_S_SYS_CPU0_USAGE_STR		_STA_SYS(0xF404)    //T=str, System CPU0 Usage, it will show string format, e.g. 6%
#define CSID_S_SYS_CPU1_USAGE_STR		_STA_SYS(0xF405)    //T=str, System CPU1 Usage, it will show string format, e.g. 6%
#define CSID_S_SYS_CPU2_USAGE_STR		_STA_SYS(0xF406)    //T=str, System CPU2 Usage, it will show string format, e.g. 6%
#define CSID_S_SYS_CPU3_USAGE_STR		_STA_SYS(0xF407)    //T=str, System CPU3 Usage, it will show string format, e.g. 6%
#define CSID_S_SYS_CPU0_USAGE   		_STA_SYS(0xF410)    //T=u32, System CPU Usage
#define CSID_S_SYS_CPU0_USAGE_MIN		_STA_SYS(0xF411)    //T=u32, System minimum CPU Usage ,
#define CSID_S_SYS_CPU0_USAGE_MAX		_STA_SYS(0xF412)    //T=u32, System maximum CPU Usage,
#define CSID_S_SYS_CPU0_USAGE_AVG		_STA_SYS(0xF413)    //T=u32, System average CPU Usage,

//==========================================================
// 0xF500~0xF50F: Wireless Hardward Information
//========================================================== 
#define CSID_S_SYS_WLAN_EEPROM			_STA_SYS(0xF500)	//T=bin, wireless EEPROM value (K power)
#define CSID_S_SYS_WLAN2_EEPROM			_STA_SYS(0xF501)	//T=bin, wireless EEPROM value (K power)
#define CSID_S_SYS_RSN0			        _STA_SYS(0xF510)	//T=str,Netcomm rsn for wlan0
#define CSID_S_SYS_RSN1			        _STA_SYS(0xF511)	//T=str,Netcomm rsn for wlan1
#define CSID_S_SYS_SERIAL_NUMBER        _STA_SYS(0xF512)    //T=str, machine serial number
#define CSID_S_SYS_EDCCA_SUPPORT        _STA_SYS(0xF513)    //T=u32, ED CCA function setting in ODM, for EN certification

/* for customer use */
#define CSID_S_SYS_PIN_CODE             _STA_SYS(0xF520)    //T=str, machine pincode

// Kernel and Rootfs and Web UI Section Checksum
#define CSID_S_SYS_ICHECKSUM			_STA_SYS(0xF600)	//T=str, Kernel and Roofs and Wev UI Section Checksum
#define CSID_S_SYS_BK_ICHECKSUM			_STA_SYS(0xF601)	//T=str, Dual image Kernel and Roofs and Wev UI Section Checksum

//For Mascot2
#define CSID_S_SYS_INFO_MASCOT2			    _STA_SYS(0xF700)	//T=str, Kernel and Roofs and Wev UI Section Checksum

#define CSID_S_SYS_WIFI_COUNT			    _STA_SYS(0xF701)	//T=u32, For Wireless init recoder
#define CSID_S_SYS_TC3162_BUTTON1			_STA_SYS(0xF702)	//T=u32, For Wireless init recoder

#define CSID_S_SYS_WORLDWIDE_MODE		    _STA_SYS(0xF703)	//T=u32, For WIreless world wide mode
#define CSID_S_SYS_INFO_REGULATORY_DOMAIN	_STA_SYS(0xF704)	//T=str, Wireless 2.4G Country Code Info for UI 
#define CSID_S_SYS_INFO_COUNTRY_CODE		_STA_SYS(0xF705)	//T=str, Wireless 5G Country Code Info for UI 

#define CSID_S_SYS_INFO_FW_license		 _STA_SYS(0xF706)	//T=str, FW License Info 
#define CSID_S_SYS_INFO_FW_MAC_START		 _STA_SYS(0xF707)	//T=str, FW License MAC START Info 
#define CSID_S_SYS_INFO_FW_MAC_END		 _STA_SYS(0xF708)	//T=str, FW License MAC END Info 
#define CSID_S_SYS_INFO_FW_LICENSE_SN		 _STA_SYS(0xF709)	//T=str, FW License SN Info 
#define CSID_S_SYS_INFO_FW_LICENSE_MODEL		 _STA_SYS(0xF70A)	//T=str, FW License MODEL Info 

#define CSID_S_SYS_MFG_PCBA_ATS_1        _STA_SYS(0xF70B)   // T=str, MFG ATS PCBA TEST Info (20 character) 
#define CSID_S_SYS_MFG_PCBA_ATS_2        _STA_SYS(0xF70C)   // T=str, MFG ATS PCBA TEST Info (20 character)  
#define CSID_S_SYS_MFG_PCBA_ATS_3        _STA_SYS(0xF70D)   // T=str, MFG ATS PCBA TEST Info (20 character)
#define CSID_S_SYS_MFG_PCBA_ATS_4        _STA_SYS(0xF70E)   // T=str, MFG ATS PCBA TEST Info (20 character)
#define CSID_S_SYS_MFG_PCBA_ATS_5        _STA_SYS(0xF70F)   // T=str, MFG ATS PCBA TEST Info (20 character)
                                                       
#define CSID_S_SYS_MFG_ASSY_ATS_1        _STA_SYS(0xF710)   // T=str, MFG ATS ASSY TEST Info (20 character) 
#define CSID_S_SYS_MFG_ASSY_ATS_2        _STA_SYS(0xF711)   // T=str, MFG ATS ASSY TEST Info (20 character)
#define CSID_S_SYS_MFG_ASSY_ATS_3        _STA_SYS(0xF712)   // T=str, MFG ATS ASSY TEST Info (20 character)
#define CSID_S_SYS_MFG_ASSY_ATS_4        _STA_SYS(0xF713)   // T=str, MFG ATS ASSY TEST Info (20 character)
#define CSID_S_SYS_MFG_ASSY_ATS_5        _STA_SYS(0xF714)   // T=str, MFG ATS ASSY TEST Info (20 character)
                                                        
#define CSID_S_SYS_MFG_FWUPG_FW          _STA_SYS(0xF715)   // T=str, MFG ATS FWUPG TEST Info (20 character)   
#define CSID_S_SYS_MFG_FWUPG_MAC         _STA_SYS(0xF716)   // T=str, MFG ATS FWUPG TEST Info (20 character)
#define CSID_S_SYS_MFG_FWUPG_OTHER_1     _STA_SYS(0xF717)   // T=str, MFG ATS FWUPG TEST Info (20 character)
#define CSID_S_SYS_MFG_FWUPG_OTHER_2     _STA_SYS(0xF718)   // T=str, MFG ATS FWUPG TEST Info (20 character)
#define CSID_S_SYS_MFG_FWUPG_OTHER_3     _STA_SYS(0xF719)   // T=str, MFG ATS FWUPG TEST Info (20 character)
                                                        
#define CSID_S_SYS_QC_ATS_1          _STA_SYS(0xF71A)   // T=str, QC ATS ASSY TEST Info (20 character)  
#define CSID_S_SYS_QC_ATS_2          _STA_SYS(0xF71B)   // T=str, QC ATS ASSY TEST Info (20 character)
#define CSID_S_SYS_QC_ATS_3          _STA_SYS(0xF71C)   // T=str, QC ATS ASSY TEST Info (20 character)
#define CSID_S_SYS_QC_ATS_4          _STA_SYS(0xF71D)   // T=str, QC ATS ASSY TEST Info (20 character)
#define CSID_S_SYS_QC_ATS_5          _STA_SYS(0xF71E)   // T=str, QC ATS ASSY TEST Info (20 character)
 
#endif //ifndef _CSID_SYS_H
