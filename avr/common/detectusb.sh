#!/bin/sh
waitstr=("-" "\\" "|" "/")
count=0
workfile=/tmp/detect_$$_
USB=

ls /dev/tty* > ${workfile}1;
if [ `grep 'tty[.]usbmodem' ${workfile}1 | wc -l` = 1 ]; then
    printf "Detecting USB port.  " >&2
    USB=`grep 'tty[.]usbmodem' ${workfile}1`
    printf "= $USB" >&2
else
    printf "Detecting USB port, reset your controller now.  " >&2
    while true; do
	sleep 0.5
	printf ${waitstr[$count]} >&2
	count=`expr $count + 1`; count=`expr $count % 4`
	ls /dev/tty* > ${workfile}2;
	USB=`comm -13 ${workfile}1 ${workfile}2 | grep -o '/dev/tty.*'`
	mv ${workfile}2 ${workfile}1
	if [ -n "$USB" ]; then
	    printf "= $USB" >&2
	    break;
	fi
	# echo $count
    done
fi
rm -f ${workfile}1
echo $USB