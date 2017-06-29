#!/bin/sh
# Copyright (C) 2006-2014 OpenWrt.org
# Copyright (C) 2006 Fokus Fraunhofer <carsten.tittel@fokus.fraunhofer.de>
# Copyright (C) 2010 Vertical Communications


debug () {
	${DEBUG:-:} "$@"
}

# newline
N="
"

_C=0
NO_EXPORT=1
LOAD_STATE=1
LIST_SEP=" "

UCI_CONFIG_DIR_DEFAULT="/usr/config"
UCI_CONFIG_DIR_MODIFIED="/etc/config"

append() {
	local var="$1"
	local value="$2"
	local sep="${3:- }"

	eval "export ${NO_EXPORT:+-n} -- \"$var=\${$var:+\${$var}\${value:+\$sep}}\$value\""
}

list_contains() {
	local var="$1"
	local str="$2"
	local val

	eval "val=\" \${$var} \""
	[ "${val%% $str *}" != "$val" ]
}

list_remove() {
	local var="$1"
	local remove="$2"
	local val

	eval "val=\" \${$var} \""
	val1="${val%% $remove *}"
	[ "$val1" = "$val" ] && return
	val2="${val##* $remove }"
	[ "$val2" = "$val" ] && return
	val="${val1## } ${val2%% }"
	val="${val%% }"
	eval "export ${NO_EXPORT:+-n} -- \"$var=\$val\""
}

config_load() {
	# First load default config
	CONFIG_DEFAULT_CONF=1
	UCI_CONFIG_DIR="$UCI_CONFIG_DIR_DEFAULT"
	uci_load "$1"
	RET=$?

	# If successfully loaded, create section1 instances from defaults, but
	# don't set any options/lists
	if [ "$RET" -eq 0 -a -n "$CONFIG_DEFAULT_SECTIONS" ]; then
		local section
		for section in ${CONFIG_DEFAULT_SECTIONS}; do
			append CONFIG_SECTIONS "${section%_default}1"
		done
		CONFIG_NUM_SECTIONS="$CONFIG_DEFAULT_NUM_SECTIONS"
	fi

	# Then load overlay config if it exists
	unset CONFIG_DEFAULT_CONF
	UCI_CONFIG_DIR="$UCI_CONFIG_DIR_MODIFIED"

	[ -f "$UCI_CONFIG_DIR/$1" ] || return "$RET"

	uci_load "$1"

	return $?
}

package() {
	return 0
}

config () {
	local cfgtype="$1"
	local name="$2"
	local def_section

	# If this is a default config, change section name to end with
	# '_section', and add to the default list
	[ -n "$CONFIG_DEFAULT_CONF" ] && {
		name="${name%?}_default"
		def_section="_DEFAULT"
	}

	# Add to CONFIG_SECTIONS, if it isn't there already
	list_contains CONFIG${def_section}_SECTIONS "$name" || {
		export ${NO_EXPORT:+-n} CONFIG${def_section}_NUM_SECTIONS=$(($CONFIG${def_section}_NUM_SECTIONS + 1))
		append CONFIG${def_section}_SECTIONS "$name"
	}
	export ${NO_EXPORT:+-n} CONFIG_SECTION="$name"
	export ${NO_EXPORT:+-n} "CONFIG_${CONFIG_SECTION}_TYPE=$cfgtype"
}

option () {
	local varname="$1"; shift
	local value="$*"

	export ${NO_EXPORT:+-n} "CONFIG_${CONFIG_SECTION}_${varname}=$value"
}

list() {
	local varname="$1"; shift
	local value="$*"
	local len
	local list_varname="${CONFIG_SECTION}_${varname}"
	local def_section=${CONFIG_DEFAULT_CONF:+"_DEFAULT"}

	eval "len=\$CONFIG_${CONFIG_SECTION}_${varname}_LENGTH"
	# Append to the default or normal list, if not added yet
	[ ${len:-0} = 0 ] && append CONFIG${def_section}_LIST_STATE "$list_varname"
	len=$(($len + 1))
	config_set "$CONFIG_SECTION" "${varname}_ITEM$len" "$value"
	config_set "$CONFIG_SECTION" "${varname}_LENGTH" "$len"
	append "CONFIG_$list_varname" "$value" "$LIST_SEP"
}

