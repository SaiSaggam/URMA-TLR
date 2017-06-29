/*
 * @file Member ID Definition of Group External Server.
 *
 * Moderator:
 * Group ID: 0x00B00000/0x80B00000
 */

#ifndef _CSID_CAPTIVE_PORTAL_H_
#define _CSID_CAPTIVE_PORTAL_H_

#include "csid_gid.h"

#define _CFG_CAPTIVE_PORTAL(x) (CSID_GIDC_CAPTIVE_PORTAL|(x))
#define _STA_CAPTIVE_PORTAL(x) (CSID_GIDS_CAPTIVE_PORTAL|(x))

/* Configuration Items */
#define CSID_C_CAPTIVE_PORTAL_ENABLE        _CFG_CAPTIVE_PORTAL(0x0002)	//T=u8, D=0, enable or disable
#define CSID_C_CAPTIVE_PORTAL_NETWORK       _CFG_CAPTIVE_PORTAL(0x0003)	//T=str, network ip
#define CSID_C_CAPTIVE_PORTAL_WAN_IF        _CFG_CAPTIVE_PORTAL(0x0004)	//T=str, wan if
#define CSID_C_CAPTIVE_PORTAL_WAN_SELECT    _CFG_CAPTIVE_PORTAL(0x0005)	//T=str, wan select
#define CSID_C_CAPTIVE_PORTAL_UAMALLOW      _CFG_CAPTIVE_PORTAL(0x0006)	//T=str, accessable white list even if user haven't logined(: seperated)
#define CSID_C_CAPTIVE_PORTAL_UAMFORMAT     _CFG_CAPTIVE_PORTAL(0x0007)	//T=str, define the actual captive portal url
#define CSID_C_CAPTIVE_PORTAL_UAMHOMEPAGE   _CFG_CAPTIVE_PORTAL(0x0008)	//T=str, define the actual captive portal url
#define CSID_C_CAPTIVE_PORTAL_UAMAAAURL     _CFG_CAPTIVE_PORTAL(0x0009)	//T=str, HTTP URL for AAA
#define CSID_C_CAPTIVE_PORTAL_HSAAA         _CFG_CAPTIVE_PORTAL(0x000a)	//T=str, if using AAA, set this to http
#define CSID_C_CAPTIVE_PORTAL_AUTH_TYPE     _CFG_CAPTIVE_PORTAL(0x000b)	//T=u8, D=0, internal radius server auth type, 0:embedded DB, 5:ext LDAP, 4:ext AD
#define CSID_C_CAPTIVE_PORTAL_WAN_IDX       _CFG_CAPTIVE_PORTAL(0x000c)	//T=u8, D=0, WAN1 0,WAN2 1,WAN3 2,selected index for which WAN
#define CSID_C_CAPTIVE_PORTAL_RADSVR_TYPE   _CFG_CAPTIVE_PORTAL(0x000d) //T=u8, D=0, radius server type, 0:internal, 1:external
#define CSID_C_CAPTIVE_PORTAL_LAN_IF        _CFG_CAPTIVE_PORTAL(0x000e)	//T=str, D="br0", lan if
#define CSID_C_CAPTIVE_PORTAL_LAN_IP        _CFG_CAPTIVE_PORTAL(0x000f)	//T=ipv4, lan ip address

#define CSID_C_CAPTIVE_PORTAL_AD_INDEX          _CFG_CAPTIVE_PORTAL(0x0010) //T=u8, AD server index from external server list(+1 and store)
#define CSID_C_CAPTIVE_PORTAL_AD_SERVER         _CFG_CAPTIVE_PORTAL(0x0011) //T=str, external AD server for internal radius server use
#define CSID_C_CAPTIVE_PORTAL_AD_DOMAIN_UPPER   _CFG_CAPTIVE_PORTAL(0x0012) //T=str, external AD domain for internal radius server use (upper case)
#define CSID_C_CAPTIVE_PORTAL_AD_DOMAIN_LOWER   _CFG_CAPTIVE_PORTAL(0x0013) //T=str, external AD domain for internal radius server use (lower case)
#define CSID_C_CAPTIVE_PORTAL_AD_DC_ADMIN       _CFG_CAPTIVE_PORTAL(0x0014) //T=str, external AD domain controller admin
#define CSID_C_CAPTIVE_PORTAL_AD_DC_PASSWD      _CFG_CAPTIVE_PORTAL(0x0015) //T=str, external AD domain controller password

#define CSID_C_CAPTIVE_PORTAL_LDAP_INDEX    _CFG_CAPTIVE_PORTAL(0x0020) //T=u8, LDAP server index from external server list(+1 and store)
#define CSID_C_CAPTIVE_PORTAL_LDAP_SERVER   _CFG_CAPTIVE_PORTAL(0x0021) //T=str, external LDAP server for internal radius server use
#define CSID_C_CAPTIVE_PORTAL_LDAP_BASEDN   _CFG_CAPTIVE_PORTAL(0x0022) //T=str, external LDAP server base DN for internal radius server use
#define CSID_C_CAPTIVE_PORTAL_LDAP_ID       _CFG_CAPTIVE_PORTAL(0x0023) //T=str, external LDAP server identity for internal radius server use
#define CSID_C_CAPTIVE_PORTAL_LDAP_PASSWD   _CFG_CAPTIVE_PORTAL(0x0024) //T=str, external LDAP server password for internal radius server use

#define CSID_C_CAPTIVE_PORTAL_UAMDOMAINS    _CFG_CAPTIVE_PORTAL(0x0030)	//T=str, accessable white list domains even if user haven't logined(; seperated)
/* Status Items */
#define CSID_S_CAPTIVE_PORTAL_ALTERED       _STA_CAPTIVE_PORTAL(0x0001) //T=u8, D=0, altered
#define CSID_S_CAPTIVE_PORTAL_PRIDNS        _STA_CAPTIVE_PORTAL(0x0002) //T=ipv4, primary DNS
#define CSID_S_CAPTIVE_PORTAL_SECDNS        _STA_CAPTIVE_PORTAL(0x0003) //T=ipv4, secondary DNS
#define CSID_S_CAPTIVE_PORTAL_UAMALLOW      _STA_CAPTIVE_PORTAL(0x0004)	//T=str, accessable white list even if user haven't logined(space seperated)
#define CSID_S_CAPTIVE_PORTAL_UAMDOMAINS    _STA_CAPTIVE_PORTAL(0x0005)	//T=str, accessable white list domains even if user haven't logined(space seperated)
#define CSID_S_CAPTIVE_UAM_NAS_ID	    _STA_CAPTIVE_PORTAL(0x0006)	//T=str, combined result of nasid_locationid
#define CSID_S_CAPTIVE_PORTAL_MASK          _STA_CAPTIVE_PORTAL(0x0007) //T=ipv4, subnet mask

//reserve for 256 entries
#define CSID_S_CAPTIVE_PORTAL_USER_IP       _STA_CAPTIVE_PORTAL(0x0100) //T=str, IP address of current online user
#define CSID_S_CAPTIVE_PORTAL_TIMELEFT      _STA_CAPTIVE_PORTAL(0x0200) //T=u32, remaining time a user can use the Internet
#endif

