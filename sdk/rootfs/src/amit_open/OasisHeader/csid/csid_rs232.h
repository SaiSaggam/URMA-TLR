/*
 * @file Member ID Definition of RS232 Modem
 *
 * Moderator: Eric chen
 * Group ID: 0x00420000/0x80420000
 */
#ifndef _CSID_RS232_H
#define _CSID_RS232_H

#include "csid_gid.h"

#define _CFG_RS232(x)		(CSID_GIDC_RS232|(x))
#define _STA_RS232(x)		(CSID_GIDS_RS232|(x))

/* Extra Definitions */
#define CNT_TYPE_RS232_ONDEMAND				0x00
#define CNT_TYPE_RS232_AUTORECONNECT		0x01
#define CNT_TYPE_RS232_MANUAL				0x02

#define DIALUP_CNT_TYPE_ONDEMAND          0x00
#define DIALUP_CNT_TYPE_AUTORECONNECT     0x01
#define DIALUP_CNT_TYPE_MANUAL            0x02
#define DIALUP_CNT_TYPE_ONFAILOVER        0x03

#define CNT_STATUS_RS232_DISCONNECT			0x00
#define CNT_STATUS_RS232_CONNECTING			0x01
#define CNT_STATUS_RS232_CONNECT			0x02
#define CNT_STATUS_RS232_DISCONNECTING		0x03
#define CNT_STATUS_RS232_WAIT_TRAFFIC		0x05
#define CNT_STATUS_RS232_AUTH_FAIL			0x06
#define CNT_STATUS_RS232_BACKUP				0x07
#define CNT_STATUS_RS232_NOT_INSCHEDULE		0x08

#define DIALUP_CNT_STATUS_DISCONNECT     0x00
#define DIALUP_CNT_STATUS_CONNECTING     0x01
#define DIALUP_CNT_STATUS_CONNECT        0x02
#define DIALUP_CNT_STATUS_DISCONNECTING  0x03
#define DIALUP_CNT_STATUS_WAIT_TRAFFIC   0x05
#define DIALUP_CNT_STATUS_AUTH_FAIL      0x06

/*	//define in CSID_3G.h
#define ONDEMAND_STATUS_DISCONNECT_NO_IP	0x00
#define ONDEMAND_STATUS_CONNECT_NO_IP		0x01
#define ONDEMAND_STATUS_CONNECT_GET_IP		0x02
*/
#define AUTHTYPE_RS232_AUTO					0x00
#define AUTHTYPE_RS232_PAP					0x01
#define AUTHTYPE_RS232_CHAP					0x02

#define KEEP_ALIVE_RS232_DISABLE			0x00
#define KEEP_ALIVE_RS232_PING				0x01
#define KEEP_ALIVE_RS232_LCP				0x02

#define AUTOBAK_STATUS_RS232_DISCONNECT		0x00
#define AUTOBAK_STATUS_RS232_CONNECTING		0x01
#define AUTOBAK_STATUS_RS232_CONNECT		0x02
#define AUTOBAK_STATUS_RS232_DISCONNECTING	0x03

/*
#define BAUDRATE_38400		0x00
#define BAUDRATE_57600		0x01
#define BAUDRATE_115200		0x02
#define BAUDRATE_230400		0x03
#define BAUDRATE_460800		0x04
*/


/* Configuration Items */
#define CSID_C_RS232_MODEM_SUPPORT			_CFG_RS232(0x0001)	//T=u32, D=0, 0:Not support, 1: Serial Modem support

#define CSID_C_RS232_NUMBER					_CFG_RS232(0x0002)	//T=str, Dialed Number
#define CSID_C_RS232_USER					_CFG_RS232(0x0003)	//T=str, user name
#define CSID_C_RS232_PASSWORD				_CFG_RS232(0x0004)	//T=str, user password
#define CSID_C_RS232_IDLETIME				_CFG_RS232(0x0005)	//T=u32, max idle time
#define CSID_C_RS232_CNT_TYPE				_CFG_RS232(0x0006)	//T=u32, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_RS232_BUADRATE				_CFG_RS232(0x0007)	//T=u32, D=1, 0:38400/1:57600/2:115200/3:230400/4:460800 bps
#define CSID_C_RS232_IF_IP					_CFG_RS232(0x0008)	//T=ipv4, static interface ip
#define CSID_C_RS232_IF_PRIDNS				_CFG_RS232(0x0009)	//T=ipv4, dynamic interface primary DNS
#define CSID_C_RS232_IF_SECDNS				_CFG_RS232(0x000A)	//T=ipv4, dynamic interface secondary DNS
#define CSID_C_RS232_EXTRA_SEETING			_CFG_RS232(0x000B)	//T=str, for customer extra modem AT command

