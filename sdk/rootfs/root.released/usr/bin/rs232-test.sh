#!/bin/sh

set_i2c m 1

echo "RS232 Reliability Test"
echo "Send ATCommand Per 5 Seconds"

while [ true ] ; do
    Date=`date`
	echo "Current Time=$Date"
	send_ATcmd /dev/ttyUSB0 Time="$Date"
	sleep 5
done
