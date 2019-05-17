#!/bin/bash

cd ..

if [ -z $1 ]; then
    LIST=$(ls examples | grep txt)
else
    LIST=$1
fi

for PROG in $LIST; do
    for MODE_TYPE in D F; do
        (TICKS=$(./sim R $MODE_TYPE $PROG 2> /dev/null) && echo -e "$PROG\t$MODE_TYPE\t$TICKS" >> test/output.real.txt) &
    done
done

