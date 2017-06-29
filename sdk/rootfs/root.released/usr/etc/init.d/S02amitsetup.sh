#!/bin/sh

MDEV="/sbin/mdev"
MAKEDEVS="/sbin/makedevs"
MAKEDEVS_CONFIG="/usr/etc/makedevs.d/lr54_devs"

export PATH=$PATH:/usr/bin/scripts
export LD_LIBRARY_PATH=/ram/lib:/lib:/usr/lib/iptables

mount -t sysfs none /sys

while [ ! -f /tmp/csman/pre4.dat ]
do
	cp -a /usr/mv2ram/* /ram
done

# Setup mdev
# Scan /sys for device nodes
${MDEV} -s
# Create the rest via config file
if [ -f "${MAKEDEVS_CONFIG}" ]; then
	${MAKEDEVS} -d "${MAKEDEVS_CONFIG}" / >/dev/null 2>&1
fi
# And create /dev/log for rsyslog
ln -sf /var/log/socket /dev/log

# Turn off all LEDs, except the power LED
gpio l 23 0 4000 0 1 4000
gpio l 25 0 4000 0 1 4000
gpio l 27 0 4000 0 1 4000
gpio l 31 0 4000 0 1 4000

# Set power LED to blinking. The Digi code will make it solid after it finished
# the boot process
gpio l 22 5 5 4000 0 0

mount -t usbfs none /proc/bus/usb
mount -t devpts none /dev/pts

#set eth2 to TC3 
#echo 8 >/proc/irq/22/smp_affinity
#set VOIP to TC2
#echo 4 > /proc/irq/12/smp_affinity

#start network
cat /proc/net/dev
ifconfig lo up

# insert usb module
#modprobe ehci-hcd
#modprobe ohci-hcd

modprobe cp210x

#echo /sbin/hotplug > /sys/kernel/uevent_helper
#gpio l 27 4000 0 1 0 4000 # Cut off 3G USB Power
#gpio l 9 4000 0 1 0 4000 # Cut off 3G Module Power

#gpio l 28 4000 0 1 0 4000 # orange led on

#insmod HW CRYPTO IPSEC
/sbin/insmod  /lib/crypto_k.ko

#bind CPU for ipsec
echo 8 > /proc/irq/19/smp_affinity
echo 2 > /proc/irq/3/smp_affinity  
echo 5 > /sys/class/net/eth0/queues/rx-0/rps_cpus
#echo 5 > /sys/class/net/eth3/queues/rx-0/rps_cpus

#bind irq22 to CPU 3; result: cat /proc/interrupts
echo 8 > /proc/irq/22/smp_affinity

# insert modules for iptables
#modprobe xt_time

# for 3G modem
mkdir -p /var/dev
mkdir -p /var/lock
mkdir -p /var/config
#vsftpd need this for secure_chroot_dir
mkdir -p /var/share/empty
#smbd
mkdir -p /var/locks
#jffs2 for AP controller proposal
mkdir -p /var/storage
mkdir -p /tmp/jffs2
mkdir -p /tmp/jffs2/apmgt

getpsec -f 0xFF60000 /tmp/csman/pre1.dat

#if [ -e /usr/bin/mysql_mips.tar.gz ] ; then

#cd /tmp
#tar zxvf /usr/bin/mysql_mips.tar.gz
#cd /tmp
#tar zxvf /usr/bin/radius.tar.gz
#tar zxvf /usr/bin/coova.tar.gz
#fi

# General pin setup
reg s be000000; reg w 60 0xa85a8

# Synchronize system time with RTC
i2c_rtc g

# Initial Ethernet interface
/usr/etc/init.d/init_ethernet.sh

# Initial Wi-Fi interface
/usr/etc/init.d/init_wifi.sh

# Initial JFFS2 Flash File System
/usr/etc/init.d/init_jffs2.sh

# Init sysctl
[ -f /usr/etc/sysctl.conf ] && sysctl -p /usr/etc/sysctl.conf >/dev/null

# Enable HW NAT
#modprobe hw_nat

# Power on 3G interface
gpio l 6 0 4000 0 1 4000 

# Power on ADSL interface
gpio l 7 0 4000 0 1 4000 

## Turn on Power LED
#gpio l 22 4000 0 1 0 4000
