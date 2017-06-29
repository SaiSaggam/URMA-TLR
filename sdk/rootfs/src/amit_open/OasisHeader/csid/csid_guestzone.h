/*
 * @file Member ID Definition of Group Wireless AP. 
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00970000/0x80970000
 */
 
#ifndef _CSID_GUESTZONE_H
#define _CSID_GUESTZONE_H

#include "csid_gid.h"

#define _CFG_GUESTZONE(x)		(CSID_GIDC_GUESTZONE|(x))
#define _STA_GUESTZONE(x)		(CSID_GIDS_GUESTZONE|(x))

/* Configuration Items */

/* DHCP Daemon */
/* DHCP Server (DHCPD) Basic Setting */
#define CSID_C_GUESTZONE_SVR_ENABLE				_CFG_GUESTZONE(0x0001)	//T=u32, D=1, Enable/Disable
#define CSID_C_GUESTZONE_SVR_LEASETIME			_CFG_GUESTZONE(0x0002)	//T=u32, D=86400, Lease Time (seconds)
#define CSID_C_GUESTZONE_SVR_IPPOOL_START		_CFG_GUESTZONE(0x0003)	//T=ipv4, D={192.168.2.100},  IP Pool Starting Address
#define CSID_C_GUESTZONE_SVR_IPPOOL_END			_CFG_GUESTZONE(0x0004)	//T=ipv4, D={192.168.2.200}, IP Pool Ending Address
#define CSID_C_GUESTZONE_SVR_DOMAIN				_CFG_GUESTZONE(0x0005)	//T=str, Domain Name
#define CSID_C_GUESTZONE_SVR_PRIDNS				_CFG_GUESTZONE(0x0006)	//T=ipv4, Primary DNS  
#define CSID_C_GUESTZONE_SVR_SECDNS				_CFG_GUESTZONE(0x0007)	//T=ipv4, Second DNS  
#define CSID_C_GUESTZONE_SVR_PRIWINS		  	_CFG_GUESTZONE(0x0008)	//T=ipv4, Primary WINS  
#define CSID_C_GUESTZONE_SVR_SECWINS		   	_CFG_GUESTZONE(0x0009)	//T=ipv4, Second WINS
#define CSID_C_GUESTZONE_SVR_GATEWAY		   	_CFG_GUESTZONE(0x000a)	//T=ipv4, Gateway default is LAN ip
#define CSID_C_GUESTZONE_SVR_MAXLEASE			_CFG_GUESTZONE(0x000b)	//T=u32, D=101, the Max. number of lease ip 
#define CSID_C_GUESTZONE_SVR_OPTION66_ENABLE   	_CFG_GUESTZONE(0x000c)	//T=u8, D=0, DHCP option 66 Enable/Disable
#define CSID_C_GUESTZONE_SVR_OPTION66_PATH		_CFG_GUESTZONE(0x000d)	//T=str, tftp server name
#define CSID_C_GUESTZONE_SVR_BRNAME         	_CFG_GUESTZONE(0x000e)	//T=str, D="br1", bridge name
#define CSID_C_GUESTZONE_SVR_IP             	_CFG_GUESTZONE(0x000f)	//T=ipv4, D={192.168.2.254}, IP Pool Starting Address

#define CSID_C_GUESTZONE_DUAL_ACCESS_SWITCH     _CFG_GUESTZONE(0x0010)	//T=u32, D=0, Enable Dual Host Access Switch(2.4G & 5G)

/* Guest Zone */
#define CSID_C_GUESTZONE_ENABLE		     		_CFG_GUESTZONE(0x0100)	//T=u32, Enable/Disable
#define CSID_C_GUESTZONE_INC_WIRELESS      		_CFG_GUESTZONE(0x0101)	//T=u32, Guest Zone include Wireless
#define CSID_C_GUESTZONE_CLIENT_ISOLATION  		_CFG_GUESTZONE(0x0102)	//T=u32, Guest Zone Client Isolation
#define CSID_C_GUESTZONE_ACCESS_HOSTZONE  		_CFG_GUESTZONE(0x0103)	//T=u32, Enable the access to Host Zone from Guest Zone
#define CSID_C_GUESTZONE_LOCK_CLIENT    		_CFG_GUESTZONE(0x0104)	//T=u32, Guest Zone Lock Client

#define CSID_C_GUESTZONE_LOCK_ACL_DENY  		_CFG_GUESTZONE(0x0105)  //T=u8, Locked Wireless Policy

/* Lock Client List 256 items are reserved */
#define CSID_C_GUESTZONE_LOCK_CLILIST_WIRELESS  _CFG_GUESTZONE(0x0200)  //T=mac, Locked Wireless Client List

/* Fixed Mapping */
#define CSID_C_GUESTZONE_SVR_FIXMAP_ENABLE		_CFG_GUESTZONE(0x1000)	//T=u8, D=0, Enable/Disable
#define CSID_C_GUESTZONE_SVR_FIXMAP_MAC			_CFG_GUESTZONE(0x1200)	//T=mac, Mapped MAC
#define CSID_C_GUESTZONE_SVR_FIXMAP_IP			_CFG_GUESTZONE(0x1400)	//T=ipv4, Mapped IP

