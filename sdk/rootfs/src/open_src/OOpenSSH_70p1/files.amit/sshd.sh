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

default_value_conifg()
{
	echo "AddressFamily any" >> $CONFIGFILE
	echo "ListenAddress 0.0.0.0" >> $CONFIGFILE
        echo "ListenAddress ::" >> $CONFIGFILE
        echo "Protocol 2" >> $CONFIGFILE
        echo "UsePAM yes" >> $CONFIGFILE
        echo "HostKey $RSA_KEY" >> $CONFIGFILE
        echo "HostKey $DSA_KEY" >> $CONFIGFILE
        echo "HostKey $ED25519_KEY" >> $CONFIGFILE
        echo "LoginGraceTime 2m" >> $CONFIGFILE
        echo "PermitRootLogin yes" >> $CONFIGFILE
        echo "MaxAuthTries 6" >> $CONFIGFILE
        echo "MaxSessions 10" >> $CONFIGFILE
        echo "PasswordAuthentication yes" >> $CONFIGFILE
        echo "UsePrivilegeSeparation no" >> $CONFIGFILE
        #echo "UsePrivilegeSeparation sandbox" >> $CONFIGFILE
        #echo "Banner /usr/etc/ssh/custom_banner" >> $CONFIGFILE
        #echo "Subsystem sftp /usr/libexec/sftp-server" >> $CONFIGFILE
        #echo "AllowUsers root" >> $CONFIGFILE
}


# SNMPv3 User configuration
#
ssh_user_add() {
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
                echo "$name:$HASH:0:0:root:/root:/bin/ash" >> $PASSWD
                # echo "$name:$password:0:0:root:/root:/bin/ash" >> $PASSWD
        elif [ "$access" = "rw" ]; then
                level="auth  "
                #echo "createUser $user $authentication $authentication_password" >> $PASSWD
				echo "$name:$password:0:0:root:/root:/bin/ash" >> $PASSWD
        elif [ "$access" = "ro" ]; then
                level="auth  "
                #echo "createUser $user $authentication $authentication_password" >> $PASSWD
				echo "$name:$password:0:0:root:/root:/bin/ash" >> $PASSWD
        else
                level="auth  "
                #echo "createUser $user $authentication $authentication_password" >> $PASSWD
				echo "$name:$password:0:0:root:/root:/bin/ash" >> $PASSWD
        fi
}

ssh_add() {
#root:$1$$N76hdwGfg11g0KdKbtyh21:0:0:root:/root:/bin/ash

        local cfg="$1"
        config_get_bool ssh_on $cfg server
		
		if [ "$ssh_on" -eq "1" ]; then
			SSH_ON="1"
			config_get ssh_port $cfg port
			echo "Port $ssh_port" >> $CONFIGFILE
		else
			return 0
		fi
}


#
# Starts here
#

[ -f "$CONFIGFILE" ] && rm -f "$CONFIGFILE"
[ -f "$PASSWD" ] && rm -f "$PASSWD"
config_load ssh

#Read SSH Configuration
config_foreach ssh_add ssh

echo >> $CONFIGFILE

if [ "$SSH_ON" -eq "1" ]; then
#Add Default Values
	default_value_conifg
# Add User
	config_foreach ssh_user_add user
	#Dafault root
	echo "root:$1$$N76hdwGfg11g0KdKbtyh21:0:0:root:/root:/bin/ash" >> $PASSWD
	echo "sshd:*:115:115:sshd:/var/empty:/bin/false" >> $PASSWD
	cp $PASSWD /etc/passwd
fi
