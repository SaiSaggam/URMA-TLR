/*
 * @file Member ID Definition of Group WiBro
 *
 * Moderator: Jon
 * Group ID: 0x00350000/0x80350000
 */

#ifndef _CSID_WIBRO_H
#define _CSID_WIBRO_H

#include "csid_gid.h"

#define _CFG_WIBRO(x)					(CSID_GIDC_WIBRO|(x))
#define _STA_WIBRO(x)					(CSID_GIDS_WIBRO|(x))

/* Extra Definitions */
#define WIBRO_CNT_TYPE_ONDEMAND			0x00
#define WIBRO_CNT_TYPE_AUTORECONNECT	0x01
#define WIBRO_CNT_TYPE_MANUAL			0x02

#define WIBRO_CNT_STATUS_DISCONNECT		0x00
#define WIBRO_CNT_STATUS_CONNECTING		0x01
#define WIBRO_CNT_STATUS_CONNECT		0x02
#define WIBRO_CNT_STATUS_DISCONNECTING	0x03
#define WIBRO_CNT_STATUS_LEASEFAIL      0x04
#define WIBRO_CNT_STATUS_WAIT_TRAFFIC	0x05
#define WIBRO_CNT_STATUS_BACKUP			0x07

#define WIBRO_EAP_METHOD_TTLS_MSCHAPv2				0x00
#define WIBRO_EAP_METHOD_TTLS_MSCHAPv2_DEVICE		0x01
#define WIBRO_EAP_METHOD_TTLS_CHAP					0x02
#define WIBRO_EAP_METHOD_TTLS_EAP_TLS				0x03
#define WIBRO_EAP_METHOD_TLS						0x04


#define WIBRO_MODEM_STATUS_DISCONNECT		0x00
#define WIBRO_MODEM_STATUS_CONNECTING		0x01
#define WIBRO_MODEM_STATUS_CONNECT			0x02
#define WIBRO_MODEM_STATUS_DISCONNECTING	0x03
#define WIBRO_MODEM_STATUS_IDLE				0x04
#define WIBRO_MODEM_STATUS_AUTH				0x05
#define WIBRO_MODEM_STATUS_INIT         	0x06


#define WIBRO_MODEM_STATUS_SIM_ERROR    	0x81
#define WIBRO_MODEM_STATUS_PIN_ERROR		0x82
#define WIBRO_MODEM_STATUS_LOCK_ERROR   	0x83
#define WIBRO_MODEM_STATUS_AUTH_ERROR   	0x84

#define WIBRO_HOTPLUG_STATUS_NOCARD			0x00
#define WIBRO_HOTPLUG_STATUS_INSERTING		0x01
#define WIBRO_HOTPLUG_STATUS_INSERTED       0x02


/* Configuration Items */
#define CSID_C_WIBRO_CNT_TYPE_UI        _CFG_WIBRO(0x5001)   //T=u8, Connect Type
#define CSID_C_WIBRO_HOSTNAME           _CFG_WIBRO(0x5002)   //T=str, Client's Host Name
#define CSID_C_WIBRO_IF_IP              _CFG_WIBRO(0x5004)   //T=ipv4, static interface ip
#define CSID_C_WIBRO_IF_NM              _CFG_WIBRO(0x5005)   //T=ipv4, static interface netmask
#define CSID_C_WIBRO_IF_GW              _CFG_WIBRO(0x5006)   //T=ipv4, static interface gateway
#define CSID_C_WIBRO_IF_PRIDNS          _CFG_WIBRO(0x5007)   //T=ipv4, static interface Primary DNS
#define CSID_C_WIBRO_IF_SECDNS          _CFG_WIBRO(0x5008)   //T=ipv4, static interface Second DNS 
#define CSID_C_WIBRO_MTU                _CFG_WIBRO(0x5009)   //T=u32, MTU
#define CSID_C_WIBRO_CNT_TYPE           _CFG_WIBRO(0x500a)   //T=u32, Connect Type
#define CSID_C_WIBRO_CHGTTL             _CFG_WIBRO(0x500b)   //T=u8, Enable/Disable , Change the TTL value
#define CSID_C_WIBRO_AUTOBKP            _CFG_WIBRO(0x500c)   //T=u8, Enable/Disable , Auto Backup
#define CSID_C_WIBRO_IDLETIME           _CFG_WIBRO(0x500d)   //T=u16, D=600, max idle time
#define CSID_C_WIBRO_NAT_DISABLE        _CFG_WIBRO(0x500f)   //T=u32, Dynamic IP wantype NAT disable
#define CSID_C_WIBRO_PIN                _CFG_WIBRO(0x5010)   //T=str, PIN