/* Schedule */
#define CSID_C_GUESTZONE_RULE_SCHE_NO           _CFG_GUESTZONE(0x2000)  //T=u16, schedule number of using rule


/* Guest Zone for interfce 2 start =================*/
#define CSID_C_GUESTZONE2_ENABLE		     	_CFG_GUESTZONE(0x8100)	//T=u32, Enable/Disable
#define CSID_C_GUESTZONE2_INC_WIRELESS      	_CFG_GUESTZONE(0x8101)	//T=u32, Guest Zone include Wireless
#define CSID_C_GUESTZONE2_CLIENT_ISOLATION  	_CFG_GUESTZONE(0x8102)	//T=u32, Guest Zone Client Isolation
#define CSID_C_GUESTZONE2_ACCESS_HOSTZONE  		_CFG_GUESTZONE(0x8103)	//T=u32, Enable the access to Host Zone from Guest Zone
#define CSID_C_GUESTZONE2_LOCK_CLIENT    		_CFG_GUESTZONE(0x8104)	//T=u32, Guest Zone Lock Client

/* Lock Client List 256 items are reserved */
#define CSID_C_GUESTZONE2_LOCK_CLILIST_WIRELESS  _CFG_GUESTZONE(0x8200)  //T=mac, Locked Wireless Client List
/* Schedule */
#define CSID_C_GUESTZONE2_RULE_SCHE_NO           _CFG_GUESTZONE(0x9000)  //T=u16, schedule number of using rule
/* Guest Zone for interfce 2 end =================*/

/* Status Items */
#define CSID_S_GUESTZONE_ALTERED		    	_STA_GUESTZONE(0x0001)  //T=u8, ui altered
#define CSID_S_GUESTZONE_LOCK_CLIENT_ALTERED   	_STA_GUESTZONE(0x0002)  //T=u8, ui altered
#define CSID_S_GUESTZONE_SVR_ALTERED   	        _STA_GUESTZONE(0x0003)  //T=u8, ui altered

/* Client-List, 256 items are reserved */
#define CSID_S_GUESTZONE_SVR_CLILIST_IP			_STA_GUESTZONE(0x0100)	//T=ipv4, The IP of the Client in Client List
#define CSID_S_GUESTZONE_SVR_CLILIST_HOSTNAME	_STA_GUESTZONE(0x0200)	//T=str, The hostname of the Client in the Client List
#define CSID_S_GUESTZONE_SVR_CLILIST_MAC   		_STA_GUESTZONE(0x0300)	//T=mac, The Mac Address of the Client in Client List
#define CSID_S_GUESTZONE_SVR_CLILIST_LEASETIME	_STA_GUESTZONE(0x0400)	//T=u32, remaining lease time
#define CSID_S_GUESTZONE_SVR_CLILIST_TYPE		_STA_GUESTZONE(0x0500)	//T=u8, wired or wireless mode
#define CSID_S_GUESTZONE_SVR_CLILIST_WLAN		_STA_GUESTZONE(0x0600)	//T=str, backup wireless client for speical issue

#define CSID_S_GUESTZONE_CLILIST_MAC  			_STA_GUESTZONE(0x0700)  //T=mac, Client List

/*Stainfo */
#define CSID_S_GUESTZONE_STAINFO_MAC			_STA_GUESTZONE(0x0800)  //T=mac, station MAC

/* Guest Zone for interfce 2 start =================*/
/* Status Items */
#define CSID_S_GUESTZONE2_ALTERED		    	_STA_GUESTZONE(0x8001)  //T=u8, ui altered
#define CSID_S_GUESTZONE2_LOCK_CLIENT_ALTERED  	_STA_GUESTZONE(0x8002)  //T=u8, ui altered
#define CSID_S_GUESTZONE2_ENABLE_CHG_STATE    	_STA_GUESTZONE(0x8003)  //T=u8, ui altered

/* Client-List, 256 items are reserved */
#define CSID_S_GUESTZONE2_SVR_CLILIST_IP			_STA_GUESTZONE(0x8100)	//T=ipv4, The IP of the Client in Client List
#define CSID_S_GUESTZONE2_SVR_CLILIST_HOSTNAME	_STA_GUESTZONE(0x8200)	//T=str, The hostname of the Client in the Client List
#define CSID_S_GUESTZONE2_SVR_CLILIST_MAC   		_STA_GUESTZONE(0x8300)	//T=mac, The Mac Address of the Client in Client List
#define CSID_S_GUESTZONE2_SVR_CLILIST_LEASETIME	_STA_GUESTZONE(0x8400)	//T=u32, remaining lease time
#define CSID_S_GUESTZONE2_SVR_CLILIST_TYPE		_STA_GUESTZONE(0x8500)	//T=u8, wired or wireless mode
#define CSID_S_GUESTZONE2_SVR_CLILIST_WLAN		_STA_GUESTZONE(0x8600)	//T=str, backup wireless client for speical issue

#define CSID_S_GUESTZONE2_CLILIST_MAC  			_STA_GUESTZONE(0x8700)  //T=mac, Client List
/* Guest Zone for interfce 2 end =================*/
/* Stainfo */
#define CSID_S_GUESTZONE2_STAINFO_MAC			_STA_GUESTZONE(0x9000)  //T=mac, station MAC


#endif
