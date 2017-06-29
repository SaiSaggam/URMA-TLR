#!/bin/sh

CSMAN_PID_FILE="/var/run/csmanv2.pid"
while [ ! -f "$CSMAN_PID_FILE" ]   ; do
csmanudsv2 -p /var/run/csmanv2.pid
sleep 1
done
#if [ ! -f "$CSMAN_PID_FILE" ]; then
#exit 0
#fi

echo "Power on USB Port"
cd /usr/etc/init.d/Cellular
./usb-control.sh off
./usb-control.sh on

echo "get CSID from UCI"
cd /usr/etc/init.d/Cellular
./getParam.sh

echo "start usblist"
usblist&
sleep 35

#connection_attemps=`uci get cellular.cellular1.connection_attempts`
#if [ "$connection_attempts" != "" ];then
#connection_attempts=1
#fi
#usb_idx=`rdcsman 0x801e5010 u16`
#i=0
#while [ $i != 30 ]; do
#    ready=`rdcsman 0x801e5020 u16`
#	if [ $ready -eq 1 ] ; then
#		break
#	fi
#	echo "3G not Ready, usb_idx=$usb_idx, ready=$ready"
#	sleep 1
#	i=`expr $i + 1`
#done
#if [ $ready -eq 1 ]; then
#    echo "Start Cellular Connect"
#	3g-action start $usb_idx 0
#else
#	echo "Cellular not ready, please check"
#fi


