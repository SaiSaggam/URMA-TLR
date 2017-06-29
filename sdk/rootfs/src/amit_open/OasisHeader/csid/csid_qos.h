/*
 * @file Member ID Definition of Group QoS.
 *
 * Moderator: JasonJiang
 * Group ID: 0x00120000/0x80120000
 */

#ifndef _CSID_QOS_H
#define _CSID_QOS_H

#include "csid_gid.h"

//for Cross-Layer QoS
#define PFIFO 0
#define SFQ 1
#define HTB 2


//for "CSID_C_QOS_L7_AP_NAME"
#define L7_AP_NAME_FTP 0
#define L7_AP_NAME_HTTP 1
#define L7_AP_NAME_EDONKEY 2
#define L7_AP_NAME_BITTORRENT 3
#define L7_AP_NAME_MSN MESSAENGER 4
#define L7_AP_NAME_BT 5
#define L3_TCP 6
#define L3_UDP 7
#define L3_ICMP 8

#define _CFG_QOS(x)         (CSID_GIDC_QOS|(x))
#define _STA_QOS(x)         (CSID_GIDS_QOS|(x))

/* Configuration Items */
#define CSID_C_QOS_ENABLE		_CFG_QOS(0x0001) //T=u8, qos enable (For Upload), 1:yes 0:no
#define CSID_C_QOS_DW_ENABLE		_CFG_QOS(0x000a) //T=u8, qos enable (For Download), 1:yes 0:no
#define CSID_C_QOS_UP_BANDWIDTH		_CFG_QOS(0x0002) //T=u16, upstreaming bandwidth of wan port,unit is kbps
#define CSID_C_QOS_DW_BANDWIDTH		_CFG_QOS(0x0003) //T=u16, downloading bandwidth of wan port,unit is kbps
#define CSID_C_QOS_DW_HI_BWH		_CFG_QOS(0x0004) //T=u16, bandwidth of downlink for users with high priority
#define CSID_C_QOS_DW_NO_BWH		_CFG_QOS(0x0005) //T=u16, bandwidth of downlink for users with normal priority
#define CSID_C_QOS_DW_LO_BWH		_CFG_QOS(0x0006) //T=u16, bandwidth of downlink for users with low priority
#define CSID_C_QOS_UP_HI_BWH		_CFG_QOS(0x0007) //T=u16, bandwidth of uplink for users with high priority
#define CSID_C_QOS_UP_NO_BWH		_CFG_QOS(0x0008) //T=u16, bandwidth of uplink for users with normal priority
#define CSID_C_QOS_UP_LO_BWH		_CFG_QOS(0x0009) //T=u16, bandwidth of uplink for users with low priority


#define CSID_C_QOS_LOCAL_IP		_CFG_QOS(0x0100) //T=ipv4, local ip 
#define CSID_C_QOS_LOCAL_PORT		_CFG_QOS(0x0200) //T=u16, local port 
#define CSID_C_QOS_REMOTE_IP		_CFG_QOS(0x0300) //T=ipv4, remote ip
#define CSID_C_QOS_REMOTE_PORT		_CFG_QOS(0x0400) //T=u16, remote port 
#define CSID_C_QOS_PRIORITY		_CFG_QOS(0x0500) //T=u16, priority level, 1=HIGH, 2=NORMAL, 3=LOW, 4=user define by self.
#define CSID_C_QOS_RULE_ENABLE		_CFG_QOS(0x0600) //T=u8, rule enable, 1:yes 0:no
#define CSID_C_QOS_USING_RULE		_CFG_QOS(0x0700) //T=u16, rule number
#define CSID_C_QOS_DEV			_CFG_QOS(0x0800) //T=str, name of the interface you want to control.

#define CSID_C_QOS_L7_UP_BANDWIDTH 	_CFG_QOS(0x0900) //T=u16, restricted upstreaming bandwidth of L7 appications,unit is kbps
#define CSID_C_QOS_L7_DW_BANDWIDTH 	_CFG_QOS(0x0A00) //T=u16, restricted download bandwidth of L7 appplications ,unit is kbps
#define CSID_C_QOS_L7_PROTOCOL   	_CFG_QOS(0x0B00) //T=str, the name of L7 applications, like BT.


