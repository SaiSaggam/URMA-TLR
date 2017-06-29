/*
 * @file Member ID Definition of Group PORTFW.
 *
 * Moderator: JasonJiang
 * Group ID: 0x000C0000/0x800C0000
 */

#ifndef _CSID_PORTFW_H
#define _CSID_PORTFW_H

#include "csid_gid.h"

#define _CFG_PORTFW(x)         (CSID_GIDC_PORTFW|(x))
#define _STA_PORTFW(x)         (CSID_GIDS_PORTFW|(x))

#define PROTO_BOTH	0
#define PROTO_TCP	1
#define PROTO_UDP	2
#define PROTO_OTHER	3

/* Configuration Items */
#define CSID_C_PORTFW_ENABLE			_CFG_PORTFW(0x0001)	//T=u8, whether starting port fowarding, 1:yes 0:no
#define CSID_C_MULTIWAN_PORTFW_MODE		_CFG_PORTFW(0x0002)	//T=u8, singleIP or multipleIP, 0:single 1:multiple
#define CSID_C_PORTFW_DROP_ENABLE		_CFG_PORTFW(0x0006)	//T=u32, drop enable
#define CSID_C_MULTIWAN_PORTFW_ENABLE		_CFG_PORTFW(0x0010)	//T=u8, whether starting port fowarding, 1:yes 0:no
#define CSID_C_PORTFW_RECORD_RULENUM	_CFG_PORTFW(0x0080)	//T=str, record the rule sequence for ZyXEL UI, reserve 128
#define CSID_C_PORTFW_OPEN_PORT_ENABLE  _CFG_PORTFW(0x0100) //T=u8, whether starting port open, 1:yes 0:no

#define CSID_C_PORTFW_RULE_ENABLE		_CFG_PORTFW(0x1000)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN0_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1080)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN1_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1100)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN2_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1180)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN3_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1200)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN4_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1280)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN5_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1300)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN6_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1380)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN7_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1400)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN8_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1480)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN9_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1500)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN10_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1580)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN11_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1600)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN12_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1680)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN13_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1700)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN14_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1780)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MULTIWAN15_PORTFW_RULE_ENABLE	_CFG_PORTFW(0x1800)	//T=u8, rule enable, 1:yes 0:no

#define CSID_C_PORTFW_SERVER_IP			_CFG_PORTFW(0x2000)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN0_PORTFW_SERVER_IP	_CFG_PORTFW(0x2080)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN1_PORTFW_SERVER_IP	_CFG_PORTFW(0x2100)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN2_PORTFW_SERVER_IP	_CFG_PORTFW(0x2180)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN3_PORTFW_SERVER_IP	_CFG_PORTFW(0x2200)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN4_PORTFW_SERVER_IP	_CFG_PORTFW(0x2280)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN5_PORTFW_SERVER_IP	_CFG_PORTFW(0x2300)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN6_PORTFW_SERVER_IP	_CFG_PORTFW(0x2380)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN7_PORTFW_SERVER_IP	_CFG_PORTFW(0x2400)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN8_PORTFW_SERVER_IP	_CFG_PORTFW(0x2480)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN9_PORTFW_SERVER_IP	_CFG_PORTFW(0x2500)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN10_PORTFW_SERVER_IP	_CFG_PORTFW(0x2580)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN11_PORTFW_SERVER_IP	_CFG_PORTFW(0x2600)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN12_PORTFW_SERVER_IP	_CFG_PORTFW(0x2680)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN13_PORTFW_SERVER_IP	_CFG_PORTFW(0x2700)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN14_PORTFW_SERVER_IP	_CFG_PORTFW(0x2780)	//T=ipv4, internal host ip
#define CSID_C_MULTIWAN15_PORTFW_SERVER_IP	_CFG_PORTFW(0x2800)	//T=ipv4, internal host ip

#define CSID_C_PORTFW_SERVER_PORT		_CFG_PORTFW(0x3000)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN0_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3080)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN1_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3100)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN2_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3180)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN3_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3200)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN4_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3280)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN5_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3300)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN6_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3380)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN7_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3400)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN8_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3480)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN9_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3500)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN10_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3580)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN11_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3600)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN12_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3680)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN13_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3700)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN14_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3780)	//T=str, forwarding port including port-range, ex 20-30
#define CSID_C_MULTIWAN15_PORTFW_SERVER_PORT	_CFG_PORTFW(0x3800)	//T=str, forwarding port including port-range, ex 20-30

#define CSID_C_PORTFW_USE_RULE_NO		_CFG_PORTFW(0x4000)	//T=u16, use_rule number
#define CSID_C_MULTIWAN0_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4080)	//T=u16, use_rule number
#define CSID_C_MULTIWAN1_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4100)	//T=u16, use_rule number
#define CSID_C_MULTIWAN2_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4180)	//T=u16, use_rule number
#define CSID_C_MULTIWAN3_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4200)	//T=u16, use_rule number
#define CSID_C_MULTIWAN4_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4280)	//T=u16, use_rule number
#define CSID_C_MULTIWAN5_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4300)	//T=u16, use_rule number
#define CSID_C_MULTIWAN6_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4380)	//T=u16, use_rule number
#define CSID_C_MULTIWAN7_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4400)	//T=u16, use_rule number
#define CSID_C_MULTIWAN8_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4480)	//T=u16, use_rule number
#define CSID_C_MULTIWAN9_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4500)	//T=u16, use_rule number
#define CSID_C_MULTIWAN10_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4580)	//T=u16, use_rule number
#define CSID_C_MULTIWAN11_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4600)	//T=u16, use_rule number
#define CSID_C_MULTIWAN12_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4680)	//T=u16, use_rule number
#define CSID_C_MULTIWAN13_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4700)	//T=u16, use_rule number
#define CSID_C_MULTIWAN14_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4780)	//T=u16, use_rule number
#define CSID_C_MULTIWAN15_PORTFW_USE_RULE_NO	_CFG_PORTFW(0x4800)	//T=u16, use_rule number

