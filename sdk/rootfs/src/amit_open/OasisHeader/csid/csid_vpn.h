/*
 * @file Member ID Definition of Group VPN.
 *
 * Moderator: Tim_Hung
 * Group ID: 0x00700000/0x80700000
 */

#ifndef _CSID_VPN_H
#define _CSID_VPN_H

#include "csid_gid.h"

#define _CFG_VPN(x)       (CSID_GIDC_VPN|(x))
#define _STA_VPN(x)       (CSID_GIDS_VPN|(x))

/* Extra Definitions */

//Enable/Disable Switch
#define	IPSEC_DISABLE		0
#define IPSEC_Embedded		1
#define IPSEC_PASSTHROUGH	2

//Tunnel Command
#define IPSEC_CMD_NOOP			0
#define IPSEC_CMD_CONNECT			1
#define IPSEC_CMD_DISCONNECT		2

//Key Exchange Method
#define	IPSEC_METHOD_IKE		0
#define IPSEC_METHOD_MK			1 //Manual Keying
#define IPSEC_METHOD_X509		2 //X.509

//Encapsulation Protocol
#define IPSEC_AH_TUN				0 //AH Tunnel
#define IPSEC_ESP_TUN				1 //ESP Tunnel
#define	IPSEC_ESP_AH_TUN			2 //ESP+AH Tunnel

//PFS Group
#define IPSEC_PFS_DISABLE			0
#define IPSEC_PFS_GROUP1			1
#define IPSEC_PFS_GROUP2			2
#define IPSEC_PFS_GROUP5			3	
//#define IPSEC_PFS_AS_PHASE1		4 //Same as Phase1
#define IPSEC_PFS_GROUP14			4	
#define IPSEC_PFS_GROUP15			5	
#define IPSEC_PFS_GROUP16			6	
#define IPSEC_PFS_GROUP17			7	
#define IPSEC_PFS_GROUP18			8	
	
//ID
#define IPSEC_ID_USERNAME			0
#define IPSEC_ID_FQDN				1
#define IPSEC_ID_USER_FQDN			2 //USER@FQDN
#define IPSEC_ID_KEY_ID_ASCII		3
#define IPSEC_ID_KEY_ID_HEX			4

//Encryption Algorithm
#define IPSEC_ENC_DES			1
#define IPSEC_ENC_3DES			2
#define IPSEC_ENC_AES128		3
#define IPSEC_ENC_AES192		4
#define IPSEC_ENC_AES256		5
#define IPSEC_ENC_NULL			6
#define IPSEC_ENC_AES			7
#define IPSEC_ENC_BLOWFISH128	8
#define IPSEC_ENC_TWOFISH128	9
#define IPSEC_ENC_CAST128		10

//Authenticate Algorithm
#define IPSEC_AUTH_ANY			0 //Only use in phase 1 proposal
#define IPSEC_AUTH_MD5			1
#define IPSEC_AUTH_SHA1			2
#define IPSEC_AUTH_NONE			3 //use for ESP without AH
#define IPSEC_AUTH_SHA2_256		4
#define IPSEC_AUTH_SHA2_512		5

//Phase 1 DHGroup
#define IPSEC_P1_GROUP_ANY		0 //Only use in phase 1 proposal
#define IPSEC_P1_GROUP1			1
#define IPSEC_P1_GROUP2			2
#define IPSEC_P1_GROUP5			3
#define IPSEC_P1_GROUP14		4
#define IPSEC_P1_GROUP15		5
#define IPSEC_P1_GROUP16		6
#define IPSEC_P1_GROUP17		7
#define IPSEC_P1_GROUP18		8

//XAUTH
#define IPSEC_XAUTH_DISABLE		0
#define IPSEC_XAUTH_SERV			1
#define IPSEC_XAUTH_CLI			2

// Connecting type
#define IPSEC_IKE_AUTO			0
#define IPSEC_IKE_MANUAL		1
#define IPSEC_IKE_ONDEMAND		2

// AUTHBY
#define IPSEC_AUTHBY_NEVER		0
#define IPSEC_AUTHBY_SECRET		1
#define IPSEC_AUTHBY_RSASIG		2
#define IPSEC_AUTHBY_SEC_RSA	3

//Tunnel Status
#define IPSEC_S_CONNECTED		1
#define IPSEC_S_WAIT			2 //Wait for traffic
#define IPSEC_S_DISCONNECT		3
#define IPSEC_S_ERROR			8
#define IPSEC_S_CONNECTING		9

#define VPN_RULE_CSID_GAP             0x10 

/* Configuration Items */
//0x0001~0x03FF

