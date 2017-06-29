#!/bin/sh

SILABS_VENDOR_ID="10c4"

VENDOR_LIST=$(find  "/sys/devices/platform/xhc_mtk/usb1" -iname "idVendor")

for VENDOR in $VENDOR_LIST; do
	VENDOR_ID=`cat $VENDOR`
	if [ "$VENDOR_ID" == "$SILABS_VENDOR_ID" ]; then
		FW_VERSION_FILE="`dirname $VENDOR`/bcdDevice"
		FW_VERSION=$(cat "$FW_VERSION_FILE")
		if [ -z "$FW_VERSION" ]; then
			echo "ERROR: couldn't read FW version!"
			exit 1
		else
			echo "FW version: $FW_VERSION"
			exit 0
		fi
	fi
done

echo "ERROR: couldn't find CP2108!"

exit 1
