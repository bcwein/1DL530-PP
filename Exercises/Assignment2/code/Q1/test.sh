#!/bin/bash

set +e
rm results.txt

set -eo pipefail

for n in {1,2,4,8,16}; do
    for t in {1,10,1000,10000,1000000,1000000000}; do
    echo "($n,$t)" >> results.txt
    ./question1 -n $n -t $t >> results.txt
    done
done