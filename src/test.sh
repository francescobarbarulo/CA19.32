#!/bin/bash

for MODE_TYPE in D F; do
    # ./sim R $MODE_TYPE
    echo "--sim R $MODE_TYPE"
done

for MODE_TYPE in D F; do
    for RANDOM_ACCESS_PROB in {0..100..20}; do
        for READ_REQ_PROB in {0..100..20}; do
            for PRNG_SEED in {0..9..1}; do
                echo -n "sim S $MODE_TYPE $RANDOM_ACCESS_PROB $READ_REQ_PROB $PRNG_SEED " >> output.txt
                ./sim S $MODE_TYPE $RANDOM_ACCESS_PROB $READ_REQ_PROB $PRNG_SEED >> output.txt
            done
        done
    done
done
