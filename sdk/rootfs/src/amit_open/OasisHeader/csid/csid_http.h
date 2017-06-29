/*
 * @file Member ID Definition of Group HTTP
 * 
 * Moderator: RupertLi
 * Group ID: 0x00020000/0x80020000
 */

#ifndef _CSID_HTTP_H
#define _CSID_HTTP_H

#include "csid_gid.h"

#define _CFG_HTTP(x)     (CSID_GIDC_HTTP|(x))
#define _STA_HTTP(x)     (CSID_GIDS_HTTP|(x))

/* Configuration Items */
#define CSID_C_HTTP_MULTI_LOGIN         _CFG_HTTP(0x0001) //T=u8, D=0, Multi login
#define CSID_C_HTTP_AUTH_TIMEOUT        _CFG_HTTP(0x0004) //T=u16, time out of administrator
#define CSID_C_HTTP_AUTH_TIMEOUT_ENABLE _CFG_HTTP(0x0005) //T=u8, enable/disable time out of administrator
#define CSID_C_HTTP_DUAL_CHECKED_FW1_SECTIONS   _CFG_HTTP(0x0008) //T=u32, info of dual flash section
#define CSID_C_HTTP_DUAL_CHECKED_FW2_SECTIONS   _CFG_HTTP(0x0009) //T=u32, info of dual flash section
#define CSID_C_HTTP_CMARK_SUC                   _CFG_HTTP(0x000A) //T=u8, dual flash cmark check
#define CSID_C_HTTP_LANG_MODE           _CFG_HTTP(0x000F) //T=u8, language mode
#define CSID_C_HTTP_HOST_NAME           _CFG_HTTP(0x0010) //T=str, http host list(exp "http://shareport.local")
#define CSID_C_HTTP_PORT_ADDR           _CFG_HTTP(0x0020) //T=u32, port's csid address for Status 301

/* Add for ZyXEL */
#define CSID_C_HTTP_CUSTOMIZE_ENABLE    _CFG_HTTP(0x0100) //T=u8, customize http access
#define CSID_C_HTTP_ACCESS_STATUS       _CFG_HTTP(0x0110) //T=u8, allow http access from lan, wan, or both
#define CSID_C_HTTP_ACCESS_ALLOW_ALL    _CFG_HTTP(0x0120) //T=u8, allow everyone to access
#define CSID_C_HTTP_ACCESS_IP           _CFG_HTTP(0x0130) //T=str, only allow special ip
#define CSID_C_HTTP_ACCESS_PORT         _CFG_HTTP(0x0140) //T=u32, assign http port

#define CSID_C_HTTPS_CUSTOMIZE_ENABLE    _CFG_HTTP(0x0150) //T=u8, customize http access
#define CSID_C_HTTPS_ACCESS_STATUS       _CFG_HTTP(0x0160) //T=u8, allow http access from lan, wan, or both
#define CSID_C_HTTPS_ACCESS_ALLOW_ALL    _CFG_HTTP(0x0170) //T=u8, allow everyone to access
#define CSID_C_HTTPS_ACCESS_IP           _CFG_HTTP(0x0180) //T=str, only allow special ip
#define CSID_C_HTTPS_ACCESS_PORT         _CFG_HTTP(0x0190) //T=u32, assign http port

#define CSID_C_HTTP_BINDING_WAN 		_CFG_HTTP(0x01A0) //T=u8, D=0, service binding WAN
#define CSID_C_HTTPS_BINDING_WAN 		_CFG_HTTP(0x01B0) //T=u8, D=0, service binding WAN
/*****************/

#define CSID_C_HTTP_WITHSSL_DISABLE      _CFG_HTTP(0x01C0) //T=u8, D=1, 1:disable https, 0:enable https

/* External Server Login */
#define CSID_C_HTTP_EXTERNAL_SERVER_LOGIN_ENABLE		_CFG_HTTP(0x0200) //T=u32, D=0, external server login enable
#define CSID_C_HTTP_EXTERNAL_SERVER_LOGIN_TYPE			_CFG_HTTP(0x0201) //T=u32, external server type, 1:radius, 2:tacacs
#define CSID_C_HTTP_EXTERNAL_SERVER_LOGIN_INDEX			_CFG_HTTP(0x0202) //T=u32, external server index

