#!/bin/sh
# Initial webserver daemon

DAEMON=/usr/bin/webserver
PIDFILE=/var/run/webserver.pid

start()
{
	echo -n "Starting DDP web server.... "
	start-stop-daemon --start -b --exec ${DAEMON}
	echo "OK"
}

stop()
{
	echo -n "Stopping DDP web server... "
	start-stop-daemon --stop --quiet --oknodo --pidfile ${PIDFILE} --signal TERM --exec ${DAEMON}
	sleep 1
	echo "OK"
}

reload()
{
	if $(start-stop-daemon -t --stop --quiet --pidfile ${PIDFILE} --exec ${DAEMON})
	then
		echo -n "Reloading DDP web server configuration... "
		start-stop-daemon --stop --pidfile ${PIDFILE} --signal HUP
		echo "OK"
	else
		start
	fi
}

restart()
{
	stop
	start
}

case "$1" in
start)
	start
	;;
stop)
	stop
	;;
reload)
	reload
	;;
restart)
	restart
	;;
*)
	echo "Usage: $0 {start|stop|reload|restart}"
	exit 1
esac

exit $?
