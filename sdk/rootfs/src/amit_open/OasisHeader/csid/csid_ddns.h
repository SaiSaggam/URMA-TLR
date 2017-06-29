/*
 * @file Member ID Definition of Group DDNS.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00090000/0x80090000
 */

#ifndef _CSID_DDNS_H
#define _CSID_DDNS_H

#include "csid_gid.h"

#define _CFG_DDNS(x)		(CSID_GIDC_DDNS|(x))
#define _STA_DDNS(x)		(CSID_GIDS_DDNS|(x))

#define ORAY_STATUS_OUTLINE			0x00
#define ORAY_STATUS_ONLINE			0x01
#define ORAY_STATUS_CONNECTING		0x02


/* Configuration Items */

#define CSID_C_DDNS_ENABLE	       _CFG_DDNS(0x0001)	//T=u8, Enable/Disable
#define CSID_C_DDNS_PROVIDER  	   _CFG_DDNS(0x0002)	//T=u8, DDNS provider
#define CSID_C_DDNS_FQDN	       _CFG_DDNS(0x0003)	//T=str, Full Qualified Domain Name
#define CSID_C_DDNS_USER    	   _CFG_DDNS(0x0004)	//T=str, username to login DDNS provider
#define CSID_C_DDNS_PASSWORD       _CFG_DDNS(0x0005)	//T=str, password to login DDNS provider
#define CSID_C_DDNS_CMDS     	   _CFG_DDNS(0x0006)	//T=str, command string
#define CSID_C_DDNS_CLIENTINFO	   _CFG_DDNS(0x0007)	//T=u32, client info
#define CSID_C_DDNS_CHALLENGEKEY   _CFG_DDNS(0x0008)	//T=u32, challenge key
#define CSID_C_DDNS_HOMECOM_ALIAS  _CFG_DDNS(0x0009)    //T=str, D="", alias for user
#define CSID_C_DDNS_HOMECOM_PREFIX _CFG_DDNS(0x0010)    //T=str, D="", prefix for username
#define CSID_C_DDNS_DOMAIN_NAME    _CFG_DDNS(0x0011)    //T=str, D="", ddns domain name for ezguard project
#define CSID_C_DDNS_UPDATE_INTERVAL   _CFG_DDNS(0x0012)	//T=u32, update intervals units is minute

/*IODATA Pre Set FQDN iobb.net */
#define CSID_C_IODATA_PRESET_FQDN      _CFG_DDNS(0x0013)	//T=str, iodata pre set Full Qualified Domain Name
#define CSID_C_IODATA_PRESET_USER      _CFG_DDNS(0x0014)	//T=str, iodata pre set sno/username
#define CSID_C_IODATA_PRESET_PASSWORD  _CFG_DDNS(0x0015)	//T=str, iodata pre set password

/* DDNS WAN Interface binding */
#define CSID_C_DDNS_BOUND_WAN          _CFG_DDNS(0x0020)	//T=u32, DDNS binds to WAN, 0: Unused, 1: WAN1, 2: WAN2, 3: WAN3, etc...


#define CSID_C_DDNSV6_ENABLE 			_CFG_DDNS(0x1001)	//T=u8, Enable/Disable
#define CSID_C_DDNSV6_PROVIDER 			_CFG_DDNS(0x1002)	//T=u8, DDNSv6 provider
#define CSID_C_DDNSV6_FQDN 				_CFG_DDNS(0x1003)	//T=str, Full Qualified Domain Name
#define CSID_C_DDNSV6_USER 				_CFG_DDNS(0x1004)	//T=str, DDNSv6 username
#define CSID_C_DDNSV6_PASSWORD 			_CFG_DDNS(0x1005)	//T=str, DDNSv6 password
#define CSID_C_DDNSV6_UPDATE_INTERVAL   _CFG_DDNS(0x1006)	//T=u32, update intervals units is minute
#define CSID_C_DDNSV6_TOKEN 			_CFG_DDNS(0x1007)	//T=str, Token of freedns to update

/* Status Items */

#define CSID_S_DDNS_STATUS        _STA_DDNS(0x0001)	//T=u16, update status
#define CSID_S_DDNS_ALTERED		  _STA_DDNS(0x0002)	//T=u8, ui altered

