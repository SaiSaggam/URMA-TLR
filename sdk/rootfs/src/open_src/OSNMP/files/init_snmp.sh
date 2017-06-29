#!/bin/sh

# NOTE: currently the persistent configuration file is stored in the RAM, so
# its content will be lost on each reboot. If desired to make it really
# persistent, move the persistent directory to the UserFS:
# 1.   modify PERSISTENT_CONFIG_DIR
# 2.a. before starting snmpd, change persistent directory location variable:
#      "SNMP_PERSISTENT_DIR=<dir>"
# 2.b. modify persistent directory path in snmp package build configuration

CONFIG_DIR="/etc/snmp"
CONFIG_FILE="$CONFIG_DIR/snmpd.conf"
PERSISTENT_CONFIG_DIR="/var/snmp"
#PERSISTENT_CONFIG_DIR="/home/digi/snmp"
PERSISTENT_CONFIG_FILE="$PERSISTENT_CONFIG_DIR/snmpd.conf"

SUB_AGENT_TCP_PORT="1705"

PID_FILE="/var/run/snmpd.pid"
DAEMON="/usr/sbin/snmpd"
DAEMON_OPTS="--master=agentx --agentXSocket=tcp:localhost:$SUB_AGENT_TCP_PORT -LS6d -Lf /dev/null -p $PID_FILE"

###############################################################################
# UCI support
###############################################################################

. /lib/functions.sh

# Default Values
default_v1="off"
default_v2c="off"
default_v3="off"
default_auth_traps="off"
default_sys_name="-"
default_sys_contact="-"
default_sys_location="-"

#
# SNMPv1/v2c Community configuration
#
snmp_com2sec_add()
{
	local cfg="$1"

	config_get community $cfg community_clear
	[ -n "$community" ] || return 0

	echo "com2sec $community default $community" >> $CONFIG_FILE
}

snmp_v1v2c_group_add()
{
	local cfg="$1"
	local group="Group"

	config_get community $cfg community_clear
	[ -n "$community" ] || return 0

	[ "$snmp_v1" -eq "1" ] && echo "group $community$group v1 $community" >> $CONFIG_FILE
	[ "$snmp_v2c" -eq "1" ] && echo "group $community$group v2c $community" >> $CONFIG_FILE
}

snmp_v1v2c_access_add()
{
	local cfg="$1"
	local group="Group"

	config_get community $cfg community_clear
	[ -n "$community" ] || return 0
	config_get access $cfg access

	if [ "$access" = "read-write" ]; then
		write="all"
	else
		write="none"
	fi

	[ "$snmp_v1" -eq "1" ] && echo "access $community$group \"\" v1 noauth exact all $write none" >> $CONFIG_FILE
	[ "$snmp_v2c" -eq "1" ] && echo "access $community$group \"\" v2c noauth exact all $write none" >> $CONFIG_FILE
}


#
# SNMPv3 User configuration
#
snmp_createuser_add()
{
	local cfg="$1"

	# Create user in the persistent config, not in the normal config file.
	# When the snmpd starts, these lines will be changed into an encrypted
	# form.

	config_get user $cfg user
	[ -n "$user" ] || return 0
	config_get authentication $cfg authentication
	config_get privacy $cfg privacy
	config_get authentication_password $cfg authentication_password_clear
	config_get privacy_password $cfg privacy_password_clear
	if [ "$authentication" = "none" ]; then
		level="noauth"
		echo "createUser $user" >> $PERSISTENT_CONFIG_FILE
	elif [ "$authentication" = "sha1" -a "$privacy" = "none" ]; then
		level="auth  "
		echo "createUser $user SHA \"$authentication_password\"" >> $PERSISTENT_CONFIG_FILE

	elif [ "$authentication" = "md5" -a "$privacy" = "none" ]; then
		level="auth  "
		echo "createUser $user MD5 \"$authentication_password\"" >> $PERSISTENT_CONFIG_FILE

	elif [ "$authentication" = "sha1" -a "$privacy" = "des" ]; then
		level="priv  "
		echo "createUser $user SHA \"$authentication_password\" DES \"$privacy_password\"" >> $PERSISTENT_CONFIG_FILE

	elif [ "$authentication" = "sha1" -a "$privacy" = "aes" ]; then
		level="priv  "
		echo "createUser $user SHA \"$authentication_password\" AES \"$privacy_password\"" >> $PERSISTENT_CONFIG_FILE

	elif [ "$authentication" = "md5" -a "$privacy" = "des" ]; then
		level="priv  "
		echo "createUser $user MD5 \"$authentication_password\" DES \"$privacy_password\"" >> $PERSISTENT_CONFIG_FILE

	elif [ "$authentication" = "md5" -a "$privacy" = "aes" ]; then
		level="priv  "
		echo "createUser $user MD5 \"$authentication_password\" AES \"$privacy_password\"" >> $PERSISTENT_CONFIG_FILE
	fi
}

