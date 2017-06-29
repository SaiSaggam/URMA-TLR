/*
 * @file Member ID Definition of Group SNMP.
 *
 * Moderator: JackChiang
 * Group ID: 0x00160000/0x80160000
 */

#ifndef _CSID_SNMP_H
#define _CSID_SNMP_H

#include "csid_gid.h"

#define _CFG_SNMP(x)		(CSID_GIDC_SNMP|(x))
#define _STA_SNMP(x)		(CSID_GIDS_SNMP|(x))

/* Configuration Items */
#define CSID_C_SNMP_MULTIWAN_ENABLE		_CFG_SNMP(0x0000) //T=u8,D=0, enable support snmp multi-wan, 1:enable, 0:disable
#define CSID_C_SNMP_LOCAL_ENABLE		_CFG_SNMP(0x0001) //T=u8,D=0, enable local snmp , 1:yes 0:no
#define CSID_C_SNMP_REMOTE_ENABLE		_CFG_SNMP(0x0002) //T=u8,D=0, enable remote snmp , 1:yes 0:no
#define CSID_C_SNMP_GET_COMMUNITY		_CFG_SNMP(0x0003) //T=str, snmp community in get query
#define CSID_C_SNMP_SET_COMMUNITY		_CFG_SNMP(0x0004) //T=str, snmp community in set action
#define CSID_C_SNMP_TRIP_IP	    		_CFG_SNMP(0x0005) //T=str, snmp send trip message to which ip 
#define CSID_C_SNMP_VERSION        		_CFG_SNMP(0x0009) //T=u8, snmp version of community
#define CSID_C_SNMP_SYSLOCATION			_CFG_SNMP(0x000a) //T=str, router's information in syslocation
#define CSID_C_SNMP_SYSCONTACT			_CFG_SNMP(0x000b) //T=str, router's information in syscontact
#define CSID_C_SNMP_SYSNAME				_CFG_SNMP(0x000c) //T=str, router's information in sysname
#define CSID_C_SNMP_WAN_ACCESS_IP		_CFG_SNMP(0x000d) //T=str, wan access IP address 
#define CSID_C_SNMP_SYSDESCR			_CFG_SNMP(0x000e) //T=str, router's information in descr
#define CSID_C_SNMP_ENABLE				_CFG_SNMP(0x000f) //T=u8, enable snmp , 1:yes 0:no

/* Configure SNMP Version */
#define CSID_C_SNMP_VERSION_V1          _CFG_SNMP(0x0010) //T=u8,D=0, snmp v1
#define CSID_C_SNMP_VERSION_V2C         _CFG_SNMP(0x0011) //T=u8,D=0, snmp v2c
#define CSID_C_SNMP_VERSION_V3          _CFG_SNMP(0x0012) //T=u8,D=0, snmp v3

/* Configure read/write SNMPv3 account */
#define CSID_C_SNMP_RW_ENABLE          	_CFG_SNMP(0x0013) //T=u8, enable local snmp , 1:yes 0:no
#define CSID_C_SNMP_RW_V3USER          	_CFG_SNMP(0x0014) //T=str, router's information in descr
#define CSID_C_SNMP_RW_V3PASSWORD      	_CFG_SNMP(0x0015) //T=str, router's information in descr
#define CSID_C_SNMP_RW_AUTHTYPE        	_CFG_SNMP(0x0016) //T=u8, D=0, MD5|SHA
#define CSID_C_SNMP_RW_PRIVACYMOD      	_CFG_SNMP(0x0017) //T=u8, D=1, noAuthNoPriv|authNoPriv|authPriv 
#define CSID_C_SNMP_RW_PRIVACY         	_CFG_SNMP(0x0018) //T=str, router's information in descr

