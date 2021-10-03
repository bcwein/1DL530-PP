
#!/bin/bash

set +e
rm results.txt

set -eo pipefail

for t in {1,2,4,8,16}; do
    for d in {200,400,800,1000}; do
        echo "($t,$d)" >> results.txt
        ./matmul $d $t >> results.txt
    done
done
