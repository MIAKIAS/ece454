#!/bin/bash

# for value in 0 50 100 200 300 400 500
# do
#     echo comparing rt1_$value.out
#     diff ./testcases_parallel/rt1_$value.out ./testcases_reference/rt1_$value.out
#     echo comparing rt2_$value.out
#     diff ./testcases_parallel/rt2_$value.out ./testcases_reference/rt1_$value.out
#     echo comparing rt4_$value.out
#     diff ./testcases_parallel/rt4_$value.out ./testcases_reference/rt1_$value.out
# done

for value in 0 50 100 200 300 400 500
do
    echo comparing rt1_$value.out
    diff ./testcases_global/rt1_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt2_$value.out
    diff ./testcases_global/rt2_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt4_$value.out
    diff ./testcases_global/rt4_$value.out ./testcases_reference/rt1_$value.out
done

for value in 0 50 100 200 300 400 500
do
    echo comparing rt1_$value.out
    diff ./testcases_list_lock/rt1_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt2_$value.out
    diff ./testcases_list_lock/rt2_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt4_$value.out
    diff ./testcases_list_lock/rt4_$value.out ./testcases_reference/rt1_$value.out
done

for value in 0 50 100 200 300 400 500
do
    echo comparing rt1_$value.out
    diff ./testcase_element_lock/rt1_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt2_$value.out
    diff ./testcase_element_lock/rt2_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt4_$value.out
    diff ./testcase_element_lock/rt4_$value.out ./testcases_reference/rt1_$value.out
done

for value in 0 50 100 200 300 400 500
do
    echo comparing rt1_$value.out
    diff ./testcases_reduction/rt1_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt2_$value.out
    diff ./testcases_reduction/rt2_$value.out ./testcases_reference/rt1_$value.out
    echo comparing rt4_$value.out
    diff ./testcases_reduction/rt4_$value.out ./testcases_reference/rt1_$value.out
done