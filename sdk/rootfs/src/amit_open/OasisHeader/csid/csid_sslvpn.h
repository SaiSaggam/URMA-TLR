/*
 * @file Member ID Definition of Group SSLVPN.
 *
 * Moderator: Austin
 * Group ID: 0x009C0000/0x809C0000
 */

#ifndef _CSID_SSLVPN_H
#define _CSID_SSLVPN_H

#include "csid_gid.h"

#define _CFG_SSLVPN(x)       (CSID_GIDC_SSLVPN|(x))
#define _STA_SSLVPN(x)       (CSID_GIDS_SSLVPN|(x))

/* Extra Definitions */
//SSLVPN

#define CSID_S_SSLVPN_ALTERED			    		_STA_SSLVPN(0x0000) //T=u32, ui altered of SSL VPN
#define CSID_S_SSLVPN_CMD                      	    _STA_SSLVPN(0x0001) //T=u32, cmd of SSLVPN
#define CSID_S_SSLVPN_ACTIDX    	            	_STA_SSLVPN(0x0002) //T=u32, idx of SSLVPN 
#define CSID_S_SSLVPN_OS_DETECTION      	    	_STA_SSLVPN(0x0003) //T=u32, os of SSLVPN 
#define CSID_S_SSLVPN_BROWSER_DETECTION       	    _STA_SSLVPN(0x0004) //T=u32, browser of SSLVPN 
#define CSID_S_SSLVPN_LOGIN_USERNAME   		    	_STA_SSLVPN(0x0005) //T=str, login username of SSLVPN 
#define CSID_S_SSLVPN_LOGIN_PASSWORD                _STA_SSLVPN(0x0006) //T=str, login password of SSLVPN 
#define CSID_S_SSLVPN_LOGO_UPLOAD_STATUS			_STA_SSLVPN(0x0007) //T=u32, D=0,logo pic upload status 1=uploading 2=ok 
#define CSID_S_SSLVPN_LOGIN_WAN_IP					_STA_SSLVPN(0x0008) //T=str, login wan ip
#define CSID_S_SSLVPN_LOGIN_WAN_IDX					_STA_SSLVPN(0x0009) //T=u32, login wan idx
#define CSID_S_SSLVPN_CLIENT_ALTERED	    		_STA_SSLVPN(0x000A) //T=u32, ui altered of SSL VPN Client
#define CSID_S_SSLVPN_CLIENT_CISCO_ALTERED	  		_STA_SSLVPN(0x000B) //T=u32, ui altered of SSL VPN Client for Cisco
#define CSID_S_SSLVPN_CLIENT_CONFIG_INDEX	  		_STA_SSLVPN(0x000C) //T=u32, ui altered of SSL VPN Client for SSLVPN client config


#define CSID_S_SSLVPN_STATUS_USERNAME				_STA_SSLVPN(0x0100) //T=str, sslvpn connected username
#define CSID_S_SSLVPN_STATUS_REMOTE_IP				_STA_SSLVPN(0x0200) //T=str, sslvpn connected remote ip
#define CSID_S_SSLVPN_STATUS_VIRTUAL_IP				_STA_SSLVPN(0x0300) //T=str, sslvpn connected virtual ip
#define CSID_S_SSLVPN_STATUS_CONNECTED				_STA_SSLVPN(0x0400) //T=u32, D=0,sslvpn connected status


#define CSID_C_SSLVPN_EN                            _CFG_SSLVPN(0x0000) //T=u32, D=0,SSLVPN Enable

#define CSID_C_SSLVPN_LOGIN_STRING                  _CFG_SSLVPN(0x0001) //T=str, login string
#define CSID_C_SSLVPN_LOGIN_LOGO		    		_CFG_SSLVPN(0x0002) //T=u32, login logo
#define CSID_C_SSLVPN_LOGIN_LOGO_FILE_NAME          _CFG_SSLVPN(0x0003) //T=str, logo file name
#define CSID_C_SSLVPN_SVR_EXTENDER_DEFAULT_EN       _CFG_SSLVPN(0x0004) //T=u32, D=0,SSLVPN extender default enable

