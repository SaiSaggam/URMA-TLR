/*
 * @file Member ID Definition of Group IPV6
 *
 * Moderator: Jon
 * Group ID: 0x00900000/0x80900000
 */
#ifndef _CSID_IPV6_H
#define _CSID_IPV6_H

#include "csid_gid.h"

#define _CFG_IPV6(x)					(CSID_GIDC_IPV6|(x))
#define _STA_IPV6(x)					(CSID_GIDS_IPV6|(x))

/* Extra Definitions */
#define IPV6_CONNECTION_TYPE_LINK_LOCAL     0x0000
#define IPV6_CONNECTION_TYPE_FIXED          0x0100
#define IPV6_CONNECTION_TYPE_DHCPV6         0x0200
#define IPV6_CONNECTION_TYPE_PPPOE          0x0300
#define IPV6_CONNECTION_TYPE_6TO4           0x0400
#define IPV6_CONNECTION_TYPE_6IN4	        0x0500
#define IPV6_CONNECTION_TYPE_PPPOA	        0x0600
#define IPV6_CONNECTION_TYPE_6RD            0x0700
#define IPV6_CONNECTION_TYPE_TSP            0x0800
#define IPV6_CONNECTION_TYPE_IPV6_PASSTHROUGH            0x0900
#define IPV6_CONNECTION_TYPE_SLAAC          0x1000
#define IPV6_CONNECTION_TYPE_AUTO           0x1100

#define IPV6_AUTO_CONFIG_TYPE_STATELESS         0x00
#define IPV6_AUTO_CONFIG_TYPE_STATEFUL_DHCPv6   0x01
#define IPV6_AUTO_CONFIG_TYPE_STATELESS_DHCPv6  0x02

#define IPV6_FIX_CNT_STATUS_DISCONNECT          0x00
#define IPV6_FIX_CNT_STATUS_CONNECTING          0x01
#define IPV6_FIX_CNT_STATUS_CONNECT             0x02
#define IPV6_FIX_CNT_STATUS_DISCONNECTING       0x03

#define IPV6_DHCPV6C_CNT_STATUS_DISCONNECT      0x00
#define IPV6_DHCPV6C_CNT_STATUS_CONNECTING      0x01
#define IPV6_DHCPV6C_CNT_STATUS_CONNECT         0x02
#define IPV6_DHCPV6C_CNT_STATUS_DISCONNECTING   0x03

#define IPV6_6TO4_CNT_STATUS_DISCONNECT         0x00
#define IPV6_6TO4_CNT_STATUS_CONNECTING         0x01
#define IPV6_6TO4_CNT_STATUS_CONNECT            0x02
#define IPV6_6TO4_CNT_STATUS_DISCONNECTING      0x03

#define IPV6_6RD_CNT_STATUS_DISCONNECT          0x00
#define IPV6_6RD_CNT_STATUS_CONNECTING          0x01
#define IPV6_6RD_CNT_STATUS_CONNECT             0x02
#define IPV6_6RD_CNT_STATUS_DISCONNECTING       0x03

#define IPV6_6IN4_CNT_STATUS_DISCONNECT         0x00
#define IPV6_6IN4_CNT_STATUS_CONNECTING         0x01
#define IPV6_6IN4_CNT_STATUS_CONNECT            0x02
#define IPV6_6IN4_CNT_STATUS_DISCONNECTING      0x03

#define IPV6_PPPOE_CNT_TYPE_ONDEMAND            0x00
#define IPV6_PPPOE_CNT_TYPE_AUTORECONNECT       0x01
#define IPV6_PPPOE_CNT_TYPE_MANUAL              0x02

#define IPV6_TSP_CNT_STATUS_DISCONNECT          0x00
#define IPV6_TSP_CNT_STATUS_CONNECTING          0x01
#define IPV6_TSP_CNT_STATUS_CONNECT             0x02
#define IPV6_TSP_CNT_STATUS_DISCONNECTING       0x03
#define IPV6_TSP_CNT_STATUS_AUTH_FAIL           0x06
#define IPV6_TSP_CNT_STATUS_GENERIC_ERROR       0x07

#define IPV6_TSP_CNT_TYPE_AUTORECONNECT         0x01
#define IPV6_TSP_CNT_TYPE_MANUAL                0x02

#define IPV6_PPPOE_CNT_STATUS_DISCONNECT        0x00
#define IPV6_PPPOE_CNT_STATUS_CONNECTING        0x01
#define IPV6_PPPOE_CNT_STATUS_CONNECT           0x02
#define IPV6_PPPOE_CNT_STATUS_DISCONNECTING     0x03
#define IPV6_PPPOE_CNT_STATUS_WAIT_TRAFFIC      0x05
#define IPV6_PPPOE_CNT_STATUS_AUTH_FAIL         0x06

