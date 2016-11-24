#!/usr/bin/gnuplot
set term png enhanced
set key left top

#set size ratio -1

set xlabel "taille de la matrice N"
set ylabel "temps d'éxécution (s)"
set output "output.png"
plot    "out.6" using 1:2 with lines title "algo 1 proc", \
	"out.5" using 1:2 with lines title "algo 4 proc", \
        "out.4" using 1:2 with lines title "algo mpi 9 proc", \
	"out.3" using 1:2 with lines title "algo mpi 16 proc"