#define CSID_C_SSLVPN_MULTI_WAN_ENABLE              _CFG_SSLVPN(0x0010) //T=u32, SSLVPN Enable

#define CSID_C_SSLVPN_SVR_IP                        _CFG_SSLVPN(0x0020) //T=str, SSLVPN ip
#define CSID_C_SSLVPN_SVR_PORT                      _CFG_SSLVPN(0x0030) //T=u32, D=443,SSLVPN Enable
#define CSID_C_SSLVPN_SVR_PROTO                     _CFG_SSLVPN(0x0040) //T=u32, D=1,0:udp,1:tcp
#define CSID_C_SSLVPN_SVR_DEV                       _CFG_SSLVPN(0x0050) //T=u32, D=0,0:tun,1:tap

#define CSID_C_SSLVPN_IDLE_TIMEOUT                  _CFG_SSLVPN(0x0060) //T=u32, D=3600, idle timeout
#define CSID_C_SSLVPN_CLEAN_WEB_CACHE_ENABLE        _CFG_SSLVPN(0x0070) //T=u32, D=0, Clean web cache enable

#define CSID_C_SSLVPN_SVR_PUSH_DNS_EN               _CFG_SSLVPN(0x0080) //T=u32, D=0, SSLVPN DNS
#define CSID_C_SSLVPN_SVR_PRIDNS                    _CFG_SSLVPN(0x0090) //T=ipv4, SSLVPN Primary DNS
#define CSID_C_SSLVPN_SVR_SECDNS                    _CFG_SSLVPN(0x00A0) //T=ipv4, SSLVPN Second DNS

#define CSID_C_SSLVPN_SVR_PUSH_WINS_EN              _CFG_SSLVPN(0x00B0) //T=u32, D=0,SSLVPN WINS
#define CSID_C_SSLVPN_SVR_PRIWINS                   _CFG_SSLVPN(0x00C0) //T=ipv4, Primary WINS 
#define CSID_C_SSLVPN_SVR_SECWINS                   _CFG_SSLVPN(0x00D0) //T=ipv4, Second WINS  

#define CSID_C_SSLVPN_SVR_PUSH_LAN                  _CFG_SSLVPN(0x00E0) //T=u32, D=0,SSLVPN Push LAN to Client

#define CSID_C_SSLVPN_VIRTUAL_IP            	    _CFG_SSLVPN(0x00F0) //T=ipv4, D={10.8.0.1}, SSLVPN Client IP
#define CSID_C_SSLVPN_VIRTUAL_IP_SUBNET				_CFG_SSLVPN(0x0100) //T=ipv4, D={255.255.255.0}, SSLVPN Client Subnet

#define CSID_C_SSLVPN_FULL_TUNNEL_ENABLE            _CFG_SSLVPN(0x0110) //T=u32, D=0, SSLVPN full tunnel enable
#define CSID_C_SSLVPN_SVR_MAXLEASE                  _CFG_SSLVPN(0x0120) //T=u32, D=5,SSLVPN IPPool Max Lease tap only
#define CSID_C_SSLVPN_SVR_IPPOOL_START              _CFG_SSLVPN(0x0130) //T=ipv4, D={10.8.0.10},SSLVPN start lan ip tap only
#define CSID_C_SSLVPN_SVR_IPPOOL_END                _CFG_SSLVPN(0x0140) //T=ipv4, D={10.8.0.20},SSLVPN end lan ip tap only

#define CSID_C_SSLVPN_SVR_MAX_CLIENT                _CFG_SSLVPN(0x0150) //T=u32, D=2,SSLVPN Max Clients

#define CSID_C_SSLVPN_SVR_MAX_KEEP_ALIVE            _CFG_SSLVPN(0x0160) //T=u32, D=60,SSLVPN maximum keep alive 
#define CSID_C_SSLVPN_SVR_MIN_KEEP_ALIVE            _CFG_SSLVPN(0x0170) //T=u32, D=20,SSLVPN minimum keep alive 

