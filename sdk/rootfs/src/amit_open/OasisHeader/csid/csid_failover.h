/*
 * @file Member ID Definition of Group Failover
 *
 * Moderator: ElektraChen
 * Group ID: 0x004300/0x804300
 */

#ifndef _CSID_FAILOVER_H
#define _CSID_FAILOVER_H

#include "csid_gid.h"

#define _CFG_FAILOVER(x)		(CSID_GIDC_FAILOVER|(x))
#define _STA_FAILOVER(x)		(CSID_GIDS_FAILOVER|(x))

/* Extra Definitions */
#define INTERFACE_IDX_FAILOVER_MODEM			0x01
#define INTERFACE_IDX_FAILOVER_3G				0x02
//#define INTERFACE_IDX_FAILOVER_3G2				0x03
#define INTERFACE_IDX_FAILOVER_FIXED			0x04
#define INTERFACE_IDX_FAILOVER_DYNAMIC			0x05
#define INTERFACE_IDX_FAILOVER_PPPOE			0x06
#define INTERFACE_IDX_FAILOVER_PPTP				0x07
#define INTERFACE_IDX_FAILOVER_L2TP				0x08
#define INTERFACE_IDX_FAILOVER_PPPOA			0x09
#define INTERFACE_IDX_FAILOVER_IPOA_DHCP		0x0A
#define INTERFACE_IDX_FAILOVER_IPOA_FIXED		0x0B
#define INTERFACE_IDX_FAILOVER_FLASHOFDM		0x0C
#define INTERFACE_IDX_FAILOVER_IBURST   		0x0D

#define FAILOVER_CNT_STATUS_DISCONNECT		0x00
#define FAILOVER_CNT_STATUS_CONNECT			0x01

/* LoadSharing Second WAN Connect status */
#define LOADSHARING_WAN_STATUS_DISCONNECT		0x00
#define LOADSHARING_WAN_STATUS_CONNECTING		0x01
#define LOADSHARING_WAN_STATUS_CONNECT			0x02
#define LOADSHARING_WAN_STATUS_DISCONNECTING	0x03

/* LoadSharing action run status */
#define LOADSHARING_RUN_STATUS_START			0x01
#define LOADSHARING_RUN_STATUS_END				0x02

/* check index */
#define CHECK_INDEX_NONE		0x00
#define CHECK_INDEX_DNS1		0x01
#define CHECK_INDEX_DNS2		0x02
#define CHECK_INDEX_GATEWAY	    0x03
#define CHECK_INDEX_OTHERHOST	0x04

/* Configuration Items */
#define CSID_C_FAILOVER_ENABLE                      _CFG_FAILOVER(0x0001)	//T=u32,D=0,auto backup enable, 0:disable, 1:enable
#define CSID_C_FAILOVER_PING_HOST                   _CFG_FAILOVER(0x0002)	//T=str, ping host for auto backup
#define CSID_C_FAILOVER_IF_MODEM_ENABLE             _CFG_FAILOVER(0x0003)	//T=u8, D=1,interface modem enable
#define CSID_C_FAILOVER_IF_3G_ENABLE                _CFG_FAILOVER(0x0004)	//T=u8, D=1,interface 3G1 enable
//#define CSID_C_FAILOVER_IF_3G2_ENABLE                 _CFG_FAILOVER(0x0005)	//T=u8, D=1,interface 3G2 enable
#define CSID_C_FAILOVER_IF_FIXED_ENABLE             _CFG_FAILOVER(0x0006)	//T=u8, D=1,interface fixed enable
#define CSID_C_FAILOVER_IF_DYNAMIC_ENABLE           _CFG_FAILOVER(0x0007)	//T=u8, D=1,interface dynamic enable
#define CSID_C_FAILOVER_IF_PPPOE_ENABLE             _CFG_FAILOVER(0x0008)	//T=u8, D=1,interface pppoe enable
#define CSID_C_FAILOVER_IF_PPTP_ENABLE              _CFG_FAILOVER(0x0009)	//T=u8, D=1,interface pptp enable
#define CSID_C_FAILOVER_IF_L2TP_ENABLE              _CFG_FAILOVER(0x000a)	//T=u8, D=1,interface l2tp enable
#define CSID_C_FAILOVER_WANTYPE                     _CFG_FAILOVER(0x000b)   //T=u32, failover wan type
#define CSID_C_FAILOVER_CHECK_MODE                  _CFG_FAILOVER(0x000c)   //T=u8, D=0, multiwanchk mode
#define CSID_C_FAILOVER_SEAMLESS_ENABLE             _CFG_FAILOVER(0x000d)    //T=u8,D=0, wan stay-by failover enable
#define CSID_C_FAILOVER_SEQUENCE                    _CFG_FAILOVER(0x000e) //T=u8,D=0, wan failover sequence

