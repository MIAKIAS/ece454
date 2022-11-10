#!/bin/bash

for value in  0 50 100 200 300 400 500
do
    echo generating rt1_$value.out...
    ./randtrack_reduction 1 $value > rt1_$value.out && sort -n rt1_$value.out > rt1_$value.outs && rm rt1_$value.out && mv rt1_$value.outs testcases_reduction/rt1_$value.out 
done

for value in  0 50 100 200 300 400 500
do
    echo generating rt2_$value.out...
    ./randtrack_reduction 2 $value > rt1_$value.out && sort -n rt1_$value.out > rt1_$value.outs && rm rt1_$value.out && mv rt1_$value.outs testcases_reduction/rt2_$value.out 
done

for value in  0 50 100 200 300 400 500
do
    echo generating rt4_$value.out...
    ./randtrack_reduction 4 $value > rt1_$value.out && sort -n rt1_$value.out > rt1_$value.outs && rm rt1_$value.out && mv rt1_$value.outs testcases_reduction/rt4_$value.out 
done