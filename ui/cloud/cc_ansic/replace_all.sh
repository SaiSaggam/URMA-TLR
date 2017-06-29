#!/bin/bash

if [ "$#" -lt "2" ]; then
    echo "Usage: `basename $0` from to [path ...]"
    exit 1
elif [ "$#" -eq "2" ]; then
    dirs=(public private doxygen)
else
    until [ "$#" -eq "2" ]; do
        dirs=("${dirs[@]}"  "$1")
        shift
    done
fi

bad=$1
good=$2

set -f
files=(\( -name *.[ch] -o -name *.rci -o -name *.c_aux -o -name *.py \))
result=`find ${dirs[@]} ${files[@]} -print0 | xargs -0 grep "$bad"`
if [ -z "$result" ]; then
    echo "\"$bad\" not found"
    exit 1
fi
echo "$result"

read -p "Replace \"$bad\" with \"$good\"? (y/n): " -n 1 choice; echo
case "$choice" in
  y|Y ) ;;
  * ) exit 1;;
esac

find "${dirs[@]}" "${files[@]}" -print0 | xargs -0 grep -l $bad | xargs sed -i "s/$bad/$good/g"