#define CSID_C_VPN_PASSTHROUGH                         _CFG_VPN(0x0001)       //T=u32, D=1, vpn passthrough
#define CSID_C_VPN_ENABLE                               _CFG_VPN(0x000F) //T=u32, D=0, 0:Disable, 1:Enable
#define CSID_C_IPSEC_TYPE								_CFG_VPN(0x0010) //T=u32, D=0,Ipsec Type
#define CSID_C_IPSEC_EMBEDDED_EN						_CFG_VPN(0x0020) //T=u32, D=1,Embedded ,0:Disable,1:Enable
#define CSID_C_IPSEC_PASSTHROUGH_EN						_CFG_VPN(0x0030) //T=u32, D=0,Passthrough,0:Disable,1:Enable
#define CSID_C_IPSEC_MAX_TUNNELS						_CFG_VPN(0x0040) //T=u32, D=32,Max number of Tunnel
#define CSID_C_IPSEC_NETBIOS							_CFG_VPN(0x0050) //T=u32, D=0,Netbios over ipsec,0:Disable,1:Enable
#define CSID_C_IPSEC_SSDP								_CFG_VPN(0x0060) //T=u32, D=0,SSDP over ipsec,0:Disable,1:Enable
#define CSID_C_IPSEC_NAT_TRAVERSAL                      _CFG_VPN(0x0070) //T=u32, D=0,NAT Traversal ,0:Disable,1:Enable
#define CSID_C_IPSEC_STATISTIC                          _CFG_VPN(0x0080) //T=u32, D=0,VPN Traffic Statistic ,0:Disable,1:Enable
#define CSID_C_IPSEC_CUSTOMIZE_FLAG                     _CFG_VPN(0x0090) //T=u32, D=0, vpn default setting
/*Basic Setting */

//Tunnel Setting 0x0800~0x0FFF
#define CSID_C_IPSEC_TUN_NAME							_CFG_VPN(0x0100) //T=str, IKE Tunnel name
#define CSID_C_IPSEC_TUN_EN								_CFG_VPN(0x0200) //T=u32, D=0,IKE Tunnel ,0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_KEY_METHOD						_CFG_VPN(0x0300) //T=u32, D=0,IKE KEY Method,0:IKE,1:Manual 

#define CSID_C_IPSEC_TUN_IKE_L_SUBNET					_CFG_VPN(0x0400) //T=ipv4,IKE Local Subnet
#define CSID_C_IPSEC_TUN_IKE_L_MASK						_CFG_VPN(0x0500) //T=ipv4,IKE Local Netmask
#define CSID_C_IPSEC_TUN_IKE_R_SUBNET					_CFG_VPN(0x0600) //T=ipv4,IKE Remote Subnet
#define CSID_C_IPSEC_TUN_IKE_R_MASK						_CFG_VPN(0x0700) //T=ipv4,IKE Remote Netmask
#define CSID_C_IPSEC_TUN_IKE_R_GATEWAY					_CFG_VPN(0x0800) //T=str,IKE Remote Gateway, support FQDN

#define CSID_C_IPSEC_TUN_MK_L_SUBNET                    _CFG_VPN(0x0900) //T=ipv4,MANUAL Local Subnet
#define CSID_C_IPSEC_TUN_MK_L_MASK                      _CFG_VPN(0x0A00) //T=ipv4,MANUAL Local Netmask
#define CSID_C_IPSEC_TUN_MK_R_SUBNET                    _CFG_VPN(0x0B00) //T=ipv4,MANUAL Remote Subnet
#define CSID_C_IPSEC_TUN_MK_R_MASK                      _CFG_VPN(0x0C00) //T=ipv4,MANUAL Remote Netmask
#define CSID_C_IPSEC_TUN_MK_R_GATEWAY                   _CFG_VPN(0x0D00) //T=str,MANUAL Remote Gateway, support FQDN
#define CSID_C_IPSEC_TUN_STATE                          _CFG_VPN(0x0E00) //T=u32, D=0,IKE Tunnel state,1:connected,2:wait for traffic, 3:unconnected,9:connecting  
#define CSID_C_IPSEC_TUN_ID								_CFG_VPN(0x0F00) //T=u32, D=0,IKE Tunnel number
#define CSID_C_IPSEC_TUN_WAN							_CFG_VPN(0x1000) //T=u32, IKE WAN, 0:WAN1,1:WAN2,2:wan3,30:br0/LAN,31:VLAN1,32:VLAN2,33:VLAN3

