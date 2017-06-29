#!/bin/sh

mount -o noatime -t jffs2 /dev/mtdblock6 /home/digi

# Run a recursive ls on the /home/digi to warm up JFFS2
nohup ls -lR /home/digi/ > /dev/null 2>&1 &

exit 0
