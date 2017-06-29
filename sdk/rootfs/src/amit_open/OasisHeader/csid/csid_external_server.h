/*
 * @file Member ID Definition of Group External Server.
 *
 * Moderator:
 * Group ID: 0x00B10000/0x80B10000
 */

#ifndef _CSID_EXTERNAL_SERVER_H_
#define _CSID_EXTERNAL_SERVER_H_

#include "csid_gid.h"

#define _CFG_EXTERNAL_SERVER(x) (CSID_GIDC_EXTERNAL_SERVER|(x))
#define _STA_EXTERNAL_SERVER(x) (CSID_GIDS_EXTERNAL_SERVER|(x))

/* Configuration Items */
//External server list, reserve for 64 entries
#define CSID_C_EXTSERV_NAME                         _CFG_EXTERNAL_SERVER(0x0000)   //T=str, external server name
#define CSID_C_EXTSERV_IP                           _CFG_EXTERNAL_SERVER(0x0040)   //T=str, external server IP/FQDN
#define CSID_C_EXTSERV_PORT                         _CFG_EXTERNAL_SERVER(0x0080)   //T=u32, external server port
#define CSID_C_EXTSERV_TYPE                         _CFG_EXTERNAL_SERVER(0x00C0)   //T=u32, external server type,1: email,2: syslog,3: radius,4: active directory,5: LDAP,6: NT domain, 7: UAM, 8: TACACS+, 9:SCEP
#define CSID_C_EXTSERV_ENABLE                       _CFG_EXTERNAL_SERVER(0x0100)   //T=u32, external server enable
#define CSID_C_EXTSERV_EMAIL_USER                   _CFG_EXTERNAL_SERVER(0x0140)   //T=str, email server username
#define CSID_C_EXTSERV_EMAIL_PASS                   _CFG_EXTERNAL_SERVER(0x0180)   //T=str, email server password
#define CSID_C_EXTSERV_RADIUS_PRI_KEY               _CFG_EXTERNAL_SERVER(0x01C0)   //T=str, radius server primary shared key
#define CSID_C_EXTSERV_RADIUS_PRI_PROTOCOL          _CFG_EXTERNAL_SERVER(0x0200)   //T=u32, radius server primary authentication protocol, 0: CHAP, 1: PAP
#define CSID_C_EXTSERV_RADIUS_PRI_SESSION_TIMEOUT   _CFG_EXTERNAL_SERVER(0x0280)   //T=u32, radius server primary session timeout
#define CSID_C_EXTSERV_RADIUS_PRI_IDLE_TIMEOUT      _CFG_EXTERNAL_SERVER(0x02C0)   //T=u32, radius server primary idle timeout
#define CSID_C_EXTSERV_RADIUS_SEC_KEY               _CFG_EXTERNAL_SERVER(0x0300)   //T=str, radius server secondary shared key
#define CSID_C_EXTSERV_RADIUS_SEC_PROTOCOL          _CFG_EXTERNAL_SERVER(0x0340)   //T=u32, radius server secondary authentication protocol, 0:CHAP, 1: PAP
#define CSID_C_EXTSERV_RADIUS_SEC_SESSION_TIMEOUT   _CFG_EXTERNAL_SERVER(0x03C0)   //T=u32, radius server secondary session timeout
#define CSID_C_EXTSERV_RADIUS_SEC_IDLE_TIMEOUT      _CFG_EXTERNAL_SERVER(0x0400)   //T=u32, radius server secondary idle timeout
#define CSID_C_EXTSERV_AD_DOMAIN                    _CFG_EXTERNAL_SERVER(0x0440)   //T=str, active directory server domain
#define CSID_C_EXTSERV_LDAP_BASEDN                  _CFG_EXTERNAL_SERVER(0x0480)   //T=str, LDAP server base DN
#define CSID_C_EXTSERV_NTDOMAIN_WORKGROUP           _CFG_EXTERNAL_SERVER(0x04C0)   //T=str, NT domain server workgroup
#define CSID_C_EXTSERV_UAM_SHARED_SECRET            _CFG_EXTERNAL_SERVER(0x0500)   //T=str, UAM server shared secret
#define CSID_C_EXTSERV_UAM_GATEWAY_ID               _CFG_EXTERNAL_SERVER(0x0540)   //T=str, UAM server NAS/gateway ID
#define CSID_C_EXTSERV_CATEGORY                     _CFG_EXTERNAL_SERVER(0x0580)   //T=u8, D=0, 0:internal server,1:external server 
#define CSID_C_EXTSERV_ID                           _CFG_EXTERNAL_SERVER(0x05C0)   //T=u32, D=0,index to keep for UI display ID.
#define CSID_C_EXTSERV_UAM_LOC_ID                   _CFG_EXTERNAL_SERVER(0x0600)   //T=u32, D=0,index to keep for location id on HotSpotSystems
#define CSID_C_EXTSERV_UAM_LOC_NAME                 _CFG_EXTERNAL_SERVER(0x0640)   //T=str, D="",optional name of location name on HotSpotSystems
#define CSID_C_EXTSERV_UAM_FORMAT                   _CFG_EXTERNAL_SERVER(0x0680)   //T=str, D="",HS_UAMFORMAT login form including the login php script
#define CSID_C_EXTSERV_LDAP_ID                      _CFG_EXTERNAL_SERVER(0x06C0)   //T=str, LDAP server identity
#define CSID_C_EXTSERV_LDAP_PASSWD                  _CFG_EXTERNAL_SERVER(0x0700)   //T=str, LDAP server password
#define CSID_C_EXTSERV_RADIUS_PRI_IP          		_CFG_EXTERNAL_SERVER(0x0740)   //T=str, radius server primary ip
#define CSID_C_EXTSERV_RADIUS_PRI_PORT            	_CFG_EXTERNAL_SERVER(0x0780)   //T=u32, D=1812, radius server primary port
#define CSID_C_EXTSERV_RADIUS_PRI_NAS_PORT          _CFG_EXTERNAL_SERVER(0x07C0)   //T=u32, radius server primary nas port
#define CSID_C_EXTSERV_RADIUS_PRI_NAS_ID            _CFG_EXTERNAL_SERVER(0x0800)   //T=str, radius server primary nas id
#define CSID_C_EXTSERV_RADIUS_SEC_IP          		_CFG_EXTERNAL_SERVER(0x0840)   //T=str, radius server secondary ip
#define CSID_C_EXTSERV_RADIUS_SEC_PORT            	_CFG_EXTERNAL_SERVER(0x0880)   //T=u32, D=1812, radius server secondary port
#define CSID_C_EXTSERV_RADIUS_SEC_NAS_PORT          _CFG_EXTERNAL_SERVER(0x08C0)   //T=u32, radius server secondary nas port
#define CSID_C_EXTSERV_RADIUS_SEC_NAS_ID            _CFG_EXTERNAL_SERVER(0x0900)   //T=str, radius server secondary nas id
#define CSID_C_EXTSERV_RADIUS_PRI_ACCOUNTING_IP          		_CFG_EXTERNAL_SERVER(0x0940)   //T=str, radius server accounting primary ip
#define CSID_C_EXTSERV_RADIUS_PRI_ACCOUNTING_PORT   			_CFG_EXTERNAL_SERVER(0x0980)   //T=u32, D=1813, radius server accounting primary port
#define CSID_C_EXTSERV_RADIUS_PRI_ACCOUNTING_NAS_ID         	_CFG_EXTERNAL_SERVER(0x09C0)   //T=str, radius server accounting primary nas id
#define CSID_C_EXTSERV_RADIUS_PRI_ACCOUNTING_NAS_PORT       	_CFG_EXTERNAL_SERVER(0x0A00)   //T=u32, radius server accounting primary nas port
#define CSID_C_EXTSERV_RADIUS_PRI_ACCOUNTING_SESSION_TIMEOUT	_CFG_EXTERNAL_SERVER(0x0A40)   //T=u32, radius server accounting primary session timeout
#define CSID_C_EXTSERV_RADIUS_PRI_ACCOUNTING_IDLE_TIMEOUT		_CFG_EXTERNAL_SERVER(0x0A80)   //T=u32, radius server accounting primary idle timeout
#define CSID_C_EXTSERV_RADIUS_SEC_ACCOUNTING_IP          		_CFG_EXTERNAL_SERVER(0x0AC0)   //T=str, radius server accounting secondary ip
#define CSID_C_EXTSERV_RADIUS_SEC_ACCOUNTING_PORT   			_CFG_EXTERNAL_SERVER(0x0B00)   //T=u32, D=1813, radius server accounting secondary port
#define CSID_C_EXTSERV_RADIUS_SEC_ACCOUNTING_NAS_ID         	_CFG_EXTERNAL_SERVER(0x0B40)   //T=str, radius server accounting secondary nas id
#define CSID_C_EXTSERV_RADIUS_SEC_ACCOUNTING_NAS_PORT       	_CFG_EXTERNAL_SERVER(0x0B80)   //T=u32, radius server accounting secondary nas port
#define CSID_C_EXTSERV_RADIUS_SEC_ACCOUNTING_SESSION_TIMEOUT	_CFG_EXTERNAL_SERVER(0x0BC0)   //T=u32, radius server accounting secondary session timeout
#define CSID_C_EXTSERV_RADIUS_SEC_ACCOUNTING_IDLE_TIMEOUT		_CFG_EXTERNAL_SERVER(0x0C00)   //T=u32, radius server accounting secondary idle timeout
#define CSID_C_EXTSERV_TACSCS_IP								_CFG_EXTERNAL_SERVER(0x0C40)   //T=str, TACACS+ primary server ip
#define CSID_C_EXTSERV_TACACS_PORT								_CFG_EXTERNAL_SERVER(0x0C80)   //T=u32, D=49, TACACS+ primary server port
#define CSID_C_EXTSERV_TACSCS_KEY                   			_CFG_EXTERNAL_SERVER(0x0CC0)   //T=str, TACACS+ primary server key
#define CSID_C_EXTSERV_TACACS_TIMEOUT               			_CFG_EXTERNAL_SERVER(0x0D00)   //T=u32, TACACS+ primary server timeout
#define CSID_C_EXTSERV_TACSCS_SEC_IP                   			_CFG_EXTERNAL_SERVER(0x0D40)   //T=str, TACACS+ secondary server ip
#define CSID_C_EXTSERV_TACACS_SEC_PORT               			_CFG_EXTERNAL_SERVER(0x0D80)   //T=u32, D=49, TACACS+ secondary server port
#define CSID_C_EXTSERV_TACSCS_SEC_KEY                   		_CFG_EXTERNAL_SERVER(0x0DC0)   //T=str, TACACS+ secondary server key
#define CSID_C_EXTSERV_TACACS_SEC_TIMEOUT               		_CFG_EXTERNAL_SERVER(0x0E00)   //T=u32, TACACS+ secondary server timeout
#define CSID_C_EXTSERV_SCEP_PATH			               		_CFG_EXTERNAL_SERVER(0x0E40)   //T=str, D="cgi-bin", scep server path
#define CSID_C_EXTSERV_SCEP_APPLICATION		              		_CFG_EXTERNAL_SERVER(0x0E80)   //T=str, D="pkiclient.exe", scep server applicatoin name