#define IPV6_MAP_CNT_STATUS_DISCONNECT          0x00
#define IPV6_MAP_CNT_STATUS_CONNECTING          0x01
#define IPV6_MAP_CNT_STATUS_CONNECT             0x02
#define IPV6_MAP_CNT_STATUS_DISCONNECTING       0x03

#define IPV6_MAP_SETTING_STATUS_NOT_SETTING     0x00
#define IPV6_MAP_SETTING_STATUS_SETTING         0x01
#define IPV6_MAP_SETTING_STATUS_SETTED          0x02

#define IPV6_SLAAC_CONFIG_CNT_STATUS_DISCONNECT  0x00
#define IPV6_SLAAC_CONFIG_CNT_STATUS_CONNECTING  0x01
#define IPV6_SLAAC_CONFIG_CNT_STATUS_CONNECT     0x02
#define IPV6_SLAAC_CONFIG_CNT_STATUS_DISCONNECTING   0x03

#define IPV6_AUTO_CONFIG_CNT_STATUS_DISCONNECT 0x00
#define IPV6_AUTO_CONFIG_CNT_STATUS_CONNECTING 0x01
#define IPV6_AUTO_CONFIG_CNT_STATUS_CONNECT    0x02
#define IPV6_AUTO_CONFIG_CNT_STATUS_DISCONNECTING 0x03


#define DISABLE_FLAG                            0x00
#define ENABLE_FLAG                             0x01

#define V6BUF_SIZE          64

/* Configuration Items */
#define CSID_C_IPV6_ENABLE			_CFG_IPV6(0x0000) //T=u32, D=1, ipv6 enable
#define CSID_C_IPV6_CONNECTION_TYPE		_CFG_IPV6(0x0001) //T=u32, connection type
#define CSID_C_IPV6_AUTO_CONFIG_ENABLE		_CFG_IPV6(0x0002) //T=u32, enable autoconfigure
#define CSID_C_IPV6_AUTO_CONFIG_TYPE		_CFG_IPV6(0x0003) //T=u32, auto config type
#define CSID_C_IPV6_DUALSTACK_ENABLE        	_CFG_IPV6(0x0004) //T=u32, D=0, dual stack enable
#define CSID_C_IPV6_MANUAL_DHCPPD           	_CFG_IPV6(0x0005) //T=u8,  D=0, where to get DHCP-PD, 0: DHCP Server, 1: Manually
#define CSID_C_IPV6_PASSTHROUGH		   	_CFG_IPV6(0x0006) //T=u32, D=0, ipv6 pass through enable
#define CSID_C_IPV6_DSLITE_ENABLE		_CFG_IPV6(0x0007) //T=u32, D=0, dual stack lite enable
#define CSID_C_IPV6_BIND_INTERFACE      _CFG_IPV6(0x0008) //T=u32, D=0,bind interface

/* [lan interface] */
#define CSID_C_IPV6_LAN_IF_LL_IP		    _CFG_IPV6(0x0010) //T=str, lan link-local addr
#define CSID_C_IPV6_LAN_IF_IP               _CFG_IPV6(0x0011) //T=str, lan address
#define CSID_C_IPV6_LAN_IF_PREFIX_LEN       _CFG_IPV6(0x0012) //T=u32, D=64,lan if prefix

/* [IPv6 Proxy] */
#define CSID_C_IPV6_RADVD_PREFIX              _CFG_IPV6(0x0013) //T=str, radvd prefix to advertise
#define CSID_C_IPV6_RADVD_PREFIX_LEN       _CFG_IPV6(0x0014) //T=u32, D=64,lan if prefix
#define CSID_C_IPV6_PROXY_DEFAULT_GW             _CFG_IPV6(0x0015) //T=str, default Gateway
#define CSID_C_IPV6_PROXY_VERBOSE_LOG            _CFG_IPV6(0x0016) //T=u32, D=0,

