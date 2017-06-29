/*
 * @file Member ID Definition of Group URLBLOCK.
 *
 * Moderator: JasonJiang
 * Group ID: 0x00100000/0x80100000
 */

#ifndef _CSID_URLBLOCK_H
#define _CSID_URLBLOCK_H

#include "csid_gid.h"

#define _CFG_URLBLOCK(x)                (CSID_GIDC_URLBLOCK|(x))
#define _STA_URLBLOCK(x)                (CSID_GIDS_URLBLOCK|(x))

//URL Blocking
#define CSID_C_URLBLOCK_ENABLE          _CFG_URLBLOCK(0x0001)  //T=u8, URL-Blocking enable, 1:yes 0:no
#define CSID_C_URLBLOCK_REDIRECT        _CFG_URLBLOCK(0x0002)  //T=u8, enable/disable redirect the connection, 1:yes 0:no
#define CSID_C_URLBLOCK_SELECT_LIST     _CFG_URLBLOCK(0x0003)  //T=u8, D=0, select black/white list, 1:white list 0:black list
#define CSID_C_URLBLOCK_LOG             _CFG_URLBLOCK(0x0004)  //T=u8, log the status of block rules, 1:yes 0:no
#define CSID_C_URLBLOCK_LIST_ENABLE     _CFG_URLBLOCK(0x0005)  //T=u8, D=0, enable/disable black list, 1:enable 0:disable
#define CSID_C_URLBLOCK_TRUSTED_IP      _CFG_URLBLOCK(0x000f)  //T=str, D="", customize for ZyXEL, permit ip to pass through rules of content filter

#define CSID_C_URLBLOCK_FILE_EXTENSION_ENABLE          _CFG_URLBLOCK(0x0010)  //T=u8, URL-file-Blocking enable, 1:yes 0:no
#define CSID_C_URLBLOCK_FILE_EXTENSION_JAVA_ENABLE     _CFG_URLBLOCK(0x0011)  //T=u8, D=0, enable/disable java block, 1:enable 0:disable
#define CSID_C_URLBLOCK_FILE_EXTENSION_ACTIVEX_ENABLE  _CFG_URLBLOCK(0x0012)  //T=u8, D=0, enable/disable activex block, 1:enable 0:disable
#define CSID_C_URLBLOCK_FILE_EXTENSION_COOKIE_ENABLE   _CFG_URLBLOCK(0x0013)  //T=u8, D=0, enable/disable cookie block, 1:enable 0:disable
#define CSID_C_URLBLOCK_FILE_EXTENSION_PROXY_ENABLE    _CFG_URLBLOCK(0x0014)  //T=u8, D=0, enable/disable proxy block, 1:enable 0:disable
#define CSID_C_URLBLOCK_FILE_EXTENSION_LOG    		   _CFG_URLBLOCK(0x0018)  //T=u8, D=0, enable/disable log the status of block rules


#define CSID_C_URLBLOCK_WHITE_LIST_URL  _CFG_URLBLOCK(0x0100)  //T=str, white list
#define CSID_C_URLBLOCK_BLACK_LIST_URL  _CFG_URLBLOCK(0x0200)  //T=str, block list

#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_LIST	_CFG_URLBLOCK(0x0300)  //T=str,save file extension separate by comma ex a,b,c
#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_ENABLE  _CFG_URLBLOCK(0x0400)   //T=u8, rule enable, 1:yes 0:no
#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_SCHE_NO _CFG_URLBLOCK(0x0500)   //T=u16, number of using rule
#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_NAME    _CFG_URLBLOCK(0x0600)   //T=str, the rule name of file extension rule

#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_SIP0    _CFG_URLBLOCK(0x0700)   //T=ipv4, start of source IP
#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_SIP     _CFG_URLBLOCK(0x0800)   //T=str, source IP range
#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_MAC     _CFG_URLBLOCK(0x0900)   //T=mac, source MAC address

#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_SIP_GROUP_NAME  _CFG_URLBLOCK(0x0A00) //T=str, apply source ip from IP Address-based group name
#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_MAC_GROUP_NAME  _CFG_URLBLOCK(0x0B00) //T=str, apply source mac from MAC Address-based group name
#define CSID_C_URLBLOCK_FILE_EXTENSION_RULE_FEXT_GROUP_NAME _CFG_URLBLOCK(0x0C00) //T=str, apply file extension from file extension-based group name


#define CSID_C_URLBLOCK_RULE_URL        _CFG_URLBLOCK(0x1000)  //T=str, URL String
#define CSID_C_URLBLOCK_RULE_ENABLE     _CFG_URLBLOCK(0x2000)  //T=u8, rule enable, 1:yes 0:no
#define CSID_C_URLBLOCK_RULE_SCHE_NO    _CFG_URLBLOCK(0x3000)  //T=u16, number of using rule
#define CSID_C_URLBLOCK_RULE_PORT       _CFG_URLBLOCK(0x4000)  //T=str, Port range
#define CSID_C_URLBLOCK_RULE_NAME       _CFG_URLBLOCK(0x5000)  //T=str, the rule name of url blocking

#define CSID_C_URLBLOCK_RULE_SIP0       _CFG_URLBLOCK(0x6000)  //T=ipv4, start of source IP
#define CSID_C_URLBLOCK_RULE_SIP        _CFG_URLBLOCK(0x7000)  //T=str, source IP range
#define CSID_C_URLBLOCK_RULE_MAC        _CFG_URLBLOCK(0x8000)  //T=mac, source MAC address

#define CSID_C_URLBLOCK_RULE_SIP_GROUP_NAME _CFG_URLBLOCK(0x9000) //T=str, apply source ip from IP Address-based group name
#define CSID_C_URLBLOCK_RULE_MAC_GROUP_NAME _CFG_URLBLOCK(0xA000) //T=str, apply source mac from MAC Address-based group name


/* Status Items */

#define CSID_S_URLBLOCK_ALTERED			 _STA_URLBLOCK(0x0001)	//T=u8, ui altered
#define CSID_S_URLBLOCK_FILE_EXTENSION_ALTERED     _STA_URLBLOCK(0x0002)	//T=u8, ui altered

#define CSID_S_URLBLOCK_RULE_SCHE_STATUS _STA_URLBLOCK(0x0100)  //T=u8, status of schedule rule 
#define CSID_S_URLBLOCK_FILE_EXTENSION_RULE_SCHE_STATUS _STA_URLBLOCK(0x0200)	//T=u8, status of schedule rule 


#endif //ifndef _CSID_URLBLOCK_H
