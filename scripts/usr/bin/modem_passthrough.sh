#!/bin/sh
# set -x
##############################################################################
# Copyright (c) 2016 Digi International Inc., All Rights Reserved
#
# This software contains proprietary and confidential information of Digi
# International Inc.  By accepting transfer of this copy, Recipient agrees
# to retain this software in confidence, to prevent disclosure to others,
# and to make no use of this software other than that for which it was
# delivered.  This is an unpublished copyrighted work of Digi International
# Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
# prohibited.
#
# Restricted Rights Legend
#
# Use, duplication, or disclosure by the Government is subject to
# restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
# Technical Data and Computer Software clause at DFARS 252.227-7031 or
# subparagraphs (c)(1) and (2) of the Commercial Computer Software -
# Restricted Rights at 48 CFR 52.227-19, as applicable.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
##############################################################################
DEFAULT_TCP_PORT="54321"
TCP_PORT=$DEFAULT_TCP_PORT
USB_AT_PORT=""
SOCAT_DAEMON="/usr/bin/socat"
PIDFILE="/var/run/passthrough.pid"
LOGGER="/usr/bin/logger"
LOGGER_USER="-p user.debug -t modem_passthrough"
TTY_OPTS="echo=0,raw"
TCP_OPTS="reuseaddr,fork"


USAGE_TEXT="Usage: "$0" {start <port_number>|stop|restart}"

# socat creates a two way connection between the modem AT port (USB_AT_PORT)
# and the specified TCP port (TCP_PORT)

start_socat()
{
    # Reading USB index
    INDEX_USB=`rdcsman 0x801E5010 u16`
    QUERY_3G_FILE="/ram/var/run/query_3g_status_$INDEX_USB.pid"
    while [ ! -f $QUERY_3G_FILE ]; do
        sleep 1
    done
    
    QUERY_3G_PID=`cat $QUERY_3G_FILE`
    kill -9 ${QUERY_3G_PID} > /dev/null 2>&1
    rm -f $QUERY_3G_FILE
    
    USB_AT_PORT=`rdcsman 0x8055008$INDEX_USB str`
    start-stop-daemon --quiet --start \
            --pidfile $PIDFILE \
            --background --make-pidfile \
            --exec $SOCAT_DAEMON -- tcp-l:"$TCP_PORT,$TCP_OPTS" file:"/dev/$USB_AT_PORT,$TTY_OPTS" < /dev/null
}

stop_socat()
{ 
    start-stop-daemon --stop --quiet --pidfile $PIDFILE --exec $SOCAT_DAEMON
    rm -f $PIDFILE
}


start () 
{        
    start_socat
    return $?
}

stop () 
{
    for PIDFILE in `ls $PIDFILE 2> /dev/null`; do
        stop_socat
    done
}

case "$1" in
    start)
        if [ $# -eq 2 ] ; then
            TCP_PORT=$2
            start
            $LOGGER $LOGGER_USER  "Starting pass-through on USB Port" $USB_AT_PORT "TCP port" $TCP_PORT
        else
            echo $USAGE_TEXT
        fi
        ;;
    stop)
        $LOGGER $LOGGER_USER  "Stopping pass-through mode"
        stop
        ;;
    restart)
        $LOGGER $LOGGER_USER  "Restarting pass-through mode"
        stop
        start
        ;;
    *)
        echo $USAGE_TEXT
        exit 3
        ;;
esac

exit 0
