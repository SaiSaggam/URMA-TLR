#!/bin/sh
# Script fetches the % complete of firmware download from wget log file.
# It greps the integer value before % in the last but one line of the 
# given log file.

tail -n2 "$1" | head -n1 | sed 's/.*[ \t][ \t]*\([0-9][0-9]*\)%.*/\1/'
