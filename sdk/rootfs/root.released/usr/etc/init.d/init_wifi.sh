#!/bin/sh

# MAC0-3 -> Eth1-4
# MAC4   -> DSL
# MAC5-  -> WiFi
WIFI_MAC_OFFSET=5

GET_ODM="/usr/sbin/getodm"

get_mac() {
	local mac_index=$1
	local ret
	local mac_addr

	mac_addr=`$GET_ODM -m $mac_index 2>/dev/null`
	ret=$?

	if [ "$ret" -ne 0 ]; then
		return 1
	fi

	# GET_ODM returns MAC in the following format: "MAC%d: xx:xx:xx:xx:xx:xx"
	# We need something like this: "0x11,0x22,0x33,0x44,0x55,0x66"
	mac_addr=$(echo "$mac_addr" | awk '{gsub(":",",0x")}; {print "0x" $2}')

	if [ -z "$mac_addr" ]; then
		return 1
	fi

	echo "$mac_addr"

	return 0
}

# Check if there's any PCIe WiFi chips
# TODO As we have two chips, we should test them separately, and load the
# proper driver. For this, maybe 'lspci' is better
PCI_DEVICES="/proc/bus/pci/devices"
[ ! -f $PCI_DEVICES -o -z "$(cat $PCI_DEVICES)" ] && {
	echo "No WiFi chip was found"
	exit 0
}

# Disable MT7621 PCIe clock SSC for ch 14 Rx CCK sensitivity with chip MT7603E
reg s be149000;reg w 414 10000060

# Inital Wi-Fi interface
if MAC=`get_mac $WIFI_MAC_OFFSET`; then
	insmod /lib/mt7603e_ap.ko MAC_ADDR=$MAC
else
	echo
	echo "ERROR, cannot get MAC for mt7603"
	echo
fi

# We reserve 4 MACs per WiFi
WIFI_MAC_OFFSET=$((WIFI_MAC_OFFSET + 4))

if MAC=`get_mac $WIFI_MAC_OFFSET`; then
	insmod /lib/rlt_wifi_ap.ko MAC_ADDR=$MAC
else
	echo
	echo "ERROR, cannot get MAC for rlt_wifi"
	echo
fi

# Rename interfaces
# Upping the first interface / driver will create the other interfaces
ifconfig rai0 up && ifconfig rai0 down
ifconfig ra0 up && ifconfig ra0 down

for i in `seq 0 1 7`; do
	j=$((i + 1))
	ip link set rai$i name wifi$j
done

for i in `seq 0 1 7`; do
	j=$((i + 1))
	ip link set ra$i name wifi5g$j
done

#ifconfig wifi1 0.0.0.0 up
#ifconfig wifi5g1 0.0.0.0 up
#ifconfig wifi8 0.0.0.0 up
#ifconfig wifi5g8 0.0.0.0 up

#iwpriv wifi1 set AccessPolicy=0
#iwpriv wifi5g1 set AccessPolicy=0
#iwpriv wifi8 set AccessPolicy=0
#iwpriv wifi5g8 set AccessPolicy=0

#iwpriv wifi1 set MaxStaNum=32
#iwpriv wifi5g1 set MaxStaNum=32
#iwpriv wifi8 set MaxStaNum=32
#iwpriv wifi5g8 set MaxStaNum=32

#Not CE country, disable EDCCA for chip MT7603E.
#iwpriv wifi8 mac 60200618=d7083f0f