/* Configure read only SNMPv3 account */
#define CSID_C_SNMP_RO_ENABLE           _CFG_SNMP(0x0019) //T=u8, enable local snmp , 1:yes 0:no
#define CSID_C_SNMP_RO_V3USER           _CFG_SNMP(0x001a) //T=str, router's information in descr
#define CSID_C_SNMP_RO_V3PASSWORD       _CFG_SNMP(0x001b) //T=str, router's information in descr
#define CSID_C_SNMP_RO_AUTHTYPE         _CFG_SNMP(0x001c) //T=u8, D=0, MD5|SHA
#define CSID_C_SNMP_RO_PRIVACYMOD       _CFG_SNMP(0x001d) //T=u8, D=1, noAuthNoPriv|authNoPriv|authPriv
#define CSID_C_SNMP_RO_PRIVACY          _CFG_SNMP(0x001e) //T=str, router's information in descr

/* DDP DUT Info */
#define CSID_C_SNMP_DDPENABLE           _CFG_SNMP(0x001f) //T=u32,  enable ddp
#define CSID_C_SNMP_CPULOAD             _CFG_SNMP(0x0020) //T=u32,  CPU load
#define CSID_C_SNMP_MEMTOTAL			_CFG_SNMP(0x0021) //T=u32,  MEM total
#define CSID_C_SNMP_MEMFREE             _CFG_SNMP(0x0022) //T=u32,  MEM free
#define CSID_C_SNMP_MEMUSED             _CFG_SNMP(0x0023) //T=u32,  MEM used
#define CSID_C_SNMP_MEMLOAD             _CFG_SNMP(0x0024) //T=u32,  MEM used
#define CSID_C_SNMP_DDPBROADFREQ        _CFG_SNMP(0x0025) //T=u32,  ddp broadcast`s time period frequency
#define CSID_C_SNMP_DDPMAXNGTIME        _CFG_SNMP(0x0026) //T=u32,  max times of ddp reply 
#define CSID_C_SNMP_DDPUSER             _CFG_SNMP(0x0027) //T=str,  ddp client user
#define CSID_C_SNMP_DDPPSWD             _CFG_SNMP(0x0028) //T=str,  ddp client pswd
#define CSID_C_SNMP_NEWIP				_CFG_SNMP(0x0029) //T=ipv4, ddp client new ip
#define CSID_C_SNMP_DDPADD              _CFG_SNMP(0x002a) //T=u32,  1:fail 2:successful
#define CSID_C_SNMP_DDPSET              _CFG_SNMP(0x002b) //T=u32,  1:fail 2:successful
#define CSID_C_SNMP_DEVNUM              _CFG_SNMP(0x002c) //T=u32,  ddp dev num
#define CSID_C_SNMP_TRAPALTERED         _CFG_SNMP(0x002d) //T=u32,  1:send 
#define CSID_C_SNMP_POSTPATH            _CFG_SNMP(0x002e) //T=str,  ddp post path
#define CSID_C_SNMP_POSTIP              _CFG_SNMP(0x002f) //T=str,  ddp post ip

/* Configure SNMPv3 multi account */
#define CSID_C_SNMP_MT_ENABLE           _CFG_SNMP(0x0030) //T=u8, enable multi account snmp, 1:yes,0:no
#define CSID_C_SNMP_MT_V3USER           _CFG_SNMP(0x0040) //T=str, snmpv3 user account
#define CSID_C_SNMP_MT_V3PASSWORD       _CFG_SNMP(0x0050) //T=str, snmpv3 user password
#define CSID_C_SNMP_MT_AUTHTYPE         _CFG_SNMP(0x0060) //T=u8, D=0, 0:MD5,1:SHA
#define CSID_C_SNMP_MT_PRIVACYMOD       _CFG_SNMP(0x0070) //T=u8, D=1, 0:noAuthNoPriv,1:authNoPriv,2:authPriv
#define CSID_C_SNMP_MT_PRIVACY          _CFG_SNMP(0x0080) //T=str, DES passphrase
#define CSID_C_SNMP_MT_RO_RW            _CFG_SNMP(0x0090) //T=u8, 0:read only,1:read write
#define CSID_C_SNMP_MT_PRIVACYTYPE		_CFG_SNMP(0x00b0) //T=u32, D=0, 0:DES, 1:AES

