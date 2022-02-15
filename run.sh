#!/bin/bash

name=$1

make ${name} && ./${name} 2 &> log/gp_${name}_${i}.log & disown

