/*
 * @file Member ID Definition of Group WAN Bonding 
 *
 * Moderator: tt_lin
 * Group ID: 0x00B200/0x80B200
 */

#ifndef _CSID_WANBONDING_H
#define _CSID_WANBONDING_H

#include "csid_gid.h"

#define _CFG_WANBONDING(x)		(CSID_GIDC_WANBONDING|(x))
#define _STA_WANBONDING(x)		(CSID_GIDS_WANBONDING|(x))

/* Extra Definitions */
#define WANBONDING_STATUS_DISCONNECT     0x00
#define WANBONDING_STATUS_CONNECTING     0x01
#define WANBONDING_STATUS_CONNECT        0x02
#define WANBONDING_STATUS_DISCONNECTING  0x03

/*max wan bonding i/f, bond0~bond31*/
#define MAX_WANBONDING_SIZE  (32)
#define MAX_WANBONDING_SLAVE_SIZE (255) 

#define CSID_C_WANBONDING_ENABLE			_CFG_WANBONDING(0x0001)		//T=u32,D=0,WAN Bonding enable, 0:disable, 1:enable
#define CSID_C_WANBONDING_LOCAL_MAC			_CFG_WANBONDING(0x0002)		//T=mac,D=1,WAN Bonding Local WAN MAC
#define CSID_C_WANBONDING_MAX_SLAVE			_CFG_WANBONDING(0x0003)		//T=u32, D=2, max slaves each bonding i/f
#define CSID_C_WANBONDING_ENCRY_MODE		_CFG_WANBONDING(0x0004)		//T=u32, D=1,bonding local encry-mode fo list, 0:None, 1: AES-128
#define CSID_C_WANBONDING_ENCRY_KEY			_CFG_WANBONDING(0x0005)	    //T=str, D=0,bonding local encry-key fo list, active when encry-mode=AES128.

// WAN Bonding status
#define CSID_S_WANBONDING_STATUS_LIST			_STA_WANBONDING(0x0010)	//T=u8, ui status, DISCONNECT=0x00, CONNECTING=0x01, CONNECT=0x02, DISCONNECTING=0x03

//WAN Bonding altered
#define CSID_S_WANBONDING_ALTERED_LIST			_STA_WANBONDING(0x0030)	//T=u8, ui altered