/* Configure SNMP Extra Items */
#define CSID_C_SNMP_PORT               				_CFG_SNMP(0x00a0) //T=u8, D=161, Set SNMP Port
#define CSID_C_SNMP_TRAP_ENABLE						_CFG_SNMP(0x00a1) //T=u8, 0:disable trap, 1:enable trap
#define CSID_C_SNMP_TRAP_COMMUNITY					_CFG_SNMP(0x00a2) //T=str, Set SNMP Trap Community
#define CSID_C_SNMP_WAN								_CFG_SNMP(0x00a3) //T=u8, SNMP WAN Index
#define CSID_C_SNMP_FWPATH							_CFG_SNMP(0x00a4) //T=u8, SNMP FIRMWARE PATH
#define CSID_C_SNMP_SECOND_TRAPPORT_ENABLE			_CFG_SNMP(0x00a5) //T=u8, SNMP swcondary trap port, 0:disable , 1: enable
#define CSID_C_SNMP_LAN_ACCESS_IP_ENABLE			_CFG_SNMP(0x00a6) //T=u8, for customer zyxel lan access ip access enable, disable
#define CSID_C_SNMP_TRAP_RECEIVER_ENABLE			_CFG_SNMP(0x00a7) //T=u8, using for enable trap receiver, 0:disable, 1:enable
#define CSID_C_SNMP_ENABLE_SNMP_FLAG				_CFG_SNMP(0x00a8) //T=u8, using for check if using CSID_C_SNMP_ENABLE, 0: not using , 1:using
#define CSID_C_SNMP_USING_NETSNMP					_CFG_SNMP(0x00a9) //T=u8, using net-snmp or ucd-snmp, 1:net-snmp, 0:ucd-snmp
#define CSID_C_SNMP_ENTERPRISE_OID					_CFG_SNMP(0x00aa) //T=str, enterprise oid, e.g. 1.3.6.1.4.1.12823
#define CSID_C_SNMP_ENTERPRISE_OID_FULL				_CFG_SNMP(0x00ab) //T=str, enterprise full oid length, e.g. 1.3.6.1.4.1.171.1.2
#define CSID_C_SNMP_ENTERPRISE_OID_CUSTOMER			_CFG_SNMP(0x00ac) //T=str, enterprise customer oid
#define CSID_C_SNMP_ENTERPRISE_OID_FULL_CUSTOMER 	_CFG_SNMP(0x00ad) //T=str, enterprise customer require full oid
#define CSID_C_SNMP_ENTERPRISE_NAME					_CFG_SNMP(0x00ae) //T=str, enterprise name, e.g. AMIT
#define CSID_C_SNMP_ENTERPRISE_NUMBER				_CFG_SNMP(0x00af) //T=str, enterprise number, e.g. 12823
#define CSID_C_SNMP_V3_ENGINEID						_CFG_SNMP(0x00b0) //T=str, v3 engine id

/*SNMP Trap */
#define CSID_C_SNMP_TRAP_ENTERPRISE_ENABLE			_CFG_SNMP(0x0100) //T=u8, enterprise trap, 0:disable, 1:enable
#define CSID_C_SNMP_TRAP_GERERIC_ENABLE				_CFG_SNMP(0x0101) //T=u8, generic trap, 0:disable, 1:enable
#define CSID_C_SNMP_TRAP_AUTHFAIL_ENABLE			_CFG_SNMP(0x0102) //T=u8, auth. fail trap, 0:disable, 1:enable
#define CSID_C_SNMP_TRAP_VRRP_ENABLE				_CFG_SNMP(0x0103) //T=u8, vrrp trap, 0:disable, 1:enable

/*D-store Agent DDPv2*/
#define CSID_C_DSTORE_AGENT_SERVER_IP				_CFG_SNMP(0x1000) //T=str, it DStore-anget sever IP
#define CSID_C_DSTORE_AGENT_SERVER_PORT				_CFG_SNMP(0x1002) //T=u32, it DStore-anget sever port

