#!/bin/bash
gcc -Wall -g -ansi  -c coding_exercise.c -o coding_exercise.o
gcc -o coding_exercise coding_exercise.o
./coding_exercise $1 $2