/* IPv6 IPSec */
#define CSID_C_IPV6_IPSEC_REMOTE_IP		    _CFG_IPV6(0x0013) //T=str, remote global
#define CSID_C_IPV6_IPSEC_REMOTE_SUBNET		_CFG_IPV6(0x0014) //T=str, remote global sub
#define CSID_C_IPV6_IPSEC_LOCAL_IP		    _CFG_IPV6(0x0015) //T=str, local global
#define CSID_C_IPV6_IPSEC_LOCAL_SUBNET		_CFG_IPV6(0x0016) //T=str, local global sub
#define CSID_C_IPV6_IPSEC_PRESHARE_KEY		_CFG_IPV6(0x0017) //T=str, preshare key
#define CSID_C_IPV6_IPSEC_LOCAL_NEXTHOP		_CFG_IPV6(0x0018) //T=str, leftnexthop
#define CSID_C_IPV6_IPSEC_TUNNEL_TYPE		_CFG_IPV6(0x0019) //T=u32, D=1, 0 subnet to subnet, 1 host to host
#define CSID_C_IPV6_IPSEC_TUNNEL_STATE		_CFG_IPV6(0x0020) //T=u32, D=2, 1 connected, 2 waiting, 3 disconnected, 9 connecting
#define CSID_C_IPV6_IPSEC_ENABLE		    _CFG_IPV6(0x0021) //T=u32, D=0, 0 disable ipv6 ipsec, 1 enable ipv6 ipsec

/* Customize for ZyXEL */
#define CSID_C_IPV6_CUSTOMIZE_FLAG		    _CFG_IPV6(0x0031) //T=u8, D=0, customize flag for ZyXEL

/* [DHCPv6c] */
#define CSID_C_IPV6_DHCP6C_IANA_ENABLE	    _CFG_IPV6(0x0032) //T=u32, D=0, enable iana request for widh dhcp6c

/* [fix] */
#define CSID_C_IPV6_FIX_IF_IP			    _CFG_IPV6(0x0100) //T=str, static wan ip
#define CSID_C_IPV6_FIX_IF_PREFIX_LEN   	_CFG_IPV6(0x0101) //T=u32, static prefix length
#define CSID_C_IPV6_FIX_IF_GW			    _CFG_IPV6(0x0102) //T=str, static gateway
#define CSID_C_IPV6_FIX_IF_PRIDNS		    _CFG_IPV6(0x0103) //T=str, static primary dns address
#define CSID_C_IPV6_FIX_IF_SECDNS		    _CFG_IPV6(0x0104) //T=str, static secondary dns address
#define CSID_C_IPV6_FIX_MTU			        _CFG_IPV6(0x0105) //T=u32, static mtu
#define CSID_C_IPV6_FIX_IF_LL_IP		    _CFG_IPV6(0x0106) //T=str, static link-local ip

/* [pppoe] */
#define CSID_C_IPV6_PPPOE_USER 			    _CFG_IPV6(0x0300) //T=str, pppoe user name
#define CSID_C_IPV6_PPPOE_PASSWORD 		    _CFG_IPV6(0x0301) //T=str, pppoe password
#define CSID_C_IPV6_PPPOE_SERVICENAME		_CFG_IPV6(0x0302) //T=str, pppoe service name
#define CSID_C_IPV6_PPPOE_MTU  			    _CFG_IPV6(0x0303) //T=u32, pppoe mtu
#define CSID_C_IPV6_PPPOE_IDLETIME 		    _CFG_IPV6(0x0304) //T=u32, pppoe max idle time
#define CSID_C_IPV6_PPPOE_HOLDOFFTIME 		_CFG_IPV6(0x0305) //T=u32, pppoe wait time before rebuild
#define CSID_C_IPV6_PPPOE_CNT_TYPE		    _CFG_IPV6(0x0306) //T=u32, D=1,pppoe connect type/ connect on demand - auto - manually
#define CSID_C_IPV6_PPPOE_ADDRESS_MODE		_CFG_IPV6(0x0307) //T=u32, pppoe address mode, 0 dynamic ip, 1 static ip
#define CSID_C_IPV6_PPPOE_IF_IP             _CFG_IPV6(0x0308) //T=str, pppoe static ip
#define CSID_C_IPV6_PPPOE_CNT_TYPE_UI		    _CFG_IPV6(0x0309) //T=u32, D=1,pppoe connect type/ connect on demand - auto - manually

/* [6to4] */
#define CSID_C_IPV6_6TO4_IF_PRIDNS          _CFG_IPV6(0x0400) //T=str, 6to4 interface Primary DNS
#define CSID_C_IPV6_6TO4_IF_SECDNS          _CFG_IPV6(0x0401) //T=str, 6to4 interface Second DNS
#define CSID_C_IPV6_6TO4_LAN_IP			    _CFG_IPV6(0x0402) //T=str, 6to4 lan ip adress
#define CSID_C_IPV6_6TO4_4TH_FIELD_OF_IP    _CFG_IPV6(0x0403) //T=str, the fourth field of 6TO4 LAN IP by customer