#define CSID_C_FAILOVER_IF_MODEM_IDX_LIST        	_CFG_FAILOVER(0x0010)	//T=u32, D=0,index of modem fo list
#define CSID_C_FAILOVER_IF_3G_IDX_LIST          	_CFG_FAILOVER(0x0020)	//T=u32, D=0,index of 3G fo list
#define CSID_C_FAILOVER_IF_FIXED_IDX_LIST        	_CFG_FAILOVER(0x0030)	//T=u32, D=0,index of fixed fo list
#define CSID_C_FAILOVER_IF_DYNAMIC_IDX_LIST      	_CFG_FAILOVER(0x0040)	//T=u32, D=0,index of dynamic fo list
#define CSID_C_FAILOVER_IF_PPPOE_IDX_LIST       	_CFG_FAILOVER(0x0050)	//T=u32, D=0,index of pppoe fo list
#define CSID_C_FAILOVER_IF_PPTP_IDX_LIST        	_CFG_FAILOVER(0x0060)	//T=u32, D=0,index of pptp fo list
#define CSID_C_FAILOVER_IF_L2TP_IDX_LIST         	_CFG_FAILOVER(0x0070)	//T=u32, D=0,index of l2tp fo list
#define CSID_C_FAILOVER_IF_FREE_IDX_LIST         	_CFG_FAILOVER(0x0080)	//T=u32, D=0,index of l2tp fo list
#define CSID_C_FAILOVER_IF_MODEM_IF_LIST        	_CFG_FAILOVER(0x0110)	//T=u32, D=0,interface of modem fo list
#define CSID_C_FAILOVER_IF_3G_IF_LIST          		_CFG_FAILOVER(0x0120)	//T=u32, interface of 3G fo list
#define CSID_C_FAILOVER_IF_FIXED_IF_LIST        	_CFG_FAILOVER(0x0130)	//T=u32, interface of fixed fo list
#define CSID_C_FAILOVER_IF_DYNAMIC_IF_LIST      	_CFG_FAILOVER(0x0140)	//T=u32, interface of dynamic fo list
#define CSID_C_FAILOVER_IF_PPPOE_IF_LIST       		_CFG_FAILOVER(0x0150)	//T=u32, interface of pppoe fo list
#define CSID_C_FAILOVER_IF_PPTP_IF_LIST        		_CFG_FAILOVER(0x0160)	//T=u32, interface of pptp fo list
#define CSID_C_FAILOVER_IF_L2TP_IF_LIST         	_CFG_FAILOVER(0x0170)	//T=u32, interface of l2tp fo list
#define CSID_C_FAILOVER_IF_FREE_IF_LIST         	_CFG_FAILOVER(0x0180)	//T=u32, interface of l2tp fo list
#define CSID_C_FAILOVER_PVC0_VPI					_CFG_FAILOVER(0x0190)	//T=u8, 0~255
#define CSID_C_FAILOVER_PVC0_VCI					_CFG_FAILOVER(0x01a0)	//T=u16, 1~65535
#define CSID_C_FAILOVER_PVC0_ENCAPSULATION			_CFG_FAILOVER(0x01b0)	//T=u8,	0=LLC,1=VC

