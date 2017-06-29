/*
 * @file Member ID Definition of Group SENDMAIL.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00080000/0x80080000
 */  

#ifndef _CSID_SENDMAIL_H
#define _CSID_SENDMAIL_H

#include "csid_gid.h"

#define _CFG_SENDMAIL(x)		(CSID_GIDC_SENDMAIL|(x))
#define _STA_SENDMAIL(x)		(CSID_GIDS_SENDMAIL|(x))

/* Configuration Items */ 

#define CSID_C_SENDMAIL_ENABLE				_CFG_SENDMAIL(0x0001)	//T=u8, enable/disable
#define CSID_C_SENDMAIL_PORT				_CFG_SENDMAIL(0x0002)	//T=u16, service port
#define CSID_C_SENDMAIL_SERVER  			_CFG_SENDMAIL(0x0003)	//T=str, mail server
#define CSID_C_SENDMAIL_RECEIVER			_CFG_SENDMAIL(0x0004)	//T=str, mail receiver
#define CSID_C_SENDMAIL_SUBJECT 		   	_CFG_SENDMAIL(0x0005)	//T=str, mail subject
#define CSID_C_SENDMAIL_USER     			_CFG_SENDMAIL(0x0006)	//T=str, authenticating user 
#define CSID_C_SENDMAIL_PASSWORD    		_CFG_SENDMAIL(0x0007)	//T=str, authenticating password
#define CSID_C_SENDMAIL_SENDER      		_CFG_SENDMAIL(0x000A)	//T=str, mail sender
#define CSID_C_SENDMAIL_AUTH_DISABLE    	_CFG_SENDMAIL(0x000B)	//T=u8, auth(0:enable/1:disable)
#define CSID_C_SENDMAIL_FULL_SEND_DISABLE   _CFG_SENDMAIL(0x000C)	//T=u8, if full log then send(0:enable/1:disable)
#define CSID_C_SENDMAIL_USE_RULE_NO         _CFG_SENDMAIL(0x000D)   //T=u8, send mail use rule number
#define CSID_C_SENDMAIL_INTERVAL            _CFG_SENDMAIL(0x000E)	//T=u32, send mail time interval
#define CSID_C_SENDMAIL_AUTH_ENABLE    	_CFG_SENDMAIL(0x000F)	//T=u8, auth(0:disable/1:enable)
#define CSID_C_SENDMAIL_CRYPT_SSL_TLS    	_CFG_SENDMAIL(0x0040)	//T=u8, use SSL/TLS

#define CSID_C_SENDMAIL_EZGUARD_INTERVAL   	_CFG_SENDMAIL(0x0008)	//T=u32, send_email_time_interval
#define CSID_C_SENDMAIL_EZGUARD_PORT		_CFG_SENDMAIL(0x0009)	//T=u16, service port
#define CSID_C_SENDMAIL_EZGUARD_SERVER  	_CFG_SENDMAIL(0x0010)	//T=str, mail server
#define CSID_C_SENDMAIL_EZGUARD_RECEIVER	_CFG_SENDMAIL(0x0011)	//T=str, mail receiver
#define CSID_C_SENDMAIL_EZGUARD_SUBJECT 	_CFG_SENDMAIL(0x0012)	//T=str, mail subject
#define CSID_C_SENDMAIL_EZGUARD_USER     	_CFG_SENDMAIL(0x0013)	//T=str, authenticating user 
#define CSID_C_SENDMAIL_EZGUARD_PASSWORD    _CFG_SENDMAIL(0x0014)	//T=str, authenticating password
#define CSID_C_SENDMAIL_EZGUARD_ENABLE		_CFG_SENDMAIL(0x0015)	//T=u8, enable/disable for ui

/*Download and Device Control State*/
#define CSID_C_SENDMAIL_DLAS         _CFG_SENDMAIL(0x001f)   //T=u8, download assistant email alert enable
#define CSID_C_SENDMAIL_HTTP         _CFG_SENDMAIL(0x0020)   //T=u8, http  email alert enable/disable for ui
#define CSID_C_SENDMAIL_FTP          _CFG_SENDMAIL(0x0021)   //T=u8, ftp email alert enable/disable for ui
#define CSID_C_SENDMAIL_BT           _CFG_SENDMAIL(0x0022)   //T=u8, bt email alert enable/disable for ui
#define CSID_C_SENDMAIL_USB_DEVICE   _CFG_SENDMAIL(0x0023)   //T=u8, usb device email alert enable/disable for ui

