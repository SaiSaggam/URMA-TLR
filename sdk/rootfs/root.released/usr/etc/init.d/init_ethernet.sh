#!/bin/sh
# Initial Ethernet interface

GET_ODM="/usr/sbin/getodm"

set_mac() {
        local interface=$1
        local mac_index=$2
        local ret
        local mac_addr

        mac_addr=$($GET_ODM -m $mac_index 2>/dev/null)
        ret=$?

        if [ "$ret" -ne 0 ]; then
                echo "ERROR: Couldn't read MAC${mac_index}"
                return 1
        fi

        # GET_ODM returns MAC in the following format: "MAC%d: xx:xx:xx:xx:xx:xx"
        mac_addr="${mac_addr#*: }"

        if [ -z "$mac_addr" ]; then
                echo "ERROR: Invalid MAC for $interface"
                return 1
        fi

        ifconfig "$interface" hw ether "$mac_addr"

        return 0
}

# split each port
# and rename them
vconfig add eth0 1
vconfig add eth0 2
vconfig add eth0 3
vconfig add eth0 4
vconfig add eth0 5

ip link set eth0.1 name eth1
ip link set eth0.2 name eth2
ip link set eth0.3 name eth3
ip link set eth0.4 name eth4
ip link set eth0.5 name eth5

# Add for DSL PVC #1
vconfig add eth5 1

ifconfig eth0 up

# set up VLAN
# port control
switch reg w 2004 410003
switch reg w 2104 420003
switch reg w 2204 440003
switch reg w 2304 480003  
switch reg w 2404 500003
switch reg w 2504 000003
switch reg w 2604 20ff0003
# port VLAN control
switch reg w 2010 810000c0
switch reg w 2110 810000c0
switch reg w 2210 810000c0
switch reg w 2310 810000c0
switch reg w 2410 810000c0
switch reg w 2510 810000c0
switch reg w 2610 81000000
# port and protocol based VLAN
switch reg w 2014 10005	#WAN
switch reg w 2114 10001
switch reg w 2214 10002
switch reg w 2314 10003
switch reg w 2414 10004
switch reg w 2514 10006
reg s be110100
reg w 0 2305e33b

#set port mapping
switch vlan set 1 0100001
switch vlan set 2 0010001
switch vlan set 3 0001001
switch vlan set 4 0000101
switch vlan set 5 1000001
switch vlan set 6 0000000
switch vlan set 7 0000000
switch vlan set 8 0000000
switch vlan set 9 0000000
switch vlan set 10 0000000
switch vlan set 11 0000000
switch vlan set 12 0000000
switch vlan set 13 0000000
switch vlan set 14 0000000
switch vlan set 15 0000000
switch vlan set 16 0000000

# Read MAC addresses from ODM
set_mac eth1 0
set_mac eth2 1
set_mac eth3 2
set_mac eth4 3
ifconfig eth5 hw ether 005018005FFE
set_mac eth5.1 4

#Assign IP address
ifconfig eth1 0.0.0.0 up
ifconfig eth2 0.0.0.0 up
ifconfig eth3 0.0.0.0 up
ifconfig eth4 0.0.0.0 up
ifconfig eth5 0.0.0.0 up
ifconfig eth5.1 0.0.0.0 up

mii_mgr -s -p 0 -r 0 -v 0x800
mii_mgr -s -p 1 -r 0 -v 0x800
mii_mgr -s -p 2 -r 0 -v 0x800
mii_mgr -s -p 3 -r 0 -v 0x800
mii_mgr -s -p 4 -r 0 -v 0x800
mii_mgr -s -p 5 -r 0 -v 0x800
mii_mgr -s -p 0 -r 0 -v 0x3300
mii_mgr -s -p 1 -r 0 -v 0x3300
mii_mgr -s -p 2 -r 0 -v 0x3300
mii_mgr -s -p 3 -r 0 -v 0x3300
mii_mgr -s -p 4 -r 0 -v 0x3300
mii_mgr -s -p 5 -r 0 -v 0x3300
ifconfig lo 127.0.0.1 up
mac_addr
