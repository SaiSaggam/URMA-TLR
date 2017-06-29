#!/bin/sh
# set -x
FTP_SITE="ftp1.digi.com"
# DIRECTORY="support/firmware/branded/sieH7/latest"
DIRECTORY="support/firmware/transport/LR54/latest"
LOGGER_USER="-p user.debug -t fwupdate_script"
WGET="/usr/bin/wget"

# Parse arguments
while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -f|--ftp_site)
    FTP_SITE="$2"
    shift
    ;;
    -d|--directory)
    DIRECTORY="$2"
    shift
    ;;
    *)
    # unknown option
	logger $LOGGER_USER "Invalid argument"
	exit 1
	shift
    ;;
esac
shift
done

# Download index.html, which includes the list of files available
# Print index.html to stdout.
OUT=$($WGET -q -O - $FTP_SITE/$DIRECTORY) > /dev/null 2>&1
if [ "$?" -ne 0 ]; then 
	logger $LOGGER_USER "Failed to get index.html"
	exit 1
fi
echo $OUT
	
