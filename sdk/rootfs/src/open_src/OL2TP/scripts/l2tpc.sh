#!/bin/sh

L2TP_LNSSERVER_IP=`rdcsman 0x80060010 str`
L2TP_RESULT_FILE="/var/run/l2tp-result"
sleep 1
l2tp-control "start-session $L2TP_LNSSERVER_IP" > $L2TP_RESULT_FILE
