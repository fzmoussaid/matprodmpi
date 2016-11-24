#!/usr/bin/env bash
#SBATCH --job-name=mijieux5
#SBATCH --output=out.5
#SBATCH --error=err.5
#SBATCH -p mistral
#SBATCH --time=02:00:00
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task 1

# 1 noeud / mkl séquentiel / 4 proc MPI

WORKDIR=${WORKDIR:-${HOME}/matprodmpi}

cd ${WORKDIR}
. ./.module.load

do_job() {
    size=$1
    file=mat5_$size.txt
    ./genmat/genmat -b -s $size > $file
    export MKL_NUM_THREADS=1
    export OMP_NUM_THREADS=1
    mpirun -n 4 ./matprod -b -p $file $file
    unset MKL_NUM_THREADS
    rm $file
}

for i in $(seq 100 100 1000); do
    do_job $i
done

do_job 2500
