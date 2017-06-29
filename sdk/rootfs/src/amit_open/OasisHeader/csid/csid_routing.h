/*
 * @file Member ID Definition of ROUTING.
 *
 * Moderator: JasonJiang
 * Group ID: 0x00130000/0x80130000
 */

#ifndef _CSID_ROUTING_H
#define _CSID_ROUTING_H

#include "csid_gid.h"

#define _CFG_ROUTING(x)         (CSID_GIDC_ROUTING|(x))
#define _STA_ROUTING(x)         (CSID_GIDS_ROUTING|(x))

/* Extra Definitions */

#define ROUTING_DYNAMIC_DISABLE         0x00
#define ROUTING_DYNAMIC_RIP_V1          0x01
#define ROUTING_DYNAMIC_RIP_V2          0x02

#define MULTIROUTE_WAN_IDX				0xE0
#define MULTIROUTE_WAN1_IDX				0xE1


/* Configuration Items */
#define CSID_C_ROUTING_DYNAMIC_ENABLE		    _CFG_ROUTING(0x0001)	//T=u8,  enable/disable dynamic routing (RIP v1/v2)
#define CSID_C_ROUTING_STATIC_ENABLE		    _CFG_ROUTING(0x0002)	//T=u8,  enable/disable static routing 
#define CSID_C_MULTIROUTE_FAILCHG_ENABLE	    _CFG_ROUTING(0x0003)	//T=u32, D=1, enable/disable multi-wan fail change
#define CSID_C_ROUTING_IPV6_STATIC_ENABLE		  _CFG_ROUTING(0x0004)	//T=u8, enable/disable static routing in IPv6 networking
#define CSID_C_ROUTING_IPV6_RIPNG_ENABLE		  _CFG_ROUTING(0x0005)	//T=u8, enable/disable ipv6 dynamic routing (RIPng)
#define CSID_C_ROUTING_DYNAMIC_FUNCTION_DISABLE		_CFG_ROUTING(0x0006) //T=u8, disable/enable all dynamic routing function (RIP/OSPF/BGP)

/* For OSPF general setting */
#define CSID_C_ROUTING_OSPF_ENABLE			    _CFG_ROUTING(0x0010)    //T=u8,  enable/disable dynamic routing:OSPF
#define CSID_C_ROUTING_OSPF_BACKBONE		    _CFG_ROUTING(0x0011)    //T=str, OSPF backbone subnet
//#define CSID_C_ROUTING_OSPF_DATA_STRING         _CFG_ROUTING(0x0012)    //T=str, OSPF data string, using save routing rule 
#define CSID_C_ROUTING_OSPF_PASSWORD            _CFG_ROUTING(0x0013)    //T=str, OSPF login password 
#define CSID_C_ROUTING_OSPF_ROUTER_ID       _CFG_ROUTING(0x0014)    //T=ipv4, OSPF router id
#define CSID_C_ROUTING_OSPF_AUTH_TYPE       _CFG_ROUTING(0x0015)    //T=u8, D=0, OSPF Authentication type (0 = None, 1 = Text, 2 = MD5)
#define CSID_C_ROUTING_OSPF_AUTH_ID         _CFG_ROUTING(0x0016)    //T=u32, OSPF Authentication ID for MD5 (Value = 1 ~ 255)
#define CSID_C_ROUTING_OSPF_AUTH_KEY        _CFG_ROUTING(0x0017)    //T=str, OSPF Authentication Key for Text/MD5 (Max Length : 8 on Text, 16 on MD5)

/* Customize for ZyXEL */
#define CSID_C_ROUTING_CUSTOMIZE_RULE		    _CFG_ROUTING(0x0020)	//T=u8, D=0, customize lan bind wan routing rule

/* For BGP general setting */
#define CSID_C_ROUTING_BGP_ENABLE			_CFG_ROUTING(0x0050)    //T=u8,  enable/disable dynamic routing:BGP
#define CSID_C_ROUTING_BGP_SELFID			_CFG_ROUTING(0x0051)    //T=u32,  BGP self ID value 
//#define CSID_C_ROUTING_BGP_DATA_STRING		_CFG_ROUTING(0x0052)	//T=str,  BGP data string, using save routing rule
#define CSID_C_ROUTING_BGP_PASSWORD             _CFG_ROUTING(0x0053)    //T=str, BGP login password 
#define CSID_C_ROUTING_BGP_ROUTER_ID       _CFG_ROUTING(0x0054)     //T=ipv4, BGP router id


