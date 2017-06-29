/*
 * @file Member ID Definition of Group PKFILTER.
 *
 * Moderator: JasonJiang
 * Group ID: 0x000e0000/0x800e0000
 */

#ifndef _CSID_PKFILTER_H
#define _CSID_PKFILTER_H

#include "csid_gid.h"

#define _CFG_PKFILTER(x)                    (CSID_GIDC_PKFILTER|(x))
#define _STA_PKFILTER(x)                    (CSID_GIDS_PKFILTER|(x))

/* Configuration Items */
#define CSID_C_PKFILTER_OUT_ENABLE           _CFG_PKFILTER(0x0001) //T=u8, whether starting pf_outbound rule, 1:yes 0:no
#define CSID_C_PKFILTER_IN_ENABLE            _CFG_PKFILTER(0x0002) //T=u8, whether starting pf_inbound rule, 1:yes 0:no
#define CSID_C_PKFILTER_OUT_DENY_ALL         _CFG_PKFILTER(0x0003) //T=u8, 0:allow all except,1:deny all except,2:rule with its action 
#define CSID_C_PKFILTER_IN_DENY_ALL          _CFG_PKFILTER(0x0004) //T=u8, deny all except any rule is matched
#define CSID_C_PKFILTER_SIP_DENY         	 _CFG_PKFILTER(0x0005) //T=u8, set 1: deny all SIP packet.
#define CSID_C_PKFILTER_MODE		         _CFG_PKFILTER(0x0006) //T=u8, 0:seperate 1: both 
#define CSID_C_PKFILTER_BOTH_ENABLE          _CFG_PKFILTER(0x0007) //T=u8, whether starting 
#define CSID_C_PKFILTER_BOTH_DENY_ALL        _CFG_PKFILTER(0x0008) //T=u8, 0:allow all except,1:deny all except,2:rule with its action 
#define CSID_C_PKFILTER_L7APP_BLOCK_ENABLE        _CFG_PKFILTER(0x0009) //T=u8, whether starting L7_app blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_OUT_LOG_ENABLE        _CFG_PKFILTER(0x000A) //T=u8, enable/disable log the status of PKTIN block rules, 1:yes 0:no
#define CSID_C_PKFILTER_IN_LOG_ENABLE        _CFG_PKFILTER(0x000B) //T=u8, enable/disable log the status of PKTOUT block rules, 1:yes 0:no
#define CSID_C_PKFILTER_BOTH_LOG_ENABLE        _CFG_PKFILTER(0x000C) //T=u8, enable/disable log the status of PKTBOTH block rules, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_LOG_ENABLE   _CFG_PKFILTER(0x000D) //T=u8, enable/disable log the status of L7APP block rules, 1:yes 0:no


