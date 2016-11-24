#!/usr/bin/gnuplot
set term png enhanced
set key left top

#set size ratio -1

set xlabel "taille de la matrice N"
set ylabel "temps d'éxécution (s)"
set output "output.png"
plot "out.1" using 1:2 with lines title "algo mpi+mkl 1 noeud ", \
    "out.0" using 1:2 with lines title "algo mpi+mkl 4 noeuds", \
    "out.2" using 1:2 with lines title "algo mpi+mkl 9 noeuds"
