#!/bin/sh

# set -x
# Start the cellular monitor daemon

start() {
	cellular_monitor -d
#	echo "skip cellular monitor"
}

stop () {
	killall cellular_monitor 
}

case "${1}" in
start)
	start
	;;
stop)
	stop
	;;
restart|force-reload)
	stop
	start
	;;
*)
	echo "Usage: $0 {start|stop|restart|force-reload}"
	exit 1
	;;
esac
exit $?
