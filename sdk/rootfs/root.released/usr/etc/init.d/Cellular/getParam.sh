#!/bin/sh


#CSID define for O3G	
#Preferred SIM Card,  1/2/3/4 SIM-A first/ SIM-B first/ SIM-A only/ SIM-B only
CSID_C_3G_MULTIWAN_SIMCARD_MODE=0x001e4000         
#Network Type 0/1/2/3/4/5  auto/2G only/3G only/2G prefer/3G prefer
CSID_C_3G_MULTIWAN_PREFER_MODE_SIMA=0x001e7070 
CSID_C_3G_MULTIWAN_PREFER_MODE_SIMB=0x001e7080
#Dial-up Profile 0/1 auto/manual configuration. if use new apn, set 1.
CSID_C_3G_MULTIWAN_AUTO_APN_SIMA=0x001e4180        
CSID_C_3G_MULTIWAN_AUTO_APN_SIMB=0x001e4190
#Pin Code
CSID_C_3G_MULTIWAN_PIN_SIMA=0x001e4090             #str type
CSID_C_3G_MULTIWAN_PIN_SIMB=0x001e40a0	           #str type
#APN
CSID_C_3G_MULTIWAN_APN1_SIMA=0x001e4010	           #str type
CSID_C_3G_MULTIWAN_APN1_SIMB=0x001e4050	           #str type	
#Dial Number
CSID_C_3G_MULTIWAN_NUMBER_SIMA=0x001e40b0          #str type		
CSID_C_3G_MULTIWAN_NUMBER_SIMB=0x001e40c0          #str type
#Account
CSID_C_3G_MULTIWAN_USER_SIMA=0x001e40d0	           #str type		
CSID_C_3G_MULTIWAN_USER_SIMB=0x001e40e0	           #str type		
#Password
CSID_C_3G_MULTIWAN_PASSWORD_SIMA=0x001e40f0        #str type		
CSID_C_3G_MULTIWAN_PASSWORD_SIMB=0x001e4110        #str type							
#Authentication
CSID_C_3G_MULTIWAN_AUTHTYPE_SIMA=0x001e4120        #0: Auto 1: PAP 2: CHAP, default is 0		?
CSID_C_3G_MULTIWAN_AUTHTYPE_SIMB=0x001e4130        #0: Auto 1: PAP 2: CHAP, default is 0	
#Primary DNS
CSID_C_3G_MULTIWAN_IF_PRIDNS_SIMA=0x001e4140       #ipv4			
CSID_C_3G_MULTIWAN_IF_PRIDNS_SIMB=0x001e4150       #ipv4			
#Secondary DNS
CSID_C_3G_MULTIWAN_IF_SECDNS_SIMA=0x001e4160       #ipv4		
CSID_C_3G_MULTIWAN_IF_SECDNS_SIMB=0x001e4170       #ipv4		
#Roaming
CSID_C_3G_MULTIWAN_ROAMING_ENABLE_SIMA=0x001e41a0  #0: disable, 1: enable, default is 0	
CSID_C_3G_MULTIWAN_ROAMING_ENABLE_SIMB=0x001e41b0  #0: disable, 1: enable, default is 0		
#Pre-command
CSID_C_3G_MULTIWAN_PRE_COMMAND=0x001e22e0 #str
#Cellular Connect Type
CSID_C_3G_MULTIWAN_CNT_TYPE=0x001e20a0 #1: Connect-on-demand, autoconnect

#FORCE_PPP
CSID_C_3G_MULTIWAN_PPP_ONLY=0x001e22d0 #FORCE_PPP , 1 for ppp only

#Cellular proc/bus/usb/devices 
CSID_C_3G_MAPPING_LEVEL=0x001e2240  	
CSID_C_3G_MAPPING_PORT=0x001e2250  	
CSID_C_3G_MAPPING_DEVICE=0x001e4200
	
		
echo "get Cellular from UCI"

