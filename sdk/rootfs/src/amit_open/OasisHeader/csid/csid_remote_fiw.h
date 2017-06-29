/*
 * @file Member ID Definition of Group REMOTE_FIW.
 *
 * Moderator: Aaron Wu
 * Group ID: 0x00790000/0x80790000
 */

#ifndef _CSID_REMOTE_FIW_H
#define _CSID_REMOTE_FIW_H

#include "csid_gid.h"

#define _CFG_REMOTE_FIW(x)       (CSID_GIDC_REMOTE_FIW|(x))
#define _STA_REMOTE_FIW(x)       (CSID_GIDS_REMOTE_FIW|(x))

/*MAC Control Items*/
#define CSID_S_REMOTE_FIW_MACCTL_ENABLE         			_STA_REMOTE_FIW(0x0000) 		//T=u8, FireWall Main function enable 
#define CSID_S_REMOTE_FIW_MACCTL_CC_DENY_ALL				_STA_REMOTE_FIW(0x0001)			//T=u8, FireWall Black List or White List
#define CSID_S_REMOTE_FIW_MACCTL_LOG_ENABLE					_STA_REMOTE_FIW(0x0002)			//T=u8, FireWall Log Alert Enable
/*MAC Control Rule Lists*/
#define CSID_S_REMOTE_FIW_MACCTL_RULE_MAC					_STA_REMOTE_FIW(0x0100)			//T=mac, FireWall MAC address
#define CSID_S_REMOTE_FIW_MACCTL_RULE_SCHE_NO				_STA_REMOTE_FIW(0x0200)			//T=u16, FireWall number of using rule 
#define CSID_S_REMOTE_FIW_MACCTL_RULE_ENABLE				_STA_REMOTE_FIW(0x0300)			//T=u8, FireWall rule enable, 1:enable 0:disable 


/*Packet Filter Items */
#define CSID_S_REMOTE_FIW_PKFILTER_ENABLE					_STA_REMOTE_FIW(0x1000)			//T=u8, PacketFilter Main function enable
#define CSID_S_REMOTE_FIW_PKFILTER_DENY_ALL					_STA_REMOTE_FIW(0x1001)			//T=u8, PacketFilter, 0:allow all except,1:deny all except,2:rule with its action  
#define CSID_S_REMOTE_FIW_PKFILTER_LOG_ENABLE				_STA_REMOTE_FIW(0x1002)			//T=u8, PacketFilter enable/disable log the status of PKTBOTH block rules, 1:yes 0:no 
/* Packet Filter Rule Lists */
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_SIP0				_STA_REMOTE_FIW(0x1100)			//T=ipv4, PacketFilter start of source IP range 
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_SIP					_STA_REMOTE_FIW(0x1200)			//T=str, PacketFilter source IP range 
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_DIP0				_STA_REMOTE_FIW(0x1300)			//T=ipv4, PacketFilter start of destination IP range
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_DIP					_STA_REMOTE_FIW(0x1400)			//T=str, PacketFilter destination IP range 
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_DPORT				_STA_REMOTE_FIW(0x1500)			//T=str, PacketFilter destination Port range 
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_PRO					_STA_REMOTE_FIW(0x1600)			//T=u16, PacketFilter protocol 1:TCP,2:UDP,3:Both,4:ICMP
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_SCHE_NO				_STA_REMOTE_FIW(0x1700)			//T=u16, PacketFilter number of using rule 	
#define CSID_S_REMOTE_FIW_PKFILTER_RULE_ENABLE				_STA_REMOTE_FIW(0x1800)			//T=u8, PacketFilter rule enable, 1:yes p:no 

#endif //ifndef _CSID_REMOTE_FIW_H