/* [IPv6 in IPv4] */
#define CSID_C_IPV6_6IN4_REMOTE_IPV4		_CFG_IPV6(0x0500) //T=ipv4, remote ipv4 address
#define CSID_C_IPV6_6IN4_REMOTE_IPV6 		_CFG_IPV6(0x0501) //T=str, remote ipv6 address
#define CSID_C_IPV6_6IN4_LOCAL_IPV4		    _CFG_IPV6(0x0502) //T=ipv4, local ipv4 address
#define CSID_C_IPV6_6IN4_LOCAL_IPV6		    _CFG_IPV6(0x0503) //T=str, local ipv6 address
#define CSID_C_IPV6_6IN4_IF_PRIDNS          _CFG_IPV6(0x0504) //T=str, 6in4 interface Primary DNS
#define CSID_C_IPV6_6IN4_IF_SECDNS          _CFG_IPV6(0x0505) //T=str, 6in4 interface Second DNS

/* [DNS] */
#define CSID_C_IPV6_DNS_IF_PRIDNS           _CFG_IPV6(0x0600) //T=str, interface Primary DNS
#define CSID_C_IPV6_DNS_IF_SECDNS           _CFG_IPV6(0x0601) //T=str, interface Second DNS 
#define CSID_C_IPV6_DNS_CONFIG              _CFG_IPV6(0x0602) //T=u32, 0:auto config, 1: manually config
#define CSID_C_IPV6_DNS_IF_AUTODNS          _CFG_IPV6(0x0603) //T=str, interface automatically DNS

/* [6rd] */
#define CSID_C_IPV6_6RD_IF_PRIDNS           _CFG_IPV6(0x0700) //T=str, 6rd interface Primary DNS
#define CSID_C_IPV6_6RD_IF_SECDNS           _CFG_IPV6(0x0701) //T=str, 6rd interface Second DNS
#define CSID_C_IPV6_6RD_LAN_IP              _CFG_IPV6(0x0702) //T=str, 6rd lan ip adress
#define CSID_C_IPV6_6RD_BORDER_PREFIX       _CFG_IPV6(0x0703) //T=str, 6rd border prefix
#define CSID_C_IPV6_6RD_BORDER_IPV4         _CFG_IPV6(0x0704) //T=str, 6rd border ipv4
#define CSID_C_IPV6_6RD_IPV4_MASK_LEN       _CFG_IPV6(0x0705) //T=u32, 6rd prefix length
#define CSID_C_IPV6_6RD_BORDER_PREFIX_LEN   _CFG_IPV6(0x0706) //T=u32, 6rd ipv4 mask length 

/* [DSLite] */
#define CSID_C_IPV6_DSLITE_AFTR_ADDR_MODE	_CFG_IPV6(0x0800) //T=u8, AFTR addr mode, 0:dynamic, 1: static
#define CSID_C_IPV6_DSLITE_AFTR_ADDR		_CFG_IPV6(0x0801) //T=str, DSLite AFTR address

/* [MAP] */
#define CSID_C_IPV6_MAP_ENABLE              _CFG_IPV6(0x0900) //T=u32, D=0, 0:disable, 1:enable
#define CSID_C_IPV6_MAP_TYPE                _CFG_IPV6(0x0901) //T=u32, D=0, 0:MAP-E, 1:MAP-T
#define CSID_C_IPV6_MAP_ROLE                _CFG_IPV6(0x0902) //T=u32, D=0, 0:CE, 1:BR
#define CSID_C_IPV6_MAP_BR_ADDR             _CFG_IPV6(0x0903) //T=str, the BR IPv6 address
#define CSID_C_IPV6_MAP_IPV6_FIXLEN         _CFG_IPV6(0x0904) //T=u32, End-user IPv6 prefix length
#define CSID_C_IPV6_MAP_RULE_NUMBER         _CFG_IPV6(0x0905) //T=u32, the number of Map rule number
#define CSID_C_IPV6_MAP_DRAFT_VERSION       _CFG_IPV6(0x0906) //T=u32, D=3, the map draft version
#define CSID_C_RULE_ID                      _CFG_IPV6(0x0907) //T=str, the ID of getting MAP rule
#define CSID_C_IPV6_MAP_UPDATE_BASE1        _CFG_IPV6(0x0910) //T=u32, D=60, base time to update MAP rule (time1)
#define CSID_C_IPV6_MAP_UPDATE_LONG1        _CFG_IPV6(0x0911) //T=u32, D=540, long time to update MAP rule (time1)
#define CSID_C_IPV6_MAP_UPDATE_BASE2        _CFG_IPV6(0x0912) //T=u32, D=10800, base time to update MAP rule (time2)
#define CSID_C_IPV6_MAP_UPDATE_LONG2        _CFG_IPV6(0x0913) //T=u32, D=75600, long time to update MAP rule (time2)
#define CSID_C_IPV6_MAP_RULE_IPV4_PREFIX    _CFG_IPV6(0x0a00) //T=str, Rule IPv4 prefix, reserve for 256 sets (0xa00-0xaff)
#define CSID_C_IPV6_MAP_RULE_IPV6_PREFIX    _CFG_IPV6(0x0b00) //T=str, Rule IPv6 prefix, reserve for 256 sets (0xb00-0xbff)
#define CSID_C_IPV6_MAP_EA_BIT              _CFG_IPV6(0x0c00) //T=u32, EA-bits length, reserve for 256 sets (0xc00-0xcff)
#define CSID_C_IPV6_MAP_PSID_OFFSET         _CFG_IPV6(0x0d00) //T=u32, PSID offset, reserve for 256 sets (0xd00-0xdff)