#define CSID_C_RS232_SERVICENAME			_CFG_RS232(0x0010)	//T=str, service name
#define CSID_C_RS232_MTU					_CFG_RS232(0x0011)	//T=u32, D=0, max transmission unit
#define CSID_C_RS232_AUTOBAK_ENABLE			_CFG_RS232(0x0012)	//T=u32, auto backup enable, 0:disable, 1:enable
#define CSID_C_RS232_PING_HOST_IP			_CFG_RS232(0x0013)	//T=ipv4, ping host IP for 3g auto backup
#define CSID_C_RS232_AUTHTYPE				_CFG_RS232(0x0014)	//T=u32, auth type, Auto, PAP, CHAP...
#define CSID_C_RS232_KEEP_ALIVE				_CFG_RS232(0x0015)	//T=u32, 3g keep alive, disable, ping, lcp
#define CSID_C_RS232_KA_PING_INTERVAL		_CFG_RS232(0x0016)	//T=u32, D=60, ping interval for 3g keep alive
#define CSID_C_RS232_KA_PING_IP				_CFG_RS232(0x0017)	//T=ipv4, ping IP for 3g keep alive
#define CSID_C_RS232_KA_LCP_INTERVAL		_CFG_RS232(0x0018)	//T=u32, D=10, lcp interval for 3g keep alive 
#define CSID_C_RS232_KA_LCP_TIMES			_CFG_RS232(0x0019)	//T=u32, D=3, lcp times for 3g keep alive
#define CSID_C_RS232_NAT_DISABLE			_CFG_RS232(0x001A)	//T=u32, wantype NAT disable
#define CSID_C_RS232_MODE_CHANGE            _CFG_RS232(0x001B)  //T=u32, D=1, 0:All off, 1:RS-232 2:RS-422/RS-485 TX 3:RS-485 RX
#define CSID_C_RS232_LISTEN_PORT            _CFG_RS232(0x001C)  //T=u32, D=4001, port number for virtual com port 

