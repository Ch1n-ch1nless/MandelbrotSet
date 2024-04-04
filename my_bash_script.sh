#!/bin/bash

for i in 1 2 3 4 5 
do
    for j in -O0 -O1 -O2 -O3
    do
        touch "time_files/simple_${i}_${j}.txt"
        touch "time_files/vector_${i}_${j}.txt"
        touch "time_files/avx_${i}_${j}.txt"
        make clean
        make OPT_LEVEL=$j
        ./mandelbrot_set -t -i simple > numbers.txt
        /bin/python3 /home/oleg/Documents/MyAsmTasks/MandelbrotSet/calculate.py > time_files/simple_${i}_${j}.txt
        echo "I calculate simple"
        ./mandelbrot_set -t -i vector > numbers.txt
        /bin/python3 /home/oleg/Documents/MyAsmTasks/MandelbrotSet/calculate.py > time_files/vector_${i}_${j}.txt
        echo "I calculate vector"
        ./mandelbrot_set -t -i avx > numbers.txt
        /bin/python3 /home/oleg/Documents/MyAsmTasks/MandelbrotSet/calculate.py > time_files/avx_${i}_${j}.txt
        echo "I calculate avx"
    done
done