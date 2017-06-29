#!/bin/sh
#
# Script for converting Strongswan UCI configuration to native format
#

CONFIGFILE="/etc/ipsec.conf"
SECRETFILE="/etc/ipsec.secrets"
UPDOWN_SCRIPT="/usr/etc/ipsec.updown"

. /lib/functions.sh

# Default Values
default_local="%any"

# Function calculates number of bit in a netmask
#
mask2cidr() {
	nbits=0
	IFS=.
	for dec in $1 ; do
		case $dec in
		255) let nbits+=8;;
		254) let nbits+=7;;
		252) let nbits+=6;;
		248) let nbits+=5;;
		240) let nbits+=4;;
		224) let nbits+=3;;
		192) let nbits+=2;;
		128) let nbits+=1;;
		0);;
		esac
	done
	echo $(($nbits))
}

dh_map_uci_to_modp() {
	local tmp=""
	local string_to_map="$2"

	for group in $string_to_map; do
		local modp_group
		case $group in
			group5) 	modp_group="modp1536";;
			group14)	modp_group="modp2048";;
			group15)	modp_group="modp3072";;
			group16)	modp_group="modp4096";;
			group17)	modp_group="modp6144";;
			group18)	modp_group="modp8192";;
			none)		modp_group="none";;	#special handling for 'none'
			*)	        modp_group="modp1536";;	#temp debug
		esac
		tmp="$tmp$modp_group "
	done

	export -n -- "$1=${tmp%?}"	# strip trailing space
}

generate_cipher_suites() {
	local tmp=""
	local encryption_algs="$2"
	local integrity_algs="$3"
	local dhgroup_algs="$4"

	for encryption in $encryption_algs; do
		for integrity in $integrity_algs; do
			if [ -n "$dhgroup_algs" ]; then
				for dhgroup in $dhgroup_algs; do
					if [ "$dhgroup" != "none" ]; then
						tmp="$tmp$encryption"-"$integrity"-"$dhgroup",
					else
						tmp="$tmp$encryption"-"$integrity",
					fi
				done
			else
				tmp="$tmp$encryption"-"$integrity",
			fi
		done
	done

	export -n -- "$1=${tmp%?}"	# strip trailing ,
}

