#!/bin/bash

MAX_ARR_SIZE=1024*1024*32
for ((i = 1024; i <= MAX_ARR_SIZE; i *= 2))
do
	g++ -DARRAY_SIZE=$i project4.cpp -o prog -lm -fopenmp
	./prog
done