//XAUTH Server 0x0400~0x07FF
#define CSID_C_IPSEC_XAUTH_SRV_GROUP                    _CFG_VPN(0x1100) //T=u32, D=0,Xauth server datebase group,0: Group1,1:Group2,2:Group3,3:Group4
#define CSID_C_IPSEC_XAUTH_SRV_USER                     _CFG_VPN(0x2000) //T=str, D=0,Xauth server user name
#define CSID_C_IPSEC_XAUTH_SRV_PASS                     _CFG_VPN(0x3000) //T=str, D=0,Xauth server password
#define CSID_C_IPSEC_REKEY			        _CFG_VPN(0x3100) //T=u32, D=0,Rekey function,0:Enable,1:Disable

//Manual Keying Setting 0x1000~0x17FFF
#define CSID_C_IPSEC_TUN_MK_CMD                         _CFG_VPN(0x3200) //T=u32, D=0,MANUAL Tunnel Mode,0:Tunnel mode,1:Transport mode
#define CSID_C_IPSEC_TUN_MK_INBOUND_SPI 				_CFG_VPN(0x4100) //T=str, D=0,Inbound spi value
#define CSID_C_IPSEC_TUN_MK_OUTBOUND_SPI				_CFG_VPN(0x4200) //T=str, D=0,Outbound spi value
#define CSID_C_IPSEC_TUN_MK_ENC_ALG						_CFG_VPN(0x4300) //T=u32, D=0,Encryption Algorithm,0:none; 1:DES; 2:3DES; 3:AES-128; 4:AES-192; 5:AES-256
#define CSID_C_IPSEC_TUN_MK_ENC_KEY1					_CFG_VPN(0x4400) //T=str, D=0,Encryption key1 value
#define CSID_C_IPSEC_TUN_MK_ENC_KEY2					_CFG_VPN(0x4500) //T=str, D=0,Encryption key2 value
#define CSID_C_IPSEC_TUN_MK_ENC_KEY3					_CFG_VPN(0x4600) //T=str, D=0,Encryption key3 value
#define CSID_C_IPSEC_TUN_MK_AUTH_ALG					_CFG_VPN(0x4700) //T=u32, D=0,Authenticate Algorithm,0:none; 1:MD5; 2:SHA1
#define CSID_C_IPSEC_TUN_MK_AUTH_KEY					_CFG_VPN(0x4800) //T=str, D=0,Authenticate key value
#define CSID_C_IPSEC_TUN_MK_ENCAP						_CFG_VPN(0x4900) //T=u32, D=5,Encapsulation Protocol,1:AH_TUNNEL,2:AH_TRANSPORT,3:ESP_TUNNEL																				,4:ESP_TRANSPORT,5:ESP_TUNNEL_WITH_AUTH,6:ESP_TRANSPORT_WITH_AUTH;
//IKE  0x1800~0x1FFF
#define CSID_C_IPSEC_TUN_IKE_CMD						_CFG_VPN(0x4A00) //T=u32, D=0,IKE Tunnel Mode,0:Tunnel mode,1:Transport mode
#define CSID_C_IPSEC_TUN_IKE_CNT_TYPE					_CFG_VPN(0x4B00) //T=u32, D=2, connecting type
#define CSID_C_IPSEC_TUN_IKE_AUTHBY						_CFG_VPN(0x4C00) //T=u32, D=1, authenticate method
#define CSID_C_IPSEC_TUN_IKE_DPD_EN						_CFG_VPN(0x4D00) //T=u32, D=0, DPD enable
#define CSID_C_IPSEC_TUN_IKE_DPD_INTERVAL               _CFG_VPN(0x4E00) //T=u32, D=30, DPD interval
#define CSID_C_IPSEC_TUN_IKE_DPD_DTIME					_CFG_VPN(0x4F00) //T=u32, D=30, DPD delay time
#define CSID_C_IPSEC_TUN_IKE_DPD_TIME					_CFG_VPN(0x5000) //T=u32, D=180, DPD timeout time
#define CSID_C_IPSEC_TUN_IKE_ENCAP						_CFG_VPN(0x5100) //T=u32, D=1,Encapsulation Protocol,0:AH,1:ESP,2:AH+ESP
#define CSID_C_IPSEC_TUN_IKE_AGGR						_CFG_VPN(0x5200) //T=u32, D=0,Aggressive Mode,0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_PRESHARE_KEY				_CFG_VPN(0x5300) //T=str, D=0,preshare key
#define CSID_C_IPSEC_TUN_IKE_PFS_GROUP					_CFG_VPN(0x5400) //T=u32, D=0,PFS Group,0:disable,1:Group 1,2:Group 2,3:Group 5,4:Group 14,5:Group 15,6:Group 16,7:Group 17,8:Group 18
#define CSID_C_IPSEC_TUN_IKE_R_ID_VALUE					_CFG_VPN(0x5500) //T=str, D=0,Remote ID value
#define CSID_C_IPSEC_TUN_IKE_R_ID_TYPE					_CFG_VPN(0x5600) //T=u32, D=0,Remote ID type,0:Username,1:FQDN,2:UserFQDN,3:Key_ID_ASCII,4:Key_ID_HEX
#define CSID_C_IPSEC_TUN_IKE_L_ID_TYPE					_CFG_VPN(0x5700) //T=u32, D=0,Local ID type,0:Username,1:FQDN,2:UserFQDN,3:Key_ID_ASCII,4:Key_ID_HEX
#define CSID_C_IPSEC_TUN_IKE_L_ID_VALUE					_CFG_VPN(0x5800) //T=str, D=0,Local ID value
#define CSID_C_IPSEC_TUN_IKE_KEEP_ALIVE_EN				_CFG_VPN(0x5900) //T=u32, D=0,Keep Alive En,0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_KEEP_ALIVE_IP				_CFG_VPN(0x5A00) //T=str, D=0,Keep Alive IP
#define CSID_C_IPSEC_TUN_IKE_KEEP_ALIVE_TIME			_CFG_VPN(0x5B00) //T=u32, D=0,Keep Alive Time
#define CSID_C_IPSEC_TUN_IKE_XAUTH						_CFG_VPN(0x5C00) //T=u32, D=0,IKE Xauth,0:XAUTH_NONEN,1:XAUTH_SERVER,2:XAUTH_CLIENT
#define CSID_C_IPSEC_TUN_IKE_XAUTH_C_USER				_CFG_VPN(0x5D00) //T=str, D=0,Xauth Client user name
#define CSID_C_IPSEC_TUN_IKE_XAUTH_C_PASS				_CFG_VPN(0x5E00) //T=str, D=0,Xauth Client password

