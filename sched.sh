#!/bin/bash
num_cpu=$(grep -c ^processor /proc/cpuinfo)
echo $num_cpu

for ((cpu=0; cpu < $num_cpu; cpu++))
do 
    echo "starting on core mask $((2**$cpu))"
    taskset $((2**$cpu)) ./gettime &
done
