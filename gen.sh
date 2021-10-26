#!/bin/bash

name=$1

number_of_cores=$2
	
rm output/*.log

make ${name} && for i in $(seq 1 ${number_of_cores})

do

	echo "Doing task ${i}"

	./${name} ${i} &> logs/gp_${name}_${i}.log & disown

done
