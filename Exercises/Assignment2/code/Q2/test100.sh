#!/bin/bash 

set -eo pipefail

result=$(./question2 -n $1 -m 100 | grep -v "time taken:" )
primes="results: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, "

if [[ "$result"  == "$primes" ]]; then 
  echo "Correct" && exit 0
else 
  echo "Incorrect" 
  echo $result 
  exit 1
fi