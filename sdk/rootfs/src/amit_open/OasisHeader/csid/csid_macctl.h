/*
 * @file Member ID Definition of Group MACCTL.
 *
 * Moderator: JasonJiang
 * Group ID: 0x000d0000/0x800d0000
 */

#ifndef _CSID_MACCTL_H
#define _CSID_MACCTL_H

#include "csid_gid.h"

#define _CFG_MACCTL(x)              (CSID_GIDC_MACCTL|(x))
#define _STA_MACCTL(x)              (CSID_GIDS_MACCTL|(x))

/* Configuration Items */
#define CSID_C_MACCTL_ENABLE        	_CFG_MACCTL(0x0001) //T=u8, whether starting MAC-Control, 1:yes 0:no
#define CSID_C_MACCTL_CC_ENABLE     	_CFG_MACCTL(0x0002) //T=u8, start of connection-control, 1:enable 0:disable
#define CSID_C_MACCTL_CC_DENY_ALL   	_CFG_MACCTL(0x0003) //T=u8, allow/deny the unspecified mac 0:allow 1:deny
#define CSID_C_MACCTL_AC_ENABLE     	_CFG_MACCTL(0x0004) //T=u8, start of association-control,1:enable 0:disable
#define CSID_C_MACCTL_AC_DENY_ALL   	_CFG_MACCTL(0x0005) //T=u8, allow/deny the unspecified mac 0:allow 1:deny
#define CSID_C_MACCTL_PRE_PUSH_STATUS   _CFG_MACCTL(0x0006) //T=u8, record last push_status, 1:deny 0:allow
#define CSID_C_MACCTL_LOG_ENABLE      _CFG_MACCTL(0x0007) //T=u8, enable/disable log the status of MAC-Control rules, 1:yes 0:no
#define CSID_C_MACCTL_RULE_ENABLE_MODE  _CFG_MACCTL(0x0008) //T=u8, whether starting MAC-Control rule enable function, 1:yes 0:no

#define CSID_C_MACCTL_ENABLE1        	_CFG_MACCTL(0x0031) //T=u8, whether starting MAC-Control, 1:yes 0:no
#define CSID_C_MACCTL_CC_ENABLE1     	_CFG_MACCTL(0x0032) //T=u8, start of connection-control, 1:enable 0:disable
#define CSID_C_MACCTL_CC_DENY_ALL1   	_CFG_MACCTL(0x0033) //T=u8, allow/deny the unspecified mac 0:allow 1:deny
#define CSID_C_MACCTL_AC_ENABLE1     	_CFG_MACCTL(0x0034) //T=u8, start of association-control,1:enable 0:disable
#define CSID_C_MACCTL_AC_DENY_ALL1   	_CFG_MACCTL(0x0035) //T=u8, allow/deny the unspecified mac 0:allow 1:deny

#define CSID_C_MACCTL_ENABLE2        	_CFG_MACCTL(0x0061) //T=u8, whether starting MAC-Control, 1:yes 0:no
#define CSID_C_MACCTL_CC_ENABLE2     	_CFG_MACCTL(0x0062) //T=u8, start of connection-control, 1:enable 0:disable
#define CSID_C_MACCTL_CC_DENY_ALL2   	_CFG_MACCTL(0x0063) //T=u8, allow/deny the unspecified mac 0:allow 1:deny
#define CSID_C_MACCTL_AC_ENABLE2     	_CFG_MACCTL(0x0064) //T=u8, start of association-control,1:enable 0:disable
#define CSID_C_MACCTL_AC_DENY_ALL2   	_CFG_MACCTL(0x0065) //T=u8, allow/deny the unspecified mac 0:allow 1:deny

#define CSID_C_MACCTL_ENABLE3        	_CFG_MACCTL(0x0091) //T=u8, whether starting MAC-Control, 1:yes 0:no
#define CSID_C_MACCTL_CC_ENABLE3     	_CFG_MACCTL(0x0092) //T=u8, start of connection-control, 1:enable 0:disable
#define CSID_C_MACCTL_CC_DENY_ALL3   	_CFG_MACCTL(0x0093) //T=u8, allow/deny the unspecified mac 0:allow 1:deny
#define CSID_C_MACCTL_AC_ENABLE3     	_CFG_MACCTL(0x0094) //T=u8, start of association-control,1:enable 0:disable
#define CSID_C_MACCTL_AC_DENY_ALL3   	_CFG_MACCTL(0x0095) //T=u8, allow/deny the unspecified mac 0:allow 1:deny