/*For Cross-Layer QoS*/
/* GIGA model */
#define CSID_C_QOSC_GIGA_MODEL          _CFG_QOS(0x000b) //T=u8, 0:10/100 model 1:GIGA model(not MT7621 CPU) 2:MT7621 CPU(internal GIGA SWITCH MT7530)
/* Configuration Items */
#define CSID_C_QOSC_MODE		_CFG_QOS(0x0010) //T=u8, 0:Smart-QoS, 1:User defined QoS rule
#define CSID_C_QOSC_ENABLE		_CFG_QOS(0x0011) //T=u8, wheather coordinator is enabled or not
#define CSID_C_QOSC_RULENUM		_CFG_QOS(0x0012) //T=u16, Number of rules
#define CSID_C_QOSC_SESSION		_CFG_QOS(0x0013) //T=u16, Number of SESSION
#define CSID_C_QOSC_TOTALRATE		_CFG_QOS(0x0014) //T=u16, Total rate
#define CSID_C_QOSC_LoadBW		_CFG_QOS(0x0015) //T=u8, Enable/Disable Load band Width

/*Lazy setting*/
#define CSID_C_QOSC_LAZY_AVERAGE	_CFG_QOS(0x0016) //T=u8, Enable/Disable average bandwidth setting
#define CSID_C_QOSC_LAZY_SETTING	_CFG_QOS(0x0017) //T=str, Lazy setting
#define CSID_C_QOSC_LAZY_FBMPER		_CFG_QOS(0xd000) //T=u16, D=0, Flexible Bandwidth percent

// MutiWAN settings ETH , USB-3G
// WAN Up/Download maximum bandwidth in MB
#define CSID_C_QOSC_ETH1_B_UP		_CFG_QOS(0x0020) //T=u32, ETH1/WAN1 Upstream Bandwidth
#define CSID_C_QOSC_WAN2_B_UP		_CFG_QOS(0x0021) //T=u32, WAN2 Upstream Bandwidth
#define CSID_C_QOSC_WAN3_B_UP		_CFG_QOS(0x0022) //T=u32, WAN3 Upstream Bandwidth
#define CSID_C_QOSC_WAN4_B_UP		_CFG_QOS(0x0023) //T=u32, WAN4 Upstream Bandwidth
#define CSID_C_QOSC_WAN5_B_UP		_CFG_QOS(0x0024) //T=u32, WAN5 Upstream Bandwidth
#define CSID_C_QOSC_3G1_B_UP		_CFG_QOS(0x0025) //T=u32, 3G1 Upstream Bandwidth
#define CSID_C_QOSC_3G2_B_UP		_CFG_QOS(0x0026) //T=u32, 3G2 Upstream Bandwidth
#define CSID_C_QOSC_ETH1_B_DOWN		_CFG_QOS(0x0030) //T=u32, ETH1/WAN1 Downstream Bandwidth
#define CSID_C_QOSC_WAN2_B_DOWN		_CFG_QOS(0x0031) //T=u32, WAN2 Downstream Bandwidth
#define CSID_C_QOSC_WAN3_B_DOWN		_CFG_QOS(0x0032) //T=u32, WAN3 Downstream Bandwidth
#define CSID_C_QOSC_WAN4_B_DOWN		_CFG_QOS(0x0033) //T=u32, WAN4 Downstream Bandwidth
#define CSID_C_QOSC_WAN5_B_DOWN		_CFG_QOS(0x0034) //T=u32, WAN5 Downstream Bandwidth
#define CSID_C_QOSC_3G1_B_DOWN		_CFG_QOS(0x0035) //T=u32, 3G1 Downstream Bandwidth
#define CSID_C_QOSC_3G2_B_DOWN		_CFG_QOS(0x0036) //T=u32, 3G2 Downstream Bandwidth