snmp_authuser_add()
{
	local cfg="$1"

	config_get user $cfg user
	[ -n "$user" ] || return 0
	config_get access $cfg access
	config_get authentication $cfg authentication $default_authentication
	config_get privacy $cfg privacy

	if [ "$access" = "read-write" ]; then
		user_access="read,write"
	else
		user_access="read      "
	fi

	if [ "$authentication" = "none" ]; then
		level="noauth"
	elif [ "$privacy" = "none" ]; then
		level="auth  "
	else
		level="priv  "
	fi

	echo "authuser $user_access -s usm $user $level" >> $CONFIG_FILE
}

remove_credentials_from_persistent()
{
	# Remove parsed and unparsed credentials
	[ -f "$PERSISTENT_CONFIG_FILE" ] &&
		sed -i '/^usmUser/ d; /^createUser/ d' $PERSISTENT_CONFIG_FILE
}


#
# UCI configuration parsing
#

parse_uci_config()
{
	mkdir -p "$CONFIG_DIR"
	[ -f "$CONFIG_FILE" ] && rm -f "$CONFIG_FILE"

	mkdir -p "$PERSISTENT_CONFIG_DIR"
	# Remove credentials from the persistent config file, to be able to
	# change the passwords
	remove_credentials_from_persistent

	config_load snmp

	config_get_bool snmp_v1 snmp1 v1 $default_v1
	config_get_bool snmp_v2c snmp1 v2c $default_v2c
	config_get_bool snmp_v3 snmp1 v3 $default_v3
	config_get snmp_port snmp1 port
	config_get_bool snmp_auth_traps snmp1 authentication_traps $default_auth_traps

	echo "# Global SNMP configuration" >> $CONFIG_FILE
	echo "agentaddress udp:$snmp_port" >> $CONFIG_FILE
	echo >> $CONFIG_FILE

    config_load snmp_community

	# com2sec configuration
	echo "#       sec.name  source  community" >> $CONFIG_FILE
	if [ "$snmp_v1" -eq "1" -o "$snmp_v2c" -eq "1"  ]; then
		config_foreach snmp_com2sec_add snmp_community
	fi
	echo >> $CONFIG_FILE

	# Group configuration
	echo "#       groupName  securityModel  securityName" >> $CONFIG_FILE
	if [ "$snmp_v1" -eq "1" -o "$snmp_v2c" -eq "1" ]; then
		config_foreach snmp_v1v2c_group_add snmp_community
	fi
	echo >> $CONFIG_FILE

	# Access configuration
	echo "#       group context sec.model sec.level match read write notif" >> $CONFIG_FILE
	if [ "$snmp_v1" -eq "1" -o "$snmp_v2c" -eq "1" ]; then
		config_foreach snmp_v1v2c_access_add snmp_community
	fi
	echo >> $CONFIG_FILE

	# SNMPv3 configuration
	if [ "$snmp_v3" -eq "1" ]; then
        config_load snmp_user
		echo "# SNMPv3 Users" >> $CONFIG_FILE
		config_foreach snmp_createuser_add snmp_user
		echo >> $CONFIG_FILE
		echo "# SNMPv3 User authentication" >> $CONFIG_FILE
		config_foreach snmp_authuser_add snmp_user
		echo >> $CONFIG_FILE
	fi


	echo "view all included  .1  " >> $CONFIG_FILE
	echo >> $CONFIG_FILE

	# Active monitoring configuration
	echo "# Active monitoring configuration" >> $CONFIG_FILE
	if [ "$snmp_auth_traps" -eq "1" ]; then
		echo "authtrapenable 1" >> $CONFIG_FILE
	else
		echo "authtrapenable 2" >> $CONFIG_FILE
	fi
	echo >> $CONFIG_FILE

	#
	# System Configuration
	#
	config_load system

	config_get system_name system1 name "$default_sys_name"
	config_get system_contact system1 contact "$default_sys_contact"
	config_get system_location system1 location "$default_sys_location"

	echo "# System Configuration" >> $CONFIG_FILE
	echo "sysContact $system_contact" >> $CONFIG_FILE
	echo "sysName $system_name" >> $CONFIG_FILE
	echo "sysLocation $system_location" >> $CONFIG_FILE
	echo >> $CONFIG_FILE
}

start_sub_agent_for_digi_mib()
{
	sub_agent -Dsub_agent -x tcp:localhost:$SUB_AGENT_TCP_PORT
}


###############################################################################
# Start and stop functions
###############################################################################
start()
{
	# Parse UCI config
	parse_uci_config

	# Set persistent directory path
	# NOTE uncomment it, if it is not the same path as it's set in build
	# configuration
	#SNMP_PERSISTENT_DIR="$PERSISTENT_CONFIG_DIR"

	# Start package
	$DAEMON $DAEMON_OPTS
	
	start_sub_agent_for_digi_mib
}

stop()
{
	[ -f "$PID_FILE" ] && {
		kill $(cat "$PID_FILE")
		rm -f "$PID_FILE"
	}
	killall sub_agent
}

reload()
{
	if [ -f "$PID_FILE" ]
	then
		parse_uci_config
		kill -s HUP $(cat "$PID_FILE")
	else
		start
	fi
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
		start
		;;
reload)
		reload
		;;
*)
		echo "Usage: $0 {start|stop|restart|reload}"
		exit 1
		;;
esac
exit $?