//IKE Phase1 Proposa1 0x2000~0x3FFF
#define CSID_C_IPSEC_TUN_IKE_P1_KEYLIFE_SEC				_CFG_VPN(0x6000) //T=str, D=0,Phase1 key life Time(Sec),0:default
#define CSID_C_IPSEC_TUN_IKE_P1_KEYLIFE_BYTE			_CFG_VPN(0x6100) //T=str, D=0,Phase1 key life Time(Byte),0:default
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_EN					_CFG_VPN(0x6200) //T=u32, D=0,Phase1 Proposal,0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_1_EN				_CFG_VPN(0x6300) //T=u32, D=0,Proposal(1),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_1_ENC				_CFG_VPN(0x6400) //T=u32, D=0,Proposal(1) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,7:AES,8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_1_AUTH				_CFG_VPN(0x6500) //T=u32, D=0,Proposal(1) Authentication,0:none,1:MD5, 2:SHA1,4:SHA2-256,5:SHA2-512
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_1_GROUP			_CFG_VPN(0x6600) //T=u32, D=0,Proposal(1) Group,0:none,1:Group1,2:Group2,3:Group5,4:Group14,5:Group15,6:Group16,7:Group17,8:Group18
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_2_EN				_CFG_VPN(0x6700) //T=u32, D=0,Proposal(2),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_2_ENC				_CFG_VPN(0x6800) //T=u32, D=0,Proposal(2) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,7:AES.8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_2_AUTH				_CFG_VPN(0x6900) //T=u32, D=0,Proposal(2) Authentication,0:none,1:MD5, 2:SHA1,4:SHA2-256,5:SHA2-512
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_2_GROUP			_CFG_VPN(0x6A00) //T=u32, D=0,Proposal(2) Group,0:none,1:Group1,2:Group2,3:Group5,4:Group14,5:Group15,6:Group16,7:Group17,8:Group18
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_3_EN               _CFG_VPN(0x6B00) //T=u32, D=0,Proposal(3),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_3_ENC              _CFG_VPN(0x6C00) //T=u32, D=0,Proposal(3) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,7:AES.8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_3_AUTH             _CFG_VPN(0x6D00) //T=u32, D=0,Proposal(3) Authentication,0:none,1:MD5, 2:SHA1,4:SHA2-256,5:SHA2-512
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_3_GROUP            _CFG_VPN(0x6E00) //T=u32, D=0,Proposal(3) Group,0:none,1:Group1,2:Group2,3:Group5,4:Group14,5:Group15,6:Group16,7:Group17,8:Group18
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_4_EN               _CFG_VPN(0x6F00) //T=u32, D=0,Proposal(4),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_4_ENC              _CFG_VPN(0x7000) //T=u32, D=0,Proposal(4) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,7:AES.8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_4_AUTH             _CFG_VPN(0x7100) //T=u32, D=0,Proposal(4) Authentication,0:none,1:MD5, 2:SHA1,4:SHA2-256,5:SHA2-512
#define CSID_C_IPSEC_TUN_IKE_P1_PROP_4_GROUP            _CFG_VPN(0x7200) //T=u32, D=0,Proposal(4) Group,0:none,1:Group1,2:Group2,3:Group5,4:Group14,5:Group15,6:Group16,7:Group17,8:Group18