/*Net-SNMP Items*/
/*Net-SNMP multi CSID for v1/v2c*/
#define CSID_C_SNMP_MTN_VERSION_V1					_CFG_SNMP(0x2100) //T=u32,D=0, snmp v1 enable/disable, 0:disable, 1:enable
#define CSID_C_SNMP_MTN_VERSION_V2C					_CFG_SNMP(0x2200) //T=u32,D=0, snmp v2c enable/disable, 0:disable, 1:enable
#define CSID_C_SNMP_MTN_VERSION_V3					_CFG_SNMP(0x2300) //T=u32,D=0, snmp v3 enable/disable, 0:disable, 1:enable
#define CSID_C_SNMP_MTN_OID_FILTER					_CFG_SNMP(0x2400) //T=str,snmp oid filter
#define CSID_C_SNMP_MTN_PORT						_CFG_SNMP(0x2500) //T=u32,snmp port
/*Net-SNMP multi CSID for v1/v2c*/
#define CSID_C_SNMP_MTN_LOCAL_ENABLE				_CFG_SNMP(0x3000)	//T=u32, D=0,SNMP local enable, 0:disable, 1:enable
#define CSID_C_SNMP_MTN_REMOTE_ENABLE				_CFG_SNMP(0x3100)	//T=u32, D=0,SNMP remote enable, 0:disable, 1:enable
#define CSID_C_SNMP_MTN_COMMUNITY_ENABLE			_CFG_SNMP(0x3200)	//T=u32, D=0,SNMP get/set community enable, 0:disable, 1:enable
#define CSID_C_SNMP_MTN_COMMUNITY					_CFG_SNMP(0x3300)	//T=str, SNMP get/set community
#define CSID_C_SNMP_MTN_COMMUNITY_ACCESS_LEVEL		_CFG_SNMP(0x3400)	//T=u32, D=0,SNMP get/set community access level, 0: none, 1: get only, 2: both get,set
#define CSID_C_SNMP_MTN_COMMUNITY_GROUP_V1			_CFG_SNMP(0x3500)   //T=str, SNMP community group name for v1
#define CSID_C_SNMP_MTN_COMMUNITY_GROUP_V2C			_CFG_SNMP(0x3600)   //T=str, SNMP community group name for v2c
#define CSID_C_SNMP_MTN_GET_COMMUNITY				_CFG_SNMP(0x3700)	//T=str, SNMP get community
#define CSID_C_SNMP_MTN_SET_COMMUNITY				_CFG_SNMP(0x3800)	//T=str, SNMP set community
#define CSID_C_SNMP_MTN_WAN_ACCESS_IP				_CFG_SNMP(0x3900)	//T=str, SNMP wan access ip
/*Net SNMP multi CSID for v3*/
#define CSID_C_SNMP_MTN_v3_ENABLE					_CFG_SNMP(0x4000)	//T=u32, D=0,enable multi account snmp, 1:yes,0:no
#define CSID_C_SNMP_MTN_v3_USERNAME					_CFG_SNMP(0x4100)	//T=str, snmpv3 user account
#define CSID_C_SNMP_MTN_v3_PASSWORD					_CFG_SNMP(0x4200)	//T=str, snmpv3 user password
#define CSID_C_SNMP_MTN_v3_AUTHTYPE					_CFG_SNMP(0x4300)	//T=u32, D=0, 0:None,1:MD5, 2:SHA1
#define CSID_C_SNMP_MTN_v3_ENCRYPTION_MOD			_CFG_SNMP(0x4400)	//T=u32, D=0, 0:noAuthNoPriv,1:authNoPriv,2:authPriv
#define CSID_C_SNMP_MTN_v3_ENCRYPTION_TYPE			_CFG_SNMP(0x4500) 	//T=u32, D=0, 0:None, 1:DES, 2:AES
#define CSID_C_SNMP_MTN_v3_ENCRYPTION_KEY			_CFG_SNMP(0x4600) 	//T=str, DES/AES encryption key
#define CSID_C_SNMP_MTN_v3_RO_RW					_CFG_SNMP(0x4700)	//T=u32,D=0,0:read only,1:read write
#define CSID_C_SNMP_MTN_v3_USER_VIEWS				_CFG_SNMP(0x4800) 	//T=str, snmp v3 user views
/*Net SNMP multi for trap*/
#define CSID_C_SNMP_MTN_TRAP_ENABLE					_CFG_SNMP(0x5000)	//T=u32, D=0, snmp trap enable, 0:disable, 1:enable
#define CSID_C_SNMP_MTN_TRAP_SERVER_IP				_CFG_SNMP(0x5100)	//T=str,snmp trap server ip
#define CSID_C_SNMP_MTN_TRAP_SERVER_PORT			_CFG_SNMP(0x5200)	//T=u32,snmp trap server port
#define CSID_C_SNMP_MTN_TRAP_INTERFACE				_CFG_SNMP(0x5300)	//T=str,snmp trap interface
#define CSID_C_SNMP_MTN_TRAP_USE_VERSION			_CFG_SNMP(0x5400)	//T=u8,snmp trap using version, D=0, 1:v1, 2:v2c, 3:v3
#define CSID_C_SNMP_MTN_TRAP_V2c_INFORM_ENABLE		_CFG_SNMP(0x5500)	//T=u32,snmp trap using version v2c inform enable,D=0, 0:disbale, 1:enable
#define CSID_C_SNMP_MTN_TRAP_V2c_INFORM_RETRY_SEC	_CFG_SNMP(0x5600)	//T=u32,snmp trap using version v2c inform retry seconds.
#define CSID_C_SNMP_MTN_TRAP_V2c_INFORM_RETRY_TIMES	_CFG_SNMP(0x5700)	//T=u32,snmp trap using version v2c inform retry times.
#define CSID_C_SNMP_MTN_TRAP_V3_INFORM_ENABLE		_CFG_SNMP(0x5800)	//T=u32,snmp trap using version v3 inform enable,D=0, 0:disbale, 1:enable
#define CSID_C_SNMP_MTN_TRAP_V3_INFORM_RETRY_SEC	_CFG_SNMP(0x5900)	//T=u32,snmp trap using version v2c inform retry seconds.
#define CSID_C_SNMP_MTN_TRAP_V3_INFORM_RETRY_TIMES	_CFG_SNMP(0x6000)	//T=u32,snmp trap using version v2c inform retry times.
#define CSID_C_SNMP_MTN_TRAP_COMMUNITY				_CFG_SNMP(0x6a00)	//T=str,snmp trap community
#define CSID_C_SNMP_MTN_TRAP_V3_USERNAME			_CFG_SNMP(0x6b00)	//T=str,snmp trap using version v3 username
#define CSID_C_SNMP_MTN_TRAP_V3_PASSWORD			_CFG_SNMP(0x6c00)	//T=str,snmp trap using version v3 password
#define CSID_C_SNMP_MTN_TRAP_v3_AUTHTYPE			_CFG_SNMP(0x6d00) 	//T=u32, D=0, 0:none,1:MD5, 2:SHA1
#define CSID_C_SNMP_MTN_TRAP_v3_ENCRYPTION_MOD		_CFG_SNMP(0x6e00) 	//T=u32, D=0, 0:noAuthNoPriv,1:authNoPriv,2:authPriv
#define CSID_C_SNMP_MTN_TRAP_v3_ENCRYPTION_TYPE		_CFG_SNMP(0x6f00) 	//T=u32, D=0, 0:none, 1:DES, 2:AES
#define CSID_C_SNMP_MTN_TRAP_v3_ENCRYPTION_KEY		_CFG_SNMP(0x7000) 	//T=str, DES/AES encryption key