#Preferred SIM Card
#state1=`uci $UCI_FILE_PATH get cellular.cellular1.state`
#state2=`uci $UCI_FILE_PATH get cellular.cellular2.state`
#if [ "$state1" == "on" ] ; then
#    SIMCARD_MODE=3
#else
#    if [ "$state2" == "on" ] ; then
#        SIMCARD_MODE=4
#    else
#        SIMCARD_MODE=0
#    fi
#fi
SIMCARD_MODE=`rdcsman $CSID_C_3G_MULTIWAN_SIMCARD_MODE u8`
    
#put value to CSID data base
if [ $SIMCARD_MODE -eq 0 ]; then
    SIMCARD_MODE=3
    wrcsman "$CSID_C_3G_MULTIWAN_SIMCARD_MODE $SIMCARD_MODE"
fi
    
echo "state1=$state1, state2=$state2, SIMCARD_MODE=$SIMCARD_MODE"

#Network Type 0/1/2/3/4/5  auto/2G only/3G only/2G prefer/3G prefer
prefer_sima=`uci get cellular.cellular1.preferred_mode`
prefer_simb=`uci get cellular.cellular2.preferred_mode`
if [ "$prefer_sima" == "auto" ];then
    PREFER_MODE_SIMA=0
elif [ "$prefer_sima" == "2g" ];then
    PREFER_MODE_SIMA=1
elif [ "$prefer_sima" == "3g" ];then
    PREFER_MODE_SIMA=2
elif [ "$prefer_sima" == "2g prefer" ];then
    PREFER_MODE_SIMA=3
elif [ "$prefer_sima" == "3g prefer" ];then
    PREFER_MODE_SIMA=4
elif [ "$prefer_sima" == "4g" ];then
    PREFER_MODE_SIMA=5
else
    PREFER_MODE_SIMA=0
fi
if [ "$prefer_simb" == "auto" ];then
    PREFER_MODE_SIMB=0
elif [ "$prefer_simb" == "2g" ];then
    PREFER_MODE_SIMB=1
elif [ "$prefer_simb" == "3g" ];then
    PREFER_MODE_SIMB=2
elif [ "$prefer_simb" == "2g prefer" ];then
    PREFER_MODE_SIMB=3
elif [ "$prefer_simb" == "3g prefer" ];then
    PREFER_MODE_SIMB=4
elif [ "$prefer_simb" == "4g" ];then
    PREFER_MODE_SIMB=5
else
    PREFER_MODE_SIMB=0
fi
echo "PREFER_MODE: 0/1/2/3/4/5 auto/2g/3g/2g prefer/3g prefer/lte"
echo "PREFER_MODE_SIMA=$PREFER_MODE_SIMA, PREFER_MODE_SIMB=$PREFER_MODE_SIMB"

#Pin Code
sima_pin=`uci get cellular.cellular1.sim_pin`
simb_pin=`uci get cellular.cellular2.sim_pin`
if [ "$sima_pin" != "" ];then
    PIN_SIMA="$sima_pin"
else
    PIN_SIMA=""
fi
if [ "$simb_pin" != "" ];then
    PIN_SIMB="$simb_pin"
else
    PIN_SIMB=""
fi
echo "PIN_SIMA=$sima_pin, PIN_SIMB=$simb_pin"

#APN and #Dial-up Profile
apn1=`uci get cellular.cellular1.apn`
if [ "$apn1" != "" ];then
    APN1_SIMA="$apn1"
    AUTO_APN_SIMA=1
else
    APN1_SIMA=""
    AUTO_APN_SIMA=0
fi
apn2=`uci get cellular.cellular2.apn`
if [ "$apn2" != "" ];then
    APN1_SIMB="$apn2"
    AUTO_APN_SIMB=1
else
    APN1_SIMB=""
    AUTO_APN_SIMB=0
