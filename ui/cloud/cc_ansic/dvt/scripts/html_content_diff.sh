#!/bin/bash

if [ "$#" -ne "2" ]; then
    echo "Usage: `basename $0` from_dir to_dir"
    exit 1
fi

from_dir=$1
to_dir=$2
temp_dir=`mktemp -d`

find "$from_dir" "$to_dir" -name \*.html -print0 | while read -d $'\0' src; do
    dst="$temp_dir/$src"
    dst_dir=$(dirname "$dst")
    mkdir -p "$dst_dir"
    html2text -o "$dst" "$src"
done
cd $temp_dir && diff --recursive --new-file --ignore-all-space "${from_dir#/}" "${to_dir#/}"
rm -rf $temp_dir