// Up/Download maximun session number
#define CSID_C_QOSC_ETH1_S_UP		_CFG_QOS(0x0040) //T=u16, ETH1/WAN1 Upstream Session number
#define CSID_C_QOSC_WAN2_S_UP		_CFG_QOS(0x0041) //T=u16, WAN2 Upstream Session number
#define CSID_C_QOSC_WAN3_S_UP		_CFG_QOS(0x0042) //T=u16, WAN3 Upstream Session number
#define CSID_C_QOSC_WAN4_S_UP		_CFG_QOS(0x0043) //T=u16, WAN4 Upstream Session number
#define CSID_C_QOSC_WAN5_S_UP		_CFG_QOS(0x0044) //T=u16, WAN5 Upstream Session number
#define CSID_C_QOSC_3G1_S_UP		_CFG_QOS(0x0045) //T=u16, 3G1 Upstream Session number
#define CSID_C_QOSC_3G2_S_UP		_CFG_QOS(0x0046) //T=u16, 3G2 Upstream Session number
#define CSID_C_QOSC_ETH1_S_DOWN		_CFG_QOS(0x0050) //T=u16, ETH1/WAN1 Downstream Session number
#define CSID_C_QOSC_WAN2_S_DOWN		_CFG_QOS(0x0051) //T=u16, WAN2 Downstream Session number
#define CSID_C_QOSC_WAN3_S_DOWN		_CFG_QOS(0x0052) //T=u16, WAN3 Downstream Session number
#define CSID_C_QOSC_WAN4_S_DOWN		_CFG_QOS(0x0053) //T=u16, WAN4 Downstream Session number
#define CSID_C_QOSC_WAN5_S_DOWN		_CFG_QOS(0x0054) //T=u16, WAN5 Downstream Session number
#define CSID_C_QOSC_3G1_S_DOWN		_CFG_QOS(0x0055) //T=u16, 3G1 Downstream Session number
#define CSID_C_QOSC_3G2_S_DOWN		_CFG_QOS(0x0056) //T=u16, 3G2 Downstream Session number
// Priority of Queue (1~6)
#define CSID_C_QOSC_ETH1_P_QUEUE	_CFG_QOS(0x0060) //T=u16, Number of priority queues
#define CSID_C_QOSC_WAN2_P_QUEUE	_CFG_QOS(0x0061) //T=u16, Number of priority queues
#define CSID_C_QOSC_WAN3_P_QUEUE	_CFG_QOS(0x0062) //T=u16, Number of priority queues
#define CSID_C_QOSC_WAN4_P_QUEUE	_CFG_QOS(0x0063) //T=u16, Number of priority queues
#define CSID_C_QOSC_WAN5_P_QUEUE	_CFG_QOS(0x0064) //T=u16, Number of priority queues
#define CSID_C_QOSC_3G1_P_QUEUE		_CFG_QOS(0x0065) //T=u16, Number of priority queues
#define CSID_C_QOSC_3G2_P_QUEUE		_CFG_QOS(0x0066) //T=u16, Number of priority queues
// type of Queue Schedule  0:PFIFO 1:SFQ 2:HTB
#define CSID_C_QOSC_ETH1_C_QUEUE	_CFG_QOS(0x0070) //T=u16, Queue Scheduling
#define CSID_C_QOSC_WAN2_C_QUEUE	_CFG_QOS(0x0071) //T=u16, Queue Scheduling
#define CSID_C_QOSC_WAN3_C_QUEUE	_CFG_QOS(0x0072) //T=u16, Queue Scheduling
#define CSID_C_QOSC_WAN4_C_QUEUE	_CFG_QOS(0x0073) //T=u16, Queue Scheduling
#define CSID_C_QOSC_WAN5_C_QUEUE	_CFG_QOS(0x0074) //T=u16, Queue Scheduling
#define CSID_C_QOSC_3G1_C_QUEUE		_CFG_QOS(0x0075) //T=u16, Queue Scheduling
#define CSID_C_QOSC_3G2_C_QUEUE		_CFG_QOS(0x0076) //T=u16, Queue Scheduling
// WAN Up/Download maximum bandwidth unit  0:Mbps 1:Kbps
#define CSID_C_QOSC_WAN1_B_UNIT_UP      _CFG_QOS(0x0080) //T=u32, WAN1 Upstream Bandwidth unit
#define CSID_C_QOSC_WAN2_B_UNIT_UP      _CFG_QOS(0x0081) //T=u32, WAN2 Upstream Bandwidth unit
#define CSID_C_QOSC_WAN3_B_UNIT_UP      _CFG_QOS(0x0082) //T=u32, WAN3 Upstream Bandwidth unit
#define CSID_C_QOSC_WAN4_B_UNIT_UP      _CFG_QOS(0x0083) //T=u32, WAN4 Upstream Bandwidth unit
#define CSID_C_QOSC_WAN5_B_UNIT_UP      _CFG_QOS(0x0084) //T=u32, WAN5 Upstream Bandwidth unit
#define CSID_C_QOSC_WAN1_B_UNIT_DOWN    _CFG_QOS(0x0090) //T=u32, WAN1 Downstream Bandwidth unit
#define CSID_C_QOSC_WAN2_B_UNIT_DOWN    _CFG_QOS(0x0091) //T=u32, WAN2 Downstream Bandwidth unit
#define CSID_C_QOSC_WAN3_B_UNIT_DOWN    _CFG_QOS(0x0092) //T=u32, WAN3 Downstream Bandwidth unit
#define CSID_C_QOSC_WAN4_B_UNIT_DOWN    _CFG_QOS(0x0093) //T=u32, WAN4 Downstream Bandwidth unit
#define CSID_C_QOSC_WAN5_B_UNIT_DOWN    _CFG_QOS(0x0094) //T=u32, WAN5 Downstream Bandwidth unit