/* [DHCPv6s] */
#define CSID_C_IPV6_DHCPV6S_START_IP		    _CFG_IPV6(0x1000) //T=str, ipv6 pool start
#define CSID_C_IPV6_DHCPV6S_END_IP              _CFG_IPV6(0x1001) //T=str, ipv6 pool end
#define CSID_C_IPV6_DHCPV6S_PREFERRED_LIFETIME	_CFG_IPV6(0x1002) //T=u32, preferred lift time
#define CSID_C_IPV6_DHCPV6S_VALID_LIFETIME      _CFG_IPV6(0x1003) //T=u32, valid lift time
#define CSID_C_IPV6_DHCPV6S_RENEW_TIME          _CFG_IPV6(0x1004) //T=u32, renew time (T1)
#define CSID_C_IPV6_DHCPV6S_REBIND_TIME         _CFG_IPV6(0x1005) //T=u32, rebind time (T2)
#define CSID_C_IPV6_DHCPV6S_PRIDNS              _CFG_IPV6(0x1006) //T=str, Primary DNS
#define CSID_C_IPV6_DHCPV6S_SECDNS              _CFG_IPV6(0x1007) //T=str, Second DNS

/* [radvd] */
#define CSID_C_IPV6_RADVD_PREFERRED_LIFETIME	_CFG_IPV6(0x1100) //T=u32, preferred lift time
#define CSID_C_IPV6_RADVD_VALID_LIFETIME        _CFG_IPV6(0x1101) //T=u32, valid lift time

/* [TSP] */
#define CSID_C_IPV6_TSP_SERVER                  _CFG_IPV6(0x1200) //T=str, Tunnel Broker server
#define CSID_C_IPV6_TSP_USERNAME                _CFG_IPV6(0x1210) //T=str, Auth username
#define CSID_C_IPV6_TSP_PASSWORD                _CFG_IPV6(0x1211) //T=str, Auth password
#define CSID_C_IPV6_TSP_ANONYMOUS               _CFG_IPV6(0x1212) //T=u32, D=0, use anonymous or username

/* */
#define CSID_C_IPV6_VERBOSE_LOG                 _CFG_IPV6(0x1300) //T=u32, D=0, enable IPv6 verbose log