#define CSID_C_FAILOVER_LOADSHARING_ENABLE          _CFG_FAILOVER(0x01c0)   //T=u32,D=0,auto load sharing disable, 0:disable, 1:enable
#define CSID_C_FAILOVER_LOADSHARING_HOSTNAME        _CFG_FAILOVER(0x01c1)   //T=str,load sharing DNS Query Hostname

#define CSID_C_FAILOVER_FAIL_THRESHOLD              _CFG_FAILOVER(0x01d0)	//T=u32,D=10,wan failover retry to disconnect
#define CSID_C_FAILOVER_CHK_IDLE                    _CFG_FAILOVER(0x01d1)	//T=u32,D=5,wan check idle time
#define CSID_C_FAILOVER_CHK_INTERVAL                _CFG_FAILOVER(0x01d2)	//T=u32,D=3,wan check interval
#define CSID_C_FAILOVER_PINGCHK_LENCRY              _CFG_FAILOVER(0x01d3)	//T=u32,D=3000,wan ping check lencey
#define CSID_C_FAILOVER_PINGCHK_TIMEOUT             _CFG_FAILOVER(0x01d4)	//T=u32,D=3,wan ping check timeout

#define CSID_C_FAILOVER_SEC_PING_HOST               _CFG_FAILOVER(0x01d6)	//T=str, sec ping host for auto backup
#define CSID_C_FAILOVER_CHK_INDEX                   _CFG_FAILOVER(0x01d7)	//T=u32,D=0, check host mode, 0:None, 1:DNS1, 2:DNS2, 3:Gateway, 4:Other Host
#define CSID_C_FAILOVER_CHK_INDEX2                  _CFG_FAILOVER(0x01d8)	//T=u32,D=0, check host2 mode, 0:None, 1:DNS1, 2:DNS2, 3:Gateway, 4:Other Host

#define CSID_C_FAILOVER_CHK_BACK_ENABLE             _CFG_FAILOVER(0x01da)	//T=u32, check back enable
#define CSID_C_FAILOVER_CHK_BACK_TIME               _CFG_FAILOVER(0x01db)	//T=u32,D=5, check back time
#define CSID_C_FAILOVER_IDLE_FAIL_THRESHOLD         _CFG_FAILOVER(0x01dc)	//T=u32,D=5,seamless failover wan idle retry to disconnect
#define CSID_C_FAILOVER_IDLE_CHK_INTERVAL           _CFG_FAILOVER(0x01dd)	//T=u32,D=60,seamless failover wan idle check interval
#define CSID_C_FAILOVER_CHK_BACK_IDLE               _CFG_FAILOVER(0x01de)	//T=u32,D=30, multiwan check back idle time

/* MULTIWAN */
#define CSID_C_FAILOVER_MULTIWAN_ENABLE             _CFG_FAILOVER(0x1000)	//T=u32,D=0,auto backup enable, 0:disable, 1:enable
#define CSID_C_FAILOVER_MULTIWAN_PING_HOST          _CFG_FAILOVER(0x1010)	//T=str, ping host for auto backup
#define CSID_C_FAILOVER_MULTIWAN_CHECK_MODE         _CFG_FAILOVER(0x1020)	//T=u8,D=0,multiwanchk mode
#define CSID_C_FAILOVER_MULTIWAN_SEAMLESS_ENABLE    _CFG_FAILOVER(0x1030)    //T=u8,D=0,multiwan stay-by failover enable
#define CSID_C_FAILOVER_MULTIWAN_SEQUENCE           _CFG_FAILOVER(0x1040) //T=u8,D=0,multiwan failover sequence
#define CSID_C_FAILOVER_MULTIWAN_SEC_PING_HOST      _CFG_FAILOVER(0x1060)	//T=str, sec ping host for auto backup