//end MutiWAN settings

/*Rule structure*/
#define CSID_C_QOSC_RULEID          _CFG_QOS(0x1000) //T=u16, Rule ID
#define CSID_C_QOSC_RULEID_ENABLE   _CFG_QOS(0x1200) //T=u8, Rule Enable/Disable
#define CSID_C_QOSC_INTERFACE       _CFG_QOS(0x1300) //T=u16, interface

#define CSID_C_QOSC_RULE_WAN_IF     _CFG_QOS(0x2000) //T=u16, Interface selection of the rule
#define CSID_C_QOSC_CLASS           _CFG_QOS(0x2100) //T=str, Layer of the rule
#define CSID_C_QOSC_CLASS_DATA      _CFG_QOS(0x2200) //T=str, Layer information of the rule
#define CSID_C_QOSC_CLASS_IPCON     _CFG_QOS(0x2300) //T=u16, IP continuation
#define CSID_C_QOSC_DIR             _CFG_QOS(0x2400) //T=u16, Direction
#define CSID_C_QOSC_FUNCTION        _CFG_QOS(0x2500) //T=str, Function of the rule
#define CSID_C_QOSC_FUNCTION_DATA   _CFG_QOS(0x2600) //T=str, Function information of the rule
#define CSID_C_QOSC_AND_RULEID      _CFG_QOS(0x2700) //T=u16, And with which Rule ID
#define CSID_C_QOSC_LINK_TIMERULE   _CFG_QOS(0x2800) //T=u16, Link to what time schedule rule
#define CSID_C_QOSC_SINGLE          _CFG_QOS(0x3000) //T=u16, 0:sharing, 1:single 

#define CSID_C_QOSC_IP_MASK         _CFG_QOS(0x2d00) //T=str, IP MASK ex: 255.255.255.0
#define CSID_C_QOSC_PROTO           _CFG_QOS(0x2e00) //T=u16, 0:all 1:tcp 2:udp 3:icmp
#define CSID_C_QOSC_PORT            _CFG_QOS(0x2f00) //T=u16, 0:all 1:tcp 2:udp 3:icmp

#define CSID_C_QOSC_MINMAX_UNIT     _CFG_QOS(0x2900) //T=u16, Bandwidth unit 0:KB 1:Kbps 2:Mbps