/* ipv6 firewall */
#define CSID_C_IPV6_FILT_ENABLE		   	        _CFG_IPV6(0x1301) //T=u32, D=0, ipv6 firewall enable
#define CSID_C_IPV6_FILT_ADVANCE           	    _CFG_IPV6(0x1302) //T=u32, D=0, 0 for deny all incom
#define CSID_C_IPV6_FILT_ALTERED                _CFG_IPV6(0x1303) //T=u32, ui altered
#define CSID_C_IPV6_FILT_HOSTNAME               _CFG_IPV6(0x1400) //T=str, The hostname of the Client
#define CSID_C_IPV6_FILT_IF_MODE                _CFG_IPV6(0x1410) //T=u8,  0=wired, 1=wireless
#define CSID_C_IPV6_FILT_MAC                    _CFG_IPV6(0x1420) //T=mac, The Mac Address of the Client
#define CSID_C_IPV6_FILT_ACT_MODE               _CFG_IPV6(0x1430) //T=u32, D=0, value= 0,1,2|deny,accept,user
#define CSID_C_IPV6_FILT_CLI_ENABLE             _CFG_IPV6(0x1500) //T=u32, D=0, firewall client rule enable
#define CSID_C_IPV6_FILT_CLI_INOUT              _CFG_IPV6(0x1600) //T=u32, D=0, 0 in, 1 out
#define CSID_C_IPV6_FILT_CLI_PROTOCOL           _CFG_IPV6(0x1700) //T=u32, D=0, 0,1 | tcp, udp
#define CSID_C_IPV6_FILT_CLI_SRC_IP             _CFG_IPV6(0x1800) //T=str, source ip for client rule
#define CSID_C_IPV6_FILT_CLI_SRC_PORT           _CFG_IPV6(0x1900) //T=u32, source port for client rule
#define CSID_C_IPV6_FILT_CLI_DEST_IP            _CFG_IPV6(0x1a00) //T=str, dest ip for client rule
#define CSID_C_IPV6_FILT_CLI_DEST_PORT          _CFG_IPV6(0x1b00) //T=u32, dest port for client rule
#define CSID_C_IPV6_FILT_CLI_ACT_TYPE           _CFG_IPV6(0x1c00) //T=u32, D=0, value= 0,1|accept, deny
#define CSID_C_IPV6_FILT_RULE_SCHE_NO            _CFG_IPV6(0x1d00) //T=u16, number of using rule
#define CSID_C_IPV6_FILT_SPORT                   _CFG_IPV6(0x1e00) //T=str, source port start range
#define CSID_C_IPV6_FILT_DPORT                   _CFG_IPV6(0x1f00) //T=str, destination port start range
#define CSID_C_IPV6_FILT_DENY_ALL                _CFG_IPV6(0x2001) //T=u8, deny all except any rule is matched

#define CSID_C_IPV6_FILT_IN_IF                   _CFG_IPV6(0x2100) //T=u8, source interface, 0:wan , 1:lan
#define CSID_C_IPV6_FILT_OUT_IF                  _CFG_IPV6(0x2200) //T=u8, destination interface, 0:wan , 1:lan
#define CSID_C_IPV6_FILT_SRC_END_IP              _CFG_IPV6(0x2300) //T=str, source ip end range
#define CSID_C_IPV6_FILT_DEST_END_IP             _CFG_IPV6(0x2400) //T=str, destination ip end range
#define CSID_C_IPV6_FILT_END_SPORT               _CFG_IPV6(0x2500) //T=str, source port end range
#define CSID_C_IPV6_FILT_END_DPORT               _CFG_IPV6(0x2600) //T=str, distination port end range
#define CSID_C_IPV6_FILT_RULE_NAME               _CFG_IPV6(0x2700) //T=str, rule name

/* autoconfiguration */
#define CSID_C_IPV6_CHECK_RA_ENABLE              _CFG_IPV6(0x2800) //T=u8, D=0, when router receives RA, check the mo-bit of RA for ipv6 address autoconfiguration


/* Status Items */
#define CSID_S_IPV6_IF_IP              		    _STA_IPV6(0x0000) //T=str, interface ip
#define CSID_S_IPV6_IF_PREFIX_LEN         	    _STA_IPV6(0x0001) //T=u32, prefix
#define CSID_S_IPV6_IF_GW                 	    _STA_IPV6(0x0002) //T=str, interface gateway
#define CSID_S_IPV6_IF_PRIDNS                   _STA_IPV6(0x0003) //T=str, Primary DNS
#define CSID_S_IPV6_IF_SECDNS                   _STA_IPV6(0x0004) //T=str, Second DNS 
#define CSID_S_IPV6_MTU                         _STA_IPV6(0x0005) //T=u32, mtu
#define CSID_S_IPV6_IF_LL_IP                    _STA_IPV6(0x0006) //T=str, link-local address
#define CSID_S_IPV6_IF_LL_GW_IP                 _STA_IPV6(0x0007) //T=str, gateway link-local address
#define CSID_S_IPV6_LAN_IF_LL_IP                _STA_IPV6(0x0008) //T=str, lan link-local address
#define CSID_S_IPV6_LAN_IF_IP			        _STA_IPV6(0x0009) //T=str, lan address
#define CSID_S_IPV6_LAN_IF_PREFIX_LEN           _STA_IPV6(0x000A) //T=u32, D=64,lan if prefix
#define CSID_S_IPV6_LAN_IF_PREFIX		        _STA_IPV6(0x000B) //T=str, lan prefix
#define CSID_S_IPV6_IF_PREFIX                   _STA_IPV6(0x000C) //T=str, ipv6 prefix
#define CSID_S_IPV6_FIX_CNT_STATUS              _STA_IPV6(0x000D) //T=u32, connect status 