#define CSID_C_FAILOVER_MULTIWAN_FAIL_THRESHOLD             _CFG_FAILOVER(0x1070)	//T=u32,D=10,multiwan failover retry to disconnect
#define CSID_C_FAILOVER_MULTIWAN_CHK_IDLE                   _CFG_FAILOVER(0x1080)	//T=u32,D=5,multiwan multiwanchk idle time
#define CSID_C_FAILOVER_MULTIWAN_CHK_INTERVAL               _CFG_FAILOVER(0x1090)	//T=u32,D=3,multiwan multiwanchk interval
#define CSID_C_FAILOVER_MULTIWAN_PINGCHK_LENCRY             _CFG_FAILOVER(0x10a0)	//T=u32,D=3000,multiwan ping check lencey
#define CSID_C_FAILOVER_MULTIWAN_PINGCHK_TIMEOUT            _CFG_FAILOVER(0x10b0)	//T=u32,D=3,multiwan ping check timeout
#define CSID_C_FAILOVER_MULTIWAN_CHK_INDEX                  _CFG_FAILOVER(0x10c0)	//T=u32,D=0, multiwan check host mode, 0:None, 1:DNS1, 2:DNS2, 3:Gateway, 4:Other Host
#define CSID_C_FAILOVER_MULTIWAN_CHK_INDEX2                 _CFG_FAILOVER(0x10d0)	//T=u32,D=0, multiwan check host2 mode, 0:None, 1:DNS1, 2:DNS2, 3:Gateway, 4:Other Host

#define CSID_C_FAILOVER_MULTIWAN_CHK_BACK_ENABLE            _CFG_FAILOVER(0x1100)	//T=u32, multiwan check back enable
#define CSID_C_FAILOVER_MULTIWAN_CHK_BACK_TIME              _CFG_FAILOVER(0x1110)	//T=u32,D=5, multiwan check back time
#define CSID_C_FAILOVER_MULTIWAN_IDLE_FAIL_THRESHOLD        _CFG_FAILOVER(0x1120)	//T=u32,D=5,seamless failover multiwan idle retry to disconnect
#define CSID_C_FAILOVER_MULTIWAN_IDLE_CHK_INTERVAL          _CFG_FAILOVER(0x1130)	//T=u32,D=60,seamless failover multiwan idle check interval
#define CSID_C_FAILOVER_MULTIWAN_CHK_BACK_IDLE              _CFG_FAILOVER(0x1140)	//T=u32,D=30, multiwan check back idle time

/* Status Items */
#define CSID_S_FAILOVER_WAN_IP                      _STA_FAILOVER(0x0002)   //T=ipv4, wan ip
#define CSID_S_FAILOVER_WAN_NM                      _STA_FAILOVER(0x0003)   //T=ipv4, wan netmask
#define CSID_S_FAILOVER_WAN_GW                      _STA_FAILOVER(0x0004)   //T=ipv4, wan gateway
#define CSID_S_FAILOVER_WAN_PRIDNS                  _STA_FAILOVER(0x0005)   //T=ipv4, wan primary dns
#define CSID_S_FAILOVER_WAN_SECDNS                  _STA_FAILOVER(0x0006)   //T=ipv4, wan secondary dns
#define CSID_S_FAILOVER_WAN_INDEX                   _STA_FAILOVER(0x000a)   //T=u32, D=99, wan index, 99:main wan, 0:multiwan0 1:multiwan1...15:multiwan15
#define CSID_S_FAILOVER_WAN_IF_NAME                 _STA_FAILOVER(0x000b)   //T=str, the interface name of wan, ppp0, eth1, ixp1
#define CSID_S_FAILOVER_CNT_TIME                    _STA_FAILOVER(0x000c)	//T=u32, connect time
#define CSID_S_FAILOVER_CHECK                       _STA_FAILOVER(0x000d)	//T=u32, failover wan check main wan back
#define CSID_S_FAILOVER_STATUS                      _STA_FAILOVER(0x0600)	//T=u32, D=0,3g failover status,0:failover down,1:failover up ; loadsharing status, 0:loadsharing disconnect, 1:loadsharing connecting, 2:loadsharing connect, 3:loadsharing disconnect
#define CSID_S_FAILOVER_CNT_STATUS                  _STA_FAILOVER(0x0700)   //T=u32, D=0,3g failover main wan connect status,1:connect,-1:disconnect,-2:physical link not connected
#define CSID_S_FAILOVER_STATUS_MODE                 _STA_FAILOVER(0x0800)	//T=u32, D=0,
#define CSID_S_FAILOVER_SEAMLESS_STATUS             _STA_FAILOVER(0x0900)   //T=u32, D=0,Seamless Status,0:seamless disconnect 1:seamless connecting 2:seamless connect 7:seamless running
#define CSID_S_FAILOVER_MULTIWAN_CHECK              _STA_FAILOVER(0x0a00)   //T=u32, D=0, failover multiwan check main wan back

