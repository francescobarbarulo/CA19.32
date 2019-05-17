#!/bin/bash

CMD=$1

./pin -t source/tools/SimpleExamples/obj-intel64/pinatrace.so -- $CMD
cat pinatrace.out | awk --non-decimal-data '{printf "R 0x%x\r\n", $1 % 0xffff}' | tail -n +4 | head -n -1 > ../examples/$CMD.i.txt
cat pinatrace.out | awk --non-decimal-data '{printf "%s 0x%x\r\n", $2, $3 % 0xffff}' | tail -n +4 | head -n -1 > ../examples/$CMD.d.txt