#define CSID_C_HTTP_ACTIVE_LOGOUT 	_CFG_HTTP(0x0300) //T=u8, D=0, force log out user, design for ZyXEL(TCH)

/* Status Items */
#define CSID_S_HTTP_PID					_STA_HTTP(0x0001) //T=u32, Httpd PID

/* upgrade FW via USB */
#define CSID_S_HTTP_FWUPG_VIA_USB       _STA_HTTP(0x0002) //T=u8, D=0, set 1 if upg fw via usb
#define CSID_S_HTTP_FWUPG_USB_FILEPATH  _STA_HTTP(0x0003) //T=str, usb file path for fwupg
#define CSID_S_HTTP_FWUPG_USB_STATUS    _STA_HTTP(0x0004) //T=str, status(error code) for UI when upg via usb

/* External Server Login Status */
#define CSID_S_HTTP_EXTERNAL_SERVER_LOGIN_STATUS	_STA_HTTP(0x0005) //T=u32, login status, 1:success, 2:fail

#define CSID_S_HTTP_AUTH_SCHEME         _STA_HTTP(0x0100) //T=u8, auth scheme: CGI, Basic or Digest?
#define CSID_S_HTTP_AUTH_PAGE401        _STA_HTTP(0x0101) //T=str, redirect page when CGI auth fail
#define CSID_S_HTTP_AUTH_REALM          _STA_HTTP(0x0102) //T=str, auth realm
#define CSID_S_HTTP_LANG_ALTERED         _STA_HTTP(0x0103) //T=u8, for standalone bt ui language support
#define CSID_S_HTTP_SETTING_ALTERED 	_STA_HTTP(0x0104) //T=u8, http or https setting is altered

#define CSID_S_HTTP_ERR_RESTART			_STA_HTTP(0x0110) //T=u32, D=0, if httpd err, rtstart it

//0x0200 ~ 0x027F
#define CSID_S_HTTP_LANG_TAB_TAG        _STA_HTTP(0x0201) //T=str, supported lang table, "tag" column
//0x0280 ~ 0x02FF
#define CSID_S_HTTP_LANG_ID_DEFAULT     _STA_HTTP(0x0280) //T=u8, default language id
#define CSID_S_HTTP_LANG_TAB_ID         _STA_HTTP(0x0281) //T=u8, supported lang table, "id" column

//Each Weblet has 0x100's CSID
#define CSID_S_HTTP_FWUPG_FW_SIZE       _STA_HTTP(0xF001) //T=u32, size of FW bin file
#define CSID_S_HTTP_FWUPG_FW_LEFT       _STA_HTTP(0xF002) //T=u32, size of FW data left
#define CSID_S_HTTP_FLUPL_FL_SIZE       _STA_HTTP(0xF003) //T=u32, size of uploaded file
#define CSID_S_HTTP_FLUPL_FL_LEFT       _STA_HTTP(0xF004) //T=u32, size of uploaded file left
#define CSID_S_HTTP_CHECKED_SECTIONS    _STA_HTTP(0xF005) //T=str, D="", info of flash section
#define CSID_S_HTTP_FWUPG_UPGRADING     _STA_HTTP(0xF006) //T=u32, Upgrading flag

#define CSID_S_HTTP_DUAL_CHECKED_SECTIONS       _STA_HTTP(0xF007) //T=str, D="", info of dual flash section
#define CSID_S_HTTP_CONF_CMARK                  _STA_HTTP(0xF008) //T=u8, dual flash config cmark check
#define CSID_S_HTTP_LANG_PACK_EXIST             _STA_HTTP(0xF009) //T=u32, Language pack exist, 0: none exist, 1: exist

#define CSID_S_HTTP_CSRF_TOKEN          _STA_HTTP(0xF00a) //T=u32, D=0, CSRF Token Value

#endif //ifndef _CSID_HTTP_H
