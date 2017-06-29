#!/bin/sh
# Initial ADSL interface

reg s be000000;reg w 60 0xa85a8
i2c_rtc i;i2c_rtc g
reg s be149000;reg w 414 10000060

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

vconfig add eth2 1
vconfig add eth2 2
vconfig add eth2 3
vconfig add eth2 4
vconfig add eth2 5
   
ifconfig eth2.1 hw ether 005018001FFE
ifconfig eth2.2 hw ether 005018002FFE
ifconfig eth2.3 hw ether 005018003FFE
ifconfig eth2.4 hw ether 005018004FFE
ifconfig eth2.5 hw ether 005018005FFE

ifconfig eth2.1 up
ifconfig eth2.2 up
ifconfig eth2.3 up
ifconfig eth2.4 up
ifconfig eth2.5 up

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
ifconfig eth2.5 0.0.0.0 up

brctl addif br0 eth2.1
ifconfig br0 up
brctl addif br1 eth2.2
ifconfig br1 up
brctl addif br2 eth2.3
ifconfig br2 up
brctl addif br3 eth2.4
ifconfig br3 up

ifconfig lo 127.0.0.1 up
mac_addr

# set PVC0
ci_cmd "wan ghs set std adsl2plus_multi"
ci_cmd "wan ghs set annex aijlm"
ci_cmd "wan atm vc service f"
ci_cmd "wan atm set 0 33 vcmux 1"
ci_cmd "wan atm setqos ubr 0 0 0 1"

# set LAN, Static WAN IP
ifconfig br0 192.168.1.254 netmask 255.255.255.0
ifconfig br1 192.168.2.254 netmask 255.255.255.0
ifconfig br2 192.168.3.254 netmask 255.255.255.0
ifconfig br3 192.168.4.254 netmask 255.255.255.0
ifconfig eth2.5 192.168.122.166 netmask 255.255.255.0

# set default route, MASQUERADE and forward
route add default gw 192.168.122.230
iptables -t nat -A POSTROUTING -s 192.168.1.0/24 -o eth2.5 -j MASQUERADE
iptables -t nat -A POSTROUTING -s 192.168.2.0/24 -o eth2.5 -j MASQUERADE
iptables -t nat -A POSTROUTING -s 192.168.3.0/24 -o eth2.5 -j MASQUERADE
iptables -t nat -A POSTROUTING -s 192.168.4.0/24 -o eth2.5 -j MASQUERADE
echo 1 > /proc/sys/net/ipv4/conf/all/forwarding