#define CSID_C_RS232_MB_SERIAL_PROTOCOL			_CFG_RS232(0x0100)  //T=u8, D=1, modbus serial port protocol 1:RTU 2:ASCII
#define CSID_C_RS232_MB_SLAVE_ID				_CFG_RS232(0x0101)  //T=u8, D=1, modbus slave id
#define CSID_C_RS232_MB_FUNCTION_CODE			_CFG_RS232(0x0102)  //T=u8,  modbus function code
#define CSID_C_RS232_MB_STARTING_ADDRESS		_CFG_RS232(0x0103)  //T=u16, modbus starting address
#define CSID_C_RS232_MB_QUANTITY				_CFG_RS232(0x0104)  //T=u16, modbus quantity
#define CSID_C_RS232_MB_TIMER_ENABLE			_CFG_RS232(0x0105)  //T=u8, 1:Enable 2:Disable
#define CSID_C_RS232_MB_TIMER_INTERVAL			_CFG_RS232(0x0106)  //T=u32, modbus timer interval
#define CSID_C_RS232_MB_RECV_DATA_TYPE			_CFG_RS232(0x0107)  //T=u8,  modbus slave data type
#define CSID_C_RS232_MB_FTP_SERVER_IP			_CFG_RS232(0x0108)  //T=ipv4, client's ftp server ip for modbus data storage
#define CSID_C_RS232_MB_FTP_USER_NAME			_CFG_RS232(0x0109)  //T=str, client's ftp server username
#define CSID_C_RS232_MB_FTP_PASSWORD			_CFG_RS232(0x0110)  //T=str, client's ftp server password
#define CSID_C_RS232_VC_KEEPALIVE_ENABLE		_CFG_RS232(0x0111)  //T=u8, enable TCP keepalive
#define CSID_C_RS232_VC_CONNECTION_IDLETIME	 	_CFG_RS232(0x0112)  //T=u32, D=0, virtual COM connection idle time, unit is ms
#define CSID_C_RS232_VC_TRUSTED_IP_ACCESS		_CFG_RS232(0x0113)  //T=u8, D=1, trusted ip access, 1:allow all, 2:specific IP
#define CSID_C_RS232_VC_IP_ADDR_1_ENABLE		_CFG_RS232(0x0114)  //T=u8, D=0, virtual COM ip addr1 enable
#define CSID_C_RS232_VC_IP_ADDR_2_ENABLE		_CFG_RS232(0x0115)  //T=u8, D=0, virtual COM ip addr2 enable
#define CSID_C_RS232_VC_IP_ADDR_3_ENABLE		_CFG_RS232(0x0116)  //T=u8, D=0, virtual COM ip addr3 enable
#define CSID_C_RS232_VC_IP_ADDR_4_ENABLE		_CFG_RS232(0x0117)  //T=u8, D=0, virtual COM ip addr4 enable
#define CSID_C_RS232_VC_IP_ADDRESS_1_From       _CFG_RS232(0x0118) //T=ipv4, virtual COM IP ADDRESS 1 from
#define CSID_C_RS232_VC_IP_ADDRESS_1_To         _CFG_RS232(0x0119) //T=ipv4, virtual COM IP ADDRESS 1 to
#define CSID_C_RS232_VC_IP_ADDRESS_2_From       _CFG_RS232(0x0120) //T=ipv4, virtual COM IP ADDRESS 2 from
#define CSID_C_RS232_VC_IP_ADDRESS_2_To         _CFG_RS232(0x0121) //T=ipv4, virtual COM IP ADDRESS 2 to
#define CSID_C_RS232_VC_IP_ADDRESS_3_From       _CFG_RS232(0x0122) //T=ipv4, virtual COM IP ADDRESS 3 from
#define CSID_C_RS232_VC_IP_ADDRESS_3_To         _CFG_RS232(0x0123) //T=ipv4, virtual COM IP ADDRESS 3 to
#define CSID_C_RS232_VC_IP_ADDRESS_4_From       _CFG_RS232(0x0124) //T=ipv4, virtual COM IP ADDRESS 4 from
#define CSID_C_RS232_VC_IP_ADDRESS_4_To         _CFG_RS232(0x0125) //T=ipv4, virtual COM IP ADDRESS 4 to
#define CSID_C_RS232_MB_TRUSTED_IP_ACCESS		_CFG_RS232(0x0126)  //T=u8, D=1, trusted ip access, 1:allow all, 2:specific IP
#define CSID_C_RS232_MB_IP_ADDR_1_ENABLE		_CFG_RS232(0x0127)  //T=u8, D=0, modbus ip addr1 enable
#define CSID_C_RS232_MB_IP_ADDR_2_ENABLE		_CFG_RS232(0x0128)  //T=u8, D=0, modbus ip addr2 enable
#define CSID_C_RS232_MB_IP_ADDR_3_ENABLE		_CFG_RS232(0x0129)  //T=u8, D=0, modbus ip addr3 enable
#define CSID_C_RS232_MB_IP_ADDR_4_ENABLE		_CFG_RS232(0x0130)  //T=u8, D=0, modbus ip addr4 enable
#define CSID_C_RS232_MB_IP_ADDRESS_1_From       _CFG_RS232(0x0131) //T=ipv4, modbus IP ADDRESS 1 from
#define CSID_C_RS232_MB_IP_ADDRESS_1_To         _CFG_RS232(0x0132) //T=ipv4, modbus IP ADDRESS 1 to
#define CSID_C_RS232_MB_IP_ADDRESS_2_From       _CFG_RS232(0x0133) //T=ipv4, modbus IP ADDRESS 2 from
#define CSID_C_RS232_MB_IP_ADDRESS_2_To         _CFG_RS232(0x0134) //T=ipv4, modbus IP ADDRESS 2 to
#define CSID_C_RS232_MB_IP_ADDRESS_3_From       _CFG_RS232(0x0135) //T=ipv4, modbus IP ADDRESS 3 from
#define CSID_C_RS232_MB_IP_ADDRESS_3_To         _CFG_RS232(0x0136) //T=ipv4, modbus IP ADDRESS 3 to
#define CSID_C_RS232_MB_IP_ADDRESS_4_From       _CFG_RS232(0x0137) //T=ipv4, modbus IP ADDRESS 4 from
#define CSID_C_RS232_MB_IP_ADDRESS_4_To         _CFG_RS232(0x0138) //T=ipv4, modbus IP ADDRESS 4 to
#define CSID_C_RS232_MB_KEEPALIVE_ENABLE		_CFG_RS232(0x0139)  //T=u8, enable TCP keepalive
#define CSID_C_RS232_MB_LISTEN_PORT    	        _CFG_RS232(0x0140)  //T=u32, D=502, port number for modbus tcp
#define CSID_C_RS232_MB_SERIAL_RSP_TIMEOUT	 	_CFG_RS232(0x0141)  //T=u32, D=1000, modbus serial response timeout, unit is ms
#define CSID_C_RS232_MB_SERIAL_TIMEOUT_RETRY	_CFG_RS232(0x0142)  //T=u8, D=0, serial timeout retry (0-5)
#define CSID_C_RS232_MB_TX_DELAY_ENABLE		 	_CFG_RS232(0x0143)  //T=u8,  delay a while for transmit new message
#define CSID_C_RS232_MB_CONN_IDLE_TIMEOUT	 	_CFG_RS232(0x0144)  //T=u32, D=300, client connection idle timeout, unit is second
#define CSID_C_RS232_MB_TCP_CONNECTION_MAX		_CFG_RS232(0x0145)  //T=u8, D=4, maximun client connection (1-4)
#define CSID_C_RS232_MB_PRIORITY_1_ENABLE		_CFG_RS232(0x0146)  //T=u8, D=0, modbus priority 1 enable
#define CSID_C_RS232_MB_PRIORITY_2_ENABLE		_CFG_RS232(0x0147)  //T=u8, D=0, modbus priority 2 enable
#define CSID_C_RS232_MB_PRIORITY_3_ENABLE		_CFG_RS232(0x0148)  //T=u8, D=0, modbus priority 3 enable
#define CSID_C_RS232_MB_PRIORITY_4_ENABLE		_CFG_RS232(0x0149)  //T=u8, D=0, modbus priority 4 enable
#define CSID_C_RS232_MB_PRIORITY_5_ENABLE		_CFG_RS232(0x0150)  //T=u8, D=0, modbus priority 5 enable
#define CSID_C_RS232_MB_PRIORITY_1_ITEM			_CFG_RS232(0x0151)  //T=u8, D=0, modbus priority 1 item
#define CSID_C_RS232_MB_PRIORITY_2_ITEM			_CFG_RS232(0x0152)  //T=u8, D=0, modbus priority 2 item
#define CSID_C_RS232_MB_PRIORITY_3_ITEM			_CFG_RS232(0x0153)  //T=u8, D=0, modbus priority 3 item
#define CSID_C_RS232_MB_PRIORITY_4_ITEM			_CFG_RS232(0x0154)  //T=u8, D=0, modbus priority 4 item
#define CSID_C_RS232_MB_PRIORITY_5_ITEM			_CFG_RS232(0x0155)  //T=u8, D=0, modbus priority 5 item
#define CSID_C_RS232_MB_PRIORITY_1_IP	        _CFG_RS232(0x0156) //T=ipv4, modbus priority 1 ip
#define CSID_C_RS232_MB_PRIORITY_2_IP	        _CFG_RS232(0x0157) //T=ipv4, modbus priority 2 ip
#define CSID_C_RS232_MB_PRIORITY_3_IP	        _CFG_RS232(0x0158) //T=ipv4, modbus priority 3 ip
#define CSID_C_RS232_MB_PRIORITY_4_IP	        _CFG_RS232(0x0159) //T=ipv4, modbus priority 4 ip
#define CSID_C_RS232_MB_PRIORITY_5_IP	        _CFG_RS232(0x0160) //T=ipv4, modbus priority 5 ip
#define CSID_C_RS232_MB_PRIORITY_1_ID	        _CFG_RS232(0x0161) //T=u8, modbus priority 1 modbus id
#define CSID_C_RS232_MB_PRIORITY_2_ID	        _CFG_RS232(0x0162) //T=u8, modbus priority 2 modbus id
#define CSID_C_RS232_MB_PRIORITY_3_ID	        _CFG_RS232(0x0163) //T=u8, modbus priority 3 modbus id
#define CSID_C_RS232_MB_PRIORITY_4_ID	        _CFG_RS232(0x0164) //T=u8, modbus priority 4 modbus id
#define CSID_C_RS232_MB_PRIORITY_5_ID	        _CFG_RS232(0x0165) //T=u8, modbus priority 5 modbus id
#define CSID_C_RS232_MB_PRIORITY_1_CODE	        _CFG_RS232(0x0166) //T=u8, modbus priority 1 function code
#define CSID_C_RS232_MB_PRIORITY_2_CODE	        _CFG_RS232(0x0167) //T=u8, modbus priority 2 function code
#define CSID_C_RS232_MB_PRIORITY_3_CODE	        _CFG_RS232(0x0168) //T=u8, modbus priority 3 function code
#define CSID_C_RS232_MB_PRIORITY_4_CODE	        _CFG_RS232(0x0169) //T=u8, modbus priority 4 function code
#define CSID_C_RS232_MB_PRIORITY_5_CODE	        _CFG_RS232(0x0170) //T=u8, modbus priority 5 function code
#define CSID_C_RS232_MB_0Bh_Exception_ENABLE    _CFG_RS232(0x0171) //T=u8, modbus Enable 0Bh Exception
#define CSID_C_RS232_OPERATION_MODE			    _CFG_RS232(0x0172) //T=u8, 0:Disable 1:Virtual COM 2:Virtual Dialup 3:Modbus
#define CSID_C_RS232_VC_OPERATION_MODE		    _CFG_RS232(0x0173) //T=u8, 1:TCP Client 2:TCP Server 3:UDP 4:RFC-2217
#define CSID_C_RS232_VD_OPERATION_MODE		    _CFG_RS232(0x0174) //T=u8,
#define CSID_C_RS232_MB_OPERATION_MODE		    _CFG_RS232(0x0175) //T=u8, 1:Gateway 2:Concentrator
#define CSID_C_RS232_INTERFACE		            _CFG_RS232(0x0176) //T=u8, D=1, 1:RS-232 2:RS-485
#define CSID_C_RS232_BAUDRATE  			      	_CFG_RS232(0x0177) //T=u32, baudrate for communication com
#define CSID_C_RS232_DATA_BITS		        	_CFG_RS232(0x0178) //T=u8, data bit for communication com
#define CSID_C_RS232_PARITY				      	_CFG_RS232(0x0179) //T=u8, parity for communication com
#define CSID_C_RS232_STOP_BITS		        	_CFG_RS232(0x0180) //T=u8, stop bit for communication com
#define CSID_C_RS232_FLOWCTRL					_CFG_RS232(0x0181)  //T=u8,  D=0, set flow control, 0:None, 1: RTS/CTS, 2: DTR/DSR
#define CSID_C_RS232_MB_MSG_BUFFER_ENABLE 	    _CFG_RS232(0x0182) //T=u8, modbus Enable message buffering
#define CSID_C_RS232_VC_TCP_CONNECTION_MAX		_CFG_RS232(0x0183)  //T=u8, D=1, maximun client connection (1-4)
#define CSID_C_RS232_VC_KEEPALIVE				_CFG_RS232(0x0184)  //T=u8, D=0, TCP keepalive setting
#define CSID_C_RS232_VC_IP_DOH_1_ENABLE			_CFG_RS232(0x0185)  //T=u8, D=0, when mode is TCP Client, address of server 1 enable
#define CSID_C_RS232_VC_IP_DOH_2_ENABLE			_CFG_RS232(0x0186)  //T=u8, D=0, when mode is TCP Client, address of server 2 enable
#define CSID_C_RS232_VC_IP_DOH_3_ENABLE			_CFG_RS232(0x0187)  //T=u8, D=0, when mode is TCP Client, address of server 3 enable
#define CSID_C_RS232_VC_IP_DOH_4_ENABLE			_CFG_RS232(0x0188)  //T=u8, D=0, when mode is TCP Client, address of server 4 enable
#define CSID_C_RS232_VC_CONNECTION_CTRL			_CFG_RS232(0x0189)  //T=u8, when mode is TCP Client, Always/On-Demand (default: Always)
#define CSID_C_RS232_VC_DOMAIN_OF_HOST_1		_CFG_RS232(0x0190)  //T=ipv4, when mode is TCP Client, address of server 1
#define CSID_C_RS232_VC_DOMAIN_OF_HOST_2		_CFG_RS232(0x0191)  //T=ipv4, when mode is TCP Client, address of server 2
#define CSID_C_RS232_VC_DOMAIN_OF_HOST_3		_CFG_RS232(0x0192)  //T=ipv4, when mode is TCP Client, address of server 3
#define CSID_C_RS232_VC_DOMAIN_OF_HOST_4		_CFG_RS232(0x0193)  //T=ipv4, when mode is TCP Client, address of server 4
#define CSID_C_RS232_VC_PORT_OF_HOST_1          _CFG_RS232(0x0194)  //T=u32, D=4001, port number of server 1 
#define CSID_C_RS232_VC_PORT_OF_HOST_2          _CFG_RS232(0x0195)  //T=u32, D=4001, port number of server 2
#define CSID_C_RS232_VC_PORT_OF_HOST_3          _CFG_RS232(0x0196)  //T=u32, D=4001, port number of server 3 
#define CSID_C_RS232_VC_PORT_OF_HOST_4          _CFG_RS232(0x0197)  //T=u32, D=4001, port number of server 4 
#define CSID_C_RS232_VC_PORT_OF_LOCAL_1         _CFG_RS232(0x0198)  //T=u32, port number of local client connected to server 1
#define CSID_C_RS232_VC_PORT_OF_LOCAL_2         _CFG_RS232(0x0199)  //T=u32, port number of local client connected to server 2
#define CSID_C_RS232_VC_PORT_OF_LOCAL_3         _CFG_RS232(0x0200)  //T=u32, port number of local client connected to server 3
#define CSID_C_RS232_VC_PORT_OF_LOCAL_4         _CFG_RS232(0x0201)  //T=u32, port number of local client connected to server 4
#define CSID_C_RS232_VC_IP_UDP_DOH_1_ENABLE		_CFG_RS232(0x0202)  //T=u8, D=0, when mode is UDP, address of server 1 enable
#define CSID_C_RS232_VC_IP_UDP_DOH_2_ENABLE		_CFG_RS232(0x0203)  //T=u8, D=0, when mode is UDP, address of server 2 enable
#define CSID_C_RS232_VC_IP_UDP_DOH_3_ENABLE		_CFG_RS232(0x0204)  //T=u8, D=0, when mode is UDP, address of server 3 enable
#define CSID_C_RS232_VC_IP_UDP_DOH_4_ENABLE		_CFG_RS232(0x0205)  //T=u8, D=0, when mode is UDP, address of server 4 enable
#define CSID_C_RS232_VC_UDP_IP_ADDR_1_From      _CFG_RS232(0x0206) //T=ipv4, virtual COM UDP mode IP ADDRESS 1 from
#define CSID_C_RS232_VC_UDP_IP_ADDR_1_To        _CFG_RS232(0x0207) //T=ipv4, virtual COM UDP mode IP ADDRESS 1 to
#define CSID_C_RS232_VC_UDP_IP_ADDR_2_From      _CFG_RS232(0x0208) //T=ipv4, virtual COM UDP mode IP ADDRESS 2 from
#define CSID_C_RS232_VC_UDP_IP_ADDR_2_To        _CFG_RS232(0x0209) //T=ipv4, virtual COM UDP mode IP ADDRESS 2 to
#define CSID_C_RS232_VC_UDP_IP_ADDR_3_From      _CFG_RS232(0x0210) //T=ipv4, virtual COM UDP mode IP ADDRESS 3 from
#define CSID_C_RS232_VC_UDP_IP_ADDR_3_To        _CFG_RS232(0x0211) //T=ipv4, virtual COM UDP mode IP ADDRESS 3 to
#define CSID_C_RS232_VC_UDP_IP_ADDR_4_From      _CFG_RS232(0x0212) //T=ipv4, virtual COM UDP mode IP ADDRESS 4 from
#define CSID_C_RS232_VC_UDP_IP_ADDR_4_To        _CFG_RS232(0x0213) //T=ipv4, virtual COM UDP mode IP ADDRESS 4 to
#define CSID_C_RS232_VC_UDP_REMOTE_PORT_1       _CFG_RS232(0x0214)  //T=u32, D=4001, port number connected to UDP server 1
#define CSID_C_RS232_VC_UDP_REMOTE_PORT_2       _CFG_RS232(0x0215)  //T=u32, D=4001, port number connected to UDP server 2
#define CSID_C_RS232_VC_UDP_REMOTE_PORT_3       _CFG_RS232(0x0216)  //T=u32, D=4001, port number connected to UDP server 3
#define CSID_C_RS232_VC_UDP_REMOTE_PORT_4       _CFG_RS232(0x0217)  //T=u32, D=4001, port number connected to UDP server 4
#define CSID_C_RS232_WAN_IFACE					_CFG_RS232(0x0218)  //T=u8, D=0, wan index

