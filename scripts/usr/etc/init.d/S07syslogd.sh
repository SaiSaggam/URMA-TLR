#!/bin/sh
#set -x

RSYSLOG_EXEC="rsyslogd"
RSYSLOG="/sbin/$RSYSLOG_EXEC"
RSYSLOG_ROTATE="/sbin/rsyslog_rotate"
PIDFILE="/var/run/rsyslogd.pid"
CONF_DIR="/etc/rsyslog.d"
LOG_DIR="/home/digi/user"
START_STOP_DAEMON="/sbin/start-stop-daemon"

generate_configs() {
	local log_level

	log_level=$(uci get system.system1.loglevel 2> /dev/null)

	if [ -z $log_level ];then
		log_level="info"
	fi

	mkdir -p ${CONF_DIR}

	# Remove existing config files
	rm -f ${CONF_DIR}/global.conf

	cat >> "${CONF_DIR}/global.conf" <<ENDCONFIG
# Log file targets
\$outchannel kern_log, ${LOG_DIR}/kern.log, 2097152, $RSYSLOG_ROTATE ${LOG_DIR}/kern.log
\$outchannel tlr_log, ${LOG_DIR}/tlr.log, 2097152, $RSYSLOG_ROTATE ${LOG_DIR}/tlr.log
\$outchannel system_log, ${LOG_DIR}/system.log, 2097152, $RSYSLOG_ROTATE ${LOG_DIR}/system.log

# Kernel log
kern.$log_level			:omfile:\$kern_log

# TLR log
user.$log_level			:omfile:\$tlr_log

# Others
*.$log_level;kern,user.none	:omfile:\$system_log
ENDCONFIG
}

start() {
	# Create log directory
	mkdir -p ${LOG_DIR}

	# Generate config files
	generate_configs

	echo -n "Starting rsyslog daemon: "
	$START_STOP_DAEMON -S -q -p "${PIDFILE}" --exec "${RSYSLOG}"
	[ $? = 0 ] && echo "OK" || echo "FAIL"
}

stop() {
	echo -n "Stopping rsyslog daemon: "
	PIDNUM=$(cat "$PIDFILE")
	$START_STOP_DAEMON --stop -q -p "${PIDFILE}" --exec "${RSYSLOG}"
	[ $? = 0 ] && echo "OK" || echo "FAIL"
}

restart() {
	stop

	# Wait until the rsyslogd stops, but max. 2 seconds
	local count=20
	while [ $count -ge 0 ]; do
		if ! killall -0 "$RSYSLOG_EXEC" >/dev/null 2>&1; then
			break
		fi

		count=$((count - 1))

		usleep 100000
	done

	# If rsyslogd couldn't gracefully stop in time, kill it
	if [ $count -lt 0 ]; then
		killall -9 "$RSYSLOG_EXEC" >/dev/null 2>&1
	fi

	start
}

case "${1}" in
start)
	start
	;;
stop)
	stop
	;;
restart|force-reload)
	restart
	;;
*)
	echo "Usage: $0 {start|stop|restart|force-reload}"
	exit 1
	;;
esac
exit $?
