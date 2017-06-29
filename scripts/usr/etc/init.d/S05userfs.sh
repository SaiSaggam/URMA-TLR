#!/bin/sh
#set -x
#Create userfilesystem

MKDIR=/bin/mkdir
CHMOD=/bin/chmod
CHOWN=/bin/chown

# <directory_name> <owner:group> <permissions>
REQUIRED_DIRECTORIES="/home/digi/user root:users ug=rwx,o=rx,g+s
/var/network/uptimes root:root u=rwx,go=rx
/var/digi root:root u=rwx,go=rx
/etc/udhcpc root:root u=rwx,go=rx
"

start() {
    saved_IFS=$IFS
    IFS="
"
    parse() {
        dir_name=$1
        owner=$2
        permission=$3
    }

    for directory in $REQUIRED_DIRECTORIES
    do
        IFS=" "
        parse $directory

        $MKDIR -p $dir_name
        $CHOWN $owner $dir_name
        $CHMOD $permission $dir_name
    done

    IFS=$saved_IFS
}

stop () {
    #empty function seems to result in error, so poke in a dummy operation
    a=1
}

case "${1}" in
start)
	start
	;;
stop)
	stop
	;;
restart|force-reload)
	stop
	start
	;;
*)
	echo "Usage: $0 {start|stop|restart|force-reload}"
	exit 1
	;;
esac
exit $?