#define CSID_C_SSLVPN_AUTH                          _CFG_SSLVPN(0x0180) //T=u32, D=0,SSLVPN authentication 0:default
#define CSID_C_SSLVPN_CIPHER                        _CFG_SSLVPN(0x0190) //T=u32, D=0,SSLVPN cipher 0:default
#define CSID_C_SSLVPN_COMP                          _CFG_SSLVPN(0x01A0) //T=u32, D=0,SSLVPN compression 0:default
#define CSID_C_SSLVPN_REDIRECT_GW_EN                _CFG_SSLVPN(0x01B0) //T=u32, D=0,SSLVPN Redirection gateway
#define CSID_C_SSLVPN_SVR_EXTENDER_EN               _CFG_SSLVPN(0x01C0) //T=u32, D=0,SSLVPN extender enable
#define CSID_C_SSLVPN_SVR_EXTENDER_SUBNET_NAME      _CFG_SSLVPN(0x0200) //T=str, SSLVPN extender enable
#define CSID_C_SSLVPN_SVR_EXTENDER_SUBNET_IP        _CFG_SSLVPN(0x0300) //T=ipv4, SSLVPN extender subnet ip
#define CSID_C_SSLVPN_SVR_EXTENDER_SUBNET_MASK      _CFG_SSLVPN(0x0400) //T=ipv4, SSLVPN extender subnet mask

#define CSID_C_SSLVPN_APPLICATION_ALL_EN	        _CFG_SSLVPN(0x0500) //T=u32, D=0,SSLVPN Application all enable
#define CSID_C_SSLVPN_APPLICATION_EN		        _CFG_SSLVPN(0x0600) //T=u32, D=0,SSLVPN Application enable
#define CSID_C_SSLVPN_APPLICATION_NAME		        _CFG_SSLVPN(0x0700) //T=str, SSLVPN Application name
#define CSID_C_SSLVPN_APPLICATION_TYPE		        _CFG_SSLVPN(0x0800) //T=u32, D=0,SSLVPN Application type
#define CSID_C_SSLVPN_APPLICATION_IP       			_CFG_SSLVPN(0x0900) //T=ipv4, Application IP
#define CSID_C_SSLVPN_APPLICATION_PORT              _CFG_SSLVPN(0x0A00) //T=u32, Application Port
#define CSID_C_SSLVPN_HTTPS_MD5             		_CFG_SSLVPN(0x0B00) //T=str, https ip to md5
#define CSID_C_SSLVPN_RDP_SCREEN_SIZE       		_CFG_SSLVPN(0x0C00) //T=u32, D=0,RDP Screen size
#define CSID_C_SSLVPN_RDP_WIDTH              		_CFG_SSLVPN(0x0D00) //T=u32, D=0,RDP width
#define CSID_C_SSLVPN_RDP_HEIGHT              		_CFG_SSLVPN(0x0E00) //T=u32, D=0,RDP height
#define CSID_C_SSLVPN_VNC_SCALING              		_CFG_SSLVPN(0x0F00) //T=u32, D=0,VPN Scaling
#define CSID_C_SSLVPN_MYPLACE_PATH             		_CFG_SSLVPN(0x1000) //T=str, My place path

#define CSID_C_SSLVPN_USR_ENABLE                  	_CFG_SSLVPN(0x1100) //T=str, SSLVPN user enable
#define CSID_C_SSLVPN_USERNAME                  	_CFG_SSLVPN(0x1200) //T=str, SSLVPN account username
#define CSID_C_SSLVPN_PASSWORD                  	_CFG_SSLVPN(0x1300) //T=str, SSLVPN account password
#define CSID_C_SSLVPN_USR_LEVEL                 	_CFG_SSLVPN(0x1400) //T=u32, D=0, SSLVPN account user level
#define CSID_C_SSLVPN_USR_GROUP_ENABLE              _CFG_SSLVPN(0x1500) //T=u32, SSLVPN group enable
#define CSID_C_SSLVPN_USR_GROUP_NAME            	_CFG_SSLVPN(0x1600) //T=str, SSLVPN group name
#define CSID_C_SSLVPN_USR_GROUP_LEVEL            	_CFG_SSLVPN(0x1700) //T=u32, SSLVPN group level
#define CSID_C_SSLVPN_USR_CONNECT_STATUS           	_CFG_SSLVPN(0x1800) //T=u32, D=0, SSLVPN user connect status
#define CSID_C_SSLVPN_USR_CONNECT_IP	           	_CFG_SSLVPN(0x1900) //T=ipv4, SSLVPN user connect ip

