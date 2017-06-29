#!/bin/sh
# Initial Ethernet interface

reg s be000000;reg w 60 0xa85a8
i2c_rtc i;i2c_rtc g;
reg s be149000;reg w 414 10000060

# set up VLAN
switch reg w 2004 ff0003
switch reg w 2104 ff0003
switch reg w 2204 ff0003
switch reg w 2304 ff0003  
switch reg w 2404 ff0003
switch reg w 2504 ff0003
switch reg w 2010 810000c0
switch reg w 2110 810000c0
switch reg w 2210 810000c0
switch reg w 2310 810000c0
switch reg w 2410 810000c0
switch reg w 2510 810000c0
switch reg w 2610 81000000
switch reg w 2604 20ff0003
switch reg w 2014 10002
switch reg w 2114 10001
switch reg w 2214 10001
switch reg w 2314 10001
switch reg w 2414 10001
switch reg w 2514 10001
reg s be110100
reg w 0 2305e33b

#set port mapping
switch vlan set 1 0111101
switch vlan set 2 1000001
switch vlan set 3 0000000
switch vlan set 4 0000000
switch vlan set 5 0000000
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

vconfig add eth2 1
vconfig add eth2 2     
vconfig add eth2 3
vconfig add eth2 4  
   
vconfig add eth2.1 1
vconfig add eth2.1 2
vconfig add eth2.1 3
vconfig add eth2.1 4
vconfig add eth2.2 1
vconfig add eth2.2 2
vconfig add eth2.2 3
vconfig add eth2.2 4
vconfig add eth2.3 1
vconfig add eth2.3 2
vconfig add eth2.3 3
vconfig add eth2.3 4
vconfig add eth2.4 1
vconfig add eth2.4 2
vconfig add eth2.4 3
vconfig add eth2.4 4

ifconfig eth2.1 hw ether 005018000FFE
ifconfig eth2.2 hw ether 005018000FFE
ifconfig eth2.3 hw ether 005018000FFE
ifconfig eth2.4 hw ether 005018000FFE

ifconfig eth2.1 up
fconfig eth2.2 up
ifconfig eth2.3 up
ifconfig eth2.4 up

brctl addbr br0
brctl stp br0 off
brctl sethello br0 0
brctl setfd br0 0
brctl setageing br0 60

brctl addbr br1
brctl stp br1 off
brctl sethello br1 0
brctl setfd br1 0
brctl setageing br1 60

brctl addbr br2
brctl stp br2 off
brctl sethello br2 0
brctl setfd br2 0
brctl setageing br2 60

brctl addbr br3
brctl stp br3 off
brctl sethello br3 0
brctl setfd br3 0
brctl setageing br3 60

ifconfig eth2.1 0.0.0.0 up
ifconfig eth2.2 0.0.0.0 up
ifconfig eth2.3 0.0.0.0 up
ifconfig eth2.4 0.0.0.0 up

brctl addif br0 eth2.1
ifconfig br0 up
brctl addif br1 eth2.2
ifconfig br1 up
brctl addif br2 eth2.3
ifconfig br2 up
brctl addif br3 eth2.4
ifconfig br3 up

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

#set IP
ifconfig br0 192.168.1.254
ifconfig br1 192.168.2.254
ifconfig br2 192.168.3.254
ifconfig br3 192.168.4.254

# turn on power led
gpio l 22 4000 0 1 0 4000


