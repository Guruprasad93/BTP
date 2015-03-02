#! usr/bin/env bash

g++ 2D_SimulationMatrix.cpp -std=c++11
./a.out
#cd Plots
bash 2D_gnuplot.sh
cd 2D_Images
ffmpeg -start_number 1 -i output%d.png -vcodec mpeg4 test.avi
