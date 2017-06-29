#!/bin/sh

#set -x

OVERLAYFS_DIR="/home/digi/overlayfs"
CHECKSUM_FILE="$OVERLAYFS_DIR/.chksum"
KEEPME_FILE="$OVERLAYFS_DIR/.keepme"
FW_CHKSUM_TOOL="/usr/sbin/fwchksum"

mount_overlayfs()
{
	echo "Mounting OverlayFS on root..."

	# Mount overlayfs to /mnt
	mount -o noatime,lowerdir=/,upperdir=$OVERLAYFS_DIR -t overlayfs "overlayfs:/" /mnt

	# Move /proc to overlayfs
	mount -o noatime,move /proc /mnt/proc

	# Change root
	mkdir -p /mnt/rom
	pivot_root /mnt /mnt/rom

	# Move other mountings
	mount -o noatime,move /rom/ram /ram
	mount -o noatime,move /rom/home/digi /home/digi
	mount -o noatime,move /rom/dev /dev
	mount -o noatime,move /rom/sys /sys
}

# Only use overlayfs if overlayfs directory exists
[ ! -d $OVERLAYFS_DIR ] && exit 0

# If there's a file '.keepme', we won't automatically clear it when the RootFS
# checksum gets changed.
# Otherwise we compare the RootFS checksum with the stored checksum. If that
# differs, we remove the overlayfs,
if [ ! -f "$KEEPME_FILE" ]; then
	# Check which firmware are we running
	PARTITION=$(cat /proc/cmdline | sed -r 's/.*root=\/dev\/mtdblock([0-9]*).*/\1/g')
	if [ "$PARTITION" == "3" ]; then
		FW_INDEX=0
	else
		FW_INDEX=1
	fi

	# Read RootFS checksum
	CHKSUM=$($FW_CHKSUM_TOOL $FW_INDEX | grep -i "rootfs")
	[ $? -ne 0 ] && {
		echo "ERROR: Couldn't read FW checksum!"
		exit 1
	}

	CHKSUM=${CHKSUM##*: }

	# If checksum file is not generated yet, do that now
	if [ ! -f "$CHECKSUM_FILE" ]; then
		echo "$CHKSUM" > $CHECKSUM_FILE
		# and we will mount the overlayfs...

	# Else check if checksum is matching. If not, remove overlayfs dir
	elif [ "$(cat $CHECKSUM_FILE)" != "$CHKSUM" ]; then
		echo "FW changed, clearing OverlayFS directory..."

		# Clear overlayfs directory
		rm -Rf "$OVERLAYFS_DIR"

		# Recreate it
		mkdir -p "$OVERLAYFS_DIR"

		# Set new checksum file
		echo "$CHKSUM" > $CHECKSUM_FILE
	fi
fi

# Mount overlayfs
mount_overlayfs