//IKE Phase2 Proposal 0x4000~0x5FFF
#define CSID_C_IPSEC_TUN_IKE_P2_KEYLIFE_SEC				_CFG_VPN(0x7300) //T=str, D=0,Phase2 key life Time(Sec),0:default
#define CSID_C_IPSEC_TUN_IKE_P2_KEYLIFE_BYTE			_CFG_VPN(0x7400) //T=str, D=0,Phase2 key life Time(Byte),0:default
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_EN					_CFG_VPN(0x7500) //T=u32, D=0,Phase2 Proposal,0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_1_EN				_CFG_VPN(0x7600) //T=u32, D=0,Proposal(1),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_1_ENC				_CFG_VPN(0x7700) //T=u32, D=0,Proposal(1) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,6:Null,7:AES,8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_1_AUTH				_CFG_VPN(0x7800) //T=u32, D=0,Proposal(1) Authentication,0:none,1:MD5, 2:SHA1,3:None,4:SHA2-256,5:SHA2-512
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_2_EN				_CFG_VPN(0x7900) //T=u32, D=0,Proposal(2),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_2_ENC				_CFG_VPN(0x7A00) //T=u32, D=0,Proposa(2) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,6:Null,7:AES,8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_2_AUTH				_CFG_VPN(0x7B00) //T=u32, D=0,Proposa(2) Authentication,0:none,1:MD5, 2:SHA1,3:None,4:SHA2-256,5:SHA2-512
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_3_EN               _CFG_VPN(0x7C00) //T=u32, D=0,Proposal(3),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_3_ENC              _CFG_VPN(0x7D00) //T=u32, D=0,Proposa(3) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,6:Null,7:AES,8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_3_AUTH             _CFG_VPN(0x7E00) //T=u32, D=0,Proposa(3) Authentication,0:none,1:MD5, 2:SHA1,3:None,4:SHA2-256,5:SHA2-512
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_4_EN               _CFG_VPN(0x7F00) //T=u32, D=0,Proposal(4),0:Disable,1:Enable
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_4_ENC              _CFG_VPN(0x8000) //T=u32, D=0,Proposa(4) Encryption,0:none,1:DES,2:3DES,3:AES-128,4:AES-192,5:AES-256,6:Null,7:AES,8:BLOWFISH128,9:TWOFISH128,10,CAST128
#define CSID_C_IPSEC_TUN_IKE_P2_PROP_4_AUTH				_CFG_VPN(0x8100) //T=u32, D=0,Proposa(4) Authentication,0:none,1:MD5,2:SHA1,3:None,4:SHA2-256,5:SHA2-512

//iPV6 IPSEC
#define CSID_C_IPSEC_V6_TUN_TYPE                        _CFG_VPN(0x8200) //T=u32, D=0,0:4in4,1:4in6,2:6in4,3:6in6,VPN IPv6 Tunnel Type
#define CSID_C_IPSEC_V6_TUN_IKE_L_IP                    _CFG_VPN(0x8300) //T=str, IKE Local IP
#define CSID_C_IPSEC_V6_TUN_IKE_L_SUBNET                _CFG_VPN(0x8400) //T=str, IKE Local SUBNET
#define CSID_C_IPSEC_V6_TUN_IKE_L_NEXTHOP               _CFG_VPN(0x8500) //T=str, IKE Local NEXTHOP
#define CSID_C_IPSEC_V6_TUN_IKE_R_IP                    _CFG_VPN(0x8600) //T=str, IKE Remote IP
#define CSID_C_IPSEC_V6_TUN_IKE_R_SUBNET                _CFG_VPN(0x8700) //T=str, IKE Remote SUBNET

#define CSID_C_IPSEC_V6_TUN_MK_L_IP                     _CFG_VPN(0x8800) //T=str, MANUAL Local IP
#define CSID_C_IPSEC_V6_TUN_MK_L_SUBNET                 _CFG_VPN(0x8900) //T=str, MANUAL Local SUBNET
#define CSID_C_IPSEC_V6_TUN_MK_L_NEXTHOP                _CFG_VPN(0x8A00) //T=str, MANUAL Local NEXTHOP
#define CSID_C_IPSEC_V6_TUN_MK_R_IP                     _CFG_VPN(0x8B00) //T=str, MANUAL Remote IP
#define CSID_C_IPSEC_V6_TUN_MK_R_SUBNET                 _CFG_VPN(0x8C00) //T=str, MANUAL Remote SUBNET