#define CSID_C_SSLVPN_EXTENDER_EN	            	_CFG_SSLVPN(0x2000) //T=u32, D=0, SSLVPN exteneder enable
#define CSID_C_SSLVPN_EXTENDER_NAME	            	_CFG_SSLVPN(0x2100) //T=str, SSLVPN exteneder name
#define CSID_C_SSLVPN_EXTENDER_SUBNET_IP           	_CFG_SSLVPN(0x2200) //T=ipv4, SSLVPN exteneder subnet ip
#define CSID_C_SSLVPN_EXTENDER_SUBNET_MASK         	_CFG_SSLVPN(0x2300) //T=ipv4, SSLVPN exteneder subnet mask

#define CSID_C_SSLVPN_CLIENT_CISCO_ENABLE			_CFG_SSLVPN(0x3000)	//T=u32, D=0, SSLVPN Client for cisco enable
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_ENABLE		_CFG_SSLVPN(0x3010)	//T=u32, D=0, SSLVPN Client for cisco tun enable
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_NAME			_CFG_SSLVPN(0x3020)	//T=str, SSLVPN Client for cisco name
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_INTERFACE	_CFG_SSLVPN(0x3030)	//T=u32, D=0, SSLVPN Client for cisco interface
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_REMOTE_IP	_CFG_SSLVPN(0x3040)	//T=str, SSLVPN Client for cisco remote ip
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_FULLTUNNEL	_CFG_SSLVPN(0x3050)	//T=u32, D=0, SSLVPN Client for cisco full tunnel
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_GROUP		_CFG_SSLVPN(0x3060)	//T=str, SSLVPN Client for cisco group
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_USERNAME		_CFG_SSLVPN(0x3070)	//T=str, SSLVPN Client for cisco username
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_PASSWORD		_CFG_SSLVPN(0x3080)	//T=str, SSLVPN Client for cisco password
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_DPD			_CFG_SSLVPN(0x3090)	//T=u32, D=30, SSLVPN Client for cisco dpd
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_RECONNECT	_CFG_SSLVPN(0x30A0)	//T=u32, D=300, SSLVPN Client for cisco reconnect timeout
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_VIRTUAL_IP	_CFG_SSLVPN(0x30B0)	//T=str, SSLVPN Client for cisco virtual ip
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_SUBNET		_CFG_SSLVPN(0x30C0)	//T=str, SSLVPN Client for cisco virtual subnet
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_STATUS		_CFG_SSLVPN(0x30D0)	//T=u32, D=0, SSLVPN Client for cisco status
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_REMOTE_SUBNET		_CFG_SSLVPN(0x30E0)	//T=str, SSLVPN Client for cisco remote subnet
#define CSID_C_SSLVPN_CLIENT_CISCO_TUN_REMOTE_NETMASK		_CFG_SSLVPN(0x30F0)	//T=str, SSLVPN Client for cisco remote netmask

