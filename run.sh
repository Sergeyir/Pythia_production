#!/bin/bash

name=$1

make ${name} ${1} && ./${name} 2

