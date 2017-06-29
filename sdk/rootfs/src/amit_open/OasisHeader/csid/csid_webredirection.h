
/*
 * @file Member ID Definition of Group webredirection.
 *
 * Moderator: JasonJiang
* #define CSID_GIDC_WEB_REDIRECTION        0X00960000
* #define CSID_GIDS_WEB_REDIRECTION        0X80960000
 */
 
#ifndef _CSID_WEBREDIRECTION_H
#define _CSID_WEBREDIRECTION_H

#include "csid_gid.h"

#define _CFG_WEBREDIRECTION(x)     (CSID_GIDC_WEBREDIRECTION|(x))
#define _STA_WEBREDIRECTION(x)     (CSID_GIDS_WEBREDIRECTION|(x))


/* Status Items */
#define CSID_S_WEBREDIRECTION_STATUS          		_STA_WEBREDIRECTION(0x0001) //T=u8, update status
#define CSID_S_WEBREDIRECTION_ALTERED          		_STA_WEBREDIRECTION(0x0002) //T=u8, ui altered
#define CSID_S_WEBREDIRECTION_WELLTONE_STATUS	   	_STA_WEBREDIRECTION(0x0003) //T=u8, update status
#define CSID_S_WEBREDIRECTION_WELLTONE_ALTERED    	_STA_WEBREDIRECTION(0x0004) //T=u8, ui altered

/* Configuration Items */
#define CSID_C_WEBREDIRECTION_ENABLE          		_CFG_WEBREDIRECTION(0x0001) //T=u8, wheather Web redirection is enabled or not

#define CSID_C_WEBREDIRECTION_HTTP_ENABLE          	_CFG_WEBREDIRECTION(0x0010) //T=u8, wheather HTTP Web redirection is enabled or not
#define CSID_C_WEBREDIRECTION_HTTPS_ENABLE          _CFG_WEBREDIRECTION(0x0020) //T=u8, wheather HTTPS Web redirection is enabled or not
#define CSID_C_WEBREDIRECTION_HTTP_URL        		_CFG_WEBREDIRECTION(0x0030) //T=str, HTTP request redirection to 
#define CSID_C_WEBREDIRECTION_HTTPS_URL        		_CFG_WEBREDIRECTION(0x0040) //T=str, HTTPS request redirection to 


#define CSID_C_WEBREDIRECTION_SSID_STATUS        	_CFG_WEBREDIRECTION(0x0050) //T=u8, web redirection status for ssid  
																				//40 ->br1, 41->br2, 42->br3

#define CSID_C_WEBREDIRECTION_WELLTONE_ENABLE	   	_CFG_WEBREDIRECTION(0x0100) //T=u8, wheather web redirection for welltone is enabled or not
#define CSID_C_WEBREDIRECTION_WELLTONE_HTTP_URL   	_CFG_WEBREDIRECTION(0x0101) //T=str, http url for web redirection transfer to
#define CSID_C_WEBREDIRECTION_WELLTONE_HTTPS_URL   	_CFG_WEBREDIRECTION(0x0102) //T=str, https url for web redirection transfer to
#define CSID_C_WEBREDIRECTION_WELLTONE_CHECK_TIME 	_CFG_WEBREDIRECTION(0x0103) //T=u32, D=30, check time, in seconds
#define CSID_C_WEBREDIRECTION_WELLTONE_IDLE_TIME 	_CFG_WEBREDIRECTION(0x0104) //T=u32, D=30  idle time, in minutes


																				
//#define CSID_C_WEBREDIRECTION_STATIC_ACL_STATUS     _CFG_WEBREDIRECTION(0x1000) //T=u8, STATIC ACL rule is enabled or not  //512
//#define CSID_C_WEBREDIRECTION_STATIC_ACL            _CFG_WEBREDIRECTION(0x1200) //T=ipv4, static ACL IP  //512

//#define CSID_C_WEBREDIRECTION_DYNAMIC_ACL			_CFG_WEBREDIRECTION(0x1400) //T=ipv4, dnyamic ACL ip  //512



#endif //ifndef _CSID_WEBREDIRECTION_H
