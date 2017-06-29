/*
 * @file Member ID Definition of Group USB
 *
 * Moderator: RockChang
 * Group ID: 0x00550000/0x80550000
 */

#ifndef _CSID_USB_H
#define _CSID_USB_H

#include "csid_gid.h"

#define _CFG_USB(x)   (CSID_GIDC_USB|(x))
#define _STA_USB(x)   (CSID_GIDS_USB|(x))

/* Extra Definitions */
#define DRIVER_USBSERIAL          0x01
#define DRIVER_OPTION             0x02
#define DRIVER_CDCACM             0x03
#define DRIVER_HSO                0x04
#define DRIVER_SIERRA             0x05
#define DRIVER_SIERRANET          0x06
#define DRIVER_USBNET             0x07
#define DRIVER_CDCETHER           0x08
#define DRIVER_CDCNCM             0x09
#define DRIVER_GOBI               0x0a
#define DRIVER_HWCDCWTHER         0x0c
#define DRIVER_IPHETH             0x0d
#define DRIVER_RNDISHOST          0x0e
#define DRIVER_QMIWWAN_CDCWDM     0x0f
#define DRIVER_UE_LTE             0x10
#define DRIVER_VIAUSBMODEM        0x11
#define DRIVER_LC_CDCECM          0x13
#define DRIVER_GCT      		  0x14
#define DRIVER_ALTAIR			  0x15
#define DRIVER_CDCMBIM			  0x16
#define DRIVER_USBSTORAGE         0xff

#define TYPE_REMOVED              0x00
#define TYPE_STORAGE              0x01
#define TYPE_MODEM                0x02
#define TYPE_WEBCAM               0x03
#define TYPE_PRINTER              0x04

//#define CONNECT_TYPE_DIRECTIP       0x02
//#define CONNECT_TYPE_HSO          0x03
//#define CONNECT_TYPE_IPHETH         0x04
//#define CONNECT_TYPE_RNDIS          0x05

//#define CNT_STATUS_3G_DISCONNECT      0x00
//#define CNT_STATUS_3G_CONNECTING      0x01
//#define CNT_STATUS_3G_CONNECT       0x02
//#define CNT_STATUS_3G_DISCONNECTING     0x03
//#define CNT_STATUS_3G_WAIT_TRAFFIC      0x05
//#define CNT_STATUS_3G_AUTH_FAIL       0x06
//#define CNT_STATUS_3G_BACKUP        0x07
//#define CNT_STATUS_3G_NOT_INSCHEDULE    0x08

/* Configuration Items */
#define CSID_C_USB_QUERY_SWITCH         _CFG_USB(0x0000)  //T=u8, D=0,query status switch
#define CSID_C_USB_PORT_SUPPORT_ENABLE  _CFG_USB(0x0010)  //T=u32, D=0, support modem, storage, cam or printer.. etc