/* Status Items */
#define CSID_S_SNMP_ALTERED             	_STA_SNMP(0x0001) //T=u8, ui altered

/* DDP Status Items*/
#define CSID_S_SNMP_DDP_ALTERED         	_STA_SNMP(0x0002) //T=u32,  ddp alter
#define CSID_S_SNMP_DDP_RELOAD				_STA_SNMP(0x0003) //T=u32, ddp reload altered
#define CSID_S_SNMP_DDP_SENDTRAP        	_STA_SNMP(0x0004) //T=u32, 1:on -> off 2:off->on 
#define CSID_S_SNMP_DDP_TRAPMAC         	_STA_SNMP(0x0005) //T=mac, sendtrap`s mac //00-50-18-12-34-56 -> 005018123456
#define CSID_S_SNMP_DDP_TRAPINDEX       	_STA_SNMP(0x0006) //T=u32, sendtrap`s index

/*  CUSTOMER_ID       */
#define CSID_S_SNMP_CUSTOMER_ID				_STA_SNMP(0x0007) //T=u8, CUSTOMER_ID

/* SNMP  Multi flag */               
#define CSID_S_SNMP_MULTI					_STA_SNMP(0x0008) //T=u8, SNMP MULTI, using for individual enable for snmp v1,v2,v3

