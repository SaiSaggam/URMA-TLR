#!/bin/sh
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

# This script uses the CLI application to execute the contents of /var/digi/autorun.
set -e

CLI_EXE="/usr/bin/cli"
BOOT_MODE="-m boot"
ACTION_MODE="-m action"
NON_INTERACTIVE_MODE="-m non-interactive"
#the name of the file containing the autorun commands
AUTORUN_FILE="/var/digi/autorun"

. /lib/functions.sh

write_autorun_config() {
    local cfg="$1"

    config_get command $cfg command
    if [ -n "$command" ]; then
        echo "$command" >> ${AUTORUN_FILE}
    fi
}

create_autorun_file() {
    echo "# Autorun commands." > ${AUTORUN_FILE}
    config_load autorun
    config_foreach write_autorun_config autorun
}

create_autorun_file

# now run any autorun commands that the user may have configured
if [ -e ${AUTORUN_FILE} ]; then
	${CLI_EXE} ${NON_INTERACTIVE_MODE} < ${AUTORUN_FILE}
fi

