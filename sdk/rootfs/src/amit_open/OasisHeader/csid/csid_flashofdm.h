/*
 * @file Member ID Definition of Group Flash-OFDM
 *
 * Moderator: CharlesTu
 * Group ID: 0x00390000/0x80390000
 */

#ifndef _CSID_FLASHOFDM_H
#define _CSID_FLASHOFDM_H

#include "csid_gid.h"

#define _CFG_FLASHOFDM(x)					(CSID_GIDC_FLASHOFDM|(x))
#define _STA_FLASHOFDM(x)					(CSID_GIDS_FLASHOFDM|(x))

/* Extra Definitions */
#define FLASHOFDM_CNT_TYPE_ONDEMAND			0x00
#define FLASHOFDM_CNT_TYPE_AUTORECONNECT	0x01
#define FLASHOFDM_CNT_TYPE_MANUAL			0x02

#define FLASHOFDM_CNT_STATUS_DISCONNECT		0x00
#define FLASHOFDM_CNT_STATUS_CONNECTING		0x01
#define FLASHOFDM_CNT_STATUS_CONNECT		0x02
#define FLASHOFDM_CNT_STATUS_DISCONNECTING	0x03
#define FLASHOFDM_CNT_STATUS_LEASEFAIL      0x04
#define FLASHOFDM_CNT_STATUS_WAIT_TRAFFIC	0x05
#define FLASHOFDM_CNT_STATUS_BACKUP			0x07


#define FLASHOFDM_MODEM_STATUS_DISCONNECT		0x00
#define FLASHOFDM_MODEM_STATUS_CONNECTING		0x01
#define FLASHOFDM_MODEM_STATUS_CONNECT			0x02
#define FLASHOFDM_MODEM_STATUS_DISCONNECTING	0x03
#define FLASHOFDM_MODEM_STATUS_IDLE				0x04
#define FLASHOFDM_MODEM_STATUS_AUTH				0x05
#define FLASHOFDM_MODEM_STATUS_INIT         	0x06


#define FLASHOFDM_MODEM_STATUS_SIM_ERROR    	0x81
#define FLASHOFDM_MODEM_STATUS_PIN_ERROR		0x82
#define FLASHOFDM_MODEM_STATUS_LOCK_ERROR   	0x83
#define FLASHOFDM_MODEM_STATUS_AUTH_ERROR   	0x84

#define FLASHOFDM_HOTPLUG_STATUS_NOCARD			0x00
#define FLASHOFDM_HOTPLUG_STATUS_INSERTING		0x01
#define FLASHOFDM_HOTPLUG_STATUS_INSERTED       0x02


/* Configuration Items */
#define CSID_C_FLASHOFDM_HOSTNAME           _CFG_FLASHOFDM(0x5002)   //T=str, Client's Host Name
#define CSID_C_FLASHOFDM_IF_IP              _CFG_FLASHOFDM(0x5004)   //T=ipv4, static interface ip
#define CSID_C_FLASHOFDM_IF_NM              _CFG_FLASHOFDM(0x5005)   //T=ipv4, static interface netmask
#define CSID_C_FLASHOFDM_IF_GW              _CFG_FLASHOFDM(0x5006)   //T=ipv4, static interface gateway
#define CSID_C_FLASHOFDM_IF_PRIDNS          _CFG_FLASHOFDM(0x5007)   //T=ipv4, static interface Primary DNS
#define CSID_C_FLASHOFDM_IF_SECDNS          _CFG_FLASHOFDM(0x5008)   //T=ipv4, static interface Second DNS 
#define CSID_C_FLASHOFDM_MTU                _CFG_FLASHOFDM(0x5009)   //T=u32, MTU
#define CSID_C_FLASHOFDM_CNT_TYPE           _CFG_FLASHOFDM(0x500a)   //T=u8, Connect Type
#define CSID_C_FLASHOFDM_CHGTTL             _CFG_FLASHOFDM(0x500b)   //T=u8, Enable/Disable , Change the TTL value
#define CSID_C_FLASHOFDM_AUTOBKP            _CFG_FLASHOFDM(0x500c)   //T=u8, Enable/Disable , Auto Backup
#define CSID_C_FLASHOFDM_IDLETIME           _CFG_FLASHOFDM(0x500d)   //T=u16, D=600, max idle time
#define CSID_C_FLASHOFDM_NAT_DISABLE        _CFG_FLASHOFDM(0x500f)   //T=u32, Dynamic IP wantype NAT disable
#define CSID_C_FLASHOFDM_PIN                _CFG_FLASHOFDM(0x5010)   //T=str, PIN
#define CSID_C_FLASHOFDM_CNT_TYPE_UI        _CFG_FLASHOFDM(0x5011)   //T=u8, Connect Type

