#!/bin/sh

print_usage()
{
echo "Usage:"    
echo "------------------"	
echo " usb-control on   "
echo " usb-control off  "
echo "------------------"
}

# main ##########################################################
case "$1" in
	on)
		echo "Power on USB port"
		gpio l 6 0 4000 0 1 4000
		sleep 1
		gpio l 12 0 4000 0 1 4000
		sleep 1
		gpio l 8 0 4000 0 1 4000
		sleep 6  #<----get ready about 6 seconds
		;;
	off)
		echo "Power off USB port"
		gpio l 6 4000 0 1 0 4000
		sleep 1
		gpio l 12 4000 0 1 0 4000
		sleep 1
		gpio l 8 4000 0 1 0 4000
		sleep 1
		;;
	*)
		print_usage
		exit 1
esac

exit 0		
			
		
