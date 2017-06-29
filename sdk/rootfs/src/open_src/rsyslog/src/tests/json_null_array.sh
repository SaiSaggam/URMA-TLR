#!/bin/bash
# added 2015-11-17 by rgerhards
# This file is part of the rsyslog project, released under ASL 2.0
echo ===============================================================================
echo \[json_null_array.sh\]: test for json containung \"null\" value
. $srcdir/diag.sh init
. $srcdir/diag.sh startup json_null_array.conf
. $srcdir/diag.sh tcpflood -m 1 -M "\"<167>Mar  6 16:57:54 172.20.245.8 test: @cee: { \\\"array\\\": [0, 1, null, 2, 3, null, 4] }\""
echo doing shutdown
. $srcdir/diag.sh shutdown-when-empty
echo wait on shutdown
. $srcdir/diag.sh wait-shutdown
. $srcdir/diag.sh seq-check 0 4
. $srcdir/diag.sh exit
