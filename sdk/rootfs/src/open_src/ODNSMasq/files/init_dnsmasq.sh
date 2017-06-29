#!/bin/sh

CONFIG_DIR="/var/dns"
CONFIGFILE="$CONFIG_DIR/dnsmasq.conf"
OPTIONFILE="$CONFIG_DIR/options.conf"
HOSTFILE="$CONFIG_DIR/hosts.conf"
LEASEFILE="$CONFIG_DIR/dnsmasq.leases"
DAEMON="/usr/sbin/dnsmasq"
ARGS="-C $CONFIGFILE"
PIDFILE="/var/run/dnsmasq.pid"

. /lib/functions.sh

default_state="off"
default_ip_address_start=""
default_ip_address_end=""
default_mask=""
default_gateway=""
default_dns1=""
default_dns2=""
default_lease_time="1440"
default_time_unit="m"
default_relay_interface=""
default_relay_server_ip=""
default_host_mac=""
default_host_ip=""
default_option_server="0"
default_option_option="0"
default_option_value=""

INTERFACES=" \
eth1 \
eth2 \
eth3 \
eth4 \
dsl \
cellular1 \
cellular2 \
wifi1 \
wifi2 \
lan1 \
lan2 \
lan3 \
lan4 \
lan5 \
lan6 \
lan7 \
lan8 \
lan9 \
lan10 \
"

dhcp_add()
{
        local cfg="$1"

        config_get_bool state $cfg state
        if [ "$state" -eq "1" ]; then
                config_get ip_address_start $cfg ip_address_start $default_ip_address_start
                config_get ip_address_end $cfg ip_address_end $default_ip_address_end
                config_get mask $cfg mask $default_mask
                config_get gateway $cfg gateway $default_gateway
                config_get dns1 $cfg dns1 $default_dns1
                config_get dns2 $cfg dns2 $default_dns2
                config_get lease_time $cfg lease_time $default_lease_time
        else
                return 0
        fi

        section=${section//_/-}
        if [ -n "$ip_address_start" -a -n "$ip_address_end" ]; then
            if [ -n "$section" ]; then
                    echo "dhcp-range=set:$section,$ip_address_start,$ip_address_end,$mask,$lease_time$default_time_unit" >> $CONFIGFILE
                    if [ -n "$dns1" -o -n "$dns2" ]; then
                            echo "tag:$section,6${dns1:+,$dns1}${dns2:+,$dns2}" >> $OPTIONFILE
                    fi
                    if [ -n "$gateway" ]; then
                            echo "tag:$section,3,$gateway" >> $OPTIONFILE
                    fi
            else
                    echo "dhcp-range=$ip_address_start,$ip_address_end,$mask,$lease_time$default_time_unit" >> $CONFIGFILE
                    if [ -n "$dns1" -o -n "$dns2" ]; then
                            echo "6${dns1:+,$dns1}${dns2:+,$dns2}" >> $OPTIONFILE
                    fi
                    if [ -n "$gateway" ]; then
                            echo "3,$gateway" >> $OPTIONFILE
                    fi
            fi
        fi
}

dhcp_option_add()
{
        local cfg="$1"

        config_get server $cfg server $default_option_server
        config_get option $cfg option $default_option_option
        config_get value $cfg value $default_option_value

        if [ "$server" -gt 0 ]; then
                if [ -n "$option" ] && [ -n "$value" ]; then
                        echo "tag:dhcp$server,$option,$value" >> $OPTIONFILE
                fi
        else
                echo "$option,$value" >> $OPTIONFILE
        fi
}

dhcp_relay_add()
{
        local cfg="$1"

        config_get interface $cfg interface $default_relay_interface
        config_get server_ip_address $cfg server_ip_address $default_relay_server_ip
		if [ -n "$interface" ]; then
			relay_ip=$(ifconfig $interface | grep "inet addr:" | awk '{split($2,a,":")};{print a[2]}')
		fi
		if [ -n "$relay_ip" ] && [ -n "$server_ip_address" ]; then
			echo "dhcp-relay=$relay_ip,$server_ip_address" >> $CONFIGFILE
		fi
}

dhcp_host_add()
{
        local cfg="$1"

        config_get mac_address $cfg mac_address $default_host_mac
        config_get ip_address $cfg ip_address $default_host_ip

        if [ -n "$mac_address" ] && [ -n "$ip_address" ]; then
                echo "$mac_address,$ip_address" >> $HOSTFILE
        fi
}

add_static_content()
{
	for i in $INTERFACES; do
		echo "resolv-file-target=$CONFIG_DIR/$i/resolv.conf" >> "$CONFIGFILE"
	done

	cat >> "$CONFIGFILE" <<END_STATIC
resolv-file=$CONFIG_DIR/resolv.conf

dhcp-hostsfile=$HOSTFILE
dhcp-optsfile=$OPTIONFILE
dhcp-leasefile=$LEASEFILE

dhcp-authoritative
dhcp-sequential-ip

END_STATIC
}

parse_uci_config()
{
	config_load dhcp_server

	add_static_content

	config_foreach dhcp_add dhcp_server
	config_foreach dhcp_option_add dhcp_option
	config_foreach dhcp_relay_add dhcp_relay
	config_foreach dhcp_host_add dhcp_host
}

###############################################################################
# Start and stop functions
###############################################################################
start()
{
	mkdir -p "$CONFIG_DIR"

	[ -f "$CONFIGFILE" ] && rm -f "$CONFIGFILE"
	[ -f "$OPTIONFILE" ] && rm -f "$OPTIONFILE"
	[ -f "$HOSTFILE" ] && rm -f "$HOSTFILE"

	# Create resolv.conf directories, inotify will watch these
	for i in $INTERFACES; do
		mkdir -p "$CONFIG_DIR/$i"
	done

	# Parse UCI config
	parse_uci_config

	# Start package
	start-stop-daemon --start --quiet --pidfile $PIDFILE --exec $DAEMON -- $ARGS
}

stop()
{
	# Try to stop dnsmasq
	start-stop-daemon --stop --quiet --signal TERM --pidfile $PIDFILE >/dev/null 2>&1
}

###############################################################################
# Start of script
###############################################################################

case "${1}" in
start)
		start
		;;
stop)
		stop
		;;
restart)
		stop

		# Wait for dnsmasq to stop
		for i in `seq 0 1 5`; do
			start-stop-daemon --stop --quiet --pidfile $PIDFILE --test >/dev/null 2>&1
			RET=$?

			[ $RET -ne 0 ] && break

			usleep 200000
		done

		# If not stopped, kill it
		if [ $RET -eq 0 ]; then
			start-stop-daemon --stop --quiet --signal KILL --pidfile $PIDFILE
		fi

		start
		;;
*)
		echo "Usage: $0 {start|stop|restart}"
		exit 1
		;;
esac
exit $?
