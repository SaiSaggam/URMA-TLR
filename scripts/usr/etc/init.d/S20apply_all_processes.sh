#!/bin/sh

start() {
	/usr/bin/apply_all_processes start
}

stop () {
	/usr/bin/apply_all_processes stop
}

case "${1}" in
start)
	/usr/etc/init.d/init_dnsmasq.sh start
	start
	;;
stop)
	/usr/etc/init.d/init_dnsmasq.sh stop
	stop
	;;
restart|force-reload)
	/usr/etc/init.d/init_dnsmasq.sh restart
	stop
	start
	;;
*)
	echo "Usage: $0 {start|stop|restart|force-reload}"
	exit 1
	;;
esac
exit $?
