#! usr/bin/env bash

cd 2D_Plots

for i in {0..3};
do 
gnuplot <<EOF
set term png
fileout = sprintf("/home/guru/BTP/1_March/2D_Images/output%d", $i)
filein = sprintf("file%d.txt",$i)
set output fileout
set multiplot
l_matrix = 5
b_matrix = 5
set xrange[-l_matrix:2*l_matrix]
set yrange[-b_matrix:2*b_matrix]
plot 'simulation_square.txt' u 1:2 w l ls 3 title ""
plot filein u 1:2 w points pointtype 3 title ""
unset multiplot
EOF
done