/*Group*/
#define CSID_C_QOSC_GROUPNUM        _CFG_QOS(0x0018) //T=u16, Number of groups
#define CSID_C_QOSC_GROUPID         _CFG_QOS(0x4000) //T=u16, Group ID
#define CSID_C_QOSC_GROUPCLASS      _CFG_QOS(0x4400) //T=str, Layer of the group class
#define CSID_C_QOSC_GROUPCLASS_DATA _CFG_QOS(0x4800) //T=str, Layer information of the group class
#define CSID_C_QOSC_GROUPCLASS_CON  _CFG_QOS(0x4c00) //T=u16, IP continuation
//#define CSID_C_QOSC_GROUPQDMA       _CFG_QOS(0x5000) //T=u16, Group ID mapping QDMA Queue
//#define CSID_C_QOSC_GROUP_USERNUM    _CFG_QOS(0x5400) //T=u16, count user number for UserGroup.
//#define CSID_C_QOSC_GROUP_TCQUEUE    _CFG_QOS(0x5420) //T=u16, the Group use which TC Queue(sharing method is Group)

/*AP*/
#define CSID_C_QOSC_AP_NUM           _CFG_QOS(0x001a) //T=u8, User number of AP Client.
#define CSID_C_QOSC_AP_IP            _CFG_QOS(0x5900) //T=str, IP from AP Client. 

/*For Average Bandwidth*/
#define CSID_C_QOSC_AVERAGE_GID		_CFG_QOS(0xc100) //T=u16, The GID AVERAGE used.
/*For DI-1000*/
#define CSID_C_QOSC_PORT4_ENABLE	_CFG_QOS(0xc200) //T=u16, port enable
#define CSID_C_QOSC_PORT4_MAXRATE	_CFG_QOS(0xc201) //T=u16, port maxrate Mbps

/* Status Items */
#define CSID_S_QOS_ALTERED              _STA_QOS(0x0001) //T=u8, ui altered
/*Cross-Layer*/
#define CSID_S_QOSC_CONFLICT            _STA_QOS(0x0011) //T=u16, confliction detect info.
#define CSID_S_QOSC_REPLACE             _STA_QOS(0x0012) //T=u16, replace info.
#define CSID_S_QOSC_SVALUE              _STA_QOS(0x0013) //T=u16, suggest value.
#define CSID_S_QOSC_LAZY_ALTERED        _STA_QOS(0x0014) //T=u16, Lazy setting changed
#define CSID_S_QOSC_SPEED               _STA_QOS(0x0015) //T=u8, Trigger Speed Testing
#define CSID_S_QOSC_GW_MAC              _STA_QOS(0x0016) //T=str, Gateway MAC address
#define CSID_S_QOS_RULE_STATUS          _STA_QOS(0x0100) //T=u8, schedule rule status
#define CSID_S_QOSC_UG_LOGIN_ALTERED    _STA_QOS(0x0101) //T=u16, Group User log in
#define CSID_S_QOSC_UG_LOGINUSER        _STA_QOS(0x0102) //T=u16, index of the login user from User Group
#define CSID_S_QOSC_UG_LOGINGROUP       _STA_QOS(0x0103) //T=u16, index of the login user group from User Group
#define CSID_S_QOSC_UG_LOGOUT_ALTERED   _STA_QOS(0x0104) //T=u16, Group User log out
#define CSID_S_QOSC_UG_LOGOUTUSER       _STA_QOS(0x0105) //T=u16, index of the logout user from User Group
#define CSID_S_QOSC_UG_LOGOUTGROUP      _STA_QOS(0x0106) //T=u16, index of the logout user group from User Group
#define CSID_S_QOSC_UG_MODRULE          _STA_QOS(0x0107) //T=u16, Rule_ID of QoS edited for UserGroup rule
#define CSID_S_QOSC_UG_MODRULE_DATA     _STA_QOS(0x0108) //T=u16, Rule Class_Data of QoS edited for UserGroup rule
#define CSID_S_QOSC_UG_USERNUM          _STA_QOS(0x0600) //T=u16, count user number for UserGroup.
#define CSID_S_QOSC_UG_GROUPQDMA        _STA_QOS(0x0200) //T=u16, Group ID mapping TC Queue
#define CSID_S_QOSC_UG_GROUP_TCQUEUE    _STA_QOS(0x0620) //T=u16, the Group use which TC Queue(sharing method is Group)


#endif //ifndef _CSID_QOS_H