fi
echo "Cellular 1: APN1_SIMA=$APN1_SIMA,AUTO_APN_SIMA=$AUTO_APN_SIMA"
echo "Cellular 2: APN1_SIMB=$APN1_SIMB,AUTO_APN_SIMB=$AUTO_APN_SIMB"

#Dial Number
NUMBER_SIMA=""
NUMBER_SIMB=""

#Account
sima_apn_username=`uci get cellular.cellular1.apn_username`
simb_apn_username=`uci get cellular.cellular2.apn_username`
if [ "$sima_apn_username" != "" ];then
    USER_SIMA="$sima_apn_username"
else
    USER_SIMA=""
fi
if [ "$simb_apn_username" != "" ];then
    USER_SIMB="$simb_apn_username"
else
    USER_SIMB=""
fi
#Password
sima_apn_password=`uci get cellular.cellular1.apn_password_clear`
simb_apn_password=`uci get cellular.cellular2.apn_password_clear`
if [ "$sima_apn_password" != "" ];then
    PASSWORD_SIMA="$sima_apn_password"
else
    PASSWORD_SIMA=""
fi
if [ "$simb_apn_password" != "" ];then
    PASSWORD_SIMB="$simb_apn_password"
else
    PASSWORD_SIMB=""
fi
#Authentication
sima_auth_type=`uci get cellular.cellular1.auth_type`
simb_auth_type=`uci get cellular.cellular2.auth_type`
if [ "$sima_auth_type" == "pap" ];then
    AUTHTYPE_SIMA=1
elif [ "$sima_auth_type" == "chap" ];then
    AUTHTYPE_SIMA=2
elif [ "$sima_auth_type" == "auto" ];then
    AUTHTYPE_SIMA=0
else
    AUTHTYPE_SIMA=0
fi
if [ "$simb_auth_type" == "pap" ];then
    AUTHTYPE_SIMB=1
elif [ "$simb_auth_type" == "chap" ];then
    AUTHTYPE_SIMB=2
elif [ "$simb_auth_type" == "auto" ];then
    AUTHTYPE_SIMB=0
else
    AUTHTYPE_SIMB=0
fi
echo "USER_SIMA=$USER_SIMA, PASSWORD_SIMA=$PASSWORD_SIMA, AUTHTYPE_SIMA=$AUTHTYPE_SIMA"
echo "USER_SIMB=$USER_SIMB, PASSWORD_SIMB=$PASSWORD_SIMB, AUTHTYPE_SIMB=$AUTHTYPE_SIMB"

#Primary DNS
IF_PRIDNS_SIMA=0
IF_PRIDNS_SIMB=0

#Secondary DNS
IF_SECDNS_SIMA=0
IF_SECDNS_SIMA=0

#Roaming
ROAMING_ENABLE_SIMA=0
ROAMING_ENABLE_SIMB=0

#Pre-command
PRE_COMMAND=0

#Cellular Connect Type
CNT_TYPE=1

#force_ppp
sima_force_ppp=`uci get cellular.cellular1.force_ppp`
simb_force_ppp=`uci get cellular.cellular2.force_ppp`
if [ "$sima_force_ppp" == "on" ];then
FORCE_PPP=1
else
FORCE_PPP=0
fi
echo FORCE_PPP="$FORCE_PPP"

#reset_attemps, after call 3g-action start n times and still disconnect, reboot cellular module
c1_rst_attemps=`uci get cellular.cellular1.connection_attempts`
c2_rst_attemps=`uci get cellular.cellular2.connection_attempts`
if [ "$c1_rst_attemps" == "" ];then
c1_rst_attemps=0
fi
if [ "$c2_rst_attemps" == "" ];then
c2_rst_attemps=0
fi
if [ $SIMCARD_MODE -eq 3 ] ; then
RST=$c1_rst_attemps
elif [ $SIMCARD_MODE -eq 4 ] ; then
RST=$c2_rst_attemps
else
RST=0
fi
echo RST=$RST
if [ $RST -gt 0 ] ; then
wrcsman "0x001e2730 1" #C_3G_MODEM_RESTART_ENABLE
else
wrcsman "0x001e2730 0"
fi
wrcsman "0x001e2740 $RST"  #C_3G_MODEM_RESTART_TIMES