#define CSID_S_IPV6_PPPOE_CIF                   _STA_IPV6(0x0020) //T=str, pppoe connection`s IF
#define CSID_S_IPV6_PPPOE_LINK                  _STA_IPV6(0x0021) //T=str, pppoe connection`s link ip
#define CSID_S_IPV6_PPPOE_GLOBAL                _STA_IPV6(0x0022) //T=str, pppoe connection`s global ip
#define CSID_S_IPV6_PPPOE_CNT_STATUS            _STA_IPV6(0x0030) //T=u32, connect status



/* UI altered */
#define CSID_S_IPV6_ALTERED       		        _STA_IPV6(0x0010) //T=u32, ui altered
#define CSID_S_IPV6_FILT_ALTERED                _STA_IPV6(0x0011) //T=u32, ui altered for ipv6 filter
#define CSID_S_IPV6_CNT_FORCE_STOP              _STA_IPV6(0x0012) //T=u32, ui altered for stoping ipv6

/* [MAP] */
#define CSID_S_IPV6_MAP_RULE_IPV4_PREFIX        _STA_IPV6(0x0a00) //T=str, Rule IPv4 prefix, reserve for 256 sets (0xa00-0xaff)
#define CSID_S_IPV6_MAP_RULE_IPV6_PREFIX        _STA_IPV6(0x0b00) //T=str, Rule IPv6 prefix, reserve for 256 sets (0xb00-0xbff)
#define CSID_S_IPV6_MAP_EA_BIT                  _STA_IPV6(0x0c00) //T=u32, EA-bits length, reserve for 256 sets (0xc00-0xcff)
#define CSID_S_IPV6_MAP_PSID_OFFSET             _STA_IPV6(0x0d00) //T=u32, PSID offset, reserve for 256 sets (0xd00-0xdff)
#define CSID_S_IPV6_MAP_WAN_IPV6_ADDRESS        _STA_IPV6(0x0e00) //T=str, the full IPv6 address of WAN
#define CSID_S_IPV6_MAP_WAN_IPV6_GATEWAY        _STA_IPV6(0x0e01) //T=str, the default gateway for IPv6 address of WAN
#define CSID_S_IPV6_MAP_CONNECT_STATUS          _STA_IPV6(0x0e02) //T=u32, the MAP connect status, 0: disconnect, 1: connecting, 2: connected
#define CSID_S_IPV6_MAP_PROXY_ADDRESS           _STA_IPV6(0x0e03) //T=str, the proxy address for MAP that created
#define CSID_S_IPV6_MAP_IPV4_FOR_GO_OUT         _STA_IPV6(0x0e04) //T=ipv4, the ipv4 address calculated by MAP-Rule for going to IPv4 network
#define CSID_S_IPV6_MAP_IPV6_FOR_GO_OUT         _STA_IPV6(0x0e05) //T=str, the ipv6 address calculated by MAP-Rule for going to IPv6 network
#define CSID_S_IPV6_MAP_SETTING_STATUS          _STA_IPV6(0x0e06) //T=u32, the status of setting of MAP (0:not yet, 1: setting, 2:     complete)

/* [DHCPv6s] */
#define CSID_S_IPV6_DHCPV6S_PREFERRED_LIFETIME  _STA_IPV6(0x1002) //T=u32, preferred lift time
#define CSID_S_IPV6_DHCPV6S_VALID_LIFETIME      _STA_IPV6(0x1003) //T=u32, valid lift time
#define CSID_S_IPV6_DHCPV6S_RENEW_TIME          _STA_IPV6(0x1004) //T=u32, renew time (T1)
#define CSID_S_IPV6_DHCPV6S_REBIND_TIME         _STA_IPV6(0x1005) //T=u32, rebind time (T2)
#define CSID_S_IPV6_DHCPV6S_FULL_START_IP       _STA_IPV6(0x1006) //T=str, full ipv6 pool start address
#define CSID_S_IPV6_DHCPV6S_FULL_END_IP         _STA_IPV6(0x1007) //T=str, full ipv6 pool end address

/* [radvd] */
#define CSID_S_IPV6_RADVD_PREFERRED_LIFETIME    _STA_IPV6(0x1100) //T=u32, preferred lift time
#define CSID_S_IPV6_RADVD_VALID_LIFETIME        _STA_IPV6(0x1101) //T=u32, valid lift time
#define CSID_S_IPV6_RADVD_MANAGED_FLAG          _STA_IPV6(0x1102) //T=u32, AdvManagedFlag 
#define CSID_S_IPV6_RADVD_OTHERCONFIG_FLAG      _STA_IPV6(0x1103) //T=u32, AdvOtherConfigFlag 

