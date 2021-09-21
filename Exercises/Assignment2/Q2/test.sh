
#!/bin/bash

set +e
rm results.txt

set -eo pipefail

for n in {1,2,4,8,16}; do
    for m in {10,100,1000000,10000000}; do
        echo "($n,$m)" >> results.txt
        ./question2 -n $n -m $m >> results.txt
    done
done