/* Status Items */
#define CSID_S_USB_BUS                  _STA_USB(0x0000)  //T=u8, USB location: bus
#define CSID_S_USB_LEVEL                _STA_USB(0x0010)  //T=u8, USB location: level
#define CSID_S_USB_PORT                 _STA_USB(0x0020)  //T=u8, USB location: port
#define CSID_S_USB_TYPE                 _STA_USB(0x0030)  //T=u32, class of USB device: storage, modem, webcam, printer.. etc
#define CSID_S_USB_VENDORID             _STA_USB(0x0040)  //T=u16, USB vendor id
#define CSID_S_USB_PRODUCTID            _STA_USB(0x0050)  //T=u16, USB product id
#define CSID_S_USB_DRIVER               _STA_USB(0x0060)  //T=u8, usbserial, option, cdcacm..etc
#define CSID_S_USB_MODEM1               _STA_USB(0x0070)  //T=str, ttyUSB0, ttyACM0
#define CSID_S_USB_MODEM2               _STA_USB(0x0080)  //T=str, ttyUSB1, ttyACM1
#define CSID_S_USB_MODEM_DIAGNOSTIC     _STA_USB(0x0090)  //T=str, ttyUSB1, ttyACM1
#define CSID_S_USB_PRODUCTNAME          _STA_USB(0x00a0)  //T=str, USB product name
#define CSID_S_USB_IF_NAME              _STA_USB(0x00b0)  //T=str, NDIS interface name, usbnet, wwan
#define CSID_S_USB_IF_NAME2             _STA_USB(0x00c0)  //T=str, NDIS interface name, usbnet, wwan
#define CSID_S_USB_QMI_NODE1            _STA_USB(0x00d0)  //T=str, qmi node name, cdc-wdm0
#define CSID_S_USB_QMI_NODE2            _STA_USB(0x00e0)  //T=str, qmi node name, cdc-wdm1
#define CSID_S_USB_IF_IP                _STA_USB(0x00f0)  //T=ipv4, WAN IP
#define CSID_S_USB_IF_IPV6              _STA_USB(0x0100)  //T=str, WAN IPV6
#define CSID_S_USB_IF_NM                _STA_USB(0x0110)  //T=ipv4, dynamic interface netmask 
#define CSID_S_USB_IF_GW                _STA_USB(0x0120)  //T=ipv4, dynamic interface gateway 
#define CSID_S_USB_IF_PRIDNS            _STA_USB(0x0130)  //T=ipv4, dynamic interface primary DNS 
#define CSID_S_USB_IF_SECDNS            _STA_USB(0x0140)  //T=ipv4, dynamic interface secondary DNS 
#define CSID_S_USB_CNT_STATUS           _STA_USB(0x0150)  //T=u16, connect status 
#define CSID_S_USB_CNT_IF_NAME          _STA_USB(0x0160)  //T=str, connect interface name, ppp, usbnet, wwan
#define CSID_S_USB_CNT_TIME             _STA_USB(0x0170)  //T=u32, connect time 
#define CSID_S_USB_CNT_ERR_CODE         _STA_USB(0x0180)  //T=u16, connect fail error code 
#define CSID_S_USB_CNT_NETWORK_NAME     _STA_USB(0x0190)  //T=str, network name 
#define CSID_S_USB_CNT_NETWORK_TYPE     _STA_USB(0x01a0)  //T=u16, network type, NONE/GPRS/EDGE/UMTS/HSDPA/HSUPA 
#define CSID_S_USB_SIGNAL_STRENGTH      _STA_USB(0x01b0)  //T=u16, signal strength
#define CSID_S_USB_PIN_STATUS           _STA_USB(0x01c0)  //T=u16, pin status
#define CSID_S_USB_MODEL_NAME           _STA_USB(0x01d0)  //T=u16, Model name, it obtained by AT+CGMM
#define CSID_S_USB_CNT_DRIVER           _STA_USB(0x01e0)  //T=u8, usbserial, option, cdcacm..etc
#define CSID_S_USB_MAC_ADDR             _STA_USB(0x01f0)  //T=str, modem mac address


/* for IPv6*/
#define CSID_S_USB_IF_GW_IPV6           _STA_USB(0x0200)  //T=str, dynamic interface gateway
#define CSID_S_USB_IF_PRIDNS_IPV6       _STA_USB(0x0210)  //T=str, dynamic interface primary DNS
#define CSID_S_USB_IF_SECDNS_IPV6       _STA_USB(0x0220)  //T=str, dynamic interface secondary DNS

/* add device interface and node */
#define CSID_S_USB_IF_NAME3             _STA_USB(0x0230)  //T=str, NDIS interface name, usbnet, wwan
#define CSID_S_USB_QMI_NODE3            _STA_USB(0x0240)  //T=str, qmi node name, cdc-wdm1, qcqmi1

/* add to specify the using driver for the modem port*/
#define CSID_S_USB_MODEM_DRIVER         _STA_USB(0x0250)  //T=u8, option, qcserial..etc, for modem port

//#define CSID_S_USB_                 _STA_USB(0x00)  T=
//#define CSID_S_USB_                 _STA_USB(0x00)  T=

//#define CSID_S_USB_UPTIME               _STA_USB(0x0140)  //T=u32, connection up time
//#define CSID_S_USB_PRODUCTNAME          _STA_USB(0x0150)  //T=str, USB product name
//#define CSID_S_USB_IMSI                 _STA_USB(0x0200)  //T=str , SIM CARD IMSI VALUE
//#define CSID_S_USB_REG_STATUS           _STA_USB(0x0210)  //T=u8, register status for 3G network 
//#define CSID_S_USB_SMS_RECEIVED         _STA_USB(0x0220)  //T=u8, D=0 ,numbers of reveived messages 
//#define CSID_S_USB_SMS_CAPACITY         _STA_USB(0x0230)  //T=u8, D=0 ,numbers of SIM card messages capacity 

#endif //ifndef _CSID_USB_H