#define CSID_C_EXTSERV_UAM_ENABLE                   _CFG_EXTERNAL_SERVER(0x1000)    //T=u32, D=0, whether to enable the UAM server
#define CSID_C_EXTSERV_UAM_IDX                      _CFG_EXTERNAL_SERVER(0x1001)    //T=u32, D=0, the relative UAM id at external server list.
#define CSID_C_EXTSERV_RADIUS1                      _CFG_EXTERNAL_SERVER(0x1002)   //T=str, radius server primary
#define CSID_C_EXTSERV_RADIUS2                      _CFG_EXTERNAL_SERVER(0x1003)   //T=str, radius server secondary
//for convenience of Uygher script mapping between  the external server and UAM server of captive portal config defaults.config
//we adjust the CSID at commander before generating the defaults.config
#define CSID_C_EXTSERV_UAM_GATEWAY_ID_TMP           _CFG_EXTERNAL_SERVER(0x1004)   //T=str, UAM server NAS/gateway ID
#define CSID_C_EXTSERV_UAM_LOC_ID_TMP                   _CFG_EXTERNAL_SERVER(0x1005)   //T=u32, D=0,index to keep for location id on HotSpotSystems
#define CSID_C_EXTSERV_UAM_LOC_NAME_TMP                 _CFG_EXTERNAL_SERVER(0x1006)   //T=str, D="",optional name of location name on HotSpotSystems
#define CSID_C_EXTSERV_UAM_SHARED_SECRET_TMP        _CFG_EXTERNAL_SERVER(0x1007)   //T=str, UAM server shared secret
#define CSID_C_EXTSERV_UAM_IP_TMP                   _CFG_EXTERNAL_SERVER(0x1008)   //T=str, external UAM server IP/FQDN
#define CSID_C_EXTSERV_RADIUS_PRI_KEY_TMP           _CFG_EXTERNAL_SERVER(0x1009)   //T=str, radius server primary shared key
#define CSID_C_EXTSERV_UAM_FORMAT_TMP               _CFG_EXTERNAL_SERVER(0x100A)   //T=str, D="",HS_UAMFORMAT login form including the login php script

/* Status Items */
//external server rule action, reserve 64 entry
#define CSID_S_EXTSERV_ACTION       _STA_EXTERNAL_SERVER(0x0000)   //T=u8, D=0, server manipulation, 0:none, 1:add, 2:del, 3:edit
#define CSID_S_EXTSERV_REMOVABLE    _STA_EXTERNAL_SERVER(0x0040)   //T=u8, D=0, check if server can be deleted, 0:no, 1:ok

#endif