/* add for beceem wimax */
#define CSID_C_WIBRO_USERNAME			_CFG_WIBRO(0x5011)   //T=str, username
#define CSID_C_WIBRO_PASSWORD			_CFG_WIBRO(0x5012)	 //T=str, password
#define CSID_C_WIBRO_BANDWIDTH_MHZ 		_CFG_WIBRO(0x5013)   //T=str, BandwidthMHz (Valid values: 5, 7, 8.75, and 10)
#define CSID_C_WIBRO_SCAN_MIN_FREQ_MHZ	_CFG_WIBRO(0x5014)	 //T=str, ScanMinFrequencyMHz (Type: Floating point, Units: MHz)
#define CSID_C_WIBRO_SCAN_MAX_FREQ_MHZ  _CFG_WIBRO(0x5015)   //T=str, ScanMaxFrequencyMHz (Type: Floating point, Units: MHz)
#define CSID_C_WIBRO_SCAN_STEP_FREQ_MHZ _CFG_WIBRO(0x5016)   //T=str, ScanStepFrequencyMHz (Type: Floating point, Units: MHz)
#define CSID_C_WIBRO_CENTER_FREQ_MHZ_1  _CFG_WIBRO(0x5017)   //T=str, Discrete frequency list parameter (2600 2610 2620)
#define CSID_C_WIBRO_CENTER_FREQ_MHZ_2  _CFG_WIBRO(0x5018)   //T=str, Discrete frequency list parameter (2600 2610 2620) 
#define CSID_C_WIBRO_CENTER_FREQ_MHZ_3  _CFG_WIBRO(0x5019)   //T=str, Discrete frequency list parameter (2600 2610 2620) 
#define CSID_C_WIBRO_EAP_METHOD			_CFG_WIBRO(0x501a)   //T=u32, D=0, (EAP-TTLS/MSCHAPv2)
#define CSID_C_WIBRO_TTLS_ANONYMOUS_IDENTITY  _CFG_WIBRO(0x501b) //T=str, (Anonymous identity for all EAP-TTLS modes)
#define CSID_C_WIBRO_OR_WIMAX			_CFG_WIBRO(0x501c)	 //T=u32, D=1, (0: wibro, 1: wimax, 2:yota wimax)
#define CSID_C_WIBRO_NETWORK_NAME		_CFG_WIBRO(0x501d)	 //T=str, network name
#define CSID_C_WIBRO_AUTH_ENABLE		_CFG_WIBRO(0x501e)	 //T=u32, D=1, authentication
#define CSID_C_WIBRO_DEVICE_PRIVATEKEY_PASSWORD		_CFG_WIBRO(0x501f)	 //T=str, DevicePrivateKeyPassword 
#define CSID_C_WIBRO_OPERATOR_ID		_CFG_WIBRO(0x5020)	 //T=u32, D=0(sprint), Operator ID
#define CSID_C_WIBRO_CONFIG_FILE_PATH			_CFG_WIBRO(0x5021)	 //T=str, path of wimaxd.conf...ugy
#define CSID_C_WIBRO_KEEP_ALIVE            _CFG_WIBRO(0x5022)   //T=u8, Enable/Disable , Wibro keep alive



/* Status Items */
#define CSID_S_WIBRO_PID                _STA_WIBRO(0x5001)   //T=u16, The PID of DHCP Client 
#define CSID_S_WIBRO_IF_IP              _STA_WIBRO(0x5002)   //T=ipv4, dynamic interface ip  
#define CSID_S_WIBRO_IF_NM              _STA_WIBRO(0x5003)   //T=ipv4, dynamic interface netmask
#define CSID_S_WIBRO_IF_GW              _STA_WIBRO(0x5004)   //T=ipv4, dynamic interface gateway
#define CSID_S_WIBRO_IF_PRIDNS          _STA_WIBRO(0x5005)   //T=ipv4, dynamic interface Primary DNS
#define CSID_S_WIBRO_IF_SECDNS          _STA_WIBRO(0x5006)   //T=ipv4, dynamic interface Second DNS 
#define CSID_S_WIBRO_CNT_STATUS         _STA_WIBRO(0x5007)   //T=u32, connect status
#define CSID_S_WIBRO_ALTERED            _STA_WIBRO(0x5008)   //T=u32, ui altered
#define CSID_S_WIBRO_LEASE_TIME         _STA_WIBRO(0x5009)   //T=u32, uptime + leasetime
#define CSID_S_WIBRO_CNT_TIME           _STA_WIBRO(0x500a)   //T=u32, connect time
#define CSID_S_WIBRO_PKTIN_NUM          _STA_WIBRO(0x500b)   //T=u32, number of packet input
#define CSID_S_WIBRO_PKTOUT_NUM         _STA_WIBRO(0x500c)   //T=u32, number of packet output
#define CSID_S_WIBRO_MODEM_STATUS       _STA_WIBRO(0x500d)   //T=u32, modem status
#define CSID_S_WIBRO_MODEM_ALTERED      _STA_WIBRO(0x500e)   //T=u32, modem altered
#define CSID_S_WIBRO_HOTPLUG_STATUS		_STA_WIBRO(0x500f)	 //T=u32, hotplug status
#define CSID_S_WIBRO_OLDWANTYPE         _STA_WIBRO(0x5010)   //T=u32,
#define CSID_S_WIBRO_CARD_TYPE          _STA_WIBRO(0x5011)   //T=u32, 0 wibro,  1 wimax
#define CSID_S_WIBRO_CARD_MAC			_STA_WIBRO(0x5012)	//T=mac, The MAC address of CARD
#define CSID_S_WIBRO_CARD_DRIVER_VERSION			_STA_WIBRO(0x5013)	//T=str, driver version of CARD

#endif //ifndef _CSID_WIBRO_H
