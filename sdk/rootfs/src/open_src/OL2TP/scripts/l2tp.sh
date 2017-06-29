#!/bin/sh
L2TP_LNSSERVER=`rdcsman 0x00060600 str`
L2TP_RESULT_FILE="/var/run/l2tp-result"

l2tp-control "start-session $L2TP_LNSSERVER" > $L2TP_RESULT_FILE