/* 2015/05/07 kevin added */
#define CSID_C_RS232_MB_GATEWAY_ENABLE		    _CFG_RS232(0x0219) //T=u8, D=1, modbus Enable gateway
#define CSID_C_RS232_MB_SLAVE_ENABLE		    _CFG_RS232(0x0220) //T=u8, D=0, modbus Enable slave
#define CSID_C_RS232_MB_SLAVE_IF_SERIAL_ENABLE	_CFG_RS232(0x0221) //T=u8, D=0, modbus slave Enable Serial Interface
#define CSID_C_RS232_MB_SLAVE_IF_TCP_NETWORK_ENABLE		_CFG_RS232(0x0222) //T=u8, D=0, modbus slave Enable TCP Network Interface
#define CSID_C_RS232_MB_SOURCE_IP_ITEM		_CFG_RS232(0x0230) //T=u8, D=0, 0:Specific IP Address 1:IP Range 2:IP Address-based group
#define CSID_C_RS232_MB_SOURCE_IP_START		_CFG_RS232(0x0240) //T=ipv4, modbus start of IP range or Specific IP Address
#define CSID_C_RS232_MB_SOURCE_IP_END		_CFG_RS232(0x0250) //T=ipv4, modbus end of IP range
#define CSID_C_RS232_MB_SOURCE_IP_GROUP		_CFG_RS232(0x0260) //T=str, modbus IP Address-based Group