/* [pppoe] */
#define CSID_S_IPV6_PPPOE_IF_ID			        _STA_IPV6(0x0300)

/* [AUTO Configure] */
#define CSID_S_IPV6_AUTO_CONFIG_IF_NAME        _STA_IPV6(0x0500) //T=str, AUTO client interface
#define CSID_S_IPV6_AUTO_CONFIG_CNT_STATUS     _STA_IPV6(0x0501) //T=u32, 0: disconnected, 1: connecting, 2:connected
#define CSID_S_IPV6_AUTO_CONFIG_IP             _STA_IPV6(0x0502) //T=str, the IPv6 address of AUTO-CONFIG
#define CSID_S_IPV6_AUTO_CONFIG_TYPE           _STA_IPV6(0x0503) //T=u32, D=0, 0: STATELESS, 1: STATEFUL_DHCPv6, 2: STATELESS_DHCPv6

/* [SLAAC Configure] */
#define CSID_S_IPV6_SLAAC_CONFIG_IF_NAME         _STA_IPV6(0x0600) //T=str, SLAAC-CONFIG client interface
#define CSID_S_IPV6_SLAAC_CONFIG_CNT_STATUS      _STA_IPV6(0x0601) //T=u32, 0: disconnected, 1: connecting, 2:connected
#define CSID_S_IPV6_SLAAC_CONFIG_IP              _STA_IPV6(0x0602) //T=str, the IPv6 address of SLAAC-CONFIG

/* [DHCPv6c] */
#define CSID_S_IPV6_DHCPV6C_IF_NAME		        _STA_IPV6(0x0700) //T=str, dhcpv6 client interface
#define CSID_S_IPV6_DHCPV6C_CNT_STATUS		    _STA_IPV6(0x0701) //T=u32, connect status

/* [6to4] */
#define CSID_S_IPV6_6TO4_CNT_STATUS		        _STA_IPV6(0x0801) //T=u32, connect status
#define CSID_S_IPV6_6TO4_PRIVATE_IP		        _STA_IPV6(0x0802) //T=u32, D=0, private ip flag
#define CSID_S_IPV6_6TO4_48BIT_PREFIX           _STA_IPV6(0x0803) //T=str, the first 48bit of the LAN IPv6 Global address
#define CSID_S_IPV6_6TO4_INTERFACE_ID           _STA_IPV6(0x0804) //T=str, the interface id of the LAN IPv6 Global address

/* [6in4] */
#define CSID_S_IPV6_6IN4_CNT_STATUS		        _STA_IPV6(0x0901) //T=u32, connect status
#define CSID_S_IPV6_6IN4_PRIVATE_IP	            _STA_IPV6(0x0902) //T=u32, D=0, private ip flag

/* [6rd] */
#define CSID_S_IPV6_6RD_CNT_STATUS              _STA_IPV6(0x0a01) //T=u32, connect status
#define CSID_S_IPV6_6RD_PRIVATE_IP              _STA_IPV6(0x0a02) //T=u32, D=0, private ip flag

/* [TSP] */
#define CSID_S_IPV6_TSP_CNT_STATUS              _STA_IPV6(0x0b00) //T=u32, connect status

/* [IPV6 WAN] */
#define CSID_S_IPV6_WAN_IF_NAME			        _STA_IPV6(0x2000) //T=str, wan interface name
#define CSID_S_IPV6_PROXY_WAN_IF_NAME			_STA_IPV6(0x2001) //T=str, ipv6 proxy wan interface name

/* Status Items */
#define CSID_S_IPV6_FILT_RULE_NUM             _STA_IPV6(0x2001) //T=u16, number of pf_inbound rules

#define CSID_S_IPV6_FILT_RULE_SCHE_STATUS     _STA_IPV6(0x2100) //T=u8, Out schedule rule status

/* Autoconfiguration*/
#define CSID_S_IPV6_RA_MOBIT_STATUS     _STA_IPV6(0x2200) //T=u8, mo-bit of RA
#define CSID_S_IPV6_IA_NA_ADDRESS       _STA_IPV6(0x2201) //T=str, IA_NA address
#define CSID_S_IPV6_RA_A_BIT_STATUS     _STA_IPV6(0x2202) //T=u32,  autonomous bit of RA

/* Configuration Items */
#define CSID_S_IPV6_NOT_REQUEST_DHCPPD  _STA_IPV6(0x2300) //T=u32, D=0, request DHCP-PD or not, 0: request, 1: NOT request

#endif //ifndef _CSID_IPV6_H


