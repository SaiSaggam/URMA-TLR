/*
 * CSID Definition of Group GRE.
 *
 * Moderator: Chad Sheu
 * Group ID: 0x00710000/0x80710000
 */

#ifndef _CSID_GRE_H
#define _CSID_GRE_H

#include "csid_gid.h"

#define _CFG_GRE(x)		(CSID_GIDC_GRE|(x))
#define _STA_GRE(x)		(CSID_GIDS_GRE|(x))

/* Extra Definitions */
#if  defined(ENABLE_WANBONDING)
#define MAX_GRE_TUNNEL				255
#else
#define MAX_GRE_TUNNEL				32
#endif

#define MAX_GRE_PEER_SUBNET          25
#define MAX_TNAME_SIZE	 		   	16
#define MAX_WAN_IF_SIZE	 		   	16
#define MAX_CMD_SIZE               256

/* Configuration Items */
#define CSID_C_GRE_TNAME         _CFG_GRE(0x0100)	//T=str,  GRE tunnel name
#define CSID_C_GRE_ENABLE        _CFG_GRE(0x0200)	//T=u8,   enable/disable GRE tunnel
#define CSID_C_GRE_DT            _CFG_GRE(0x0300)	//T=u8,   default tunnel
#define CSID_C_GRE_PEER_IP       _CFG_GRE(0x0400)	//T=ipv4, the IP address of remote router
#define CSID_C_GRE_KEY           _CFG_GRE(0x0500)	//T=str,  pre-shared key for the tunnel
#define CSID_C_GRE_TTL           _CFG_GRE(0x0600)	//T=u16,  time to live 
#define CSID_C_GRE_PEER_SUBNET   _CFG_GRE(0x0700)	//T=ipv4, the domain of the remote LAN 
#define CSID_C_GRE_PEER_MASK     _CFG_GRE(0x0800)	//T=u8,   the submask of the remote LAN 
#define CSID_C_GRE_LOCAL_IP      _CFG_GRE(0x0900)	//T=ipv4, the IP address of local router
#define CSID_C_GRE_NETBIOS       _CFG_GRE(0x0A00)   //T=u32,  Gre over netbios enable/disable
#define CSID_C_GRE_TOTAL_ENABLE  _CFG_GRE(0x0B00)   //T=u32,  Gre total enable/disable
#define CSID_C_GRE_WAN           _CFG_GRE(0x0C00)	//T=u32,  D=0, Gre WAN, 0:WAN1,1:WAN2,2:WAN3,30:br0/LAN,31:VLAN1,32:VLAN2,33:VLAN3
#define CSID_C_GRE_3GFO          _CFG_GRE(0x0D00)       //T=u8,   enable/disable enable GRE tunnel when wantype is 3G
#define CSID_C_GRE_MAX_TUNNEL    _CFG_GRE(0x0E00)	//T=u32,  D=32, Maximum numbers of GRE tunnel
#define CSID_C_GRE_TUN_FAILOVER	 _CFG_GRE(0x0F00)   //T=u32, D=0,Operation mode,0:Always on,1:Failover,2:Load Balance
#define CSID_C_GRE_TUN_FAILOVER_IDX  _CFG_GRE(0x1100)  // T=u32, D=0,the tunnel idx use this failover tunnel
#define CSID_C_GRE_TUN_TYPE_FAILOVER  _CFG_GRE(0x1200)  //T=u32,D=0,the major tunnel type which use this failover tunnel, 0:IPsec,1:PPTP,2:L2TP,3:GRE
#define CSID_C_GRE_KEEP_ALIVE_ENABLE  _CFG_GRE(0x1300)  //T=u32,D=0,0:Disable,1:Enable
#define CSID_C_GRE_DMVPN_SPOKE_ENABLE  _CFG_GRE(0x1400) //T=u32,D=0,0:Disable,1:Enable
#define CSID_C_GRE_DMVPN_PRE_KEY  _CFG_GRE(0x1500)  //T=str, DMVPN pre-shared key
#define CSID_C_GRE_KEEP_ALIVE_IP    _CFG_GRE(0x1600) //T=str, GRE keep alive IP
#define CSID_C_GRE_KEEP_ALIVE_TIME  _CFG_GRE(0x1700) //T=u32, D=0, GRE keep alive time
#define CSID_C_GRE_DMVPN_CMD           _CFG_GRE(0x1800) //T=u32, D=0, 0:Transport mode, 1:Tunnel mode
#define CSID_C_GRE_DMVPN_NAT_TRAVERSAL _CFG_GRE(0x1900) //T=u32, D=0, 0:Enable, 1:Disable
#define CSID_C_GRE_DEFAULT_GW_EN       _CFG_GRE(0x2000) //T=u32, D=0, 0:Disable, 1:Enable, Enable this tunnel as default gateway

/* Status Items */ 
#define CSID_S_GRE_ALTERED		 _STA_GRE(0x0001)	//T=u8,  D=0, UI altered
#define CSID_S_GRE_TX_PKTS		 _STA_GRE(0x0100)	//T=u32, D=0, Transmitted Packets
#define CSID_S_GRE_TX_BYTES		 _STA_GRE(0x0200)	//T=u32, D=0, Transmitted Bytes 
#define CSID_S_GRE_RX_PKTS		 _STA_GRE(0x0300)	//T=u32, D=0, Reveived Packets
#define CSID_S_GRE_RX_BYTES		 _STA_GRE(0x0400)	//T=u32, D=0, Reveices Bytes
#define CSID_S_GRE_STATUS		 _STA_GRE(0x0500)	//T=u32, D=0, 0:not live,1:live 
#define CSID_S_GRE_ACTION		 _STA_GRE(0x0600)	//T=u32, D=0, 0:no action,1:action
#define CSID_S_GRE_TOTAL_PEER_SUBNET_LIST _STA_GRE(0x0700)      //T=str, Total GRE peer subnet list
#define CSID_S_GRE_TUN_ALTERED            _STA_GRE(0x0800)       //T=u32, D=0, 0:tunnel altered

#endif 

