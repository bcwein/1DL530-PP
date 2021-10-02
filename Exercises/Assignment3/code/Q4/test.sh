
#!/bin/bash

filename="results_$PROG.txt"

set +e
rm $filename

set -eo pipefail

for n in {1,2,4,8,16}; do
    for u in {3,100,1000,10000}; do
        OMP_NUM_THREADS=$n ./$PROG -u $u >> $filename
    done
done