/* reserved 256 items */
#define CSID_C_MACCTL_RULE_MAC      _CFG_MACCTL(0x0100) //T=mac, MAC address
#define CSID_C_MACCTL_RULE_IP       _CFG_MACCTL(0x0200) //T=ipv4, IP address
#define CSID_C_MACCTL_RULE_C_FLAG   _CFG_MACCTL(0x0300) //T=u8, MAC-control type, 1:enable 0:disable
#define CSID_C_MACCTL_RULE_A_FLAG   _CFG_MACCTL(0x0400) //T=u8, MAC-control type, 1:enable 0:disable
#define CSID_C_MACCTL_RULE_C_TYPE   _CFG_MACCTL(0x0500) //T=u8, MAC-control type, 1:wireless 0:wired 
#define CSID_C_MACCTL_RULE_SCHE_NO  _CFG_MACCTL(0x0600) //T=u16, schedule number of using rule
#define CSID_C_MACCTL_RULE_MAC_NAME _CFG_MACCTL(0x0700) //T=str, MAC address name
#define CSID_C_MACCTL_RULE_ENABLE   _CFG_MACCTL(0x0800) //T=u8, rule enable, 1:enable 0:disable
#define CSID_C_MACCTL_RULE_NAME     _CFG_MACCTL(0x0900) //T=str, the rule name of MAC Address


#define CSID_C_MACCTL_RULE_MAC1     _CFG_MACCTL(0x1100) //T=mac, MAC address
#define CSID_C_MACCTL_RULE_IP1      _CFG_MACCTL(0x1200) //T=ipv4, IP address
#define CSID_C_MACCTL_RULE_C_FLAG1  _CFG_MACCTL(0x1300) //T=u8, MAC-control type, 1:enable 0:disable
#define CSID_C_MACCTL_RULE_A_FLAG1  _CFG_MACCTL(0x1400) //T=u8, MAC-control type, 1:enable 0:disable

#define CSID_C_MACCTL_RULE_MAC2     _CFG_MACCTL(0x2100) //T=mac, MAC address
#define CSID_C_MACCTL_RULE_IP2      _CFG_MACCTL(0x2200) //T=ipv4, IP address
#define CSID_C_MACCTL_RULE_C_FLAG2  _CFG_MACCTL(0x2300) //T=u8, MAC-control type, 1:enable 0:disable
#define CSID_C_MACCTL_RULE_A_FLAG2  _CFG_MACCTL(0x2400) //T=u8, MAC-control type, 1:enable 0:disable

#define CSID_C_MACCTL_RULE_MAC3     _CFG_MACCTL(0x3100) //T=mac, MAC address
#define CSID_C_MACCTL_RULE_IP3      _CFG_MACCTL(0x3200) //T=ipv4, IP address
#define CSID_C_MACCTL_RULE_C_FLAG3  _CFG_MACCTL(0x3300) //T=u8, MAC-control type, 1:enable 0:disable
#define CSID_C_MACCTL_RULE_A_FLAG3  _CFG_MACCTL(0x3400) //T=u8, MAC-control type, 1:enable 0:disable



/* Status Items */
#define CSID_S_MACCTL_RULE_NUM      	_STA_MACCTL(0x0001) //T=u16, number of MC rules
#define CSID_S_MACCTL_ALTERED	    	_STA_MACCTL(0x0002)	//T=u8, ui altered
#define CSID_S_MACCTL_RULE_MAC_LIST 	_STA_MACCTL(0x0003) //T=str, a list of wireless MAC address
#define CSID_S_MACCTL_RULE_MAC_LIST1 	_STA_MACCTL(0x0004) //T=str, a list of wireless MAC address
#define CSID_S_MACCTL_RULE_MAC_LIST2 	_STA_MACCTL(0x0005) //T=str, a list of wireless MAC address
#define CSID_S_MACCTL_RULE_MAC_LIST3 	_STA_MACCTL(0x0006) //T=str, a list of wireless MAC address
#define CSID_S_MACCTL_RULE_SCHE_STATUS  _STA_MACCTL(0x0100) //T=u8, schedule status

#endif //ifndef _CSID_MACCTL_H
