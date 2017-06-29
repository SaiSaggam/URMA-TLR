#!/bin/bash
# Test concurrency of exec_template function with msg variables
# Added 2015-12-11 by rgerhards
# This file is part of the rsyslog project, released  under ASL 2.0
echo ===============================================================================
echo \[exec_tpl-concurrency.sh\]: testing concurrency of exec_template w variables
. $srcdir/diag.sh init
. $srcdir/diag.sh startup exec_tpl-concurrency.conf
sleep 1
. $srcdir/diag.sh tcpflood -m500000
. $srcdir/diag.sh shutdown-when-empty # shut down rsyslogd when done processing messages
. $srcdir/diag.sh wait-shutdown
. $srcdir/diag.sh seq-check 0 499999
. $srcdir/diag.sh exit
