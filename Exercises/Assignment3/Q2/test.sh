#!/bin/bash

set +e
rm results.txt

set -eo pipefail

for t in {1,2,4,8,16}; do
    for w in {64, 1024, 4096}; do
        for s in {1000, 2000}; do
            echo "($t,$w, $s)" >> results.txt
            ./Game_Of_Life $w $s $t >> results.txt
        done
    done
done