#define CSID_C_PORTFW_PRIVATE_PORT		_CFG_PORTFW(0x5000)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN0_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5080)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN1_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5100)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN2_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5180)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN3_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5200)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN4_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5280)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN5_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5300)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN6_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5380)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN7_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5400)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN8_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5480)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN9_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5500)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN10_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5580)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN11_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5600)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN12_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5680)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN13_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5700)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN14_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5780)	//T=str, private port for lan server bind
#define CSID_C_MULTIWAN15_PORTFW_PRIVATE_PORT	_CFG_PORTFW(0x5800)	//T=str, private port for lan server bind

#define CSID_C_PORTFW_PROTOCOL			_CFG_PORTFW(0x6000)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN0_PORTFW_PROTOCOL	_CFG_PORTFW(0x6080)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN1_PORTFW_PROTOCOL	_CFG_PORTFW(0x6100)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN2_PORTFW_PROTOCOL	_CFG_PORTFW(0x6180)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN3_PORTFW_PROTOCOL	_CFG_PORTFW(0x6200)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN4_PORTFW_PROTOCOL	_CFG_PORTFW(0x6280)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN5_PORTFW_PROTOCOL	_CFG_PORTFW(0x6300)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN6_PORTFW_PROTOCOL	_CFG_PORTFW(0x6380)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN7_PORTFW_PROTOCOL	_CFG_PORTFW(0x6400)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN8_PORTFW_PROTOCOL	_CFG_PORTFW(0x6480)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN9_PORTFW_PROTOCOL	_CFG_PORTFW(0x6500)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN10_PORTFW_PROTOCOL	_CFG_PORTFW(0x6580)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN11_PORTFW_PROTOCOL	_CFG_PORTFW(0x6600)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN12_PORTFW_PROTOCOL	_CFG_PORTFW(0x6680)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN13_PORTFW_PROTOCOL	_CFG_PORTFW(0x6700)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN14_PORTFW_PROTOCOL	_CFG_PORTFW(0x6780)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other
#define CSID_C_MULTIWAN15_PORTFW_PROTOCOL	_CFG_PORTFW(0x6800)	//T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP 3:Other

#define CSID_C_PORTFW_EXTERNAL_PORT		_CFG_PORTFW(0x7000)     //T=str, external port
#define CSID_C_PORTFW_RULE_NAME			_CFG_PORTFW(0x8000)	//T=str, rule name
/* Protocol ID , 256 items are reserved */
#define CSID_C_PORTFW_PROTOCOL_ID		_CFG_PORTFW(0x9000)	//T=str, D="", Protocol ID 0 ~ 255

/*port open*/
#define CSID_C_PORTFW_OPEN_PORT_RULE_ENABLE     _CFG_PORTFW(0xA000) //T=u8, whether starting port open per rule, 1:yes 0::no
#define CSID_C_PORTFW_OPEN_PORT_RULE_NAME       _CFG_PORTFW(0xA080) //T=str, D="", port open name
#define CSID_C_PORTFW_OPEN_PORT_TCP_PORT        _CFG_PORTFW(0xA100) //T=str, D="", public port for tcp
#define CSID_C_PORTFW_OPEN_PORT_UDP_PORT        _CFG_PORTFW(0xA180) //T=str, D="", public port for udp
#define CSID_C_PORTFW_OPEN_PORT_SERVER_IP       _CFG_PORTFW(0xA200) //T=ipv4,private address for open port
#define CSID_C_PORTFW_OPEN_PORT_USE_RULE_NO     _CFG_PORTFW(0xA280) //T=u16,use_rule number

/*port select index*/
#define CSID_C_PORTFW_WELLKNOWN_INDEX     _CFG_PORTFW(0xB000) //T=u32, D=0, for ui to save well-known service

/* Multi-WAN Selection for Port Forwarding */
#define CSID_C_PORTFW_WAN_LIST            _CFG_PORTFW(0xC000) //T=u32, WAN List for Port Forwarding, 0: All, 1: WAN1, 2: WAN2, 3: WAN1~2, etc...

/* Status Items */
#define CSID_S_PORTFW_RULE_NUM			_STA_PORTFW(0x0001)	//T=u16, number of port-fowarding rules
#define CSID_S_PORTFW_ALTERED			_STA_PORTFW(0x0002)	//T=u8, ui altered
#define CSID_S_PORTFW_OPEN_PORT_ALTERED _STA_PORTFW(0x0003)	//T=u8, ui altered
#define CSID_S_PORTFW_RULE_STATUS		_STA_PORTFW(0x0100)	//T=u8, schedule_rule status


#define CSID_S_PORTFW_OPEN_PORT_RULE_STATUS     _STA_PORTFW(0x0200) //T=u8, schedule_rule status

#define CSID_S_PORTFW_TMP_SERVER_PORT		_STA_PORTFW(0x0300)	//T=str, tmp forwarding port

#endif //ifndef _CSID_PORTFW_H