#define CSID_C_SSLVPN_CLIENT_ENABLE					_CFG_SSLVPN(0x4000)	//T=u32, D=0, SSLVPN Client enable
#define CSID_C_SSLVPN_CLIENT_TUN_ENABLE				_CFG_SSLVPN(0x4010)	//T=u32, D=0, SSLVPN Client tun enable
#define CSID_C_SSLVPN_CLIENT_TUN_NAME				_CFG_SSLVPN(0x4020)	//T=str, SSLVPN Client name
#define CSID_C_SSLVPN_CLIENT_TUN_INTERFACE			_CFG_SSLVPN(0x4030)	//T=u32, D=0, SSLVPN Client interface
#define CSID_C_SSLVPN_CLIENT_TUN_REMOTE_IP			_CFG_SSLVPN(0x4040)	//T=str, SSLVPN Client for remote ip
#define CSID_C_SSLVPN_CLIENT_TUN_REMOTE_SUBNET		_CFG_SSLVPN(0x4050)	//T=str, SSLVPN Client for remote subnet
#define CSID_C_SSLVPN_CLIENT_TUN_REMOTE_NETMASK		_CFG_SSLVPN(0x4060)	//T=str, SSLVPN Client for remote netmask
#define CSID_C_SSLVPN_CLIENT_TUN_PROTO				_CFG_SSLVPN(0x4070)	//T=u32, D=1,0:udp,1:tcp, SSLVPN Client proto
#define CSID_C_SSLVPN_CLIENT_TUN_PORT				_CFG_SSLVPN(0x4080)	//T=u32, D=443, SSLVPN Client port
#define CSID_C_SSLVPN_CLIENT_TUN_DEV				_CFG_SSLVPN(0x4090)	//T=u32, D=0, 0:tun,1:tap, SSLVPN Client device
#define CSID_C_SSLVPN_CLIENT_TUN_AUTH_MODE			_CFG_SSLVPN(0x40A0)	//T=u32, D=1, 0:static,1:tls, SSLVPN Client auth mode
#define CSID_C_SSLVPN_CLIENT_TUN_L_ENDPOINT_IP		_CFG_SSLVPN(0x40B0)	//T=str, SSLVPN Client local endpoint ip
#define CSID_C_SSLVPN_CLIENT_TUN_R_ENDPOINT_IP		_CFG_SSLVPN(0x40C0)	//T=str, SSLVPN Client remote endpoint ip
#define CSID_C_SSLVPN_CLIENT_TUN_STATIC_KEY			_CFG_SSLVPN(0x40D0)	//T=str, SSLVPN Client static key
#define CSID_C_SSLVPN_CLIENT_TUN_CA					_CFG_SSLVPN(0x40E0)	//T=str, SSLVPN Client ca certificate
#define CSID_C_SSLVPN_CLIENT_TUN_CLIENT_CERT		_CFG_SSLVPN(0x40F0)	//T=str, SSLVPN Client client certificate
#define CSID_C_SSLVPN_CLIENT_TUN_ENCRY_CIPHER		_CFG_SSLVPN(0x4100)	//T=u32, D=1, 0:None,1:bf,2:aes128,3:aes192,4:aes256,5:aes512, SSLVPN Client encry cipher
#define CSID_C_SSLVPN_CLIENT_TUN_HASH_ALG			_CFG_SSLVPN(0x4110)	//T=u32, D=1, 0:None,1:sha1,2:sha256,3:sha512,4:md4,5:md5, SSLVPN Client hash algorithm
#define CSID_C_SSLVPN_CLIENT_TUN_ADVANCE			_CFG_SSLVPN(0x4120)	//T=u32, D=0, SSLVPN Client advance option
#define CSID_C_SSLVPN_CLIENT_TUN_TLS_CIPHER			_CFG_SSLVPN(0x4130)	//T=u32, D=2, 0:None,1:rc4-md5,2:rsa-aes128,3:rsa-aes256,4:rsa-sha384,5:dhe-aes128,6:dhe-aes256,7:dhe-sha384 SSLVPN Client TLS CIPHER
#define CSID_C_SSLVPN_CLIENT_TUN_LZO				_CFG_SSLVPN(0x4140)	//T=u32, D=0, 0:Adaptive,1:yes,2:no SSLVPN Client lzo
#define CSID_C_SSLVPN_CLIENT_TUN_TLS_KEY			_CFG_SSLVPN(0x4150)	//T=str, SSLVPN Client tls key
#define CSID_C_SSLVPN_CLIENT_TUN_USERNAME			_CFG_SSLVPN(0x4160)	//T=str, SSLVPN Client for username
#define CSID_C_SSLVPN_CLIENT_TUN_PASSWORD			_CFG_SSLVPN(0x4170)	//T=str, SSLVPN Client for password
#define CSID_C_SSLVPN_CLIENT_TUN_NAT_ENABLE			_CFG_SSLVPN(0x4180)	//T=u32, D=1, SSLVPN Client nat enable
#define CSID_C_SSLVPN_CLIENT_TUN_BRIDGE_ENABLE		_CFG_SSLVPN(0x4190)	//T=u32, D=0, SSLVPN Client bridge enable
#define CSID_C_SSLVPN_CLIENT_TUN_BRIDGE_VLAN		_CFG_SSLVPN(0x41A0)	//T=u32, D=1, SSLVPN Client bridge vlan
#define CSID_C_SSLVPN_CLIENT_TUN_FIREWALL_ENABLE	_CFG_SSLVPN(0x41B0)	//T=u32, D=1, SSLVPN Client firewall enable
#define CSID_C_SSLVPN_CLIENT_TUN_IP_TYPE			_CFG_SSLVPN(0x41C0)	//T=u32, D=0, 0:dynamic 1:static SSLVPN Client ip type
#define CSID_C_SSLVPN_CLIENT_TUN_IP_ADDR			_CFG_SSLVPN(0x41D0)	//T=str, SSLVPN Client ip address
#define CSID_C_SSLVPN_CLIENT_TUN_SUBNET_MASK		_CFG_SSLVPN(0x41E0)	//T=str, SSLVPN Client subnet mask
#define CSID_C_SSLVPN_CLIENT_TUN_MTU				_CFG_SSLVPN(0x41F0)	//T=u32, D=1500, SSLVPN Client mtu
#define CSID_C_SSLVPN_CLIENT_TUN_UDP_FRAGMENT		_CFG_SSLVPN(0x4200)	//T=u32, D=1500, SSLVPN Client udp fragment
#define CSID_C_SSLVPN_CLIENT_TUN_MSS_FIX_ENABLE		_CFG_SSLVPN(0x4210)	//T=u32, D=0, SSLVPN Client mss fix
#define CSID_C_SSLVPN_CLIENT_TUN_NSCERT_VERIFY_ENABLE	_CFG_SSLVPN(0x4220)	//T=u32, D=0, SSLVPN Client nsCertType verification
#define CSID_C_SSLVPN_CLIENT_TUN_REDIRECT_ENABLE	_CFG_SSLVPN(0x4230)	//T=u32, D=0, SSLVPN Client Redirect Internet traffic 
#define CSID_C_SSLVPN_CLIENT_TUN_TLS_TIME			_CFG_SSLVPN(0x4240)	//T=u32, D=3600, SSLVPN Client TLS Renegotiation Time
#define CSID_C_SSLVPN_CLIENT_TUN_CONN_RETRY			_CFG_SSLVPN(0x4250)	//T=u32, D=-1, -1=infiite, SSLVPN Client Connection Retry
#define CSID_C_SSLVPN_CLIENT_TUN_DNS_ACCEPT_ENABLE	_CFG_SSLVPN(0x4260)	//T=u32, D=0, SSLVPN Client Accept DNS from Server
#define CSID_C_SSLVPN_CLIENT_TUN_DNS_SET_ENABLE		_CFG_SSLVPN(0x4270)	//T=u32, D=0, SSLVPN Client Set DNS Enable
#define CSID_C_SSLVPN_CLIENT_TUN_DNS_PRIMARY		_CFG_SSLVPN(0x4280)	//T=str, SSLVPN Client Primary DNS
#define CSID_C_SSLVPN_CLIENT_TUN_DNS_SECONDARY		_CFG_SSLVPN(0x4290)	//T=str, SSLVPN Client Secondary DNS

#endif //ifndef _CSID_X509_H