/* for FAILOVER Failover ping check */
#define CSID_S_FAILOVER_ALTERED			            _STA_FAILOVER(0x2b10)   //T=u8, failover interface change altered
#define CSID_S_FAILOVER_NAT_ALTERED                 _STA_FAILOVER(0x2b11)   //T=u32, D=0,failover interface change altered

#define CSID_S_FAILOVER_LOADSHARING_LAST_WANTYPE    _STA_FAILOVER(0x2b20)   //T=u32, D=0,loadsharing last wan type
#define CSID_S_FAILOVER_LOADSHARING_RULE_ENABLE    	_STA_FAILOVER(0x2b21)   //T=u32, D=0,loadsharing rule status, enable :1, disable :0
#define CSID_S_FAILOVER_LOADSHARING_FIRST_CNT_STATUS	_STA_FAILOVER(0x2b22)   //T=u32, D=0,loadsharing first connect status, 0: disconnect, 1: connecting, 2: connect, 3: disconnect
#define CSID_S_FAILOVER_LOADSHARING_FIRST_IF_NAME	_STA_FAILOVER(0x2b24)   //T=str, ,loadsharing first wan interface name
#define CSID_S_FAILOVER_LOADSHARING_RUN_STATUS		_STA_FAILOVER(0x2b25)   //T=u32, ,loadsharing run status , 1:start, 2:end

#define CSID_S_FAILOVER_TRIGGER                    	_STA_FAILOVER(0x5001)   //T=u32, D=0, wan failover trigger 
#define CSID_S_FAILOVER_TIME                        _STA_FAILOVER(0x5002)   //T=u32, D=0, wan first failover wan start time
#define CSID_S_FAILOVER_STOP_TRIGGER                _STA_FAILOVER(0x5003)   //T=u32, D=0, failover wan stop trigger
#define CSID_S_FAILOVER_START_TRIGGER               _STA_FAILOVER(0x5004)   //T=u32, D=0, failover wan start trigger

#define CSID_S_FAILOVER_MULTIWAN_TRIGGER            _STA_FAILOVER(0x5100)   //T=u32, D=0, multiwan failover trigger
#define CSID_S_FAILOVER_MULTIWAN_TIME               _STA_FAILOVER(0x5110)   //T=u32, D=0, multiwan first failover wan start time
#define CSID_S_FAILOVER_MULTIWAN_STOP_TRIGGER       _STA_FAILOVER(0x5120)   //T=u32, D=0, multiwan failover wan stop trigger
#define CSID_S_FAILOVER_MULTIWAN_START_TRIGGER      _STA_FAILOVER(0x5130)   //T=u32, D=0, multiwan failover wan start trigger

#endif //ifndef _CSID_FAILOVER_H