#define CSID_C_IPSEC_V6_TUN_EN                          _CFG_VPN(0x8D00) //T=u32, D=0,IKE Tunnel ,0:Disable,1:Enable
#define CSID_C_IPSEC_V6_TUN_NAME                        _CFG_VPN(0x8E00) //T=str, IKE Tunnel name
#define CSID_C_IPSEC_V6_TUN_ID                          _CFG_VPN(0x8F00) //T=u32, D=0,IKE Tunnel number


//GRE over IPSEC item
#define CSID_C_GRE_OVER_IPSEC_TNAME					    _CFG_VPN(0x9000) //T=str,  GRE Over IPSEC tunnel name
#define CSID_C_GRE_OVER_IPSEC_ENABLE                    _CFG_VPN(0x9100) //T=u32,   enable/disable GRE Over IPSEC tunnel
#define CSID_C_GRE_OVER_IPSEC_PEER_IP                   _CFG_VPN(0x9200) //T=ipv4, the IP address of remote router
#define CSID_C_GRE_OVER_IPSEC_PEER_SUBNET               _CFG_VPN(0x9300) //T=ipv4, the domain of the remote LAN
#define CSID_C_GRE_OVER_IPSEC_PEER_MASK                 _CFG_VPN(0x9400) //T=u32,   the submask of the remote LAN
#define CSID_C_GRE_OVER_IPSEC_LOCAL_IP					_CFG_VPN(0x9500) //T=ipv4, the IP address of local router
#define CSID_C_GRE_OVER_IPSEC_LOCAL_MASK				_CFG_VPN(0x9600) //T=u32, the IP address of local router
#define CSID_C_GRE_OVER_IPSEC_KEY						_CFG_VPN(0x9700) //T=str,  pre-shared key for the tunnel
#define CSID_C_GRE_OVER_IPSEC_TTL						_CFG_VPN(0x9800) //T=u32,  time to live
#define CSID_C_L2TP_OVER_IPSEC_ENABLE					_CFG_VPN(0x9900) //T=u32,  L2TP over IPSEC
#define CSID_C_IPSEC_TUN_UI_IKE_R_GATEWAY						_CFG_VPN(0x9A00) //T=str, for UI display FQDN 
#define CSID_C_IPSEC_TUN_UI_MK_R_GATEWAY						_CFG_VPN(0x9B00) //T=str, for UI display FQDN
#define CSID_C_IPSEC_V6_TUN_IKE_L_MASK					_CFG_VPN(0x9C00) //T=str, IKE IPv6 Local Netmask		
#define CSID_C_IPSEC_V6_TUN_IKE_R_MASK					_CFG_VPN(0x9D00) //T=str, IKE IPv6 Remote Netmask

//Failover with full tunnel items
#define CSID_C_IPSEC_TUN_FAILOVER				_CFG_VPN(0x9E00) //T=u32, D=0,Operation mode,0:Always,1:Failover,2:Load Balance
#define CSID_C_IPSEC_TUN_FULL					_CFG_VPN(0x9F00) //T=u32, D=0,Full tunnel,0: Disable,1:Enable
//
#define CSID_C_IPSEC_TUN_OTT					_CFG_VPN(0xA000) //T=u32, D=0,ON THE TOP mode,0: Disable,1:Enable
#define CSID_C_IPSEC_TUN_FAILOVER_IDX           _CFG_VPN(0xA100) //T=u32, D=0,the tunnel idx use this failover tunnel
#define CSID_C_IPSEC_TUN_TYPE_FAILOVER          _CFG_VPN(0xA200) //T=u32,D=0,the major tunnel type which use this failover tunnel, 0:IPsec,1:PPTP,2:L2TP,3:GRE
//Hub and spoke
#define CSID_C_IPSEC_HUB_AND_SPOKE_ENABLE		_CFG_VPN(0xA300) //T=u32, D=0,IPsec hub and spoke enable, 0:disable, 1:enable
#define CSID_C_IPSEC_TUN_IKE_L_MULTI_SUBNET		_CFG_VPN(0xA400) //T=ipv4,IKE Multi Local Subnet
#define CSID_C_IPSEC_TUN_IKE_L_MULTI_MASK       _CFG_VPN(0xA600) //T=u32,IKE Multi Local Mask
#define CSID_C_IPSEC_TUN_IKE_R_MULTI_SUBNET		_CFG_VPN(0xA800) //T=ipv4,IKE Multi Remote Subnet
#define CSID_C_IPSEC_TUN_IKE_R_MULTI_MASK		_CFG_VPN(0xAA00) //T=u32,IKE Multi Remote Mask
//For IPsec manual
#define CSID_C_IPSEC_TUN_MK_R_ID_VALUE			_CFG_VPN(0xAC00) //T=u32, D=0,Manual Remote ID
#define CSID_C_IPSEC_TUN_MK_L_ID_VALUE			_CFG_VPN(0xAD00) //T=u32, D=0,Manual Local ID
#define CSID_C_IPSEC_TUN_IKE_VER				_CFG_VPN(0xAE00) //T=u32, D=0,0:IKEv1 only ,1:IKEv2 only ,2:IKEv1 & IKEv2
#define CSID_C_IPSEC_TUN_IKE_EZVPN_CLI_EN               _CFG_VPN(0xAF00) //T=u32, D=0,Enable CISCO EzVPN Client, 0:Disable, 1:Enable
#define CSID_C_IPSEC_TUN_SCENARIO                   _CFG_VPN(0xB000) //T=u32, D=0,0:site-to-site,1:site-to-host,2:host-to-site,3:host-to-host,4:Dynamic
#define CSID_C_IPSEC_HUB_AND_SPOKE_ROLE             _CFG_VPN(0xB100) //T=u32, D=0,0:As hub role,1:As spoke role