/* For OSPF customize config. Reversed 64 items (0x0080 ~ 0x00BF) */
#define CSID_C_ROUTING_OSPF_DATA_STRING         _CFG_ROUTING(0x0080)    //T=str, OSPF data string, using save routing rule 

/* For BGP customize config. Reversed 64 items (0x00C0 ~ 0x00FF) */
#define CSID_C_ROUTING_BGP_DATA_STRING		_CFG_ROUTING(0x00C0)	//T=str,  BGP data string, using save routing rule


/* for Multi-WAN Multi-Routing Reserved 256 items*/
#define CSID_C_MULTIROUTE_ENABLE			_CFG_ROUTING(0x0100)	//T=u8, D=0, enable/disable multi routing rule
#define CSID_C_MULTIROUTE_PROTO				_CFG_ROUTING(0x0200)	//T=u8, D=0, protocol, 0:All, 1:TCP, 2:UDP, 3:Both
#define CSID_C_MULTIROUTE_WAN_INTERFACE		_CFG_ROUTING(0x0300)	//T=u32, D=0, wan interface	
#define CSID_C_MULTIROUTE_SRCIP_START		_CFG_ROUTING(0x0400)	//T=str, source start IP
#define CSID_C_MULTIROUTE_SRCIP_END			_CFG_ROUTING(0x0500)	//T=str, source end IP	
#define CSID_C_MULTIROUTE_DSTIP_START		_CFG_ROUTING(0x0600)	//T=str, destination start IP
#define CSID_C_MULTIROUTE_DSTIP_END			_CFG_ROUTING(0x0700)	//T=str, destination end IP
#define CSID_C_MULTIROUTE_DSTPORT_START		_CFG_ROUTING(0x0800)	//T=u32, destination start port
#define CSID_C_MULTIROUTE_DSTPORT_END		_CFG_ROUTING(0x0900)	//T=u32, destination end port
/* end */

#define CSID_C_ROUTING_STATIC_RULE_ENABLE	_CFG_ROUTING(0x1000)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_ROUTING_STATIC_DEST_IP		_CFG_ROUTING(0x1100)	//T=ipv4, destination ip
#define CSID_C_ROUTING_STATIC_SUB_MASK		_CFG_ROUTING(0x1200)	//T=ipv4, subnet mask
#define CSID_C_ROUTING_STATIC_GW_IP		_CFG_ROUTING(0x1300)	//T=ipv4, gateway ip
#define CSID_C_ROUTING_STATIC_HOP		_CFG_ROUTING(0x1400)	//T=u16, numbers of hop
#define CSID_C_ROUTING_STATIC_IFNAME		_CFG_ROUTING(0x1500)	//T=str, interface name
#define CSID_C_ROUTING_STATIC_NAME		_CFG_ROUTING(0x1600)	//T=str, static routing name
#define CSID_C_ROUTING_STATIC_IF_GROUP		_CFG_ROUTING(0x1700)	//T=u32, interface group, 0: Auto, 1: LAN/VLAN, 2: WAN
#define CSID_C_ROUTING_STATIC_IF_IDX		_CFG_ROUTING(0x1800)	//T=u32, index number for interface type

/* For OSPF rule setting, reserved 256 items */
#define CSID_C_ROUTING_OSPF_RULE_ENABLE				_CFG_ROUTING(0x2100)    //T=u8,  enable/disable OSPF rules
#define CSID_C_ROUTING_OSPF_RULE_SUBNET				_CFG_ROUTING(0x2200)    //T=str, OSPF Area subnet
#define CSID_C_ROUTING_OSPF_RULE_ID					_CFG_ROUTING(0x2300)    //T=ipv4, OSPF Area ID


/* For BGP rule setting, reserved 256 items */
#define CSID_C_ROUTING_BGP_RULE_ENABLE		_CFG_ROUTING(0x3000)    //T=u8,  enable/disable BGP rules
#define CSID_C_ROUTING_BGP_RULE_IP			_CFG_ROUTING(0x3100)    //T=ipv4, BGP Neighbor IP
#define CSID_C_ROUTING_BGP_RULE_ID			_CFG_ROUTING(0x3200)    //T=u16, BGP Neighbor ID
//#define CSID_C_ROUTING_BGP_RULE_SUBNET		_CFG_ROUTING(0x7400)    //T=u16, BGP Out subnet
#define CSID_C_ROUTING_BGP_SUBNET_RULE_ENABLE				_CFG_ROUTING(0x3300)    //T=u8,  enable/disable BGP network subnet rules
#define CSID_C_ROUTING_BGP_SUBNET           				_CFG_ROUTING(0x3400)    //T=str, BGP network subnet

