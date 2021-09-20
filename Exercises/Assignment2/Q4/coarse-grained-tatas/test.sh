#!/bin/bash

set +e
rm results.txt

set -eo pipefail

for n in {1,2,4,8,16}; do
    echo "$n" >> results.txt
    ./benchmark_example $n >> results.txt
    done
done