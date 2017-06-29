#!/bin/sh

#This is for MT7620 port state
#used by ZyXEL UI
PORT_LIST="0 1 2 3 4"
for i in $PORT_LIST; do
	eval "PORT$i=`switch reg r $((3008+$i*100)) | awk '{print $5}' | awk -F = '{print $2}'`"
	eval "PORT_=0x\$PORT$i"
	if [ $(($PORT_&1)) -eq 0 ]; then
		eval "STATE$i=\"Down\""
		eval "RATE$i=\"\""
	else
		eval "STATE$i=\"UP\""
		if [ $(($PORT_&12)) -eq 0 ]; then
			eval "RATE$i=\"10M\""
		elif [ $(($PORT_&12)) -eq 4 ]; then
			eval "RATE$i=\"100M\""
		elif [ $(($PORT_&12)) -eq 8 ]; then
			eval "RATE$i=\"1000M\""
		fi
	fi
done

#Wireless
PORT5=`iwconfig ra0 | grep Rate | awk '{printf $2}' | awk -F = '{print $2}'`
if [ "$PORT5" == "" ]; then
	STATE5="Down"
	RATE5=""
else
	STATE5="UP"
	eval "RATE5=\"$PORT5\"M"
fi

echo "$STATE4|$STATE3|$STATE2|$STATE1|$STATE0|$STATE5-$RATE4|$RATE3|$RATE2|$RATE1|$RATE0|$RATE5"