/* Status Items */
#define CSID_S_RS232_ALTERED					_STA_RS232(0x0001)	//T=u32, ui altered
#define CSID_S_RS232_MODEM_STATUS				_STA_RS232(0x0002)	//T=u32, MODEM status 0:offline, 1:Ready.

#define CSID_S_RS232_CNT_TIME					_STA_RS232(0x0100)	//T=u32, connect time
#define CSID_S_RS232_IF_IP						_STA_RS232(0x0101)	//T=ipv4, dynamic interface ip
#define CSID_S_RS232_IF_NM						_STA_RS232(0x0102)	//T=ipv4, dynamic interface netmask
#define CSID_S_RS232_IF_GW						_STA_RS232(0x0103)	//T=ipv4, dynamic interface gateway
#define CSID_S_RS232_IF_PRIDNS					_STA_RS232(0x0104)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_RS232_IF_SECDNS					_STA_RS232(0x0105)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_RS232_CNT_STATUS					_STA_RS232(0x0200)	//T=u16, connect status
#define CSID_S_RS232_ERR_CODE					_STA_RS232(0x0201)	//T=u16, connect fail error code
#define CSID_S_RS232_PKTIN_NUM					_STA_RS232(0x0300)	//T=u32, number of packet input
#define CSID_S_RS232_PKTOUT_NUM					_STA_RS232(0x0301)	//T=u32, number of packet output
#define CSID_S_RS232_PKTIN_NUM_VCP				_STA_RS232(0x0302)	//T=u32, number of packet input, using for Virtual Com Port
#define CSID_S_RS232_PKTOUT_NUM_VCP    			_STA_RS232(0x0303)	//T=u32, number of packet output, using for Virtual Com Port
#define CSID_S_RS232_ONDEMAND_STATUS			_STA_RS232(0x0400)	//T=u16, demend wan type status for script so it don't care for UI
#define CSID_S_RS232_AUTOBAK_STATUS				_STA_RS232(0x0500)	//T=u16, 3g failover status
#define CSID_S_RS232_SERIAL_ALTERED         	_STA_RS232(0x0600)  //T=u32, ui altered
#define CSID_S_RS232_SERIAL_BAUDRATE        	_STA_RS232(0x0700)  //T=u32, baudrate for virtual com port
#define CSID_S_RS232_SERIAL_PARITY_BIT      	_STA_RS232(0x0701)  //T=u32, parity for virtual com port
#define CSID_S_RS232_SERIAL_DATA_BIT        	_STA_RS232(0x0702)  //T=u32, data bit for virtual com port
#define CSID_S_RS232_SERIAL_STOP_BIT        	_STA_RS232(0x0703)  //T=u32, stop bit for virtual com port
#define CSID_S_RS232_SERIAL_PROTOCOL        	_STA_RS232(0x0704)  //T=u32, protocol for virtual com port, 1:TCP, 2:UDP, 3:RFC2217
#define CSID_S_RS232_SERIAL_PROTOCOL_MODE   	_STA_RS232(0x0705)  //T=u32, protocol mode for virtual com port, 1:SERVER, 2:ClIENT
#define CSID_S_RS232_SERIAL_SERVER_IP       	_STA_RS232(0x0706)  //T=ipv4, protocol mode client ip for virtual com port
#define CSID_S_RS232_SERIAL_BAUDRATE_IN     	_STA_RS232(0x0707)  //T=u32, incoming baudrate for virtual com port
#define CSID_S_RS232_SERIAL_BAUDRATE_OUT    	_STA_RS232(0x0708)  //T=u32, output baudrate for virtual com port
#define CSID_S_RS232_SERIAL_RFC2217_IDLETIME  	_STA_RS232(0x0709)  //T=u32, D=10, RFC2217 waiting data transmission idle time, unit is minute
#define CSID_S_RS232_SERIAL_PRE_BAUDRATE 		_STA_RS232(0x0710)  //T=u32, D=0, show present baudrate set by stty 
#define CSID_S_RS232_SERIAL_FLOWCONTROL_MODE	_STA_RS232(0x0711)  //T=u8,  D=0, set flow control, 0:None, 1: XON/XOFF, 2: RTS/CTS
#define CSID_S_RS232_I2C_GPOA_DATA    			_STA_RS232(0x0712)  //T=u32, D=0, MCP23017 I2C GPOA data
#define CSID_S_RS232_I2C_GPOB_DATA              _STA_RS232(0x0713)  //T=u32, D=0, MCP23017 I2C GPOB data
#define CSID_S_RS232_CMD_STATUS                 _STA_RS232(0x0714)  //T=u32, dialup command status


#define CSID_S_RS232_WAN_IP						_STA_RS232(0x1000) //T=ipv4, wan ip
#define CSID_S_RS232_WAN_GW						_STA_RS232(0x2100) //T=ipv4, wan gateway
#define CSID_S_RS232_WAN_IF_NAME				_STA_RS232(0x2200) //T=str, the interface name of wan, ppp0, eth1, ixp1

#endif //ifndef _CSID_RS232_H