#define CSID_S_IPSEC_ALTERED							_STA_VPN(0x0002) //T=u32, vpn setting altered
#define CSID_S_IPSEC_DISCONNECT_ALTERED					_STA_VPN(0x0003) //T=u32, vpn tunnel disconnected altered
//#define CSID_S_IPSEC_RETRY_ALTERED						_STA_VPN(0x0004) //T=u32, vpn tunnel retry altered
#define CSID_S_IPSEC_TUN_ACTIDX                         _STA_VPN(0x0005) //T=u32, idx of Tunnel button
#define CSID_S_IPSEC_TUN_CMD							_STA_VPN(0x0006) //T=u32, command for ui event
#define CSID_S_IPSEC_ACTIVE_TUNNELS						_STA_VPN(0x0007) //T=u32, total active tunnels
#define CSID_S_IPSEC_DYN_TUNNELS						_STA_VPN(0x0008) //T=u32, total active dynamic tunnels
#define CSID_S_IPSEC_DYN_2_TUNNELS                                                _STA_VPN(0x0009) //T=u32, total active dynamic_2 tunnels
#define CSID_S_IPSEC_DYN_3_TUNNELS                                                _STA_VPN(0x000A) //T=u32, total active dynamic_3 tunnels
#define CSID_S_IPSEC_TUN_STATE                          _STA_VPN(0x0100) //T=u32, D=0,IKE Tunnel state,1:connected,2:wait for traffic,3:unconnected,9:connecting 
#define CSID_S_IPSEC_TUN_PKT_IN                         _STA_VPN(0x0200) //T=u32, packets incoming this tunnel
#define	CSID_S_IPSEC_TUN_PKT_OUT						_STA_VPN(0x0300) //T=u32, packets outgoing this tunnel
#define CSID_S_IPSEC_TUN_UPTIME							_STA_VPN(0x0400) //T=u32, tunnel up time
#define CSID_S_IPSEC_TUN_CNT_TIME						_STA_VPN(0x0500) //T=str, tunnel connecting time

/*Ping Keep Alive state ,Record Remote GATEWAY IP Address*/
#define CSID_S_IPSEC_TUN_R_GATEWAY_IP				    _STA_VPN(0x0700) //T=ipv4, remote GATEWAY IP Address
#define CSID_S_IPSEC_TUN_QUERY_R_GATEWAY_IP				_STA_VPN(0x0800) //T=ipv4, Recorad remote GATEWAY's  IP Address
#define CSID_S_IPSEC_TUN_KEEP_ALIVE_IP_STATE			_STA_VPN(0x0900) //T=u32, IKE_KEEP_ALIVE_IP STATE ,1:need to restart tunnel
/*RUN FULL TUNNEL INDEX*/
#define CSID_S_IPSEC_FULL_TUN_MASTER_IDX				_STA_VPN(0x0A00) //T=u32, D=0,full tunnel master index
#define CSID_S_IPSEC_FULL_TUN_RUN_IDX					_STA_VPN(0x0A01) //T=u32, D=0,full tunnel run index
#define CSID_S_IPSEC_TUN_ALTERED                                        _STA_VPN(0x0B00) //T=u32, D=0, VPN tunnel altered
#define CSID_S_IPSEC_TUN_R_SUBNET_IP                                _STA_VPN(0x0C00) //T=ipv4, Record remote subnet IP for tunnel scenario

