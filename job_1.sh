#!/usr/bin/env bash
#SBATCH --job-name=mijieux1
#SBATCH --output=out.1
#SBATCH --error=err.1
#SBATCH -p mistral
#SBATCH --exclusive
#SBATCH --time=02:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task 10

# 1 noeud / 1 proc MPI / mkl parallele 10/20 threads

WORKDIR=${WORKDIR:-${HOME}/matprodmpi}

cd ${WORKDIR}
. ./.module.load
mpd &

do_job() {
    size=$1
    file=mat1_$size.txt
    ./genmat/genmat -b -s $size > $file
    mpirun -n 1 ./matprod -b -p $file $file
    rm $file
}

for i in $(seq 100 100 1000); do
    do_job $i
done

do_job 2500

for i in $(seq 5000 5000 20000); do
    do_job $i
done
