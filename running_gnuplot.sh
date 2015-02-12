#! usr/bin/env bash

gnuplot <<EOF
set multiplot
set xrange[0:100]
set yrange[0:100]
set zrange[0:100]
splot 'Simulationcube.txt' u 1:2:3:(1) w l ls 3
splot 'Initial.txt' u 1:2:3 w points pointtype 3
a = 1
fileout = sprintf("output%d", a)
set output fileout
EOF


