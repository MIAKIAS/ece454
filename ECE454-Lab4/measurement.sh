#!/bin/bash

# echo "=====================original======================"
# for i in {1..5}
# do
#     /usr/bin/time ./randtrack 1 100 > rt.out
# done

echo "=====================1======================"
for i in {1..5}
do
    rm rt.out
    /usr/bin/time ./randtrack_element_lock 1 100 > rt.out
done

echo "=====================2======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack_element_lock 2 100 > rt.out
done

echo "=====================4======================"
for i in {1..5}
do
    /usr/bin/time ./randtrack_element_lock 4 100 > rt.out
done






