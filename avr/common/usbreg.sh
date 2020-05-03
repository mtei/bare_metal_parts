#! /bin/sh
# usage:
#   $ usbreg
#   $ usbreg -l

workfile=/tmp/usbreg_$$_
trap 'cd /tmp/; rm -f ${workfile}1 ${workfile}2; echo; exit' 0 1 2 15

OPT=$1

ioreg $OPT -p IOUSB -w 78 > ${workfile}1;
while true
do
    sleep 0.5
    ioreg $OPT -p IOUSB -w 78 > ${workfile}2;
    if cmp ${workfile}1 ${workfile}2 > /dev/null; then
       :
    else
       echo; echo; date
       echo "================================================"
       diff -u ${workfile}1 ${workfile}2
       mv ${workfile}2 ${workfile}1
    fi
done