/* IPv6 IPSEC */
#define CSID_S_IPSEC_V6_ALTERED                         _STA_VPN(0x1002) //T=u32, vpn setting altered
#define CSID_S_IPSEC_V6_DISCONNECT_ALTERED              _STA_VPN(0x1003) //T=u32, vpn tunnel disconnected altered
//#define CSID_S_IPSEC_V6_RETRY_ALTERED                 _STA_VPN(0x1004) //T=u32, vpn tunnel retry altered
#define CSID_S_IPSEC_V6_TUN_ACTIDX                      _STA_VPN(0x1005) //T=u32, idx of Tunnel button
#define CSID_S_IPSEC_V6_TUN_CMD                         _STA_VPN(0x1006) //T=u32, command for ui event
#define CSID_S_IPSEC_V6_ACTIVE_TUNNELS                  _STA_VPN(0x1007) //T=u32, total active tunnels
#define CSID_S_IPSEC_V6_DYN_TUNNELS                     _STA_VPN(0x1008) //T=u32, total active dynamic tunnels
#define CSID_S_IPSEC_V6_TUN_STATE                       _STA_VPN(0x1100) //T=u32, D=0,IKE Tunnel state,1:connected,2:wait for traffic,3:unconnected,9:connecting
#define CSID_S_IPSEC_V6_TUN_PKT_IN                      _STA_VPN(0x1200) //T=u32, packets incoming this tunnel
#define CSID_S_IPSEC_V6_TUN_PKT_OUT                     _STA_VPN(0x1300) //T=u32, packets outgoing this tunnel
#define CSID_S_IPSEC_V6_TUN_UPTIME                      _STA_VPN(0x1400) //T=u32, tunnel up time
#define CSID_S_IPSEC_V6_TUN_CNT_TIME                    _STA_VPN(0x1500) //T=str, tunnel connecting time
#define CSID_S_IPSEC_V6_DYN_CLIENTS                     _STA_VPN(0x2000) //T=ipv4, Dynamic IPSec client list


#define CSID_S_IPSEC_DYN_CLIENTS 					    _STA_VPN(0x2100) //T=ipv4, Dynamic IPSec client IPv4 list
#define CSID_S_IPSEC_DYN_CLIENTS_MASK					_STA_VPN(0x2200) //T=ipv4, Dynamic IPSec client subnet mask list
#define CSID_S_IPSEC_DYN_CLIENTS_GW						_STA_VPN(0x2300) //T=ipv4, Dynamic IPSec client gateway list
#define CSID_S_IPSEC_TUN_FAILOVER_ACTION                _STA_VPN(0x2400) //T=u32, D=0, Tunnel failover action 
#define CSID_S_IPSEC_TUN_REMOTE_SUBNET_LIST             _STA_VPN(0x2500) //T=str, All IPsec tunnel remote subnet list
#define CSID_S_IPSEC_TUN_EZVPN_CLI_IP                   _STA_VPN(0x2600) //T=ipv4, Virtual IP assigend by EzVPN sever
#define CSID_S_IPSEC_TUN_EZVPN_CLI_MASK                 _STA_VPN(0x2700) //T=ipv4, Virtual IP netmask assigned by EzVPN server
#define CSID_S_IPSEC_TUN_EZVPN_CLI_DNS                  _STA_VPN(0x2800) //T=ipv4, DNS server assigned by EzVPN server
#define CSID_S_IPSEC_TUN_EZVPN_CLI_SPLIT_TUN            _STA_VPN(0x2A00) //T=str, Split tunnel assigned by EzVPN server
/*For multi Dynamic IPesec server*/
#define CSID_S_IPSEC_DYN_2_CLIENTS                      _STA_VPN(0x3100) //T=ipv4, Dynamic_2 IPSec client IPv4 list
#define CSID_S_IPSEC_DYN_2_CLIENTS_MASK                 _STA_VPN(0x3200) //T=ipv4, Dynamic_2 IPSec client subnet mask list
#define CSID_S_IPSEC_DYN_2_CLIENTS_GW                   _STA_VPN(0x3300) //T=ipv4, Dynamic_2 IPSec client gateway list
#define CSID_S_IPSEC_DYN_3_CLIENTS                      _STA_VPN(0x3400) //T=ipv4, Dynamic_3 IPSec client IPv4 list
#define CSID_S_IPSEC_DYN_3_CLIENTS_MASK                 _STA_VPN(0x3500) //T=ipv4, Dynamic_3 IPSec client subnet mask list
#define CSID_S_IPSEC_DYN_3_CLIENTS_GW                   _STA_VPN(0x3600) //T=ipv4, Dynamic_3 IPSec client gateway list


#endif //ifndef _CSID_VPN_H

