#!/bin/sh

# set -x
# Start the bridge monitor daemon

MONITOR_NAME=bridge_monitor

start() {
	$(MONITOR_NAME) -d
}

stop () {
	killall $(MONITOR_NAME)
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