ipsec_tun_config() {

	local cfg="$1"

	echo >> $CONFIGFILE
	echo "conn $section" >> $CONFIGFILE
	echo "  auto=add " >> $CONFIGFILE
	echo "  leftupdown=\"$UPDOWN_SCRIPT\"" >> $CONFIGFILE

	#######Write local#######
	config_get local $cfg local $default_local
	if [ -n "$local" ]; then
		echo "  left=$local " >> $CONFIGFILE
	fi
	#######Write peer#######
	config_get peer $cfg peer
	if [ -n "$peer" ]; then
		echo "  right=$peer " >> $CONFIGFILE
	fi

	#######Write remote_network & remote_mask#######
	config_get remote_network $cfg remote_network
	config_get remote_mask $cfg remote_mask
	if [ -n "$remote_network" ] && [ -n "$remote_mask" ]; then
		remote_mask_bits=`mask2cidr $remote_mask`
		echo "  rightsubnet=$remote_network/$remote_mask_bits " >> $CONFIGFILE
	fi

	#######Write local_network & local_mask#######
	config_get local_network $cfg local_network
	config_get local_mask $cfg local_mask
	if [ -n "$local_network" ] && [ -n "$local_mask" ]; then
		local_mask_bits=`mask2cidr $local_mask`
		echo "  leftsubnet=$local_network/$local_mask_bits " >> $CONFIGFILE
	fi

	#######Write phase2 encryption & authentication#######
	config_get esp_authentication $cfg esp_authentication
	config_get esp_encryption $cfg esp_encryption
	config_get esp_diffie_hellman $cfg esp_diffie_hellman
	dh_map_uci_to_modp esp_diffie_hellman "$esp_diffie_hellman"
	generate_cipher_suites cipher_suites "$esp_encryption" "$esp_authentication" "$esp_diffie_hellman"
	echo "  esp=$cipher_suites! " >> $CONFIGFILE

	#######Write authby#######
	config_get auth_by $cfg auth_by
	echo "  authby=$auth_by " >> $CONFIGFILE

	#######Write local_id & remote_id#######
	config_get local_id $cfg local_id
	config_get remote_id $cfg remote_id
	config_get psk_clear $cfg psk_clear
	if [ -n "$local_id" ]; then
		echo "  leftid=$local_id " >> $CONFIGFILE
	fi
	if [ -n "$remote_id" ]; then
		echo "  rightid=$remote_id " >> $CONFIGFILE
	fi
	if [ -n "$local_id" ]; then

		#######Write psk#######
		config_get psk_clear $cfg psk_clear
		if [ -n "$psk_clear" ]; then
			if [ -n "$remote_id" ]; then
				echo " \"$local_id\" \"$remote_id\" : PSK \"$psk_clear\" " >> $SECRETFILE
			else
				echo " \"$local_id\" : PSK \"$psk_clear\" " >> $SECRETFILE
			fi
		fi
	else
		if [ -n "$remote_id" ]; then
			#######Write remote_psk#######
			if [ -n "$psk_clear" ]; then
				echo " \"$remote_id\" : PSK \"$psk_clear\" " >> $SECRETFILE
			fi
		else
			if [ -n "$psk_clear" ]; then
				echo "$peer : PSK \"$psk_clear\" " >> $SECRETFILE
			fi
		fi
	fi

	#######Write ike version#######
	config_get ike $cfg ike
	if [ "$ike" = "1" ]; then
		echo "  keyexchange=ikev1 " >> $CONFIGFILE
	elif [ "$ike" = "2" ]; then
		echo "  keyexchange=ikev2 " >> $CONFIGFILE
        else
		echo "  keyexchange=ike" >> $CONFIGFILE
	fi

	#######Write ike mode#######
	config_get ike_mode $cfg ike_mode
	if [ "$ike_mode" = "main" ]; then
		echo "  aggressive=no " >> $CONFIGFILE
	else
		echo "  aggressive=yes " >> $CONFIGFILE
	fi

	#######Write phase1 encryption & authentication#######
	config_get ike_authentication $cfg ike_authentication
	config_get ike_encryption $cfg ike_encryption
	config_get ike_diffie_hellman $cfg ike_diffie_hellman
	dh_map_uci_to_modp ike_diffie_hellman "$ike_diffie_hellman"
	generate_cipher_suites cipher_suites "$ike_encryption" "$ike_authentication" "$ike_diffie_hellman"
	echo "  ike=$cipher_suites! " >> $CONFIGFILE

	#######Write lifetime#######
	config_get lifetime $cfg lifetime
	echo "  lifetime=$lifetime"s" " >> $CONFIGFILE

	#######Write ike lifetime & tries & bytes#######
	config_get ike_lifetime $cfg ike_lifetime
	echo "  ikelifetime=$ike_lifetime"s" " >> $CONFIGFILE
	config_get ike_tries $cfg ike_tries
	echo "  keyingtries=$ike_tries " >> $CONFIGFILE

	#######Write ike lifebytes#######
	config_get lifebytes $cfg lifebytes
	m_count="`echo "$lifebytes" | grep -c m`"
	if [ "$m_count" = "0" ]; then
		#units is not m
		k_count="`echo "$lifebytes" | grep -c k`"
		if [ "$k_count" = "0" ]; then
			#units is not m and k
			echo "  lifebytes=$lifebytes " >> $CONFIGFILE
		else
			#units is k
			lifebytes_value="`echo "$lifebytes" |  cut -d 'k' -f 1`"
			echo "  lifebytes=$lifebytes_value"000" " >> $CONFIGFILE
		fi
	else
		#units is m
		lifebytes_value="`echo "$lifebytes" |  cut -d 'm' -f 1`"
		echo "  lifebytes=$lifebytes_value"000000" " >> $CONFIGFILE
	fi

	#######Write margin time & bytes#######
	config_get margintime $cfg margintime
	echo "  margintime=$margintime"s" " >> $CONFIGFILE
	config_get marginbytes $cfg marginbytes
	m_count="`echo "$marginbytes" | grep -c m`"
	if [ "$m_count" = "0" ]; then
		#units is not m
		k_count="`echo "$marginbytes" | grep -c k`"
		if [ "$k_count" = "0" ]; then
			#units is not m and k
			echo "  marginbytes=$marginbytes " >> $CONFIGFILE
		else
			#units is k
			marginbytes_value="`echo "$marginbytes" |  cut -d 'k' -f 1`"
			echo "  marginbytes=$marginbytes_value"000" " >> $CONFIGFILE
		fi
	else
		#units is m
		marginbytes_value="`echo "$marginbytes" |  cut -d 'm' -f 1`"
		echo "  marginbytes=$marginbytes_value"000000" " >> $CONFIGFILE
	fi

	#######Write random#######
	config_get random $cfg random
	echo "  rekeyfuzz=$random"%" " >> $CONFIGFILE

	#######Write dpd parameters#######
        config_get dpd $cfg dpd
        if [ "$dpd" = "off" ]; then
	    echo "  dpdaction=none " >> $CONFIGFILE
        else
	    echo "  dpdaction=clear " >> $CONFIGFILE
        fi
	config_get dpddelay $cfg dpddelay
	echo "  dpddelay=$dpddelay"s" " >> $CONFIGFILE
	config_get dpdtimeout $cfg dpdtimeout
	echo "  dpdtimeout=$dpdtimeout"s" " >> $CONFIGFILE

	#######Write tunnel config ending mark#######
	echo >> $CONFIGFILE
	echo "###conn_$section"_"config_end###" >> $CONFIGFILE
}

#
# UCI configuration parsing
#
parse_uci_config()
{
	[ -f "$CONFIGFILE" ] && rm -f "$CONFIGFILE"
	[ -f "$SECRETFILE" ] && rm -f "$SECRETFILE"
	config_load ipsec

	echo "# Global IPsec configuration" >> $CONFIGFILE
	echo "config setup" >> $CONFIGFILE
	echo >> $CONFIGFILE

	# each tunnel config
	config_foreach ipsec_tun_config ipsec
	echo >> $CONFIGFILE
}

###############################################################################
# Start and stop functions
###############################################################################
start()
{
	# Parse UCI config
	parse_uci_config

	# Start package
	ipsec start
}

stop()
{
	ipsec stop
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
	# Reloads the whole configuration. Currently established connections are not effected
	parse_uci_config
	ipsec reload
	# As the user may have changed a PSK, we also need to reload the secrets file
	ipsec rereadsecrets
	;;
update)
	# Determines changes in configuration, and updates it on the running daemon. Currently established connections are not effected
	parse_uci_config
	ipsec update
	# As the user may have changed a PSK, we also need to reload the secrets file
	ipsec rereadsecrets
	;;
parse_uci)
	parse_uci_config
	;;
*)
	echo "Usage: $0 {start|stop|restart|reload|update|parse_uci}"
	exit 1
	;;
esac
exit $?