/* Wan Bonding interface, the max I/F is 32(0x20)*/ //(master)
#define CSID_C_WANBONDING_IF_ID_LIST			_CFG_WANBONDING(0x0070)	//T=u32, D=0,(L)index of Bonding I/F fo list
#define CSID_C_WANBONDING_IF_NAME_LIST			_CFG_WANBONDING(0x0090)	//T=str, D=0,(L)bonding name fo list
#define CSID_C_WANBONDING_IF_REMOTE_LAN_IP_LIST	_CFG_WANBONDING(0x00B0)	//T=ipv4,   ,(R)bonding remote LAN IP fo list  (fw eternal usage only)
#define CSID_C_WANBONDING_IF_OPMODE_LIST		_CFG_WANBONDING(0x00D0)	//T=u32, D=0,(R)binding op-mode fo list, 0: balance-rr, 1:active-backup, 2:blanace-xor, 3:broadcast (fw eternal usage only, be fixed to 0)																																		
#define CSID_C_WANBONDING_IF_ENCRY_MODE_LIST	_CFG_WANBONDING(0x00F0)	//T=u32, D=1,(L)bonding remote encry-mode fo list, 0:None, 1: AES-128
#define CSID_C_WANBONDING_IF_ENCRY_KEY_LIST		_CFG_WANBONDING(0x0110)	//T=str, D=0,(L)bonding remote encry-key fo list, active when encry-mode=AES128.
#define CSID_C_WANBONDING_IF_KEEP_ALIVE_LIST	_CFG_WANBONDING(0x0130)	//T=u32, D=1,(L)bonding keep-alive fo list, 0: disable, 1: enable
#define CSID_C_WANBONDING_IF_REMOTE_MAC_LIST	_CFG_WANBONDING(0x0150)	//T=mac, D=1,(L)remote first MAC of WAN port  fo list 
#define CSID_C_WANBONDING_IF_IP_LIST			_CFG_WANBONDING(0x0170)	//T=ipv4,   ,(L)IP Address for LOCAL Bonding I/F fo list  (fw internal usage only)
#define CSID_C_WANBONDING_IF_ENABLE_LIST		_CFG_WANBONDING(0x0190)	//T=u32, D=1,(L)bonding enble-rule fo list, 0: disable, 1: enable 
/* for Pre-Query, JOIN into bondx, or not. NOW, define WAN1, WAN2, WAN3 only*/
#define CSID_C_WANBONDING_IF_PQ1_LOCALWAN_LIST	_CFG_WANBONDING(0x01B0)	//T=u32,    ,(L)local wan interface for Pre-Query wan 1 fo list
#define CSID_C_WANBONDING_IF_PQ1_REMOTEGW_LIST	_CFG_WANBONDING(0x01D0)	//T=ipv4,   ,(L)the remote gateway for Pre-Query wan 1 fo list
#define CSID_C_WANBONDING_IF_PQ1_ENABLE_LIST	_CFG_WANBONDING(0x01F0)	//T=u32, D=1,(L)Pre-Query wan 1 ENABLE fo list, 0: disable, 1: enable 
#define CSID_C_WANBONDING_IF_PQ2_LOCALWAN_LIST	_CFG_WANBONDING(0x0210)	//T=u32,    ,(L)local wan interface for Pre-Query wan 2 fo list
#define CSID_C_WANBONDING_IF_PQ2_REMOTEGW_LIST	_CFG_WANBONDING(0x0230)	//T=ipv4,   ,(L)the remote gateway for Pre-Query wan 2 fo list
#define CSID_C_WANBONDING_IF_PQ2_ENABLE_LIST	_CFG_WANBONDING(0x0250)	//T=u32, D=1,(L)Pre-Query wan 2 ENABLE for list, 0: disable, 1: enable 
#define CSID_C_WANBONDING_IF_PQ3_LOCALWAN_LIST	_CFG_WANBONDING(0x0270)	//T=u32,    ,(L)local wan interface for Pre-Query wan 3 for list
#define CSID_C_WANBONDING_IF_PQ3_REMOTEGW_LIST	_CFG_WANBONDING(0x0290)	//T=ipv4,   ,(L)the remote gateway for Pre-Query wan 3 for list
#define CSID_C_WANBONDING_IF_PQ3_ENABLE_LIST	_CFG_WANBONDING(0x02B0)	//T=u32, D=1,(L)Pre-Query wan 3 ENABLE fo list, 0: disable, 1: enable 
#define CSID_S_WANBONDING_IF_PQ_INDEX_LIST		_STA_WANBONDING(0x0050)	//T=u32,    ,(L)query index for Pre-Query fo list

/* Bonding-slave list(pool type, pool size: 0xFF) */ //(slave)
#define CSID_S_WANBONDING_SLAVE_IF_ID_LIST			_STA_WANBONDING(0x0110)	//T=u32, D=0, (L)Slave ID fo list
#define CSID_S_WANBONDING_SLAVE_IF_REFID_LIST		_STA_WANBONDING(0x0210)	//T=u32, D=FF,(L)Ref to which bonding-ID fo list, this slave is refed by which bonding I/F. FF: without any reference.(index_b)
#define CSID_S_WANBONDING_SLAVE_IF_LOCALWAN_LIST	_STA_WANBONDING(0x0310)	//T=u32, D=FF,(L)slave interface of the local router fo list(local wan id, wan1=>0, wan2=>1,...)(index_l)
#define CSID_S_WANBONDING_SLAVE_IF_WEIGHT_LIST		_STA_WANBONDING(0x0410)	//T=u32, D=0(auto),(L) the percentage of weight. (-1)=> disable
#define CSID_S_WANBONDING_SLAVE_IF_REMOTEGW_LIST	_STA_WANBONDING(0x0510)	//T=ipv4,    ,(R)the remote gateway for this slave interface fo list
#define CSID_S_WANBONDING_SLAVE_IF_REMOTEWAN_LIST	_STA_WANBONDING(0x0610)	//T=u32, D=FF,(R)slave interface of the remote router fo list(remote wan id, wan1=>0, wan2=>1,...)(index_r)
#define CSID_S_WANBONDING_SLAVE_IF_ENABLE_LIST		_STA_WANBONDING(0x0710)	//T=u32, D=1 ,(L)bonding slave enble-rule fo list, 0: disable, 1: enable

#endif //_CSID_WANBONDING_H

