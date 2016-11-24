
module slurm utilisés: fichier ".module.load"

algorithme de fox:
compilation:    make
lancement  : ./matprod fichier_mat1.txt fichier_mat2.txt


version séquentielle:
compilation: make -B -C sequential clean all
lancement: ./sequential/matprod_seq fichier_mat1.txt fichier_mat2.txt
    (optionnellement avec MKL_NUM_THREADS=1 en préfixe de la ligne
        de commande pour du "vrai" séquentiel)

génération de matrice:
compilation: make -C genmat
lancement: ./genmat/genmat -s TAILLE > fichier_matX.txt

