#!/bin/bash

#for t in 1000 2000 4000 8000 16000 32000 64000 128000 256000 512000 1024000 2048000 4096000 8000000
for t in 0 1 2 3 4 5 6 7 8 9 10 11 12 13
do
    for s in 8 16 32 64 128 256 512
    do
        g++ -DLOCAL_SIZE=$s -DNMB=$t -o reduction reduction.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
        ./reduction
    done >> reduction.csv
done