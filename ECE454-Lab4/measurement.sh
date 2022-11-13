#!/bin/bash

echo "=====================1======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack_reduction 1 100 > rt.out
done

echo "=====================2======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack_reduction 2 100 > rt.out
done
echo "=====================4======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack_reduction 4 100 > rt.out
done