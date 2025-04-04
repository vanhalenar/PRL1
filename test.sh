#!/bin/bash

./run.sh $1 > oets.out

nums="$(cat oets.out | head -1)"

echo $nums | python3 sort.py > ref.out