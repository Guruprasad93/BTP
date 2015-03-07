#! usr/bin/env bash

cd 2D_Plots

a=0;

for i in {0..2999};
do 
a=$((a+1));
gnuplot <<EOF
set term png
fileout = sprintf("/home/guru/BTP/7_March/2D_Images/output%d.png", $a)
filein = sprintf("file%d.txt",$i)
set output fileout
set multiplot
l_matrix = 50
b_matrix = 50
set xrange[-1:l_matrix+1]
set yrange[-1:b_matrix+1]
plot 'simulation_square.txt' u 1:2 w l ls 3 title ""
plot filein u 1:2 w points pointtype 3 title ""
unset multiplot
EOF
done