//Status Items for DDC DDNS support
#define CSID_S_DDNS_HOMECOM_ALTERED	            	_STA_DDNS(0x0003)	//T=u8, D=0,ui altered
#define CSID_S_DDNS_HOMECOM_ENABLE	   		 	    _STA_DDNS(0x0004)	//T=u8,  D=0, Enable/Disable
#define CSID_S_DDNS_HOMECOM_USER      			    _STA_DDNS(0x0005)	//T=str, D="", username to login DDC DDNS 
#define CSID_S_DDNS_HOMECOM_PASSWORD  		        _STA_DDNS(0x0006)	//T=str, D="", password to login DDC DDNS
#define CSID_S_DDNS_HOMECOM_ALIAS_TEST        	    _STA_DDNS(0x0007)   //T=u8, alias altered for ui
#define CSID_S_DDNS_HOMECOM_ALIAS_SAVE          	_STA_DDNS(0x0008)   //T=u8, alias altered for ui
#define CSID_S_DDNS_HOMECOM_ALIAS_LOOKUP        	_STA_DDNS(0x0009)   //T=u8, alias altered for ui
#define CSID_S_DDNS_HOMECOM_ALIAS_CHECK_IP		    _STA_DDNS(0x0010)   //T=u8, alias altered for ui 
#define CSID_S_DDNS_HOMECOM_ALIAS_TEST_ENABLE   	_STA_DDNS(0x0011)   //T=u8, alias_test_function_enable 
#define CSID_S_DDNS_HOMECOM_ALIAS_SAVE_ENABLE   	_STA_DDNS(0x0012)   //T=u8, alias_save_function_enable
#define CSID_S_DDNS_HOMECOM_ALIAS_LOOKUP_ENABLE 	_STA_DDNS(0x0013)   //T=u8, alias_lookup_function_enable 
#define CSID_S_DDNS_HOMECOM_ALIAS_CHECK_IP_ENABLE   _STA_DDNS(0x0014)   //T=u8, alias_check_ip_function_enable
#define CSID_S_DDNS_HOMECOM_ALIAS_TEST_RESULT   	_STA_DDNS(0x0015)   //T=str, D="", alias of testing result 
#define CSID_S_DDNS_HOMECOM_ALIAS_SAVE_RESULT   	_STA_DDNS(0x0016)   //T=str, D="", alias of saving result
#define CSID_S_DDNS_HOMECOM_ALIAS_LOOKUP_RESULT 	_STA_DDNS(0x0017)   //T=str, D="", alias of lookup result 
#define CSID_S_DDNS_HOMECOM_ALIAS_CHECK_IP_RESULT   _STA_DDNS(0x0018)   //T=str, D="", alias_check_ip result
#define CSID_S_DDNS_HOMECOM_ALIAS_ERROR         	_STA_DDNS(0x0019)   //T=str, D="", alias error message
#define CSID_S_DDNS_HOMECOM_ALIAS               	_STA_DDNS(0x0020)   //T=str, D="", alias for user
#define CSID_S_DDNS_HOMECOM_NSUPDATE               	_STA_DDNS(0x0021)   //T=u8, D="", check nsupdate status
#define CSID_S_DDNS_HOMECOM_ESP_ALIAS_SAVE          _STA_DDNS(0x0022)   //T=u8, D="",ESP setting status

#define CSID_S_DDNS_ORAY_STATUS						_STA_DDNS(0x0023)   //T=u8, D=0 ,Oray running status
#define CSID_S_DDNS_ORAY_USER_TYPE					_STA_DDNS(0x0024)   //T=u8, D=0 ,Oray user type , 0:standard user, 1:Professional user
/* Domain 32 items are reserved */
#define CSID_S_DDNS_ORAY_DOMAIN						_STA_DDNS(0x0025)   //T=str, D="" ,Oray domain name

/*IODATA Pre Set FQDN iobb.net */
#define CSID_S_IODATA_PRESET_FQDN	       _STA_DDNS(0x0100)	//T=str, iodata pre set Full Qualified Domain Name
#define CSID_S_IODATA_DDNS_PRE_TYPE	       _STA_DDNS(0x0101)	//T=u32, D=0 ,pre iobb.net

/* IPv6 DDNS */
#define CSID_S_DDNSV6_STATUS 			_STA_DDNS(0x1001)	//T=u16, update status
#define CSID_S_DDNSV6_ALTERED 			_STA_DDNS(0x1002)	//T=u8, ui altered

#endif //ifndef _CSID_DDNS_H
