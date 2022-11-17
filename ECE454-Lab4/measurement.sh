#!/bin/bash

echo "=====================original======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack 1 50 > rt.out
done

echo "=====================1======================"
for i in {1..5}
do
    rm rt.out
    /usr/bin/time ./randtrack_reduction 1 50 > rt.out
done

echo "=====================2======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack_reduction 2 50 > rt.out
done

echo "=====================4======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack_reduction 4 50 > rt.out
done