#Cellular proc/bus/usb/devices, do not change
MAPPING_LEVEL=1
MAPPING_PORT=0
MAPPING_DEVICE=1

wrcsman "$CSID_C_3G_MULTIWAN_PREFER_MODE_SIMA $PREFER_MODE_SIMA && \
         $CSID_C_3G_MULTIWAN_PREFER_MODE_SIMB $PREFER_MODE_SIMB && \
         $CSID_C_3G_MULTIWAN_AUTO_APN_SIMA $AUTO_APN_SIMA && \
         $CSID_C_3G_MULTIWAN_AUTO_APN_SIMB $AUTO_APN_SIMB && \
         $CSID_C_3G_MULTIWAN_PPP_ONLY $FORCE_PPP"

wrcsman "0x001e7090 1" #set as multi select base module
wrcsman "0x801e0c01 1" #altered


wrcsman "$CSID_C_3G_MULTIWAN_PIN_SIMA \"$PIN_SIMA && \
         $CSID_C_3G_MULTIWAN_PIN_SIMB \"$PIN_SIMB && \
	 $CSID_C_3G_MULTIWAN_APN1_SIMA \"$APN1_SIMA && \
	 $CSID_C_3G_MULTIWAN_APN1_SIMB \"$APN1_SIMB && \
	 $CSID_C_3G_MULTIWAN_NUMBER_SIMA \"$NUMBER_SIMA && \
	 $CSID_C_3G_MULTIWAN_NUMBER_SIMB \"$NUMBER_SIMB && \
	 $CSID_C_3G_MULTIWAN_USER_SIMA \"$USER_SIMA && \
	 $CSID_C_3G_MULTIWAN_USER_SIMB \"$USER_SIMB && \
	 $CSID_C_3G_MULTIWAN_PASSWORD_SIMA \"$PASSWORD_SIMA && \
	 $CSID_C_3G_MULTIWAN_PASSWORD_SIMB \"$PASSWORD_SIMB"

wrcsman "$CSID_C_3G_MULTIWAN_AUTHTYPE_SIMA $AUTHTYPE_SIMA && \
	 $CSID_C_3G_MULTIWAN_AUTHTYPE_SIMB $AUTHTYPE_SIMB && \
	 $CSID_C_3G_MULTIWAN_ROAMING_ENABLE_SIMA $ROAMING_ENABLE_SIMA && \
	 $CSID_C_3G_MULTIWAN_ROAMING_ENABLE_SIMB $ROAMING_ENABLE_SIMB && \
         $CSID_C_3G_MULTIWAN_PRE_COMMAND $PRE_COMMAND && \
         $CSID_C_3G_MULTIWAN_CNT_TYPE $CNT_TYPE && \
         $CSID_C_3G_MAPPING_LEVEL $MAPPING_LEVEL && \
         $CSID_C_3G_MAPPING_PORT $MAPPING_PORT && \
         $CSID_C_3G_MAPPING_DEVICE $MAPPING_DEVICE"

wrcsman "$CSID_C_3G_MULTIWAN_IF_PRIDNS_SIMA {$IF_PRIDNS_SIMA} && \
	 $CSID_C_3G_MULTIWAN_IF_PRIDNS_SIMB {$IF_PRIDNS_SIMB} && \
	 $CSID_C_3G_MULTIWAN_IF_SECDNS_SIMA {$IF_SECDNS_SIMA} && \
	 $CSID_C_3G_MULTIWAN_IF_SECDNS_SIMB {$IF_SECDNS_SIMB}"

#C_3G_MODULE_PWR_GPIO Power GPIO for 3G Module
wrcsman "0x001e0033 6"