/* for Multi-WAN Multi-Routing extra configuration */
#define CSID_C_MULTIROUTE_SRCIP_TYPE        _CFG_ROUTING(0x4000)    //T=u8, D=0, source IP type, 0:any, 1:subnet, 2:range, 3:single
#define CSID_C_MULTIROUTE_DSTIP_TYPE        _CFG_ROUTING(0x4100)    //T=u8, D=0, destination IP type, 0:any, 1:subnet, 2:range, 3:single, 4:domain name
#define CSID_C_MULTIROUTE_DSTPORT_TYPE      _CFG_ROUTING(0x4200)    //T=u8, D=0, destination port type, 0:all, 1:range, 2:single, 3:well-known application

/* for Static Routing in IPv6, reserved 256 items */
#define CSID_C_ROUTING_IPV6_STATIC_RULE_ENABLE  _CFG_ROUTING(0x5000)	//T=u8, ipv6 static rule enable, 1:yes 0:no
#define CSID_C_ROUTING_IPV6_STATIC_DEST_IP      _CFG_ROUTING(0x5100)	//T=str, ipv6 destination
#define CSID_C_ROUTING_IPV6_STATIC_PREFIX_LEN   _CFG_ROUTING(0x5200)	//T=u32, ipv6 prefix length
#define CSID_C_ROUTING_IPV6_STATIC_GW_IP        _CFG_ROUTING(0x5300)	//T=str, ipv6 gateway ip
#define CSID_C_ROUTING_IPV6_STATIC_HOP          _CFG_ROUTING(0x5400)	//T=u16, ipv6 numbers of hop
#define CSID_C_ROUTING_IPV6_STATIC_NAME         _CFG_ROUTING(0x5500)	//T=str, ipv6 static routing name
#define CSID_C_ROUTING_IPV6_STATIC_IF_GROUP     _CFG_ROUTING(0x5600)	//T=u32, ipv6 interface group, 0: Auto, 1: LAN/VLAN, 2: WAN
#define CSID_C_ROUTING_IPV6_STATIC_IF_IDX       _CFG_ROUTING(0x5700)	//T=u32, ipv6 index number for interface type


/* For Zebra customize config. Reversed 64 items (0xF000 ~ 0xF07F) */
#define CSID_C_ROUTING_ZEBRA_DATA_STRING        _CFG_ROUTING(0xF000)    //T=str, Zebra data string, using save routing rule 

/* For RIP customize config. Reversed 64 items (0xF080 ~ 0xF0BF) */
#define CSID_C_ROUTING_RIP_DATA_STRING		_CFG_ROUTING(0xF080)	//T=str, RIP data string, using save routing rule


/* Status Items */
#define CSID_S_ROUTING_STATIC_RULE_NUM		_STA_ROUTING(0x0001)	//T=u16, number of static routing rules
#define CSID_S_ROUTING_ALTERED				_STA_ROUTING(0x0002)	//T=u8, ui altered
#define CSID_S_ROUTING_RESTART_FLAG			_STA_ROUTING(0x0003)	//T=u16, D=0, check pptp pptpserver l2tp l2tp server if connect write 1, to run ripd restart write 0
/* WAN NM NUM reserved 16 items */
#define CSID_S_ROUTING_WAN_NM_NUM			_STA_ROUTING(0x0004)	//T=u32, WAN subnet mask value

#define CSID_S_HANDLE_ROUTING_ALTERED      _STA_ROUTING(0x0010)    //T=u8, handle routing altered

/* LAN NM NUM reversed 16 items */
#define CSID_S_ROUTING_LAN_NM_NUM			_STA_ROUTING(0x0020)	//T=u32, LAN subnet mask value

/*Routing Information */
#define CSID_S_ROUTING_INFO_DEST		_STA_ROUTING(0x1000)	//T=str, routing info dest
#define CSID_S_ROUTING_INFO_GATEWAY		_STA_ROUTING(0x1100)	//T=str, routing info gateway
#define CSID_S_ROUTING_INFO_GENMASK		_STA_ROUTING(0x1200)	//T=str, routing info genmask
#define CSID_S_ROUTING_INFO_METRIC		_STA_ROUTING(0x1300)	//T=u8, routing infot metric
#define CSID_S_ROUTING_INFO_INTERFACE		_STA_ROUTING(0x1400)	//T=str, routing info Interfacee
#define CSID_S_ROUTING_INFO_NUMBER	        _STA_ROUTING(0x1500)	//T=u32, routing info total number

#endif //ifndef _CSID_ROUTING_H
