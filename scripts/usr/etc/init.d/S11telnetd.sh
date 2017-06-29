#!/bin/sh
#set -x

start() {
	telnetd
}

stop () {
	killall telnetd
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