#if defined(CDE570AM_U)
#define CSID_C_SENDMAIL_AMULE        _CFG_SENDMAIL(0x0024)   //T=u8, aMule email alert enable/disable for ui
#endif

#define CSID_C_SENDMAIL_USB_SPACE       _CFG_SENDMAIL(0x0025)   //T=u8, usb space mail alert enable

#define CSID_C_SENDMAIL_NAS_PORT		_CFG_SENDMAIL(0x0032)	//T=u16, service port
#define CSID_C_SENDMAIL_NAS_SERVER  		_CFG_SENDMAIL(0x0033)	//T=str, mail server
#define CSID_C_SENDMAIL_NAS_RECEIVER		_CFG_SENDMAIL(0x0034)	//T=str, mail receiver
#define CSID_C_SENDMAIL_NAS_SUBJECT 		_CFG_SENDMAIL(0x0035)	//T=str, mail subject
#define CSID_C_SENDMAIL_NAS_USER     		_CFG_SENDMAIL(0x0036)	//T=str, authenticating user 
#define CSID_C_SENDMAIL_NAS_PASSWORD    	_CFG_SENDMAIL(0x0037)	//T=str, authenticating password
#define CSID_C_SENDMAIL_NAS_SENDER      	_CFG_SENDMAIL(0x003A)	//T=str, mail sender
#define CSID_C_SENDMAIL_NAS_AUTH_DISABLE    	_CFG_SENDMAIL(0x003B)	//T=u8, auth(0:enable/1:disable)

/* index of external server */
#define CSID_C_SENDMAIL_SERVER_INDEX        _CFG_SENDMAIL(0x0041)   //T=u8, index of external server
#define CSID_C_SENDMAIL_NAS_SERVER_INDEX    _CFG_SENDMAIL(0x0042)   //T=u8, index of external server

/* IODATA MESSAGE start*/
#define CSID_C_MAIL_MSG_ENABLE   	    _CFG_SENDMAIL(0x0050)	//T=u32, MESSAGE MAIL (1:enable/0:disable)
#define CSID_C_MAIL_MSG_SENDER      	_CFG_SENDMAIL(0x0051)	//T=str, mail sender
#define CSID_C_MAIL_MSG_RECEIVER		_CFG_SENDMAIL(0x0052)	//T=str, mail receiver
#define CSID_C_MAIL_MSG_SERVER  		_CFG_SENDMAIL(0x0053)	//T=str, SMTP mail server
#define CSID_C_MAIL_MSG_PORT		    _CFG_SENDMAIL(0x0054)	//T=u32, SMTP service port
#define CSID_C_MAIL_MSG_SUBJECT 		_CFG_SENDMAIL(0x0055)	//T=str, mail subject
#define CSID_C_MAIL_MSG_AUTH_ENABLE    	_CFG_SENDMAIL(0x0056)	//T=u32, auth(0:enable/1:disable)
#define CSID_C_MAIL_MSG_AUTH_TYPE    	_CFG_SENDMAIL(0x0057)	//T=u32, auth(0:enable/1:disable)
#define CSID_C_MAIL_MSG_USER     		_CFG_SENDMAIL(0x0058)	//T=str, authenticating user 
#define CSID_C_MAIL_MSG_PASSWORD    	_CFG_SENDMAIL(0x0059)	//T=str, authenticating password

/* IODATA MESSAGE end*/

/* Status Items */ 
#define CSID_S_SENDMAIL_STATUS      		_STA_SENDMAIL(0x0001)	//T=u16, sendmail status
#define CSID_S_SENDMAIL_ALTERED			    _STA_SENDMAIL(0x0002)	//T=u8, ui altered
#define CSID_S_SENDMAIL_SAVE_ALTERED_ENABLE	_STA_SENDMAIL(0x0003)	//Unused
#define CSID_S_SENDMAIL_SAVE_ALTERED		_STA_SENDMAIL(0x0004)	//T=u8, enable/disable for ui
#define CSID_S_SENDMAIL_SENDOK              _STA_SENDMAIL(0x0005)   //T=u32, mailtool send ok or not
#define CSID_S_SENDMAIL_SCHEDULE_DISABLE    _STA_SENDMAIL(0x0006)   //T=u8, sendmail schedule(0:enable/1:disable)

/* IODATA MESSAGE start*/
#define CSID_S_MAIL_MSG_ALTERED			    _STA_SENDMAIL(0x0007)	//T=u32, ui altered
/* IODATA MESSAGE end*/
#endif //ifndef _CSID_SENDMAIL_H