/*SNMP another TRAP port*/
#define CSID_S_SNMP_TRAPPORT           		_STA_SNMP(0x0009) //T=u32, D=12823, Set another trap port for SNMP trap
#define CSID_S_SNMP_SYSTEM_UPTIME			_STA_SNMP(0x0010) //T=str, record System uptime.

/* DDP Device Info */
#define CSID_S_SNMP_IPVERSION           	_STA_SNMP(0x1000) //T=u32, ipVersion
#define CSID_S_SNMP_PRODUCTNAME         	_STA_SNMP(0x1100) //T=str, productName
#define CSID_S_SNMP_FIRMEWAREVERSION    	_STA_SNMP(0x1200) //T=str, firmewareVersion
#define CSID_S_SNMP_HARDWAREVERSION     	_STA_SNMP(0x1300) //T=str, hardwareVersion 
#define CSID_S_SNMP_PRODUCTTYPE         	_STA_SNMP(0x1400) //T=u32, productType
#define CSID_S_SNMP_PRODUCTHWSPEC       	_STA_SNMP(0x1500) //T=u32, productHwSpecification
#define CSID_S_SNMP_MACADDRESS          	_STA_SNMP(0x1600) //T=mac, macAddress
#define CSID_S_SNMP_SYSTEMNAME          	_STA_SNMP(0x1700) //T=str, systemName
#define CSID_S_SNMP_SERIALNUMBER        	_STA_SNMP(0x1800) //T=str, serialNumber
#define CSID_S_SNMP_PRODUCTCATEGORY     	_STA_SNMP(0x1900) //T=str, productCategory
#define CSID_S_SNMP_DHCPSTATUS          	_STA_SNMP(0x1a00) //T=u32, dhcpStatus
#define CSID_S_SNMP_ALERTREPORT         	_STA_SNMP(0x1b00) //T=u32, 0/1 no-alert/alert happen
#define CSID_S_SNMP_IPADDRESS           	_STA_SNMP(0x1c00) //T=ipv4, ipAddress
#define CSID_S_SNMP_DDPNONDDP           	_STA_SNMP(0x1d00) //T=u32, ddp or none ddp
#define CSID_S_SNMP_WEBPORTNUMBER       	_STA_SNMP(0x1e00) //T=u32, webPortNumber
#define CSID_S_SNMP_SPECIFYCUSTDDNS     	_STA_SNMP(0x1f00) //T=u32, specifyCustomizedDns
#define CSID_S_SNMP_SUBNETMASK         	 	_STA_SNMP(0x2000) //T=ipv4, subnetMask
#define CSID_S_SNMP_DEFAULTGATEWAY      	_STA_SNMP(0x2100) //T=ipv4, defaultGateway
#define CSID_S_SNMP_PRIMARYDNS          	_STA_SNMP(0x2200) //T=ipv4, primaryDns
#define CSID_S_SNMP_SECONDARYDNS        	_STA_SNMP(0x2300) //T=ipv4, secondaryDns
#define CSID_S_SNMP_DEVICESTATUS        	_STA_SNMP(0x2400) //T=u32, node status, inactive 1, active 2
#define CSID_S_SNMP_DEVICENGTIME        	_STA_SNMP(0x2500) //T=u32, node no ack times
#define CSID_S_SNMP_DATETIME            	_STA_SNMP(0x2600) //T=str, datetime
#define CSID_S_SNMP_TIMEZONE            	_STA_SNMP(0x2700) //T=u32, timezone
#define CSID_S_SNMP_DST                 	_STA_SNMP(0x2800) //T=u32, day-light saving, 0: disable | 1:enable
#define CSID_S_SNMP_PORTNUM             	_STA_SNMP(0x2900) //T=str, port number: Port1/LAN1/WAN1/DMZ1
#define CSID_S_SNMP_SETTYPE             	_STA_SNMP(0x2a00) //T=u32,  0:dhcp 1:static

