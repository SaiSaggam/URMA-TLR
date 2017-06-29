#!/bin/sh
#set -x

FTP_SITE="ftp1.digi.com"
# DIRECTORY="support/firmware/branded/sieH7/latest"
DIRECTORY="support/firmware/transport/LR54/latest"
DESTINATION_DIRECTORY="/tmp/fw"
FW_FILE_PREFIX="lr54-"
FW_FILE_SUFFIX=".bin"
LOGGER_USER="-p user.debug -t fwupdate_script"
FW_UPDATE_COMMAND="/usr/bin/action_update_firmware"
WGET="/usr/bin/wget"
FW_APPLY="yes"

# If no file name is passed in grab the most recent version
FILE_NAME=""

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
    -n|--file_name)
    FILE_NAME="$2"
    shift
    ;;
    -p|--file_prefix)
    FW_FILE_PREFIX="$2"
    shift
    ;;
    -s|--file_suffix)
    FW_FILE_SUFFIX="$2"
    shift
    ;;
    -a|--apply)
    FW_APPLY="$2"
    shift
    ;;
    *)
    # unknown option
	logger $LOGGER_USER "Invalid argument"
	shift
    ;;
esac
shift
done

get_latest_fw_version_file_name()
{ 
	# Download index.html, which includes the list of files available, 
	# strip out the lr54-xx.xx.xx.xx.bin strings and sort the result, then select 
	# the lexicographically greatest file name.
	OUT=$($WGET -q -O - $FTP_SITE/$DIRECTORY) > /dev/null 2>&1
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Failed to get index.html"
		exit 1
	fi
	
	FILE_NAME=$(echo "$OUT" | grep -m 1 -o -E "$FW_FILE_PREFIX[^<>]*?$FW_FILE_SUFFIX" | sort | awk 'END{print}') > /dev/null 2>&1
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Failed to find file name"
		exit 1
	fi
	
	logger $LOGGER_USER "Downloading file " $FILE_NAME
}
retrieve_file()
{
	$WGET -r -l1 -np $FTP_SITE/$DIRECTORY/$FILE_NAME -nd -P $DESTINATION_DIRECTORY -o $DESTINATION_DIRECTORY/status.log
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Failed to download image"
		exit 1
	fi
}

create_tmp_directory()
{
	rm -rf $DESTINATION_DIRECTORY > /dev/null 2>&1
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Could not delete directory " $DESTINATION_DIRECTORY
		exit 1 
	fi
	
	mkdir -p $DESTINATION_DIRECTORY > /dev/null 2>&1
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Could not create directory " $DESTINATION_DIRECTORY
		exit 1
	fi
}

update_fw()
{
	# The AMIT update library requires we are in the same directory as the
	# file
	cd $DESTINATION_DIRECTORY > /dev/null 2>&1
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Could not switch to download directory" 
		exit 1
	fi
	
	$FW_UPDATE_COMMAND ./$FILE_NAME > /dev/null 2>&1
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Failed updating FW" 
		exit 1
	fi
	rm -rf ./$FILE_NAME
	if [ "$?" -ne 0 ]; then 
		logger $LOGGER_USER "Could not delete FW update file." 
		exit 1
	fi
}

# Download process:
create_tmp_directory
if [ -z $FILE_NAME ]; then 
	get_latest_fw_version_file_name
fi
retrieve_file
if [ "$FW_APPLY" -eq "yes" ]; then
	update_fw
fi

