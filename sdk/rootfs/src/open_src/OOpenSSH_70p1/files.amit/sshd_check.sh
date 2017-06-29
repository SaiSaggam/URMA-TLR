#!/bin/bash
#
# Script for converting SNMP UCI configuration to native format
#

export UCI_CONFIG_DIR="/etc/ssh"

CONFIGFILE="/etc/ssh/sshd_conf"
PASSWD="/etc/ssh/passwd"

. /lib/functions.sh
. /lib/config/uci.sh
#Austin chmod +x pw_hash

# Default Values
default_port="22"
default_pam="yes"
default_access="ro"
SSH_ON="0"

KEY_DIR="/etc/ssh"
RSA1_KEY="${KEY_DIR}/ssh_host_key"
RSA_KEY="${KEY_DIR}/ssh_host_rsa_key"
DSA_KEY="${KEY_DIR}/ssh_host_dsa_key"
ED25519_KEY="${KEY_DIR}/ssh_host_ed25519_key"

# SNMPv3 User configuration
#
ssh_user_check() {
#root:$1$$N76hdwGfg11g0KdKbtyh21:0:0:root:/root:/bin/ash
        local cfg="$1"
        config_get name $cfg name
        [ -n "$name" ] || return 0
		config_get password $cfg password
        config_get access $cfg access $default_access
        if [ "$access" = "none" ]; then
                echo "#$name $user $password $access" >> $PASSWD
        elif [ "$access" = "super" ]; then
                ./pwtohash $password > hash
                HASH=`cat hash`
                # Austin echo "$name:$password:0:0:root:/root:/bin/ash" >> $PASSWD
				if [ "`grep -wc \"$name:$HASH:0:0:root:/root:/bin/ash\" $PASSWD`" -eq 1 ]; then
					echo "SSHD user configuration checking $name... passed"
				else
					echo "SSHD user configuration checking $name... failed"
				fi
        elif [ "$access" = "rw" ]; then
                if [ "`grep -wc \"$name:$password:0:0:root:/root:/bin/ash\" $PASSWD`" -eq 1 ]; then
					echo "SSHD user configuration checking $name... passed"
				else
					echo "SSHD user configuration checking $name... failed"
				fi
        elif [ "$access" = "ro" ]; then
                if [ "`grep -wc \"$name:$password:0:0:root:/root:/bin/ash\" $PASSWD`" -eq 1 ]; then
					echo "SSHD user configuration checking $name... passed"
				else
					echo "SSHD user configuration checking $name... failed"
				fi
        else
                if [ "`grep -wc \"$name:$password:0:0:root:/root:/bin/ash\" $PASSWD`" -eq 1 ]; then
					echo "SSHD user configuration checking $name... passed"
				else
					echo "SSHD user configuration checking $name... failed"
				fi
        fi
}

ssh_check() {
#root:$1$$N76hdwGfg11g0KdKbtyh21:0:0:root:/root:/bin/ash

        local cfg="$1"
        config_get_bool ssh_on $cfg server
		
		if [ "$ssh_on" -eq "1" ]; then
			SSH_ON="1"
			config_get ssh_port $cfg port
			if [ "`grep -wc \"Port $ssh_port\" $CONFIGFILE`" -eq 1 ]; then
				echo "SSHD configuration sshd port checking... passed"
			else
				echo "SSHD configuration sshd port checking... failed"
			fi
		else
			return 0
		fi
}


#
# Starts here
#

config_load ssh

#Read SSH Configuration
config_foreach ssh_check ssh

echo >> $CONFIGFILE

if [ "$SSH_ON" -eq "1" ]; then
# Check User
	config_foreach ssh_user_check user
fi