# Converts section name to default section name
get_default_section_name () {
	local var="$1"
	local section="$2"

	eval $var="${section%${section##*[^0-9]}}_default"
}

config_rename() {
	local OLD="$1"
	local NEW="$2"
	local oldvar
	local newvar
	
	[ -n "$OLD" -a -n "$NEW" ] || return
	for oldvar in `set | grep ^CONFIG_${OLD}_ | \
		sed -e 's/\(.*\)=.*$/\1/'` ; do
		newvar="CONFIG_${NEW}_${oldvar##CONFIG_${OLD}_}"
		eval "export ${NO_EXPORT:+-n} \"$newvar=\${$oldvar}\""
		unset "$oldvar"
	done
	export ${NO_EXPORT:+-n} CONFIG_SECTIONS="$(echo " $CONFIG_SECTIONS " | sed -e "s, $OLD , $NEW ,")"
	
	[ "$CONFIG_SECTION" = "$OLD" ] && export ${NO_EXPORT:+-n} CONFIG_SECTION="$NEW"
}

config_unset() {
	config_set "$1" "$2" ""
}

config_clear() {
	local SECTION="$1"
	local oldvar

	list_remove CONFIG_SECTIONS "$SECTION"
	export ${NO_EXPORT:+-n} CONFIG_SECTIONS="${SECTION:+$CONFIG_SECTIONS}"

	for oldvar in `set | grep ^CONFIG_${SECTION:+${SECTION}_} | \
		sed -e 's/\(.*\)=.*$/\1/'` ; do 
		unset $oldvar 
	done
}

# config_get <variable> <section> <option> [<default>]
# config_get <section> <option>
config_get() {
	# If option is not in modified config, get from default config.
	# If it is not in default config, return with the passed <default>
	# argument
	local value
	local def_section
	local var
	case "$3" in
		"")
			eval value="\$CONFIG_${1}_${2}"
			[ -z "$value" ] && {
				get_default_section_name def_section "$1"
				eval value="\$CONFIG_${def_section}_${2}"
			}
			echo "$value"
			;;
		*)
			eval value="\$CONFIG_${2}_${3}"
			[ -z "$value" ] && {
				get_default_section_name def_section "$2"
				eval value="\$CONFIG_${def_section}_${3}"
			}
			export ${NO_EXPORT:+-n} -- "${1}=${value:-${4}}"
			;;
	esac
}

# config_get_bool <variable> <section> <option> [<default>]
config_get_bool() {
	local _tmp
	config_get _tmp "$2" "$3" "$4"
	case "$_tmp" in
		1|on|true|yes|enabled) _tmp=1;;
		0|off|false|no|disabled) _tmp=0;;
		*) _tmp="$4";;
	esac
	export ${NO_EXPORT:+-n} "$1=$_tmp"
}

config_set() {
	local section="$1"
	local option="$2"
	local value="$3"
	local old_section="$CONFIG_SECTION"

	CONFIG_SECTION="$section"
	option "$option" "$value"
	CONFIG_SECTION="$old_section"
}

config_foreach() {
	local ___function="$1"
	[ "$#" -ge 1 ] && shift
	local ___type="$1"
	[ "$#" -ge 1 ] && shift
	local section cfgtype

	[ -z "$CONFIG_SECTIONS" ] && return 0
	for section in ${CONFIG_SECTIONS}; do
		config_get cfgtype "$section" TYPE
		[ -n "$___type" -a "x$cfgtype" != "x$___type" ] && continue
		eval "$___function \"\$section\" \"\$@\""
	done
}

config_list_foreach() {
	[ "$#" -ge 3 ] || return 0
	local section="$1"; shift
	local option="$1"; shift
	local function="$1"; shift
	local val
	local len
	local c=1

	config_get len "${section}" "${option}_LENGTH"
	[ -z "$len" ] && return 0
	while [ $c -le "$len" ]; do
		config_get val "${section}" "${option}_ITEM$c"
		eval "$function \"\$val\" \"\$@\""
		c="$(($c + 1))"
	done
}

include() {
	local file
	
	for file in $(ls $1/*.sh 2>/dev/null); do
		. $file
	done
}

. /lib/config/uci.sh
