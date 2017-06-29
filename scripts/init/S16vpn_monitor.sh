#!/bin/sh

# set -x
# Start the vpn monitor daemon

start() {
    # temp debug empty functions cause an error, so stick something in it until we start the vpn monitor for real.
    echo "start VPN monitor"
    vpn_monitor -d
}

stop () {
    killall vpn_monitor 
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