// Reserved for L7APP Blocking (0020 ~ 0F9F, L7 Profile: 128, L7 Items space: 31)
#define CSID_C_PKFILTER_L7APP_BLOCK_RULE_ENABLE                     _CFG_PKFILTER(0x0020) //T=u8, L7_app rule enable, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_SCHE_NO                         _CFG_PKFILTER(0x00A0) //T=u16, number of using rule
#define CSID_C_PKFILTER_L7APP_BLOCK_RULE_NAME                       _CFG_PKFILTER(0x0120) //T=str, using rule name
#define CSID_C_PKFILTER_L7APP_BLOCK_RULE_SIP0                       _CFG_PKFILTER(0x01A0) //T=ipv4, start of source IP
#define CSID_C_PKFILTER_L7APP_BLOCK_RULE_SIP                        _CFG_PKFILTER(0x0220) //T=str, source IP range
#define CSID_C_PKFILTER_L7APP_BLOCK_RULE_MAC                        _CFG_PKFILTER(0x02A0) //T=mac, source MAC address
#define CSID_C_PKFILTER_L7APP_BLOCK_SIP_GROUP_NAME                  _CFG_PKFILTER(0x0320) //T=str, apply source ip from IP Address-based group name
#define CSID_C_PKFILTER_L7APP_BLOCK_MAC_GROUP_NAME                  _CFG_PKFILTER(0x03A0) //T=str, apply source mac from MAC Address-based group name
#define CSID_C_PKFILTER_L7APP_BLOCK_APP_GROUP_NAME                  _CFG_PKFILTER(0x0420) //T=str, apply application from Application-based group name
#define CSID_C_PKFILTER_L7APP_BLOCK_QQ_ENABLE                       _CFG_PKFILTER(0x04A0) //T=u8, whether starting QQ blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_FB_ENABLE                       _CFG_PKFILTER(0x0520) //T=u8, whether starting facebook blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_SKYPE_ENABLE                    _CFG_PKFILTER(0x05A0) //T=u8, whether starting skype blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_ALIWW_ENABLE                    _CFG_PKFILTER(0x0620) //T=u8, whether starting Aliww rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_LINE_ENABLE                     _CFG_PKFILTER(0x06A0) //T=u8, whether starting LINE blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_BT_ENABLE                       _CFG_PKFILTER(0x0720) //T=u8, whether starting BT blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_ED2K_ENABLE                     _CFG_PKFILTER(0x07A0) //T=u8, whether starting ED2K blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_MT_HTTP_DL_ENABLE               _CFG_PKFILTER(0x0820) //T=u8, whether starting Multi-thread HTTP Download blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_THUNDER_ENABLE                  _CFG_PKFILTER(0x08A0) //T=u8, whether starting Thunder blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_BAOFENG_ENABLE                  _CFG_PKFILTER(0x0920) //T=u8, whether starting Baofeng blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_PROXY_ENABLE                    _CFG_PKFILTER(0x09A0) //T=u8, whether starting proxy (http) blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_PROXY_SOCKS_4_5_ENABLE          _CFG_PKFILTER(0x0A20) //T=u8, whether starting proxy socks 4/5 blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_MMS_ENABLE                      _CFG_PKFILTER(0x0AA0) //T=u8, whether starting MMS blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_RTSP_ENABLE                     _CFG_PKFILTER(0x0B20) //T=u8, whether starting RTSP blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_PPSTREAM_ENABLE                 _CFG_PKFILTER(0x0BA0) //T=u8, whether starting PPStream blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_PPLIVE_ENABLE                   _CFG_PKFILTER(0x0C20) //T=u8, whether starting PPLive blocking rule, 1:yes 0:no
#define CSID_C_PKFILTER_L7APP_BLOCK_QVOD_ENABLE                     _CFG_PKFILTER(0x0CA0) //T=u8, whether starting Qvod blocking rule, 1:yes 0:no


//===================
#define CSID_C_PKFILTER_OUT_RULE_ENABLE      _CFG_PKFILTER(0x1000) //T=u8, rule enable, 1:yes p:no
#define CSID_C_PKFILTER_OUT_RULE_SIP0        _CFG_PKFILTER(0x1800) //T=ipv4, start of source IP range
#define CSID_C_PKFILTER_OUT_RULE_SIP       	 _CFG_PKFILTER(0x2000) //T=str,  source IP range
#define CSID_C_PKFILTER_OUT_RULE_SPORT       _CFG_PKFILTER(0x2800) //T=str, source Port range:192.168.1.0-192.168.1.254
#define CSID_C_PKFILTER_OUT_RULE_PRO         _CFG_PKFILTER(0x3000) //T=u16, source/dest protocol 0:All 1:TCP,2:UDP,3:Both,4:ICMP
#define CSID_C_PKFILTER_OUT_RULE_DIP0        _CFG_PKFILTER(0x3800) //T=ipv4, start of destination IP range
#define CSID_C_PKFILTER_OUT_RULE_DIP         _CFG_PKFILTER(0x4000) //T=str, destination IP range:192.168.1.0-192.168.1.254
#define CSID_C_PKFILTER_OUT_RULE_DPORT       _CFG_PKFILTER(0x4800) //T=str, destination Port range
#define CSID_C_PKFILTER_OUT_RULE_SCHE_NO     _CFG_PKFILTER(0x5800) //T=u16, number of using rule
#define CSID_C_PKFILTER_OUT_RULE_NAME        _CFG_PKFILTER(0x5900) //T=str, using rule name
#define CSID_C_PKFILTER_OUT_RULE_SIFNAME      _CFG_PKFILTER(0x5A00) //T=u16, using rule source interface name
#define CSID_C_PKFILTER_OUT_RULE_DIFNAME      _CFG_PKFILTER(0x5B00) //T=u16, using rule destination interface name
#define CSID_C_PKFILTER_OUT_RULE_ACTION	      _CFG_PKFILTER(0x5C00) //T=u8, rule action, 0: DROP, 1:ACCEPT

