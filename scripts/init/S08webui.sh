#!/bin/ash
#===============================================================================
#
#  S08webui.sh
#
#  Copyright (C) 2015 by Digi International Inc.
#  All rights reserved.
#
#  !Revision:    $Revision: 1.0 $:
#  !Description: Copy Web UI files to /var/www.
#
#===============================================================================

# This script replicates Web UI to /var/www which is in RAM
# this action provides read-write ability for Web UI.

DIR_SRC="/usr/web"
DIR_DEST="/var/www"

if [ -d ${DIR_SRC} ]; then
	echo
	echo -n "Copying Web UI to /var/www..."
	if [ -d ${DIR_DEST} ]; then
		rm -r ${DIR_DEST}
	fi
	mkdir ${DIR_DEST}
	cp -r ${DIR_SRC}/* ${DIR_DEST}
	echo " Done."
	echo
else
	echo "Warning!"
	echo "Failed to locate Web UI."
fi
