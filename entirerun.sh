#! usr/bin/env bash

g++ PopulateMatrix_gnuplot.cpp -std=c++11
./a.out
cd Plots
bash running_gnuplot.sh