//===================
#define CSID_C_PKFILTER_BOTH_RULE_ENABLE      _CFG_PKFILTER(0x7000) //T=u8, rule enable, 1:yes p:no
#define CSID_C_PKFILTER_BOTH_RULE_SIP0        _CFG_PKFILTER(0x7100) //T=ipv4, start of source IP range
#define CSID_C_PKFILTER_BOTH_RULE_SIP       	 _CFG_PKFILTER(0x7200) //T=str,  source IP range
#define CSID_C_PKFILTER_BOTH_RULE_SPORT       _CFG_PKFILTER(0x7300) //T=str, source Port 
#define CSID_C_PKFILTER_BOTH_RULE_PRO         _CFG_PKFILTER(0x7400) //T=u16, protocol 0:Any,1:TCP,2:UDP,3:Both,4:ICMP,5:GRE,6:AH,7:ESP,8:SCTP
#define CSID_C_PKFILTER_BOTH_RULE_DIP0        _CFG_PKFILTER(0x7500) //T=ipv4, start of destination IP range
#define CSID_C_PKFILTER_BOTH_RULE_DIP         _CFG_PKFILTER(0x7600) //T=str, destination IP
#define CSID_C_PKFILTER_BOTH_RULE_DPORT       _CFG_PKFILTER(0x7700) //T=str, destination Port range

#define CSID_C_PKFILTER_BOTH_RULE_SCHE_NO     _CFG_PKFILTER(0x7800) //T=u16, number of using rule
#define CSID_C_PKFILTER_BOTH_RULE_NAME        _CFG_PKFILTER(0x7900) //T=str, using rule name
#define CSID_C_PKFILTER_BOTH_RULE_SIFNAME      _CFG_PKFILTER(0x7A00) //T=u16, using rule source interface name
#define CSID_C_PKFILTER_BOTH_RULE_DIFNAME      _CFG_PKFILTER(0x7B00) //T=u16, using rule destination interface name
#define CSID_C_PKFILTER_BOTH_RULE_ACTION	   _CFG_PKFILTER(0x7C00) //T=u8, rule action, 0: DROP, 1:ACCEPT

#define CSID_C_PKFILTER_BOTH_RULE_MAC         _CFG_PKFILTER(0x7D00) //T=mac, MAC address

#define CSID_C_PKFILTER_BOTH_RULE_SIFNAME_IDX  _CFG_PKFILTER(0x7E00) //T=u16, the index of rule source interface name by SIFNAME (0: Any LAN or Any WAN, 1: LAN1 or WAN1, 2: LAN2 or WAN2 ...)
#define CSID_C_PKFILTER_BOTH_RULE_DIFNAME_IDX  _CFG_PKFILTER(0x7F00) //T=u16, the index of rule destination interface name by DIFNAME (0: Any or Any WAN, 1: LAN1 or WAN1, 2: LAN2 or WAN2 ...)