/* Status Items */
#define CSID_S_FLASHOFDM_PID                _STA_FLASHOFDM(0x5001)   //T=u16, The PID of DHCP Client 
#define CSID_S_FLASHOFDM_IF_IP              _STA_FLASHOFDM(0x5002)   //T=ipv4, dynamic interface ip  
#define CSID_S_FLASHOFDM_IF_NM              _STA_FLASHOFDM(0x5003)   //T=ipv4, dynamic interface netmask
#define CSID_S_FLASHOFDM_IF_GW              _STA_FLASHOFDM(0x5004)   //T=ipv4, dynamic interface gateway
#define CSID_S_FLASHOFDM_IF_PRIDNS          _STA_FLASHOFDM(0x5005)   //T=ipv4, dynamic interface Primary DNS
#define CSID_S_FLASHOFDM_IF_SECDNS          _STA_FLASHOFDM(0x5006)   //T=ipv4, dynamic interface Second DNS 
#define CSID_S_FLASHOFDM_CNT_STATUS         _STA_FLASHOFDM(0x5007)   //T=u32, connect status
#define CSID_S_FLASHOFDM_ALTERED            _STA_FLASHOFDM(0x5008)   //T=u32, ui altered
#define CSID_S_FLASHOFDM_LEASE_TIME         _STA_FLASHOFDM(0x5009)   //T=u32, uptime + leasetime
#define CSID_S_FLASHOFDM_CNT_TIME           _STA_FLASHOFDM(0x500a)   //T=u32, connect time
#define CSID_S_FLASHOFDM_PKTIN_NUM          _STA_FLASHOFDM(0x500b)   //T=u32, number of packet input
#define CSID_S_FLASHOFDM_PKTOUT_NUM         _STA_FLASHOFDM(0x500c)   //T=u32, number of packet output
#define CSID_S_FLASHOFDM_MODEM_STATUS       _STA_FLASHOFDM(0x500d)   //T=u32, modem status
#define CSID_S_FLASHOFDM_MODEM_ALTERED      _STA_FLASHOFDM(0x500e)   //T=u32, modem altered
#define CSID_S_FLASHOFDM_HOTPLUG_STATUS		_STA_FLASHOFDM(0x500f)	 //T=u32, hotplug status
#define CSID_S_FLASHOFDM_SIGNAL_STRENGTH	_STA_FLASHOFDM(0x5010)	 //T=str, signal strength
#define CSID_S_FLASHOFDM_TX_QUALITY			_STA_FLASHOFDM(0x5011)	 //T=str, TX signal quality
#define CSID_S_FLASHOFDM_RX_QUALITY			_STA_FLASHOFDM(0x5012)	 //T=str, RX signal quality
#define CSID_S_FLASHOFDM_NETWORK_STATE		_STA_FLASHOFDM(0x5013)	 //T=str, module Network State
#define CSID_S_FLASHOFDM_EUI64				_STA_FLASHOFDM(0x5014)	 //T=str, module EUI64
#define CSID_S_FLASHOFDM_MSN				_STA_FLASHOFDM(0x5015)	 //T=str, module Hardware Serial Number
#define CSID_S_FLASHOFDM_FW					_STA_FLASHOFDM(0x5016)	 //T=str, DSP version number
#define CSID_S_FLASHOFDM_CHANNEL			_STA_FLASHOFDM(0x5017)	 //T=str, DSP support channel and band
#define CSID_S_FLASHOFDM_IF_NAME            _STA_FLASHOFDM(0x5018)   //T=str, Flash OFDM interface name
#define CSID_S_FLASHOFDM_USB_PORT			_STA_FLASHOFDM(0x5030)  //T=u32, 1:PCIE 2:USB

#endif //ifndef _CSID_FLASHOFDM_H
