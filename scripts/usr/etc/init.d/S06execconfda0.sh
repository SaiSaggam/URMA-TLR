#!/bin/ash
#===============================================================================
#
#  Copyright (C) 2016 by Digi International Inc.
#  All rights reserved.
#
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License version 2 as published by
#  the Free Software Foundation.
#
#
#  !Revision:    $Revision: 1.0 $:
#  !Description: Execute contents of the configured configuration file.
#
#===============================================================================

# This script uses the CLI application to execute the configured configuration file.
# The current configuration file contains the differences between the platform default configuration
# settings and the configuration the user has set.
set -e

if [ "${1}" != "start" ]; then
	exit 0
fi

CLI_EXE="/usr/bin/cli"
BOOT_MODE="-m boot"
ACTION_MODE="-m action"
NON_INTERACTIVE_MODE="-m non-interactive"

CONFIG_FAC="/usr/config/config.fac"

CONFIG_PATH="/home/digi/user"
USER_CONFIG_FAC=${CONFIG_PATH}/config.fac
CONFIGFILE="/home/digi/configfile"

RESTORE_FIREWALL_EXE=/usr/bin/restore_firewall

add_users_wperm()
{
	local filename="$1"

	/bin/chown root:users "$filename"
	/bin/chmod 0664 "$filename"
}

# we need to have the RAM directory available for the UCI config files ready before running CLI the first time
mkdir -p /etc/config

if [ ! -e ${CONFIGFILE} ]; then
	echo "config.da0" > ${CONFIGFILE}
	configf="config.da0"
else
	configf=$(cat ${CONFIGFILE})
	if [ -z ${configf// } ]; then
		echo "config.da0" > ${CONFIGFILE}
		configf="config.da0"
	fi
fi

if [ ! -e ${USER_CONFIG_FAC} ]; then
    if [ -e ${CONFIG_FAC} ]; then
        cp ${CONFIG_FAC} ${USER_CONFIG_FAC}

        add_users_wperm ${USER_CONFIG_FAC}
    else
        echo "${CONFIG_FAC} does not exist!!!"
        exit 1
    fi
fi


if [ ! -e ${CONFIG_PATH}/${configf} ]; then
	configf="config.da0"
fi

if [ ! -e ${CONFIG_PATH}/${configf} ]; then
    if [ -e ${USER_CONFIG_FAC} ]; then
        # If there is no config.da0, then create one by copying config.fac to config.da0.
        # config.da0 contain the defaults for the unit from the factory.  These may be 
        # different than the platform defaults.
        echo "${CONFIG_PATH}/${configf}  does not exist, copying ${USER_CONFIG_FAC} to ${CONFIG_PATH}/${configf} "
        cp ${USER_CONFIG_FAC} ${CONFIG_PATH}/${configf}

        add_users_wperm ${CONFIG_PATH}/${configf}
    else
        echo "Neither ${CONFIG_PATH}/${configf} nor ${USER_CONFIG_FAC} exist!!!"
        exit 1
    fi
fi

# Execute config.da0 by piping it into the CLI application's stdin.  Execute the CLI in
# boot mode so that it does not attempt to apply each configuration command as they are
# executed.
${CLI_EXE} ${BOOT_MODE} < ${CONFIG_PATH}/${configf}

# Now run any action commands that may be in the config file
${CLI_EXE} ${ACTION_MODE} < ${CONFIG_PATH}/${configf}

${RESTORE_FIREWALL_EXE}