/*SNMP Multi LAN IPtable */
#define CSID_S_SNMP_MULTI_LAN_ENABLE		_STA_SNMP(0x3000) //T=u32, D=0, using for enable Multi LAN index, 1:enable, 0:disable
#define CSID_S_SNMP_MULTI_LAN_IFNAME		_STA_SNMP(0x3100) //T=str, Multi LAN IF name
#define CSID_S_SNMP_MULTI_WAN_ENABLE		_STA_SNMP(0x3200) //T=u32, D=0, using for enable Multi WAN index, 1:enable, 0:disable
#define CSID_S_SNMP_MULTI_WAN_IFNAME		_STA_SNMP(0x3300) //T=ste Multi WAN IF name

/*SNMP 5 level Trap status */
#define CSID_S_SNMP_TRAP_FWUPGRADE			_STA_SNMP(0x4000) //T=u32, monitor FW Upgrade if success, 1: success, 2:romid fail,3 :upgrade fail
#define CSID_S_SNMP_TRAP_RESET				_STA_SNMP(0x4001) //T=u32, monitor reset if success, 1:success, 2:fail 
#define CSID_S_SNMP_TRAP_PROPOSAL			_STA_SNMP(0x4002) //T=u32, monitor Proposal file if set success. 1:receive, 2:set, 3: set success, 4:set fail
#define CSID_S_SNMP_TRAP_CONFIGRESTORE		_STA_SNMP(0x4003) //T=u32, monitor config restore status, 1: success, 2:fail
#define CSID_S_SNMP_TRAP_STATUS_REBOOT		_STA_SNMP(0x4004) //T=u32, using for before rebooting trap alter
#define CSID_S_SNMP_TRAP_STATUS_RESET		_STA_SNMP(0x4005) //T=u32, using for before rest to default trap alter

/*SNMP linking information, trap information*/
#define CSID_S_SNMP_TRAP_INFO_LEVEL			_STA_SNMP(0x4100) //T=u32, record trap info level
#define CSID_S_SNMP_TRAP_INFO_EVENT			_STA_SNMP(0x4200) //T=str, record trap info event	
#define CSID_S_SNMP_TRAP_INFO_TIME			_STA_SNMP(0x4300) //T=str, record trap info receive time
#define CSID_S_SNMP_LINKINGINFO_TIME		_STA_SNMP(0x5000) //T=str, SNMP linking status time
#define CSID_S_SNMP_LINKINGINFO_VERSION		_STA_SNMP(0x5100) //T=u32, SNMP linking status version
#define CSID_S_SNMP_LINKINGINFO_COMMUNITY	_STA_SNMP(0x5200) //T=str, SNMP linking status community
#define CSID_S_SNMP_LINKINGINFO_USERNAME	_STA_SNMP(0x5300) //T=str, SNMP linking status username
#define CSID_S_SNMP_LINKINGINFO_PASSWORD	_STA_SNMP(0x5400) //T=str, SNMP linking status password
#define CSID_S_SNMP_LINKINGINFO_PRIVMODE	_STA_SNMP(0x5500) //T=u32, SNMP linking status privmode
#define CSID_S_SNMP_LINKINGINFO_AUTHMODE	_STA_SNMP(0x5600) //T=u32, SNMP linking status authmode
#define CSID_S_SNMP_LINKINGINFO_LEGAL		_STA_SNMP(0x5700) //T=u32, SNMP linking status legal
#define CSID_S_SNMP_LINKINGINFO_DESTIP		_STA_SNMP(0x5800) //T=str, SNMP linking status destip
#define CSID_S_SNMP_LINKINGINFO_PORT		_STA_SNMP(0x5900) //T=u32, SNMP linking status port

#endif //ifndef _CSID_SNMP_H