#define CSID_C_PKFILTER_BOTH_RULE_IP_PRO       _CFG_PKFILTER(0x8000) //T=u8, ip protocol number (Value range: 0 ~ 255)

#define CSID_C_PKFILTER_BOTH_RULE_SIP_GROUP_NAME _CFG_PKFILTER(0x8100) //T=str, apply source ip from IP Address-based group name
#define CSID_C_PKFILTER_BOTH_RULE_DIP_GROUP_NAME _CFG_PKFILTER(0x8200) //T=str, apply destination ip from IP Address-based group name
#define CSID_C_PKFILTER_BOTH_RULE_MAC_GROUP_NAME _CFG_PKFILTER(0x8300) //T=str, apply source mac from MAC Address-based group name

//===================


#define CSID_C_PKFILTER_IN_RULE_ENABLE       _CFG_PKFILTER(0x9000) //T=u8, rule enable, 1:yes 0:no
#define CSID_C_PKFILTER_IN_RULE_SIP0         _CFG_PKFILTER(0x9800) //T=ipv4, start of source IP range
#define CSID_C_PKFILTER_IN_RULE_SIP          _CFG_PKFILTER(0xA000) //T=str, source IP range:192.168.1.0-192.168.1.254
#define CSID_C_PKFILTER_IN_RULE_SPORT        _CFG_PKFILTER(0xA800) //T=str, source Port range
#define CSID_C_PKFILTER_IN_RULE_PRO          _CFG_PKFILTER(0xB000) //T=u16, source/destination protocol 1:TCP,2:UDP,3:Both 
//#define CSID_C_PKFILTER_IN_RULE_SPORT1      _CFG_PKFILTER(0xB000) //T=u16, end of source Port range
#define CSID_C_PKFILTER_IN_RULE_DIP0         _CFG_PKFILTER(0xB800) //T=ipv4, start of destination IP range
#define CSID_C_PKFILTER_IN_RULE_DIP          _CFG_PKFILTER(0xC000) //T=str, destination IP range:192.168.1.0-192.168.1.254
#define CSID_C_PKFILTER_IN_RULE_DPORT        _CFG_PKFILTER(0xC800) //T=str, destination Port range
//#define CSID_C_PKFILTER_IN_RULE_DPORT1      _CFG_PKFILTER(0xD000) //T=u16, end of destination Port range
#define CSID_C_PKFILTER_IN_RULE_SCHE_NO      _CFG_PKFILTER(0xD800) //T=u16, number of using rule

/* Status Items */
#define CSID_S_PKFILTER_IN_RULE_NUM          _STA_PKFILTER(0x0001) //T=u16, number of pf_inbound rules
#define CSID_S_PKFILTER_OUT_RULE_NUM         _STA_PKFILTER(0x0002) //T=u16, number of pf_outbound rules
#define CSID_S_PKFILTER_ALTERED			     _STA_PKFILTER(0x0003) //T=u8, ui altered
#define CSID_S_PKFILTER_BOTH_RULE_NUM      _STA_PKFILTER(0x0004) //T=u16, number of pf_outbound rules
#define CSID_S_PKFILTER_L7APP_BLOCK_ALTERED	   _STA_PKFILTER(0x0005) //T=u8, ui altered
#define CSID_S_PKFILTER_L7APP_BLOCK_SCHE_STATUS  _STA_PKFILTER(0x0006) //T=u8, layer 7 applicaiotn block schedule rule status

#define CSID_S_PKFILTER_OUT_RULE_SCHE_STATUS _STA_PKFILTER(0x0100) //T=u8, Out schedule rule status
#define CSID_S_PKFILTER_IN_RULE_SCHE_STATUS  _STA_PKFILTER(0x0200) //T=u8, In schedule rule status
#define CSID_S_PKFILTER_BOTH_RULE_SCHE_STATUS  _STA_PKFILTER(0x0300) //T=u8, In schedule rule status

#endif //ifndef _CSID_PKFILTER_H
